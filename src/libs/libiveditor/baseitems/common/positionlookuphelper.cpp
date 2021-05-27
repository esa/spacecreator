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

#include "positionlookuphelper.h"

#include <QTransform>
#include <QtDebug>
#include <QtMath>

namespace ive {

PositionLookupHelper::PositionLookupHelper(const QHash<Qt::Alignment, QPainterPath> &sidePaths,
        const QRectF &parentRect, const QList<QRectF> &siblingsRects, const QRectF &itemRect,
        const QPointF &originPoint, const shared::graphicsviewutils::LookupDirection direction)
    : m_sidePaths(sidePaths)
    , m_siblingsRects(siblingsRects)
    , m_parentRect(parentRect)
    , m_initialSideIdx(shared::graphicsviewutils::kRectSides.indexOf(
              shared::graphicsviewutils::getNearestSide(parentRect, itemRect.topLeft() - originPoint)))
    , m_intersectedRect()
    , m_itemRect(itemRect)
    , m_offset(originPoint)
    , m_direction(direction)
    , m_sideIdx(m_initialSideIdx)
{
    shared::graphicsviewutils::isCollided(m_siblingsRects, m_itemRect, &m_intersectedRect);
}

bool PositionLookupHelper::lookup()
{
    if (!hasNext())
        return false;

    nextRect();
    if (isReady())
        return true;
    else if (!isBounded())
        nextSide();
    return false;
}

bool PositionLookupHelper::isReady() const
{
    return isBounded() && !shared::graphicsviewutils::isCollided(m_siblingsRects, m_itemRect);
}

bool PositionLookupHelper::hasNext() const
{
    return qAbs(m_sideIdx - m_initialSideIdx) < shared::graphicsviewutils::kRectSides.size();
}

bool PositionLookupHelper::nextRect()
{
    if (shared::graphicsviewutils::isCollided(m_siblingsRects, m_itemRect, &m_intersectedRect)) {
        m_itemRect = shared::graphicsviewutils::adjustedRect(
                m_itemRect, m_intersectedRect, shared::graphicsviewutils::sideFromIndex(m_sideIdx), m_direction);
        return true;
    }
    return false;
}

void PositionLookupHelper::nextSide()
{
    if (m_direction == shared::graphicsviewutils::LookupDirection::Clockwise)
        ++m_sideIdx;
    else
        --m_sideIdx;

    QPainterPath sideShape = m_sidePaths.value(side());
    // resetting origin point of rotated rect will be placed on another side of function
    m_offset = sideShape.boundingRect().topLeft();

    // resetting transposed rect to keep origin point on another edge of function
    sideShape.translate(m_itemRect.topLeft() - m_offset);
    m_itemRect = shared::graphicsviewutils::alignRectToSide(
            m_parentRect, sideShape.boundingRect(), shared::graphicsviewutils::sideFromIndex(m_sideIdx), m_offset);
}

bool PositionLookupHelper::isBounded() const
{
    return m_parentRect.contains(m_itemRect.topLeft() - m_offset);
}

bool PositionLookupHelper::isSideChanged() const
{
    return m_initialSideIdx != m_sideIdx;
}

QPointF PositionLookupHelper::mappedOriginPoint() const
{
    return m_itemRect.topLeft() - m_offset;
}

Qt::Alignment PositionLookupHelper::side() const
{
    return shared::graphicsviewutils::sideFromIndex(m_sideIdx);
}

} // namespace ive
