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

#include "topohelper/geometry.h"

#include <QtMath>

static const QMarginsF kMargins { topohelp::kInterfaceLayoutOffset, topohelp::kInterfaceLayoutOffset,
    topohelp::kInterfaceLayoutOffset, topohelp::kInterfaceLayoutOffset };

void alignPosWithinRect(const QRectF &rect, Qt::Alignment alignment, QPointF &pos, QMarginsF margins)
{
    if (margins.top() + margins.bottom() > rect.height()) {
        const qreal margin = std::floor(rect.height() / 2.);
        margins.setTop(margin);
        margins.setBottom(margin);
    }
    if (margins.left() + margins.right() > rect.width()) {
        const qreal margin = std::floor(rect.width() / 2.);
        margins.setLeft(margin);
        margins.setRight(margin);
    }

    switch (alignment) {
    case Qt::AlignLeft:
        pos.setX(rect.left());
        pos.setY(qBound(rect.top() + margins.top(), pos.y(), rect.bottom() - margins.bottom()));
        break;
    case Qt::AlignTop:
        pos.setX(qBound(rect.left() + margins.left(), pos.x(), rect.right() - margins.right()));
        pos.setY(rect.top());
        break;
    case Qt::AlignRight:
        pos.setX(rect.right());
        pos.setY(qBound(rect.top() + margins.top(), pos.y(), rect.bottom() - margins.bottom()));
        break;
    case Qt::AlignBottom:
        pos.setX(qBound(rect.left() + margins.left(), pos.x(), rect.right() - margins.right()));
        pos.setY(rect.bottom());
        break;
    }
};

namespace shared {

struct PositionLookupHelper::PositionLookupPrivate {
    PositionLookupPrivate(const QList<QPair<Qt::Alignment, QPainterPath>> &sidePaths, const QRectF &parentRect,
            const QList<QRectF> &siblingsRects, const QPointF &pos, const topohelp::LookupDirection direction)
        : m_sidePaths(sidePaths)
        , m_siblingsRects(siblingsRects)
        , m_parentRect(parentRect)
        , m_initialSideIdx(indexFromSide(topohelp::geom::getNearestSide(parentRect, pos)))
        , m_direction(direction)
        , m_cw({ this, pos, m_initialSideIdx, topohelp::LookupDirection::Clockwise })
        , m_ccw({ this, pos, m_initialSideIdx, topohelp::LookupDirection::CounterClockwise })
        , m_mixed({ this, pos, m_initialSideIdx, topohelp::LookupDirection::Mixed })
    {
    }
    const QList<QPair<Qt::Alignment, QPainterPath>> m_sidePaths;
    const QList<QRectF> m_siblingsRects;
    const QRectF m_parentRect;
    const int m_initialSideIdx;
    const topohelp::LookupDirection m_direction;

    Qt::Alignment sideFromIndex(const int idx) const
    {
        if (idx >= 0)
            return m_sidePaths.value(idx % m_sidePaths.size()).first;

        return m_sidePaths.value((m_sidePaths.size() - qAbs(idx)) % m_sidePaths.size()).first;
    }

    int indexFromSide(Qt::Alignment side)
    {
        const auto it = std::find_if(m_sidePaths.cbegin(), m_sidePaths.cend(),
                [side](const QPair<Qt::Alignment, QPainterPath> &p) { return p.first == side; });
        return it == m_sidePaths.cend() ? -1 : std::distance(m_sidePaths.cbegin(), it);
    }

    struct DirectionHelper {
        DirectionHelper(PositionLookupHelper::PositionLookupPrivate *q, const QPointF &itemPos, int sideIdx,
                topohelp::LookupDirection direction)
            : q(q)
            , itemPos(itemPos)
            , sideIdx(sideIdx)
            , direction(direction)
        {
            alignPosWithinRect(q->m_parentRect, side(), this->itemPos, kMargins);
        }
        PositionLookupHelper::PositionLookupPrivate *q;
        QRectF intersectedRect;
        QPointF itemPos;
        int sideIdx;
        const topohelp::LookupDirection direction;

