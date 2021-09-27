/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "veconnectiongraphicsitem.h"

#include "graphicsviewutils.h"
#include "grippointshandler.h"
#include "veconnectionendpointgraphicsitem.h"
#include "veobject.h"
#include "verectgraphicsitem.h"

#include <QApplication>
#include <QtDebug>

static const qreal kSelectionOffset = 10;

namespace shared {
namespace ui {

//! Get list of graphics items intersected with \a connection
static inline QList<QGraphicsItem *> intersectedItems(VEConnectionGraphicsItem *connection)
{
    const QVector<QPointF> &points = connection->points();
    QList<QGraphicsItem *> intersectedGraphicsItems = connection->scene()->items(points);
    auto endIt = std::remove_if(intersectedGraphicsItems.begin(), intersectedGraphicsItems.end(),
            [connection, points](QGraphicsItem *item) {
                if (qobject_cast<VERectGraphicsItem *>(item->toGraphicsObject()) == nullptr) {
                    return true;
                }

                const auto intersectionPoints =
                        graphicsviewutils::intersectionPoints(item->sceneBoundingRect(), points);
                return intersectionPoints.size() <= 1;
            });
    intersectedGraphicsItems.erase(endIt, intersectedGraphicsItems.end());
    return intersectedGraphicsItems;
}

/*
 * Generates a path for existing \a connection
 */
static inline QVector<QPointF> generateConnectionPath(VEConnectionGraphicsItem *connection)
{
    if (!connection || !connection->scene())
        return {};

    const VEConnectionEndPointGraphicsItem *startItem = connection->startItem();
    const VEConnectionEndPointGraphicsItem *endItem = connection->endItem();
    if (!startItem || !endItem)
        return {};

    const QGraphicsScene *scene = connection->scene();
    Q_ASSERT(startItem->scene() == endItem->scene() && scene);

    const bool isStartEndpointNested = startItem->targetItem()->isAncestorOf(endItem);
    const bool isEndEndpointNested = endItem->targetItem()->isAncestorOf(startItem);

    return graphicsviewutils::createConnectionPath(graphicsviewutils::siblingItemsRects(connection),
            startItem->connectionEndPoint(isStartEndpointNested), startItem->targetItem()->sceneBoundingRect(),
            endItem->connectionEndPoint(isEndEndpointNested), endItem->targetItem()->sceneBoundingRect());
}

//! Get the connection path with replaced \a connection segments intersected with \a cachedIntersectedItems
static inline QVector<QPointF> replaceIntersectedSegments(
        const QList<QGraphicsItem *> &cachedIntersectedItems, VEConnectionGraphicsItem *connection)
{
    QVector<QPointF> points = connection->points();
    QList<QPair<QPointF, QPointF>> sections;
    QPointF point;
    for (int idx = 1; idx < points.size(); ++idx) {
        if (points.at(idx - 1) == points.at(idx)) {
            continue;
        }
        for (int cacheIdx = 0; cacheIdx < cachedIntersectedItems.size(); ++cacheIdx) {
            const auto intersectionPoints = shared::graphicsviewutils::intersectionPoints(
                    cachedIntersectedItems.value(cacheIdx)->sceneBoundingRect(),
                    QVector<QPointF> { points.value(idx - 1), points.value(idx) });
            const int properIntersectionPointsCount = std::count_if(
                    intersectionPoints.cbegin(), intersectionPoints.cend(), [&points](const QPointF &intesectionPoint) {
                        return points.first() != intesectionPoint && points.last() != intesectionPoint;
                    });
            if (properIntersectionPointsCount == 0) {
                /// Current segment doesn't intersect Function(Type) item
                continue;
            }
            Q_ASSERT(properIntersectionPointsCount <= 2);
            if (!point.isNull()) {
                /// Merging sequential sections
                if (!sections.isEmpty() && sections.last().second == point) {
                    sections.last().second = points.value(idx);
                } else {
                    sections.append(qMakePair(point, points.value(idx)));
                }
                point = QPointF();
            } else if (properIntersectionPointsCount == 2) {
                sections.append(qMakePair(points.value(idx - 1), points.value(idx)));
                /// Checking other intersected items could be overlapped by current segment
                continue;
            } else {
                point = points.value(idx - 1);
            }
            break;
        }
    }
    sections.erase(std::unique(sections.begin(), sections.end()), sections.end());
    const QList<QRectF> existingRects = shared::graphicsviewutils::siblingItemsRects(connection);
    for (auto chunk : sections) {
        const QVector<QPointF> subPath = shared::graphicsviewutils::path(existingRects, chunk.first, chunk.second);
        if (!points.isEmpty()) {
            /// Remove overlapped chunk
            const int idxStart = points.indexOf(chunk.first);
            const int idxEnd = points.indexOf(chunk.second);
            points.remove(idxStart, idxEnd - idxStart);

            /// Insert new subpath instead of removed one
            for (int idx = 0; idx < subPath.size(); ++idx) {
                points.insert(idxStart + idx, subPath.value(idx));
            }
        } else {
            /// Normally shouldn't be here, but if there is an error during subpath finding
            /// stop update and recreate the whole path
            points = generateConnectionPath(connection);
            break;
        }
    }
    return shared::graphicsviewutils::simplifyPoints(points);
}

VEConnectionGraphicsItem::GraphicsPathItem::GraphicsPathItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
}

QPainterPath VEConnectionGraphicsItem::GraphicsPathItem::shape() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(kSelectionOffset);
    stroker.setJoinStyle(Qt::MiterJoin);

