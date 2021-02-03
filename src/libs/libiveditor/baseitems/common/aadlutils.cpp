/*
  Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlutils.h"

#include "aadlobject.h"
#include "baseitems/interactiveobject.h"
#include "connectioncreationvalidator.h"
#include "graphicsviewutils.h"
#include "interface/aadlcommentgraphicsitem.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlconnectiongroupgraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlfunctiontypegraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"

#include <QDebug>
#include <QGraphicsView>
#include <QMetaEnum>
#include <QtGlobal>
#include <QtMath>

namespace ive {

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
                            if (ive::distanceLine(sectionStartPoint, intersectionPoint)
                                    > ive::distanceLine(p, sectionStartPoint)) {
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
        if (isBounded(angle, angleTolerance, qCeil(line.angle()))) {
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
    return rectPoints;
}

/*!
 * Converts the AADL/XMl coordinates pair to the QPointF.
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
 * Converts four coordinate numbers from AADL/XMl to the QRectF.
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
 * Converts coordinate numbers from AADL/XMl to vector of QPointF.
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
    return QRectF { pos - adjustmentPoint, pos + adjustmentPoint };
}

QList<int> knownGraphicsItemTypes()
{
    QList<int> result;

    const QMetaEnum &me = QMetaEnum::fromType<ivm::AADLObject::Type>();
    for (int i = 0; i < me.keyCount(); ++i) {
        int itemType = 0;
        const ivm::AADLObject::Type objectType = static_cast<ivm::AADLObject::Type>(me.value(i));
        switch (objectType) {
        case ivm::AADLObject::Type::Function:
            itemType = ive::AADLFunctionGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::FunctionType:
            itemType = ive::AADLFunctionTypeGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::InterfaceGroup:
        case ivm::AADLObject::Type::ProvidedInterface:
        case ivm::AADLObject::Type::RequiredInterface:
            itemType = ive::AADLInterfaceGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::Comment:
            itemType = ive::AADLCommentGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::Connection:
            itemType = ive::AADLConnectionGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::ConnectionGroup:
            itemType = ive::AADLConnectionGroupGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::Unknown:
            continue;
        }
        if (itemType != 0) {
            result.append(itemType);
        }
    }

    return result;
}

qreal itemLevel(const ivm::AADLObject *const object, bool itemSelected)
{
    if (!object || itemSelected) {
        return ZOrder.Selected;
    }

    switch (object->aadlType()) {
    case ivm::AADLObject::Type::Function:
    case ivm::AADLObject::Type::FunctionType:
        return ZOrder.Function;
    case ivm::AADLObject::Type::InterfaceGroup:
    case ivm::AADLObject::Type::RequiredInterface:
    case ivm::AADLObject::Type::ProvidedInterface:
        return ZOrder.Interface;
    case ivm::AADLObject::Type::Comment:
        return ZOrder.Comment;
    case ivm::AADLObject::Type::ConnectionGroup:
    case ivm::AADLObject::Type::Connection:
        return ZOrder.Connection;
    default:
        return ZOrder.Selected;
    }
}

/*!
 * Returns a \a itemRect aligned inside of \a boundingRect but keeping the offset of \a originPointOffset
 */

QRectF alignRectToSide(
        const QRectF &boundingRect, const QRectF &itemRect, Qt::Alignment side, const QPointF &originPointOffset)
{
    if (!boundingRect.isValid() || !itemRect.isValid())
        return {};

    QRectF rect { itemRect };
    auto adjustVertically = [=](QRectF &rect) {
        rect.moveTop(qBound(
                boundingRect.top() + originPointOffset.y(), rect.top(), boundingRect.bottom() + originPointOffset.y()));
    };
    auto adjustHorizontally = [=](QRectF &rect) {
        rect.moveLeft(qBound(boundingRect.left() + originPointOffset.x(), rect.left(),
                boundingRect.right() + originPointOffset.x()));
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
    return rect;
}

/*!
 * Moves \a itemRect on the \a side from \a intersectedItemRect
 */

QRectF adjustedRect(
        const QRectF &itemRect, const QRectF &intersectedItemRect, const Qt::Alignment side, const bool clockwise)
{
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
        qWarning() << "Unhandled side:" << side;
        return {};
    }
    return resultRect;
}

