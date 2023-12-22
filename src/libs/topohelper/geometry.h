/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QLineF>
#include <QList>
#include <QPointF>
#include <QRectF>

class QPolygonF;

namespace topohelp {

using PointsList = QList<QPointF>;
using LinesList = QList<QLineF>;
using RectsList = QList<QRectF>;

enum class IntersectionType
{
    Edge,
    Single,
    Multiple,
};

enum class LookupDirection
{
    CounterClockwise = 0x1,
    Clockwise = 0x2,
    Bidirectional = CounterClockwise | Clockwise,
    Mixed,
};

static const QMarginsF kTextMargins = { 20, 20, 20, 20 };
static const QMarginsF kRootMargins = { 50, 50, 50, 50 };
static const QMarginsF kContentMargins = { 22, 22, 22, 22 };

static const QList<Qt::Alignment> kRectSides = { Qt::AlignLeft, Qt::AlignTop, Qt::AlignRight, Qt::AlignBottom };

static const QSizeF kDefaultGraphicsItemSize = { 200, 80 };
static const QSizeF kDefaultNodeGraphicsItemSize = { 160, 160 };

static const qreal kInterfaceLayoutOffset = 17.0;
static const qreal kInterfaceBaseLength = 12.0;
static const qreal kSiblingMinDistance = 8.0;

/*!
 * \brief The tolerance used to find an IVInterfaceGraphicsItem on scene (the size of a square used as a search area)
 */
static const qreal kInterfaceTolerance = 5.;

/*!
 * \brief The tolerance used to find an IVFunction(Type)GraphicsItem on scene (the size of a square used as a search
 * area)
 */
static const qreal kFunctionTolerance = 20.;

/*!
 * Length of the helper lines to the left and right
 */
static const qreal kHelperLineLength = 9999.;

namespace geom {

PointsList simplifyPoints(const PointsList &points);
QLineF ifaceSegment(const QRectF &sceneRect, const QPointF &firstEndPoint, const QPointF &lastEndPoint);
PointsList path(const RectsList &existingRects, const QLineF &startDirection, const QLineF &endDirection);
PointsList path(const RectsList &existingRects, const QPointF &startPoint, const QPointF &endPoint);
Qt::Alignment getNearestSide(const QRectF &boundingArea, const QPointF &pos);
PointsList intersectionPoints(const QRectF &rect, const QPolygonF &polygon);
qreal calcDistance(const QPointF &pnt1, const QPointF &pnt2);
QRectF getNearestIntersectedRect(
        const RectsList &existingRects, const PointsList &points, IntersectionType intersectionType);
bool intersects(const QRectF &rect, const QLineF &line, QPointF *intersectPos = nullptr);
bool intersects(const QRectF &rect, const QPolygonF &polygon, QPointF *intersectPos = nullptr);
PointsList generateSegments(const QLineF &startDirection, const QLineF &endDirection);
QList<PointsList> generateSegments(const QPointF &startPoint, const QPointF &endPoint);
QRectF adjustedRect(const QRectF &itemRect, const QRectF &intersectedItemRect, const Qt::Alignment side,
        const LookupDirection direction);
bool isCollided(const RectsList &itemRects, const QRectF &itemRect, QRectF *collidingRect = nullptr);
bool isRectBounded(const QRectF &outerRect, const QRectF &innerRect);
QRectF adjustFromPoint(const QPointF &pos, const qreal &adjustment);
QRectF collidingRect(const QRectF &rect, const RectsList &existingRects);
void findGeometryForRect(QRectF &itemRect, QRectF &boundedRect, const RectsList &existingRects = {},
        const QMarginsF &margins = topohelp::kRootMargins);
PointsList findPath(const RectsList &existingRects, const QLineF &startDirection, const QLineF &endDirection,
        QRectF *intersectedRect = nullptr);
QList<PointsList> findSubPath(
        const QRectF &itemRect, const PointsList &prevPoints, const PointsList &nextPoints, bool strict = true);

QVector<qint32> coordinates(const QPointF &point);
QVector<qint32> coordinates(const QRectF &rect);
QVector<qint32> coordinates(const PointsList &points);
bool comparePolygones(const PointsList &v1, const PointsList &v2);
QPointF getSidePosition(const QRectF &boundingArea, const QPointF &pos, Qt::Alignment side);
bool alignedLine(QLineF &line, int angleTolerance = 10);

QPointF pos(const QVector<qint32> &coordinates);
QRectF rect(const QVector<qint32> &coordinates);
PointsList polygon(const QVector<qint32> &coordinates);

}

} // namespace topohelp
