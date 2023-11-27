#pragma once

#include <QLineF>
#include <QList>
#include <QPointF>
#include <QRectF>

namespace topohelp {

using PointsList = QList<QPointF>;
using LinesList = QList<QLineF>;
using RectsList = QList<QRectF>;

namespace utils {

PointsList simplifyPoints(const PointsList &points);
QLineF ifaceSegment(const QRectF &sceneRect, const QPointF &firstEndPoint, const QPointF &lastEndPoint);
PointsList path(const RectsList &existingRects, const QLineF &startDirection, const QLineF &endDirection);

}

} // namespace topohelp
