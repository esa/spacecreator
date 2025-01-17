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

#include "geometry.h"

#include <QDebug>
#include <QPolygonF>
#include <QtMath>

#define WRN qWarning() << Q_FUNC_INFO

namespace topohelp {

namespace geom {

static const qreal kConnectionMargin = 16.0;
static const qreal kMinSegmentLength = 10.0;
static const qreal kMinSegmentAngle = 5.0;

/*!
 * \brief Helper to detect if the \a line intersects the \a rect.
 * Coordinates of the intersection point stored in \a intersectPos.
 * Returns \c true if \a rect and \a line are not null and intersected.
 */
bool intersects(const QRectF &rect, const QLineF &line, QPointF *intersectPos)
{
    if (rect.isNull() || line.isNull()) {
        return false;
    }

    const LinesList rectLines = {
        { rect.topLeft(), rect.topRight() },
        { rect.topRight(), rect.bottomRight() },
        { rect.bottomRight(), rect.bottomLeft() },
        { rect.bottomLeft(), rect.topLeft() },
    };

    const bool hasIntersection =
            std::any_of(rectLines.cbegin(), rectLines.cend(), [&line, &intersectPos](const QLineF &rectLine) {
                return rectLine.intersects(line, intersectPos) == QLineF::BoundedIntersection;
            });

    return hasIntersection;
}

/*!
 * \brief Helper to detect if at least one sub segment of the \a polygon intersects the \a rect.
 * Coordinates of the intersection point stored in \a intersctPos.
 * Returns \c true if a sub segment of the \a polygon intersects the \a rect.
 */
bool intersects(const QRectF &rect, const QPolygonF &polygon, QPointF *intersectPos)
{
    for (int idx = 1; idx < polygon.size(); ++idx) {
        if (intersects(rect, QLineF(polygon.value(idx - 1), polygon.value(idx)), intersectPos)) {
            return true;
        }
    }
    return false;
}

/*!
 * Returns the edge location of \a boundingArea's which is most close to the \a pos.
 */
Qt::Alignment getNearestSide(const QRectF &boundingArea, const QPointF &pos)
{
    if (!boundingArea.isValid()) {
        return Qt::AlignCenter;
    }

    Qt::Alignment side = Qt::AlignCenter;
    if (boundingArea.contains(pos)) {
        qreal distance = std::numeric_limits<qreal>::max();
        const qreal leftDistance = pos.x() - boundingArea.left();
        if (leftDistance < distance) {
            distance = leftDistance;
            side = Qt::AlignLeft;
        }
        const qreal topDistance = pos.y() - boundingArea.top();
        if (topDistance < distance) {
            distance = topDistance;
            side = Qt::AlignTop;
        }
        const qreal rightDistance = boundingArea.right() - pos.x();
        if (rightDistance < distance) {
            distance = rightDistance;
            side = Qt::AlignRight;
        }
        const qreal bottomDistance = boundingArea.bottom() - pos.y();
        if (bottomDistance < distance) {
            distance = bottomDistance;
            side = Qt::AlignBottom;
        }
    } else {
        const QVector<QPair<QLineF, Qt::Alignment>> rectLines = {
            { QLineF(boundingArea.bottomLeft(), boundingArea.topLeft()), Qt::AlignLeft },
            { QLineF(boundingArea.topLeft(), boundingArea.topRight()), Qt::AlignTop },
            { QLineF(boundingArea.topRight(), boundingArea.bottomRight()), Qt::AlignRight },
            { QLineF(boundingArea.bottomRight(), boundingArea.bottomLeft()), Qt::AlignBottom },
        };
        const QLineF line { boundingArea.center(), pos };
        auto it = std::find_if(
                rectLines.constBegin(), rectLines.constEnd(), [line](const QPair<QLineF, Qt::Alignment> &rectSide) {
                    QPointF dummyPoint;
                    return line.intersects(rectSide.first, &dummyPoint) == QLineF::BoundedIntersection;
                });
        if (it != rectLines.constEnd()) {
            side = it->second;
        }
    }
    return side;
}

/*!
 * Returns a normal vector from the \a sceneRect's edge closest to the \a point.
 *
 * TODO: check the direction of the resulting normal, does it always point "outside"?
 */
QLineF getDirection(const QRectF &sceneRect, const QPointF &point)
{
    switch (getNearestSide(sceneRect, point)) {
    case Qt::AlignTop:
        return QLineF(sceneRect.topLeft(), sceneRect.topRight()).normalVector();
    case Qt::AlignBottom:
        return QLineF(sceneRect.bottomRight(), sceneRect.bottomLeft()).normalVector();
    case Qt::AlignLeft:
        return QLineF(sceneRect.bottomLeft(), sceneRect.topLeft()).normalVector();
    case Qt::AlignRight:
        return QLineF(sceneRect.topRight(), sceneRect.bottomRight()).normalVector();
    default:
        break;
    }
    return QLineF();
};

/*!
 * Generates a line connecting the \a firstEndPoint to the \a lastEndPoint:
 * \param sceneRect as a geometry of an item where connected \a IVInterfaceGraphicsItem is placed
 * \param firstEndPoint as starting point
 * \param lastEndPoint as point used to find direction of segment (into/out the \a sceneRect)
 * \return the resulting line.
 */
QLineF ifaceSegment(const QRectF &sceneRect, const QPointF &firstEndPoint, const QPointF &lastEndPoint)
{
    QLineF vector = getDirection(sceneRect, firstEndPoint);
    if (vector.isNull()) {
        return {};
    }

    vector.translate(firstEndPoint - vector.p1());
    vector.setLength(kConnectionMargin);

    if (sceneRect.contains(lastEndPoint)) {
        static constexpr int deg180 = 180;
        vector.setAngle(deg180 + vector.angle());
    }

    return vector;
}

/*!
 * Generates angled segments connecting a \a startDirection to \a endDirection points
 * \param startDirection the "previous" segment
 * \param endDirection as the "next" segment
 * \return set of points making the resulting connection path
 */
PointsList generateSegments(const QLineF &startDirection, const QLineF &endDirection)
{
    if (startDirection.isNull() || endDirection.isNull()) {
        return {};
    }

    PointsList connectionPoints { startDirection.p2(), endDirection.p2() };
    const qreal angle = startDirection.angleTo(endDirection);
    static const qreal tolerance = 0.1;
    if (std::abs(std::sin(qDegreesToRadians(angle))) <= tolerance) { // ||
        const QPointF mid = QLineF(connectionPoints.first(), connectionPoints.last()).center();
        QLineF midLine { mid, QPointF(0, 0) };
        static constexpr int deg90 = 90;
        midLine.setAngle(startDirection.angle() - deg90);

        QPointF startLastPoint;
        midLine.intersects(startDirection, &startLastPoint);
        connectionPoints.insert(connectionPoints.size() - 1, startLastPoint);

        QPointF endLastPoint;
        midLine.intersects(endDirection, &endLastPoint);
        connectionPoints.insert(connectionPoints.size() - 1, endLastPoint);
    } else { // |_, \_, /_, etc
        QPointF mid;
        startDirection.intersects(endDirection, &mid);
        connectionPoints.insert(connectionPoints.size() - 1, mid);
    }
    return connectionPoints;
}

/*!
 * \brief Helper to detect if each sub segment of the \a polygon intersects the \a rect.
 * Returns coordinates of intersection points.
 */
PointsList intersectionPoints(const QRectF &rect, const QPolygonF &polygon)
{
    if (rect.isNull() || polygon.isEmpty()) {
        return {};
    }

    const LinesList rectLines = {
        { rect.topLeft(), rect.topRight() },
        { rect.topRight(), rect.bottomRight() },
        { rect.bottomRight(), rect.bottomLeft() },
        { rect.bottomLeft(), rect.topLeft() },
    };

    QPointF intersectPos;
    PointsList points;
    for (int idx = 1; idx < polygon.size(); ++idx) {
        const QLineF line = { polygon.value(idx - 1), polygon.value(idx) };
        for (const QLineF &rectLine : rectLines) {
            if (rectLine.intersects(line, &intersectPos) == QLineF::BoundedIntersection) {
                points.append(intersectPos);
            }
        }
    }
    return points;
}

/*!
 * \brief Returns the distance between \a p1 and \a p2.
 *
 * TODO: check why it is not the QLineF.length() call?
 */
qreal calcDistance(const QPointF &pnt1, const QPointF &pnt2)
{
    return QLineF(pnt1, pnt2).length();
}

/*!
 * Returns the nearest rect from \a existingRects which intersects with \a points according to \a intersectionType:
 *  \p Edge - any intersection point, even located on the edge of rect
    \p Single - intersection point should be located inside the rect, and only one
    \p Multiple - 2+ intersection points
 */
QRectF getNearestIntersectedRect(
        const RectsList &existingRects, const PointsList &points, IntersectionType intersectionType)
{
    if (points.size() < 2) {
        return {};
    }

    struct SearchIndex {
        QRectF closestIntersectedRect;
        qreal distance = 0;
        int sectionIdx = -1;
    };
    QList<SearchIndex> indexes;
    for (int idx = 1; idx < points.size(); ++idx) {
        const QLineF line { points.at(idx - 1), points.at(idx) };
        if (qFuzzyIsNull(line.length())) {
            continue;
        }
        for (auto it = existingRects.cbegin(); it != existingRects.cend(); ++it) {
            QPointF intersectionPoint;
            if (intersectionType == IntersectionType::Edge
                    && !topohelp::geom::intersects(*it, line, &intersectionPoint)) {
                continue;
            } else {
                const auto intersectPoints =
                        topohelp::geom::intersectionPoints(*it, QPolygonF(PointsList { line.p1(), line.p2() }));
                if (intersectPoints.isEmpty()) {
                    continue;
                }
                if (intersectionType == IntersectionType::Multiple) {
                    if (intersectPoints.size() <= 1) {
                        continue;
                    }
                } else {
                    const int properPointsCount = std::count_if(
                            intersectPoints.cbegin(), intersectPoints.cend(), [points](const QPointF &point) {
                                return points.first() != point && points.last() != point;
                            });
                    if (properPointsCount == 0) {
                        continue;
                    }
                }
                std::for_each(intersectPoints.cbegin(), intersectPoints.cend(),
                        [&intersectionPoint, sectionStartPoint = line.p1()](const QPointF &pnt) {
                            if (calcDistance(sectionStartPoint, intersectionPoint)
                                    > calcDistance(pnt, sectionStartPoint)) {
                                intersectionPoint = pnt;
                            }
                        });
            }
            indexes << SearchIndex { *it, QLineF(line.p1(), intersectionPoint).length(), idx };
        }
    }

    std::sort(indexes.begin(), indexes.end(), [](const SearchIndex &sh1, const SearchIndex &sh2) {
        return sh1.sectionIdx == sh2.sectionIdx ? sh1.distance < sh2.distance : sh1.sectionIdx < sh2.sectionIdx;
    });

    return indexes.isEmpty() ? QRectF() : indexes.first().closestIntersectedRect;
}

/*!
 * Generates the path for connecting the \a startPoint to the \a endPoint
 * \param existingRects geometries of existing items in scene coordinates that should be checked on collisions
 * \param startDirection as first polyline segment
 * \param endDirection as last polyline segment
 * \return set of points making the resulting polyline
 */
PointsList findPath(const RectsList &existingRects, const QLineF &startDirection, const QLineF &endDirection,
        QRectF *intersectedRect)
{
    PointsList points = generateSegments(startDirection, endDirection);
    const QRectF rect = getNearestIntersectedRect(existingRects, points, IntersectionType::Single);
    if (!rect.isValid()) {
        return points;
    }

    if (intersectedRect) {
        *intersectedRect = rect;
    }
    return {};
}

/*!
 * Returns the total length of all subsegments of \a polygon.
 */
qreal calcDistance(const PointsList &polygon)
{
    qreal distance = 0;
    for (auto it = std::next(polygon.constBegin()); it != polygon.constEnd(); ++it) {
        distance += calcDistance(*std::prev(it), *it);
    }
    return distance;
}

/*!
 * Returns the list of the \a area corner points, sorted by minimal distance between the corner and \a point1 or \a
 * point2.
 */
PointsList sortedCorners(const QRectF &area, const QPointF &point1, const QPointF &point2)
{
    PointsList rectPoints { area.topLeft(), area.topRight(), area.bottomLeft(), area.bottomRight() };
    std::sort(rectPoints.begin(), rectPoints.end(), [=](const QPointF &pnt1, const QPointF &pnt2) {
        return calcDistance({ point1, pnt1, point2 }) < calcDistance({ point1, pnt2, point2 });
    });
    return rectPoints;
}

/*!
 * Generates angled segments for a connection between the \a startPoint and \a endPoint
 * \param startDirection as previous point for generated one
 * \param endDirection as next point for generated one
 * \return set of points making a connection between the \a startPoint and \a endPoint
 */
QList<PointsList> generateSegments(const QPointF &startPoint, const QPointF &endPoint)
{
    if (startPoint == endPoint) {
        return {};
    }

    if (qFuzzyCompare(startPoint.x(), endPoint.x()) || qFuzzyCompare(startPoint.y(), endPoint.y())) {
        return { { startPoint, endPoint } };
    }

    return { { startPoint, { startPoint.x(), endPoint.y() }, endPoint },
        { startPoint, { endPoint.x(), startPoint.y() }, endPoint } };
}

/*!
 * Generates the segments for a connection from the \a prevPoints to the \a nextPoints
 * \param itemRect geometry of intersected item bounding box
 * \param prevPoints as starting points
 * \param nextPoints as points generated segments should be connected to
 * \param strict as a flag showing if it should check full intersection with \a itemRect
 * or some node could be placed on the edge of noticed rect
 * \return set of possible paths
 */
QList<PointsList> findSubPath(
        const QRectF &itemRect, const PointsList &prevPoints, const PointsList &nextPoints, bool strict)
{
    Q_ASSERT(itemRect.isValid());
    Q_ASSERT(!prevPoints.isEmpty());
    Q_ASSERT(!nextPoints.isEmpty());

    if (!itemRect.isValid() || prevPoints.isEmpty() || nextPoints.isEmpty()) {
        return {};
    }

    const QPointF startPoint = prevPoints.last();
    const QPointF endPoint = nextPoints.last();
    Q_ASSERT(startPoint != endPoint);
    if (startPoint == endPoint) {
        return {};
    }

    const QRectF itemRectMargins =
            itemRect.adjusted(-kConnectionMargin, -kConnectionMargin, kConnectionMargin, kConnectionMargin);
    const PointsList rectCorners = sortedCorners(itemRectMargins, startPoint, endPoint);
    QList<PointsList> allPaths;
    for (const QPointF &pnt : rectCorners) {
        const PointsList lastSegment = { pnt, endPoint };
        for (const auto &polygon : generateSegments(startPoint, pnt)) {
            const bool withoutIntersections = strict ? !topohelp::geom::intersects(itemRect, polygon)
                            && !topohelp::geom::intersects(itemRect, QLineF(pnt, endPoint))
                                                     : topohelp::geom::intersectionPoints(itemRect, polygon).size() < 2
                            && topohelp::geom::intersectionPoints(itemRect, lastSegment).size() < 2;
            if (withoutIntersections) {
                PointsList previousPoints(prevPoints);
                previousPoints.removeLast();
                previousPoints << polygon;

                PointsList uniquePreviousPoints;
                for (const QPointF& point : previousPoints) {
                    if (!uniquePreviousPoints.contains(point)) {
                        uniquePreviousPoints.append(point);
                    }
                }

                allPaths << uniquePreviousPoints;
            }
        }
    }
    return allPaths;
}

/*!
 * Generates the whole path for a connection
 * \param existingRects geometries of existing items in scene coordinates that should be checked on collisions
 * \param startDirection as first polyline segment
 * \param endDirection as last polyline segment
 * \return set of polyline nodes coordinates
 */
PointsList path(const RectsList &existingRects, const QLineF &startDirection, const QLineF &endDirection)
{
    QRectF intersectedRect;
    QList<PointsList> paths { { startDirection.p1(), startDirection.p2() } };
    while (true) {
        QList<PointsList> deeper;
        QList<PointsList> results;
        for (const auto &currPath : paths) {
            Q_ASSERT(currPath.size() >= 2);
            if (currPath.size() < 2) {
                return {};
            }

            const QLineF prevDirection { currPath.value(currPath.size() - 2), currPath.value(currPath.size() - 1) };
            PointsList shortPath = findPath(existingRects, prevDirection, endDirection, &intersectedRect);
            if (!shortPath.isEmpty()) {
                PointsList result;
                result.append(startDirection.p1());
                result.append(currPath);
                result.append(shortPath);
                result.append(endDirection.p1());
                results.append(result);
                continue;
            }
            const QList<PointsList> subPaths =
                    findSubPath(intersectedRect, currPath, { endDirection.p1(), endDirection.p2() });
            for (auto subPath : subPaths) {
                if (subPath.isEmpty()) {
                    continue;
                }

                const QRectF item = getNearestIntersectedRect(existingRects, subPath, IntersectionType::Single);
                if (item.isValid()) {
                    continue;
                } else if (subPath.last() == endDirection.p2()) {
                    results.append(subPath);
                } else {
                    deeper.append(subPath);
                }
            }
        }
        if (!results.isEmpty()) {
            std::sort(results.begin(), results.end(), [](const PointsList &pnt1, const PointsList &pont2) {
                if (pnt1.size() == pont2.size()) {
                    return calcDistance(pnt1) < calcDistance(pont2);
                }

                return pnt1.size() < pont2.size();
            });

            return results.first();
        }

        if (paths.size() != deeper.size() || !std::equal(paths.constBegin(), paths.constEnd(), deeper.constBegin())) {
            paths = deeper;
        } else if (!deeper.isEmpty()) {
            return deeper.front();
        } else {
            break;
        }
    }
    return {};
}

/*!
 * Generates the path for a connection between the \a startPoint and \a endPoint
 * \param existingRects existing items geometries in scene coordinates shouldn't been overlapped
 * \param startPoint as first polyline node
 * \param endPoint as last polyline node
 * \return set of polyline nodes coordinates
 */
PointsList path(const RectsList &existingRects, const QPointF &startPoint, const QPointF &endPoint)
{
    const PointsList points { startPoint, endPoint };
    auto item = getNearestIntersectedRect(existingRects, points, IntersectionType::Multiple);
    if (!item.isValid())
        return points;

    const QList<PointsList> possiblePaths = findSubPath(item, { startPoint }, { endPoint }, false);
    if (possiblePaths.isEmpty()) {
        return {};
    }

    QList<PointsList> paths;
    for (const PointsList &possiblePath : possiblePaths) {
        if (possiblePath.isEmpty()) {
            continue;
        }

        if (possiblePath.last() == endPoint) {
            paths.append(possiblePath);
        } else {
            const auto p = path(existingRects, possiblePath.last(), endPoint);
            if (!p.isEmpty()) {
                PointsList prevPath(possiblePath);
                prevPath.removeLast();
                paths.append(prevPath << p);
            }
        }
    }
    if (paths.isEmpty()) {
        return {};
    }

    PointsList shortestPath;
    qreal minLength = std::numeric_limits<qreal>::max();
    for (const PointsList &path : paths) {
        if (path.size() < 2) {
            continue;
        }
        const qreal length = calcDistance(path);
        if (minLength > length && !qFuzzyIsNull(length)) {
            minLength = length;
            shortestPath = path;
        }
    }
    if (shortestPath.isEmpty())
        return {};

    return shortestPath;
}

/*!
 * \brief Takes a list of points and applies a simplification algorithm to reduce the number of points
 * while preserving the essential shape of the original path.
 * The simplification is based on angle and length criteria between consecutive line segments.
 * \param points: A constant reference to the original list of points to be simplified.
 * \return PointsList: A new list of points representing the simplified path.
 * If the input list has two or fewer points, it is returned unchanged.
 */
PointsList simplifyPoints(const PointsList &points)
{
    if (points.size() <= 2) {
        return points;
    }

    static constexpr int piDeg = 180;

    auto checkLines = [](const QLineF &line1, const QLineF &line2) -> bool {
        if (line1.length() < kMinSegmentLength) {
            return true;
        }
        const int simplifiedAngle = qAbs(qRound(line1.angleTo(line2)) % piDeg);
        const bool angleOk = simplifiedAngle < kMinSegmentAngle || piDeg - simplifiedAngle < kMinSegmentAngle;
        return angleOk;
    };

    PointsList simplifiedPoints(points);
    for (int idx = 1; idx < simplifiedPoints.size() - 1;) {
        const QLineF currentLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx - 1) };
        const QLineF nextLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx + 1) };
        if (checkLines(currentLine, nextLine)) {
            simplifiedPoints.removeAt(idx);
            continue;
        }
        ++idx;
    }
    return simplifiedPoints;
}

