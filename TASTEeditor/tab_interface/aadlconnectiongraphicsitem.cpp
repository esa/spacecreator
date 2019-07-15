/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlconnectiongraphicsitem.h"

#include <QtDebug>

static const qreal kDefaulPenWidth = 2.0;

namespace taste3 {
namespace aadl {

AADLConnectionGraphicsItem::AADLConnectionGraphicsItem(QGraphicsItem *parentItem)
    : InteractiveObject(nullptr, parentItem)
    , m_item(new QGraphicsPathItem(this))
{
    setObjectName(QLatin1String("AADLConnectionGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemHasNoContents);
    m_item->setPen(QPen(Qt::black, kDefaulPenWidth));
}

void AADLConnectionGraphicsItem::setPoints(const QVector<QPointF> &points)
{
    prepareGeometryChange();
    QPainterPath pp;
    pp.addPolygon(QPolygonF(points));
    m_item->setPath(pp);
    m_boundingRect = pp.boundingRect();
}

QPainterPath AADLConnectionGraphicsItem::shape() const
{
    return m_item->path();
}

QVariant AADLConnectionGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedChange) {
        QPen pen = m_item->pen();
        pen.setWidthF(value.toBool() ? 1.5 * kDefaulPenWidth : kDefaulPenWidth);
        pen.setStyle(value.toBool() ? Qt::DotLine : Qt::SolidLine);
        m_item->setPen(pen);
    }

    return InteractiveObject::itemChange(change, value);
}

} // namespace aadl
} // namespace taste3
