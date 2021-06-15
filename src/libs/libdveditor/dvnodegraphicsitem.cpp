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

#include "dvnodegraphicsitem.h"

#include "dvconnectiongraphicsitem.h"
#include "dvdevicegraphicsitem.h"
#include "graphicsviewutils.h"

#include <QPainter>

namespace dve {

DVNodeGraphicsItem::DVNodeGraphicsItem(dvm::DVNode *entity, QGraphicsItem *parent)
    : shared::ui::VERectGraphicsItem(entity, parent)
{
}

dvm::DVNode *dve::DVNodeGraphicsItem::entity() const
{
    return m_dataObject.isNull() ? nullptr : m_dataObject->as<dvm::DVNode *>();
}

void DVNodeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!entity())
        return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    const QRectF br = boundingRect();
    painter->drawRoundedRect(br, 5, 5);
    painter->drawRoundedRect(br.adjusted(2, 2, -2, -2), 4, 4);
    painter->setFont(font());
    painter->setPen(QPen(Qt::black));
    painter->drawText(br.adjusted(20, 20, -20, -20), Qt::AlignLeft | Qt::AlignTop, entity()->titleUI());
    painter->restore();
}

QSizeF DVNodeGraphicsItem::minimalSize() const
{
    return shared::graphicsviewutils::kDefaultNodeGraphicsItemSize;
}

int DVNodeGraphicsItem::itemLevel(bool isSelected) const
{
    return isSelected ? 1 : 0;
}

shared::ColorManager::HandledColors DVNodeGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Node;
}

void DVNodeGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

void DVNodeGraphicsItem::onManualResizeProgress(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt)
        return;

    const QRectF rect = transformedRect(grip, pressedAt, releasedAt);
    if (shared::graphicsviewutils::isBounded(this, rect)) {
        shared::ui::VERectGraphicsItem::onManualResizeProgress(grip, pressedAt, releasedAt);
        layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

void DVNodeGraphicsItem::onManualMoveProgress(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt)
        return;

    const QRectF rect = transformedRect(grip, pressedAt, releasedAt);
    if (shared::graphicsviewutils::isBounded(this, rect)) {
        shared::ui::VERectGraphicsItem::onManualMoveProgress(grip, pressedAt, releasedAt);
        layoutConnectionsOnMove(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

void DVNodeGraphicsItem::onManualResizeFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (pressedAt == releasedAt)
        return;

    if (shared::graphicsviewutils::isBounded(this, sceneBoundingRect())
            && !shared::graphicsviewutils::isCollided(this, sceneBoundingRect())) {
        layoutInterfaces();
        layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::PartialRebuild);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

void DVNodeGraphicsItem::onManualMoveFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (pressedAt == releasedAt)
        return;

    if (shared::graphicsviewutils::isBounded(this, sceneBoundingRect())
            && !shared::graphicsviewutils::isCollided(this, sceneBoundingRect())) {
        layoutConnectionsOnMove(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::PartialRebuild);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        layoutConnectionsOnMove(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

void DVNodeGraphicsItem::layoutInterfaces()
{
    for (auto child : childItems()) {
        if (child->type() == DVDeviceGraphicsItem::Type) {
            if (auto iObj = qgraphicsitem_cast<DVDeviceGraphicsItem *>(child)) {
                iObj->adjustItem();
            }
        }
    }
}

void DVNodeGraphicsItem::layoutConnectionsOnResize(
        shared::ui::VEConnectionGraphicsItem::CollisionsPolicy collisionsPolicy)
{
    /// Changing inner and outer connections bound to current item
    for (const auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<DVDeviceGraphicsItem *>(item)) {
            shared::ui::VEConnectionGraphicsItem::layoutInterfaceConnections(
                    iface, shared::ui::VEConnectionGraphicsItem::LayoutPolicy::Scaling, collisionsPolicy, true);
        } else if (auto connection = qgraphicsitem_cast<DVConnectionGraphicsItem *>(item)) {
            if (connection->sourceItem() != this && connection->targetItem() != this)
                connection->layout();
        }
    }
}

void DVNodeGraphicsItem::layoutConnectionsOnMove(
        shared::ui::VEConnectionGraphicsItem::CollisionsPolicy collisionsPolicy)
{
    /// Changing outer connections only cause inner stay unchanged as children of current item
    for (const auto item : childItems()) {
        if (auto iface = qgraphicsitem_cast<DVDeviceGraphicsItem *>(item)) {
            shared::ui::VEConnectionGraphicsItem::layoutInterfaceConnections(
                    iface, shared::ui::VEConnectionGraphicsItem::LayoutPolicy::Scaling, collisionsPolicy, false);
        }
    }
}

} // namespace dve
