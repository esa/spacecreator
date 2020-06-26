/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "creatortool.h"

#include "aadlcommentgraphicsitem.h"
#include "aadlconnectiongraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlfunctiontypegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "baseitems/common/aadlutils.h"
#include "commands/cmdcommentitemcreate.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/cmdfunctiontypeitemcreate.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "commandsstack.h"
#include "connectioncreationvalidator.h"
#include "context/action/actionsmanager.h"
#include "graphicsitemhelpers.h"
#include "graphicsviewutils.h"
#include "ui/grippointshandler.h"

#include <QAction>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <QPointer>
#include <QSet>
#include <QVector>
#include <QtDebug>
#include <QtMath>
#include <limits>

static const qreal kContextMenuItemTolerance = 10.;
static const QList<int> kFunctionTypes = { aadlinterface::AADLFunctionGraphicsItem::Type,
    aadlinterface::AADLFunctionTypeGraphicsItem::Type };
static const qreal kPreviewItemPenWidth = 2.;

namespace {

struct ValidationResult {
    aadl::AADLObjectIface *startIface { nullptr };
    aadl::AADLObjectIface *endIface { nullptr };
    shared::Id startIfaceId = {};
    shared::Id endIfaceId = {};
    QPointF startPointAdjusted {};
    QPointF endPointAdjusted {};
    QLineF connectionLine {};
    QGraphicsItem *functionAtStartPos { nullptr };
    aadl::AADLObjectFunction *startObject { nullptr };
    QGraphicsItem *functionAtEndPos { nullptr };
    aadl::AADLObjectFunction *endObject { nullptr };
    bool isToOrFromNested { false };

    aadl::ConnectionCreationValidator::FailReason status { aadl::ConnectionCreationValidator::FailReason::NoScene };
    inline bool failed() const { return status != aadl::ConnectionCreationValidator::FailReason::NotFail; }
    inline bool setFailed(aadl::ConnectionCreationValidator::FailReason s)
    {
        status = s;
        return failed();
    }
};

}

/*!
 * \brief The tolerance used to find an AADLInterfaceGraphicsItem on scene (the size of a squre used as a search area)
 */
static const qreal kInterfaceTolerance = 20.;

/*!
 * \brief Performs the validation to detect if it's possible to connect the \a scene's items located in \a startPos and
 * \a endPos.
 *
 * Returns the status of such validation as instance of ConnectionCreationValidator::ValidationResult.
 * Anything except the FailReason::NotFail in ConnectionCreationValidator::ValidationResult::status
 * means that the connection creation is prohibited.
 */
static ValidationResult validateCreate(QGraphicsScene *scene, const QPointF &startPos, const QPointF &endPos)
{
    ValidationResult result;

    result.connectionLine = { startPos, endPos };
    result.functionAtStartPos =
            aadlinterface::nearestItem(scene, aadlinterface::adjustFromPoint(startPos, kInterfaceTolerance),
                    { aadlinterface::AADLFunctionGraphicsItem::Type });
    result.functionAtEndPos =
            aadlinterface::nearestItem(scene, aadlinterface::adjustFromPoint(endPos, kInterfaceTolerance),
                    { aadlinterface::AADLFunctionGraphicsItem::Type });
    result.startObject = aadlinterface::gi::functionObject(result.functionAtStartPos);
    result.endObject = aadlinterface::gi::functionObject(result.functionAtEndPos);
    result.isToOrFromNested =
            (result.functionAtStartPos && result.functionAtStartPos->isAncestorOf(result.functionAtEndPos))
            || (result.functionAtEndPos && result.functionAtEndPos->isAncestorOf(result.functionAtStartPos));

    if (!result.startObject) {
        result.setFailed(aadl::ConnectionCreationValidator::FailReason::NoStartFunction);
        return result;
    }

    if (auto startIfaceItem = qgraphicsitem_cast<aadlinterface::AADLInterfaceGraphicsItem *>(
                aadlinterface::nearestItem(scene, aadlinterface::adjustFromPoint(startPos, kInterfaceTolerance),
                        { aadlinterface::AADLInterfaceGraphicsItem::Type }))) {
        result.startIface = startIfaceItem->entity();
        result.startPointAdjusted = startIfaceItem->scenePos();
    } else if (!shared::graphicsviewutils::intersects(result.functionAtStartPos->sceneBoundingRect(),
                       result.connectionLine, &result.startPointAdjusted)) {
        result.setFailed(aadl::ConnectionCreationValidator::FailReason::CannotCreateStartIface);
        return result;
    }

    if (!result.endObject) {
        result.setFailed(aadl::ConnectionCreationValidator::FailReason::NoEndFunction);
        return result;
    }

    if (auto endIfaceItem = qgraphicsitem_cast<aadlinterface::AADLInterfaceGraphicsItem *>(
                aadlinterface::nearestItem(scene, aadlinterface::adjustFromPoint(endPos, kInterfaceTolerance),
                        { aadlinterface::AADLInterfaceGraphicsItem::Type }))) {
        result.endIface = endIfaceItem->entity();
        result.endPointAdjusted = endIfaceItem->scenePos();
    } else if (!shared::graphicsviewutils::intersects(
                       result.functionAtEndPos->sceneBoundingRect(), result.connectionLine, &result.endPointAdjusted)) {
        result.setFailed(aadl::ConnectionCreationValidator::FailReason::CannotCreateEndIface);
        return result;
    }

    if (result.startIface && result.endIface && result.isToOrFromNested) {
        if (result.startIface->direction() != result.endIface->direction()) {
            result.setFailed(aadl::ConnectionCreationValidator::FailReason::ToFromNestedDifferentDirection);
            return result;
        }
    }

    if (result.startIface && result.endIface) {
        if (result.startIface->direction() == result.endIface->direction() && !result.isToOrFromNested) {
            result.setFailed(aadl::ConnectionCreationValidator::FailReason::SameDirectionIfaceWrongParents);
            return result;
        }
    }

    if (!result.startIface) {
        if (auto fn = result.startObject->as<const aadl::AADLObjectFunction *>())
            if (fn->instanceOf()) {
                result.setFailed(
                        aadl::ConnectionCreationValidator::FailReason::DirectIfaceCreationInInstanceOfFunctionType);
                return result;
            }
    }

    if (!result.endIface) {
        if (auto fn = result.endObject->as<const aadl::AADLObjectFunction *>())
            if (fn->instanceOf()) {
                result.setFailed(
                        aadl::ConnectionCreationValidator::FailReason::DirectIfaceCreationInInstanceOfFunctionType);
                return result;
            }
    }

    result.startIfaceId = result.startIface ? result.startIface->id() : shared::createId();
    result.endIfaceId = result.endIface ? result.endIface->id() : shared::createId();

    result.status = aadl::ConnectionCreationValidator::canConnect(
            result.startObject, result.endObject, result.startIface, result.endIface);
    return result;
}

