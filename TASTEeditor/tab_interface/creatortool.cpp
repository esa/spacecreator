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
#include "app/context/action/actionsmanager.h"
#include "commands/cmdcommentitemcreate.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/cmdfunctiontypeitemcreate.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <QtDebug>
#include <QtMath>
#include <app/commandsstack.h>
#include <baseitems/common/utils.h>
#include <limits>
#include <tab_aadl/aadlobjectcomment.h>
#include <tab_aadl/aadlobjectfunction.h>
#include <tab_aadl/aadlobjectfunctiontype.h>
#include <tab_aadl/aadlobjectiface.h>
#include <tab_aadl/aadlobjectsmodel.h>

static const qreal kInterfaceTolerance = 20.;
static const qreal kConnectionTolerance = 20.;
static const qreal kContextMenuItemTolerance = 10.;
static const QMarginsF kMargins { 50., 50., 50., 50. };
static const QList<int> kFunctionTypes = { taste3::aadl::AADLFunctionGraphicsItem::Type,
                                           taste3::aadl::AADLFunctionTypeGraphicsItem::Type };

namespace taste3 {
namespace aadl {

CreatorTool::CreatorTool(QGraphicsView *view, AADLObjectsModel *model, QObject *parent)
    : QObject(parent)
    , m_view(view)
    , m_model(model)
{
    setObjectName(QLatin1String("CreatorTool"));

    Q_ASSERT(view);
    Q_ASSERT(model);

    if (m_view && m_view->viewport()) {
        m_view->installEventFilter(this);
        m_view->viewport()->installEventFilter(this);
    }
}

CreatorTool::ToolType CreatorTool::toolType() const
{
    return m_toolType;
}

void CreatorTool::setCurrentToolType(CreatorTool::ToolType type)
{
    if (m_toolType == type)
        return;

    m_toolType = type;

    clearPreviewItem();
}

bool CreatorTool::eventFilter(QObject *watched, QEvent *event)
{
    if (m_view && watched == m_view->viewport()) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            return onMousePress(static_cast<QMouseEvent *>(event));
        case QEvent::MouseButtonRelease:
            return onMouseRelease(static_cast<QMouseEvent *>(event));
        case QEvent::MouseMove:
            return onMouseMove(static_cast<QMouseEvent *>(event));
        default:
            break;
        }
    }
    if (m_view == watched && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key()) {
        case Qt::Key_Delete: {
            removeSelectedItems();
        } break;
        case Qt::Key_Escape: {
            if (toolType() == ToolType::Pointer) {
                if (auto scene = m_view->scene())
                    scene->clearSelection();
            } else {
                clearPreviewItem();
            }
        } break;
        }
    }

    return false;
}

