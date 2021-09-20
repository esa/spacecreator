#include "graphicsviewutils.h"

#include "ui/veinteractiveobject.h"
#include "ui/verectgraphicsitem.h"
#include "veobject.h"

#include <QFontMetricsF>
#include <QPainter>
#include <QPainterPath>
#include <QTextLayout>
#include <QtDebug>
#include <QtMath>

namespace shared {
namespace graphicsviewutils {

static const qreal kMinSegmentLength = 10.0;
static const qreal kMinSegmentAngle = 5.0;
static const qreal kConnectionMargin = 16.0;

QPainterPath lineShape(const QLineF &line, qreal span)
{
    QPainterPath result;

    QLineF normalSpan(line.normalVector());
    normalSpan.setLength(span);

    const QPointF delta = line.p1() - normalSpan.p2();
    const QLineF dec(line.translated(delta));
    const QLineF inc(line.translated(-delta));

    result.moveTo(dec.p1());
    result.lineTo(dec.p2());
    result.lineTo(inc.p2());
    result.lineTo(inc.p1());

    return result;
}

/*!
   Constructs a rectangular octagon. \p rect gives the size and \p cut is the length (in x and y) of the cut.
 */
QPainterPath edgeCuttedRectShape(const QRectF &rect, qreal cut)
{
    QPainterPath result;

    const QPointF hcut(cut, 0.);
    const QPointF vcut(0., cut);

    result.moveTo(rect.topLeft() + hcut);
    result.lineTo(rect.topRight() - hcut);
    result.lineTo(rect.topRight() + vcut);
    result.lineTo(rect.bottomRight() - vcut);
    result.lineTo(rect.bottomRight() - hcut);
    result.lineTo(rect.bottomLeft() + hcut);
    result.lineTo(rect.bottomLeft() - vcut);
    result.lineTo(rect.topLeft() + vcut);
    result.lineTo(rect.topLeft() + hcut);

    return result;
}

/*!
 * \brief Helper to detect if the \a line intersects the \a rect.
 * Coordinates of the intersection point stored in \a intersectPos.
 * Returns \c true if \a rect and \a line are not null and intersected.
 */
bool intersects(const QRectF &rect, const QLineF &line, QPointF *intersectPos)
{
    if (rect.isNull() || line.isNull())
        return false;

    const QVector<QLineF> rectLines = {
        { rect.topLeft(), rect.topRight() },
        { rect.topRight(), rect.bottomRight() },
        { rect.bottomRight(), rect.bottomLeft() },
        { rect.bottomLeft(), rect.topLeft() },
    };

    for (const QLineF &rectLine : rectLines) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        if (rectLine.intersects(line, intersectPos) == QLineF::BoundedIntersection)
#else
        if (rectLine.intersect(line, intersectPos) == QLineF::BoundedIntersection)
#endif
            return true;
    }

    return false;
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
        if (intersects(rect, QLineF(polygon.value(idx - 1), polygon.value(idx)), intersectPos))
            return true;
    }
    return false;
}

/*!
 * \brief Helper to detect if each sub segment of the \a polygon intersects the \a rect.
 * Returns coordinates of intersection points.
 */
QVector<QPointF> intersectionPoints(const QRectF &rect, const QPolygonF &polygon)
{
    if (rect.isNull() || polygon.isEmpty())
        return {};

    const QVector<QLineF> rectLines = {
        { rect.topLeft(), rect.topRight() },
        { rect.topRight(), rect.bottomRight() },
        { rect.bottomRight(), rect.bottomLeft() },
        { rect.bottomLeft(), rect.topLeft() },
    };

    QPointF intersectPos;
    QVector<QPointF> points;
    for (int idx = 1; idx < polygon.size(); ++idx) {
        const QLineF line = { polygon.value(idx - 1), polygon.value(idx) };
        for (const QLineF &rectLine : rectLines) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
            if (rectLine.intersects(line, &intersectPos) == QLineF::BoundedIntersection)
#else
            if (rectLine.intersect(line, &intersectPos) == QLineF::BoundedIntersection)
#endif
                points.append(intersectPos);
        }
    }
    return points;
}

/*!
 * Returns the nearest rect from \a existingRects which intersects with \a points according to \a intersectionType:
 *  \p Edge - any intersection point, even located on the edge of rect
    \p Single - intersection point should be located inside rect, and only one
    \p Multiple - 2+ intersection points
 */
