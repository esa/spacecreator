/*
 Copyright (C) 2018-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "verectgraphicsitem.h"

#include "graphicsviewutils.h"
#include "topohelper/geometry.h"
#include "ui/grippointshandler.h"
#include "ui/minimizelimits.h"
#include "ui/resizelimits.h"
#include "ui/textitem.h"
#include "veconnectionendpointgraphicsitem.h"
#include "veobject.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QtDebug>

namespace shared {
namespace ui {

VERectGraphicsItem::VERectGraphicsItem(VEObject *entity, QGraphicsItem *parentGraphicsItem)
    : VEInteractiveObject(entity, parentGraphicsItem)
{
    connect(this, &VEInteractiveObject::relocated, this, &VERectGraphicsItem::onGeometryChanged);
    connect(this, &VEInteractiveObject::boundingBoxChanged, this, &VERectGraphicsItem::onGeometryChanged);
    setHighlightable(true);
}

bool VERectGraphicsItem::setGeometry(const QRectF &sceneGeometry)
{
    if (sceneGeometry == sceneBoundingRect())
        return false;

    /// Silent geometry change without relayouting
    bool sceneGeometryIsResize = sceneBoundingRect().size() != sceneGeometry.size();
    const QPointF shift = sceneBoundingRect().topLeft() - sceneGeometry.topLeft();
    setPos(mapToParent(mapFromScene(sceneGeometry.topLeft())));
    if (sceneGeometryIsResize) {
        for (QGraphicsItem *child : childItems()) {
            auto iObj = qobject_cast<VERectGraphicsItem *>(child->toGraphicsObject());
            if (iObj) {
                iObj->moveBy(shift.x(), shift.y());
            }
        }
    }

    QRectF br = boundingRect();
    if (br.size() != sceneGeometry.size()) {
        prepareGeometryChange();
        br.setSize(sceneGeometry.size());
        setBoundingRect(br);
    }

    Q_EMIT boundingBoxChanged();

    updateGripPoints();
    return true;
}

void VERectGraphicsItem::setRect(const QRectF &geometry)
{
    if (setGeometry(geometry)) {
        instantLayoutUpdate();
    }
}

void VERectGraphicsItem::initGripPoints()
{
    VEInteractiveObject::initGripPoints();
    gripPointsHandler()->setUsedPoints({ GripPoint::Location::Top, GripPoint::Location::Left,
            GripPoint::Location::Bottom, GripPoint::Location::Right, GripPoint::Location::TopLeft,
            GripPoint::Location::BottomLeft, GripPoint::Location::TopRight, GripPoint::Location::BottomRight });
}

/**
 * @brief updateFromEntity
 * Updates this instance's geometry with data from the model entity
 */
void VERectGraphicsItem::updateFromEntity()
{
    shared::VEObject *obj = entity();
    Q_ASSERT(obj);
    if (!obj)
        return;

    const QRectF itemSceneRect { topohelp::geom::rect(obj->coordinates()) };
    if (!itemSceneRect.isValid()) {
        doLayout();
    } else {
        setRect(itemSceneRect);
        doLayout();
    }
}

QList<QPair<shared::VEObject *, QVector<QPointF>>> VERectGraphicsItem::prepareChangeCoordinatesCommandParams() const
{
    QList<QPair<VEObject *, QVector<QPointF>>> params;
    const QVector<QPointF> sceneGeometry { sceneBoundingRect().topLeft(), sceneBoundingRect().bottomRight() };
    params.append({ entity(), sceneGeometry });
    params.append(VEInteractiveObject::prepareChangeCoordinatesCommandParams());
    return params;
}

void VERectGraphicsItem::onManualMoveProgress(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt) {
        return;
    }

    const QRectF rect = graphicsviewutils::snappedRect(scene(), movedRect(pressedAt, releasedAt));
    if (QGraphicsItem *parentObj = parentItem()) {
        const QRectF parentRect = parentObj->sceneBoundingRect().marginsRemoved(topohelp::kContentMargins);
        if (!topohelp::geom::isRectBounded(parentRect, rect)) {
            return;
        }
    }
    setGeometry(rect);
}

