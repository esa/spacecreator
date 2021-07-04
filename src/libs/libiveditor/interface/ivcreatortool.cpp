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

#include "ivcreatortool.h"

#include "baseitems/common/ivutils.h"
#include "commands/cmdcommentitemcreate.h"
#include "commands/cmdconnectiongroupitemcreate.h"
#include "commands/cmdconnectionitemcreate.h"
#include "commands/cmdentitiesremove.h"
#include "commands/cmdentitygeometrychange.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/cmdfunctiontypeitemcreate.h"
#include "commands/cmdinterfaceitemcreate.h"
#include "commandsstack.h"
#include "connectioncreationvalidator.h"
#include "context/action/actionsmanager.h"
#include "graphicsitemhelpers.h"
#include "graphicsviewutils.h"
#include "interface/createconnectiongroupdialog.h"
#include "interfacedocument.h"
#include "ivcomment.h"
#include "ivcommentgraphicsitem.h"
#include "ivconnection.h"
#include "ivconnectiongraphicsitem.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiongraphicsitem.h"
#include "ivfunctiontype.h"
#include "ivfunctiontypegraphicsitem.h"
#include "ivinterface.h"
#include "ivinterfacegraphicsitem.h"
#include "ivitemmodel.h"
#include "ui/grippointshandler.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QItemSelectionModel>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QPointer>
#include <QSet>
#include <QVector>
#include <QtDebug>
#include <QtMath>
#include <limits>

static const qreal kContextMenuItemTolerance = 10.;
static const QList<int> kFunctionTypes = { ive::IVFunctionGraphicsItem::Type, ive::IVFunctionTypeGraphicsItem::Type };
static const qreal kPreviewItemPenWidth = 2.;

