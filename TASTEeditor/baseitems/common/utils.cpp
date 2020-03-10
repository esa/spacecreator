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

#include "utils.h"

#include "baseitems/interactiveobject.h"
#include "tab_aadl/aadlobject.h"

#include <QGraphicsView>
#include <QMetaEnum>
#include <QPropertyAnimation>
#include <QtGlobal>
#include <QtMath>

namespace taste3 {
namespace utils {

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

QPointF lineCenter(const QLineF &line)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    return line.center();
#else
    return 0.5 * line.p1() + 0.5 * line.p2();
#endif
}

QPointF pointFromPath(const QPainterPath &path, int num)
{
    QPointF result;
    if (!path.isEmpty()) {
        num = qBound(0, num, path.elementCount() - 1);
        result = path.elementAt(num);
    }
    return result;
}

QPropertyAnimation *createLinearAnimation(QObject *target, const QString &propName, const QVariant &from,
                                          const QVariant &to, const int durationMs)
{
    QPropertyAnimation *anim = new QPropertyAnimation(target, propName.toUtf8());
    anim->setDuration(durationMs);
    anim->setEasingCurve(QEasingCurve::Linear);

    anim->setStartValue(from);
    anim->setEndValue(to);

    return anim;
}

QPointF snapToPointByX(const QPointF &target, const QPointF &source, qreal tolerance)
{
    QPointF result(source);
    const QLineF delta(source, target);
    if (qAbs(delta.dx()) <= tolerance) {
        result.rx() += delta.dx();
    }
    return result;
}

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
        if (rectLine.intersect(line, intersectPos) == QLineF::BoundedIntersection)
            return true;
    }

    return false;
}

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
            if (rectLine.intersect(line, &intersectPos) == QLineF::BoundedIntersection)
                points.append(intersectPos);
        }
    }
    return points;
}

bool intersects(const QRectF &rect, const QPolygonF &polygon, QPointF *intersectPos)
{
    for (int idx = 1; idx < polygon.size(); ++idx) {
        if (intersects(rect, QLineF(polygon.value(idx - 1), polygon.value(idx)), intersectPos))
            return true;
    }
    return false;
}

QRectF framedRect(const QRectF &rect, qreal frameWidth)
{
    if (qFuzzyIsNull(frameWidth))
        return rect;

    const qreal halfWidth = frameWidth / 2.;
    return rect.adjusted(halfWidth, halfWidth, -halfWidth, -halfWidth);
}

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
        auto it = std::find_if(rectLines.constBegin(), rectLines.constEnd(),
                               [line](const QPair<QLineF, Qt::Alignment> &rectSide) {
                                   QPointF dummyPoint;
                                   return line.intersect(rectSide.first, &dummyPoint) == QLineF::BoundedIntersection;
                               });
        if (it != rectLines.constEnd())
            side = it->second;
    }
    return side;
}

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

QGraphicsItem *nearestItem(QGraphicsScene *scene, const QPointF &pos, const QList<int> &acceptableTypes)
{

    for (QGraphicsItem *item : scene->items(pos)) {
        if (acceptableTypes.contains(item->type()))
            return item;
    }
    return nullptr;
}

QGraphicsItem *nearestItem(QGraphicsScene *scene, const QRectF &area, const QList<int> &acceptableTypes)
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

QGraphicsItem *nearestItem(QGraphicsScene *scene, const QPointF &center, qreal offset,
                           const QList<int> &acceptableTypes)
{
    const QRectF area { center - QPointF(offset / 2, offset / 2), center + QPointF(offset / 2, offset / 2) };
    return nearestItem(scene, area, acceptableTypes);
}

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

qreal distanceLine(const QPointF &p1, const QPointF &p2)
{
    return std::sqrt(std::pow((p2.x() - p1.x()), 2) + std::pow((p2.y() - p1.y()), 2));
}

qreal distancePolygon(const QVector<QPointF> &polygon)
{
    qreal distance = 0;
    for (auto it = std::next(polygon.constBegin()); it != polygon.constEnd(); ++it)
        distance += distanceLine(*std::prev(it), *it);
    return distance;
}

QList<QPointF> sortedCorners(const QRectF &area, const QPointF &point1, const QPointF &point2)
{
    QList<QPointF> rectPoints { area.topLeft(), area.topRight(), area.bottomLeft(), area.bottomRight() };
    std::sort(rectPoints.begin(), rectPoints.end(), [=](const QPointF &p1, const QPointF &p2) {
        return distancePolygon({ point1, p1, point2 }) < distancePolygon({ point1, p2, point2 });
    });
    return rectPoints;
}

bool isOnEdge(const QRectF &rect, const QPointF &point)
{
    const bool isHorizontalEdge = qFuzzyCompare(rect.top(), point.y()) || qFuzzyCompare(rect.bottom(), point.y());
    const bool isVericalEdge = qFuzzyCompare(rect.left(), point.x()) || qFuzzyCompare(rect.right(), point.x());

    const bool isHorBounded = rect.left() < point.x() && rect.right() < point.x();
    const bool isVerBounded = rect.top() < point.y() && rect.bottom() < point.y();

    return (isHorizontalEdge && (isVericalEdge || isHorBounded))
            || (isVericalEdge && (isHorizontalEdge || isVerBounded));
}

QPointF pos(const QVector<qint32> &coordinates)
{
    if (coordinates.isEmpty())
        return {};

    Q_ASSERT(coordinates.size() == 2);
    if (coordinates.size() != 2)
        return {};

    return QPointF(coordinates.first(), coordinates.last());
}

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
    static QList<int> result;

    if (result.isEmpty()) {
        const QMetaEnum &me = QMetaEnum::fromType<taste3::aadl::AADLObject::AADLObjectType>();
        for (int i = 0; i < me.keyCount(); ++i)
            if (static_cast<aadl::AADLObject::AADLObjectType>(me.value(i))
                != aadl::AADLObject::AADLObjectType::AADLUnknown)
                result.append(QGraphicsItem::UserType + me.value(i));
    }

    return result;
}

} // ns utils
} // ns taste3
