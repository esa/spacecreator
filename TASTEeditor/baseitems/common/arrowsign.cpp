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
#include "arrowsign.h"

#include <QTransform>

namespace taste3 {

/*!
  \class msc::ArrowSign
  \brief An arrow head/tail triangle holder

  \inmodule MscWidgets

*/

ArrowSign ArrowSign::createArrow(ArrowSign::Flow direction)
{
    QPointF pntFrom;
    ArrowSign arrow;
    arrow.m_head = pntFrom;
    if (ArrowSign::Flow::LeftToRight == direction)
        pntFrom.rx() += ARROW_WIDTH;
    else
        pntFrom.rx() -= ARROW_WIDTH;

    pntFrom.ry() -= ARROW_HEIGHT / 2.;
    arrow.m_top = pntFrom;
    pntFrom.ry() += ARROW_HEIGHT;
    arrow.m_bottom = pntFrom;

    arrow.updatePath();

    return arrow;
}

ArrowSign ArrowSign::createArrowLeft()
{
    return ArrowSign::createArrow(ArrowSign::Flow::LeftToRight);
}

ArrowSign ArrowSign::createArrowRight()
{
    return ArrowSign::createArrow(ArrowSign::Flow::RightToLeft);
}

ArrowSign::ArrowSign()
    : m_head(QPointF(0., 0.))
    , m_top(ARROW_WIDTH, -ARROW_HEIGHT / 2)
    , m_bottom(ARROW_WIDTH, -ARROW_HEIGHT / 2 + ARROW_HEIGHT)
{
    updatePath();
}

ArrowSign::ArrowSign(const ArrowSign &other)
    : m_head(other.m_head)
    , m_top(other.m_top)
    , m_bottom(other.m_bottom)
{
    updatePath();
}

QPainterPath ArrowSign::path() const
{
    return m_path;
}

void ArrowSign::pointTo(const QPointF &target, const qreal angle)
{
    m_head = target;
    QLineF line(target, target + QPointF(ARROW_WIDTH, 0));
    line.setLength(ARROW_WIDTH);
    line.setAngle(angle);
    line = QLineF(line.p2(), line.p1());
    QLineF normalVector = line.normalVector();
    normalVector.setLength(ARROW_HEIGHT / 2);
    m_bottom = normalVector.p2();
    normalVector.setAngle(normalVector.angle() - 180);
    m_top = normalVector.p2();

    updatePath();
}
void ArrowSign::pointTo(const QPointF &target)
{
    const QPointF offset = target - m_head;
    m_head += offset;
    m_top += offset;
    m_bottom += offset;

    updatePath();
}

void ArrowSign::updatePath()
{
    m_path = QPainterPath();
    m_path.moveTo(m_head);
    m_path.lineTo(m_top);
    m_path.lineTo(m_bottom);
    m_path.closeSubpath();
}
} // ns taste3