/*!
 * Moves the \a itemRect to the \a intersectedItemRect's \a side
 * \param itemRect as the rect to adjust
 * \param intersectedItemRect as the target rect to move to
 * \param side as the direction of snapping
 * \param direction as the movement CW/CCW direction flag
 */
QRectF adjustedRect(const QRectF &itemRect, const QRectF &intersectedItemRect, const Qt::Alignment side,
        const LookupDirection direction)
{
    const bool clockwise = direction == LookupDirection::Clockwise;
    QRectF resultRect { itemRect };
    switch (side) {
    case Qt::AlignLeft:
        if (clockwise)
            resultRect.moveBottom(intersectedItemRect.top() - kInterfaceLayoutOffset);
        else
            resultRect.moveTop(intersectedItemRect.bottom() + kInterfaceLayoutOffset);
        break;
    case Qt::AlignRight:
        if (clockwise)
            resultRect.moveTop(intersectedItemRect.bottom() + kInterfaceLayoutOffset);
        else
            resultRect.moveBottom(intersectedItemRect.top() - kInterfaceLayoutOffset);
        break;
    case Qt::AlignTop:
        if (clockwise)
            resultRect.moveLeft(intersectedItemRect.right() + kInterfaceLayoutOffset);
        else
            resultRect.moveRight(intersectedItemRect.left() - kInterfaceLayoutOffset);
        break;
    case Qt::AlignBottom:
        if (clockwise)
            resultRect.moveRight(intersectedItemRect.left() - kInterfaceLayoutOffset);
        else
            resultRect.moveLeft(intersectedItemRect.right() + kInterfaceLayoutOffset);
        break;
    default:
        WRN << "Unhandled side:" << side;
        return {};
    }
    return resultRect;
}

