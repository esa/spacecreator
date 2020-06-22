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
#include "interface/aadlcommentgraphicsitem.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlfunctiontypegraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"

#include <QDebug>
#include <QGraphicsView>
#include <QMetaEnum>
#include <QtGlobal>
#include <QtMath>

namespace aadlinterface {

/*!
 * Returns the side type of \a boundingArea's which is most close to the \a pos.
 */
Qt::Alignment getNearestSide(const QRectF &boundingArea, const QPointF &pos)
{
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
 * \brief Adjusts appropriate coordinate of \a pos to the \a boundingArea side specificed by \a side.
 * Returns the adjusted \a pos.
 */
QPointF getSidePosition(const QRectF &boundingArea, const QPointF &pos, Qt::Alignment side)
{
    switch (side) {
    case Qt::AlignLeft:
        return QPointF(boundingArea.left(), pos.y());
    case Qt::AlignRight:
        return QPointF(boundingArea.right(), pos.y());
    case Qt::AlignTop:
        return QPointF(pos.x(), boundingArea.top());
    case Qt::AlignBottom:
        return QPointF(pos.x(), boundingArea.bottom());
    }

    return boundingArea.center();
}

/*!
 * \brief Iterates over all \a scene items in the specificied \a pos and returns the first one which QGraphicsItem::type
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
 * \brief Iterates over all \a scene items in the specificied \a area and returns the first one which
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
 * \fn aadlinterface::bool alignedLine(QLineF &line, int angleTolerance)
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

    const QMetaEnum &me = QMetaEnum::fromType<aadl::AADLObject::Type>();
    for (int i = 0; i < me.keyCount(); ++i) {
        int itemType = 0;
        const aadl::AADLObject::Type objectType = static_cast<aadl::AADLObject::Type>(me.value(i));
        switch (objectType) {
        case aadl::AADLObject::Type::Function:
            itemType = aadlinterface::AADLFunctionGraphicsItem::Type;
            break;
        case aadl::AADLObject::Type::FunctionType:
            itemType = aadlinterface::AADLFunctionTypeGraphicsItem::Type;
            break;
        case aadl::AADLObject::Type::ProvidedInterface:
        case aadl::AADLObject::Type::RequiredInterface:
            itemType = aadlinterface::AADLInterfaceGraphicsItem::Type;
            break;
        case aadl::AADLObject::Type::Comment:
            itemType = aadlinterface::AADLCommentGraphicsItem::Type;
            break;
        case aadl::AADLObject::Type::Connection:
            itemType = aadlinterface::AADLConnectionGraphicsItem::Type;
            break;
        case aadl::AADLObject::Type::Unknown:
            continue;
        }
        if (itemType != 0) {
            result.append(itemType);
        }
    }

    return result;
}

qreal itemLevel(const aadl::AADLObject *const object, bool itemSelected)
{
    if (!object || itemSelected) {
        return ZOrder.Selected;
    }

    switch (object->aadlType()) {
    case aadl::AADLObject::Type::Function:
    case aadl::AADLObject::Type::FunctionType:
        return ZOrder.Function;
    case aadl::AADLObject::Type::RequiredInterface:
    case aadl::AADLObject::Type::ProvidedInterface:
        return ZOrder.Interface;
    case aadl::AADLObject::Type::Comment:
        return ZOrder.Comment;
    case aadl::AADLObject::Type::Connection:
        return ZOrder.Connection;
    default:
        return ZOrder.Selected;
    }
}

}