/*!
 * \brief The helper method that perfoms validation and prints its status.
 * Used to bypass the \a scene and edge points of \a connectionPoints to
 * ConnectionCreationValidator::validateCreate() and returns the result of such validation.
 */
static ValidationResult validate(QGraphicsScene *scene, const QVector<QPointF> &connectionPoints)
{
    const ValidationResult info = validateCreate(scene, connectionPoints.first(), connectionPoints.last());
    qDebug() << info.status;
    return info;
}

namespace aadlinterface {

struct CreatorTool::CreatorToolPrivate {
    CreatorToolPrivate(CreatorTool *tool, QGraphicsView *v, aadl::AADLObjectsModel *m)
        : thisTool(tool)
        , view(v)
        , model(m)
    {
    }

    bool showContextMenu(const QPoint &globalPos);
    void populateContextMenu_commonCreate(QMenu *menu, const QPointF &scenePos);
    void populateContextMenu_propertiesDialog(QMenu *menu);
    void populateContextMenu_user(QMenu *menu, const QPointF &scenePos);

    void handleToolType(CreatorTool::ToolType type, const QPointF &pos);
    void handleComment(QGraphicsScene *scene, const QPointF &pos);
    void handleFunctionType(QGraphicsScene *scene, const QPointF &pos);
    void handleFunction(QGraphicsScene *scene, const QPointF &pos);
    void handleInterface(QGraphicsScene *scene, aadl::AADLObjectIface::IfaceType type, const QPointF &pos);
    bool handleConnectionCreate(const QPointF &pos);
    void handleDirectConnection(const QPointF &pos);
    void handleConnection(const QVector<QPointF> &connectionPoints) const;

    bool warnConnectionPreview(const QPointF &pos);

    QUndoCommand *createInterfaceCommand(const aadl::AADLObjectIface::CreationInfo &info) const;

    void clearPreviewItem();

    CreatorTool *thisTool;

    QPointF cursorInScene() const;
    QPointF cursorInScene(const QPoint &screenPos) const;

    QPointer<QGraphicsView> view;
    QPointer<aadl::AADLObjectsModel> model;
    QGraphicsRectItem *previewItem = nullptr;
    QGraphicsPathItem *previewConnectionItem = nullptr;
    QVector<QPointF> connectionPoints;
    QPointF clickScenePos;
    QCursor cursor;
    CreatorTool::ToolType toolType = ToolType::Pointer;
    QSet<InteractiveObject *> collidedItems;
};

CreatorTool::CreatorTool(QGraphicsView *view, aadl::AADLObjectsModel *model, QObject *parent)
    : QObject(parent)
    , d(new CreatorToolPrivate(this, view, model))
{
    Q_ASSERT(view != nullptr);
    Q_ASSERT(model != nullptr);

    if (d->view && d->view->viewport()) {
        d->view->installEventFilter(this);
        d->view->viewport()->installEventFilter(this);
    }
}

CreatorTool::~CreatorTool()
{
    delete d;
    d = nullptr;
}

CreatorTool::ToolType CreatorTool::toolType() const
{
    return d->toolType;
}

void CreatorTool::setCurrentToolType(CreatorTool::ToolType type)
{
    if (d->toolType == type)
        return;

    d->toolType = type;

    d->clearPreviewItem();

    if (d->view)
        d->view->setFocus();
}

void CreatorTool::removeSelectedItems()
{
    if (!d->view)
        return;

    if (auto scene = d->view->scene()) {
        QStringList clonedIfaces;
        cmd::CommandsStack::current()->beginMacro(tr("Remove selected item(s)"));
        while (!scene->selectedItems().isEmpty()) {
            d->clearPreviewItem();

            QGraphicsItem *item = scene->selectedItems().first();
            item->setSelected(false);

            if (auto iObj = qobject_cast<InteractiveObject *>(item->toGraphicsObject())) {
                if (auto entity = iObj->aadlObject()) {
                    if (entity->isInterface()) {
                        if (auto iface = entity->as<const aadl::AADLObjectIface *>()) {
                            if (auto srcIface = iface->cloneOf()) {
                                clonedIfaces.append(QStringLiteral("%1's %2 is from %3")
                                                            .arg(iface->parentObject()->title(), iface->title(),
                                                                    srcIface->parentObject()->title()));
                                continue;
                            }
                        }
                    }
                    const QVariantList params = { QVariant::fromValue(entity), QVariant::fromValue(d->model.data()) };
                    if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::RemoveEntity, params))
                        cmd::CommandsStack::current()->push(cmdRm);
                }
            }
        }
        cmd::CommandsStack::current()->endMacro();