    return stroker.createStroke(path()).simplified();
}

VEConnectionGraphicsItem::VEConnectionGraphicsItem(VEObject *entity, VEConnectionEndPointGraphicsItem *startItem,
        VEConnectionEndPointGraphicsItem *endItem, QGraphicsItem *parent)
    : VEInteractiveObject(entity, parent)
    , m_startItem(startItem)
    , m_endItem(endItem)
    , m_item(new GraphicsPathItem(this))
{
}

VEConnectionGraphicsItem::~VEConnectionGraphicsItem()
{
    m_points.clear();

    updateInterfaceConnectionsReference(ConnectionEndPointReference::Remove);
}

void VEConnectionGraphicsItem::setPoints(const QVector<QPointF> &points)
{
    if (points.isEmpty()) {
        if (m_startItem && m_endItem)
            instantLayoutUpdate();
        else
            m_points.clear();
        return;
    }

    if (!graphicsviewutils::comparePolygones(m_points, points)) {
        m_points = points;
        instantLayoutUpdate();
    }
}

QVector<QPointF> VEConnectionGraphicsItem::points() const
{
    return m_points;
}

QVector<QPointF> VEConnectionGraphicsItem::graphicsPoints() const
{
    QPolygonF polygon = m_item->path().toFillPolygon();
    if (polygon.isClosed())
        polygon.removeLast();
    return mapToScene(polygon);
}

void VEConnectionGraphicsItem::updateFromEntity()
{
    Q_ASSERT(entity());
    if (!entity())
        return;

    setPoints(graphicsviewutils::polygon(entity()->coordinates()));
}

void VEConnectionGraphicsItem::init()
{
    VEInteractiveObject::init();
    updateInterfaceConnectionsReference(ConnectionEndPointReference::Set);
}

QPainterPath VEConnectionGraphicsItem::shape() const
{
    return m_item->shape();
}

QRectF VEConnectionGraphicsItem::boundingRect() const
{
    return m_item->boundingRect();
}

void VEConnectionGraphicsItem::initGripPoints()
{
    VEInteractiveObject::initGripPoints();
    for (int idx = 0; idx < m_points.size(); ++idx)
        gripPointsHandler()->createGripPoint(GripPoint::Absolute);
}

