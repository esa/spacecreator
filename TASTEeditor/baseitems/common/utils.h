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
#pragma once

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QPointF>
#include <QPointer>

class QLineF;
class QPropertyAnimation;

namespace taste3 {
namespace utils {

static constexpr qreal LineHoverTolerance = 10.;
static constexpr qreal LineHorizontalityTolerance = 15.;

QPainterPath lineShape(const QLineF &line, qreal span);
QPointF lineCenter(const QLineF &line);
QPointF pointFromPath(const QPainterPath &path, int num);
QPropertyAnimation *createLinearAnimation(QObject *target, const QString &propName, const QVariant &from,
                                          const QVariant &to, const int durationMs);
QPointF snapToPointByX(const QPointF &target, const QPointF &source, qreal tolerance);
bool intersects(const QRectF &rect, const QLineF &line);

bool isHorizontal(const QLineF &line, const qreal verticalTolerance = LineHorizontalityTolerance);
bool isHorizontal(const QPointF &p1, const QPointF &p2, const qreal verticalTolerance = LineHorizontalityTolerance);
bool isHorizontal(const QVector<QPointF> &twoPoints, const qreal verticalTolerance = LineHorizontalityTolerance);

QRectF framedRect(const QRectF &rect, qreal frameWidth);

} // ns utils
} // ns taste3