        if (!clonedIfaces.isEmpty()) {
            const QString names = clonedIfaces.join(QStringLiteral("<br>"));
            const QString msg = tr("The following interfaces can not be removed directly:<br><br>"
                                   "<b>%1</b><br><br>"
                                   "Please edit the related FunctionType.")
                                        .arg(names);
            Q_EMIT informUser(tr("Interface removal"), msg);
        }
    }
}

bool CreatorTool::eventFilter(QObject *watched, QEvent *event)
{
    if (d->view && watched == d->view->viewport()) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            return onMousePress(static_cast<QMouseEvent *>(event));
        case QEvent::MouseButtonRelease:
            return onMouseRelease(static_cast<QMouseEvent *>(event));
        case QEvent::MouseMove:
            return onMouseMove(static_cast<QMouseEvent *>(event));
        case QEvent::MouseButtonDblClick: {
            QMouseEvent *e = static_cast<QMouseEvent *>(event);
            if (e->button() & Qt::RightButton) // block double click for right click and handle it
                return onMousePress(e);
            return false;
        }
        default:
            break;
        }
    }

    if (d->view == watched) {
        if (event->type() == QEvent::ContextMenu)
            return onContextMenu(static_cast<QContextMenuEvent *>(event));

        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            switch (keyEvent->key()) {
            case Qt::Key_Delete: {
                removeSelectedItems();
            } break;
            case Qt::Key_Escape: {
                if (toolType() == ToolType::Pointer) {
                    if (auto scene = d->view->scene())
                        scene->clearSelection();
                } else {
                    d->clearPreviewItem();
                    Q_EMIT created();
                }
            } break;
            }
        }
    }

    return false;
}

// Right mouse (ctrl or not) = get the context menu with the creation choices
// Left mouse + ctrl = shortcut to creating a connection + the PI and RI between two boxes
// Left mouse without ctrl: If a tool is selected, perform the tool. Otherwise, select the items
bool CreatorTool::onMousePress(QMouseEvent *e)
{
    if (d->view.isNull()) {
        return false;
    }

    auto scene = d->view->scene();
    if (scene == nullptr) {
        return false;
    }

    const QPointF scenePos = d->cursorInScene(e->globalPos());
    if (e->modifiers() & Qt::ControlModifier) {
        auto itemAtCursor = d->view->itemAt(e->pos());
        if ((e->button() & Qt::MouseButton::LeftButton)
                && (!itemAtCursor || itemAtCursor->type() != shared::ui::GripPoint::Type))
            d->toolType = ToolType::DirectConnection;
        else
            return false;
    } else if (!(e->button() & Qt::RightButton) && d->toolType == ToolType::Pointer) {
        return false;
    }

    if (d->toolType == ToolType::DirectConnection && e->button() != Qt::RightButton) {
        static const QList<int> functionItemsOnly { static_cast<int>(AADLFunctionGraphicsItem::Type) };
        if (!nearestItem(scene, scenePos, functionItemsOnly)) {
            if (!nearestItem(scene, scenePos, ::kInterfaceTolerance / 2, functionItemsOnly))
                return false;
        }

        if (d->previewConnectionItem) {
            d->connectionPoints.clear();
        } else {
            d->previewConnectionItem = new QGraphicsPathItem;
            d->previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
            d->previewConnectionItem->setZValue(ZOrder.Preview);
            scene->addItem(d->previewConnectionItem);
        }
        d->connectionPoints.append(scenePos);
        return true;
    } else if (d->toolType == ToolType::MultiPointConnection && e->button() != Qt::RightButton) {
        if (!d->previewConnectionItem) {
            QGraphicsItem *item =
                    nearestItem(scene, scenePos, kInterfaceTolerance, { AADLInterfaceGraphicsItem::Type });
            if (!item)
                return false;

            const QPointF startPoint = item->mapToScene(QPointF(0, 0));
            d->previewConnectionItem = new QGraphicsPathItem;
            d->previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
            d->previewConnectionItem->setZValue(ZOrder.Preview);
            scene->addItem(d->previewConnectionItem);
            d->connectionPoints.append(startPoint);
            return true;
        }
        return !d->connectionPoints.contains(scenePos);
    } else if (e->button() == Qt::RightButton
            || (d->toolType != ToolType::RequiredInterface && d->toolType != ToolType::ProvidedInterface)) {
        if (!d->previewItem) {
            QGraphicsItem *parentItem = d->view->itemAt(e->pos());
            while (parentItem != nullptr && parentItem->type() != AADLFunctionGraphicsItem::Type) {
                parentItem = parentItem->parentItem();
            }
            d->previewItem = new QGraphicsRectItem(parentItem);
            d->previewItem->setPen(QPen(Qt::blue, kPreviewItemPenWidth, Qt::SolidLine));
            d->previewItem->setBrush(QBrush(QColor(30, 144, 255, 90)));
            d->previewItem->setZValue(ZOrder.Preview);
            d->clickScenePos = scenePos;

            if (!parentItem)
                scene->addItem(d->previewItem);

            if (!e->buttons().testFlag(Qt::MaxMouseButton)) {
                auto items = d->view->items(e->pos());
                for (auto item : items) {
                    if (item->type() > QGraphicsItem::UserType) {
                        if (!item->isSelected()) {
                            scene->clearSelection();
                            item->setSelected(true);
                        }
                        break;
                    }
                }
            }
        }
        const QPointF mappedScenePos = d->previewItem->mapFromScene(scenePos);
        d->previewItem->setRect({ mappedScenePos, mappedScenePos });
        return true;
    }

    return true;
}

