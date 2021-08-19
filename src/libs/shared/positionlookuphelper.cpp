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
#include "graphicsviewutils.h"
#include <QtMath>

namespace gu = shared::graphicsviewutils;

static const QMarginsF kMargins { gu::kInterfaceLayoutOffset, gu::kInterfaceLayoutOffset,
    gu::kInterfaceLayoutOffset, gu::kInterfaceLayoutOffset };

namespace shared {

struct PositionLookupHelper::PositionLookupPrivate {
    PositionLookupPrivate(const QList<QPair<Qt::Alignment, QPainterPath> > &sidePaths,
            const QRectF &parentRect, const QList<QRectF> &siblingsRects, const QRectF &itemRect,
            const QPointF &originPoint, const gu::LookupDirection direction)
        : m_sidePaths(sidePaths)
        , m_siblingsRects(siblingsRects)
        , m_parentRect(parentRect)
        , m_initialSideIdx(indexFromSide(gu::getNearestSide(parentRect, itemRect.topLeft() - originPoint)))
        , m_direction(direction)
        , m_cw({ this, itemRect, m_initialSideIdx, gu::LookupDirection::Clockwise })
        , m_ccw({ this, itemRect, m_initialSideIdx, gu::LookupDirection::CounterClockwise })
        , m_mixed({ this, itemRect, m_initialSideIdx, gu::LookupDirection::Mixed })
    {}
    const QList<QPair<Qt::Alignment, QPainterPath>> m_sidePaths;
    const QList<QRectF> m_siblingsRects;
    const QRectF m_parentRect;
    const int m_initialSideIdx;
    const gu::LookupDirection m_direction;

    Qt::Alignment sideFromIndex(const int idx) const
    {
        if (idx >= 0)
            return m_sidePaths.value(idx % m_sidePaths.size()).first;

        return m_sidePaths.value((m_sidePaths.size() - qAbs(idx)) % m_sidePaths.size()).first;
    }

    int indexFromSide(Qt::Alignment side)
    {
        const auto it = std::find_if(m_sidePaths.cbegin(), m_sidePaths.cend(),
                [side](const QPair<Qt::Alignment, QPainterPath> &p){
                    return p.first == side;
                });
        return it == m_sidePaths.cend() ? -1 : std::distance(m_sidePaths.cbegin(), it);
    }

    struct DirectionHelper {
        DirectionHelper(PositionLookupHelper::PositionLookupPrivate *q, const QRectF &itemRect,
                        int sideIdx, gu::LookupDirection direction)
            : q(q), itemRect(itemRect), sideIdx(sideIdx), direction(direction)
        {
            originPoint = shapeForSide().boundingRect().topLeft();
            if (!gu::isCollided(q->m_siblingsRects, itemRect, &m_intersectedRect) && mappedOriginPoint().isNull()) {
                this->itemRect = gu::alignRectToSide(q->m_parentRect, itemRect, side(), originPoint, kMargins);
            }
        }
        PositionLookupHelper::PositionLookupPrivate *q;
        QRectF m_intersectedRect;
        QRectF itemRect;
        QPointF originPoint;
        int sideIdx;
        const gu::LookupDirection direction;

        bool lookup()
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

        Qt::Alignment side() const
        {
            return q->sideFromIndex(sideIdx);
        }

        QPointF mappedOriginPoint() const
        {
            return itemRect.topLeft() - originPoint;
        }

        QPainterPath shapeForSide() const
        {
            auto it = std::find_if(q->m_sidePaths.cbegin(), q->m_sidePaths.cend(),
                    [s = side()](const QPair<Qt::Alignment, QPainterPath> &p){
                        return p.first == s;
                    });
            return it == q->m_sidePaths.cend() ? QPainterPath() : it->second;
        }

