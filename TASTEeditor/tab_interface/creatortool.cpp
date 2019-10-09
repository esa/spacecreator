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
#include <QtMath>
#include <app/commandsstack.h>
#include <baseitems/common/utils.h>
#include <limits>
#include <tab_aadl/aadlobjectcomment.h>
#include <tab_aadl/aadlobjectcontainer.h>
#include <tab_aadl/aadlobjectfunction.h>
#include <tab_aadl/aadlobjectiface.h>
#include <tab_aadl/aadlobjectsmodel.h>

static const qreal kInterfaceTolerance = 20;
static const qreal kConnectionTolerance = 20;
static const QMarginsF kMargins { 50, 50, 50, 50 };

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
                setCurrentToolType(ToolType::Pointer);
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
        QGraphicsItem *item = utils::nearestItem(scene, scenePos,
                                                 { AADLContainerGraphicsItem::Type, AADLFunctionGraphicsItem::Type });
        if (!item)
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
        QGraphicsItem *item = item =
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
                const QList<int> types = { AADLContainerGraphicsItem::Type, AADLFunctionGraphicsItem::Type };
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
            QMenu *menu = new QMenu(m_view);
            menu->setAttribute(Qt::WA_DeleteOnClose);
            //            menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/container.svg")),
            //            tr("Container"), this,
            //                            [this]() { handleToolType(ToolType::Container); });
            menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")), tr("Function"), this,
                            [this]() { handleToolType(ToolType::Function); });
            menu->addAction(QIcon(QLatin1String(":/tab_interface/toolbar/icns/comment.svg")), tr("Comment"), this,
                            [this]() { handleToolType(ToolType::Comment); });
            connect(menu, &QMenu::aboutToHide, this, [this]() { m_previewItem->setVisible(false); });
            menu->exec(m_view->mapToGlobal(m_view->mapFromScene(scenePos)));
            clearPreviewItem();
            return true;
        }
    } else {
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
        if (auto scene = m_view->scene()) {
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

bool alignPoint(const QGraphicsScene *scene, const QPointF &point, QPointF &pointToAlign)
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

void CreatorTool::handleToolType(CreatorTool::ToolType type)
{
    if (!m_view)
        return;

    const QPointF pos = cursorInScene();
    if (auto scene = m_view->scene()) {

        auto containerObject = [](QGraphicsItem *item) -> AADLObjectContainer * {
            if (auto containerItem = qgraphicsitem_cast<aadl::AADLContainerGraphicsItem *>(item))
                return containerItem->entity();
            if (auto functionItem = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(item))
                return functionItem->entity();
            return nullptr;
        };

        static const QList<int> containerTypes = { AADLFunctionGraphicsItem::Type, AADLContainerGraphicsItem::Type };

        auto adjustToSize = [](const QRectF &rect, const QSizeF &minSize) {
            QRectF itemRect = rect;
            if (itemRect.width() < minSize.width())
                itemRect.setWidth(minSize.width());
            if (itemRect.height() < minSize.height())
                itemRect.setHeight(minSize.height());
            return itemRect;
        };

        switch (type) {
        case ToolType::Comment:
            if (m_previewItem) {
                const QRectF itemSceneRect = adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()),
                                                          AADLCommentGraphicsItem::defaultSize());
                const QVariantList params = { qVariantFromValue(m_model.data()), itemSceneRect };
                taste3::cmd::CommandsStack::current()->push(
                        cmd::CommandsFactory::create(cmd::CreateCommentEntity, params));
            }
            break;
        case ToolType::Container:
            if (m_previewItem) {
                const QRectF itemSceneRect = adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()),
                                                          AADLContainerGraphicsItem::defaultSize());
                AADLObjectContainer *parentObject = containerObject(m_previewItem->parentItem());
                const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(parentObject),
                                              itemSceneRect };
                taste3::cmd::CommandsStack::current()->push(
                        cmd::CommandsFactory::create(cmd::CreateContainerEntity, params));
            }
            break;
        case ToolType::Function:
            if (m_previewItem) {
                const QRectF itemSceneRect = adjustToSize(m_previewItem->mapRectToScene(m_previewItem->rect()),
                                                          AADLFunctionGraphicsItem::defaultSize());
                AADLObjectContainer *parentObject = containerObject(m_previewItem->parentItem());
                const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(parentObject),
                                              itemSceneRect };
                taste3::cmd::CommandsStack::current()->push(
                        cmd::CommandsFactory::create(cmd::CreateFunctionEntity, params));
            }
            break;
        case ToolType::ProvidedInterface: {
            if (QGraphicsItem *parentItem =
                        utils::nearestItem(scene, adjustFromPoint(pos, kInterfaceTolerance), containerTypes)) {
                AADLObjectContainer *parentObject = containerObject(parentItem);
                const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(parentObject), pos };
                taste3::cmd::CommandsStack::current()->push(
                        cmd::CommandsFactory::create(cmd::CreateProvidedInterfaceEntity, params));
            }
        } break;
        case ToolType::RequiredInterface: {
            if (auto parentItem =
                        utils::nearestItem(scene, adjustFromPoint(pos, kInterfaceTolerance), containerTypes)) {
                AADLObjectContainer *parentObject = containerObject(parentItem);
                const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(parentObject), pos };
                taste3::cmd::CommandsStack::current()->push(
                        cmd::CommandsFactory::create(cmd::CreateRequiredInterfaceEntity, params));
            }
        } break;
        case ToolType::MultiPointConnection: {
            if (!handleConnectionCreate(scene, pos))
                return;

            const QList<int> ifaceTypes { AADLInterfaceGraphicsItem::Type };
            AADLObjectContainer *startObject = containerObject(
                    utils::nearestItem(scene, m_connectionPoints.first(), kInterfaceTolerance, containerTypes));
            AADLObjectContainer *endObject = containerObject(
                    utils::nearestItem(scene, m_connectionPoints.last(), kInterfaceTolerance, containerTypes));
            if (!startObject || !endObject)
                break;

            AADLInterfaceGraphicsItem *pi = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                    utils::nearestItem(scene, m_connectionPoints.first(), kInterfaceTolerance, ifaceTypes));
            AADLInterfaceGraphicsItem *ri = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                    utils::nearestItem(scene, m_connectionPoints.last(), kInterfaceTolerance, ifaceTypes));
            if (!pi || !ri)
                break;

            if (pi->entity()->isRequired() && ri->entity()->isProvided()) {
                qSwap(startObject, endObject);
                qSwap(pi, ri);
            }
            AADLObjectIfaceProvided *providedIface = qobject_cast<AADLObjectIfaceProvided *>(pi->entity());
            AADLObjectIfaceRequired *requiredIface = qobject_cast<AADLObjectIfaceRequired *>(ri->entity());

            if (!providedIface || !requiredIface)
                break;

            const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(startObject),
                                          qVariantFromValue(endObject),      qVariantFromValue(providedIface),
                                          qVariantFromValue(requiredIface),  qVariantFromValue(m_connectionPoints) };
            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateManualConnectionEntity, params));
        } break;
        case ToolType::DirectConnection: {
            if (m_connectionPoints.size() < 1)
                break;

            static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLContainerGraphicsItem::Type };
            const QLineF connectionLine { m_connectionPoints.first(), pos };

            auto itemAtStartPos =
                    utils::nearestItem(scene, adjustFromPoint(m_connectionPoints.first(), kInterfaceTolerance), types);
            AADLObjectContainer *startObject = containerObject(itemAtStartPos);
            if (!startObject)
                break;

            QPointF startPointAdjusted(m_connectionPoints.first());
            if (!utils::intersects(itemAtStartPos->sceneBoundingRect(), connectionLine, &startPointAdjusted))
                break;

            auto itemAtEndPos = utils::nearestItem(scene, adjustFromPoint(pos, kInterfaceTolerance), types);
            AADLObjectContainer *endObject = containerObject(itemAtEndPos);
            if (!endObject)
                break;

            QPointF endPointAdjusted(pos);
            if (!utils::intersects(itemAtEndPos->sceneBoundingRect(), connectionLine, &endPointAdjusted))
                break;

            const QVariantList params = { qVariantFromValue(m_model.data()), qVariantFromValue(startObject),
                                          qVariantFromValue(endObject), startPointAdjusted, endPointAdjusted };
            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateDirectConnectionEntity, params));

            m_connectionPoints.append(pos);
        } break;
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
        if (!alignPoint(scene, finishPoint, m_connectionPoints.last()))
            return false;

        m_connectionPoints.append(finishPoint);
        return true;
    }

    QPointF alignedPos { pos };
    if (m_connectionPoints.size() == 1) {
        if (!alignPoint(scene, m_connectionPoints.value(0), alignedPos))
            return false;
    } else if (m_connectionPoints.size() >= 2) {
        const QLineF line { m_connectionPoints.last(), m_connectionPoints.value(m_connectionPoints.size() - 2) };
        const QLineF current { m_connectionPoints.last(), pos };
        const qreal length = current.length() * qSin(qDegreesToRadians(line.angleTo(current)));
        QLineF normal = line.normalVector();
        normal.setLength(qAbs(length));
        if (length < 0)
            normal.setAngle(normal.angle() + 180);
        alignedPos = normal.p2();
    }

    m_connectionPoints.append(alignedPos);

    QPainterPath pp;
    pp.addPolygon(m_connectionPoints);
    pp.lineTo(pos);
    m_previewConnectionItem->setPath(pp);
    return false;
}

void CreatorTool::removeSelectedItems()
{
    if (!m_view)
        return;

    if (auto scene = m_view->scene()) {
        taste3::cmd::CommandsStack::current()->beginMacro(tr("Change connection"));
        for (QGraphicsItem *item : scene->selectedItems()) {
            AADLObject *entity = nullptr;
            if (auto connectionItem = qgraphicsitem_cast<AADLConnectionGraphicsItem *>(item))
                entity = connectionItem->entity();
            else if (auto iObj = qobject_cast<InteractiveObject *>(item->toGraphicsObject()))
                entity = qobject_cast<AADLObject *>(iObj->modelEntity());

            if (entity) {
                const QVariantList params = { qVariantFromValue(entity), qVariantFromValue(m_model.data()) };
                taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::RemoveEntity, params));
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

} // namespace aadl
} // namespace taste3
