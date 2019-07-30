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

#include "aadlconnectiongraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"

#include "baseitems/common/utils.h"
#include "baseitems/grippoint.h"
#include "baseitems/common/utils.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QtDebug>
#include <QtMath>

static const qreal kDefaulPenWidth = 2.0;
static const QRectF kGripPointRect = { 0., 0., 12., 12. };
static const QColor kGripPointBackground = QColor::fromRgbF(0, 0, 0.5, 0.75);
static const QColor kGripPointBorder = Qt::red;
static const qreal kSelectionOffset = 10;
static const qreal kMinLineLength = 20;

namespace taste3 {
namespace aadl {

AADLConnectionGraphicsItem::AADLConnectionGraphicsItem(QGraphicsItem *parentItem)
    : QGraphicsObject(parentItem)
    , m_item(new QGraphicsPathItem(this))
{
    setObjectName(QLatin1String("AADLConnectionGraphicsItem"));
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemHasNoContents);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    m_item->setPen(QPen(Qt::black, kDefaulPenWidth));
}

void AADLConnectionGraphicsItem::setPoints(const QVector<QPointF> &points)
{
    Q_ASSERT(points.size() >= 2);
    m_points = points;

    m_startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(utils::nearestItem(scene(), points.first(), QList<int>{ AADLInterfaceGraphicsItem::Type }));
    Q_ASSERT(m_startItem);

    m_endItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(utils::nearestItem(scene(), points.last(), QList<int>{ AADLInterfaceGraphicsItem::Type }));
    Q_ASSERT(m_endItem);

    rebuildLayout();
}

QPainterPath AADLConnectionGraphicsItem::shape() const
{
    QPainterPath pp;
    for (int idx = 1; idx < m_points.size(); ++idx)
        pp.addPath(utils::lineShape(QLineF(m_points.value(idx - 1), m_points.value(idx)), kSelectionOffset));

    return pp;
}

QRectF AADLConnectionGraphicsItem::boundingRect() const
{
    return m_boundingRect;
}

void AADLConnectionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QVariant AADLConnectionGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedChange)
        handleSelectionChanged(value.toBool());

    return QGraphicsItem::itemChange(change, value);
}

bool AADLConnectionGraphicsItem::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMousePress) {
        if (auto grip = qgraphicsitem_cast<QGraphicsRectItem *>(watched))
            return handleGripPointPress(grip, static_cast<QGraphicsSceneMouseEvent *>(event));
    } else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
        if (auto grip = qgraphicsitem_cast<QGraphicsRectItem *>(watched))
            return handleGripPointRelease(grip, static_cast<QGraphicsSceneMouseEvent *>(event));
    } else if (event->type() == QEvent::GraphicsSceneMouseMove) {
        if (auto grip = qgraphicsitem_cast<QGraphicsRectItem *>(watched))
            return handleGripPointMove(grip, static_cast<QGraphicsSceneMouseEvent *>(event));
    }
    return QGraphicsObject::sceneEventFilter(watched, event);
}

void AADLConnectionGraphicsItem::rebuildLayout()
{
    auto updateBoundingRect = [this](){
        QPainterPath pp;
        pp.addPolygon(QPolygonF(m_points));
        m_item->setPath(pp);
        m_boundingRect = pp.boundingRect();
        updateGripPoints();
    };

    prepareGeometryChange();
    if (!m_startItem || !m_endItem || m_points.size() < 2) {
        updateBoundingRect();
        return;
    }

    m_startItem->setPos(m_startItem->parentItem()->mapFromScene(m_points.first()));
    m_startItem->instantLayoutUpdate();
    const QRectF startSceneRect = m_startItem->sceneBoundingRect();
    if (!startSceneRect.contains(m_points.first()))
        m_points.prepend(m_startItem->scenePos());

    m_endItem->setPos(m_endItem->parentItem()->mapFromScene(m_points.last()));
    m_endItem->instantLayoutUpdate();
    const QRectF endSceneRect = m_endItem->sceneBoundingRect();
    if (!endSceneRect.contains(m_points.last()))
        m_points.append(m_endItem->scenePos());

    int newStart = -1, newEnd = -1;
    for (int idx = 1; idx < m_points.size() - 1; ++idx) {
        const QPointF &point = m_points.at(idx);
        if (startSceneRect.contains(point))
            newStart = idx;
        if (newEnd == -1 && endSceneRect.contains(point))
            newEnd = idx;
    }
    if (newEnd != -1 && newStart != -1 && newEnd < newStart)
        qSwap(newEnd, newStart);
    if (newEnd != -1)
        m_points.resize(newEnd + 1);
    while (--newStart >= 0)
        m_points.removeFirst();

    simplify();
    updateBoundingRect();
}