bool CreatorTool::onMouseRelease(QMouseEvent *e)
{
    if (!d->view)
        return false;

    if ((e->button() & Qt::RightButton) && d->previewItem) {
        return d->showContextMenu(e->globalPos());
    } else if (d->toolType != ToolType::Pointer) {
        const bool hasPreview = d->previewItem || d->previewConnectionItem;
        const bool isIface = d->toolType == ToolType::ProvidedInterface || d->toolType == ToolType::RequiredInterface;
        if (hasPreview || isIface) {
            const QPointF &scenePos = d->cursorInScene(e->globalPos());
            d->handleToolType(d->toolType, scenePos);
            return true;
        }
    }
    return false;
}

bool CreatorTool::onMouseMove(QMouseEvent *e)
{
    if (!d->view || !d->view->scene())
        return false;

    const QPointF &scenePos = d->cursorInScene(e->globalPos());
    if (d->previewItem && d->previewItem->isVisible()) {
        const QRectF newGeometry = QRectF(d->clickScenePos, scenePos).normalized();
        if (!newGeometry.isValid())
            return true;

        QSet<InteractiveObject *> items;
        const QRectF expandedGeometry { newGeometry.marginsAdded(kContentMargins) };
        QList<QGraphicsItem *> newCollidedItems = d->view->scene()->items(expandedGeometry);
        std::for_each(newCollidedItems.begin(), newCollidedItems.end(),
                [this, &items, expandedGeometry](QGraphicsItem *item) {
                    if (item->type() == AADLInterfaceGraphicsItem::Type || item->type() == d->previewItem->type())
                        return;

                    auto iObjItem = qobject_cast<InteractiveObject *>(item->toGraphicsObject());
                    if (!iObjItem)
                        return;

                    if (item->parentItem() == d->previewItem->parentItem()
                            || (d->previewItem->parentItem() == item
                                    && !item->sceneBoundingRect().contains(expandedGeometry))) {
                        items.insert(iObjItem);
                    }
                });
        QSet<InteractiveObject *> newItems(items);
        newItems.subtract(d->collidedItems);
        for (auto item : newItems)
            item->doHighlighting(Qt::red, true);

        QSet<InteractiveObject *> oldItems(d->collidedItems);
        oldItems.subtract(items);

        for (auto item : oldItems)
            item->doHighlighting(Qt::green, false);

        d->collidedItems = items;
        d->previewItem->setRect(d->previewItem->mapRectFromScene(newGeometry));
        return true;
    } else if (d->previewConnectionItem && d->previewConnectionItem->isVisible() && !d->connectionPoints.isEmpty()) {
        if (d->view->scene()) {
            QPainterPath pp;
            pp.addPolygon(d->connectionPoints);
            pp.lineTo(scenePos);
            d->previewConnectionItem->setPath(pp);
            d->warnConnectionPreview(scenePos);
            return true;
        }
    }

    return false;
}

bool CreatorTool::onContextMenu(QContextMenuEvent *e)
{
    if (!d->view || e->reason() == QContextMenuEvent::Mouse)
        return false;

    QGraphicsScene *scene = d->view->scene();
    if (!scene)
        return false;

    QPoint viewPos, globalPos;
    if (!scene->selectedItems().isEmpty()) {
        QGraphicsItem *selectedItem = scene->selectedItems().first();
        const QPointF &scenePos = selectedItem->mapToScene(selectedItem->boundingRect().bottomRight());
        viewPos = d->view->mapFromScene(scenePos);
        globalPos = d->view->mapToGlobal(viewPos);
    } else {
        globalPos = QCursor::pos();
        viewPos = d->view->mapFromGlobal(globalPos);
    }

    // onMousePress is needed to set an apppropriate d->previewItem
    QMouseEvent mouseEvent(QEvent::MouseButtonPress, viewPos, Qt::RightButton, Qt::RightButton | Qt::MaxMouseButton,
            0); // Qt::MaxMouseButton is a fake button
                // to distinguish this mouse event
                // and thus avoid selecting of another object
    onMousePress(&mouseEvent);

    return d->showContextMenu(globalPos);
}

template<typename ItemType>
ItemType *itemAt(const QGraphicsScene *scene, const QPointF &point)
{
    QList<QGraphicsItem *> items = scene->items(point);
    if (items.isEmpty())
        return nullptr;
    auto it = std::find_if(items.constBegin(), items.constEnd(),
            [](const QGraphicsItem *item) { return item->type() == ItemType::Type; });
    if (it == items.constEnd())
        return nullptr;

    return qgraphicsitem_cast<ItemType *>(*it);
}

static inline bool alignToSidePoint(const QGraphicsScene *scene, const QPointF &point, QPointF &pointToAlign)
{
    auto foundItem = nearestItem(scene, point, kInterfaceTolerance / 2, { AADLInterfaceGraphicsItem::Type });
    if (!foundItem)
        return false;

    auto ifaceItem = qgraphicsitem_cast<const AADLInterfaceGraphicsItem *>(foundItem);
    if (!ifaceItem)
        return false;

    QGraphicsItem *targetItem = ifaceItem->targetItem();
    if (!targetItem)
        return false;

    switch (getNearestSide(targetItem->sceneBoundingRect(), point)) {
    case Qt::AlignTop:
    case Qt::AlignBottom:
        pointToAlign.setX(point.x());
        break;
    case Qt::AlignLeft:
    case Qt::AlignRight:
        pointToAlign.setY(point.y());
        break;
    default:
        break;
    }
    return true;
}

