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

#include "veconnectionendpointgraphicsitem.h"

#include "positionlookuphelper.h"
#include "textitem.h"
#include "veconnectiongraphicsitem.h"
#include "veobject.h"
#include "verectgraphicsitem.h"

static const int kSiblingMinDistance = 20;

namespace shared {
namespace ui {

VEConnectionEndPointGraphicsItem::VEConnectionEndPointGraphicsItem(VEObject *object, QGraphicsItem *parent)
    : VEInteractiveObject(object, parent)
{
}

VEConnectionEndPointGraphicsItem::~VEConnectionEndPointGraphicsItem() { }

void VEConnectionEndPointGraphicsItem::addConnection(VEConnectionGraphicsItem *item)
{
    if (!item || m_connections.contains(item))
        return;

    m_connections.append(item);
}

void VEConnectionEndPointGraphicsItem::removeConnection(VEConnectionGraphicsItem *item)
{
    if (!item)
        return;

    m_connections.removeAll(item);
}

QList<QPointer<VEConnectionGraphicsItem>> VEConnectionEndPointGraphicsItem::connectionItems() const
{
    return m_connections;
}

VERectGraphicsItem *VEConnectionEndPointGraphicsItem::targetItem() const
{
    return parentItem() ? qobject_cast<VERectGraphicsItem *>(parentItem()->toGraphicsObject()) : nullptr;
}

void VEConnectionEndPointGraphicsItem::setTargetItem(VERectGraphicsItem *item, const QPointF &globalPos)
{
    if (!item)
        return;

    setParentItem(item);
    setPos(parentItem()->mapFromScene(globalPos));
    instantLayoutUpdate();
}

QPointF VEConnectionEndPointGraphicsItem::connectionEndPoint(const bool innerConnection) const
{
    Q_UNUSED(innerConnection)

    return pos();
}

QPointF VEConnectionEndPointGraphicsItem::connectionEndPoint(VEConnectionGraphicsItem *connection) const
{
    Q_UNUSED(connection)

    return pos();
}

void VEConnectionEndPointGraphicsItem::updateFromEntity()
{
    Q_ASSERT(entity());
    if (!entity())
        return;

    const QPointF coordinates = graphicsviewutils::pos(entity()->coordinates());
    if (coordinates.isNull())
        instantLayoutUpdate();
    else
        setTargetItem(qobject_cast<VERectGraphicsItem *>(parentObject()), coordinates);
    adjustItem();
}

QList<QPair<VEObject *, QVector<QPointF>>>
VEConnectionEndPointGraphicsItem::prepareChangeCoordinatesCommandParams() const
{
    QVector<QPointF> pos;
    pos.append(scenePos());
    QList<QPair<shared::VEObject *, QVector<QPointF>>> params = { { entity(), pos } };
    for (const auto &connection : connectionItems()) {
        if (connection) {
            params.append({ connection->entity(), connection->graphicsPoints() });
        }
    }

    return params;
}

void VEConnectionEndPointGraphicsItem::adjustItem()
{
    if (!parentItem()) {
        return;
    }

    QList<QRectF> siblingsRects;
    const QList<QGraphicsItem *> siblingItems = parentItem()->childItems();
    std::for_each(siblingItems.cbegin(), siblingItems.cend(), [this, &siblingsRects](const QGraphicsItem *sibling) {
        if (qobject_cast<const VEConnectionEndPointGraphicsItem *>(sibling->toGraphicsObject()) != nullptr
                && sibling != this) {
            QRectF itemRect = sibling->boundingRect();
            itemRect.adjust(-kSiblingMinDistance / 2, -kSiblingMinDistance / 2, kSiblingMinDistance / 2,
                    kSiblingMinDistance / 2);
            itemRect = sibling->mapRectToParent(itemRect);
            siblingsRects.append(itemRect);
        }
    });

    QRectF itemRect = boundingRect();
    const QPointF initialOffset = itemRect.topLeft();
    itemRect = mapRectToParent(itemRect);
    const QRectF parentRect = parentItem()->boundingRect();

    if ((pos().isNull() || shared::graphicsviewutils::isCollided(siblingsRects, itemRect)) && parentRect.isValid()) {
        PositionLookupHelper helper(sidePaths(), parentRect, siblingsRects, itemRect, initialOffset, lookupType());
        if (helper.lookup()) {
            if (helper.isSideChanged()) {
                updateInternalItems(helper.side());
            }
            setPos(helper.mappedOriginPoint());
            for (VEConnectionGraphicsItem *connection : qAsConst(m_connections)) {
                if (connection) {
                    connection->layout();
                }
            }
        }
    }
}

qreal VEConnectionEndPointGraphicsItem::minSiblingDistance()
{
    return kSiblingMinDistance;
}

void VEConnectionEndPointGraphicsItem::layout()
{
    const QPointF pos = shared::graphicsviewutils::pos(entity()->coordinates());
    if (pos.isNull()) {
        adjustItem();
        return;
    }

    const QRectF parentRect = targetItem()->sceneBoundingRect();
    const Qt::Alignment side = shared::graphicsviewutils::getNearestSide(parentRect, pos);
    const QPointF stickyPos = shared::graphicsviewutils::getSidePosition(parentRect, pos, side);
    updateInternalItems(side);
    setPos(targetItem()->mapFromScene(stickyPos));
}

QPainterPath VEConnectionEndPointGraphicsItem::ifaceShape() const
{
    return mapToScene(shape());
}

QVariant VEConnectionEndPointGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemVisibleHasChanged: {
        for (VEConnectionGraphicsItem *connection : qAsConst(m_connections)) {
            if (connection->startItem() && connection->endItem()) {
                connection->setVisible(connection->startItem()->isVisible() && connection->endItem()->isVisible());
            } else {
                connection->setVisible(false);
            }
        }
        break;
    }
    default:
        break;
    }
    return VEInteractiveObject::itemChange(change, value);
}