QRectF getNearestIntersectedRect(
        const QList<QRectF> &existingRects, const QVector<QPointF> &points, IntersectionType intersectionType)
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
                    && !shared::graphicsviewutils::intersects(*it, line, &intersectionPoint)) {
                continue;
            } else {
                const auto intersectionPoints = shared::graphicsviewutils::intersectionPoints(
                        *it, QPolygonF(QVector<QPointF> { line.p1(), line.p2() }));
                if (intersectionPoints.isEmpty()) {
                    continue;
                }
                if (intersectionType == IntersectionType::Multiple) {
                    if (intersectionPoints.size() <= 1) {
                        continue;
                    }
                } else {
                    const int properPointsCount = std::count_if(
                            intersectionPoints.cbegin(), intersectionPoints.cend(), [points](const QPointF &point) {
                                return points.first() != point && points.last() != point;
                            });
                    if (properPointsCount == 0) {
                        continue;
                    }
                }
                std::for_each(intersectionPoints.cbegin(), intersectionPoints.cend(),
                        [&intersectionPoint, sectionStartPoint = line.p1()](const QPointF &p) {
                            if (distanceLine(sectionStartPoint, intersectionPoint)
                                    > distanceLine(p, sectionStartPoint)) {
                                intersectionPoint = p;
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
 * Returns the side type of \a boundingArea's which is most close to the \a pos.
 */
Qt::Alignment getNearestSide(const QRectF &boundingArea, const QPointF &pos)
{
    if (!boundingArea.isValid())
        return 0;

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
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
                    return line.intersects(rectSide.first, &dummyPoint) == QLineF::BoundedIntersection;
#else
                                   return line.intersect(rectSide.first, &dummyPoint) == QLineF::BoundedIntersection;
#endif
                });
        if (it != rectLines.constEnd())
            side = it->second;
    }
    return side;
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
 * \brief Iterates over all \a scene items in the specified \a pos and returns the first one which QGraphicsItem::type
 * found in \a acceptableTypes list.
 */
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QPointF &pos, const QList<int> &acceptableTypes)
{
    if (scene)
        for (QGraphicsItem *item : scene->items(pos)) {
            if (acceptableTypes.contains(item->type()))
                return item;
        }
    return nullptr;
}

/*!
 * \brief Iterates over all \a scene items in the specified \a area and returns the first one which
 * QGraphicsItem::type found in \a acceptableTypes list.
 */
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QRectF &area, const QList<int> &acceptableTypes)
{
    const QList<QGraphicsItem *> areaItems = scene->items(area);
    if (areaItems.isEmpty())
        return nullptr;

    const QPointF point = area.center();
    if (areaItems.size() == 1) {
        auto item = areaItems.value(0);
        if (item && item->contains(point) && acceptableTypes.contains(item->type()))
            return item;
    }

    qreal distance = std::numeric_limits<int>::max();
    QGraphicsItem *nearestToCenter = nullptr;
    for (QGraphicsItem *item : areaItems) {
        if (!acceptableTypes.isEmpty() && !acceptableTypes.contains(item->type()))
            continue;

        const QRectF itemRect = item->sceneBoundingRect();
        qreal itemDistance = qAbs(itemRect.right() - point.x());
        itemDistance = std::min(itemDistance, qAbs(itemRect.left() - point.x()));
        itemDistance = std::min(itemDistance, qAbs(itemRect.top() - point.y()));
        itemDistance = std::min(itemDistance, qAbs(itemRect.bottom() - point.y()));
        if (itemDistance < distance) {
            nearestToCenter = item;
            distance = itemDistance;
        }
    }
    return nearestToCenter;
}

/*!
 * \brief Iterates over all \a scene items in the square with center in \a center and the size of \a offset and returns
 * the first one which QGraphicsItem::type found in \a acceptableTypes list.
 */
QGraphicsItem *nearestItem(
        const QGraphicsScene *scene, const QPointF &center, qreal offset, const QList<int> &acceptableTypes)
{
    const QRectF area { center - QPointF(offset / 2, offset / 2), center + QPointF(offset / 2, offset / 2) };
    return nearestItem(scene, area, acceptableTypes);
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
        if (isBounded(angle, angleTolerance, std::ceil(line.angle()))) {
            line.setAngle(angle);
            return true;
        }
    }
    return false;
}

