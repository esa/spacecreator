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

#include <QPointF>
#include <QPainterPath>

namespace msc {

class ArrowSign
{
public:
    static constexpr qreal ARROW_HEIGHT = 10.0;
    static constexpr qreal ARROW_WIDTH = 20.0;

    ArrowSign();
    ArrowSign(const ArrowSign &other);

    QPointF head() const { return m_head; }
    QPointF top() const { return m_top; }
    QPointF bottom() const { return m_bottom; }

    QPainterPath path() const;
    void pointTo(const QPointF &target);

    static ArrowSign createArrowLeft();
    static ArrowSign createArrowRight();

private:
    enum class Flow
    {
        LeftToRight = 0,
        RightToLeft
    };

    ArrowSign &operator=(const ArrowSign &) = delete;
    QPointF m_head, m_top, m_bottom;
    QPainterPath m_path;

    static ArrowSign createArrow(ArrowSign::Flow direction);
    void updatePath();
};

} // ns msc