bool CreatorTool::onMousePress(QMouseEvent *e)
{
    if (!m_view)
        return false;

    auto scene = m_view->scene();
    if (!scene)
        return false;

    if (e->modifiers() & Qt::ControlModifier)
        m_toolType = ToolType::DirectConnection;

    if (!(e->button() & Qt::RightButton) && m_toolType == ToolType::Pointer)
        return false;

    const QPointF scenePos = cursorInScene(e->globalPos());
    if (m_toolType == ToolType::DirectConnection) {
        if (!utils::nearestItem(scene, scenePos,
                                { AADLFunctionTypeGraphicsItem::Type, AADLFunctionGraphicsItem::Type }))
            if (!utils::nearestItem(scene, scenePos, kInterfaceTolerance / 2, { AADLFunctionGraphicsItem::Type }))
                return false;

        if (m_previewConnectionItem) {
            m_connectionPoints.clear();
        } else {
            m_previewConnectionItem = new QGraphicsPathItem;
            m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
            m_previewConnectionItem->setZValue(1);
            scene->addItem(m_previewConnectionItem);
        }
        m_connectionPoints.append(scenePos);
        return true;
    } else if (m_toolType == ToolType::MultiPointConnection) {
        QGraphicsItem *item =
                utils::nearestItem(scene, scenePos, kConnectionTolerance, { AADLInterfaceGraphicsItem::Type });
        if (!m_previewConnectionItem) {
            if (!item)
                return false;

            const QPointF startPoint = item->sceneBoundingRect().center();
            m_previewConnectionItem = new QGraphicsPathItem;
            m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
            m_previewConnectionItem->setZValue(1);
            scene->addItem(m_previewConnectionItem);
            m_connectionPoints.append(startPoint);
            return true;
        }
        return !m_connectionPoints.contains(scenePos);
    } else if (m_toolType != ToolType::RequiredInterface && m_toolType != ToolType::ProvidedInterface) {
        if (!m_previewItem) {
            auto findParent = [](QGraphicsItem *baseItem) -> QGraphicsItem * {
                const QList<int> types = { AADLFunctionTypeGraphicsItem::Type, AADLFunctionGraphicsItem::Type };
                if (!baseItem || types.contains(baseItem->type()))
                    return baseItem;

                while (auto parentItem = baseItem->parentItem()) {
                    if (types.contains(parentItem->type()))
                        return parentItem;

                    baseItem = parentItem;
                }
                return nullptr;
            };

            QGraphicsItem *parentItem = findParent(m_view->itemAt(e->localPos().toPoint()));
            m_previewItem = new QGraphicsRectItem(parentItem);
            m_previewItem->setPen(QPen(Qt::blue, 2, Qt::SolidLine));
            m_previewItem->setBrush(QBrush(QColor(30, 144, 255, 90)));
            m_previewItem->setZValue(1);
            if (!parentItem)
                scene->addItem(m_previewItem);
        }
        const QPointF mappedScenePos = m_previewItem->mapFromScene(scenePos);
        m_previewItem->setRect({ mappedScenePos, mappedScenePos });
        return true;
    }

    return true;
}

bool CreatorTool::onMouseRelease(QMouseEvent *e)
{
    if (!m_view)
        return false;

    const QPointF scenePos = cursorInScene(e->globalPos());

    if (m_toolType == ToolType::Pointer) {
        if (e->button() & Qt::RightButton && e->buttons() == Qt::NoButton) {
            if (QMenu *menu = populateContextMenu(scenePos)) {
                connect(menu, &QMenu::aboutToHide, this, [this]() {
                    if (m_previewItem)
                        m_previewItem->setVisible(false);
                });
                menu->exec(e->globalPos());
                clearPreviewItem();
                return true;
            }
        }
    } else {
        handleToolType(m_toolType, scenePos);
        return true;
    }
    return false;
}

bool CreatorTool::onMouseMove(QMouseEvent *e)
{
    if (!m_view)
        return false;

    if (m_previewItem && m_previewItem->isVisible()) {
        const QPointF scenePos = m_previewItem->mapFromScene(cursorInScene(e->globalPos()));
        const QRectF newGeometry { m_previewItem->rect().topLeft(), scenePos };
        if (m_previewItem->parentItem()) {
            if (!m_previewItem->parentItem()->boundingRect().contains(newGeometry))
                return false;
        } else if (auto scene = m_view->scene()) {
            const QList<QGraphicsItem *> collidedItems = scene->items(newGeometry.normalized().marginsAdded(kMargins));
            auto it = std::find_if(
                    collidedItems.constBegin(), collidedItems.constEnd(),
                    [this](const QGraphicsItem *item) { return item != m_previewItem && !item->parentItem(); });
            if (it != collidedItems.constEnd())
                return false;
        }
        m_previewItem->setRect(newGeometry);
        return true;
    } else if (m_previewConnectionItem && m_previewConnectionItem->isVisible() && !m_connectionPoints.isEmpty()) {
        if (m_view->scene()) {
            QPainterPath pp;
            pp.addPolygon(m_connectionPoints);
            pp.lineTo(cursorInScene(e->globalPos()));
            m_previewConnectionItem->setPath(pp);
            return true;
        }
    }

    return false;
}

QPointF CreatorTool::cursorInScene() const
{
    return cursorInScene(QCursor::pos()); // TODO: add current screen detection
}

