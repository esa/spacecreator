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

#include "dvconnectiongraphicsitem.h"

#include "dvdevicegraphicsitem.h"
#include "dvnodegraphicsitem.h"
#include "graphicsviewutils.h"

#include <QPainter>

namespace dve {

DVConnectionGraphicsItem::DVConnectionGraphicsItem(dvm::DVConnection *connection, DVDeviceGraphicsItem *startItem,
        DVDeviceGraphicsItem *endItem, QGraphicsItem *parent)
    : shared::ui::VEInteractiveObject(connection, parent)
    , m_startItem(startItem)
    , m_endItem(endItem)
{
}

DVConnectionGraphicsItem::~DVConnectionGraphicsItem()
{
    for (DVDeviceGraphicsItem *item : { startItem(), endItem() }) {
        if (item) {
            item->removeConnection(this);
        }
    }
}

dvm::DVConnection *dve::DVConnectionGraphicsItem::entity() const
{
    return m_dataObject.isNull() ? nullptr : m_dataObject->as<dvm::DVConnection *>();
}

void DVConnectionGraphicsItem::init()
{
    for (DVDeviceGraphicsItem *item : { startItem(), endItem() }) {
        if (item) {
            item->addConnection(this);
        }
    }
}

DVDeviceGraphicsItem *DVConnectionGraphicsItem::startItem() const
{
    return m_startItem;
}

DVDeviceGraphicsItem *DVConnectionGraphicsItem::endItem() const
{
    return m_endItem;
}

void DVConnectionGraphicsItem::setPoints(const QVector<QPointF> &points)
{
    if (points.isEmpty()) {
        if (m_startItem && m_endItem)
            instantLayoutUpdate();
        else
            m_points.clear();
        return;
    }

    if (!shared::graphicsviewutils::comparePolygones(m_points, points)) {
        m_points = points;
        instantLayoutUpdate();
    }
}

void DVConnectionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!entity())
        return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->drawPolyline(m_points);
    painter->restore();
}

int DVConnectionGraphicsItem::itemLevel(bool isSelected) const
{
    return isSelected ? 1 : 0;
}

void DVConnectionGraphicsItem::updateFromEntity()
{
    m_points = mapFromScene(shared::graphicsviewutils::polygon(entity()->coordinates()));
    if (m_points.isEmpty()) {
        layout();
        updateEntity();
    }
    rebuildLayout();
}

shared::ColorManager::HandledColors DVConnectionGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::BusConnection;
}

void DVConnectionGraphicsItem::layout()
{
    if (!startItem() || !endItem())
        return;

    Q_ASSERT(startItem()->scene() == endItem()->scene() && scene());

    m_points = shared::graphicsviewutils::createConnectionPath(shared::graphicsviewutils::siblingItemsRects(this),
            startItem()->sceneBoundingRect().center(), startItem()->targetItem()->sceneBoundingRect(),
            endItem()->sceneBoundingRect().center(), endItem()->targetItem()->sceneBoundingRect());

    setBoundingRect(m_points.boundingRect());
}

void DVConnectionGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

} // namespace dve