/*!
 * Checks intersection \a itemRect with \a itemRects
 * and assign first intersection to \a collidingRect
 */
bool isCollided(const RectsList &itemRects, const QRectF &itemRect, QRectF *collidingRect)
{
    auto it = std::find_if(itemRects.cbegin(), itemRects.cend(),
            [itemRect](const QRectF &siblingRect) { return siblingRect.intersects(itemRect); });
    if (it != itemRects.cend()) {
        if (collidingRect)
            *collidingRect = *it;
        return true;
    }
    return false;
}

/*!
 * \brief checks whether an inner rectangle is completely bounded by an outer rectangle.
 * \param outerRect: A constant reference to the outer rectangle
 * \param innerRect: A constant reference to the inner rectangle.
 * \return true if the inner rectangle is valid and completely bounded by the outer rectangle.
 * Returns false if the inner rectangle is invalid or if the outer rectangle is invalid.
 */
bool isRectBounded(const QRectF &outerRect, const QRectF &innerRect)
{
    if (!innerRect.isValid()) {
        return true;
    } else if (!outerRect.isValid()) {
        return false;
    }
    return outerRect.contains(innerRect);
}

/*!
 * \brief constructs a new QRectF centered around a specified point. The size of the rectangle is determined by the
 * given adjustment value, allowing the adjustment of the rectangle's dimensions from the specified point.
 * \param pos: A constant reference to the point around which the rectangle will be centered.
 * \param adjustment: A constant reference to the adjustment value, influencing the dimensions of the resulting
 * rectangle. \return a new rectangle with dimensions adjusted based on the specified point and adjustment value.
 */
