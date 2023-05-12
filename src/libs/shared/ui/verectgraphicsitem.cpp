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

using graphicsviewutils::kContentMargins;

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

    const QRectF itemSceneRect { graphicsviewutils::rect(obj->coordinates()) };
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

    const QRectF rect = movedRect(pressedAt, releasedAt);
    if (QGraphicsItem *parentObj = parentItem()) {
        const QRectF parentRect =
                parentObj->sceneBoundingRect().marginsRemoved(kContentMargins);
        if (!shared::graphicsviewutils::isRectBounded(parentRect, rect)) {
            return;
        }
    }
    setGeometry(rect);
    layoutConnectionsOnMove(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);
}

void VERectGraphicsItem::onManualResizeProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    if (from == to) {
        return;
    }

    // Calculate new rect for this item, given that 'grip' was moved as descriped by 'from' and 'to'
    ResizeLimits resizeLimits(grip, from, to, sceneBoundingRect());
    QRectF newRect = resizedRect(resizeLimits);
    setGeometry(newRect);

    // Update positions of interface attachment points (iface)
    const QRectF entityRect = graphicsviewutils::rect(entity()->coordinates());
    for (auto child : childItems()) {
        auto iface = qobject_cast<VEConnectionEndPointGraphicsItem *>(child->toGraphicsObject());
        if (iface) {
            // Read the stored coordinates
            const VEObject *obj = iface->entity();
            Q_ASSERT(obj);
            if (!obj) {
                return;
            }

            const QPointF storedPos = graphicsviewutils::pos(obj->coordinates());
            if (storedPos.isNull()) {
                iface->instantLayoutUpdate();
                continue;
            }

            // Calculate the new postion of the iface.
            const Qt::Alignment side = graphicsviewutils::getNearestSide(entityRect, storedPos);
            const QRectF sceneRect = sceneBoundingRect();
            const QPointF pos = graphicsviewutils::getSidePosition(sceneRect, storedPos, side);
            iface->setPos(iface->parentItem()->mapFromScene(pos));
        }
    }
    layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::Ignore);
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

void VERectGraphicsItem::onManualResizeFinish(GripPoint *grip,
                                              const QPointF &pressedAt,
                                              const QPointF &releasedAt)
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

void VERectGraphicsItem::onManualMoveFinish(GripPoint *grip,
                                            const QPointF &pressedAt,
                                            const QPointF &releasedAt)
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




//ResizeLimits VERectGraphicsItem::resizeLimitsForCollision(ResizeLimits resizeLimits)
//{
//    // In case this VERectGraphicsItem has a parent (is a nested function) check
//    // for collision with the parent's edges
//    QGraphicsItem *parent = parentItem();
//    VERectGraphicsItem *parentVERectGraphicsItem = nullptr;
//    if (parent) {
//        parentVERectGraphicsItem = qobject_cast<VERectGraphicsItem *>(parent->toGraphicsObject());
//    }
//    if (parentVERectGraphicsItem) {
//        QRectF parentRect = parentVERectGraphicsItem->sceneBoundingRect();

//        if (resizeLimits.isLeftEdgeMaximizing()) {
//            // See if left edge of this rect is left of parent left edge
//            qreal parentLeftEdge = parentRect.left();
//            parentLeftEdge += kContentMargins.left();
//            qreal unlimitedLeft = resizeLimits.getUnlimitedRect().left();
//            if (parentLeftEdge > unlimitedLeft) {
//                resizeLimits.setVerticalLimit(parentLeftEdge);
//            }
//        }

//        if (resizeLimits.isRightEdgeMaximizing()) {
//            // See if right edge of this rect is right of parent right edge
//            qreal parentRightEdge = parentRect.right();
//            parentRightEdge -= kContentMargins.right();
//            qreal unlimitedRight = resizeLimits.getUnlimitedRect().right();
//            if (parentRightEdge < unlimitedRight) {
//                resizeLimits.setVerticalLimit(parentRightEdge);
//            }
//        }

//        if (resizeLimits.isTopEdgeMaximizing()) {
//            // See if top edge of this rect is above parent top edge
//            qreal parentTopEdge = parentRect.top();
//            parentTopEdge += kContentMargins.top();
//            qreal unlimitedTop = resizeLimits.getUnlimitedRect().top();
//            if (parentTopEdge > unlimitedTop) {
//                resizeLimits.setHorizontalLimit(parentTopEdge);
//            }
//        }

//        if (resizeLimits.isBottomEdgeMaximizing()) {
//            // See if bottom edge of this rect is below parent bottom edge
//            qreal parentBottomEdge = parentRect.bottom();
//            parentBottomEdge -= kContentMargins.bottom();
//            qreal unlimitedBottom = resizeLimits.getUnlimitedRect().bottom();
//            if (parentBottomEdge < unlimitedBottom) {
//                resizeLimits.setHorizontalLimit(parentBottomEdge);
//            }
//        }
//    }