/*!
 * \brief Returns the distance between \a p1 and \a p2.
 */
qreal distanceLine(const QPointF &p1, const QPointF &p2)
{
    return std::sqrt(std::pow((p2.x() - p1.x()), 2) + std::pow((p2.y() - p1.y()), 2));
}

/*!
 * Returns the total length of all subsegments of \a polygon.
 */
qreal distancePolygon(const QVector<QPointF> &polygon)
{
    qreal distance = 0;
    for (auto it = std::next(polygon.constBegin()); it != polygon.constEnd(); ++it)
        distance += distanceLine(*std::prev(it), *it);
    return distance;
}

/*!
 * Returns the list of \a area corner points, sorted by minimal distance between the corner and \a point1 or \a point2.
 */
QList<QPointF> sortedCorners(const QRectF &area, const QPointF &point1, const QPointF &point2)
{
    QList<QPointF> rectPoints { area.topLeft(), area.topRight(), area.bottomLeft(), area.bottomRight() };
    std::sort(rectPoints.begin(), rectPoints.end(), [=](const QPointF &p1, const QPointF &p2) {
        return distancePolygon({ point1, p1, point2 }) < distancePolygon({ point1, p2, point2 });
    });
    return round(rectPoints);
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

QVector<QPointF> polygon(const QVector<qint32> &coordinates)
{
    if (coordinates.isEmpty())
        return {};

    Q_ASSERT(coordinates.size() % 2 == 0);
    if (coordinates.size() % 2 != 0)
        return {};

    QVector<QPointF> points;
    for (int idx = 0; idx + 1 < coordinates.size(); idx += 2)
        points.append(QPointF(coordinates.value(idx), coordinates.value(idx + 1)));

    return points;
}

QVector<qint32> coordinates(const QPointF &point)
{
    return coordinates(QVector<QPointF> { point });
}

QVector<qint32> coordinates(const QRectF &rect)
{
    return coordinates(QVector<QPointF> { rect.topLeft(), rect.bottomRight() });
}

QVector<qint32> coordinates(const QVector<QPointF> &points)
{
    QVector<qint32> coordinates;
    for (const QPointF &point : points)
        coordinates << qRound(point.x()) << qRound(point.y());
    return coordinates;
}

QRectF adjustFromPoint(const QPointF &pos, const qreal &adjustment)
{
    const QPointF adjustmentPoint { adjustment / 2, adjustment / 2 };
    return QRectF { (pos - adjustmentPoint).toPoint(), (pos + adjustmentPoint).toPoint() };
}

/*!
 * Returns a \a itemRect aligned inside of \a boundingRect but keeping the offset of \a originPointOffset
 */

QRectF alignRectToSide(const QRectF &boundingRect, const QRectF &itemRect, Qt::Alignment side,
        const QPointF &originPointOffset, const QMarginsF &margins)
{
    if (!boundingRect.isValid() || !itemRect.isValid())
        return {};

    QRectF rect { itemRect.toRect() };
    auto adjustVertically = [=](QRectF &rect) {
        rect.moveTop(qBound(boundingRect.top() + originPointOffset.y() + margins.top(), rect.top(),
                boundingRect.bottom() + originPointOffset.y() - margins.bottom()));
    };
    auto adjustHorizontally = [=](QRectF &rect) {
        rect.moveLeft(qBound(boundingRect.left() + originPointOffset.x() + margins.left(), rect.left(),
                boundingRect.right() + originPointOffset.x() - margins.right()));
    };

    switch (side) {
    case Qt::AlignLeft:
        rect.moveLeft(boundingRect.left() + originPointOffset.x());
        adjustVertically(rect);
        break;
    case Qt::AlignTop:
        rect.moveTop(boundingRect.top() + originPointOffset.y());
        adjustHorizontally(rect);
        break;
    case Qt::AlignRight:
        rect.moveLeft(boundingRect.right() + originPointOffset.x());
        adjustVertically(rect);
        break;
    case Qt::AlignBottom:
        rect.moveTop(boundingRect.bottom() + originPointOffset.y());
        adjustHorizontally(rect);
        break;
    default:
        return {};
    }
    return rect.toRect();
}

/*!
 * Moves \a itemRect on the \a side from \a intersectedItemRect
 */

QRectF adjustedRect(const QRectF &itemRect, const QRectF &intersectedItemRect, const Qt::Alignment side,
        const LookupDirection direction)
{
    const bool clockwise = direction == LookupDirection::Clockwise;
    QRectF resultRect { itemRect.toRect() };
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
        qWarning() << "Unhandled side:" << side;
        return {};
    }
    return resultRect.toRect();
}

