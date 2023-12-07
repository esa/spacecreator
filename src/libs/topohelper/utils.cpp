#include "utils.h"

#include <QDebug>
#include <QPolygonF>
#include <QtMath>

#define WRN qWarning() << Q_FUNC_INFO

namespace topohelp {

namespace utils {

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
/*!
 * Returns coordinates of intersection points.
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
 * Returns the list of \a area corner points, sorted by minimal distance between the corner and \a point1 or \a
 * point2.
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

/*!
 * Generates the path for \a IVConnectionGraphicsItem from \a startPoint to \a endPoint
 * \param existingRects existing items geometries in scene coordinates shouldn't been overlapped
 * \param startPoint as first polyline node
 * \param endDirection as last polyline node
 * \return set of polyline nodes coordinates
 */
PointsList path(const RectsList &existingRects, const QPointF &startPoint, const QPointF &endPoint)
{
    const QVector<QPointF> points { startPoint, endPoint };
    auto item = getNearestIntersectedRect(existingRects, points, IntersectionType::Multiple);
    if (!item.isValid())
        return points;

    const QList<QVector<QPointF>> possiblePaths = findSubPath(item, { startPoint }, { endPoint }, false);
    if (possiblePaths.isEmpty()) {
        return {};
    }

    QList<QVector<QPointF>> paths;
    for (const QVector<QPointF> &possiblePath : possiblePaths) {
        if (possiblePath.isEmpty()) {
            continue;
        }

        if (possiblePath.last() == endPoint) {
            paths.append(possiblePath);
        } else {
            const auto p = path(existingRects, possiblePath.last(), endPoint);
            if (!p.isEmpty()) {
                QVector<QPointF> prevPath(possiblePath);
                prevPath.removeLast();
                paths.append(prevPath << p);
            }
        }
    }
    if (paths.isEmpty()) {
        return {};
    }

    QVector<QPointF> shortestPath;
    qreal minLength = std::numeric_limits<qreal>::max();
    for (const QVector<QPointF> &path : paths) {
        if (path.size() < 2) {
            continue;
        }
        const qreal length = distancePolygon(path);
        if (minLength > length && !qFuzzyIsNull(length)) {
            minLength = length;
            shortestPath = path;
        }
    }
    if (shortestPath.isEmpty())
        return {};

    return shortestPath;
}

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
 * Moves \a itemRect on the \a side from \a intersectedItemRect
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

bool isRectBounded(const QRectF &outerRect, const QRectF &innerRect)
{
    if (!innerRect.isValid()) {
        return true;
    } else if (!outerRect.isValid()) {
        return false;
    }
    return outerRect.contains(innerRect);
}

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
};

QVector<qint32> coordinates(const QPointF &point)
{
    if (point.isNull())
        return {};

    return coordinates(QVector<QPointF> { point });
}

QVector<qint32> coordinates(const QRectF &rect)
{
    if (rect.isValid())
        return coordinates(QVector<QPointF> { rect.topLeft(), rect.bottomRight() });

    return {};
}

QVector<qint32> coordinates(const PointsList &points)
{
    QVector<qint32> coordinates;
    for (const QPointF &point : points)
        coordinates << qRound(point.x()) << qRound(point.y());
    return coordinates;
}

} // namespace utils
} // namespace topohelp