void AADLConnectionGraphicsItem::updateGripPoints()
{
    for (int idx = 1; idx < m_points.size(); ++idx) {
        const QPointF point = utils::lineCenter(QLineF(m_points.value(idx - 1), m_points.value(idx)));
        QGraphicsRectItem *grip = idx - 1 < m_grips.size() ? m_grips.value(idx - 1) : createGripPoint();
        QRectF br = grip->rect();
        br.moveCenter(point);
        grip->setRect(br);
    }
    while (m_grips.size() > m_points.size() - 1) {
        if (auto grip = m_grips.takeLast()) {
            if (auto scene = grip->scene())
                scene->removeItem(grip);
            delete grip;
        }
    }
}

void AADLConnectionGraphicsItem::handleSelectionChanged(bool isSelected)
{
    QPen pen = m_item->pen();
    pen.setWidthF(isSelected ? 1.5 * kDefaulPenWidth : kDefaulPenWidth);
    pen.setStyle(isSelected ? Qt::DotLine : Qt::SolidLine);
    m_item->setPen(pen);

    if (isSelected) {
        updateGripPoints();
    } else {
        qDeleteAll(m_grips);
        m_grips.clear();
    }
}

bool AADLConnectionGraphicsItem::handleGripPointPress(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    const int idx = m_grips.indexOf(handle);
    if (idx == -1)
        return false;

    return true;
}

bool AADLConnectionGraphicsItem::handleGripPointMove(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event)
{
    const int idx = m_grips.indexOf(handle);
    if (idx == -1)
        return false;

    const QPointF p0 = event->scenePos();
    const QPointF p1 = m_points.value(idx);
    const QPointF p2 = m_points.value(idx + 1);
    const QLineF current { p1, p2 };
    QLineF path { event->lastScenePos(), p0 };
    const qreal length = path.length() * qSin(qDegreesToRadians(current.angleTo(path)));
    path.setAngle(current.angle() + (length < 0 ? -90 : 90));
    path.setLength(qAbs(length));
    const QPointF delta = path.p2() - path.p1();

    m_points[idx] += delta;
    m_points[idx + 1] += delta;

    rebuildLayout();
    return true;
}

bool AADLConnectionGraphicsItem::handleGripPointRelease(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    const int idx = m_grips.indexOf(handle);
    if (idx == -1)
        return false;

    return true;
}

QGraphicsRectItem *AADLConnectionGraphicsItem::createGripPoint()
{
    QGraphicsRectItem *gripPoint =
            scene()->addRect(kGripPointRect, QPen(kGripPointBorder), QBrush(kGripPointBackground));
    gripPoint->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIgnoresTransformations);
    gripPoint->installSceneEventFilter(this);
    m_grips.append(gripPoint);
    return gripPoint;
}

void AADLConnectionGraphicsItem::simplify()
{
    if (m_points.size() <= 3)
        return;

    const int pointsCount = m_points.size();
    for (int idx = 1; idx < m_points.size() - 2; ++idx) {
        const QLineF currentLine { m_points.value(idx), m_points.value(idx + 1) };
        QLineF prevLine { m_points.value(idx - 1), m_points.value(idx) };
        QLineF nextLine { m_points.value(idx + 1), m_points.value(idx + 2) };

        if (qFuzzyCompare(prevLine.angle(), nextLine.angle()) && currentLine.length() < kMinLineLength) {
            const QPointF midPoint = utils::lineCenter(currentLine);
            const QPointF prevOffset = midPoint - currentLine.p1();
            m_points[idx - 1] = prevLine.p1() + prevOffset;
            const QPointF nextOffset = midPoint - currentLine.p2();
            m_points[idx + 2] = nextLine.p2() + nextOffset;
            m_points.removeAt(idx + 1);
            m_points.removeAt(idx);
        }
    }
    if (m_points.size() != pointsCount)
        rebuildLayout();
}

} // namespace aadl
} // namespace taste3