void VERectGraphicsItem::onManualResizeProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    if (from == to) {
        return;
    }

    // Calculate new rect for this item, given that 'grip' was moved as descriped by 'from' and 'to'
    ResizeLimits resizeLimits(grip, from, to, sceneBoundingRect());
    QRectF newRect = resizedRect(resizeLimits);
    newRect = graphicsviewutils::snappedRect(scene(), newRect);
    setGeometry(newRect);
}

/**
 * @brief layoutInterfaces updates the position of each connection point on
 * this VERectGraphicsItem instance
 */
void VERectGraphicsItem::layoutInterfaces()
{
    for (auto child : childItems()) {
        if (auto endPoint = qobject_cast<VEConnectionEndPointGraphicsItem *>(child->toGraphicsObject())) {
            endPoint->adjustItem();
        }
    }
}

void VERectGraphicsItem::updateTextPosition()
{
    if (m_textItem) {
        m_textItem->setExplicitSize(boundingRect().size());
    }
}

void VERectGraphicsItem::showHelperLines(bool show)
{
    if (show && helpLinesSupported()) {
        const QRectF rect = boundingRect();
        if (!m_topHelper) {
            m_topHelper = graphicsviewutils::horizontalHelper(0, this);
        }
        if (!m_bottomHelper) {
            m_bottomHelper = graphicsviewutils::horizontalHelper(rect.height(), this);
        }
        if (!m_leftHelper) {
            m_leftHelper = graphicsviewutils::verticalHelper(0, this);
        }
        if (!m_rightHelper) {
            m_rightHelper = graphicsviewutils::verticalHelper(rect.width(), this);
        }
    } else {
        delete m_topHelper;
        m_topHelper = nullptr;
        delete m_bottomHelper;
        m_bottomHelper = nullptr;
        delete m_leftHelper;
        m_leftHelper = nullptr;
        delete m_rightHelper;
        m_rightHelper = nullptr;
    }
}

void VERectGraphicsItem::updateHelperLines()
{
    using topohelp::kHelperLineLength;

    const QRectF rect = boundingRect();
    if (m_topHelper) {
        const qreal y = rect.y();
        m_topHelper->setLine(QLineF(-kHelperLineLength, y, kHelperLineLength, y));
    }
    if (m_bottomHelper) {
        const qreal y = rect.y() + rect.height();
        m_bottomHelper->setLine(QLineF(-kHelperLineLength, y, kHelperLineLength, y));
    }
    if (m_leftHelper) {
        const qreal x = rect.x();
        m_leftHelper->setLine(QLineF(x, -kHelperLineLength, x, kHelperLineLength));
    }
    if (m_rightHelper) {
        const qreal x = rect.x() + rect.width();
        m_rightHelper->setLine(QLineF(x, -kHelperLineLength, x, kHelperLineLength));
    }
}

/*!
 *  Update positions of interface attachment points (iface)
 */
void VERectGraphicsItem::updateChildrenGeometry()
{
    if (!entity()) {
        return;
    }

    const QRectF entityRect = topohelp::geom::rect(entity()->coordinates());
    for (auto child : childItems()) {
        auto iface = qobject_cast<VEConnectionEndPointGraphicsItem *>(child->toGraphicsObject());
        if (iface) {
            // Read the stored coordinates
            const VEObject *obj = iface->entity();
            Q_ASSERT(obj);
            if (!obj) {
                return;
            }

            const QPointF storedPos = topohelp::geom::pos(obj->coordinates());
            if (storedPos.isNull()) {
                iface->instantLayoutUpdate();
                continue;
            }

            // Calculate the new postion of the iface.
            const Qt::Alignment side = topohelp::geom::getNearestSide(entityRect, storedPos);
            const QRectF sceneRect = sceneBoundingRect();
            QPointF pos;
            if (sceneRect.size() == entityRect.size()) {
                // moving
                const QPointF diff = sceneRect.topLeft() - entityRect.topLeft();
                pos = storedPos + diff;
            } else {
                // resizing
                pos = topohelp::geom::getSidePosition(sceneRect, storedPos, side);
            }
            iface->setPos(iface->parentItem()->mapFromScene(pos));
        }
    }
}