/*!
 * Checks intersection \a itemRect with \a itemRects
 * and assign first intersection to \a collidingRect
 */

bool isCollided(const QList<QRectF> &itemRects, const QRectF &itemRect, QRectF *collidingRect)
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
 * Generates angled segments for \a IVConnectionGraphicsItem from \a startPoint to \a endPoint
 * \param startDirection as previous point for generated one
 * \param endDirection as next point for generated one
 * \return set of points as segment for \a IVConnectionGraphicsItem connects \a startPoint and \a endPoint
 */
QList<QVector<QPointF>> generateSegments(const QPointF &startPoint, const QPointF &endPoint)
{
    QPointF startPt = startPoint.toPoint();
    QPointF endPt = endPoint.toPoint();
    if (startPt == endPt) {
        return {};
    }

    if (qFuzzyCompare(startPt.x(), endPt.x()) || qFuzzyCompare(startPt.y(), endPt.y()))
        return { { startPt, endPt } };

    return { { startPt, { startPt.x(), endPt.y() }, endPt }, { startPt, { endPt.x(), startPt.y() }, endPt } };
}

/*!
 * Generates angled segments for \a IVConnectionGraphicsItem from \a startDirection to \a endDirection
 * \param startDirection as previous segment for generated one
 * \param endDirection as next segment for generated one
 * \return set of points as segment for \a IVConnectionGraphicsItem connects \a startDirection and \a endDirection
 */
QVector<QPointF> generateSegments(const QLineF &startDirection, const QLineF &endDirection)
{
    if (startDirection.isNull() || endDirection.isNull())
        return {};

    QVector<QPointF> connectionPoints { startDirection.p2(), endDirection.p2() };
    const qreal angle = startDirection.angleTo(endDirection);
    static const qreal tolerance = 0.1;
    if (std::abs(std::sin(qDegreesToRadians(angle))) <= tolerance) { // ||
        const QPointF mid = QLineF(connectionPoints.first(), connectionPoints.last()).center();
        QLineF midLine { mid, QPointF(0, 0) };
        midLine.setAngle(startDirection.angle() - 90);

        QPointF startLastPoint;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        midLine.intersects(startDirection, &startLastPoint);
#else
        midLine.intersect(startDirection, &startLastPoint);
#endif
        connectionPoints.insert(connectionPoints.size() - 1, startLastPoint);

        QPointF endLastPoint;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        midLine.intersects(endDirection, &endLastPoint);
#else
        midLine.intersect(endDirection, &endLastPoint);
#endif
        connectionPoints.insert(connectionPoints.size() - 1, endLastPoint);
    } else { // |_, \_, /_, etc
        QPointF mid;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        startDirection.intersects(endDirection, &mid);
#else
        startDirection.intersect(endDirection, &mid);
#endif
        connectionPoints.insert(connectionPoints.size() - 1, mid);
    }
    return round(connectionPoints);
}

static inline QLineF getDirection(const QRectF &sceneRect, const QPointF &point)
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
    if (vector.isNull())
        return {};

    vector.translate(firstEndPoint - vector.p1());
    vector.setLength(kConnectionMargin);

    if (sceneRect.contains(lastEndPoint))
        vector.setAngle(180 + vector.angle());

    return vector.toLine();
}

/*!
 * Generates the path for \a IVConnectionGraphicsItem from \a startPoint to \a endPoint
 * \param existingRects existing items geometries in scene coordinates shouldn't been overlapped
 * \param startPoint as first polyline node
 * \param endDirection as last polyline node
 * \return set of polyline nodes coordinates
 */