QPointF CreatorTool::cursorInScene(const QPoint &globalPos) const
{
    QPointF sceneCoordinates;
    if (m_view) {
        const QPoint viewCoordinates = m_view->mapFromGlobal(globalPos);
        sceneCoordinates = m_view->mapToScene(viewCoordinates);
    }
    return sceneCoordinates;
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
    auto ifaceItem = itemAt<AADLInterfaceGraphicsItem>(scene, point);
    if (!ifaceItem)
        return false;

    QGraphicsItem *targetItem = ifaceItem->targetItem();
    if (!targetItem)
        return false;

    switch (utils::getNearestSide(targetItem->sceneBoundingRect(), point)) {
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

static inline QRectF adjustFromPoint(const QPointF &pos, const qreal &adjustment)
{
    const QPointF adjustmentPoint { adjustment / 2, adjustment / 2 };
    return QRectF { pos - adjustmentPoint, pos + adjustmentPoint };
}

static inline AADLObjectFunction *functionObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLFunctionGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

static inline AADLObjectFunctionType *functionTypeObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLFunctionTypeGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

static inline AADLObjectIface *interfaceObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLInterfaceGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

static inline AADLObjectComment *commentObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLCommentGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

static inline AADLObjectConnection *connectionObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLConnectionGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

static inline QRectF adjustToSize(const QRectF &rect, const QSizeF &minSize)
{
    QRectF itemRect = rect;
    if (itemRect.width() < minSize.width())
        itemRect.setWidth(minSize.width());
    if (itemRect.height() < minSize.height())
        itemRect.setHeight(minSize.height());
    return itemRect;
};

void CreatorTool::handleToolType(CreatorTool::ToolType type, const QPointF &pos)
{
    if (!m_view)
        return;

    if (auto scene = m_view->scene()) {
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
            handleProvidedInterface(scene, pos);
            break;
        case ToolType::RequiredInterface:
            handleRequiredInterface(scene, pos);
            break;
        case ToolType::MultiPointConnection:
            if (!handleConnectionCreate(scene, pos))
                return;
            handleMultiPointConnection(scene, pos);
            break;
        case ToolType::DirectConnection:
            handleDirectConnection(scene, pos);
            break;
        default:
            break;
        }
        clearPreviewItem();
    }

    emit created();
}

bool CreatorTool::handleConnectionCreate(QGraphicsScene *scene, const QPointF &pos)
{
    if (!m_previewConnectionItem)
        return false;

    if (QGraphicsItem *itemUnderCursor =
                utils::nearestItem(scene, pos, kConnectionTolerance, { AADLInterfaceGraphicsItem::Type })) {
        if (m_connectionPoints.size() < 2)
            return false;

        const QPointF finishPoint = itemUnderCursor->sceneBoundingRect().center();
        if (!alignToSidePoint(scene, finishPoint, m_connectionPoints.last()))
            return false;

        m_connectionPoints.append(finishPoint);
        return true;
    }

    QPointF alignedPos { pos };
    if (m_connectionPoints.size() == 1) {
        if (!alignToSidePoint(scene, m_connectionPoints.value(0), alignedPos))
            return false;
    } else {
        QLineF currentPath { m_connectionPoints.last(), pos };
        if (utils::alignedLine(currentPath))
            alignedPos = currentPath.p2();
    }
    m_connectionPoints.append(alignedPos);

    QPainterPath pp;
    pp.addPolygon(m_connectionPoints);
    pp.lineTo(pos);
    m_previewConnectionItem->setPath(pp);
    return false;
}

void CreatorTool::handleComment(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene);
    Q_UNUSED(pos);

    if (m_previewItem) {
        const QRectF itemSceneRect =
                adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()), utils::DefaultGraphicsItemSize);
        const QVariantList params = { qVariantFromValue(m_model.data()), itemSceneRect };
        taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateCommentEntity, params));
    }
}

void CreatorTool::handleFunctionType(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene);
    Q_UNUSED(pos);

    if (m_previewItem) {
        const QRectF itemSceneRect =
                adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()), utils::DefaultGraphicsItemSize);
        AADLObjectFunction *parentObject = functionObject(m_previewItem->parentItem());
        if (m_previewItem->parentItem() && !parentObject)
            return;

        const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(parentObject),
                                      itemSceneRect };
        taste3::cmd::CommandsStack::current()->push(
                cmd::CommandsFactory::create(cmd::CreateFunctionTypeEntity, params));
    }
}