QRectF adjustFromPoint(const QPointF &pos, const qreal &adjustment)
{
    const QPointF adjustmentPoint { adjustment / 2, adjustment / 2 };
    return QRectF { pos - adjustmentPoint, pos + adjustmentPoint };
}

/*!
 * Searches location for \a itemRect within \a boundedRect that could be expanded to include mentioned rect
 * without overlapping with \a existingRects
 */
void findGeometryForRect(
        QRectF &itemRect, QRectF &boundedRect, const RectsList &existingRects, const QMarginsF &margins)
{
    using topohelp::kDefaultGraphicsItemSize;
    if (!itemRect.isValid()) {
        const QRectF contentRect = boundedRect.marginsRemoved(margins);
        const QSizeF size = contentRect.isValid() ? kDefaultGraphicsItemSize.boundedTo(contentRect.size())
                                                  : kDefaultGraphicsItemSize;
        itemRect = QRectF(contentRect.topLeft(), size);
    }
    QRectF itemGeometry(itemRect.marginsAdded(margins));

    if (!existingRects.isEmpty()) {
        static const qreal kOffset = 2;
        if (boundedRect.isValid()) {
            QRectF contentGeometry = boundedRect.marginsRemoved(margins);
            itemGeometry.moveTopLeft(contentGeometry.topLeft());

            QRectF intersectedRect = collidingRect(itemGeometry, existingRects);
            while (!intersectedRect.isNull()) {
                if (contentGeometry.right() - intersectedRect.right() > itemGeometry.width()) {
                    itemGeometry.moveLeft(intersectedRect.right() + kOffset);
                } else if (contentGeometry.bottom() - intersectedRect.bottom() > itemGeometry.height()) {
                    itemGeometry.moveLeft(contentGeometry.left() + kOffset);
                    itemGeometry.moveTop(intersectedRect.bottom() + kOffset);
                } else if (contentGeometry.width() <= contentGeometry.height()) {
                    itemGeometry.moveLeft(intersectedRect.right() + kOffset);
                    contentGeometry.setRight(itemGeometry.right());
                } else {
                    itemGeometry.moveLeft(contentGeometry.left() + kOffset);
                    itemGeometry.moveTop(intersectedRect.bottom() + kOffset);
                    contentGeometry.setBottom(itemGeometry.bottom());
                }
                intersectedRect = collidingRect(itemGeometry, existingRects);
            }
        } else {
            QRectF existingRect;
            std::for_each(existingRects.cbegin(), existingRects.cend(),
                    [&existingRect](const QRectF &r) { existingRect |= r; });
            QRectF intersectedRect = collidingRect(itemGeometry, existingRects);
            while (!intersectedRect.isNull()) {
                if (existingRect.height() > existingRect.width()) {
                    itemGeometry.moveLeft(intersectedRect.right() + kOffset);
                } else {
                    itemGeometry.moveTop(intersectedRect.bottom() + kOffset);
                }
                intersectedRect = collidingRect(itemGeometry, existingRects);
            }
        }
    }

    itemRect = itemGeometry.marginsRemoved(margins);
    QRectF newBoundingRect;
    for (const QRectF &rect : existingRects) {
        newBoundingRect |= rect;
    }
    newBoundingRect |= itemRect;
    boundedRect |= newBoundingRect.marginsAdded(margins);
}

