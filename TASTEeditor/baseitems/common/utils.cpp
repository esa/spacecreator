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

} // ns utils
} // ns taste3
