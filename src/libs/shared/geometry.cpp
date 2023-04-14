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

#include "geometry.h"

#include <QRect>

namespace shared {

/**
   Ensures that the returned rect is inside of parentRect.
   @param sourceRect the rectangle to move and shrink if needed
   @param parentRect the rectangle that the sourceRect should fit in
 */
QRect rectInRect(const QRect &sourceRect, const QRect &parentRect)
{
    QRect rect = sourceRect;
    if (rect.width() > parentRect.width()) {
        rect.setWidth(parentRect.width());
    }
    if (rect.height() > parentRect.height()) {
        rect.setHeight(parentRect.height());
    }

    if (rect.left() < parentRect.left()) {
        rect.moveLeft(parentRect.left());
    }
    if (rect.top() < parentRect.top()) {
        rect.moveTop(parentRect.top());
    }

    if (rect.right() > parentRect.right()) {
        rect.moveRight(parentRect.right());
    }
    if (rect.bottom() > parentRect.bottom()) {
        rect.moveBottom(parentRect.bottom());
    }

    return rect;
}

/**
 * Returns if the given point is a valid one. Invalid positions are generated using INVALID_POS
 * @param pos the point to check
 */
bool isValidPosition(const QPointF &pos)
{
    return pos != INVALID_POS;
}

} // namespace shared