void VEConnectionGraphicsItem::updateGripPoints()
{
    if (gripPointsHandler() == nullptr)
        return;

    const QVector<QPointF> points = graphicsPoints();
    if (points.isEmpty()) {
        return;
    }

    auto grips = gripPointsHandler()->gripPoints();
    for (int idx = 0; idx < qMax(points.size(), grips.size()); ++idx) {
        if (idx >= points.size()) {
            gripPointsHandler()->removeGripPoint(grips.value(idx));
            continue;
        }
        if (idx >= grips.size()) {
            gripPointsHandler()->createGripPoint(GripPoint::Absolute);
        }
        if (auto grip = grips.value(idx)) {
            gripPointsHandler()->setGripPointPos(grip, points.value(idx));
        }
    }
    VEInteractiveObject::updateGripPoints();
}

VEConnectionEndPointGraphicsItem *VEConnectionGraphicsItem::endItem() const
{
    return m_endItem;
}

VEConnectionEndPointGraphicsItem *VEConnectionGraphicsItem::startItem() const
{
    return m_startItem;
}

VERectGraphicsItem *VEConnectionGraphicsItem::sourceItem() const
{
    return m_startItem && m_startItem->targetItem()
            ? qobject_cast<VERectGraphicsItem *>(m_startItem->targetItem()->toGraphicsObject())
            : nullptr;
}

VERectGraphicsItem *VEConnectionGraphicsItem::targetItem() const
{
    return m_endItem && m_endItem->targetItem()
            ? qobject_cast<VERectGraphicsItem *>(m_endItem->targetItem()->toGraphicsObject())
            : nullptr;
}

QList<QPair<VEObject *, QVector<QPointF>>> VEConnectionGraphicsItem::prepareChangeCoordinatesCommandParams() const
{
    if (!entity() || !m_startItem || !m_startItem->entity() || !m_endItem || !m_endItem->entity()) {
        return {};
    }

    auto prepareParams = [](VEConnectionEndPointGraphicsItem *item) -> QPair<VEObject *, QVector<QPointF>> {
        QVector<QPointF> pos;
        pos.append(item->scenePos());
        return { item->entity(), pos };
    };

    QList<QPair<VEObject *, QVector<QPointF>>> params;
    params.append({ entity(), graphicsPoints() });
    params.append(prepareParams(m_startItem));
    params.append(prepareParams(m_endItem));
    return params;
}

/*!
   Re-generates the connection path
   Returns true, if the points did change
 */
bool VEConnectionGraphicsItem::layout()
{
    if (!m_startItem || !m_startItem->isVisible() || !m_endItem || !m_endItem->isVisible()) {
        setVisible(false);
        return false;
    }

    const QVector<QPointF> oldPoints = m_points;
    m_points = generateConnectionPath(this);
    updateBoundingRect();
    return !shared::graphicsviewutils::comparePolygones(oldPoints, m_points);
}

bool VEConnectionGraphicsItem::replaceInterface(
        VEConnectionEndPointGraphicsItem *ifaceToBeReplaced, VEConnectionEndPointGraphicsItem *newIface)
{
    if (m_startItem == ifaceToBeReplaced) {
        Q_ASSERT(m_startItem->parentItem() == newIface->parentItem());
        m_startItem->removeConnection(this);
        m_startItem = newIface;
    } else if (m_endItem == ifaceToBeReplaced) {
        Q_ASSERT(m_endItem->parentItem() == newIface->parentItem());
        m_endItem->removeConnection(this);
        m_endItem = newIface;
    } else {
        return false;
    }

    newIface->addConnection(this);
    transformToEndPoint(newIface);
    return true;
}

//! Updates all connections linked to \a ifaceItem and layout/rebuild them
//! according to /a layoutPolicy and /a collisionsPolicy including \a includingNested if it's set to true
void VEConnectionGraphicsItem::layoutInterfaceConnections(VEConnectionEndPointGraphicsItem *ifaceItem,
        LayoutPolicy layoutPolicy, CollisionsPolicy collisionsPolicy, bool includingNested)
{
    for (VEConnectionGraphicsItem *connection : ifaceItem->connectionItems()) {
        Q_ASSERT(connection && connection->startItem() && connection->endItem());
        if (!connection) {
            continue;
        }
        if (includingNested || connection->parentItem() != ifaceItem->parentItem()) {
            connection->layoutConnection(ifaceItem, layoutPolicy, collisionsPolicy);
        }
    }
}

