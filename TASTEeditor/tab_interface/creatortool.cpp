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
#include "aadlcontainergraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "commands/cmdcommentitemcreate.h"
#include "commands/cmdcontaineritemcreate.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <QtDebug>
#include <app/commandsstack.h>
#include <limits>
#include <tab_aadl/aadlobjectcomment.h>
#include <tab_aadl/aadlobjectcontainer.h>
#include <tab_aadl/aadlobjectfunction.h>
#include <tab_aadl/aadlobjectiface.h>
#include <tab_aadl/aadlobjectsmodel.h>

static const qreal kInterfaceTolerance = 20;
static const qreal kConnectionTolerance = 20;

namespace taste3 {
namespace aadl {

static inline QGraphicsItem *nearestItem(QGraphicsScene *scene, const QRectF &area,
                                         const QList<int> &acceptableTypes = QList<int>())
{
    const QList<QGraphicsItem *> areaItems = scene->items(area);
    if (areaItems.isEmpty())
        return nullptr;

    const QPointF point = area.center();
    if (areaItems.size() == 1) {
        auto item = areaItems.value(0);
        if (item && item->contains(point) && acceptableTypes.contains(item->type()))
            return item;
    }

    qreal distance = std::numeric_limits<int>::max();
    QGraphicsItem *nearestToCenter = nullptr;
    for (QGraphicsItem *item : areaItems) {
        if (!acceptableTypes.isEmpty() && !acceptableTypes.contains(item->type()))
            continue;

        const QRectF itemRect = item->sceneBoundingRect();
        qreal itemDistance = qAbs(itemRect.right() - point.x());
        itemDistance = std::min(itemDistance, qAbs(itemRect.left() - point.x()));
        itemDistance = std::min(itemDistance, qAbs(itemRect.top() - point.y()));
        itemDistance = std::min(itemDistance, qAbs(itemRect.bottom() - point.y()));
        if (itemDistance < distance) {
            nearestToCenter = item;
            distance = itemDistance;
        }
    }
    return nearestToCenter;
}
static inline QGraphicsItem *nearestItem(QGraphicsScene *scene, const QPointF &center, qreal offset,
                                         const QList<int> &acceptableTypes = QList<int>())
{
    const QRectF area { center - QPointF(offset / 2, offset / 2), center + QPointF(offset / 2, offset / 2) };
    return nearestItem(scene, area, acceptableTypes);
}

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
        if (keyEvent->key() == Qt::Key_Delete) {
            removeSelectedItems();
        } else if (keyEvent->key() == Qt::Key_Escape) {
            if (auto scene = m_view->scene())
                scene->clearSelection();
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

    if (!(e->button() & Qt::RightButton) && m_toolType == ToolType::Pointer)
        return false;

    if (m_toolType == ToolType::Connection) {
        const QPointF scenePos = cursorInScene(e->globalPos());
        QGraphicsItem *item = nearestItem(scene, scenePos, kConnectionTolerance, { AADLInterfaceGraphicsItem::Type });
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
        if (m_connectionPoints.contains(scenePos))
            return false;

        if (!m_connectionPoints.isEmpty())
            m_connectionPoints.append(scenePos);

        QPainterPath pp;
        pp.addPolygon(m_connectionPoints);
        m_previewConnectionItem->setPath(pp);
        return true;
    } else if (m_toolType != ToolType::RequiredInterface && m_toolType != ToolType::ProvidedInterface) {
        if (!m_previewItem) {
            QGraphicsItem *parentItem = m_view->itemAt(e->localPos().toPoint());
            m_previewItem = new QGraphicsRectItem(parentItem ? parentItem->topLevelItem() : nullptr);
            m_previewItem->setPen(QPen(Qt::blue, 2, Qt::SolidLine));
            m_previewItem->setBrush(QBrush(QColor(30, 144, 255, 90)));
            m_previewItem->setZValue(1);
            scene->addItem(m_previewItem);
        }
        const QPointF scenePos = m_previewItem->mapFromScene(cursorInScene(e->globalPos()));
        m_previewItem->setRect({ scenePos, scenePos });
        return true;
    }

    return true;
}

bool CreatorTool::onMouseRelease(QMouseEvent *e)
{
    if (!m_view)
        return false;

    const QPointF scenePos = cursorInScene(e->globalPos());

    if (e->button() & Qt::RightButton) {
        QMenu *menu = new QMenu(m_view);
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/container.svg")), tr("Container"), this,
                        [this]() { handleToolType(ToolType::Container); });
        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")), tr("Function"), this,
                        [this]() { handleToolType(ToolType::Function); });
        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/ri.svg")), tr("Comment"), this,
                        [this]() { handleToolType(ToolType::Comment); });
        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/pi.svg")), tr("Prov.I"), this,
                        [this]() { handleToolType(ToolType::ProvidedInterface); });
        menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/comment.svg")), tr("Req.I"), this,
                        [this]() { handleToolType(ToolType::RequiredInterface); });
        connect(menu, &QMenu::aboutToHide, this, [this]() { m_previewItem->setVisible(false); });
        menu->exec(m_view->mapToGlobal(m_view->mapFromScene(scenePos)));
        return true;
    } else if (m_toolType != ToolType::Pointer) {
        handleToolType(m_toolType);
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
        m_previewItem->setRect({ m_previewItem->rect().topLeft(), scenePos });
        return true;
    } else if (m_previewConnectionItem && m_previewConnectionItem->isVisible() && !m_connectionPoints.isEmpty()) {
        if (auto scene = m_view->scene()) {
            const QPointF startPoint = m_connectionPoints.value(0);
            QGraphicsItem *startItem = scene->itemAt(startPoint, QTransform());
            AADLInterfaceGraphicsItem *ifaceItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(startItem);
            if (!ifaceItem)
                return false;

            QGraphicsItem *targetItem = ifaceItem->targetItem();
            if (!targetItem)
                return false;

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

void CreatorTool::handleToolType(CreatorTool::ToolType type)
{
    if (!m_view)
        return;

    const QPointF pos = cursorInScene();
    const QRectF itemSceneRect = m_previewItem
            ? m_previewItem->rect()
            : QRectF { pos - QPointF(kInterfaceTolerance / 2, kInterfaceTolerance / 2),
                       pos + QPointF(kInterfaceTolerance / 2, kInterfaceTolerance / 2) };
    const QVariantList params = { itemSceneRect, qVariantFromValue(m_model.data()) };
    if (auto scene = m_view->scene()) {
        switch (type) {
        case ToolType::Comment: {
            auto item = new AADLCommentGraphicsItem(new AADLObjectComment(tr("Comment"), m_model),
                                                    m_previewItem ? m_previewItem->parentItem() : nullptr);
            item->setRect(itemSceneRect);
            if (!item->parentItem())
                scene->addItem(item);

            taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateCommentEntity, params));
        } break;
        case ToolType::Container: {
            auto item = new AADLContainerGraphicsItem(new AADLObjectContainer(tr("Function interface"), m_model),
                                                      m_previewItem ? m_previewItem->parentItem() : nullptr);
            item->setRect(itemSceneRect);
            if (!item->parentItem())
                scene->addItem(item);

            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateContainerEntity, params));
        } break;
        case ToolType::Function: {
            auto item = new AADLFunctionGraphicsItem(new AADLObjectFunction(tr("Function"), m_model),
                                                     m_previewItem ? m_previewItem->parentItem() : nullptr);
            item->setRect(itemSceneRect);
            if (!item->parentItem())
                scene->addItem(item);

            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateFunctionEntity, params));
        } break;
        case ToolType::ProvidedInterface: {
            static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLContainerGraphicsItem::Type };

            if (auto parentItem = nearestItem(scene, itemSceneRect, types)) {
                auto item = new AADLInterfaceGraphicsItem(
                        new AADLObjectIface(AADLObjectIface::IfaceType::Provided, tr("PI")));
                item->setTargetItem(parentItem, pos);

                taste3::cmd::CommandsStack::current()->push(
                        cmd::CommandsFactory::create(cmd::CreateProvidedInterfaceEntity, params));
            }
        } break;
        case ToolType::RequiredInterface: {
            static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLContainerGraphicsItem::Type };
            if (auto parentItem = nearestItem(scene, itemSceneRect, types)) {
                auto item = new AADLInterfaceGraphicsItem(
                        new AADLObjectIface(AADLObjectIface::IfaceType::Required, tr("RI")));
                item->setTargetItem(parentItem, pos);

                taste3::cmd::CommandsStack::current()->push(
                        cmd::CommandsFactory::create(cmd::CreateRequiredInterfaceEntity, params));
            }
        } break;
        case ToolType::Connection: {
            if (!m_previewConnectionItem)
                return;

            QGraphicsItem *itemUnderCursor =
                    nearestItem(scene, pos, kConnectionTolerance, { AADLInterfaceGraphicsItem::Type });
            if (m_connectionPoints.size() >= 2 && itemUnderCursor) {
                auto item = new AADLConnectionGraphicsItem();
                item->setPoints(m_connectionPoints);
                scene->addItem(item);

                taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(
                        cmd::CreateConnectionEntity,
                        { qVariantFromValue(m_connectionPoints), qVariantFromValue(m_model.data()) }));
            } else {
                return;
            }
        } break;
        default:
            break;
        }
        clearPreviewItem();
    }

    emit created();
}

void CreatorTool::removeSelectedItems()
{
    if (!m_view)
        return;

    if (auto scene = m_view->scene()) {
        for (QGraphicsItem *item : scene->selectedItems()) {
            //            const QVariantList params = { item, qVariantFromValue(m_model.data()) };
            //            taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::DeleteEntity,
            //            params));
            scene->removeItem(item);
            delete item;
        }
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

} // namespace aadl
} // namespace taste3
