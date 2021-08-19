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

#include "graphicsviewutils.h"

#include <QHash>
#include <QPainterPath>
#include <qnamespace.h>
#include <memory>

namespace shared {

/*!
   \class shared::PositionLookupHelper
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
    explicit PositionLookupHelper(const QList<QPair<Qt::Alignment, QPainterPath>> &sidePaths, const QRectF &parentRect,
            const QList<QRectF> &siblingsRects, const QRectF &itemRect, const QPointF &originPoint,
            const shared::graphicsviewutils::LookupDirection direction = shared::graphicsviewutils::LookupDirection::Bidirectional);

    ~PositionLookupHelper();

    /*!
     * Performs search for free space for itemRect
     */
    bool lookup();

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
     * Current side of parent rect where \a itemRect is placed
     */
    Qt::Alignment side() const;

private:
    struct PositionLookupPrivate;
    std::unique_ptr<PositionLookupPrivate> d;
};

} // namespace shared