namespace ive {

IVCreatorTool::IVCreatorTool(InterfaceDocument *doc)
    : shared::ui::CreatorTool(doc->graphicsView(), doc->itemsModel(), doc)
    , m_doc(doc)
{
}

IVCreatorTool::~IVCreatorTool() { }

void IVCreatorTool::removeSelectedItems()
{
    if (!m_view)
        return;

    if (auto scene = m_view->scene()) {
        QStringList clonedIfaces;
        QList<QPointer<ivm::IVObject>> entities;
        clearPreviewItem();
        while (!scene->selectedItems().isEmpty()) {
            QGraphicsItem *item = scene->selectedItems().first();
            item->setSelected(false);

            if (auto iObj = qobject_cast<shared::ui::VEInteractiveObject *>(item->toGraphicsObject())) {
                if (auto entity = iObj->entity() ? iObj->entity()->as<ivm::IVObject *>() : nullptr) {
                    if (entity->isRootObject()) {
                        continue;
                    }
                    if (entity->isInterface()) {
                        if (auto iface = entity->as<const ivm::IVInterface *>()) {
                            if (auto srcIface = iface->cloneOf()) {
                                clonedIfaces.append(QStringLiteral("%1's %2 is from %3")
                                                            .arg(iface->parentObject()->title(), iface->title(),
                                                                    srcIface->parentObject()->title()));
                                continue;
                            }
                        }
                    }
                    entities.append(entity);
                }
            }
        }
        auto cmdRm = new cmd::CmdEntitiesRemove(entities, model()->objectsModel());
        cmdRm->setText(tr("Remove selected item(s)"));
        m_doc->commandsStack()->push(cmdRm);

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

void IVCreatorTool::groupSelectedItems()
{
    QList<ivm::IVConnectionGroup::CreationInfo> groupCreationDataList;

    auto processConnection = [&](ivm::IVConnection *connection) {
        auto it = std::find_if(groupCreationDataList.begin(), groupCreationDataList.end(),
                [connection](const ivm::IVConnectionGroup::CreationInfo &data) {
                    const bool functionMatch = data.sourceObject->id() == connection->source()->id()
                            && data.targetObject->id() == connection->target()->id();
                    const bool functionMatchReversed = data.targetObject->id() == connection->source()->id()
                            && data.sourceObject->id() == connection->target()->id();

                    return functionMatch || functionMatchReversed;
                });

        if (it != groupCreationDataList.end()) {
            it->connections.append(connection);
        } else {
            QVector<QPointF> points = shared::graphicsviewutils::polygon(connection->coordinates());
            if (points.isEmpty()) {
                const QGraphicsItem *sourceItem = m_model->getItem(connection->source()->id());
                const QGraphicsItem *targetItem = m_model->getItem(connection->target()->id());
                if (!sourceItem || !targetItem) {
                    return;
                }

                QPointF startPoint = sourceItem->sceneBoundingRect().center();
                QPointF endPoint = targetItem->sceneBoundingRect().center();
                const bool startAdjusted = shared::graphicsviewutils::intersects(
                        sourceItem->sceneBoundingRect(), { startPoint, endPoint }, &startPoint);
                const bool endAdjusted = shared::graphicsviewutils::intersects(
                        targetItem->sceneBoundingRect(), { startPoint, endPoint }, &endPoint);
                if (!startAdjusted || !endAdjusted) {
                    return;
                }
                points << startPoint << endPoint;
            }
            groupCreationDataList.append({ model()->objectsModel(), connection->parentObject(), connection->source(),
                    connection->target(), {}, { connection }, points });
        }
    };

    for (const auto item : m_view->scene()->selectedItems()) {
        if (item->type() == IVConnectionGraphicsItem::Type) {
            if (auto connectionItem = qgraphicsitem_cast<IVConnectionGraphicsItem *>(item)) {
                if (ivm::IVConnection *connectionObj = connectionItem->entity()) {
                    processConnection(connectionObj);
                }
            }
        }
    }

    CreateConnectionGroupDialog *dialog = new CreateConnectionGroupDialog(groupCreationDataList, m_view->window());
    if (dialog->exec() == QDialog::Accepted) {
        for (const ivm::IVConnectionGroup::CreationInfo &data : dialog->info()) {
            auto cmd = new cmd::CmdConnectionGroupItemCreate(data);
            m_doc->commandsStack()->push(cmd);
        }
    }
    dialog->deleteLater();
}

// Right mouse (ctrl or not) = get the context menu with the creation choices
// Left mouse + ctrl = shortcut to creating a connection + the PI and RI between two boxes
// Left mouse without ctrl: If a tool is selected, perform the tool. Otherwise, select the items
bool IVCreatorTool::onMousePress(QMouseEvent *e)
{
    if (m_view.isNull()) {
        return false;
    }

    auto scene = m_view->scene();
    if (scene == nullptr) {
        return false;
    }

    const QPointF scenePos = cursorInScene(e->globalPos());
    if ((m_toolType == ToolType::ReCreateConnection || e->modifiers() & Qt::ShiftModifier)
            && e->button() != Qt::RightButton) {
        if (!m_previewConnectionItem) {
            QGraphicsItem *item = shared::graphicsviewutils::nearestItem(
                    scene, scenePos, shared::graphicsviewutils::kInterfaceTolerance, { IVInterfaceGraphicsItem::Type });
            if (!item || item->type() != IVInterfaceGraphicsItem::Type)
                return false;

            auto interfaceItem = qgraphicsitem_cast<const IVInterfaceGraphicsItem *>(item);
            if (!interfaceItem)
                return false;

            if (interfaceItem->connectionItems().size() != 1 || !interfaceItem->entity()->isProvided())
                return false;

            auto connectionItem = interfaceItem->connectionItems().front();
            Q_ASSERT(connectionItem);

            auto reqIface = connectionItem->startItem();
            Q_ASSERT(reqIface);

            m_toolType = ToolType::ReCreateConnection;
            m_previewConnectionItem = new QGraphicsPathItem;
            m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
            m_previewConnectionItem->setZValue(ZOrder.Preview);
            m_previewConnectionItem->setData(Qt::UserRole, connectionItem->entity()->id());
            scene->addItem(m_previewConnectionItem);
            m_connectionPoints.append(reqIface->connectionEndPoint(connectionItem));
            connectionItem->setVisible(false);
        }
        return true;
    } else if (e->modifiers() & Qt::ControlModifier) {
        auto itemAtCursor = m_view->itemAt(e->pos());
        if ((e->button() & Qt::MouseButton::LeftButton)
                && (!itemAtCursor || itemAtCursor->type() != shared::ui::GripPoint::Type))
            m_toolType = ToolType::DirectConnection;
        else
            return false;
    } else if (!(e->button() & Qt::RightButton) && m_toolType == ToolType::Pointer) {
        return false;
    }

    if (m_toolType == ToolType::DirectConnection && e->button() != Qt::RightButton) {
        if (!shared::graphicsviewutils::nearestItem(scene, scenePos, QList<int> { IVFunctionGraphicsItem::Type })) {
            if (!shared::graphicsviewutils::nearestItem(scene, scenePos, shared::graphicsviewutils::kInterfaceTolerance,
                        { IVInterfaceGraphicsItem::Type }))
                return false;
        }

        if (m_previewConnectionItem) {
            m_connectionPoints.clear();
        } else {
            m_previewConnectionItem = new QGraphicsPathItem;
            m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
            m_previewConnectionItem->setZValue(ZOrder.Preview);
            scene->addItem(m_previewConnectionItem);
        }
        m_connectionPoints.append(scenePos);
        return true;
    } else if (m_toolType == ToolType::MultiPointConnection && e->button() != Qt::RightButton) {
        if (!m_previewConnectionItem) {
            QGraphicsItem *item = shared::graphicsviewutils::nearestItem(
                    scene, scenePos, shared::graphicsviewutils::kInterfaceTolerance, { IVInterfaceGraphicsItem::Type });
            if (!item)
                return false;

            const QPointF startPoint = item->mapToScene(QPointF(0, 0));
            m_previewConnectionItem = new QGraphicsPathItem;
            m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
            m_previewConnectionItem->setZValue(ZOrder.Preview);
            scene->addItem(m_previewConnectionItem);
            m_connectionPoints.append(startPoint);
            return true;
        }
        return !m_connectionPoints.contains(scenePos);
    } else if (e->button() == Qt::RightButton
            || (m_toolType != ToolType::RequiredInterface && m_toolType != ToolType::ProvidedInterface)) {
        if (!m_previewItem) {
            QGraphicsItem *parentItem = m_view->itemAt(e->pos());
            while (parentItem != nullptr && parentItem->type() != IVFunctionGraphicsItem::Type
                    && parentItem->type() != IVFunctionTypeGraphicsItem::Type) {
                parentItem = parentItem->parentItem();
            }
            m_previewItem = new QGraphicsRectItem(parentItem);
            m_previewItem->setPen(QPen(Qt::blue, kPreviewItemPenWidth, Qt::SolidLine));
            m_previewItem->setBrush(QBrush(QColor(30, 144, 255, 90)));
            m_previewItem->setZValue(ZOrder.Preview);
            m_clickScenePos = scenePos;

            if (!parentItem)
                scene->addItem(m_previewItem);

            if (!e->buttons().testFlag(Qt::MaxMouseButton)) {
                auto items = m_view->items(e->pos());
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
        const QPointF mappedScenePos = m_previewItem->mapFromScene(scenePos);
        m_previewItem->setRect({ mappedScenePos, mappedScenePos });
        return true;
    }

    return true;
}

bool IVCreatorTool::onMouseRelease(QMouseEvent *e)
{
    if (!m_view)
        return false;

    if ((e->button() & Qt::RightButton) && m_previewItem) {
        return showContextMenu(e->globalPos());
    } else if (m_toolType != ToolType::Pointer) {
        const bool hasPreview = m_previewItem || m_previewConnectionItem;
        const bool isIface = m_toolType == ToolType::ProvidedInterface || m_toolType == ToolType::RequiredInterface;
        if (hasPreview || isIface) {
            const QPointF &scenePos = cursorInScene(e->globalPos());
            return handleToolType(m_toolType, scenePos);
        }
    }
    return false;
}

bool IVCreatorTool::onMouseMove(QMouseEvent *e)
{
    if (!m_view || !m_view->scene())
        return false;

    const QPointF &scenePos = cursorInScene(e->globalPos());
    if (m_previewItem && m_previewItem->isVisible()) {
        const QRectF newGeometry = QRectF(m_clickScenePos, scenePos).normalized();
        if (!newGeometry.isValid())
            return true;

        QSet<shared::ui::VEInteractiveObject *> items;
        const QRectF expandedGeometry { newGeometry.marginsAdded(shared::graphicsviewutils::kContentMargins) };
        QList<QGraphicsItem *> newCollidedItems = m_view->scene()->items(expandedGeometry);
        std::for_each(newCollidedItems.begin(), newCollidedItems.end(),
                [this, &items, expandedGeometry](QGraphicsItem *item) {
                    if (item->type() == IVInterfaceGraphicsItem::Type || item->type() == m_previewItem->type())
                        return;

                    auto iObjItem = qobject_cast<shared::ui::VEInteractiveObject *>(item->toGraphicsObject());
                    if (!iObjItem)
                        return;

                    if (item->parentItem() == m_previewItem->parentItem()
                            || (m_previewItem->parentItem() == item
                                    && !item->sceneBoundingRect().contains(expandedGeometry))) {
                        items.insert(iObjItem);
                    }
                });
        QSet<shared::ui::VEInteractiveObject *> newItems(items);
        newItems.subtract(m_collidedItems);
        for (auto item : newItems)
            item->doHighlighting(Qt::red, true);

        QSet<shared::ui::VEInteractiveObject *> oldItems(m_collidedItems);
        oldItems.subtract(items);

        for (auto item : oldItems)
            item->doHighlighting(Qt::green, false);

        m_collidedItems = items;
        m_previewItem->setRect(m_previewItem->mapRectFromScene(newGeometry));
        return true;
    } else if (m_previewConnectionItem && m_previewConnectionItem->isVisible() && !m_connectionPoints.isEmpty()) {
        if (m_view->scene()) {
            QPainterPath pp;
            pp.addPolygon(m_connectionPoints);
            pp.lineTo(scenePos);
            m_previewConnectionItem->setPath(pp);
            warnConnectionPreview(scenePos);
            return true;
        }
    }

    return false;
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

void IVCreatorTool::populateContextMenu_commonCreate(QMenu *menu, const QPointF &scenePos)
{
    if (m_previewItem) {
        static const QSizeF emptyPreviewItemSize = QSizeF(kPreviewItemPenWidth, kPreviewItemPenWidth);
        const bool isRect = m_previewItem->boundingRect().size() != emptyPreviewItemSize;
        // TODO: use a Fn/FnType/Comment's min size to disable related actions if the creation is impossible?

        auto action = menu->addAction(QIcon(QLatin1String(":/toolbar/icns/function_type.svg")), tr("Function Type"),
                this, [this, scenePos]() { handleToolType(ToolType::FunctionType, scenePos); });

        action = menu->addAction(QIcon(QLatin1String(":/toolbar/icns/function.svg")), tr("Function"), this,
                [this, scenePos]() { handleToolType(ToolType::Function, scenePos); });

        action = menu->addAction(QIcon(QLatin1String(":/toolbar/icns/comment.svg")), tr("Comment"), this,
                [this, scenePos]() { handleToolType(ToolType::Comment, scenePos); });

        action = menu->addAction(QIcon(QLatin1String(":/toolbar/icns/ri.svg")), tr("Required Interface"), this,
                [this, scenePos]() { handleToolType(ToolType::RequiredInterface, scenePos); });
        action->setEnabled(!isRect && m_previewItem->parentItem());

        action = menu->addAction(QIcon(QLatin1String(":/toolbar/icns/pi.svg")), tr("Provided Interface"), this,
                [this, scenePos]() { handleToolType(ToolType::ProvidedInterface, scenePos); });
        action->setEnabled(!isRect && m_previewItem->parentItem());

        action = menu->addAction(QIcon(QLatin1String(":/toolbar/icns/connection_group.svg")), tr("Connection group"),
                this, [this]() { groupSelectedItems(); });
        const auto selectedItems = m_previewItem->scene()->selectedItems();
        const auto it = std::find_if(selectedItems.cbegin(), selectedItems.cend(),
                [](const QGraphicsItem *item) { return item->type() == IVConnectionGraphicsItem::Type; });
        action->setEnabled(it != selectedItems.cend());
    }
}

void IVCreatorTool::populateContextMenu_user(QMenu *menu, const QPointF &scenePos)
{
    QGraphicsScene *scene = m_view->scene();
    if (!scene)
        return;

    static const QList<int> showProps { IVInterfaceGraphicsItem::Type, IVFunctionTypeGraphicsItem::Type,
        IVFunctionGraphicsItem::Type, IVCommentGraphicsItem::Type, IVConnectionGraphicsItem::Type };

    ivm::IVObject *ivObj { nullptr };
    if (QGraphicsItem *gi = scene->selectedItems().size() == 1
                    ? scene->selectedItems().first()
                    : shared::graphicsviewutils::nearestItem(scene, scenePos, kContextMenuItemTolerance, showProps)) {

        switch (gi->type()) {
        case IVFunctionTypeGraphicsItem::Type: {
            ivObj = gi::functionTypeObject(gi);
            break;
        }
        case IVFunctionGraphicsItem::Type: {
            ivObj = gi::functionObject(gi);
            break;
        }
        case IVInterfaceGraphicsItem::Type: {
            ivObj = gi::interfaceObject(gi);
            break;
        }
        case IVCommentGraphicsItem::Type: {
            ivObj = gi::commentObject(gi);
            break;
        }
        case IVConnectionGraphicsItem::Type: {
            ivObj = gi::connectionObject(gi);
            break;
        }
        default:
            return;
        }
    }

    ActionsManager::populateMenu(menu, ivObj, m_doc);
}

bool IVCreatorTool::handleToolType(int type, const QPointF &pos)
{
    if (!m_view)
        return false;

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
            handleInterface(scene, ivm::IVInterface::InterfaceType::Provided, pos);
            break;
        case ToolType::RequiredInterface:
            handleInterface(scene, ivm::IVInterface::InterfaceType::Required, pos);
            break;
        case ToolType::MultiPointConnection:
            if (!handleConnectionCreate(pos))
                return false;
            handleConnection(m_connectionPoints);
            break;
        case ToolType::DirectConnection:
            handleDirectConnection(pos);
            break;
        case ToolType::ReCreateConnection:
            if (!handleConnectionCreate(pos))
                return false;
            handleConnectionReCreate(m_connectionPoints);
            break;
        default:
            break;
        }
        clearPreviewItem();
    }

    Q_EMIT created();
    return true;
}

IVItemModel *IVCreatorTool::model() const
{
    return qobject_cast<IVItemModel *>(m_model);
}

void IVCreatorTool::handleComment(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene)
    Q_UNUSED(pos)

    if (m_previewItem) {
        ivm::IVFunctionType *parentObject = gi::functionObject(m_previewItem->parentItem());
        if (!parentObject)
            parentObject = gi::functionTypeObject(m_previewItem->parentItem());

        QRectF itemSceneRect = adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()),
                shared::graphicsviewutils::kDefaultGraphicsItemSize);
        if (auto parentItem = m_previewItem->parentItem()) {
            if (!parentItem->sceneBoundingRect()
                            .marginsRemoved(shared::graphicsviewutils::kRootMargins)
                            .contains(itemSceneRect)) {
                itemSceneRect = QRectF();
            }
        }
        auto cmd = new cmd::CmdCommentItemCreate(model()->objectsModel(), parentObject, itemSceneRect);
        m_doc->commandsStack()->push(cmd);
    }
}

void IVCreatorTool::handleFunctionType(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene)
    Q_UNUSED(pos)