/*!
 * \brief VEConnectionGraphicsItem::layoutConnection changes path of connection
 * \param ifaceItem Affected interface, path to which should be updated
 * \param layoutPolicy \a VEConnectionGraphicsItem::LayoutPolicy policy for connection layouting
 * \param collisionsPolicy \a VEConnectionGraphicsItem::CollisionsPolicy policy for connection
 * rebuilding when it has collisions with other boxes
 */
void VEConnectionGraphicsItem::layoutConnection(
        VEConnectionEndPointGraphicsItem *ifaceItem, LayoutPolicy layoutPolicy, CollisionsPolicy collisionsPolicy)
{
    if (layoutPolicy == LayoutPolicy::Default) {
        layout();
        return;
    } else if (layoutPolicy == LayoutPolicy::LastSegment) {
        updateEndPoint(ifaceItem);
    } else if (layoutPolicy == LayoutPolicy::Scaling) {
        transformToEndPoint(ifaceItem);
    }

    if (CollisionsPolicy::Ignore == collisionsPolicy) {
        return;
    } else if (CollisionsPolicy::PartialRebuild == collisionsPolicy) {
        updateOverlappedSections();
    } else if (CollisionsPolicy::Rebuild == collisionsPolicy) {
        const QList<QGraphicsItem *> overlappedItems = intersectedItems(this);
        if (!overlappedItems.isEmpty()) {
            layout();
            return;
        }
    }
}

//! Replaces intersected segments of connection path with newly generated subpaths if any
void VEConnectionGraphicsItem::updateOverlappedSections()
{
    if (m_points.isEmpty()) {
        return;
    }

    /// Cache intersected items to avoid grabbing intersections of all items from scene
    /// during checking all connection segments
    const QList<QGraphicsItem *> cachedIntersectedItems = intersectedItems(this);
    /// Nothing to update without intersections with Function(Type) items
    if (cachedIntersectedItems.isEmpty()) {
        m_points = shared::graphicsviewutils::simplifyPoints(m_points);
    } else {
        m_points = replaceIntersectedSegments(cachedIntersectedItems, this);
    }
    updateBoundingRect();
}

void VEConnectionGraphicsItem::onManualMoveStart(GripPoint *gp, const QPointF &at)
{
    if (gripPointsHandler() == nullptr)
        return;

    const auto &grips = gripPointsHandler()->gripPoints();
    const int idx = grips.indexOf(gp ? gp : gripPointByPos(grips, at));
    if (idx == -1)
        return;

    if (qApp->keyboardModifiers().testFlag(Qt::ControlModifier)) {
        m_points.insert(idx, at);
        auto grip = gripPointsHandler()->createGripPoint(gp->location(), idx);
        gripPointsHandler()->setGripPointPos(grip, at);
        grip->stackBefore(gp);
        updateBoundingRect();
    }
}

void VEConnectionGraphicsItem::onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gripPointsHandler() == nullptr)
        return;

    const auto &grips = gripPointsHandler()->gripPoints();
    int idx = grips.indexOf(gp ? gp : gripPointByPos(grips, from));
    if (idx == -1)
        return;

    m_points[idx] = to;

    if (!qApp->keyboardModifiers().testFlag(Qt::ControlModifier)) {
        if (removeCollidedGrips(gp)) {
            idx = grips.indexOf(gp);
            if (idx == -1)
                return;
        }

        auto updateEndPointItem = [&](VEConnectionEndPointGraphicsItem *endPointItem) {
            endPointItem->setPos(endPointItem->parentItem()->mapFromScene(to));
            layoutInterfaceConnections(endPointItem, LayoutPolicy::LastSegment, CollisionsPolicy::Ignore, false);
        };
        if (idx == 0)
            updateEndPointItem(m_startItem);
        else if (idx == grips.size() - 1)
            updateEndPointItem(m_endItem);
    }

    updateBoundingRect();
}

