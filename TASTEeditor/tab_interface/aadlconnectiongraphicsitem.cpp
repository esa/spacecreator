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

#include "aadlfunctiontypegraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "baseitems/common/utils.h"
#include "baseitems/grippoint.h"
#include "commands/cmdentitygeometrychange.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "tab_aadl/aadlobjectconnection.h"
#include "tab_aadl/aadlobjectiface.h"

#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QPen>
#include <QtDebug>
#include <QtMath>
#include <app/commandsstack.h>

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
    static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLFunctionTypeGraphicsItem::Type };
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
            static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLFunctionTypeGraphicsItem::Type };
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
                if (v1.size() == v2.size())
                    return distancePolygon(v1) < distancePolygon(v2);
                return v1.size() < v2.size();
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

    if (startItem->targetItem()->sceneBoundingRect().contains(endPointAdjusted))
        startDirection.setAngle(180 + startDirection.angle());
    if (endItem->targetItem()->sceneBoundingRect().contains(startPointAdjusted))
        endDirection.setAngle(180 + endDirection.angle());

    const auto points = path(scene, startDirection, endDirection);
    return AADLConnectionGraphicsItem::simplify(points);
}

AADLConnectionGraphicsItem::GraphicsPathItem::GraphicsPathItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
}

QPainterPath AADLConnectionGraphicsItem::GraphicsPathItem::shape() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(kSelectionOffset);
    stroker.setJoinStyle(Qt::MiterJoin);

    return stroker.createStroke(path()).simplified();
}

AADLConnectionGraphicsItem::AADLConnectionGraphicsItem(AADLObjectConnection *connection, QGraphicsItem *parentItem)
    : QGraphicsObject(parentItem)
    , m_item(new GraphicsPathItem(this))
    , m_connection(connection)
{
    setObjectName(QLatin1String("AADLConnectionGraphicsItem"));
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemHasNoContents | QGraphicsItem::ItemClipsToShape
             | QGraphicsItem::ItemContainsChildrenInShape);
    m_item->setPen(QPen(Qt::black, kDefaulPenWidth));
}

AADLConnectionGraphicsItem::~AADLConnectionGraphicsItem()
{
    clear();
}

void AADLConnectionGraphicsItem::setEndPoints(AADLInterfaceGraphicsItem *pi, AADLInterfaceGraphicsItem *ri)
{
    updateProvidedInterface(pi);
    updateRequiredInterface(ri);
    rebuildLayout();
}

void AADLConnectionGraphicsItem::scheduleLayoutUpdate()
{
    if (m_layoutDirty)
        return;

    m_layoutDirty = true;
    QMetaObject::invokeMethod(this, "instantLayoutUpdate", Qt::QueuedConnection);
}

void AADLConnectionGraphicsItem::instantLayoutUpdate()
{
    rebuildLayout();
    m_layoutDirty = false;

    update();
}

void AADLConnectionGraphicsItem::setPoints(const QVector<QPointF> &points)
{
    Q_ASSERT(points.size() >= 2);
    if (points.isEmpty()) {
        clear();
        return;
    }

    m_points = points;

    auto startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
            utils::nearestItem(scene(), points.first(), QList<int> { AADLInterfaceGraphicsItem::Type }));
    if (startItem)
        updateProvidedInterface(startItem);

    auto endItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
            utils::nearestItem(scene(), points.last(), QList<int> { AADLInterfaceGraphicsItem::Type }));
    if (endItem)
        updateRequiredInterface(endItem);

    updateBoundingRect();
}

QVector<QPointF> AADLConnectionGraphicsItem::points() const
{
    return m_points;
}

AADLObjectConnection *AADLConnectionGraphicsItem::entity() const
{
    return m_connection;
}

QPainterPath AADLConnectionGraphicsItem::shape() const
{
    return m_item->shape();
}

QRectF AADLConnectionGraphicsItem::boundingRect() const
{
    return m_item->boundingRect();
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
    m_points = updatePoints(scene(), m_startItem, m_endItem);
    updateBoundingRect();
}

void AADLConnectionGraphicsItem::updateBoundingRect()
{
    prepareGeometryChange();

    QPainterPath pp;
    pp.addPolygon(QPolygonF(m_points));
    m_item->setPath(pp);
    m_boundingRect = pp.boundingRect();
    updateGripPoints();
}

void AADLConnectionGraphicsItem::createCommand()
{
    auto prepareParams = [](AADLInterfaceGraphicsItem *item) -> QVariantList {
        return { qVariantFromValue(item->entity()), qVariantFromValue<QVector<QPointF>>({ item->scenePos() }) };
    };

    taste3::cmd::CommandsStack::current()->beginMacro(tr("Change connection"));

    const QVariantList params = { qVariantFromValue(m_connection), qVariantFromValue(m_points) };
    taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::ChangeEntityGeometry, params));

    const auto ifaceStartCmd = cmd::CommandsFactory::create(cmd::ChangeEntityGeometry, prepareParams(m_startItem));
    taste3::cmd::CommandsStack::current()->push(ifaceStartCmd);

    const auto ifaceEndCmd = cmd::CommandsFactory::create(cmd::ChangeEntityGeometry, prepareParams(m_endItem));
    taste3::cmd::CommandsStack::current()->push(ifaceEndCmd);

    taste3::cmd::CommandsStack::current()->endMacro();
};