    if (m_previewItem) {
        QRectF itemSceneRect = adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()),
                shared::graphicsviewutils::kDefaultGraphicsItemSize);

        if (!shared::graphicsviewutils::isBounded(m_previewItem, itemSceneRect))
            return;

        if (auto parentItem = m_previewItem->parentItem()) {
            if (!parentItem->sceneBoundingRect()
                            .marginsRemoved(shared::graphicsviewutils::kRootMargins)
                            .contains(itemSceneRect)) {
                itemSceneRect = QRectF();
            }
        }

        ivm::IVFunction *parentObject = gi::functionObject(m_previewItem->parentItem());
        const shared::Id id = shared::createId();
        auto cmd = new cmd::CmdFunctionTypeItemCreate(model()->objectsModel(), parentObject, itemSceneRect, id);
        if (m_doc->commandsStack()->push(cmd)) {
            Q_EMIT functionCreated(id);
        }
    }
}

void IVCreatorTool::handleFunction(QGraphicsScene *scene, const QPointF &pos)
{
    Q_UNUSED(scene)
    Q_UNUSED(pos)

    if (m_previewItem) {
        QRectF itemSceneRect = adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()),
                shared::graphicsviewutils::kDefaultGraphicsItemSize);

        if (!shared::graphicsviewutils::isBounded(m_previewItem, itemSceneRect))
            return;

        if (auto parentItem = m_previewItem->parentItem()) {
            if (!parentItem->sceneBoundingRect()
                            .marginsRemoved(shared::graphicsviewutils::kRootMargins)
                            .contains(itemSceneRect)) {
                itemSceneRect = QRectF();
            }
        }

        ivm::IVFunction *parentObject = gi::functionObject(m_previewItem->parentItem());
        const shared::Id id = shared::createId();
        auto cmd = new cmd::CmdFunctionItemCreate(model()->objectsModel(), parentObject, itemSceneRect, QString(), id);
        if (m_doc->commandsStack()->push(cmd)) {
            Q_EMIT functionCreated(id);
        }
    }
}

