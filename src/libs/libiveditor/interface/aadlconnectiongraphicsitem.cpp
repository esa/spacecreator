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

#include "aadlcommentgraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlfunctiontypegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "aadlnamevalidator.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "baseitems/common/aadlutils.h"
#include "colors/colormanager.h"
#include "commands/cmdentitygeometrychange.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "commandsstack.h"
#include "graphicsviewutils.h"
#include "ui/grippointshandler.h"

#include <QGuiApplication>
#include <QPainter>
#include <QPen>
#include <QtDebug>
#include <QtMath>

static const qreal kSelectionOffset = 10;

namespace aadlinterface {

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

AADLConnectionGraphicsItem::AADLConnectionGraphicsItem(aadl::AADLObjectConnection *connection,
        AADLInterfaceGraphicsItem *startIface, AADLInterfaceGraphicsItem *endIface, QGraphicsItem *parentItem)
    : InteractiveObject(connection, parentItem)
    , m_startItem(startIface)
    , m_endItem(endIface)
    , m_item(new GraphicsPathItem(this))
    , m_points()
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemHasNoContents | QGraphicsItem::ItemClipsToShape);
    setZValue(ZOrder.Connection);
}

AADLConnectionGraphicsItem::~AADLConnectionGraphicsItem()
{
    m_points.clear();

    updateInterfaceConnectionsReference(IfaceConnectionReference::Remove);
}

void AADLConnectionGraphicsItem::updateInterfaceConnectionsReference(IfaceConnectionReference action)
{
    for (AADLInterfaceGraphicsItem *iface : { startItem(), endItem() }) {
        if (iface) {
            if (action == IfaceConnectionReference::Remove)
                iface->removeConnection(this);
            else
                iface->addConnection(this);
        }
    }
}

void AADLConnectionGraphicsItem::updateFromEntity()
{
    aadl::AADLObjectConnection *obj = entity();
    Q_ASSERT(obj);
    if (!obj)
        return;

    setPoints(polygon(obj->coordinates()));
}

void AADLConnectionGraphicsItem::init()
{
    InteractiveObject::init();
    updateInterfaceConnectionsReference(IfaceConnectionReference::Set);
}

void AADLConnectionGraphicsItem::setPoints(const QVector<QPointF> &points)
{
    if (points.isEmpty()) {
        if (m_startItem && m_endItem)
            instantLayoutUpdate();
        else
            m_points.clear();
        return;
    }

    if (!comparePolygones(m_points, points)) {
        m_points = points;
        instantLayoutUpdate();
    }
}

QVector<QPointF> AADLConnectionGraphicsItem::points() const
{
    return m_points;
}

QVector<QPointF> AADLConnectionGraphicsItem::graphicsPoints() const
{
    QPolygonF polygon = m_item->path().toFillPolygon();
    if (polygon.isClosed())
        polygon.removeLast();
    return mapToScene(polygon);
}

aadl::AADLObjectConnection *AADLConnectionGraphicsItem::entity() const
{
    return qobject_cast<aadl::AADLObjectConnection *>(aadlObject());
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
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void AADLConnectionGraphicsItem::rebuildLayout()
{
    if (!m_startItem || !m_startItem->isVisible() || !m_endItem || !m_endItem->isVisible() || !m_dataObject
            || !m_dataObject->isVisible()) {
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
        layout();
        mergeGeometry();
        return;
    }

    updateBoundingRect();
    setVisible(true);
}

void AADLConnectionGraphicsItem::updateEdgePoint(const AADLInterfaceGraphicsItem *iface)
{
    if (!iface || m_points.size() < 2)
        return;

    if (iface == startItem()) {
        m_points.first() = iface->connectionEndPoint(this);
        m_points.last() = endItem()->connectionEndPoint(this);
    } else if (iface == endItem()) {
        m_points.first() = startItem()->connectionEndPoint(this);
        m_points.last() = iface->connectionEndPoint(this);
    } else {
        qWarning() << "Attempt to update from an unknown interface";
        return;
    }

    updateBoundingRect();
}

void AADLConnectionGraphicsItem::layout()
{
    if (!m_startItem || !m_startItem->isVisible() || !m_endItem || !m_endItem->isVisible()) {
        setVisible(false);
        return;
    }

    m_points = connectionPath(m_startItem, m_endItem);
    updateBoundingRect();
}

bool AADLConnectionGraphicsItem::replaceInterface(
        AADLInterfaceGraphicsItem *ifaceToBeReplaced, AADLInterfaceGraphicsItem *newIface)
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
    updateEndPoint(newIface);
    return true;
}