QVector<QPointF> path(const QList<QRectF> &existingRects, const QPointF &startPoint, const QPointF &endPoint)
{
    const QVector<QPointF> points { startPoint, endPoint };
    auto item = getNearestIntersectedRect(existingRects, points, IntersectionType::Multiple);
    if (!item.isValid()) {
        return round(points);
    }

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

    return round(shortestPath);
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
QList<QVector<QPointF>> findSubPath(
        const QRectF &itemRect, const QVector<QPointF> &prevPoints, const QVector<QPointF> &nextPoints, bool strict)
{
    Q_ASSERT(itemRect.isValid());
    Q_ASSERT(!prevPoints.isEmpty());
    Q_ASSERT(!nextPoints.isEmpty());

    if (!itemRect.isValid() || prevPoints.isEmpty() || nextPoints.isEmpty())
        return {};

    const QPointF startPoint = prevPoints.last();
    const QPointF endPoint = nextPoints.last();
    Q_ASSERT(startPoint != endPoint);
    if (startPoint == endPoint)
        return {};

    const QRectF itemRectMargins =
            itemRect.adjusted(-kConnectionMargin, -kConnectionMargin, kConnectionMargin, kConnectionMargin);
    const QList<QPointF> rectCorners = sortedCorners(itemRectMargins, startPoint, endPoint);
    QList<QVector<QPointF>> allPaths;
    for (const QPointF &p : rectCorners) {
        const QVector<QPointF> lastSegment = { p, endPoint };
        for (const auto &polygon : generateSegments(startPoint, p)) {
            const bool withoutIntersections = strict
                    ? !shared::graphicsviewutils::intersects(itemRect, polygon)
                            && !shared::graphicsviewutils::intersects(itemRect, QLineF(p, endPoint))
                    : shared::graphicsviewutils::intersectionPoints(itemRect, polygon).size() < 2
                            && shared::graphicsviewutils::intersectionPoints(itemRect, lastSegment).size() < 2;
            if (withoutIntersections) {
                QVector<QPointF> previousPoints(prevPoints);
                previousPoints.removeLast();
                previousPoints << polygon;
                allPaths << round(previousPoints);
            }
        }
    }
    return allPaths;
}

/*!
 * Generates the path for \a IVConnectionGraphicsItem from \a startPoint to \a endPoint
 * \param existingRects geometries of existing items in scene coordinates that should be checked on collisions
 * \param startDirection as first polyline segment
 * \param endDirection as last polyline segment
 * \return set of polyline nodes coordinates
 */

QVector<QPointF> findPath(const QList<QRectF> &existingRects, const QLineF &startDirection, const QLineF &endDirection,
        QRectF *intersectedRect)
{
    const QVector<QPointF> points = generateSegments(startDirection, endDirection);
    const QRectF rect = getNearestIntersectedRect(existingRects, points, IntersectionType::Single);
    if (!rect.isValid())
        return points;

    if (intersectedRect)
        *intersectedRect = rect;
    return {};
}

/*!
 * Generates the whole path for \a IVConnectionGraphicsItem
 * \param existingRects geometries of existing items in scene coordinates that should be checked on collisions
 * \param startDirection as first polyline segment
 * \param endDirection as last polyline segment
 * \return set of polyline nodes coordinates
 */
QVector<QPointF> path(const QList<QRectF> &existingRects, const QLineF &startDirection, const QLineF &endDirection)
{
    QRectF intersectedRect;
    QList<QVector<QPointF>> paths { { startDirection.p1(), startDirection.p2() } };
    while (true) {
        QList<QVector<QPointF>> deeper;
        QList<QVector<QPointF>> results;
        for (const auto &path : paths) {
            Q_ASSERT(path.size() >= 2);
            if (path.size() < 2)
                return {};

            const QLineF prevDirection { path.value(path.size() - 2), path.value(path.size() - 1) };
            auto shortPath = findPath(existingRects, prevDirection, endDirection, &intersectedRect);
            if (!shortPath.isEmpty()) {
                QVector<QPointF> result;
                result.append(startDirection.p1());
                result.append(path);
                result.append(shortPath);
                result.append(endDirection.p1());
                results.append(result);
                continue;
            }
            const auto subPaths = findSubPath(intersectedRect, path, { endDirection.p1(), endDirection.p2() });
            for (auto subPath : subPaths) {
                if (subPath.isEmpty()) {
                    continue;
                }

                const QRectF item = getNearestIntersectedRect(existingRects, subPath, IntersectionType::Single);
                if (item.isValid())
                    continue;
                else if (subPath.last() == endDirection.p2())
                    results.append(subPath);
                else
                    deeper.append(subPath);
            }
        }
        if (!results.isEmpty()) {
            std::sort(results.begin(), results.end(), [](const QVector<QPointF> &v1, const QVector<QPointF> &v2) {
                if (v1.size() == v2.size())
                    return distancePolygon(v1) < distancePolygon(v2);
                return v1.size() < v2.size();
            });

            return round(results.first());
        }

        if (paths.size() != deeper.size() || !std::equal(paths.constBegin(), paths.constEnd(), deeper.constBegin()))
            paths = deeper;
        else if (!deeper.isEmpty())
            return round(deeper.front());
        else
            break;
    }
    return {};
}

QVector<QPointF> createConnectionPath(const QList<QRectF> &existingRects, const QPointF &startIfacePos,
        const QRectF &sourceRect, const QPointF &endIfacePos, const QRectF &targetRect)
{
    const QLineF startDirection = ifaceSegment(sourceRect, startIfacePos, endIfacePos);
    if (startDirection.isNull())
        return {};

    const QLineF endDirection = ifaceSegment(targetRect, endIfacePos, startIfacePos);
    if (endDirection.isNull())
        return {};

    const auto points = path(existingRects, startDirection, endDirection);
    return simplifyPoints(points);
}

QVector<QPointF> simplifyPoints(const QVector<QPointF> &points)
{
    if (points.size() <= 2)
        return round(points);

    auto checkLines = [](const QLineF &line1, const QLineF &line2) -> bool {
        if (line1.length() < kMinSegmentLength) {
            return true;
        }
        const int simplifiedAngle = qAbs(qRound(line1.angleTo(line2)) % 180);
        if (simplifiedAngle < kMinSegmentAngle || 180 - simplifiedAngle < kMinSegmentAngle) {
            return true;
        }
        return false;
    };

    QVector<QPointF> simplifiedPoints(points);
    for (int idx = 1; idx < simplifiedPoints.size() - 1;) {
        const QLineF currentLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx - 1) };
        const QLineF nextLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx + 1) };
        if (checkLines(currentLine, nextLine)) {
            simplifiedPoints.removeAt(idx);
            continue;
        }
        ++idx;
    }
    return round(simplifiedPoints);
}

