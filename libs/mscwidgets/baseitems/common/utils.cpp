/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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
#include "instanceitem.h"

#include <QPropertyAnimation>

namespace msc {
namespace utils {

/*!
  \namespace msc::utils
  \brief Collection of utils for internal use.

  \inmodule MscWidgets

*/

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

QPointF pointFromPath(const QPainterPath &path, int num)
{
    QPointF result;
    if (!path.isEmpty()) {
        num = qBound(0, num, path.elementCount()-1);
        result = path.elementAt(num);
    }
    return result;
}

QPropertyAnimation *createLinearAnimation(QObject *target,
                                          const QString &propName,
                                          const QVariant &from,
                                          const QVariant &to,
                                          const int durationMs)
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

} // ns utils
} // ns msc