static inline QRectF adjustToSize(const QRectF &rect, const QSizeF &minSize)
{
    QRectF itemRect = rect;
    if (itemRect.width() < minSize.width())
        itemRect.setWidth(minSize.width());
    if (itemRect.height() < minSize.height())
        itemRect.setHeight(minSize.height());
    return itemRect;
};

bool CreatorTool::CreatorToolPrivate::showContextMenu(const QPoint &globalPos)
{
    QMenu *menu = new QMenu(view);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    auto scenePos = cursorInScene(globalPos);
    populateContextMenu_commonCreate(menu, scenePos);
    populateContextMenu_propertiesDialog(menu);
    populateContextMenu_user(menu, scenePos);

    if (menu->isEmpty()) {
        delete menu;
        menu = nullptr;
    }

    connect(menu, &QMenu::aboutToHide, [this]() {
        if (previewItem) {
            previewItem->setVisible(false);
        }
    });
    menu->exec(globalPos);
    clearPreviewItem();
    return true;
}

void CreatorTool::CreatorToolPrivate::populateContextMenu_commonCreate(QMenu *menu, const QPointF &scenePos)
{
    if (this->previewItem) {
        const QSizeF emptyPreviewItemSize = QSizeF(kPreviewItemPenWidth, kPreviewItemPenWidth);
        const bool isRect = this->previewItem->boundingRect().size() != emptyPreviewItemSize;
        // TODO: use a Fn/FnType/Comment's min size to disable related actions if the creation is impossible?

        auto action = menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/function_type.svg")),
                thisTool->tr("Function Type"), thisTool,
                [this, scenePos]() { handleToolType(ToolType::FunctionType, scenePos); });

        action = menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")),
                thisTool->tr("Function"), thisTool,
                [this, scenePos]() { handleToolType(ToolType::Function, scenePos); });

        action = menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/comment.svg")),
                thisTool->tr("Comment"), thisTool, [this, scenePos]() { handleToolType(ToolType::Comment, scenePos); });

        action = menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/ri.svg")),
                thisTool->tr("Required Interface"), thisTool,
                [this, scenePos]() { handleToolType(ToolType::RequiredInterface, scenePos); });
        action->setEnabled(!isRect && this->previewItem->parentItem());

        action = menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/pi.svg")),
                thisTool->tr("Provided Interface"), thisTool,
                [this, scenePos]() { handleToolType(ToolType::ProvidedInterface, scenePos); });
        action->setEnabled(!isRect && this->previewItem->parentItem());
    }
}

void CreatorTool::CreatorToolPrivate::populateContextMenu_propertiesDialog(QMenu *menu)
{
    QGraphicsScene *scene = view->scene();
    if (!scene) {
        return;
    }

    aadl::AADLObject *aadlObj { nullptr };
    if (QGraphicsItem *gi = scene->selectedItems().isEmpty() ? nullptr : scene->selectedItems().first()) {
        switch (gi->type()) {
        case AADLFunctionTypeGraphicsItem::Type: {
            aadlObj = gi::functionTypeObject(gi);
            break;
        }
        case AADLFunctionGraphicsItem::Type: {
            aadlObj = gi::functionObject(gi);
            break;
        }
        case AADLInterfaceGraphicsItem::Type: {
            aadlObj = gi::interfaceObject(gi);
            break;
        }
        default:
            break;
        }
    }

    menu->addSeparator();
    QAction *action = menu->addAction(tr("Properties"));
    action->setEnabled(aadlObj);

    if (aadlObj) {
        connect(action, &QAction::triggered, [this, aadlObj]() { Q_EMIT thisTool->propertyEditorRequest(aadlObj); });
        ActionsManager::registerAction(
                Q_FUNC_INFO, menu->actions().last(), "Properties", "Show AADL object properties editor");
    }
}

void CreatorTool::CreatorToolPrivate::populateContextMenu_user(QMenu *menu, const QPointF &scenePos)
{
    QGraphicsScene *scene = view->scene();
    if (!scene)
        return;

    static const QList<int> showProps { AADLInterfaceGraphicsItem::Type, AADLFunctionTypeGraphicsItem::Type,
        AADLFunctionGraphicsItem::Type, AADLCommentGraphicsItem::Type, AADLConnectionGraphicsItem::Type };

    aadl::AADLObject *aadlObj { nullptr };
    if (QGraphicsItem *gi = scene->selectedItems().size() == 1
                    ? scene->selectedItems().first()
                    : nearestItem(scene, scenePos, kContextMenuItemTolerance, showProps)) {

        switch (gi->type()) {
        case AADLFunctionTypeGraphicsItem::Type: {
            aadlObj = gi::functionTypeObject(gi);
            break;
        }
        case AADLFunctionGraphicsItem::Type: {
            aadlObj = gi::functionObject(gi);
            break;
        }
        case AADLInterfaceGraphicsItem::Type: {
            aadlObj = gi::interfaceObject(gi);
            break;
        }
        case AADLCommentGraphicsItem::Type: {
            aadlObj = gi::commentObject(gi);
            break;
        }
        case AADLConnectionGraphicsItem::Type: {
            aadlObj = gi::connectionObject(gi);
            break;
        }
        default:
            return;
        }
    }

    ActionsManager::populateMenu(menu, aadlObj);
}