void CreatorTool::handleFunction(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene);
    Q_UNUSED(pos);

    if (m_previewItem) {
        const QRectF itemSceneRect =
                adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()), utils::DefaultGraphicsItemSize);
        AADLObjectFunction *parentObject = functionObject(m_previewItem->parentItem());
        if (m_previewItem->parentItem() && !parentObject)
            return;

        const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(parentObject),
                                      itemSceneRect };
        taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateFunctionEntity, params));
    }
}

void CreatorTool::handleProvidedInterface(QGraphicsScene *scene, const QPointF &pos)
{
    if (QGraphicsItem *parentItem =
                utils::nearestItem(scene, adjustFromPoint(pos, kInterfaceTolerance), kFunctionTypes)) {
        AADLObjectFunctionType *parentObject = functionTypeObject(parentItem);
        const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(parentObject), pos };
        taste3::cmd::CommandsStack::current()->push(
                cmd::CommandsFactory::create(cmd::CreateProvidedInterfaceEntity, params));
    }
}

void CreatorTool::handleRequiredInterface(QGraphicsScene *scene, const QPointF &pos)
{
    if (auto parentItem = utils::nearestItem(scene, adjustFromPoint(pos, kInterfaceTolerance), kFunctionTypes)) {
        AADLObjectFunctionType *parentObject = functionTypeObject(parentItem);
        const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(parentObject), pos };
        taste3::cmd::CommandsStack::current()->push(
                cmd::CommandsFactory::create(cmd::CreateRequiredInterfaceEntity, params));
    }
}

void CreatorTool::handleMultiPointConnection(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(pos);

    const QList<int> ifaceTypes { AADLInterfaceGraphicsItem::Type };
    AADLObjectFunction *startObject =
            functionObject(utils::nearestItem(scene, m_connectionPoints.first(), kInterfaceTolerance, kFunctionTypes));
    AADLObjectFunction *endObject =
            functionObject(utils::nearestItem(scene, m_connectionPoints.last(), kInterfaceTolerance, kFunctionTypes));
    if (!startObject || !endObject)
        return;

    AADLInterfaceGraphicsItem *pi = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
            utils::nearestItem(scene, m_connectionPoints.first(), kInterfaceTolerance, ifaceTypes));
    AADLInterfaceGraphicsItem *ri = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
            utils::nearestItem(scene, m_connectionPoints.last(), kInterfaceTolerance, ifaceTypes));
    if (!pi || !ri)
        return;

    if (pi->entity()->isRequired() && ri->entity()->isProvided()) {
        qSwap(startObject, endObject);
        qSwap(pi, ri);
    }
    AADLObjectIfaceProvided *providedIface = qobject_cast<AADLObjectIfaceProvided *>(pi->entity());
    AADLObjectIfaceRequired *requiredIface = qobject_cast<AADLObjectIfaceRequired *>(ri->entity());

    if (!providedIface || !requiredIface)
        return;

    const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(startObject),
                                  qVariantFromValue(endObject),      qVariantFromValue(providedIface),
                                  qVariantFromValue(requiredIface),  qVariantFromValue(m_connectionPoints) };
    taste3::cmd::CommandsStack::current()->push(
            cmd::CommandsFactory::create(cmd::CreateManualConnectionEntity, params));
}