void VEConnectionGraphicsItem::onManualMoveFinish(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (gripPointsHandler() == nullptr)
        return;

    const auto &grips = gripPointsHandler()->gripPoints();
    const auto grip = gp ? gp : gripPointByPos(grips, releasedAt);
    if (pressedAt == releasedAt || !grip)
        return;

    const int idx = grips.indexOf(grip);
    if (idx == -1)
        return;

    QPointF intersectionPoint { releasedAt };
    if (idx > 0 && idx < m_points.size() - 1) {
        QLineF prevLine = { m_points.value(idx - 1), intersectionPoint };
        if (shared::graphicsviewutils::alignedLine(prevLine))
            intersectionPoint = prevLine.p2();

        QLineF nextLine = { m_points.value(idx + 1), intersectionPoint };
        if (shared::graphicsviewutils::alignedLine(nextLine))
            intersectionPoint = nextLine.p2();
    }
    m_points[idx] = intersectionPoint;

    auto updateEndPointItem = [](VEConnectionEndPointGraphicsItem *endPointItem) {
        if (endPointItem) {
            endPointItem->instantLayoutUpdate();
            layoutInterfaceConnections(endPointItem, LayoutPolicy::Default, CollisionsPolicy::Rebuild, true);
        }
    };

    if (idx == 0) {
        updateEndPointItem(m_startItem);
    } else if (idx == grips.size() - 1) {
        updateEndPointItem(m_endItem);
    } else {
        updateOverlappedSections();
    }

    updateBoundingRect();
    updateEntity();
}

void VEConnectionGraphicsItem::updateBoundingRect()
{
    prepareGeometryChange();

    QPainterPath pp;
    pp.addPolygon(mapFromScene(QPolygonF(m_points)));
    m_item->setPath(pp);
    setBoundingRect(pp.boundingRect());
    updateGripPoints();
}

void VEConnectionGraphicsItem::rebuildLayout()
{
    if (!m_startItem || !m_endItem || !entity()) {
        setVisible(false);
        return;
    }

    if (m_startItem->entity()->coordinates().isEmpty())
        m_startItem->layout();
    if (m_endItem->entity()->coordinates().isEmpty())
        m_endItem->layout();

    bool pathObsolete(true);
    if (m_points.size() > 2) {
        pathObsolete = (!startItem() || !startItem()->ifaceShape().boundingRect().contains(m_points.first()))
                || (!endItem() || !endItem()->ifaceShape().boundingRect().contains(m_points.last()));
    }

    if (pathObsolete) {
        const bool changed = layout();
        if (changed) {
            mergeGeometry();
        }
        return;
    }

    updateBoundingRect();
    setVisible(true);
}

void VEConnectionGraphicsItem::onSelectionChanged(bool isSelected)
{
    const shared::ColorHandler h = colorHandler();
    QPen pen = h.pen();
    pen.setWidthF(isSelected ? 1.5 * pen.widthF() : pen.widthF());
    pen.setStyle(isSelected ? Qt::DotLine : Qt::SolidLine);
    m_item->setPen(pen);

    shared::ui::VEInteractiveObject::onSelectionChanged(isSelected);
}

bool VEConnectionGraphicsItem::removeCollidedGrips(GripPoint *gp)
{
    auto grips = gripPointsHandler()->gripPoints();
    const int idx = grips.indexOf(gp);
    if (idx == -1)
        return false;

    auto it = std::find_if(grips.cbegin(), grips.cend(), [gp](const GripPoint *const grip) {
        return grip != gp && grip->sceneBoundingRect().intersects(gp->sceneBoundingRect());
    });
    if (it != grips.cend()) {
        const int intersectionIdx = std::distance(grips.cbegin(), it);
        const int startIdx = intersectionIdx < idx ? intersectionIdx : idx + 1;
        const int endIdx = intersectionIdx < idx ? idx : intersectionIdx + 1;
        for (int i = endIdx - 1; i >= startIdx; --i) {
            auto item = grips.takeAt(i);
            gripPointsHandler()->removeGripPoint(item);
            m_points.removeAt(i);
        }
        return true;
    }
    return false;
}

