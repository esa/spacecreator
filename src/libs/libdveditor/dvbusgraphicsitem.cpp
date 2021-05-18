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

#include "dvbusgraphicsitem.h"

#include "graphicsviewutils.h"

#include <QPainter>

namespace dve {

DVBusGraphicsItem::DVBusGraphicsItem(dvm::DVBus *bus, QGraphicsItem *parent)
    : shared::ui::VERectGraphicsItem(bus, parent)
{
}

dvm::DVBus *DVBusGraphicsItem::entity() const
{
    return m_dataObject.isNull() ? nullptr : m_dataObject->as<dvm::DVBus *>();
}

QSizeF DVBusGraphicsItem::minimalSize() const
{
    return shared::graphicsviewutils::kDefaultGraphicsItemSize;
}

void DVBusGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

void DVBusGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->setFont(font());
    painter->drawText(br, Qt::AlignCenter, entity()->titleUI());
    painter->restore();
}

shared::ColorManager::HandledColors DVBusGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Bus;
}

} // namespace dve