//! Get list of graphics items intersected with \a connection
static inline QList<QGraphicsItem *> intersectedItems(AADLConnectionGraphicsItem *connection,
        const QList<int> &types = { AADLFunctionGraphicsItem::Type, AADLFunctionTypeGraphicsItem::Type })
{
    const QVector<QPointF> &points = connection->points();
    QList<QGraphicsItem *> intersectedGraphicsItems = connection->scene()->items(points);
    auto endIt = std::remove_if(intersectedGraphicsItems.begin(), intersectedGraphicsItems.end(),
            [connection, points, types](QGraphicsItem *item) {
                if (!types.contains(item->type())) {
                    return true;
                }

                if (item->parentItem() != connection->parentItem()) {
                    return true;
                }

                const auto intersectionPoints =
                        shared::graphicsviewutils::intersectionPoints(item->sceneBoundingRect(), points);
                return intersectionPoints.size() <= 1;
            });
    intersectedGraphicsItems.erase(endIt, intersectedGraphicsItems.end());
    return intersectedGraphicsItems;
}

//! Get the connection path with replaced \a connection segments intersected with \a cachedIntersectedItems
static inline QVector<QPointF> replaceIntersectedSegments(
        const QList<QGraphicsItem *> &cachedIntersectedItems, AADLConnectionGraphicsItem *connection)
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
    for (auto chunk : sections) {
        const QVector<QPointF> subPath = path(connection->scene(), chunk.first, chunk.second);
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
            points = AADLConnectionGraphicsItem::connectionPath(connection->startItem(), connection->endItem());
            break;
        }
    }
    return simplifyPoints(points);
}

//! Replaces intersected segments of connection path with newly generated subpaths if any
void AADLConnectionGraphicsItem::updateOverlappedSections()
{
    if (m_points.isEmpty()) {
        return;
    }

    /// Cache intersected items to avoid grabbing intersections of all items from scene
    /// during checking all connection segments
    const QList<QGraphicsItem *> cachedIntersectedItems = intersectedItems(this);
    /// Nothing to update without intersections with Function(Type) items
    if (cachedIntersectedItems.isEmpty()) {
        return;
    }
    m_points = replaceIntersectedSegments(cachedIntersectedItems, this);
    updateBoundingRect();
}

void AADLConnectionGraphicsItem::onSelectionChanged(bool isSelected)
{
    const ColorHandler h = colorHandler();
    QPen pen = h.pen();
    pen.setWidthF(isSelected ? 1.5 * pen.widthF() : pen.widthF());
    pen.setStyle(isSelected ? Qt::DotLine : Qt::SolidLine);
    m_item->setPen(pen);

    InteractiveObject::onSelectionChanged(isSelected);
}

void AADLConnectionGraphicsItem::updateBoundingRect()
{
    prepareGeometryChange();

    QPainterPath pp;
    pp.addPolygon(mapFromScene(QPolygonF(m_points)));
    m_item->setPath(pp);
    setBoundingRect(pp.boundingRect());
    updateGripPoints();
}

AADLInterfaceGraphicsItem *AADLConnectionGraphicsItem::endItem() const
{
    return m_endItem;
}

AADLInterfaceGraphicsItem *AADLConnectionGraphicsItem::startItem() const
{
    return m_startItem;
}

AADLFunctionGraphicsItem *AADLConnectionGraphicsItem::sourceItem() const
{
    return m_startItem ? qgraphicsitem_cast<AADLFunctionGraphicsItem *>(m_startItem->targetItem()) : nullptr;
}

AADLFunctionGraphicsItem *AADLConnectionGraphicsItem::targetItem() const
{
    return m_endItem ? qgraphicsitem_cast<AADLFunctionGraphicsItem *>(m_endItem->targetItem()) : nullptr;
}

QList<QVariantList> AADLConnectionGraphicsItem::prepareChangeCoordinatesCommandParams() const
{
    if (!entity() || !m_startItem || !m_startItem->entity() || !m_endItem || !m_endItem->entity()) {
        return {};
    }

    // item->prepareChangeCoordinatesCommandParams() - will be fixed during work on Undo/Redo issues
    auto prepareParams = [](AADLInterfaceGraphicsItem *item) -> QVariantList {
        return { QVariant::fromValue(item->entity()), QVariant::fromValue<QVector<QPointF>>({ item->scenePos() }) };
    };

    QList<QVariantList> params;
    params.append({ QVariant::fromValue(entity()), QVariant::fromValue(graphicsPoints()) });
    params.append(prepareParams(m_startItem));
    params.append(prepareParams(m_endItem));
    return params;
}

