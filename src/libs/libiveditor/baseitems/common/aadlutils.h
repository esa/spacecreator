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

#include <QGraphicsScene>

class QLineF;
class QPointF;

namespace aadl {
class AADLObject;
}

namespace aadlinterface {

static constexpr qreal LineHoverTolerance = 10.;
static constexpr qreal LineHorizontalityTolerance = 15.;
static const QSizeF DefaultGraphicsItemSize = { 200, 80 };
static const QMarginsF kContentMargins = { 30, 30, 30, 30 };
static const QMarginsF kRootMargins = { 50, 50, 50, 50 };
static const QMarginsF kTextMargins = { 20, 20, 20, 20 };

/*!
  \brief aadlinterface::ZOrder

  This static anonymous struct holds values for items Z ordering:
    \var Topmost
        Maximum available Z level.
    \var Selected
        The Z level for a selected item; When selected, the item is raised to be almost-the-topmost (Topmost-1),
        so a Connection overlapped by a Comment is visible.
    \var Preview
        The alias for Topmost Z level. Used for preview an item during its creation, so it does not go under
  existent/selected items.

    \var Function
        Z level of AADLFunctionGraphicsItem and AADLFunctionTypeGraphicsItem.
    \var Connection
        Z level of AADLConnectionGraphicsItem.
    \var Interface
        Z level of AADLInterfaceGraphicsItem.
    \var Comment
        Z level of AADLCommentGraphicsItem.
    \var GripPoint
        Z level of GripPoint item — alias for Selected (almost-the-topmost).
 */

static const struct {
    const qreal Topmost = 10.;
    const qreal Selected = Topmost - 1.;
    const qreal Preview = Topmost;

    const qreal Function = 0.;
    const qreal Connection = 1.;
    const qreal Interface = 2.;
    const qreal Comment = 3.;
    const qreal GripPoint = Selected;
} ZOrder {};

qreal distanceLine(const QPointF &p1, const QPointF &p2);
qreal distancePolygon(const QVector<QPointF> &polygon);
QList<QPointF> sortedCorners(const QRectF &area, const QPointF &point1, const QPointF &point2);

Qt::Alignment getNearestSide(const QRectF &boundingArea, const QPointF &pos);
QPointF getSidePosition(const QRectF &boundingArea, const QPointF &pos, Qt::Alignment side);

QGraphicsItem *nearestItem(
        const QGraphicsScene *scene, const QPointF &pos, const QList<int> &acceptableTypes = QList<int>());
QGraphicsItem *nearestItem(
        const QGraphicsScene *scene, const QRectF &area, const QList<int> &acceptableTypes = QList<int>());
QGraphicsItem *nearestItem(const QGraphicsScene *scene, const QPointF &center, qreal offset,
        const QList<int> &acceptableTypes = QList<int>());

bool alignedLine(QLineF &line, int angleTolerance = 10);

QPointF pos(const QVector<qint32> &coordinates);
QRectF rect(const QVector<qint32> &coordinates);
QVector<QPointF> polygon(const QVector<qint32> &coordinates);

QVector<qint32> coordinates(const QPointF &point);
QVector<qint32> coordinates(const QRectF &rect);
QVector<qint32> coordinates(const QVector<QPointF> &points);

QRectF adjustFromPoint(const QPointF &pos, const qreal &adjustment);

QList<int> knownGraphicsItemTypes();

qreal itemLevel(const aadl::AADLObject *const object, bool itemSelected);

}
