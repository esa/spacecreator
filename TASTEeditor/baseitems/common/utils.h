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

#include <QAction>
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
static constexpr QSizeF DefaultGraphicsItemSize = { 200, 80 };
static const QMarginsF kContentMargins = { 30, 30, 30, 30 };
static const QMarginsF kRootMargins = { 50, 50, 50, 50 };
static const QMarginsF kTextMargins = { 20, 20, 20, 20 };

QPainterPath lineShape(const QLineF &line, qreal span);
QPointF lineCenter(const QLineF &line);
QPointF pointFromPath(const QPainterPath &path, int num);
QPropertyAnimation *createLinearAnimation(QObject *target, const QString &propName, const QVariant &from,
                                          const QVariant &to, const int durationMs);
QPointF snapToPointByX(const QPointF &target, const QPointF &source, qreal tolerance);
bool intersects(const QRectF &rect, const QLineF &line, QPointF *intersectPos = nullptr);
bool intersects(const QRectF &rect, const QPolygonF &polygon, QPointF *intersectPos = nullptr);
QVector<QPointF> intersectionPoints(const QRectF &rect, const QPolygonF &polygon);

qreal distanceLine(const QPointF &p1, const QPointF &p2);
qreal distancePolygon(const QVector<QPointF> &polygon);
QList<QPointF> sortedCorners(const QRectF &area, const QPointF &point1, const QPointF &point2);

QRectF framedRect(const QRectF &rect, qreal frameWidth);

Qt::Alignment getNearestSide(const QRectF &boundingArea, const QPointF &pos);
QPointF getSidePosition(const QRectF &boundingArea, const QPointF &pos, Qt::Alignment side);

QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QPointF &pos,
                           const QList<int> &acceptableTypes = QList<int>());
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QRectF &area,
                           const QList<int> &acceptableTypes = QList<int>());
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QPointF &center, qreal offset,
                           const QList<int> &acceptableTypes = QList<int>());

bool alignedLine(QLineF &line, int angleTolerance = 10);

bool isOnEdge(const QRectF &rect, const QPointF &point);

QPointF pos(const QVector<qint32> &coordinates);
QRectF rect(const QVector<qint32> &coordinates);
QVector<QPointF> polygon(const QVector<qint32> &coordinates);

QVector<qint32> coordinates(const QPointF &point);
QVector<qint32> coordinates(const QRectF &rect);
QVector<qint32> coordinates(const QVector<QPointF> &points);

template<typename T>
bool isAncestorOf(const T *const parent, T *object)
{
    if (!parent || !object)
        return false;

    if (parent == object)
        return true;

    T *obj { object };
    while (auto objParent = qobject_cast<T *>(obj->parent())) {
        if (objParent == parent)
            return true;

        obj = objParent;
    }

    return false;
}

QRectF adjustFromPoint(const QPointF &pos, const qreal &adjustment);

QList<int> knownGraphicsItemTypes();

} // ns utils
} // ns taste3
