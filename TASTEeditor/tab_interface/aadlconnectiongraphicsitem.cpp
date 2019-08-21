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

#include "aadlcontainergraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "baseitems/common/utils.h"
#include "baseitems/grippoint.h"
#include "tab_aadl/aadlobjectiface.h"

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
static const qreal kConnectionMargin = 20;

namespace taste3 {
namespace aadl {

static inline QList<QVector<QPointF>> generateConnection(const QPointF &startPoint, const QPointF &endPoint)
{
    if (startPoint == endPoint)
        return {};

    if (qFuzzyCompare(startPoint.x(), endPoint.x()) || qFuzzyCompare(startPoint.y(), endPoint.y()))
        return { { startPoint, endPoint } };

    return { { startPoint, { startPoint.x(), endPoint.y() }, endPoint },
             { startPoint, { endPoint.x(), startPoint.y() }, endPoint } };
}

static inline QVector<QPointF> generateConnection(const QLineF &startDirection, const QLineF &endDirection)
{
    QVector<QPointF> connectionPoints { startDirection.p2(), endDirection.p2() };
    const qreal angle = startDirection.angleTo(endDirection);
    static const qreal tolerance = 0.1;
    if (qAbs(qSin(qDegreesToRadians(angle))) <= tolerance) { // ||
        const QPointF mid = utils::lineCenter(QLineF(connectionPoints.first(), connectionPoints.last()));
        QLineF midLine { mid, QPointF(0, 0) };
        midLine.setAngle(startDirection.angle() - 90);

        QPointF startLastPoint;
        midLine.intersect(startDirection, &startLastPoint);
        connectionPoints.insert(connectionPoints.size() - 1, startLastPoint);

        QPointF endLastPoint;
        midLine.intersect(endDirection, &endLastPoint);
        connectionPoints.insert(connectionPoints.size() - 1, endLastPoint);
    } else if (qAbs(qCos(qDegreesToRadians(angle))) <= tolerance) { // |_
        QPointF mid;
        startDirection.intersect(endDirection, &mid);
        connectionPoints.insert(connectionPoints.size() - 1, mid);
    } else {
        qCritical() << startDirection << endDirection << angle;
    }
    return connectionPoints;
}

static inline qreal distanceLine(const QPointF &p1, const QPointF &p2)
{
    return std::sqrt(std::pow((p2.x() - p1.x()), 2) + std::pow((p2.y() - p1.y()), 2));
};

static inline qreal distancePolygon(const QVector<QPointF> &polygon)
{
    qreal distance = 0;
    for (auto it = std::next(polygon.constBegin()); it != polygon.constEnd(); ++it)
        distance += distanceLine(*std::prev(it), *it);
    return distance;
};
static inline QList<QPointF> sortedCorners(const QRectF &area, const QPointF &point1, const QPointF &point2)
{
    QList<QPointF> rectPoints { area.topLeft(), area.topRight(), area.bottomLeft(), area.bottomRight() };
    std::sort(rectPoints.begin(), rectPoints.end(), [=](const QPointF &p1, const QPointF &p2) {
        return distancePolygon({ point1, p1, point2 }) < distancePolygon({ point1, p2, point2 });
    });
    return rectPoints;
};

static inline QLineF getDirection(const QRectF &sceneRect, const QPointF &point)
{
    switch (utils::getNearestSide(sceneRect, point)) {
    case Qt::AlignTop:
        return QLineF(sceneRect.topLeft(), sceneRect.topRight()).normalVector();
    case Qt::AlignBottom:
        return QLineF(sceneRect.bottomRight(), sceneRect.bottomLeft()).normalVector();
    case Qt::AlignLeft:
        return QLineF(sceneRect.bottomLeft(), sceneRect.topLeft()).normalVector();
    case Qt::AlignRight:
        return QLineF(sceneRect.topRight(), sceneRect.bottomRight()).normalVector();
    default:
        break;
    }
    return QLineF();
};

static inline QList<QVector<QPointF>> findSubPath(const QRectF &itemRect, const QVector<QPointF> &prevPoints,
                                                  const QVector<QPointF> &nextPoints)
{
    Q_ASSERT(itemRect.isValid());
    Q_ASSERT(!prevPoints.isEmpty());
    Q_ASSERT(!nextPoints.isEmpty());

    if (!itemRect.isValid() || prevPoints.isEmpty() || nextPoints.isEmpty())
        return {};

    const QPointF startPoint = prevPoints.last();
    const QPointF endPoint = nextPoints.last();
    Q_ASSERT(startPoint != endPoint);
    if (startPoint == endPoint)
        return {};

    const QRectF itemRectMargins =
            itemRect.adjusted(-kConnectionMargin, -kConnectionMargin, kConnectionMargin, kConnectionMargin);
    const QList<QPointF> rectCorners = sortedCorners(itemRectMargins, startPoint, endPoint);
    QList<QVector<QPointF>> allPaths;
    for (const QPointF &p : rectCorners) {
        for (auto polygon : generateConnection(startPoint, p)) {
            if (!utils::intersects(itemRect, polygon) && !utils::intersects(itemRect, QLineF(p, endPoint))) {
                QVector<QPointF> previousPoints(prevPoints);
                previousPoints.removeLast();
                previousPoints << polygon;
                allPaths << previousPoints;
            }
        }
    }
    return allPaths;
}

static inline QVector<QPointF> findPath(QGraphicsScene *scene, const QLineF &startDirection, const QLineF &endDirection,
                                        QRectF *intersectedRect)
{
    static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLContainerGraphicsItem::Type };
    QVector<QPointF> points = generateConnection(startDirection, endDirection);
    const QList<QGraphicsItem *> intersectedItems = scene->items(points);