void VEConnectionEndPointGraphicsItem::onManualMoveProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    if (!scene())
        return;

    const QPointF shift = { to - from };
    if (shift.isNull())
        return;

    const QPointF newScenePos = scenePos() + shift;
    const QPointF mappedPos = mapToParent(mapFromScene(newScenePos));
    const QRectF parentRect = targetItem()->boundingRect();
    const Qt::Alignment alignment = graphicsviewutils::getNearestSide(parentRect, mappedPos);
    updateInternalItems(alignment);
    setPos(mappedPos);
    updateGripPoints();

    VEConnectionGraphicsItem::layoutInterfaceConnections(this, VEConnectionGraphicsItem::LayoutPolicy::LastSegment,
            VEConnectionGraphicsItem::CollisionsPolicy::Ignore, true);
}

void VEConnectionEndPointGraphicsItem::onManualMoveFinish(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QPointF shift = { to - from };
    if (shift.isNull())
        return;

    rebuildLayout();
    VEConnectionGraphicsItem::layoutInterfaceConnections(this, VEConnectionGraphicsItem::LayoutPolicy::Default,
            VEConnectionGraphicsItem::CollisionsPolicy::Rebuild, true);
    updateEntity();
}

void VEConnectionEndPointGraphicsItem::updateInternalItems(Qt::Alignment alignment)
{
    Q_UNUSED(alignment)
}

QPainterPath VEConnectionEndPointGraphicsItem::itemPath(Qt::Alignment alignment) const
{
    Q_UNUSED(alignment)

    return shape();
}

QList<QPair<Qt::Alignment, QPainterPath>> VEConnectionEndPointGraphicsItem::sidePaths() const
{
    return {
        { Qt::AlignLeft, itemPath(Qt::AlignLeft) },
        { Qt::AlignTop, itemPath(Qt::AlignTop) },
        { Qt::AlignRight, itemPath(Qt::AlignRight) },
        { Qt::AlignBottom, itemPath(Qt::AlignBottom) },
    };
}

graphicsviewutils::LookupDirection VEConnectionEndPointGraphicsItem::lookupType() const
{
    return graphicsviewutils::LookupDirection::Bidirectional;
}

void VEConnectionEndPointGraphicsItem::updateTextPosition()
{
    if (m_textItem) {
        m_textItem->setPos(QPointF(0, 0));
    }
}

} // namespace ui
} // namespace shared
