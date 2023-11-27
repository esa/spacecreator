#include "utils.h"

#include <QPolygonF>
#include <QtMath>

namespace topohelp {

namespace utils {

static const qreal kConnectionMargin = 16.0;

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

/*!
 * \brief Helper to detect if the \a line intersects the \a rect.
 * Coordinates of the intersection point stored in \a intersectPos.
 * Returns \c true if \a rect and \a line are not null and intersected.
 */
bool intersects(const QRectF &rect, const QLineF &line, QPointF *intersectPos = nullptr)
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
/*!
 * Returns coordinates of intersection points.
 */
bool intersects(const QRectF &rect, const QPolygonF &polygon, QPointF *intersectPos = nullptr)
{
    for (int idx = 1; idx < polygon.size(); ++idx) {
        if (intersects(rect, QLineF(polygon.value(idx - 1), polygon.value(idx)), intersectPos)) {
            return true;
        }
    }
    return false;
}

/*!
 * Returns the side type of \a boundingArea's which is most close to the \a pos.
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
 * Generates the segments for \a IVConnectionGraphicsItem from \a firstEndPoint with direction to \a lastEndPoint
 * \param sceneRect as a geometry of an item where connected \a IVInterfaceGraphicsItem is placed
 * \param firstEndPoint as starting point
 * \param lastEndPoint as point used to find direction of segment (into/out the \a sceneRect)
 * \return last segment for \a IVConnectionGraphicsItem linked to \a IVInterfaceGraphicsItem
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
 * Generates angled segments for \a IVConnectionGraphicsItem from \a startDirection to \a endDirection
 * \param startDirection as previous segment for generated one
 * \param endDirection as next segment for generated one
 * \return set of points as segment for \a IVConnectionGraphicsItem connects \a startDirection and \a endDirection
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
 */
qreal distanceLine(const QPointF &pnt1, const QPointF &pnt2)
{
    return std::sqrt(std::pow((pnt2.x() - pnt1.x()), 2) + std::pow((pnt2.y() - pnt1.y()), 2));
}

/*!
 * Returns the nearest rect from \a existingRects which intersects with \a points according to \a intersectionType:
 *  \p Edge - any intersection point, even located on the edge of rect
    \p Single - intersection point should be located inside rect, and only one
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
                    && !topohelp::utils::intersects(*it, line, &intersectionPoint)) {
                continue;
            } else {
                const auto intersectPoints =
                        topohelp::utils::intersectionPoints(*it, QPolygonF(PointsList { line.p1(), line.p2() }));
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
                            if (distanceLine(sectionStartPoint, intersectionPoint)
                                    > distanceLine(pnt, sectionStartPoint)) {
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
 * Generates the path for \a IVConnectionGraphicsItem from \a startPoint to \a endPoint
 * \param existingRects geometries of existing items in scene coordinates that should be checked on collisions
 * \param startDirection as first polyline segment
 * \param endDirection as last polyline segment
 * \return set of polyline nodes coordinates
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
qreal distancePolygon(const PointsList &polygon)
{
    qreal distance = 0;
    for (auto it = std::next(polygon.constBegin()); it != polygon.constEnd(); ++it) {
        distance += distanceLine(*std::prev(it), *it);
    }
    return distance;
}

/*!
 * Returns the list of \a area corner points, sorted by minimal distance between the corner and \a point1 or \a point2.
 */
PointsList sortedCorners(const QRectF &area, const QPointF &point1, const QPointF &point2)
{
    PointsList rectPoints { area.topLeft(), area.topRight(), area.bottomLeft(), area.bottomRight() };
    std::sort(rectPoints.begin(), rectPoints.end(), [=](const QPointF &pnt1, const QPointF &pnt2) {
        return distancePolygon({ point1, pnt1, point2 }) < distancePolygon({ point1, pnt2, point2 });
    });
    return rectPoints;
}

/*!
 * Generates angled segments for \a IVConnectionGraphicsItem from \a startPoint to \a endPoint
 * \param startDirection as previous point for generated one
 * \param endDirection as next point for generated one
 * \return set of points as segment for \a IVConnectionGraphicsItem connects \a startPoint and \a endPoint
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
 * Generates the segments for \a IVConnectionGraphicsItem from \a prevPoints to \a nextPoints
 * \param itemRect geometry of intersected item bounding box
 * \param prevPoints as starting points
 * \param nextPoints as points generated segments should be connected to
 * \param strict as a flag showing if it should check full intersection with \a itemRect
 * or some node could be placed on the edge of noticed rect
 * \return set of possible paths
 */
QList<PointsList> findSubPath(
        const QRectF &itemRect, const PointsList &prevPoints, const PointsList &nextPoints, bool strict = true)
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
            const bool withoutIntersections = strict ? !topohelp::utils::intersects(itemRect, polygon)
                            && !topohelp::utils::intersects(itemRect, QLineF(pnt, endPoint))
                                                     : topohelp::utils::intersectionPoints(itemRect, polygon).size() < 2
                            && topohelp::utils::intersectionPoints(itemRect, lastSegment).size() < 2;
            if (withoutIntersections) {
                PointsList previousPoints(prevPoints);
                previousPoints.removeLast();
                previousPoints << polygon;
                allPaths << previousPoints;
            }
        }
    }
    return allPaths;
}

/*!
 * Generates the whole path for \a IVConnectionGraphicsItem
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
                    return distancePolygon(pnt1) < distancePolygon(pont2);
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

} // namespace utils
} // namespace topohelp