bool comparePolygones(const QVector<QPointF> &v1, const QVector<QPointF> &v2)
{
    if (v1.size() != v2.size())
        return false;

    for (int idx = 0; idx < v1.size(); ++idx) {
        if (v1.at(idx).toPoint() != v2.at(idx).toPoint())
            return false;
    }
    return true;
}

bool isOnVerticalSide(const QRectF &rect, const QPointF &point)
{
    return (qFuzzyCompare(rect.left(), point.x()) || qFuzzyCompare(rect.right(), point.x()))
            && ((rect.top() < point.y() && rect.bottom() > point.y()) || qFuzzyCompare(rect.top(), point.y())
                    || qFuzzyCompare(rect.bottom(), point.y()));
}

bool isOnHorizontalSide(const QRectF &rect, const QPointF &point)
{
    return (qFuzzyCompare(rect.top(), point.y()) || qFuzzyCompare(rect.bottom(), point.y()))
            && ((rect.left() < point.x() && rect.right() > point.x()) || qFuzzyCompare(rect.left(), point.x())
                    || qFuzzyCompare(rect.right(), point.x()));
}

bool rectContainsPoint(const QRectF &rect, const QPointF &point, bool proper)
{
    if (!rect.contains(point)) {
        return false;
    }
    return !proper || (!isOnHorizontalSide(rect, point) && !isOnVerticalSide(rect, point));
}

/*!
 * Searches rect from \a existingRects that intersects with \a rect
 */
QRectF collidingRect(const QRectF &rect, const QList<QRectF> &existingRects)
{
    auto it = std::find_if(existingRects.constBegin(), existingRects.constEnd(),
            [rect](const QRectF &r) { return r.intersects(rect); });
    if (it != existingRects.constEnd()) {
        return *it;
    }
    return QRectF();
};

/*!
 * Searches location for \a itemRect within \a boundedRect that could be expanded to include mentioned rect
 * without overlapping with \a existingRects
 */