void CreatorTool::CreatorToolPrivate::handleToolType(CreatorTool::ToolType type, const QPointF &pos)
{
    if (!view)
        return;

    if (auto scene = view->scene()) {
        switch (type) {
        case ToolType::Comment:
            handleComment(scene, pos);
            break;
        case ToolType::FunctionType:
            handleFunctionType(scene, pos);
            break;
        case ToolType::Function:
            handleFunction(scene, pos);
            break;
        case ToolType::ProvidedInterface:
            handleInterface(scene, aadl::AADLObjectIface::IfaceType::Provided, pos);
            break;
        case ToolType::RequiredInterface:
            handleInterface(scene, aadl::AADLObjectIface::IfaceType::Required, pos);
            break;
        case ToolType::MultiPointConnection:
            if (!handleConnectionCreate(pos))
                return;
            handleConnection(connectionPoints);
            break;
        case ToolType::DirectConnection:
            handleDirectConnection(pos);
            break;
        default:
            break;
        }
        clearPreviewItem();
    }

    Q_EMIT thisTool->created();
}

void CreatorTool::CreatorToolPrivate::handleComment(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene)
    Q_UNUSED(pos)

    if (this->previewItem) {
        aadl::AADLObjectFunctionType *parentObject = gi::functionObject(this->previewItem->parentItem());
        if (!parentObject)
            parentObject = gi::functionTypeObject(this->previewItem->parentItem());

        const QRectF itemSceneRect =
                adjustToSize(this->previewItem->mapRectToScene(this->previewItem->rect()), DefaultGraphicsItemSize);
        const QVariantList params = { QVariant::fromValue(this->model.data()), QVariant::fromValue(parentObject),
            itemSceneRect };
        cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateCommentEntity, params));
    }
}

void CreatorTool::CreatorToolPrivate::handleFunctionType(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene)
    Q_UNUSED(pos)

    if (this->previewItem) {
        const QRectF itemSceneRect =
                adjustToSize(this->previewItem->mapRectToScene(this->previewItem->rect()), DefaultGraphicsItemSize);

        if (!gi::canPlaceRect(scene, this->previewItem, itemSceneRect, gi::RectOperation::Create))
            return;

        aadl::AADLObjectFunction *parentObject = gi::functionObject(this->previewItem->parentItem());

        const QVariantList params = { QVariant::fromValue(this->model.data()), QVariant::fromValue(parentObject),
            itemSceneRect };
        cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateFunctionTypeEntity, params));
    }
}

void CreatorTool::CreatorToolPrivate::handleFunction(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene)
    Q_UNUSED(pos)

    if (this->previewItem) {
        const QRectF itemSceneRect =
                adjustToSize(this->previewItem->mapRectToScene(this->previewItem->rect()), DefaultGraphicsItemSize);

        if (!gi::canPlaceRect(scene, this->previewItem, itemSceneRect, gi::RectOperation::Create))
            return;

        aadl::AADLObjectFunction *parentObject = gi::functionObject(this->previewItem->parentItem());
        const QVariantList params = { QVariant::fromValue(this->model.data()), QVariant::fromValue(parentObject),
            itemSceneRect };

        cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateFunctionEntity, params));
    }
}

void CreatorTool::CreatorToolPrivate::handleInterface(
        QGraphicsScene *scene, aadl::AADLObjectIface::IfaceType type, const QPointF &pos)
{
    if (auto parentItem = nearestItem(scene, adjustFromPoint(pos, ::kInterfaceTolerance), kFunctionTypes)) {
        aadl::AADLObjectFunctionType *parentObject = gi::functionTypeObject(parentItem);
        aadl::AADLObjectIface::CreationInfo ifaceDescr(this->model.data(), parentObject, pos, type, shared::InvalidId);
        ifaceDescr.resetKind();

        if (auto cmd = createInterfaceCommand(ifaceDescr))
            cmd::CommandsStack::current()->push(cmd);
    }
}

bool CreatorTool::CreatorToolPrivate::handleConnectionCreate(const QPointF &pos)
{
    QGraphicsScene *scene = this->view ? this->view->scene() : nullptr;
    if (!scene)
        return false;

    if (!this->previewConnectionItem)
        return false;

    QPointF alignedPos { pos };
    if (this->connectionPoints.size() > 2) {
        if (auto itemUnderCursor =
                        nearestItem(scene, pos, ::kInterfaceTolerance, { AADLInterfaceGraphicsItem::Type })) {
            const QPointF finishPoint = itemUnderCursor->mapToScene(QPointF(0, 0));
            if (!alignToSidePoint(scene, finishPoint, this->connectionPoints.last()))
                return false;

            this->connectionPoints.append(finishPoint);
            return true;
        }
    } else if (!alignToSidePoint(scene, this->connectionPoints.value(0), alignedPos))
        return false;

    this->connectionPoints.append(alignedPos);

    QPainterPath pp;
    pp.addPolygon(this->connectionPoints);
    pp.lineTo(pos);
    this->previewConnectionItem->setPath(pp);
    return false;
}

void CreatorTool::CreatorToolPrivate::handleDirectConnection(const QPointF &pos)
{
    if (this->connectionPoints.size() < 1)
        return;

    this->connectionPoints.append(pos);

    handleConnection(this->connectionPoints);
}

