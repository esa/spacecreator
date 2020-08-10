/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QHash>
#include <QPainterPath>
#include <qnamespace.h>

namespace aadlinterface {

/*!
   \class aadlinterface::PositionLookupHelper
   \brief Class to find free place

    Class is used to find the place for \a itemRect
    keeping \a originPoint on the edge of on \a parentRect
    avoiding colliding with \a siblingsRects moving \a clockwise
    according to value set

    \arg \c sidePaths are possible shapes (geometries) for parent rect sides
    \arg \c parentRect is the geometry where \a itemRect should be placed aligned
    \arg \c siblingsRects are other rects to avoid colliding with
    \arg \c itemRect is the initial value of rect should be aligned
    \arg \c originPoint is the initial origin point position
    \arg \c clockwise moving if \a true otherwise counterclockwise
 */

class PositionLookupHelper
{
public:
    PositionLookupHelper(const QHash<Qt::Alignment, QPainterPath> &sidePaths, const QRectF &parentRect,
            const QList<QRectF> &siblingsRects, const QRectF &itemRect, const QPointF &originPoint,
            const bool clockwise);

    /*!
     * Performs next lookup iteration
     */
    bool lookup();

    bool hasNext() const;

    /*!
     * Checks if place was found
     */
    bool isReady() const;

    /*!
     * Performs next search on current side
     */
    bool nextRect();

    /*!
     * Moves to another side according to \a clockwise set
     */
    void nextSide();

    /*!
     * Checks if origin point \a m_offset placed within \a parentRect
     */
    bool isBounded() const;

    /*!
     * Checks if side was changed during previous searches
     */
    bool isSideChanged() const;

    /*!
     * Current origin point, result of the search. Is used to position item
     * if it's ready
     */
    QPointF mappedOriginPoint() const;

    /*!
     * Current side of parent rect there \a itemRect is placed
     */
    Qt::Alignment side() const;

private:
    const QHash<Qt::Alignment, QPainterPath> m_sidePaths;
    const QList<QRectF> m_siblingsRects;
    const QRectF m_parentRect;
    const int m_initialSideIdx;

    QRectF m_intersectedRect;
    QRectF m_itemRect;
    QPointF m_offset;
    int m_sideIdx;
    bool m_clockwise;
};

} // namespace aadlinterface