void CreatorTool::handleDirectConnection(QGraphicsScene *scene, const QPointF &pos)
{
    if (m_connectionPoints.size() < 1)
        return;

    const QLineF connectionLine { m_connectionPoints.first(), pos };

    AADLObjectIfaceProvided *providedIface { nullptr };
    AADLObjectIfaceRequired *requiredIface { nullptr };
    QPointF providedPoint;
    QPointF requiredPoint;
    QPointF startPointAdjusted;
    QPointF endPointAdjusted;

    auto itemAtStartPos = utils::nearestItem(scene, adjustFromPoint(m_connectionPoints.first(), kInterfaceTolerance),
                                             { AADLFunctionGraphicsItem::Type });
    AADLObjectFunction *startObject = functionObject(itemAtStartPos);
    if (!startObject)
        return;

    bool requiredAtStart = false;
    if (auto startIfaceItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                utils::nearestItem(scene, adjustFromPoint(m_connectionPoints.first(), kInterfaceTolerance),
                                   { AADLInterfaceGraphicsItem::Type }))) {
        if (auto iface = startIfaceItem->entity()) {
            requiredAtStart = iface->isRequired();
            if (iface->isProvided()) {
                if (providedIface)
                    return;
                providedIface = qobject_cast<AADLObjectIfaceProvided *>(iface);
                providedPoint = startIfaceItem->scenePos();
            } else if (iface->isRequired()) {
                if (requiredIface)
                    return;
                requiredIface = qobject_cast<AADLObjectIfaceRequired *>(iface);
                requiredPoint = startIfaceItem->scenePos();
            }
        } else {
            return;
        }
    } else if (!utils::intersects(itemAtStartPos->sceneBoundingRect(), connectionLine, &startPointAdjusted)) {
        return;
    }

    auto itemAtEndPos =
            utils::nearestItem(scene, adjustFromPoint(pos, kInterfaceTolerance), { AADLFunctionGraphicsItem::Type });
    AADLObjectFunction *endObject = functionObject(itemAtEndPos);
    if (!endObject)
        return;

    if (auto endIfaceItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(utils::nearestItem(
                scene, adjustFromPoint(pos, kInterfaceTolerance), { AADLInterfaceGraphicsItem::Type }))) {
        if (auto iface = endIfaceItem->entity()) {
            requiredAtStart = iface->isProvided();
            if (iface->isProvided()) {
                if (providedIface)
                    return;
                providedIface = qobject_cast<AADLObjectIfaceProvided *>(iface);
                providedPoint = endIfaceItem->scenePos();
            } else if (iface->isRequired()) {
                if (requiredIface)
                    return;
                requiredIface = qobject_cast<AADLObjectIfaceRequired *>(iface);
                requiredPoint = endIfaceItem->scenePos();
            }
        } else {
            return;
        }
    } else if (!utils::intersects(itemAtEndPos->sceneBoundingRect(), connectionLine, &endPointAdjusted)) {
        return;
    }

    if (providedPoint.isNull())
        providedPoint = requiredAtStart ? endPointAdjusted : startPointAdjusted;
    if (requiredPoint.isNull())
        requiredPoint = requiredAtStart ? startPointAdjusted : endPointAdjusted;

    const QVariantList params = { qVariantFromValue(m_model.data()),
                                  qVariantFromValue(requiredAtStart ? endObject : startObject),
                                  qVariantFromValue(providedIface),
                                  providedPoint,
                                  qVariantFromValue(requiredAtStart ? startObject : endObject),
                                  qVariantFromValue(requiredIface),
                                  requiredPoint };
    taste3::cmd::CommandsStack::current()->push(
            cmd::CommandsFactory::create(cmd::CreateDirectConnectionEntity, params));

    m_connectionPoints.append(pos);
}

void CreatorTool::removeSelectedItems()
{
    if (!m_view)
        return;

    if (auto scene = m_view->scene()) {
        taste3::cmd::CommandsStack::current()->beginMacro(tr("Remove selected item(s)"));
        while (!scene->selectedItems().isEmpty()) {
            clearPreviewItem();

            QGraphicsItem *item = scene->selectedItems().first();
            item->setSelected(false);

            AADLObject *entity = nullptr;
            if (auto connectionItem = qgraphicsitem_cast<AADLConnectionGraphicsItem *>(item))
                entity = connectionItem->entity();
            else if (auto iObj = qobject_cast<InteractiveObject *>(item->toGraphicsObject()))
                entity = qobject_cast<AADLObject *>(iObj->modelEntity());
            else if (item->type() == QGraphicsTextItem::Type)
                continue;

            if (entity) {
                const QVariantList params = { qVariantFromValue(entity), qVariantFromValue(m_model.data()) };
                if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::RemoveEntity, params))
                    taste3::cmd::CommandsStack::current()->push(cmdRm);
            }
        }
        taste3::cmd::CommandsStack::current()->endMacro();
    }
}

void CreatorTool::clearPreviewItem()
{
    m_connectionPoints.clear();
    if (m_previewConnectionItem) {
        m_previewConnectionItem->scene()->removeItem(m_previewConnectionItem);
        delete m_previewConnectionItem;
        m_previewConnectionItem = nullptr;
    }

    if (m_previewItem) {
        m_previewItem->scene()->removeItem(m_previewItem);
        delete m_previewItem;
        m_previewItem = nullptr;
    }
}