void VERectGraphicsItem::onManualResizeFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt && !m_overridingCursor) {
        return;
    }

    const QRectF rect = sceneBoundingRect();
    bool isBounded = shared::graphicsviewutils::isBounded(this, rect);
    bool noCollision = !shared::graphicsviewutils::isCollided(this, rect);
    if (isBounded && noCollision) {
        layoutInterfaces();
        layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::PartialRebuild);
        updateEntity();
    }
    layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);

    // If a collision takes place, reread all positions from the model
    if (shared::graphicsviewutils::isCollided(this, rect)) {
        updateFromEntity();
        // Make children update from entity also
        for (auto child : childItems()) {
            if (auto connectionEndPoint = qobject_cast<VEConnectionEndPointGraphicsItem *>(child->toGraphicsObject())) {
                connectionEndPoint->updateFromEntity();
            }
        }
        layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

void VERectGraphicsItem::onManualMoveFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt && !m_overridingCursor) {
        return;
    }

    bool isBounded = shared::graphicsviewutils::isBounded(this, sceneBoundingRect());
    bool noCollision = !shared::graphicsviewutils::isCollided(this, sceneBoundingRect());
    if (isBounded && noCollision) {
        layoutConnectionsOnMove(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::PartialRebuild);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        layoutConnectionsOnMove(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);
    }
}

/**
 * @brief minimumSize
 * Children classes will know how to calculate their minimum size.
 */
QSizeF VERectGraphicsItem::minimumSize() const
{
    // This very naive minimum size will at least keep the children from becoming invalid. Children should
    // provide their own implementation.
    return QSizeF(100, 50);
}

QList<VERectGraphicsItem *> VERectGraphicsItem::findCollidingVERectGraphicsItems(
        const QRectF &rect, QMarginsF margins) const
{
    QList<VERectGraphicsItem *> collidingSiblings;
    QList<QGraphicsItem *> items = scene()->items();
    for (auto item : items) {
        if (item == this) {
            continue;
        }
        if (parentItem() == item) {
            continue;
        }
        QRectF itemRect = item->sceneBoundingRect();
        if (rect.intersects(itemRect.marginsAdded(margins))) {
            auto sibling = qobject_cast<VERectGraphicsItem *>(item->toGraphicsObject());
            if (sibling) {
                collidingSiblings.append(sibling);
            }
        }
    }
    return collidingSiblings;
}