Qt::Alignment sideFromIndex(const int idx)
{
    if (idx >= 0)
        return kRectSides.value(idx % kRectSides.size());

    return kRectSides.value((kRectSides.size() - qAbs(idx)) % kRectSides.size());
}

int indexFromSide(Qt::Alignment side)
{
    return kRectSides.indexOf(side);
}

/*!
 * Checks intersection \a itemRect with \a itemRects
 * and assign first intersection to \a collidingRect
 */

bool checkCollision(const QList<QRectF> &itemRects, const QRectF &itemRect, QRectF *collidingRect)
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
 * Generates angled segments for \a AADLConnectionGraphicsItem from \a startPoint to \a endPoint
 * \param startDirection as previous point for generated one
 * \param endDirection as next point for generated one
 * \return set of points as segment for \a AADLConnectionGraphicsItem connects \a startPoint and \a endPoint
 */
QList<QVector<QPointF>> generateSegments(const QPointF &startPoint, const QPointF &endPoint)
{
    if (startPoint == endPoint)
        return {};

    if (qFuzzyCompare(startPoint.x(), endPoint.x()) || qFuzzyCompare(startPoint.y(), endPoint.y()))
        return { { startPoint, endPoint } };

    return { { startPoint, { startPoint.x(), endPoint.y() }, endPoint },
        { startPoint, { endPoint.x(), startPoint.y() }, endPoint } };
}

/*!
 * Generates angled segments for \a AADLConnectionGraphicsItem from \a startDirection to \a endDirection
 * \param startDirection as previous segment for generated one
 * \param endDirection as next segment for generated one
 * \return set of points as segment for \a AADLConnectionGraphicsItem connects \a startDirection and \a endDirection
 */
