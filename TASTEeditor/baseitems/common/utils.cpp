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

#include <QDebug>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QtGlobal>

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

bool intersects(const QRectF &rect, const QLineF &line)
{
    if (rect.isNull() || line.isNull())
        return false;

    const QVector<QLineF> rectLines = {
        { rect.topLeft(), rect.topRight() },
        { rect.topRight(), rect.bottomRight() },
        { rect.bottomRight(), rect.bottomLeft() },
        { rect.bottomLeft(), rect.topLeft() },
    };

    QPointF intersectionPoint;
    for (const QLineF &rectLine : rectLines)
        if (rectLine.intersect(line, &intersectionPoint) == QLineF::BoundedIntersection)
            return true;

    return false;
}

bool isHorizontal(const QLineF &line, const qreal verticalTolerance)
{
    return qAbs(line.dy()) < verticalTolerance;
}
bool isHorizontal(const QPointF &p1, const QPointF &p2, const qreal verticalTolerance)
{
    return isHorizontal(QLineF(p1, p2), verticalTolerance);
}
bool isHorizontal(const QVector<QPointF> &twoPoints, const qreal verticalTolerance)
{
    return isHorizontal(twoPoints.first(), twoPoints.last(), verticalTolerance);
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
    Qt::Alignment alignment = Qt::AlignCenter;
    qreal distance = std::numeric_limits<qreal>::max();
    const qreal leftDistance = qAbs(pos.x() - boundingArea.left());
    if (leftDistance < distance) {
        distance = leftDistance;
        alignment = Qt::AlignLeft;
    }
    const qreal topDistance = pos.y() - boundingArea.top();
    if (topDistance < distance) {
        distance = topDistance;
        alignment = Qt::AlignTop;
    }
    const qreal rightDistance = boundingArea.width() - pos.x();
    if (rightDistance < distance) {
        distance = rightDistance;
        alignment = Qt::AlignRight;
    }
    const qreal bottomDistance = boundingArea.height() - pos.y();
    if (bottomDistance < distance) {
        distance = bottomDistance;
        alignment = Qt::AlignBottom;
    }
    return alignment;
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

QGraphicsItem *nearestItem(QGraphicsScene *scene, const QPointF &center, qreal offset, const QList<int> &acceptableTypes)
{
    const QRectF area { center - QPointF(offset / 2, offset / 2), center + QPointF(offset / 2, offset / 2) };
    return nearestItem(scene, area, acceptableTypes);
}

} // ns utils
} // ns taste3