    qreal distance = std::numeric_limits<qreal>::max();
    QGraphicsItem *intersectionItem = nullptr;
    for (auto it = intersectedItems.constBegin(); it != intersectedItems.constEnd(); ++it) {
        if (!types.contains((*it)->type()))
            continue;

        QPointF intersectionPoint;
        if (utils::intersects((*it)->sceneBoundingRect(), points, &intersectionPoint)
            && QLineF(startDirection.p2(), intersectionPoint).length() < distance) {
            intersectionItem = *it;
        }
    }
    if (!intersectionItem)
        return points;

    *intersectedRect = intersectionItem->sceneBoundingRect();
    return {};
}

static inline QVector<QPointF> path(QGraphicsScene *scene, const QLineF &startDirection, const QLineF &endDirection)
{
    QRectF intersectedRect;
    QList<QVector<QPointF>> paths { { startDirection.p1(), startDirection.p2() } };
    while (true) {
        QList<QVector<QPointF>> deeper;
        QList<QVector<QPointF>> results;
        for (auto path : paths) {
            Q_ASSERT(path.size() >= 2);
            if (path.size() < 2)
                return {};

            const QLineF prevDirection { path.value(path.size() - 2), path.value(path.size() - 1) };
            auto shortPath = findPath(scene, prevDirection, endDirection, &intersectedRect);
            if (!shortPath.isEmpty()) {
                QVector<QPointF> result;
                result.append(startDirection.p1());
                result.append(path);
                result.append(shortPath);
                result.append(endDirection.p1());
                results.append(result);
                continue;
            }
            const auto subPaths = findSubPath(intersectedRect, path, { endDirection.p1(), endDirection.p2() });
            static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLContainerGraphicsItem::Type };
            for (auto subPath : subPaths) {
                if (subPath.isEmpty())
                    continue;

                const QList<QGraphicsItem *> intersectedItems = scene->items(subPath);
                auto it = std::find_if(
                        intersectedItems.constBegin(), intersectedItems.constEnd(), [subPath](QGraphicsItem *item) {
                            if (!types.contains(item->type()))
                                return false;
                            return utils::intersectionPoints(item->sceneBoundingRect(), subPath).size() > 1;
                        });
                if (it != intersectedItems.constEnd())
                    continue;
                else if (subPath.last() == endDirection.p2())
                    results.append(subPath);
                else
                    deeper.append(subPath);
            }
        }
        if (!results.isEmpty()) {
            std::sort(results.begin(), results.end(), [](const QVector<QPointF> &v1, const QVector<QPointF> &v2) {
                return distancePolygon(v1) < distancePolygon(v2);
            });

            return results.first();
        }

        if (paths.size() != deeper.size() || !std::equal(paths.constBegin(), paths.constEnd(), deeper.constBegin()))
            paths = deeper;
        else if (!deeper.isEmpty())
            return deeper.front();
        else
            break;
    }
    return {};
}