ResizeLimits VERectGraphicsItem::resizeRectForConnectionEndpoints(ResizeLimits resizeLimits)
{
    MinimizeLimits::Limits limits;
    // Calculate the limits of movement for each grip-point on a rect with no connections
    QRectF result = sceneBoundingRect();
    limits.leftGripsRightMostLimit =
            result.right(); // The furthes to the right, the left-grip can go is the right side of the rect.
    limits.rightGripsLeftMostLimit =
            result.left(); // The furthest to the left, the right-grip can go is the left side of the rect.
    limits.topGripsBottomMostLimit = result.bottom(); // The deepest the top-grip can go, is the bottom of the rect.
    limits.bottomGripsTopMostLimit = result.top(); // The highest the bottom-grip can go, is the top of the rect.

    // Calculate the limits of movement with respect to connection endpoints
    for (auto child : childItems()) {
        auto endPoint = qobject_cast<ui::VEConnectionEndPointGraphicsItem *>(child->toGraphicsObject());
        if (endPoint == nullptr) {
            continue;
        }

        auto alignment = endPoint->alignment();
        bool endPointIsHorizonal = alignment == Qt::AlignTop || alignment == Qt::AlignBottom;
        bool endPointIsVertical = alignment == Qt::AlignLeft || alignment == Qt::AlignRight;
        Q_ASSERT(endPointIsHorizonal || endPointIsVertical);

        auto endPointRect = endPoint->sceneBoundingRect();
        if (endPointIsHorizonal) {
            auto left = endPointRect.left();
            limits.leftGripsRightMostLimit = qMin(limits.leftGripsRightMostLimit, left);

            auto right = endPointRect.right();
            limits.rightGripsLeftMostLimit = qMax(limits.rightGripsLeftMostLimit, right);
        }
        if (endPointIsVertical) {
            auto top = endPointRect.top();
            limits.topGripsBottomMostLimit = qMin(limits.topGripsBottomMostLimit, top);

            auto bottom = endPointRect.bottom();
            limits.bottomGripsTopMostLimit = qMax(limits.bottomGripsTopMostLimit, bottom);
        }
    }

    if (resizeLimits.isTopEdgeMinimizing()) {
        resizeLimits.setHorizontalLimit(limits.topGripsBottomMostLimit);
    }

    if (resizeLimits.isBottomEdgeMinimizing()) {
        resizeLimits.setHorizontalLimit(limits.bottomGripsTopMostLimit);
    }

    if (resizeLimits.isLeftEdgeMinimizing()) {
        resizeLimits.setVerticalLimit(limits.leftGripsRightMostLimit);
    }

    if (resizeLimits.isRightEdgeMinimizing()) {
        resizeLimits.setVerticalLimit(limits.rightGripsLeftMostLimit);
    }

    return resizeLimits;
}

QRectF VERectGraphicsItem::movedRect(const QPointF &from, const QPointF &to)
{
    auto deltaPoint = to - from;
    QRectF boundingRect = sceneBoundingRect();
    boundingRect.translate(deltaPoint);
    return boundingRect;
}

QRectF VERectGraphicsItem::nestedItemsSceneBoundingRect() const
{
    QRectF nestedItemsBoundingRect;
    for (const QGraphicsItem *item : childItems()) {
        if (auto rectItem = qobject_cast<const VERectGraphicsItem *>(item->toGraphicsObject())) {
            const QRectF nestedRect = rectItem->sceneBoundingRect();
            if (nestedRect.isValid())
                nestedItemsBoundingRect |= nestedRect;
        }
    }
    return nestedItemsBoundingRect;
}

void VERectGraphicsItem::onGeometryChanged()
{
    QSet<VEInteractiveObject *> items;
    QRectF areaToCheckForCollisions = sceneBoundingRect().marginsAdded(topohelp::kContentMargins);
    QList<QGraphicsItem *> collidedItems = scene()->items(areaToCheckForCollisions);
    std::for_each(collidedItems.begin(), collidedItems.end(), [this, &items](QGraphicsItem *item) {
        auto rectItem = qobject_cast<VERectGraphicsItem *>(item->toGraphicsObject());
        if (rectItem && item != this && item->parentItem() == parentItem())
            items.insert(rectItem);
    });
    QSet<VEInteractiveObject *> newItems(items);
    newItems.subtract(m_collidedItems);
    for (auto item : newItems) {
        // item->doHighlighting(Qt::red, true);
    }
    QSet<VEInteractiveObject *> oldItems(m_collidedItems);
    oldItems.subtract(items);

    for (auto item : oldItems) {
        // item->doHighlighting(Qt::green, false);
    }
    m_collidedItems = items;

    if (m_collidedItems.isEmpty()) {
        if (m_overridingCursor) {
            qApp->restoreOverrideCursor();
            m_overridingCursor = false;
        }
        clearHighlight();
    } else {
        if (!m_overridingCursor) {
            qApp->setOverrideCursor(Qt::ForbiddenCursor);
            m_overridingCursor = true;
        }
    }

    updateChildrenGeometry();
    layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);

    updateHelperLines();
}