void findGeometryForRect(
        QRectF &itemRect, QRectF &boundedRect, const QList<QRectF> &existingRects, const QMarginsF &margins)
{
    if (!itemRect.isValid()) {
        itemRect = QRectF(QPointF(0, 0), kDefaultGraphicsItemSize);
    }
    QRectF itemGeometry(itemRect.marginsAdded(margins));
    QRectF contentGeometry;

    if (boundedRect.isValid()) {
        contentGeometry = boundedRect.marginsRemoved(margins);
        itemGeometry.moveTopLeft(contentGeometry.topLeft());
    }
    contentGeometry |= itemGeometry;

    static const qreal kOffset = 2;
    if (!existingRects.isEmpty()) {
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
    }

    itemRect = itemGeometry.marginsRemoved(margins);
    QRectF newBoundingRect;
    for (const QRectF &rect : existingRects) {
        newBoundingRect |= rect;
    }
    newBoundingRect |= itemRect;
    boundedRect = newBoundingRect.marginsAdded(margins);
}

/*!
 * Gets geometries of rectangular items with the same level
 * as \a item in scene coordinates with \a margins added
 */
QList<QRectF> siblingItemsRects(const QGraphicsItem *item)
{
    if (!item) {
        return {};
    }

    const QGraphicsScene *scene = item->scene();
    if (!scene) {
        return {};
    }

    QList<QRectF> rects;
    const QList<QGraphicsItem *> siblingItems = item->parentItem() ? item->parentItem()->childItems() : scene->items();
    for (auto siblingItem : siblingItems) {
        if (item != siblingItem && qobject_cast<ui::VERectGraphicsItem *>(siblingItem->toGraphicsObject()) != nullptr
                && siblingItem->parentItem() == item->parentItem()) {
            rects.append(siblingItem->sceneBoundingRect());
        }
    }
    return rects;
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

bool isCollided(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect)
{
    if (!upcomingItem || !upcomingItemRect.isValid()) {
        return false;
    }

    const QList<QRectF> siblingRects = siblingItemsRects(upcomingItem);
    return isCollided(siblingRects, upcomingItemRect.marginsAdded(kContentMargins));
}

bool isBounded(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect)
{
    if (!upcomingItem || !upcomingItemRect.isValid()) {
        return false;
    }

    if (auto rectItem = qobject_cast<const ui::VERectGraphicsItem *>(upcomingItem->toGraphicsObject())) {
        if (auto parentObj = rectItem->parentItem()) {
            const QRectF outerRect = parentObj->sceneBoundingRect().marginsRemoved(kContentMargins);
            return isRectBounded(outerRect, upcomingItemRect);
        }
        return isRectBounded(
                upcomingItemRect.marginsRemoved(kContentMargins), rectItem->nestedItemsSceneBoundingRect());
    }
    return true;
}

void drawText(QPainter *painter, const QRectF &rect, const QString &text, qreal margin)
{
    qreal y = margin;
    const qreal lineWidth = rect.width() - 2 * margin;
    const qreal maxY = rect.height() - margin;
    const QFontMetricsF fm(painter->font());
    bool complete = false;
    for (auto line : text.split(QLatin1Char('\n'))) {
        QTextLayout textLayout(line);
        textLayout.setFont(painter->font());
        textLayout.beginLayout();
        while (true) {
            QTextLine textLine = textLayout.createLine();
            if (!textLine.isValid()) {
                break;
            }

            textLine.setLineWidth(lineWidth);
            if (maxY < y + textLine.height()) {
                const QString lastLine = line.mid(textLine.textStart());
                const QString elidedLastLine = fm.elidedText(lastLine, Qt::ElideRight, lineWidth);
                painter->drawText(QPointF(margin, y + fm.ascent()) + rect.topLeft(), elidedLastLine);
                complete = true;
                break;
            }

            textLine.draw(painter, QPointF(margin, y) + rect.topLeft());
            y += textLine.height();
        }
        textLayout.endLayout();
        if (complete)
            break;
    }
}

/*!
   Round all coordinates
 */
QVector<QPointF> round(const QVector<QPointF> &points)
{
    QVector<QPointF> result;
    result.reserve(points.size());
    for (const QPointF &pt : points) {
        result.push_back(QPointF(qRound(pt.x()), qRound(pt.y())));
    }
    return result;
}

/*!
   Round all coordinates
 */
QList<QPointF> round(const QList<QPointF> &points)
{
    QList<QPointF> result;
    result.reserve(points.size());
    for (const QPointF &pt : points) {
        result.push_back(QPointF(qRound(pt.x()), qRound(pt.y())));
    }
    return result;
}

}

}