void AADLConnectionGraphicsItem::updateGripPoints(bool forceVisible)
{
    adjustGripPointCount();
    if (m_points.isEmpty())
        return;

    const QTransform tr = scene()->views().isEmpty() ? QTransform() : scene()->views().front()->viewportTransform();
    const QTransform dt = deviceTransform(tr);
    const QPointF currScale { dt.m11(), dt.m22() };

    for (int idx = 0; idx < m_points.size(); ++idx) {
        const QPointF point = m_points.value(idx);
        if (QGraphicsRectItem *grip = m_grips.value(idx)) {
            const QPointF destination { mapFromScene(point) };
            const QPointF destinationScaled { destination.x() * currScale.x(), destination.y() * currScale.y() };
            QRectF br = grip->rect();
            br.moveCenter(destinationScaled);
            grip->setRect(br);
            grip->setVisible(forceVisible || isSelected());
        }
    }
}

void AADLConnectionGraphicsItem::updateProvidedInterface(AADLInterfaceGraphicsItem *pi)
{
    if (pi != m_startItem) {
        if (m_startItem)
            m_startItem->disconnect(this);

        m_startItem = pi;
        if (pi)
            m_startItem->connect(this);
    }
}

void AADLConnectionGraphicsItem::updateRequiredInterface(AADLInterfaceGraphicsItem *ri)
{
    if (ri != m_endItem) {
        if (m_endItem)
            m_endItem->disconnect(this);

        m_endItem = ri;
        if (ri)
            m_endItem->connect(this);
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
    int idx = m_grips.indexOf(handle);
    if (idx == -1)
        return false;

    const QVector<QPointF> points(m_points);

    m_points[idx] = event->scenePos();

    auto it = std::find_if(m_grips.begin(), m_grips.end(), [handle](const QGraphicsRectItem *const grip) {
        return grip != handle && grip->sceneBoundingRect().intersects(handle->sceneBoundingRect());
    });
    if (it != m_grips.end()) {
        const int intersectionIdx = std::distance(m_grips.begin(), it);
        const int startIdx = intersectionIdx < idx ? intersectionIdx : idx + 1;
        const int endIdx = intersectionIdx < idx ? idx : intersectionIdx + 1;
        for (int i = endIdx - 1; i >= startIdx; --i) {
            auto item = m_grips.takeAt(i);
            scene()->removeItem(item);
            delete item;
            m_points.removeAt(i);
        }
        idx = m_grips.indexOf(handle);
    }
    if (idx == 0 || idx == m_points.size() - 1) {
        auto updateEdgeItem = [&](InteractiveObject *item, bool isStart) {
            QSignalBlocker sb(item);
            item->setPos(item->parentItem()->mapFromScene(isStart ? m_points.first() : m_points.last()));
            item->instantLayoutUpdate();
            const QRectF itemSceneRect = item->sceneBoundingRect();
            if (itemSceneRect.contains(isStart ? m_points.first() : m_points.last()))
                return;

            m_points.insert(isStart ? 0 : m_points.size(), item->scenePos());
            m_grips.insert(isStart ? 0 : m_points.size(), createGripPoint());
        };
        updateEdgeItem(m_startItem, true);
        updateEdgeItem(m_endItem, false);
    }

    updateBoundingRect();
    return true;
}

bool AADLConnectionGraphicsItem::handleGripPointRelease(QGraphicsRectItem *handle, QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    const int idx = m_grips.indexOf(handle);
    if (idx == -1)
        return false;

    QPointF intersectionPoint { event->scenePos() };
    if (idx > 0 && idx < m_points.size() - 1) {
        QLineF prevLine = { m_points.value(idx - 1), intersectionPoint };
        if (utils::alignedLine(prevLine))
            intersectionPoint = prevLine.p2();

        QLineF nextLine = { m_points.value(idx + 1), intersectionPoint };
        if (utils::alignedLine(nextLine))
            intersectionPoint = nextLine.p2();
    }
    m_points[idx] = intersectionPoint;

    static const QList<int> types = { AADLFunctionGraphicsItem::Type, AADLFunctionTypeGraphicsItem::Type };
    for (auto item : scene()->items(m_points)) {
        if (types.contains(item->type())
            && utils::intersectionPoints(item->sceneBoundingRect(), QPolygonF(m_points)).size() > 1) {
            m_startItem->setPos(m_startItem->parentItem()->mapFromScene(m_tmpPoints.first()));
            m_endItem->setPos(m_endItem->parentItem()->mapFromScene(m_tmpPoints.last()));
            setPoints(m_tmpPoints);
            m_tmpPoints.clear();
            adjustGripPointCount();
            return true;
        }
    }

    m_tmpPoints.clear();
    adjustGripPointCount();
    updateBoundingRect();

    createCommand();
    return true;
}

void AADLConnectionGraphicsItem::adjustGripPointCount()
{
    const int diff = m_points.isEmpty() ? 0 : m_points.size() - m_grips.size();
    if (diff == 0)
        return;

    while (m_grips.size() > m_points.size()) {
        if (auto grip = m_grips.takeLast()) {
            if (auto scene = grip->scene())
                scene->removeItem(grip);
            delete grip;
        }
    }
    while (m_grips.size() < m_points.size())
        m_grips.append(createGripPoint());
}

QGraphicsRectItem *AADLConnectionGraphicsItem::createGripPoint()
{
    QGraphicsRectItem *gripPoint =
            scene()->addRect(kGripPointRect, QPen(kGripPointBorder), QBrush(kGripPointBackground));
    gripPoint->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIgnoresTransformations);
    gripPoint->installSceneEventFilter(this);
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
    if (simplifiedPoints.size() == 2)
        return { points.first(), points.last() };
    return simplifiedPoints;
}

void AADLConnectionGraphicsItem::clear()
{
    qDeleteAll(m_grips);
    m_grips.clear();
    m_points.clear();
}

} // namespace aadl
} // namespace taste3