shared::ui::GripPoint *AADLConnectionGraphicsItem::gripPointByPos(
        const QList<shared::ui::GripPoint *> &grips, const QPointF &pos) const
{
    if (grips.isEmpty()) {
        return nullptr;
    }

    const auto found = std::find_if(grips.cbegin(), grips.cend(), [&pos](const shared::ui::GripPoint *const grip) {
        return grip && grip->sceneBoundingRect().contains(pos);
    });

    if (found != grips.end()) {
        return *found;
    }

    return nullptr;
}

void AADLConnectionGraphicsItem::onManualMoveStart(shared::ui::GripPoint *gp, const QPointF &at)
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

void AADLConnectionGraphicsItem::updateGripPoints()
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
            gripPointsHandler()->createGripPoint(shared::ui::GripPoint::Absolute);
        }
        if (auto grip = grips.value(idx)) {
            gripPointsHandler()->setGripPointPos(grip, points.value(idx));
        }
    }
    InteractiveObject::updateGripPoints();
}

void AADLConnectionGraphicsItem::onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to)
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

        auto updateEdgeItem = [&](AADLInterfaceGraphicsItem *iface) {
            iface->setPos(iface->parentItem()->mapFromScene(to));
            for (auto connection : iface->connectionItems())
                if (connection != this)
                    connection->updateEdgePoint(iface);
        };
        if (idx == 0)
            updateEdgeItem(m_startItem);
        else if (idx == grips.size() - 1)
            updateEdgeItem(m_endItem);
    }

    updateBoundingRect();
}

void AADLConnectionGraphicsItem::onManualMoveFinish(
        shared::ui::GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
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
        if (alignedLine(prevLine))
            intersectionPoint = prevLine.p2();

        QLineF nextLine = { m_points.value(idx + 1), intersectionPoint };
        if (alignedLine(nextLine))
            intersectionPoint = nextLine.p2();
    }
    m_points[idx] = intersectionPoint;

    auto updateIfaceItem = [](AADLInterfaceGraphicsItem *ifaceItem) {
        if (ifaceItem) {
            ifaceItem->instantLayoutUpdate();
            for (auto connectionItem : ifaceItem->connectionItems()) {
                if (!connectionItem.isNull() && connectionItem->isVisible()) {
                    connectionItem->updateLastChunk(ifaceItem);
                }
            }
        }
    };

    if (idx == 0) {
        updateIfaceItem(m_startItem);
    } else if (idx == grips.size() - 1) {
        updateIfaceItem(m_endItem);
    }

    updateOverlappedSections();
    updateBoundingRect();
    updateEntity();
}

void AADLConnectionGraphicsItem::simplify()
{
    m_points = simplifyPoints(m_points);
}

void AADLConnectionGraphicsItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    for (int idx = 0; idx < m_points.size(); ++idx)
        gripPointsHandler()->createGripPoint(shared::ui::GripPoint::Absolute);
}

ColorManager::HandledColors AADLConnectionGraphicsItem::handledColorType() const
{
    return ColorManager::HandledColors::Connection;
}

void AADLConnectionGraphicsItem::applyColorScheme()
{
    const ColorHandler &h = colorHandler();
    m_item->setPen(h.pen());
    update();
}

