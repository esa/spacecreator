/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvcreatortool.h"

#include "abstractsystemchecks.h"
#include "commands/cmdconnectionentitycreate.h"
#include "commands/cmdentitiesremove.h"
#include "commands/cmdentitygeometrychange.h"
#include "commands/cmdfunctionbind.h"
#include "commands/cmdfunctionunbind.h"
#include "commands/cmdpartitionentitycreate.h"
#include "commandsstackbase.h"
#include "connectionvalidator.h"
#include "dveditorcore.h"
#include "dvfunction.h"
#include "graphicsviewutils.h"
#include "itemeditor/dvconnectiongraphicsitem.h"
#include "itemeditor/dvdevicegraphicsitem.h"
#include "itemeditor/dvnodegraphicsitem.h"
#include "itemeditor/dvpartitiongraphicsitem.h"
#include "itemeditor/graphicsitemhelpers.h"
#include "ui/veconnectiongraphicsitem.h"
#include "ui/veinteractiveobject.h"
#include "veobject.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <QPointer>

static const qreal kContextMenuItemTolerance = 10.;
static const qreal kPreviewItemPenWidth = 2.;

namespace dve {

DVCreatorTool::DVCreatorTool(QGraphicsView *view, DVItemModel *model, DVEditorCore *dvCore, QObject *parent)
    : shared::ui::CreatorTool(view, model, parent)
    , m_dvCore(dvCore)
{
}

DVCreatorTool::~DVCreatorTool() {}

void DVCreatorTool::removeSelectedItems()
{
    if (!m_view)
        return;

    if (auto scene = m_view->scene()) {
        QList<QPointer<dvm::DVObject>> entities;
        clearPreviewItem();
        while (!scene->selectedItems().isEmpty()) {
            QGraphicsItem *item = scene->selectedItems().first();
            item->setSelected(false);

            if (auto iObj = qobject_cast<shared::ui::VEInteractiveObject *>(item->toGraphicsObject())) {
                if (auto entity = iObj->entity() ? iObj->entity()->as<dvm::DVObject *>() : nullptr) {
                    entities.append(entity);
                }
            }
        }
        auto cmdRm = new cmd::CmdEntitiesRemove(entities, model()->objectsModel());
        cmdRm->setText(tr("Remove selected item(s)"));
        m_dvCore->commandsStack()->push(cmdRm);
    }
}

bool DVCreatorTool::handleToolType(int type, const QPointF &pos)
{
    if (!m_view)
        return false;

    if (auto scene = m_view->scene()) {
        switch (type) {
        case ToolType::Partition:
            handlePartition(pos);
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

void DVCreatorTool::handlePartition(const QPointF &pos)
{
    if (m_previewItem) {
        QGraphicsItem *parentItem = m_previewItem->parentItem();
        if (!parentItem || parentItem->type() != DVNodeGraphicsItem::Type) {
            return;
        }

        QRectF itemSceneRect = m_previewItem->mapRectToScene(m_previewItem->rect());

        if (!shared::graphicsviewutils::isBounded(m_previewItem, itemSceneRect))
            return;

        auto nodeItem = qgraphicsitem_cast<DVNodeGraphicsItem *>(parentItem);
        if (!nodeItem) {
            return;
        }
        if (!parentItem->sceneBoundingRect()
                        .marginsRemoved(shared::graphicsviewutils::kRootMargins)
                        .contains(itemSceneRect)) {
            itemSceneRect = QRectF();
        }

        dvm::DVNode *node = nodeItem->entity();
        auto cmd = new cmd::CmdPartitionEntityCreate(model()->objectsModel(), node, itemSceneRect);
        m_dvCore->commandsStack()->push(cmd);
    }
}

DVItemModel *DVCreatorTool::model() const
{
    return qobject_cast<DVItemModel *>(m_model);
}

bool DVCreatorTool::onMousePress(QMouseEvent *e)
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
        return prepareReCreateConnectionPreview(e);
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
        return prepareDirectConnectionPreview(e);
    } else if (m_toolType == ToolType::MultiPointConnection && e->button() != Qt::RightButton) {
        return prepareMultiPointConnectionPreview(e);
    } else if (e->button() == Qt::RightButton || (m_toolType == ToolType::Partition)) {
        return prepareRectPreview(e);
    }

    return true;
}

bool DVCreatorTool::onMouseRelease(QMouseEvent *e)
{
    if (!m_view)
        return false;

    if ((e->button() & Qt::RightButton) && m_previewItem) {
        return showContextMenu(e->globalPos());
    } else if (m_toolType != ToolType::Pointer) {
        const bool hasPreview = m_previewItem || m_previewConnectionItem;
        if (hasPreview) {
            const QPointF &scenePos = cursorInScene(e->globalPos());
            return handleToolType(m_toolType, scenePos);
        }
    }
    return false;
}

bool DVCreatorTool::onMouseMove(QMouseEvent *e)
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
                    if (item->type() == DVDeviceGraphicsItem::Type || item->type() == m_previewItem->type())
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

bool DVCreatorTool::warnConnectionPreview(const QPointF &pos)
{
    QVector<QPointF> connectionPoints = m_connectionPoints;
    if (connectionPoints.size() > 1)
        connectionPoints.replace(connectionPoints.size() - 1, pos);
    else
        connectionPoints.append(pos);

    auto info = dve::gi::validateConnectionCreate(m_view ? m_view->scene() : nullptr, connectionPoints);
    bool warn = true;
    if (m_toolType == ToolType::ReCreateConnection) {
        if ((info.status != dvm::ConnectionValidator::FailReason::NotFail) || info.endDeviceId.isNull()
                || info.startDeviceId.isNull()) {
            warn = true;
        } else {
            auto startItem = qgraphicsitem_cast<DVDeviceGraphicsItem *>(m_model->getItem(info.startDeviceId));
            auto endItem = qgraphicsitem_cast<DVDeviceGraphicsItem *>(m_model->getItem(info.endDeviceId));
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

void DVCreatorTool::populateContextMenu_commonCreate(QMenu *menu, const QPointF &scenePos)
{
    if (m_previewItem) {
        static const QSizeF emptyPreviewItemSize = QSizeF(kPreviewItemPenWidth, kPreviewItemPenWidth);
        const auto selectedItems = m_previewItem->scene()->selectedItems();
        auto action = menu->addAction(QIcon(QLatin1String(":/toolbar/icns/partition.svg")), tr("Partition"), this,
                [this, scenePos]() { handleToolType(ToolType::Partition, scenePos); });
        auto it = std::find_if(selectedItems.cbegin(), selectedItems.cend(),
                [](const QGraphicsItem *item) { return item->type() == DVNodeGraphicsItem::Type; });
        action->setEnabled(it != selectedItems.cend());

        action = menu->addAction(QIcon(QLatin1String(":/toolbar/icns/connection.svg")), tr("Re-create Connection"),
                this, [this, scenePos]() { handleToolType(ToolType::ReCreateConnection, scenePos); });

        it = std::find_if(selectedItems.cbegin(), selectedItems.cend(),
                [](const QGraphicsItem *item) { return item->type() == DVConnectionGraphicsItem::Type; });
        action->setEnabled(it != selectedItems.cend());
    }
}

void DVCreatorTool::populateContextMenu_user(QMenu *menu, const QPointF &scenePos)
{
    /// TODO:
}

void DVCreatorTool::populateContextMenu_commonEdit(QMenu *menu, const QPointF &scenePos)
{
    CreatorTool::populateContextMenu_commonEdit(menu, scenePos);
    if (!m_dvCore->commandsStack()) {
        return;
    }
    QGraphicsItem *item = m_view->scene()->itemAt(scenePos, m_view->transform());
    if (!item || item->type() != DVPartitionGraphicsItem::Type) {
        return;
    }
    auto partitionItem = qgraphicsitem_cast<DVPartitionGraphicsItem *>(item);
    if (!partitionItem) {
        return;
    }
    dvm::DVPartition *partition = partitionItem->entity();
    if (!partition) {
        return;
    }

    if (!m_dvCore->systemChecker()) {
        return;
    }

    QMenu *bindingMenu = menu->addMenu(tr("Bindings"));

    const QStringList allFunctions = m_dvCore->systemChecker()->terminalFunctionsNames();
    const QStringList systemFunctions = m_dvCore->systemChecker()->pseudoFunctionsNames();
    const QVector<dvm::DVFunction *> allBoundFunctionsEntities =
            model()->objectsModel()->allObjectsByType<dvm::DVFunction>();
    QStringList nonboundFunctions;
    for (const QString &fnName : qAsConst(allFunctions)) {
        auto it = std::find_if(allBoundFunctionsEntities.cbegin(), allBoundFunctionsEntities.cend(),
                [fnName](const dvm::DVFunction *fn) { return fn->title() == fnName; });
        if (it == allBoundFunctionsEntities.cend() && !systemFunctions.contains(fnName)) {
            nonboundFunctions.append(fnName);
        }
    }
    QAction *action = bindingMenu->addAction(tr("Bind All"));
    QMenu *bindMenu = bindingMenu->addMenu(tr("Bind"));
    if (nonboundFunctions.isEmpty()) {
        action->setEnabled(false);
        bindMenu->setEnabled(false);
    } else {
        connect(action, &QAction::triggered, this, [this, partition, nonboundFunctions, cmdTitle = action->text()]() {
            m_dvCore->commandsStack()->undoStack()->beginMacro(cmdTitle);
            for (const QString &fn : qAsConst(nonboundFunctions)) {
                auto cmd = new cmd::CmdFunctionBind(partition, fn, nullptr);
                m_dvCore->commandsStack()->push(cmd);
            }
            m_dvCore->commandsStack()->undoStack()->endMacro();
        });
        for (const QString &functionName : qAsConst(nonboundFunctions)) {
            QAction *functionAction = bindMenu->addAction(functionName);
            connect(functionAction, &QAction::triggered, this, [this, partition, functionName]() {
                auto cmd = new cmd::CmdFunctionBind(partition, functionName, m_dvCore);
                m_dvCore->commandsStack()->push(cmd);
            });
        }
    }

    const QList<dvm::DVFunction *> boundFunctionsEntities = partition->functions();
    action = bindingMenu->addAction(tr("Unbind All"));
    QMenu *unbindMenu = bindingMenu->addMenu(tr("Unbind"));
    if (boundFunctionsEntities.isEmpty()) {
        action->setEnabled(false);
        unbindMenu->setEnabled(false);
    } else {
        connect(action, &QAction::triggered, this,
                [this, boundFunctionsEntities, partition, cmdTitle = action->text()]() {
                    m_dvCore->commandsStack()->undoStack()->beginMacro(cmdTitle);
                    for (dvm::DVFunction *fn : qAsConst(boundFunctionsEntities)) {
                        auto cmd = new cmd::CmdFunctionUnbind(partition, fn, nullptr);
                        m_dvCore->commandsStack()->push(cmd);
                    }
                    m_dvCore->commandsStack()->undoStack()->endMacro();
                });

        for (dvm::DVFunction *function : qAsConst(boundFunctionsEntities)) {
            QAction *functionAction = unbindMenu->addAction(function->title());
            connect(functionAction, &QAction::triggered, this, [this, partition, function]() {
                auto cmd = new cmd::CmdFunctionUnbind(partition, function, m_dvCore);
                m_dvCore->commandsStack()->push(cmd);
            });
        }
    }
}

bool DVCreatorTool::handleConnectionCreate(const QPointF &pos)
{
    QGraphicsScene *scene = m_view ? m_view->scene() : nullptr;
    if (!scene)
        return false;

    if (!m_previewConnectionItem)
        return false;

    if (auto itemUnderCursor = qgraphicsitem_cast<DVDeviceGraphicsItem *>(shared::graphicsviewutils::nearestItem(
                scene, pos, shared::graphicsviewutils::kInterfaceTolerance, { DVDeviceGraphicsItem::Type }))) {
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

void DVCreatorTool::handleDirectConnection(const QPointF &pos)
{
    if (m_connectionPoints.size() < 1)
        return;

    m_connectionPoints.append(pos);

    handleConnection(m_connectionPoints);
}

void DVCreatorTool::handleConnection(const QVector<QPointF> &graphicPoints) const
{
    const auto info = dve::gi::validateConnectionCreate(m_view ? m_view->scene() : nullptr, graphicPoints);
    if (info.failed())
        return;

    auto cmd = new dve::cmd::CmdConnectionEntityCreate(
            model()->objectsModel(), graphicPoints, info.startDeviceId, info.endDeviceId);
    m_dvCore->commandsStack()->push(cmd);
}

void DVCreatorTool::handleConnectionReCreate(const QVector<QPointF> &graphicPoints)
{
    m_toolType = ToolType::Pointer;
    const auto info = dve::gi::validateConnectionCreate(m_view ? m_view->scene() : nullptr, graphicPoints);
    if (info.status != dvm::ConnectionValidator::FailReason::NotFail) {
        return;
    }
    const shared::Id id = m_previewConnectionItem->data(Qt::UserRole).toUuid();
    if (id.isNull()) {
        return;
    }

    if (auto item = m_model->getItem(id)) {
        if (auto connection = qgraphicsitem_cast<DVConnectionGraphicsItem *>(item)) {
            if (connection->startItem()->entity()->id() != info.startDeviceId
                    || connection->endItem()->entity()->id() != info.endDeviceId) {
                return;
            }

            QList<QPair<shared::VEObject *, QVector<QPointF>>> paramsList { { connection->entity(),
                    info.connectionPoints } };
            auto cmd = new shared::cmd::CmdEntityGeometryChange(paramsList);
            m_dvCore->commandsStack()->push(cmd);
        }
    }
}

bool DVCreatorTool::prepareMultiPointConnectionPreview(QMouseEvent *e)
{
    QGraphicsScene *scene = m_view->scene();
    const QPointF scenePos = cursorInScene(e->globalPos());
    QGraphicsItem *item = shared::graphicsviewutils::nearestItem(
            scene, scenePos, shared::graphicsviewutils::kInterfaceTolerance, { DVDeviceGraphicsItem::Type });
    if (!item) {
        return false;
    }

    if (!m_previewConnectionItem) {
        const QPointF startPoint = item->mapToScene(QPointF(0, 0));
        m_previewConnectionItem = new QGraphicsPathItem;
        m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
        scene->addItem(m_previewConnectionItem);
        m_connectionPoints.append(startPoint);
        return true;
    }

    return !m_connectionPoints.contains(scenePos);
}

bool DVCreatorTool::prepareReCreateConnectionPreview(QMouseEvent *e)
{
    QGraphicsScene *scene = m_view->scene();
    const QPointF scenePos = cursorInScene(e->globalPos());

    if (!m_previewConnectionItem) {
        QGraphicsItem *item = shared::graphicsviewutils::nearestItem(
                scene, scenePos, shared::graphicsviewutils::kInterfaceTolerance, { DVDeviceGraphicsItem::Type });
        if (!item || item->type() != DVDeviceGraphicsItem::Type)
            return false;

        auto deviceItem = qgraphicsitem_cast<const DVDeviceGraphicsItem *>(item);
        if (!deviceItem)
            return false;

        if (deviceItem->connectionItems().size() != 1)
            return false;

        auto connectionItem = deviceItem->connectionItems().front();
        Q_ASSERT(connectionItem);

        auto reqIface = connectionItem->startItem();
        Q_ASSERT(reqIface);

        m_toolType = ToolType::ReCreateConnection;
        m_previewConnectionItem = new QGraphicsPathItem;
        m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
        //            m_previewConnectionItem->setZValue(ZOrder.Preview);
        m_previewConnectionItem->setData(Qt::UserRole, connectionItem->entity()->id());
        scene->addItem(m_previewConnectionItem);
        m_connectionPoints.append(reqIface->connectionEndPoint(connectionItem));
        connectionItem->setVisible(false);
    }
    return true;
}

bool DVCreatorTool::prepareRectPreview(QMouseEvent *e)
{
    QGraphicsScene *scene = m_view->scene();
    const QPointF scenePos = cursorInScene(e->globalPos());
    QGraphicsItem *parentItem = m_view->itemAt(e->pos());
    if (!parentItem) {
        return false;
    }
    if (!m_previewItem) {
        while (parentItem != nullptr && parentItem->type() != DVNodeGraphicsItem::Type) {
            parentItem = parentItem->parentItem();
        }
        m_previewItem = new QGraphicsRectItem(parentItem);
        m_previewItem->setPen(QPen(Qt::blue, kPreviewItemPenWidth, Qt::SolidLine));
        m_previewItem->setBrush(QBrush(QColor(30, 144, 255, 90)));
        m_clickScenePos = scenePos;

        if (!parentItem)
            scene->addItem(m_previewItem);

        if (!e->buttons().testFlag(Qt::MaxMouseButton)) {
            const QList<QGraphicsItem *> items = m_view->items(e->pos());
            for (auto item : qAsConst(items)) {
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

bool DVCreatorTool::prepareDirectConnectionPreview(QMouseEvent *e)
{
    QGraphicsScene *scene = m_view->scene();
    const QPointF scenePos = cursorInScene(e->globalPos());

    if (!shared::graphicsviewutils::nearestItem(
                scene, scenePos, shared::graphicsviewutils::kInterfaceTolerance, { DVDeviceGraphicsItem::Type })) {
        return false;
    }

    if (m_previewConnectionItem) {
        m_connectionPoints.clear();
    } else {
        m_previewConnectionItem = new QGraphicsPathItem;
        m_previewConnectionItem->setPen(QPen(Qt::black, 2, Qt::DotLine));
        scene->addItem(m_previewConnectionItem);
    }
    m_connectionPoints.append(scenePos);
    return true;
}

} // namespace dve
