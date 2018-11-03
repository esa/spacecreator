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

#pragma once

#include <QBrush>
#include <QPen>

namespace msc {

class DrawRectInfo
{
public:
    DrawRectInfo();

    DrawRectInfo(const QSizeF &rectSize, qreal borderWidth,
                 const QColor &borderColor, const QColor &bodyColor);
    QSizeF rectSize() const;
    void setRectSize(const QSizeF &sz);

    qreal borderWidth() const;
    void setBorderWidth(qreal w);

    QColor borderColor() const;
    void setBorderColor(const QColor &color);

    QColor bodyColor() const;
    void setBodyColor(const QColor &color);

    QPen border() const;
    QBrush body() const;

private:
    QSizeF m_rectSize;
    qreal m_borderWidth;
    QColor m_borderColor;
    QColor m_bodyColor;
    QPen m_border;
    QBrush m_body;

    void update();
};

} // ns msc