bool VERectGraphicsItem::doLayout()
{
    if (layoutShouldBeChanged()) {
        const auto parentFunction = qobject_cast<VERectGraphicsItem *>(parentObject());

        QRectF boundedRect;
        QMarginsF margins;
        if (parentFunction) {
            boundedRect = parentFunction->sceneBoundingRect();
            margins = topohelp::kContentMargins;
        } else {
            boundedRect = scene()->itemsBoundingRect();
            margins = topohelp::kRootMargins;
        }

        QRectF itemRect = this->boundingRect();
        itemRect.moveTopLeft(boundedRect.topLeft());

        topohelp::geom::findGeometryForRect(itemRect, boundedRect, graphicsviewutils::siblingItemsRects(this), margins);
        if (itemRect != sceneBoundingRect()) {
            setRect(itemRect);
            return true;
        }
    }
    return false;
}

bool VERectGraphicsItem::layoutShouldBeChanged() const
{
    const QRectF currentRect = sceneBoundingRect();
    if (!currentRect.isValid()) {
        return true;
    }

    bool collidesWithOther = graphicsviewutils::isCollided(this, currentRect);

    return collidesWithOther || !graphicsviewutils::isBounded(this, currentRect);
}

void VERectGraphicsItem::layoutConnectionsOnResize(VEConnectionGraphicsItem::CollisionsPolicy collisionsPolicy)
{
    /// Changing inner and outer connections bound to current item
    for (const auto item : childItems()) {
        if (auto iface = qobject_cast<VEConnectionEndPointGraphicsItem *>(item->toGraphicsObject())) {
            VEConnectionGraphicsItem::layoutInterfaceConnections(
                    iface, VEConnectionGraphicsItem::LayoutPolicy::Scaling, collisionsPolicy, true);
        } else if (auto connection = qobject_cast<VEConnectionGraphicsItem *>(item->toGraphicsObject())) {
            if (connection->sourceItem() != this && connection->targetItem() != this)
                connection->doLayout();
        }
    }
}

void VERectGraphicsItem::layoutConnectionsOnMove(VEConnectionGraphicsItem::CollisionsPolicy collisionsPolicy)
{
    /// Changing outer connections only cause inner stay unchanged as children of current item
    for (const auto item : childItems()) {
        if (auto iface = qobject_cast<VEConnectionEndPointGraphicsItem *>(item->toGraphicsObject())) {
            VEConnectionGraphicsItem::layoutInterfaceConnections(
                    iface, VEConnectionGraphicsItem::LayoutPolicy::Scaling, collisionsPolicy, false);
        }
    }
}

/**
 * @brief resizedRect takes a GripPoint sitting on this VERectGraphicsItem and two
 * QPointFs representing the movement of the GripPoint and returns
 * a QRectF representing the new sceneBoundingRect of this VERectGraphicsItem based on
 * the allowed movement of that particular handle.
 *
 * A corner GripPoint can move freely. A GripPoint on a horizontal line can only move
 * up or down. A GripPoint on a vertical line can only move left or right.
 *
 * If a transformation is illegal, a QRectF is calculated that performs as much as possible of the
 * transformation as is legal. This includes not making the rectangle invalid.
 *
 * @param grip a GripPoint
 * @param from a QPointF the GripPoint was moved from
 * @param to a QPointF the GripPoint was moved to
 * @return a QRectF representing the new sceneBoundingRect of this VERectGraphicsItem
 */
QRectF VERectGraphicsItem::resizedRect(ResizeLimits resizeLimits)
{
    resizeLimits = resizeRectForConnectionEndpoints(resizeLimits);
    QRectF limitedRect = resizeLimits.getLimitedRect();
    return limitedRect;
}

} // namespace ui

} // namespace shared
