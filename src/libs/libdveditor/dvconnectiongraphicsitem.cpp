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
    : shared::ui::VEConnectionGraphicsItem(connection, startItem, endItem, parent)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemHasNoContents | QGraphicsItem::ItemClipsToShape);
}

DVConnectionGraphicsItem::~DVConnectionGraphicsItem() { }

dvm::DVConnection *dve::DVConnectionGraphicsItem::entity() const
{
    return m_dataObject.isNull() ? nullptr : m_dataObject->as<dvm::DVConnection *>();
}

int DVConnectionGraphicsItem::itemLevel(bool isSelected) const
{
    return isSelected ? 1 : 0;
}

shared::ColorManager::HandledColors DVConnectionGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::BusConnection;
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