void CreatorTool::CreatorToolPrivate::handleConnection(const QVector<QPointF> &connectionPoints) const
{
    auto info = ::validate(this->view ? this->view->scene() : nullptr, connectionPoints);
    if (info.failed())
        return;

    const AADLFunctionGraphicsItem *parentForConnection = nullptr;
    QPointF startInterfacePoint { info.startPointAdjusted };
    QPointF endInterfacePoint { info.endPointAdjusted };
    aadl::AADLObjectIface::CreationInfo ifaceCommons;
    cmd::CommandsStack::Macro cmdMacro(tr("Create connection"));

    bool startRequired = true;
    if (info.startIface && !info.endIface) {
        startRequired = info.startIface->isRequired();

        ifaceCommons = aadl::AADLObjectIface::CreationInfo::fromIface(info.startIface);
        ifaceCommons.function = info.endObject;
        ifaceCommons.position = info.endPointAdjusted;
        ifaceCommons.type = (startRequired && info.isToOrFromNested) || (!startRequired && !info.isToOrFromNested)
                ? aadl::AADLObjectIface::IfaceType::Required
                : aadl::AADLObjectIface::IfaceType::Provided;
        ifaceCommons.id = info.endIfaceId;
        ifaceCommons.resetKind();

        if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
            return;
    } else if (info.endIface && !info.startIface) {
        startRequired = (info.endIface->isRequired() && info.isToOrFromNested)
                || (info.endIface->isProvided() && !info.isToOrFromNested);

        ifaceCommons = aadl::AADLObjectIface::CreationInfo::fromIface(info.endIface);
        ifaceCommons.function = info.startObject;
        ifaceCommons.position = info.startPointAdjusted;
        ifaceCommons.type =
                startRequired ? aadl::AADLObjectIface::IfaceType::Required : aadl::AADLObjectIface::IfaceType::Provided;
        ifaceCommons.id = info.startIfaceId;
        ifaceCommons.resetKind();

        if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
            return;
    } else if (!info.startIface && !info.endIface) {
        ifaceCommons.model = this->model.data();

        ifaceCommons.function = info.startObject;
        ifaceCommons.position = info.startPointAdjusted;
        ifaceCommons.id = info.startIfaceId;

        const bool isToNested = info.isToOrFromNested && info.functionAtEndPos->isAncestorOf(info.functionAtStartPos);
        if (info.isToOrFromNested)
            ifaceCommons.type = (isToNested) ? aadl::AADLObjectIface::IfaceType::Required
                                             : aadl::AADLObjectIface::IfaceType::Provided;
        else
            ifaceCommons.type = (startRequired ? aadl::AADLObjectIface::IfaceType::Required
                                               : aadl::AADLObjectIface::IfaceType::Provided);
        ifaceCommons.resetKind();

        if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
            return;

        ifaceCommons.function = info.endObject;
        ifaceCommons.position = info.endPointAdjusted;
        ifaceCommons.id = info.endIfaceId;

        if (info.isToOrFromNested)
            ifaceCommons.type = (isToNested) ? aadl::AADLObjectIface::IfaceType::Required
                                             : aadl::AADLObjectIface::IfaceType::Provided;
        else
            ifaceCommons.type = startRequired ? aadl::AADLObjectIface::IfaceType::Provided
                                              : aadl::AADLObjectIface::IfaceType::Required;
        ifaceCommons.resetKind();

        if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
            return;
    } else {
        aadl::AADLObjectIface *pi = aadl::AADLObjectConnection::selectIface<aadl::AADLObjectIfaceProvided *>(
                info.startIface, info.endIface);
        if (!pi)
            pi = info.startIface;
        ifaceCommons = aadl::AADLObjectIface::CreationInfo::fromIface(pi);
        ifaceCommons.resetKind();
        ifaceCommons.name.clear();
    }

    AADLFunctionGraphicsItem *prevStartItem =
            qgraphicsitem_cast<aadlinterface::AADLFunctionGraphicsItem *>(info.functionAtStartPos);
    QPointF firstExcludedPoint = *std::next(connectionPoints.constBegin());
    shared::Id prevStartIfaceId = info.startIfaceId;
    /// TODO: check creating connection from nested function as a start function
    while (auto item = qgraphicsitem_cast<aadlinterface::AADLFunctionGraphicsItem *>(prevStartItem->parentItem())) {
        const QVector<QPointF> intersectionPoints =
                shared::graphicsviewutils::intersectionPoints(item->sceneBoundingRect(), connectionPoints);
        if (intersectionPoints.isEmpty() || intersectionPoints.size() % 2 == 0) {
            parentForConnection = item;
            break;
        }

        const QRectF rect = item->sceneBoundingRect();
        auto beginIt = std::find(connectionPoints.constBegin(), connectionPoints.constEnd(), firstExcludedPoint);
        auto endIt = std::find_if(connectionPoints.constBegin(), connectionPoints.constEnd(),
                [&rect](const QPointF &point) { return !rect.contains(point); });
        QVector<QPointF> points { startInterfacePoint };
        std::copy(beginIt, endIt, std::back_inserter(points));
        points.append(intersectionPoints.last());

        ifaceCommons.type =
                startRequired ? aadl::AADLObjectIface::IfaceType::Required : aadl::AADLObjectIface::IfaceType::Provided;

        if (item == info.functionAtEndPos) {
            ifaceCommons.id = info.endIfaceId;
        } else {
            ifaceCommons.function = item->entity();
            ifaceCommons.position = intersectionPoints.last();
            ifaceCommons.id = shared::createId();

            if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
                return;
        }

        const QVariantList params = { QVariant::fromValue(this->model.data()), QVariant::fromValue(item->entity()),
            prevStartIfaceId, ifaceCommons.id, QVariant::fromValue(points) };
        if (!cmdMacro.push(cmd::CommandsFactory::create(cmd::CreateConnectionEntity, params)))
            return;

        firstExcludedPoint = endIt != connectionPoints.constEnd() ? *endIt : QPointF();
        startInterfacePoint = intersectionPoints.last();
        prevStartItem = item;
        prevStartIfaceId = ifaceCommons.id;
    }

    QPointF lastExcludedPoint = *std::next(connectionPoints.crbegin());
    auto prevEndItem = qgraphicsitem_cast<aadlinterface::AADLFunctionGraphicsItem *>(info.functionAtEndPos);
    shared::Id prevEndIfaceId = info.endIfaceId;
    /// TODO: check creating connection from parent item as a start function
    while (auto item = qgraphicsitem_cast<aadlinterface::AADLFunctionGraphicsItem *>(prevEndItem->parentItem())) {
        auto intersectionPoints =
                shared::graphicsviewutils::intersectionPoints(item->sceneBoundingRect(), connectionPoints);
        if (intersectionPoints.isEmpty() || intersectionPoints.size() % 2 == 0) {
            Q_ASSERT(parentForConnection == item || parentForConnection == nullptr);
            parentForConnection = item;
            break;
        }
        const QRectF rect = item->sceneBoundingRect();
        auto beginIt = std::find(connectionPoints.crbegin(), connectionPoints.crend(), lastExcludedPoint);
        auto endIt = std::find_if(connectionPoints.crbegin(), connectionPoints.crend(),
                [&rect](const QPointF &point) { return !rect.contains(point); });
        QVector<QPointF> points { endInterfacePoint };
        std::copy(beginIt, endIt, std::back_inserter(points));
        points.append(intersectionPoints.last());

        if (item == info.functionAtStartPos) {
            ifaceCommons.id = info.startIfaceId;
        } else {
            ifaceCommons.id = shared::createId();

            ifaceCommons.function = item->entity();
            ifaceCommons.position = intersectionPoints.last();
            ifaceCommons.type = startRequired ? aadl::AADLObjectIface::IfaceType::Provided
                                              : aadl::AADLObjectIface::IfaceType::Required;
            ifaceCommons.id = ifaceCommons.id;

            if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
                return;
        }
        const QVariantList params = { QVariant::fromValue(this->model.data()), QVariant::fromValue(item->entity()),
            prevEndIfaceId, ifaceCommons.id, QVariant::fromValue(points) };
        if (!cmdMacro.push(cmd::CommandsFactory::create(cmd::CreateConnectionEntity, params)))
            return;

        lastExcludedPoint = endIt != connectionPoints.crend() ? *endIt : QPointF();
        endInterfacePoint = intersectionPoints.last();
        prevEndItem = item;
        prevEndIfaceId = ifaceCommons.id;
    }

    auto beginIt = std::find(connectionPoints.constBegin(), connectionPoints.constEnd(), firstExcludedPoint);
    auto endIt = std::find(beginIt, connectionPoints.constEnd(), lastExcludedPoint);
    QVector<QPointF> points { startInterfacePoint };
    if (endIt != connectionPoints.constEnd()) {
        std::advance(endIt, 1);
        std::copy(beginIt, endIt, std::back_inserter(points));
    }
    points.append(endInterfacePoint);
    Q_ASSERT(points.size() >= 2);
    if (points.first() != points.last()) {
        const QVariantList params = { QVariant::fromValue(this->model.data()),
            QVariant::fromValue(parentForConnection ? parentForConnection->entity() : nullptr), prevStartIfaceId,
            prevEndIfaceId, QVariant::fromValue(points) };
        if (!cmdMacro.push(cmd::CommandsFactory::create(cmd::CreateConnectionEntity, params)))
            return;
    }

    cmdMacro.setComplete(true);
}

