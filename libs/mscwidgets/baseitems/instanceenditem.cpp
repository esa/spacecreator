/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "instanceenditem.h"

#include <QBrush>
#include <QDebug>
#include <QPainter>
#include <QPen>

namespace msc {

InstanceEndItem::InstanceEndItem(bool stop, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setStopImpl(stop);
}

bool InstanceEndItem::isStop() const
{
    return m_stop;
}

void InstanceEndItem::setStop(bool stop)
{
    if (isStop() == stop)
        return;
    setStopImpl(stop);
}

void InstanceEndItem::setStopImpl(bool stop)
{
    m_stop = stop;
    m_bgrColor = m_stop ? Qt::white : Qt::black;

    setBrush(QBrush(m_bgrColor));

    // Note: explicit update currently is not needed
}

void InstanceEndItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if (isStop()) {
        const QRectF &bounds = rect();
        const QVector<QPointF> linesHeads = { bounds.topLeft(), bounds.bottomRight(), bounds.bottomLeft(),
                                              bounds.topRight() };

        QPen pen(this->pen());
        pen.setWidthF(1.5);
        pen.setColor(Qt::black);
        pen.setCapStyle(Qt::RoundCap);

        painter->save();

        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(pen);
        painter->drawLines(linesHeads);

        painter->restore();
    } else {
        QGraphicsRectItem::paint(painter, option, widget);
    }
}

void InstanceEndItem::setRect(const QRectF &r)
{
    QRectF newRect(r);
    if (isStop()) {
        // make square
        const qreal side = qMin(r.width(), height());
        newRect.setWidth(side);
        newRect.setHeight(side);
        newRect.moveCenter(r.center());
    }

    QGraphicsRectItem::setRect(newRect);
}

qreal InstanceEndItem::height() const
{
    return isStop() ? StopSymbolHeight : EndSymbolHeight;
}

} // ns msc