void IVCreatorTool::handleInterface(QGraphicsScene *scene, ivm::IVInterface::InterfaceType type, const QPointF &pos)
{
    if (auto parentItem = shared::graphicsviewutils::nearestItem(scene,
                shared::graphicsviewutils::adjustFromPoint(pos, shared::graphicsviewutils::kInterfaceTolerance),
                kFunctionTypes)) {
        ivm::IVFunctionType *parentObject = gi::functionTypeObject(parentItem);
        ivm::IVInterface::CreationInfo ifaceDescr(model()->objectsModel(), parentObject, pos, type, shared::createId());
        ifaceDescr.resetKind();

        if (auto cmd = createInterfaceCommand(ifaceDescr))
            m_doc->commandsStack()->push(cmd);
    }
}

bool IVCreatorTool::handleConnectionCreate(const QPointF &pos)
{
    QGraphicsScene *scene = m_view ? m_view->scene() : nullptr;
    if (!scene)
        return false;

    if (!m_previewConnectionItem)
        return false;

    if (auto itemUnderCursor = qgraphicsitem_cast<IVInterfaceGraphicsItem *>(shared::graphicsviewutils::nearestItem(
                scene, pos, shared::graphicsviewutils::kInterfaceTolerance, { IVInterfaceGraphicsItem::Type }))) {
        const QPointF finishPoint = itemUnderCursor->connectionEndPoint();
        if (!itemUnderCursor->ifaceShape().boundingRect().contains(m_connectionPoints.front())) {
            m_connectionPoints.append(finishPoint);
            return true;
        }
    } else {
        m_connectionPoints.append(pos);
    }

    QPainterPath pp;
    pp.addPolygon(m_connectionPoints);
    pp.lineTo(pos);
    m_previewConnectionItem->setPath(pp);
    return false;
}