bool CreatorTool::CreatorToolPrivate::warnConnectionPreview(const QPointF &pos)
{
    QVector<QPointF> connectionPoints = this->connectionPoints;
    if (connectionPoints.size() > 1)
        connectionPoints.replace(connectionPoints.size() - 1, pos);
    else
        connectionPoints.append(pos);

    auto info = ::validate(this->view ? this->view->scene() : nullptr, connectionPoints);
    const bool warn = info.failed();

    if (this->previewConnectionItem) {
        QPen p = this->previewConnectionItem->pen();
        p.setColor(warn ? Qt::red : Qt::black);
        this->previewConnectionItem->setPen(p);
    }

    return warn;
}

QUndoCommand *CreatorTool::CreatorToolPrivate::createInterfaceCommand(
        const aadl::AADLObjectIface::CreationInfo &info) const
{
    if (!info.function)
        return nullptr;

    if (info.function->isFunction()) {
        if (auto fn = info.function->as<const aadl::AADLObjectFunction *>()) {
            if (const aadl::AADLObjectFunctionType *fnType = fn->instanceOf()) {
                const QString message = thisTool->tr("Can't add interface directly in <b>%1</b>.<br>"
                                                     "Please edit the related <b>%2</b> instead.")
                                                .arg(fn->title(), fnType->title());
                Q_EMIT thisTool->informUser(thisTool->tr("Interface adding"), message);
                return nullptr;
            }
        }
    }

    return cmd::CommandsFactory::create(cmd::CreateInterfaceEntity, info.toVarList());
}

void CreatorTool::CreatorToolPrivate::clearPreviewItem()
{
    for (auto iObj : this->collidedItems) {
        iObj->highlightConnected();
    }
    this->collidedItems.clear();

    this->clickScenePos = QPointF();

    this->connectionPoints.clear();
    if (this->previewConnectionItem) {
        this->previewConnectionItem->scene()->removeItem(this->previewConnectionItem);
        delete this->previewConnectionItem;
        this->previewConnectionItem = nullptr;
    }

    if (this->previewItem) {
        this->previewItem->scene()->removeItem(this->previewItem);
        delete this->previewItem;
        this->previewItem = nullptr;
    }
}

QPointF CreatorTool::CreatorToolPrivate::cursorInScene() const
{
    return cursorInScene(QCursor::pos()); // TODO: add current screen detection
}

QPointF CreatorTool::CreatorToolPrivate::cursorInScene(const QPoint &globalPos) const
{
    QPointF sceneCoordinates;
    if (this->view) {
        const QPoint viewCoordinates = this->view->viewport()->mapFromGlobal(globalPos);
        sceneCoordinates = this->view->mapToScene(viewCoordinates);
    }
    return sceneCoordinates;
}

}