//    bool bothResizeTypesAreMax = resizeLimits.getVerticalResizeType() == ResizeType::Max
//            && resizeLimits.getHorizontalResizeType() == ResizeType::Max;
//    if (bothResizeTypesAreMax) {
//        VerticalEdge vEdge = resizeLimits.getVerticalEdge();
//        HorizontalEdge hEdge = resizeLimits.getHorizontalEdge();
//        bool areaIsTopLeft = hEdge == HorizontalEdge::TopEdge && vEdge == VerticalEdge::LeftEdge;
//        bool areaIsTopRight = hEdge == HorizontalEdge::TopEdge && vEdge == VerticalEdge::RightEdge;
//        bool areaIsBottomRight = hEdge == HorizontalEdge::BottomEdge && vEdge == VerticalEdge::RightEdge;
//        bool areaIsBottomLeft = hEdge == HorizontalEdge::BottomEdge && vEdge == VerticalEdge::LeftEdge;

//        QList<VERectGraphicsItem *> collidingItems;
//        if (areaIsTopLeft) {
//            QRectF topLeftRect = resizeLimits.getUnlimitedTopLeftRect();
//            collidingItems = findCollidingVERectGraphicsItems(topLeftRect, QMarginsF(kContentMargins.left(), kContentMargins.top(), 0.0, 0.0));
//        }
//        if (areaIsTopRight) {
//            QRectF topRightRect = resizeLimits.getUnlimitedTopRightRect();
//            collidingItems = findCollidingVERectGraphicsItems(topRightRect, QMarginsF(0.0, kContentMargins.top(), kContentMargins.right(), 0.0));
//        }
//        if (areaIsBottomRight) {
//            QRectF bottomRight = resizeLimits.getUnlimitedBottomRightRect();
//            collidingItems = findCollidingVERectGraphicsItems(bottomRight, QMarginsF(0.0, 0.0, kContentMargins.right(), kContentMargins.bottom()));
//        }
//        if (areaIsBottomLeft) {
//            QRectF bottomLeftRect = resizeLimits.getUnlimitedBottomLeftRect();
//            collidingItems = findCollidingVERectGraphicsItems(bottomLeftRect, QMarginsF(kContentMargins.left(), 0.0, 0.0, kContentMargins.bottom()));
//        }

//        if (!collidingItems.isEmpty()) {
//            QRectF collidingItemRect = collidingItems.first()->sceneBoundingRect();
//            QPair<HorizontalEdge, VerticalEdge> intersectedEdges = resizeLimits.getIntersectedEdges(collidingItemRect);
//            HorizontalEdge intersectedHorizontalEdge = intersectedEdges.first;
//            VerticalEdge intersectedVerticalEdge = intersectedEdges.second;

//            if (intersectedHorizontalEdge == HorizontalEdge::TopEdge) {
//                if (resizeLimits.isHorizontalLimitSet()) {
//                    qreal horizontalLimit = resizeLimits.getHorizontalLimit();
//                    resizeLimits.setHorizontalLimit(qMin(horizontalLimit, collidingItemRect.top()));
//                } else {
//                    resizeLimits.setHorizontalLimit(collidingItemRect.top());
//                }
//            }
//            if (intersectedHorizontalEdge == HorizontalEdge::BottomEdge) {
//                if (resizeLimits.isHorizontalLimitSet()) {
//                    qreal horizontalLimit = resizeLimits.getHorizontalLimit();
//                    resizeLimits.setHorizontalLimit(qMax(horizontalLimit, collidingItemRect.bottom()));
//                } else {
//                    resizeLimits.setHorizontalLimit(collidingItemRect.bottom());
//                }
//            }

//            if (intersectedVerticalEdge == VerticalEdge::LeftEdge) {
//                if (resizeLimits.isVerticalLimitSet()) {
//                    qreal verticalLimit = resizeLimits.getVerticalLimit();
//                    resizeLimits.setVerticalLimit(qMin(verticalLimit, collidingItemRect.left()));
//                } else {
//                    resizeLimits.setVerticalLimit(collidingItemRect.left());
//                }
//            }
//            if (intersectedVerticalEdge == VerticalEdge::RightEdge) {
//                if (resizeLimits.isVerticalLimitSet()) {
//                    qreal verticalLimit = resizeLimits.getVerticalLimit();
//                    resizeLimits.setVerticalLimit(qMax(verticalLimit, collidingItemRect.right()));
//                } else {
//                    resizeLimits.setVerticalLimit(collidingItemRect.right());
//                }
//            }
//        }
//    }