QMenu *CreatorTool::populateContextMenu(const QPointF &scenePos)
{
    QMenu *menu = new QMenu(m_view);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    populateContextMenu_commonCreate(menu, scenePos);
    populateContextMenu_propertiesDialog(menu, scenePos);
    populateContextMenu_user(menu, scenePos);

    if (menu->isEmpty()) {
        delete menu;
        menu = nullptr;
    }

    return menu;
}

void CreatorTool::populateContextMenu_commonCreate(QMenu *menu, const QPointF &scenePos)
{
    if (m_previewItem) {

        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/function_type.svg")), tr("Function Type"),
                        this, [this, scenePos]() { handleToolType(ToolType::FunctionType, scenePos); });
        common::registerAction(Q_FUNC_INFO, menu->actions().last(), "Create FunctionType",
                               "Activate the FunctionType creation mode");

        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")), tr("Function"), this,
                        [this, scenePos]() { handleToolType(ToolType::Function, scenePos); });
        common::registerAction(Q_FUNC_INFO, menu->actions().last(), "Create Function",
                               "Activate the Function creation mode");

        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/comment.svg")), tr("Comment"), this,
                        [this, scenePos]() { handleToolType(ToolType::Comment, scenePos); });
        common::registerAction(Q_FUNC_INFO, menu->actions().last(), "Create Comment",
                               "Activate the Comment creation mode");
    }
}

void CreatorTool::populateContextMenu_propertiesDialog(QMenu *menu, const QPointF &scenePos)
{
    QGraphicsScene *scene = m_view->scene();
    if (!scene)
        return;

    static const QList<int> &showProps = { AADLInterfaceGraphicsItem::Type, AADLFunctionTypeGraphicsItem::Type,
                                           AADLFunctionGraphicsItem::Type };
    QGraphicsItem *gi = utils::nearestItem(scene, scenePos, kContextMenuItemTolerance, showProps);
    if (!gi)
        return;

    AADLObject *aadlObj { nullptr };
    switch (gi->type()) {
    case AADLFunctionTypeGraphicsItem::Type: {
        aadlObj = functionTypeObject(gi);
        break;
    }
    case AADLFunctionGraphicsItem::Type: {
        aadlObj = functionObject(gi);
        break;
    }
    case AADLInterfaceGraphicsItem::Type: {
        aadlObj = interfaceObject(gi);
        break;
    }
    default:
        return;
    }

    if (aadlObj) {
        menu->addSeparator();
        menu->addAction(tr("Properties"), this, [this, aadlObj]() { emit propertyEditorRequest(aadlObj); });
        common::registerAction(Q_FUNC_INFO, menu->actions().last(), "Properties", "Show AADL object properties editor");
    }
}

void CreatorTool::populateContextMenu_user(QMenu *menu, const QPointF &scenePos)
{
    QGraphicsScene *scene = m_view->scene();
    if (!scene)
        return;

    static const QList<int> &showProps = { AADLInterfaceGraphicsItem::Type, AADLFunctionTypeGraphicsItem::Type,
                                           AADLFunctionGraphicsItem::Type, AADLCommentGraphicsItem::Type,
                                           AADLConnectionGraphicsItem::Type };

    AADLObject *aadlObj { nullptr };
    if (QGraphicsItem *gi = utils::nearestItem(scene, scenePos, kContextMenuItemTolerance, showProps)) {

        switch (gi->type()) {
        case AADLFunctionTypeGraphicsItem::Type: {
            aadlObj = functionTypeObject(gi);
            break;
        }
        case AADLFunctionGraphicsItem::Type: {
            aadlObj = functionObject(gi);
            break;
        }
        case AADLInterfaceGraphicsItem::Type: {
            aadlObj = interfaceObject(gi);
            break;
        }
        case AADLCommentGraphicsItem::Type: {
            aadlObj = commentObject(gi);
            break;
        }
        case AADLConnectionGraphicsItem::Type: {
            aadlObj = connectionObject(gi);
            break;
        }
        default:
            return;
        }
    }

    ctx::ActionsManager::populateMenu(menu, aadlObj);
}

} // namespace aadl
} // namespace taste3