static inline QVector<QPointF> updatePoints(QGraphicsScene *scene, AADLInterfaceGraphicsItem *startItem,
                                            AADLInterfaceGraphicsItem *endItem)
{
    if (!startItem || !endItem)
        return {};

    const QPointF startPointAdjusted = startItem->scenePos();
    QLineF startDirection = getDirection(startItem->targetItem()->sceneBoundingRect(), startPointAdjusted);
    if (startDirection.isNull())
        return {};

    startDirection.translate(startPointAdjusted - startDirection.p1());
    startDirection.setLength(kConnectionMargin);

    const QPointF endPointAdjusted = endItem->scenePos();
    QLineF endDirection = getDirection(endItem->targetItem()->sceneBoundingRect(), endPointAdjusted);
    if (endDirection.isNull())
        return {};

    endDirection.translate(endPointAdjusted - endDirection.p1());
    endDirection.setLength(kConnectionMargin);

    const auto points = path(scene, startDirection, endDirection);
    return AADLConnectionGraphicsItem::simplify(points);
}

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
    if (points.isEmpty())
        return;

    m_points = points;

    auto startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
            utils::nearestItem(scene(), points.first(), QList<int> { AADLInterfaceGraphicsItem::Type }));
    if (startItem != m_startItem) {
        if (m_startItem)
            m_startItem->disconnect(this);

        m_startItem = startItem;
        connect(m_startItem, &InteractiveObject::moved, this, [this](InteractiveObject *iObj) {
            if (m_points.size() < 2 || iObj != m_startItem)
                return;

            const QPointF offset = m_startItem->scenePos() - m_points.first();
            m_points[0] = m_startItem->scenePos();
            m_points[1] += offset;
            rebuildLayout();
        });
    }
    Q_ASSERT(m_startItem);

    auto endItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
            utils::nearestItem(scene(), points.last(), QList<int> { AADLInterfaceGraphicsItem::Type }));
    if (endItem != m_endItem) {
        if (m_endItem)
            m_endItem->disconnect(this);

        m_endItem = endItem;
        connect(m_endItem, &InteractiveObject::moved, this, [this](InteractiveObject *iObj) {
            if (m_points.size() < 2 || iObj != m_endItem)
                return;

            const QPointF offset = m_endItem->scenePos() - m_points.last();
            m_points.last() = m_endItem->scenePos();
            m_points[m_points.size() - 2] += offset;
            rebuildLayout();
        });
    }
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
    prepareGeometryChange();
    if (m_startItem && m_endItem && m_points.size() >= 2)
        m_points = updatePoints(scene(), m_startItem, m_endItem);

    updateBoundingRect();
}

void AADLConnectionGraphicsItem::updateBoundingRect()
{
    QPainterPath pp;
    pp.addPolygon(QPolygonF(m_points));
    m_item->setPath(pp);
    m_boundingRect = pp.boundingRect();
    updateGripPoints();
};

void AADLConnectionGraphicsItem::updateGripPoints(bool forceVisible)
{
    if (m_points.isEmpty())
        return;

    for (int idx = 1; idx < m_points.size(); ++idx) {
        const QPointF point = utils::lineCenter(QLineF(m_points.value(idx - 1), m_points.value(idx)));
        QGraphicsRectItem *grip = idx - 1 < m_grips.size() ? m_grips.value(idx - 1) : createGripPoint();
        QRectF br = grip->rect();
        br.moveCenter(point);
        grip->setRect(br);
        grip->setVisible(forceVisible || isSelected());
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
        updateGripPoints(isSelected);
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

    m_tmpPoints = m_points;

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

    auto updateEdgeItem = [this](InteractiveObject *item, bool isStart) {
        QSignalBlocker sb(item);
        item->setPos(item->parentItem()->mapFromScene(isStart ? m_points.first() : m_points.last()));
        item->instantLayoutUpdate();
        const QRectF sceneRect = item->sceneBoundingRect();
        if (sceneRect.contains(isStart ? m_points.first() : m_points.last()))
            return;

        if (isStart)
            m_points.prepend(item->scenePos());
        else
            m_points.append(item->scenePos());
    };

    updateEdgeItem(m_startItem, true);
    updateEdgeItem(m_endItem, false);

    const QRectF startSceneRect = m_startItem->sceneBoundingRect();
    const QRectF endSceneRect = m_endItem->sceneBoundingRect();
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
    return true;
}

bool AADLConnectionGraphicsItem::handleGripPointRelease(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    const int idx = m_grips.indexOf(handle);
    if (idx == -1)
        return false;

    static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLContainerGraphicsItem::Type };
    for (auto item : scene()->items(m_points)) {
        if (types.contains(item->type())
            && utils::intersectionPoints(item->sceneBoundingRect(), QPolygonF(m_points)).size() > 1) {
            setPoints(m_tmpPoints);
            break;
        }
    }

    m_tmpPoints.clear();
    updateBoundingRect();
    return true;
}

