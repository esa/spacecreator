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

#include "dvdevicegraphicsitem.h"

#include "dvconnectiongraphicsitem.h"
#include "dvnodegraphicsitem.h"
#include "graphicsviewutils.h"
#include "positionlookuphelper.h"

#include <QPainter>

namespace dve {

DVDeviceGraphicsItem::DVDeviceGraphicsItem(dvm::DVDevice *device, QGraphicsItem *parent)
    : shared::ui::VEInteractiveObject(device, parent)
{
    setBoundingRect({ -10, -10, 20, 20 });
}

DVDeviceGraphicsItem::~DVDeviceGraphicsItem() { }

dvm::DVDevice *DVDeviceGraphicsItem::entity() const
{
    return m_dataObject.isNull() ? nullptr : m_dataObject->as<dvm::DVDevice *>();
}

int DVDeviceGraphicsItem::itemLevel(bool isSelected) const
{
    return isSelected ? 1 : 0;
}

DVNodeGraphicsItem *DVDeviceGraphicsItem::targetItem() const
{
    return m_node;
}

void DVDeviceGraphicsItem::setTargetItem(DVNodeGraphicsItem *item, const QPointF &globalPos)
{
    if (!item)
        return;

    setParentItem(item);
    setPos(parentItem()->mapFromScene(globalPos));
    instantLayoutUpdate();
}

void DVDeviceGraphicsItem::addConnection(DVConnectionGraphicsItem *item)
{
    if (!item || m_connections.contains(item))
        return;

    m_connections.append(item);
}

void DVDeviceGraphicsItem::removeConnection(DVConnectionGraphicsItem *item)
{
    if (!item)
        return;

    m_connections.removeAll(item);
}

QList<QPointer<DVConnectionGraphicsItem>> DVDeviceGraphicsItem::connectionItems() const
{
    return m_connections;
}

void DVDeviceGraphicsItem::updateFromEntity()
{
    const dvm::DVDevice *obj = entity();
    Q_ASSERT(obj);
    if (!obj)
        return;

    const QPointF coordinates = shared::graphicsviewutils::pos(obj->coordinates());
    if (coordinates.isNull())
        instantLayoutUpdate();
    else
        setTargetItem(qgraphicsitem_cast<DVNodeGraphicsItem *>(parentItem()), coordinates);
    adjustItem();
}

void DVDeviceGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

void DVDeviceGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->drawRect(br);
    painter->restore();
}

shared::ColorManager::HandledColors DVDeviceGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Device;
}

void DVDeviceGraphicsItem::adjustItem()
{
    static qreal kSiblingMinDistance = 10.0;

    if (!parentItem()) {
        return;
    }

    QList<QRectF> siblingsRects;
    const QList<QGraphicsItem *> siblingItems = parentItem()->childItems();
    std::for_each(siblingItems.cbegin(), siblingItems.cend(), [this, &siblingsRects](const QGraphicsItem *sibling) {
        if (sibling->type() == DVDeviceGraphicsItem::Type && sibling != this) {
            QRectF itemRect = sibling->boundingRect();
            itemRect.setWidth(kSiblingMinDistance - itemRect.x());
            itemRect = sibling->mapRectToParent(itemRect);
            siblingsRects.append(itemRect);
        }
    });

    const QPointF initialOffset = boundingRect().topLeft();
    QRectF itemRect = boundingRect();
    itemRect.setWidth(kSiblingMinDistance - itemRect.x());
    itemRect = mapRectToParent(itemRect);
    const QRectF parentRect = parentItem()->boundingRect();

    auto itemPath = [this](Qt::Alignment) {
        QPainterPath pp;
        pp.addRect(boundingRect());
        return pp;
    };

    QRectF intersectedRect;
    if (shared::graphicsviewutils::isCollided(siblingsRects, itemRect, &intersectedRect) && parentRect.isValid()) {
        const QHash<Qt::Alignment, QPainterPath> kSidePaths {
            { Qt::AlignLeft, itemPath(Qt::AlignLeft) },
            { Qt::AlignTop, itemPath(Qt::AlignTop) },
            { Qt::AlignRight, itemPath(Qt::AlignRight) },
            { Qt::AlignBottom, itemPath(Qt::AlignBottom) },
        };
        shared::PositionLookupHelper cwHelper(kSidePaths, parentRect, siblingsRects, itemRect, initialOffset,
                shared::graphicsviewutils::LookupDirection::Clockwise);
        shared::PositionLookupHelper ccwHelper(kSidePaths, parentRect, siblingsRects, itemRect, initialOffset,
                shared::graphicsviewutils::LookupDirection::CounterClockwise);
        while (cwHelper.hasNext() || ccwHelper.hasNext()) {
            if (cwHelper.lookup()) {
                setPos(cwHelper.mappedOriginPoint());
                break;
            } else if (ccwHelper.lookup()) {
                setPos(ccwHelper.mappedOriginPoint());
                break;
            }
        }
        for (DVConnectionGraphicsItem *connection : qAsConst(m_connections)) {
            if (connection) {
                connection->layout();
            }
        }
    }
}

} // namespace dve