/*!
 * Searches rect from \a existingRects that intersects with \a rect
 */
QRectF collidingRect(const QRectF &rect, const RectsList &existingRects)
{
    auto it = std::find_if(existingRects.constBegin(), existingRects.constEnd(),
            [rect](const QRectF &r) { return r.intersects(rect); });
    if (it != existingRects.constEnd()) {
        return *it;
    }
    return QRectF();
}

/*!
 * \brief converts a QPointF into a QVector of qint32 (32-bit signed integers) representing the x and y coordinates
 * of the point.
 * \param point: A constant reference to the QPointF representing the 2D point for which coordinates are to be
 * extracted.
 * \return a QVector containing two qint32 values representing the x and y coordinates of the input point. If the input
 * point is null, an empty QVector is returned.
 */
QVector<qint32> coordinates(const QPointF &point)
{
    if (point.isNull())
        return {};

    return coordinates(PointsList { point });
}

/*!
 * \brief  Converts a QRectF (floating-point 2D rectangle) into a QVector of qint32 (32-bit signed integers)
 * representing the coordinates of the top-left and bottom-right corners of the rectangle. If the input QRectF is
 * invalid, an empty QVector is returned.
 * \param rect A constant reference to the QRectF representing the 2D rectangle for which coordinates are to be
 * extracted.
 * \return QVector<qint32> Returns a QVector containing four qint32 values representing the x and y
 * coordinates of the top-left and bottom-right corners of the input rectangle. If the input rectangle is invalid, an
 * empty QVector is returned.
 */