        void nextSide()
        {
            if (direction == gu::LookupDirection::Clockwise
                    || direction == gu::LookupDirection::Mixed) {
                ++sideIdx;
            } else if (direction == gu::LookupDirection::CounterClockwise) {
                --sideIdx;
            }

            // resetting origin point of rotated rect will be placed on another side of function
            originPoint = shapeForSide().boundingRect().topLeft();

            // resetting transposed rect to keep origin point on another edge of function
            if (direction == gu::LookupDirection::Mixed) {
                switch (side()) {
                case Qt::AlignLeft:
                case Qt::AlignRight:
                    itemRect = gu::alignRectToSide(q->m_parentRect, itemRect, Qt::AlignTop, originPoint, kMargins);
                    break;
                case Qt::AlignTop:
                case Qt::AlignBottom:
                    itemRect = gu::alignRectToSide(q->m_parentRect, itemRect, Qt::AlignLeft, originPoint, kMargins);
                    break;
                default:
                    break;
                }
            }
            itemRect = gu::alignRectToSide(q->m_parentRect, itemRect, side(), originPoint, kMargins);
        }

        bool isBounded() const
        {
            return q->m_parentRect.contains(mappedOriginPoint());
        }

        bool isReady() const
        {
            return isBounded() && !gu::isCollided(q->m_siblingsRects, itemRect);
        }

        bool hasNext() const
        {
            if (q->m_sidePaths.isEmpty())
                return false;

            return qAbs(sideIdx - q->m_initialSideIdx) < q->m_sidePaths.size();
        }

        bool nextRect()
        {
            if (gu::isCollided(q->m_siblingsRects, itemRect, &m_intersectedRect)) {
                auto dir = direction == gu::LookupDirection::Mixed
                        ? ((side() == Qt::AlignLeft || side() == Qt::AlignBottom) ? gu::LookupDirection::CounterClockwise
                                                                                  : gu::LookupDirection::Clockwise)
                        : direction;
                itemRect = gu::adjustedRect(itemRect, m_intersectedRect, side(), dir);
                return true;
            }
            return false;
        }
    };

    DirectionHelper m_cw;
    DirectionHelper m_ccw;
    DirectionHelper m_mixed;

    DirectionHelper *m_cache { nullptr };
};



PositionLookupHelper::PositionLookupHelper(const QList<QPair<Qt::Alignment, QPainterPath> > &sidePaths,
        const QRectF &parentRect, const QList<QRectF> &siblingsRects, const QRectF &itemRect,
        const QPointF &originPoint, const gu::LookupDirection direction)
    : d(new PositionLookupPrivate(sidePaths, parentRect, siblingsRects, itemRect, originPoint, direction))
{
}

PositionLookupHelper::~PositionLookupHelper()
{

}

bool PositionLookupHelper::lookup()
{
    auto isValid = [this](PositionLookupPrivate::DirectionHelper *helper){
        return (d->m_direction == helper->direction || (d->m_direction == gu::LookupDirection::Bidirectional
                        && (helper->direction == gu::LookupDirection::Clockwise
                            || helper->direction == gu::LookupDirection::CounterClockwise))) && helper->hasNext();
    };

    const QVector<PositionLookupPrivate::DirectionHelper *> helpers { &d->m_cw, &d->m_ccw, &d->m_mixed };
    int count = helpers.size();
    while (count) {
        count = 0;
        for (PositionLookupPrivate::DirectionHelper *helper: { &d->m_cw, &d->m_ccw, &d->m_mixed }) {
            if (isValid(helper)) {
                count += 1;
                if (helper->lookup()) {
                    d->m_cache = helper;
                    return true;
                }
            }
        }
    }

    return false;
}

bool PositionLookupHelper::isSideChanged() const
{
    return d->m_cache ? d->m_cache->sideIdx != d->m_initialSideIdx : false;
}

QPointF PositionLookupHelper::mappedOriginPoint() const
{
    return d->m_cache ? d->m_cache->mappedOriginPoint() : QPointF();
}

Qt::Alignment PositionLookupHelper::side() const
{
    return d->m_cache ? d->m_cache->side() : Qt::AlignCenter;
}

} // namespace shared