        QRectF itemRect() const { return QRectF(itemPos + originPoint(), shapeForSide().boundingRect().size()); }
        QPointF originPoint() const { return shapeForSide().boundingRect().topLeft(); }

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

        Qt::Alignment side() const { return q->sideFromIndex(sideIdx); }

        QPainterPath shapeForSide() const
        {
            auto it = std::find_if(q->m_sidePaths.cbegin(), q->m_sidePaths.cend(),
                    [s = side()](const QPair<Qt::Alignment, QPainterPath> &p) { return p.first == s; });
            return it == q->m_sidePaths.cend() ? QPainterPath() : it->second;
        }

        void nextSide()
        {
            if (direction == topohelp::LookupDirection::Clockwise || direction == topohelp::LookupDirection::Mixed) {
                ++sideIdx;
            } else if (direction == topohelp::LookupDirection::CounterClockwise) {
                --sideIdx;
            }

            // resetting transposed rect to keep origin point on another edge of function
            if (direction == topohelp::LookupDirection::Mixed) {
                switch (side()) {
                case Qt::AlignLeft:
                case Qt::AlignRight:
                    itemPos.setY(q->m_parentRect.top());
                    break;
                case Qt::AlignTop:
                case Qt::AlignBottom:
                    itemPos.setX(q->m_parentRect.left());
                    break;
                default:
                    break;
                }
            }
            alignPosWithinRect(q->m_parentRect, side(), itemPos, kMargins);
        }

        bool isBounded() const { return q->m_parentRect.contains(itemPos); }

        bool isReady() const { return isBounded() && !topohelp::geom::isCollided(q->m_siblingsRects, itemRect()); }

        bool hasNext() const
        {
            if (q->m_sidePaths.isEmpty())
                return false;

            return qAbs(sideIdx - q->m_initialSideIdx) < q->m_sidePaths.size();
        }

        bool nextRect()
        {
            if (topohelp::geom::isCollided(q->m_siblingsRects, itemRect(), &intersectedRect)) {
                auto dir = direction == topohelp::LookupDirection::Mixed
                        ? ((side() == Qt::AlignLeft || side() == Qt::AlignBottom)
                                        ? topohelp::LookupDirection::CounterClockwise
                                        : topohelp::LookupDirection::Clockwise)
                        : direction;
                const QRectF rect = topohelp::geom::adjustedRect(itemRect(), intersectedRect, side(), dir);
                itemPos = rect.topLeft() - originPoint();
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

PositionLookupHelper::PositionLookupHelper(const QList<QPair<Qt::Alignment, QPainterPath>> &sidePaths,
        const QRectF &parentRect, const QList<QRectF> &siblingsRects, const QPointF &pos,
        const topohelp::LookupDirection direction)
    : d(new PositionLookupPrivate(sidePaths, parentRect, siblingsRects, pos, direction))
{
}

PositionLookupHelper::~PositionLookupHelper() { }

bool PositionLookupHelper::lookup()
{
    auto isValid = [this](PositionLookupPrivate::DirectionHelper *helper) {
        return (d->m_direction == helper->direction
                       || (d->m_direction == topohelp::LookupDirection::Bidirectional
                               && (helper->direction == topohelp::LookupDirection::Clockwise
                                       || helper->direction == topohelp::LookupDirection::CounterClockwise)))
                && helper->hasNext();
    };

    const QVector<PositionLookupPrivate::DirectionHelper *> helpers { &d->m_cw, &d->m_ccw, &d->m_mixed };
    int count = helpers.size();
    while (count) {
        count = 0;
        for (PositionLookupPrivate::DirectionHelper *helper : helpers) {
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
    return d->m_cache ? d->m_cache->itemPos : QPointF();
}

Qt::Alignment PositionLookupHelper::side() const
{
    return d->m_cache ? d->m_cache->side() : Qt::AlignCenter;
}

} // namespace shared
