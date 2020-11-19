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
static const QList<Qt::Alignment> kRectSides = { Qt::AlignLeft, Qt::AlignTop, Qt::AlignRight, Qt::AlignBottom };
static const qreal kInterfaceLayoutOffset = 12.0;
static const qreal kConnectionMargin = 20;
static const qreal kMinLineLength = 20;

/*!
 * \brief The tolerance used to find an AADLInterfaceGraphicsItem on scene (the size of a square used as a search area)
 */
static const qreal kInterfaceTolerance = 5.;

/*!
 * \brief The tolerance used to find an AADLFunction(Type)GraphicsItem on scene (the size of a square used as a search
 * area)
 */
static const qreal kFunctionTolerance = 20.;

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

    const qreal Connection = 0.;
    const qreal Function = 1.;
    const qreal InternalConnection = 1.5;
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

QRectF alignRectToSide(
        const QRectF &boundingRect, const QRectF &itemRect, Qt::Alignment side, const QPointF &originPointOffset);

QRectF adjustedRect(
        const QRectF &itemRect, const QRectF &intersectedItemRect, const Qt::Alignment side, const bool clockwise);

Qt::Alignment sideFromIndex(const int idx);
int indexFromSide(Qt::Alignment side);

bool checkCollision(const QList<QRectF> &itemRects, const QRectF &itemRect, QRectF *collidingRect = nullptr);

QList<QVector<QPointF>> generateSegments(const QPointF &startPoint, const QPointF &endPoint);

QVector<QPointF> generateSegments(const QLineF &startDirection, const QLineF &endDirection);

QLineF ifaceSegment(const QRectF &sceneRect, const QPointF &firstEndPoint, const QPointF &lastEndPoint);

QList<QVector<QPointF>> findSubPath(const QRectF &itemRect, const QVector<QPointF> &prevPoints,
        const QVector<QPointF> &nextPoints, bool strict = true);

QVector<QPointF> findPath(
        QGraphicsScene *scene, const QLineF &startDirection, const QLineF &endDirection, QRectF *intersectedRect);

QVector<QPointF> path(QGraphicsScene *scene, const QPointF &startPoint, const QPointF &endPoint);

QVector<QPointF> path(QGraphicsScene *scene, const QLineF &startDirection, const QLineF &endDirection);

QVector<QPointF> createConnectionPath(QGraphicsScene *scene, const QPointF &startIfacePos, const QRectF &sourceRect,
        const QPointF &endIfacePos, const QRectF &targetRect);

QVector<QPointF> simplifyPoints(const QVector<QPointF> &points);

bool comparePolygones(const QVector<QPointF> &v1, const QVector<QPointF> &v2);

int nestingLevel(aadl::AADLObject *object);

} // namespace aadlinterface
