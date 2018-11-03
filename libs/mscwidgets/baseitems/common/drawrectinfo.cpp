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
#include "drawrectinfo.h"

namespace msc {

DrawRectInfo::DrawRectInfo()
    : DrawRectInfo(QSizeF(12., 12.), 0.5, Qt::red,
                   { QColor::fromRgbF(0, 0, 0.5, 0.75) }) {}

DrawRectInfo::DrawRectInfo(const QSizeF &rectSize, qreal borderWidth,
                           const QColor &borderColor, const QColor &bodyColor)
    : m_rectSize(rectSize)
    , m_borderWidth(borderWidth)
    , m_borderColor(borderColor)
    , m_bodyColor(bodyColor)
    , m_border(m_borderColor)
    , m_body(m_bodyColor)
{
    m_border.setWidthF(m_borderWidth);
    update();
}

QSizeF DrawRectInfo::rectSize() const
{
    return m_rectSize;
}

void DrawRectInfo::setRectSize(const QSizeF &sz)
{
    if (sz == m_rectSize)
        return;

    m_rectSize = sz;
    update();
}

qreal DrawRectInfo::borderWidth() const
{
    return m_borderWidth;
}

void DrawRectInfo::setBorderWidth(qreal w)
{
    if (!qFuzzyCompare(w + 1., m_borderWidth + 1.))
        return;

    m_borderWidth = w;
    update();
}

QColor DrawRectInfo::borderColor() const
{
    return m_borderColor;
}

void DrawRectInfo::setBorderColor(const QColor &color)
{
    if (color == m_borderColor)
        return;

    m_borderColor = color;
    update();
}

QColor DrawRectInfo::bodyColor() const
{
    return m_bodyColor;
}

void DrawRectInfo::setBodyColor(const QColor &color)
{
    if (color == m_bodyColor)
        return;

    m_bodyColor = color;
    update();
}

QPen DrawRectInfo::border() const
{
    return m_border;
}

QBrush DrawRectInfo::body() const
{
    return m_body;
}

void DrawRectInfo::update()
{
    m_border.setColor(m_borderColor);
    m_border.setWidthF(m_borderWidth);
    m_body = QBrush(m_bodyColor);
};

} // ns msc