QVector<qint32> coordinates(const QRectF &rect)
{
    if (rect.isValid())
        return coordinates(PointsList { rect.topLeft(), rect.bottomRight() });

    return {};
}

/*!
 * \brief Converts a PointsList (list of QPointF) into a QVector of qint32 (32-bit signed integers) representing the x
 * and y coordinates of each point in the list.
 * \param points A constant reference to the PointsList representing the
 * list of points for which coordinates are to be extracted.
 * \return QVector<qint32> Returns a QVector containing pairs
 * of qint32 values representing the x and y coordinates of each point in the input list.
 */
QVector<qint32> coordinates(const PointsList &points)
{
    QVector<qint32> coordinates;
    for (const QPointF &point : points)
        coordinates << qRound(point.x()) << qRound(point.y());
    return coordinates;
}

/*!
 * \brief  Compares two PointsList representing polygons to determine if they are equal. Equality is based on the
 * size of the polygons and the equality of corresponding points.
 *
 * \param v1 A constant reference to the first PointsList representing the points of the first polygon.
 * \param v2 A constant reference to the second PointsList representing the points of the second polygon.
 * \return bool Returns true if the polygons are equal (i.e., have the same size and corresponding points are equal),
 * and false otherwise.
 */
bool comparePolygones(const PointsList &v1, const PointsList &v2)
{
    if (v1.size() != v2.size())
        return false;

    for (int idx = 0; idx < v1.size(); ++idx) {
        if (v1.at(idx).toPoint() != v2.at(idx).toPoint())
            return false;
    }
    return true;
}