//    // This section is for when the user drags the right or left grippoint in a maximizing manner
//    bool rectIsLeftAndRight = resizeLimits.getVerticalResizeType() == ResizeType::Max;
//    if (rectIsLeftAndRight) {
//        // Detect collision left and right
//        QRectF leftRightRect = resizeLimits.getUnlimitedVerticalRect(); // Get the unlimited rect as if the user had
//                                                                        // only used the right-, or left-grippoint.
//        QList<VERectGraphicsItem *> collidingItemsLeftOrRight = findCollidingVERectGraphicsItems(leftRightRect, QMarginsF(kContentMargins.left(), 0.0, kContentMargins.right(), 0.0));

//        if (!collidingItemsLeftOrRight.isEmpty()) {
//            if (resizeLimits.getVerticalEdge() == VerticalEdge::LeftEdge) {
//                qreal rightMostEdge = findRightMostEdge(collidingItemsLeftOrRight);
//                rightMostEdge += kContentMargins.left();
//                if (resizeLimits.isVerticalLimitSet()) {
//                    qreal verticalLimit = resizeLimits.getVerticalLimit();
//                    resizeLimits.setVerticalLimit(qMax(verticalLimit, rightMostEdge));
//                } else {
//                    resizeLimits.setVerticalLimit(rightMostEdge);
//                }
//                resizeLimits.setVerticalLimit(rightMostEdge);
//            }
//            if (resizeLimits.getVerticalEdge() == VerticalEdge::RightEdge) {
//                qreal leftMostEdge = findLeftMostEdge(collidingItemsLeftOrRight);
//                leftMostEdge -= kContentMargins.right();
//                if (resizeLimits.isVerticalLimitSet()) {
//                    qreal verticalLimit = resizeLimits.getVerticalLimit();
//                    resizeLimits.setVerticalLimit(qMin(verticalLimit, leftMostEdge));
//                } else {
//                    resizeLimits.setVerticalLimit(leftMostEdge);
//                }
//            }
//        }
//    }

//    // This section is for when the user drags the top or bottom grippoint
//    bool rectIsUpAndDown = resizeLimits.getHorizontalResizeType() == ResizeType::Max;
//    if (rectIsUpAndDown) {
//        // Detect collision up and down
//        QRectF upDownRect = resizeLimits.getUnlimitedHorizontalRect(); // Get the unlimited rect as if the user had only
//                                                                       // used the top-, or bottom-grippoint.
//        QList<VERectGraphicsItem *> collidingItemsUpOrDown = findCollidingVERectGraphicsItems(upDownRect, QMarginsF(0.0, kContentMargins.top(), 0.0, kContentMargins.bottom()));

//        if (!collidingItemsUpOrDown.isEmpty()) {
//            if (resizeLimits.getHorizontalEdge() == HorizontalEdge::TopEdge) {
//                qreal bottomMostEdge = findBottomMostEdge(collidingItemsUpOrDown);
//                bottomMostEdge += kContentMargins.top();
//                if (resizeLimits.isHorizontalLimitSet()) {
//                    qreal horizontalLimit = resizeLimits.getHorizontalLimit();
//                    resizeLimits.setHorizontalLimit(qMax(horizontalLimit, bottomMostEdge));
//                } else {
//                    resizeLimits.setHorizontalLimit(bottomMostEdge);
//                }
//            }
//            if (resizeLimits.getHorizontalEdge() == HorizontalEdge::BottomEdge) {
//                qreal topMostEdge = findTopMostEdge(collidingItemsUpOrDown);
//                topMostEdge -= kContentMargins.bottom();
//                if (resizeLimits.isHorizontalLimitSet()) {
//                    qreal horizontalLimit = resizeLimits.getHorizontalLimit();
//                    resizeLimits.setHorizontalLimit(qMin(horizontalLimit, topMostEdge));
//                } else {
//                    resizeLimits.setHorizontalLimit(topMostEdge);
//                }
//            }
//        }
//    }

//    return resizeLimits;
//}


QList<VERectGraphicsItem *> VERectGraphicsItem::findCollidingVERectGraphicsItems(const QRectF &rect, QMarginsF margins) const
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
            if (sibling)
            {
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
    QRectF areaToCheckForCollisions = sceneBoundingRect().marginsAdded(graphicsviewutils::kContentMargins);
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
        //item->doHighlighting(Qt::green, false);
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
}


bool VERectGraphicsItem::doLayout()
{
    if (layoutShouldBeChanged()) {
        const auto parentFunction = qobject_cast<VERectGraphicsItem *>(parentObject());

        QRectF boundedRect;
        QMarginsF margins;
        if (parentFunction) {
            boundedRect = parentFunction->sceneBoundingRect();
            margins = graphicsviewutils::kContentMargins;
        } else {
            boundedRect = scene()->itemsBoundingRect();
            margins = graphicsviewutils::kRootMargins;
        }

        QRectF itemRect = this->boundingRect();
        itemRect.moveTopLeft(boundedRect.topLeft());

        graphicsviewutils::findGeometryForRect(
                itemRect, boundedRect, graphicsviewutils::siblingItemsRects(this), margins);
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