bool AADLConnectionGraphicsItem::removeCollidedGrips(shared::ui::GripPoint *gp)
{
    auto grips = gripPointsHandler()->gripPoints();
    const int idx = grips.indexOf(gp);
    if (idx == -1)
        return false;

    auto it = std::find_if(grips.cbegin(), grips.cend(), [gp](const shared::ui::GripPoint *const grip) {
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
};

QString AADLConnectionGraphicsItem::prepareTooltip() const
{
    const QString sourceName =
            aadl::AADLNameValidator::decodeName(aadl::AADLObject::Type::Function, entity()->sourceName());
    const QString sourceInterfaceName = aadl::AADLNameValidator::decodeName(
            aadl::AADLObject::Type::RequiredInterface, entity()->sourceInterfaceName());
    const QString targetName =
            aadl::AADLNameValidator::decodeName(aadl::AADLObject::Type::Function, entity()->targetName());
    const QString targetInterfaceName = aadl::AADLNameValidator::decodeName(
            aadl::AADLObject::Type::ProvidedInterface, entity()->targetInterfaceName());
    const QString sign = entity()->sourceInterface()->isRequired() ? "->" : "<-";
    const QString tooltip =
            QString("%1.%2 %3 %4.%5").arg(sourceName, sourceInterfaceName, sign, targetName, targetInterfaceName);
    return tooltip;
}

void AADLConnectionGraphicsItem::updateEndPoint(const AADLInterfaceGraphicsItem *iface)
{
    if (!iface || m_points.size() < 2) {
        return;
    }

    const QPointF ifaceEndPoint = iface->connectionEndPoint(this);
    if (m_points.front().toPoint() == ifaceEndPoint.toPoint() && m_points.last().toPoint() == ifaceEndPoint.toPoint()) {
        return;
    }

    if (m_points.size() > 2) {
        QVector<QPointF> initialPoints = aadlinterface::polygon(entity()->coordinates());
        const QRectF currentRect = QRectF(initialPoints.first(), initialPoints.last());
        QRectF newRect;
        if (iface == startItem()) {
            newRect = QRectF(ifaceEndPoint, m_points.last());
        } else if (iface == endItem()) {
            newRect = QRectF(m_points.first(), ifaceEndPoint);
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

    if (iface == startItem()) {
        m_points.first() = ifaceEndPoint;
        m_points.last() = endItem()->connectionEndPoint(this);
    } else if (iface == endItem()) {
        m_points.first() = startItem()->connectionEndPoint(this);
        m_points.last() = ifaceEndPoint;
    }

    updateBoundingRect();
}

QVector<QPointF> AADLConnectionGraphicsItem::connectionPath(
        AADLInterfaceGraphicsItem *startItem, AADLInterfaceGraphicsItem *endItem)
{
    if (!startItem || !endItem)
        return {};

    QGraphicsScene *scene = startItem->scene();
    Q_ASSERT(startItem->scene() == endItem->scene() && scene);

    const bool isStartEndpointNested = startItem->targetItem()->isAncestorOf(endItem);
    const bool isEndEndpointNested = endItem->targetItem()->isAncestorOf(startItem);

    return createConnectionPath(scene, startItem->connectionEndPoint(isStartEndpointNested),
            startItem->targetItem()->sceneBoundingRect(), endItem->connectionEndPoint(isEndEndpointNested),
            endItem->targetItem()->sceneBoundingRect());
}

/*!
 * Update last segment of \a AADLConnectionGraphicsItem as argument
 * gets \a AADLInterfaceGraphicsItem on moved/resized \a AADLFunctionGraphicsItem
 */
void AADLConnectionGraphicsItem::updateLastChunk(const AADLInterfaceGraphicsItem *iface)
{
    if (!iface || m_points.size() < 2) {
        return;
    }

    if (iface != startItem() && iface != endItem()) {
        qWarning() << "Attempt to update from an unknown interface";
        return;
    }

    const bool reverse = iface == startItem();
    const auto itemRect = iface->targetItem()->sceneBoundingRect();
    int pos = reverse ? 1 : m_points.size() - 2;
    if (reverse) {
        for (int idx = m_points.size() - 1; idx >= 0; --idx) {
            if (idx - 1 >= 0) {
                const QLineF line = QLineF(m_points.value(idx), m_points.value(idx - 1));
                if (shared::graphicsviewutils::intersects(itemRect, line)) {
                    pos = idx;
                    break;
                }
            }
        }
    } else {
        for (int idx = 0; idx < m_points.size(); ++idx) {
            if (idx + 1 < m_points.size()) {
                const QLineF line = QLineF(m_points.value(idx), m_points.value(idx + 1));
                if (shared::graphicsviewutils::intersects(itemRect, line)) {
                    pos = idx;
                    break;
                }
            }
        }
    }
    const QVector<QPointF> points = path(scene(), m_points.value(pos), iface->connectionEndPoint(this));
    if (!points.isEmpty()) {
        if (reverse) {
            m_points.remove(0, pos + 1);
            for (const QPointF &point : points)
                m_points.prepend(point);
        } else {
            m_points.remove(pos, m_points.size() - pos);
            for (const QPointF &point : points)
                m_points.append(point);
        }
        m_points = simplifyPoints(m_points);
        updateGripPoints();
    } else {
        m_points = connectionPath(m_startItem, m_endItem);
    }

    updateBoundingRect();
}

} // namespace aadlinterface