/*!
 * \brief Adjusts appropriate coordinate of \a pos to the \a boundingArea side specified by \a side.
 * Returns the adjusted \a pos.
 */
QPointF getSidePosition(const QRectF &boundingArea, const QPointF &pos, Qt::Alignment side)
{
    switch (side) {
    case Qt::AlignLeft:
        return QPointF(boundingArea.left(), qBound(boundingArea.top(), pos.y(), boundingArea.bottom()));
    case Qt::AlignRight:
        return QPointF(boundingArea.right(), qBound(boundingArea.top(), pos.y(), boundingArea.bottom()));
    case Qt::AlignTop:
        return QPointF(qBound(boundingArea.left(), pos.x(), boundingArea.right()), boundingArea.top());
    case Qt::AlignBottom:
        return QPointF(qBound(boundingArea.left(), pos.x(), boundingArea.right()), boundingArea.bottom());
    }

    return boundingArea.center();
}

/*!
 * \fn ive::bool alignedLine(QLineF &line, int angleTolerance)
 * \brief  Adjusts the angle of the \a line to the nearest x90 degrees ccw with \a angleTolerance.
 * Returns true if the \a line's angle adjusted.
 */
bool alignedLine(QLineF &line, int angleTolerance)
{
    if (line.isNull())
        return false;

    static const int kStep = 90;

    auto isBounded = [](int straightAngle, int tolerance, int angle) {
        return straightAngle - tolerance < angle && angle < straightAngle + tolerance;
    };

    for (int angle = 360; angle >= 0; angle -= kStep) {
        if (!qFuzzyCompare(1. + line.angle(), 1. + qreal(angle))
                && isBounded(angle, angleTolerance, std::ceil(line.angle()))) {
            line.setAngle(angle);
            return true;
        }
    }
    return false;
}