QVector<QPointF> generateSegments(const QLineF &startDirection, const QLineF &endDirection)
{
    if (startDirection.isNull() || endDirection.isNull())
        return {};

    QVector<QPointF> connectionPoints { startDirection.p2(), endDirection.p2() };
    const qreal angle = startDirection.angleTo(endDirection);
    static const qreal tolerance = 0.1;
    if (qAbs(qSin(qDegreesToRadians(angle))) <= tolerance) { // ||
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
    return connectionPoints;
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
 * Generates the segments for \a AADLConnectionGraphicsItem from \a firstEndPoint with direction to \a lastEndPoint
 * \param sceneRect as a geometry of an item where connected \a AADLInterfaceGraphicsItem is placed
 * \param firstEndPoint as starting point
 * \param lastEndPoint as point used to find direction of segment (into/out the \a sceneRect)
 * \return last segment for \a AADLConnectionGraphicsItem linked to \a AADLInterfaceGraphicsItem
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

    return vector;
}

/*!
 * Generates the path for \a AADLConnectionGraphicsItem from \a startPoint to \a endPoint
 * \param existingRects existing items geometries in scene coordinates shouldn't been overlapped
 * \param startPoint as first polyline node
 * \param endDirection as last polyline node
 * \return set of polyline nodes coordinates
 */
QVector<QPointF> path(const QList<QRectF> &existingRects, const QPointF &startPoint, const QPointF &endPoint)
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

/*!
 * Generates the segments for \a AADLConnectionGraphicsItem from \a prevPoints to \a nextPoints
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
                allPaths << previousPoints;
            }
        }
    }
    return allPaths;
}

/*!
 * Generates the path for \a AADLConnectionGraphicsItem from \a startPoint to \a endPoint
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
 * Generates the whole path for \a AADLConnectionGraphicsItem
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

            return results.first();
        }

        if (paths.size() != deeper.size() || !std::equal(paths.constBegin(), paths.constEnd(), deeper.constBegin()))
            paths = deeper;
        else if (!deeper.isEmpty())
            return deeper.front();
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
    if (points.size() <= 3)
        return points;

    /// TODO: optimize flow
    QVector<QPointF> simplifiedPoints(points);

    for (int idx = 0; idx < simplifiedPoints.size() - 1;) {
        const QLineF currentLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx + 1) };
        if (qFuzzyIsNull(currentLine.length())) {
            simplifiedPoints.removeAt(idx + 1);
            continue;
        }
        if (idx + 2 < simplifiedPoints.size()) {
            const QLineF nextLine { simplifiedPoints.value(idx + 1), simplifiedPoints.value(idx + 2) };
            if (qFuzzyIsNull(nextLine.length()) || qFuzzyCompare(currentLine.angle(), nextLine.angle())
                    || int(currentLine.angleTo(nextLine)) % 180 == 0) {
                simplifiedPoints.removeAt(idx + 1);
                continue;
            }
        }

        ++idx;
    }
    for (int idx = 1; idx < simplifiedPoints.size() - 2; ++idx) {
        const QLineF currentLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx + 1) };
        const QLineF prevLine { simplifiedPoints.value(idx - 1), simplifiedPoints.value(idx) };
        const QLineF nextLine { simplifiedPoints.value(idx + 1), simplifiedPoints.value(idx + 2) };

        if (qFuzzyCompare(prevLine.angle(), nextLine.angle()) && currentLine.length() < kMinLineLength) {
            const QPointF midPoint = currentLine.center();
            const QPointF prevOffset = midPoint - currentLine.p1();
            simplifiedPoints[idx - 1] = prevLine.p1() + prevOffset;
            const QPointF nextOffset = midPoint - currentLine.p2();
            simplifiedPoints[idx + 2] = nextLine.p2() + nextOffset;
            simplifiedPoints.removeAt(idx + 1);
            simplifiedPoints.removeAt(idx);
        }
    }
    if (simplifiedPoints.size() == 2)
        return { points.first(), points.last() };
    return simplifiedPoints;
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

int nestingLevel(ivm::AADLObject *object)
{
    if (!object || object->isRootObject())
        return -1;

    if (object->aadlType() == ivm::AADLObject::Type::InterfaceGroup
            || object->aadlType() == ivm::AADLObject::Type::ProvidedInterface
            || object->aadlType() == ivm::AADLObject::Type::RequiredInterface) {
        object = object->parentObject();
    }

    int level = 0;
    while (auto parentObject = object->parentObject()) {
        if (!parentObject->isRootObject()
                && (parentObject->aadlType() == ivm::AADLObject::Type::Function
                        || parentObject->aadlType() == ivm::AADLObject::Type::FunctionType)) {
            ++level;
        }
        object = parentObject;
    }
    return level;
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
        itemRect = QRectF(QPointF(0, 0), DefaultGraphicsItemSize);
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
 * Gets geometries of all AADLFunctionGraphicsItems and AADLFunctionTypeGraphicsItem
 * with the same level as \a item in scene coordinates
 */
QList<QRectF> siblingSceneRects(QGraphicsItem *item)
{
    Q_ASSERT(item && item->scene());

    QList<QRectF> existingRects;
    static const QList<int> kNonIntersectableTypes = { AADLFunctionGraphicsItem::Type,
        AADLFunctionTypeGraphicsItem::Type };
    for (auto graphicsItem : item->scene()->items()) {
        if (item != graphicsItem && graphicsItem->parentItem() == item->parentItem()
                && kNonIntersectableTypes.contains(graphicsItem->type()))
            existingRects.append(graphicsItem->sceneBoundingRect());
    }
    return existingRects;
}

} // namespace ive
