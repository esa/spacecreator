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

#include "dvprocessorgraphicsitem.h"

#include "graphicsviewutils.h"

#include <QPainter>

static const qreal hShift { 40. };
static const qreal vShift { 60. };

namespace dve {

DVProcessorGraphicsItem::DVProcessorGraphicsItem(dvm::DVProcessor *entity, QGraphicsItem *parent)
    : shared::ui::VERectGraphicsItem(entity, parent)
{
}

dvm::DVProcessor *DVProcessorGraphicsItem::entity() const
{
    return m_dataObject.isNull() ? nullptr : m_dataObject->as<dvm::DVProcessor *>();
}

QSizeF DVProcessorGraphicsItem::minimalSize() const
{
    return shared::graphicsviewutils::kDefaultGraphicsItemSize;
}

QRectF DVProcessorGraphicsItem::contentRect() const
{
    return boundingRect().adjusted(0, vShift, -hShift, 0);
}

void DVProcessorGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

void DVProcessorGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->drawPolygon(generateBox());
    painter->setFont(font());
    painter->drawText(br, Qt::AlignTop | Qt::AlignHCenter, entity()->titleUI());
    painter->restore();
}

shared::ColorManager::HandledColors DVProcessorGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Processor;
}

QPolygonF DVProcessorGraphicsItem::generateBox() const
{
    const QRectF contentArea = contentRect();
    const QRectF shiftedContentArea = contentArea.translated(hShift, -vShift);
    QVector<QPointF> points = { contentArea.bottomRight(), contentArea.bottomLeft(), contentArea.topLeft(),
        contentArea.topRight(), contentArea.bottomRight(), shiftedContentArea.bottomRight(),
        shiftedContentArea.topRight(), contentArea.topRight(), contentArea.topLeft(), shiftedContentArea.topLeft(),
        shiftedContentArea.topRight() };
    return points;
}

} // namespace dve