/*!
 * Converts the IV[AADL/XML] coordinates pair to the QPointF.
 * If \a coordinates is empty or its size != 2 an empty point returned.
 */
QPointF pos(const QVector<qint32> &coordinates)
{
    if (coordinates.isEmpty())
        return {};

    Q_ASSERT(coordinates.size() == 2);
    if (coordinates.size() != 2)
        return {};

    return QPointF(coordinates.first(), coordinates.last());
}

/*!
 * Converts four coordinate numbers from IV[AADL/XML] to the QRectF.
 * If \a coordinates is empty or its size != 4 returns an empty rect.
 */
QRectF rect(const QVector<qint32> &coordinates)
{
    if (coordinates.isEmpty())
        return {};

    Q_ASSERT(coordinates.size() == 4);
    if (coordinates.size() != 4)
        return {};

    QList<QPointF> points;
    for (int idx = 0; idx + 1 < coordinates.size(); idx += 2)
        points.append(QPointF(coordinates.value(idx), coordinates.value(idx + 1)));

    return { points.first(), points.last() };
}

/*!
 * Converts coordinate numbers from IV[AADL/XML] to vector of QPointF.
 * If \a coordinates is empty or its size is odd returns an empty vector.
 */
PointsList polygon(const QVector<qint32> &coordinates)
{
    if (coordinates.isEmpty())
        return {};

    Q_ASSERT(coordinates.size() % 2 == 0);
    if (coordinates.size() % 2 != 0)
        return {};

    PointsList points;
    for (int idx = 0; idx + 1 < coordinates.size(); idx += 2)
        points.append(QPointF(coordinates.value(idx), coordinates.value(idx + 1)));

    return points;
}

} // namespace utils
} // namespace topohelp