void VEConnectionGraphicsItem::simplify()
{
    m_points = shared::graphicsviewutils::simplifyPoints(m_points);
}

void VEConnectionGraphicsItem::transformToEndPoint(const VEConnectionEndPointGraphicsItem *endPoint)
{
    if (!endPoint || m_points.size() < 2) {
        return;
    }

    const QPointF endPointPos = endPoint->connectionEndPoint(this);
    if (m_points.front().toPoint() == endPointPos.toPoint() && m_points.last().toPoint() == endPointPos.toPoint()) {
        return;
    }

    if (m_points.size() > 2) {
        QVector<QPointF> initialPoints = graphicsviewutils::polygon(entity()->coordinates());
        const QRectF currentRect = QRectF(initialPoints.first(), initialPoints.last());
        QRectF newRect;
        if (endPoint == startItem()) {
            newRect = QRectF(endPointPos, m_points.last());
        } else if (endPoint == endItem()) {
            newRect = QRectF(m_points.first(), endPointPos);
        } else {
            qWarning() << "Attempt to update from an unknown interface";
            return;
        }

        const qreal xScale = newRect.width() / currentRect.width();
        const qreal yScale = newRect.height() / currentRect.height();
        if (qFuzzyCompare(xScale, 1.0) && qFuzzyCompare(yScale, 1.0)) {
            return;
        }
        for (auto it = std::next(initialPoints.begin()); it != std::prev(initialPoints.end()); ++it) {
            qreal x = 0, y = 0;
            if (it->x() <= currentRect.left() && it->x() < currentRect.right()) {
                x = it->x() - currentRect.left() + newRect.left();
            } else if (it->x() >= currentRect.right() && it->x() > currentRect.left()) {
                x = it->x() - currentRect.right() + newRect.right();
            } else {
                x = (it->x() - currentRect.left()) * xScale + newRect.left();
            }

            if (it->y() <= currentRect.top() && it->y() < currentRect.bottom()) {
                y = it->y() - currentRect.top() + newRect.top();
            } else if (it->y() >= currentRect.bottom() && it->y() > currentRect.top()) {
                y = it->y() - currentRect.bottom() + newRect.bottom();
            } else {
                y = (it->y() - currentRect.top()) * yScale + newRect.top();
            }

            *it = { x, y };
        }
        m_points = initialPoints;
    }

    updateEndPoint(endPoint);
}

void VEConnectionGraphicsItem::updateEndPoint(const VEConnectionEndPointGraphicsItem *endPoint)
{
    if (!endPoint || m_points.size() < 2)
        return;

    if (endPoint == startItem()) {
        m_points.first() = endPoint->connectionEndPoint(this);
        m_points.last() = endItem()->connectionEndPoint(this);
    } else if (endPoint == endItem()) {
        m_points.first() = startItem()->connectionEndPoint(this);
        m_points.last() = endPoint->connectionEndPoint(this);
    } else {
        qWarning() << "Attempt to update from an unknown interface";
        return;
    }

    updateBoundingRect();
}

void VEConnectionGraphicsItem::updateInterfaceConnectionsReference(ConnectionEndPointReference action)
{
    for (VEConnectionEndPointGraphicsItem *endPoint : { startItem(), endItem() }) {
        if (endPoint) {
            if (action == ConnectionEndPointReference::Remove)
                endPoint->removeConnection(this);
            else
                endPoint->addConnection(this);
        }
    }
}

GripPoint *VEConnectionGraphicsItem::gripPointByPos(const QList<GripPoint *> &grips, const QPointF &pos) const
{
    if (grips.isEmpty()) {
        return nullptr;
    }

    const auto found = std::find_if(grips.cbegin(), grips.cend(),
            [&pos](const GripPoint *const grip) { return grip && grip->sceneBoundingRect().contains(pos); });

    if (found != grips.end()) {
        return *found;
    }

    return nullptr;
}

} // namespace ui
} // namespace shared