void IVCreatorTool::handleDirectConnection(const QPointF &pos)
{
    if (m_connectionPoints.size() < 1)
        return;

    m_connectionPoints.append(pos);

    handleConnection(m_connectionPoints);
}

void IVCreatorTool::handleConnection(const QVector<QPointF> &graphicPoints) const
{
    const auto info = ive::gi::validateConnectionCreate(m_view ? m_view->scene() : nullptr, graphicPoints);
    if (info.failed())
        return;

    const IVFunctionGraphicsItem *parentForConnection = nullptr;
    QPointF startInterfacePoint { info.startPointAdjusted };
    QPointF endInterfacePoint { info.endPointAdjusted };
    ivm::IVInterface::CreationInfo ifaceCommons;
    cmd::CommandsStack::Macro cmdMacro(m_doc->undoStack(), tr("Create connection"));

    if (info.startIface && !info.endIface) {
        ifaceCommons = ivm::IVInterface::CreationInfo::fromIface(info.startIface, info.endObject);
        ifaceCommons.position = info.endPointAdjusted;
        ifaceCommons.type =
                info.isToOrFromNested ? info.startIface->direction() : ivm::IVInterface::InterfaceType::Provided;
        ifaceCommons.id = info.endIfaceId;
        ifaceCommons.resetKind();

        if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
            return;
    } else if (info.endIface && !info.startIface) {
        ifaceCommons = ivm::IVInterface::CreationInfo::fromIface(info.endIface, info.startObject);
        ifaceCommons.position = info.startPointAdjusted;
        ifaceCommons.type =
                info.isToOrFromNested ? info.endIface->direction() : ivm::IVInterface::InterfaceType::Required;
        ifaceCommons.id = info.startIfaceId;
        ifaceCommons.resetKind();

        if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
            return;
    } else if (!info.startIface && !info.endIface) {
        ifaceCommons.model = model()->objectsModel();

        ifaceCommons.function = info.startObject;
        ifaceCommons.position = info.startPointAdjusted;
        ifaceCommons.id = info.startIfaceId;
        ifaceCommons.type = info.isToOrFromNested && graphicPoints.last() == info.connectionPoints.first()
                ? ivm::IVInterface::InterfaceType::Provided
                : ivm::IVInterface::InterfaceType::Required;
        ifaceCommons.resetKind();

        if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
            return;

        ifaceCommons.function = info.endObject;
        ifaceCommons.position = info.endPointAdjusted;
        ifaceCommons.id = info.endIfaceId;
        if (!info.isToOrFromNested) {
            ifaceCommons.type = ifaceCommons.type == ivm::IVInterface::InterfaceType::Provided
                    ? ivm::IVInterface::InterfaceType::Required
                    : ivm::IVInterface::InterfaceType::Provided;
        }
        ifaceCommons.resetKind();

        if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
            return;
    } else {
        ivm::IVInterface *pi =
                ivm::IVConnection::selectIface<ivm::IVInterfaceProvided *>(info.startIface, info.endIface);
        if (!pi)
            pi = info.startIface;
        ifaceCommons = ivm::IVInterface::CreationInfo::fromIface(pi);
        ifaceCommons.resetKind();
        ifaceCommons.name.clear();
    }

    IVFunctionGraphicsItem *prevStartItem = qgraphicsitem_cast<ive::IVFunctionGraphicsItem *>(info.functionAtStartPos);
    QPointF firstExcludedPoint = *std::next(info.connectionPoints.constBegin());
    shared::Id prevStartIfaceId = info.startIfaceId;
    while (auto item = qgraphicsitem_cast<ive::IVFunctionGraphicsItem *>(prevStartItem->parentItem())) {
        if (item == info.functionAtEndPos && info.endIface) {
            parentForConnection = item;
            break;
        }
        const QRectF rect = item->sceneBoundingRect();
        const QVector<QPointF> intersectionPoints =
                shared::graphicsviewutils::intersectionPoints(rect, info.connectionPoints);
        if (intersectionPoints.isEmpty() || intersectionPoints.size() % 2 == 0) {
            parentForConnection = item;
            break;
        }

        auto beginIt = std::find(info.connectionPoints.cbegin(), info.connectionPoints.cend(), firstExcludedPoint);
        auto endIt = std::find_if(
                beginIt, info.connectionPoints.cend(), [&rect](const QPointF &point) { return !rect.contains(point); });
        QVector<QPointF> points { startInterfacePoint };
        std::copy(beginIt, endIt, std::back_inserter(points));
        points.append(intersectionPoints.last());

        ifaceCommons.type = info.startIface
                ? info.startIface->direction()
                : (graphicPoints.last() == info.connectionPoints.first() ? ivm::IVInterface::InterfaceType::Provided
                                                                         : ivm::IVInterface::InterfaceType::Required);
        if (item == info.functionAtEndPos) {
            ifaceCommons.id = info.endIfaceId;
        } else {
            ifaceCommons.function = item->entity();
            ifaceCommons.position = intersectionPoints.last();
            ifaceCommons.id = shared::createId();

            if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
                return;
        }

        auto cmd = new cmd::CmdConnectionItemCreate(
                model()->objectsModel(), item->entity(), prevStartIfaceId, ifaceCommons.id, points);
        if (!cmdMacro.push(cmd))
            return;

        firstExcludedPoint = endIt != points.constEnd() ? *endIt : QPointF();
        startInterfacePoint = intersectionPoints.last();
        prevStartItem = item;
        prevStartIfaceId = ifaceCommons.id;
    }

    QPointF lastExcludedPoint = *std::next(info.connectionPoints.crbegin());
    auto prevEndItem = qgraphicsitem_cast<ive::IVFunctionGraphicsItem *>(info.functionAtEndPos);
    shared::Id prevEndIfaceId = info.endIfaceId;
    while (auto item = qgraphicsitem_cast<ive::IVFunctionGraphicsItem *>(prevEndItem->parentItem())) {
        if (item == info.functionAtStartPos && info.startIface) {
            Q_ASSERT(parentForConnection == item || parentForConnection == nullptr);
            parentForConnection = item;
            break;
        }
        const QRectF rect = item->sceneBoundingRect();
        const auto intersectionPoints = shared::graphicsviewutils::intersectionPoints(rect, info.connectionPoints);
        if (intersectionPoints.isEmpty() || intersectionPoints.size() % 2 == 0) {
            Q_ASSERT(parentForConnection == item || parentForConnection == nullptr);
            parentForConnection = item;
            break;
        }
        auto beginIt = std::find(info.connectionPoints.crbegin(), info.connectionPoints.crend(), lastExcludedPoint);
        auto endIt = std::find_if(beginIt, info.connectionPoints.crend(),
                [&rect](const QPointF &point) { return !rect.contains(point); });
        QVector<QPointF> points { endInterfacePoint };
        std::copy(beginIt, endIt, std::back_inserter(points));
        points.append(intersectionPoints.first());

        if (item == info.functionAtStartPos) {
            ifaceCommons.id = info.startIfaceId;
        } else {
            ifaceCommons.id = shared::createId();

            ifaceCommons.function = item->entity();
            ifaceCommons.position = intersectionPoints.first();

            ifaceCommons.type = info.endIface ? info.endIface->direction()
                                              : (graphicPoints.last() == info.connectionPoints.first()
                                                              ? ivm::IVInterface::InterfaceType::Required
                                                              : ivm::IVInterface::InterfaceType::Provided);

            if (!cmdMacro.push(createInterfaceCommand(ifaceCommons)))
                return;
        }

        auto cmd = new cmd::CmdConnectionItemCreate(
                model()->objectsModel(), item->entity(), prevEndIfaceId, ifaceCommons.id, points);
        if (!cmdMacro.push(cmd))
            return;

        lastExcludedPoint = endIt != info.connectionPoints.crend() ? *endIt : QPointF();
        endInterfacePoint = intersectionPoints.first();
        prevEndItem = item;
        prevEndIfaceId = ifaceCommons.id;
    }

    auto beginIt = std::find(info.connectionPoints.cbegin(), info.connectionPoints.cend(), firstExcludedPoint);
    auto endIt = std::find(beginIt, info.connectionPoints.cend(), lastExcludedPoint);
    QVector<QPointF> resultPoints { startInterfacePoint };
    if (endIt != info.connectionPoints.cend()) {
        std::advance(endIt, 1);
        std::copy(beginIt, endIt, std::back_inserter(resultPoints));
    }
    resultPoints.append(endInterfacePoint);
    Q_ASSERT(resultPoints.size() >= 2);
    if (resultPoints.first() != resultPoints.last()) {
        auto cmd = new cmd::CmdConnectionItemCreate(model()->objectsModel(),
                parentForConnection ? parentForConnection->entity() : nullptr, prevStartIfaceId, prevEndIfaceId,
                resultPoints);
        if (!cmdMacro.push(cmd))
            return;
    }

    cmdMacro.setComplete(true);
}