AADLConnectionGraphicsItem *
AADLConnectionGraphicsItem::createConnection(QGraphicsScene *scene, const QPointF &startPoint, const QPointF &endPoint)
{
    static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLContainerGraphicsItem::Type };
    QGraphicsItem *startItem = utils::nearestItem(scene, startPoint, types);
    QGraphicsItem *endItem = utils::nearestItem(scene, endPoint, types);
    if (!startItem || !endItem)
        return nullptr;

    const QLineF connectionLine { startPoint, endPoint };

    QPointF startPointAdjusted(startPoint);
    if (!utils::intersects(startItem->sceneBoundingRect(), connectionLine, &startPointAdjusted))
        return nullptr;

    QPointF endPointAdjusted(endPoint);
    if (!utils::intersects(endItem->sceneBoundingRect(), connectionLine, &endPointAdjusted))
        return nullptr;

    auto startIfaceItem =
            new AADLInterfaceGraphicsItem(new AADLObjectIface(AADLObjectIface::IfaceType::Provided, tr("PI")));
    startIfaceItem->setTargetItem(startItem, startPointAdjusted);

    auto endIfaceItem =
            new AADLInterfaceGraphicsItem(new AADLObjectIface(AADLObjectIface::IfaceType::Required, tr("RI")));
    endIfaceItem->setTargetItem(endItem, endPointAdjusted);

    auto connectionItem = new AADLConnectionGraphicsItem();
    scene->addItem(connectionItem);
    connectionItem->setPoints(updatePoints(scene, startIfaceItem, endIfaceItem));
    return connectionItem;
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
    m_points = simplify(m_points);
}

QVector<QPointF> AADLConnectionGraphicsItem::simplify(const QVector<QPointF> &points)
{
    if (points.size() <= 3)
        return points;

    /// TODO: optimize flow
    QVector<QPointF> simplifiedPoints(points);

    for (int idx = 0; idx < simplifiedPoints.size() - 1;) {
        const QLineF currentLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx + 1) };
        if (qFuzzyIsNull(currentLine.length())) {
            simplifiedPoints.removeAt(idx + 1);
            continue;
        }
        if (idx + 2 < simplifiedPoints.size()) {
            const QLineF nextLine { simplifiedPoints.value(idx + 1), simplifiedPoints.value(idx + 2) };
            if (qFuzzyIsNull(nextLine.length()) || qFuzzyCompare(currentLine.angle(), nextLine.angle())
                || int(currentLine.angleTo(nextLine)) % 180 == 0) {
                simplifiedPoints.removeAt(idx + 1);
                continue;
            }
        }

        ++idx;
    }
    for (int idx = 1; idx < simplifiedPoints.size() - 2; ++idx) {
        const QLineF currentLine { simplifiedPoints.value(idx), simplifiedPoints.value(idx + 1) };
        const QLineF prevLine { simplifiedPoints.value(idx - 1), simplifiedPoints.value(idx) };
        const QLineF nextLine { simplifiedPoints.value(idx + 1), simplifiedPoints.value(idx + 2) };

        if (qFuzzyCompare(prevLine.angle(), nextLine.angle()) && currentLine.length() < kMinLineLength) {
            const QPointF midPoint = utils::lineCenter(currentLine);
            const QPointF prevOffset = midPoint - currentLine.p1();
            simplifiedPoints[idx - 1] = prevLine.p1() + prevOffset;
            const QPointF nextOffset = midPoint - currentLine.p2();
            simplifiedPoints[idx + 2] = nextLine.p2() + nextOffset;
            simplifiedPoints.removeAt(idx + 1);
            simplifiedPoints.removeAt(idx);
        }
    }
    return simplifiedPoints;
}

} // namespace aadl
} // namespace taste3