void IVCreatorTool::handleConnectionReCreate(const QVector<QPointF> &graphicPoints)
{
    m_toolType = ToolType::Pointer;
    const auto info = ive::gi::validateConnectionCreate(m_view ? m_view->scene() : nullptr, graphicPoints);
    if (info.status != ivm::ConnectionCreationValidator::FailReason::MulticastDisabled
            && info.status != ivm::ConnectionCreationValidator::FailReason::NotFail) {
        return;
    }
    const shared::Id id = m_previewConnectionItem->data(Qt::UserRole).toUuid();
    if (id.isNull()) {
        return;
    }

    if (auto item = m_model->getItem(id)) {
        if (auto connection = qgraphicsitem_cast<IVConnectionGraphicsItem *>(item)) {
            if (connection->startItem()->entity()->id() != info.startIfaceId
                    || connection->endItem()->entity()->id() != info.endIfaceId) {
                return;
            }

            QList<QPair<shared::VEObject *, QVector<QPointF>>> paramsList { { connection->entity(),
                    info.connectionPoints } };
            auto cmd = new shared::cmd::CmdEntityGeometryChange(paramsList);
            m_doc->commandsStack()->push(cmd);
        }
    }
}

bool IVCreatorTool::warnConnectionPreview(const QPointF &pos)
{
    QVector<QPointF> connectionPoints = m_connectionPoints;
    if (connectionPoints.size() > 1)
        connectionPoints.replace(connectionPoints.size() - 1, pos);
    else
        connectionPoints.append(pos);

    auto info = ive::gi::validateConnectionCreate(m_view ? m_view->scene() : nullptr, connectionPoints);
    bool warn = true;
    if (m_toolType == ToolType::ReCreateConnection) {
        if ((info.status != ivm::ConnectionCreationValidator::FailReason::MulticastDisabled
                    && info.status != ivm::ConnectionCreationValidator::FailReason::NotFail)
                || !info.endIface || !info.startIface) {
            warn = true;
        } else {
            auto startItem = qgraphicsitem_cast<IVInterfaceGraphicsItem *>(m_model->getItem(info.startIfaceId));
            auto endItem = qgraphicsitem_cast<IVInterfaceGraphicsItem *>(m_model->getItem(info.endIfaceId));
            if (!startItem || !endItem) {
                warn = true;
            } else {
                warn = !startItem->ifaceShape().boundingRect().contains(connectionPoints.first())
                        && endItem->ifaceShape().boundingRect().contains(connectionPoints.last());
            }
        }
    } else {
        warn = info.failed();
    }
    if (m_previewConnectionItem) {
        QPen p = m_previewConnectionItem->pen();
        p.setColor(warn ? Qt::red : Qt::black);
        m_previewConnectionItem->setPen(p);
    }

    return warn;
}

QUndoCommand *IVCreatorTool::createInterfaceCommand(const ivm::IVInterface::CreationInfo &info) const
{
    if (!info.function)
        return nullptr;

    if (info.function->isFunction()) {
        if (auto fn = info.function->as<const ivm::IVFunction *>()) {
            if (const ivm::IVFunctionType *fnType = fn->instanceOf()) {
                const QString message = tr("Can't add interface directly in <b>%1</b>.<br>"
                                           "Please edit the related <b>%2</b> instead.")
                                                .arg(fn->titleUI(), fnType->titleUI());
                Q_EMIT informUser(tr("Interface adding"), message);
                return nullptr;
            }
        }
    }
    if (info.type == ivm::IVInterface::InterfaceType::Provided) {
        Q_EMIT propertyEditorRequest(info.id);
    }

    return new cmd::CmdInterfaceItemCreate(info);
}

}
