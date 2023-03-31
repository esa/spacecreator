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
    if (pressedAt == releasedAt)
        return;

    const QRectF rect = movedRect(pressedAt, releasedAt);
    if (QGraphicsItem *parentObj = parentItem()) {
        const QRectF parentRect =
                parentObj->sceneBoundingRect().marginsRemoved(shared::graphicsviewutils::kContentMargins);
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
    QRectF newRect = resizedRect(grip, from, to);
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

void VERectGraphicsItem::onManualResizeFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt) {
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
    if (pressedAt == releasedAt)
        return;

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

QSizeF VERectGraphicsItem::minimumSize() const
{
    // This very naive minimum size will at least keep the children from becoming invalid. Children should
    // provide their own implementation.
    return QSizeF(100, 50);
}

QRectF VERectGraphicsItem::resizedRect(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    QRectF sBoundingRect = sceneBoundingRect();
    QRectF rectWithRespectToConnections = checkConnectionEndpoints(grip, to, sBoundingRect);
    return rectWithRespectToConnections;
}

QRectF VERectGraphicsItem::checkConnectionEndpoints(GripPoint *grip, const QPointF &to, const QRectF &sceneBoundingRect)
{
    QRectF result = sceneBoundingRect;
    // Note: In QGraphicsView the Y-axis grows downwards.
    // https://doc.qt.io/qt-6/graphicsview.html#the-graphics-view-coordinate-system
    qreal leftMost = result.right(); // The furthest to the left, the right-grip can go is the right side of the rect.
    qreal rightMost = result.left(); // The furthes to the right, the left-grip can go is the left side of the rect.
    qreal topMost = result.bottom(); // The deepest the top-grip can go, is the bottom of the rect.
    qreal bottomMost = result.top(); // The highest the bottom-grip can go, is the top of the rect.

    for (auto child : childItems()) {
        auto endPoint = qobject_cast<ui::VEConnectionEndPointGraphicsItem *>(child->toGraphicsObject());
        if (endPoint == nullptr) {
            continue;
        }

        auto alignment = endPoint->alignment();
        bool endPointIsHorizonal = alignment == Qt::AlignTop || alignment == Qt::AlignBottom;
        bool endPointIsVertical = alignment == Qt::AlignLeft || alignment == Qt::AlignRight;
        Q_ASSERT(endPointIsHorizonal || endPointIsVertical);
        auto childSceneBoundingRect = endPoint->sceneBoundingRect();
        if (endPointIsHorizonal) {
            auto left = childSceneBoundingRect.left();
            leftMost = qMin(leftMost, left);

            auto right = childSceneBoundingRect.right();
            rightMost = qMax(rightMost, right);
        }
        if (endPointIsVertical) {
            auto top = childSceneBoundingRect.top();
            topMost = qMin(topMost, top);

            auto bottom = childSceneBoundingRect.bottom();
            bottomMost = qMax(bottomMost, bottom);
        }
    }

    switch (grip->location()) {
    case GripPoint::Left: {
        result.setLeft(qMin(to.x(), leftMost));
        break;
    }
    case GripPoint::Right: {
        result.setRight(qMax(to.x(), rightMost));
        break;
    }
    case GripPoint::Top: {
        result.setTop(qMin(to.y(), topMost));
        break;
    }
    case GripPoint::Bottom: {
        result.setBottom(qMax(to.y(), bottomMost));
        break;
    }
    case GripPoint::TopLeft: {
        result.setTopLeft(QPoint(qMin(to.x(), leftMost), qMin(to.y(), topMost)));
        break;
    }
    case GripPoint::TopRight: {
        result.setTopRight(QPoint(qMax(to.x(), rightMost), qMin(to.y(), topMost)));
        break;
    }
    case GripPoint::BottomLeft: {
        result.setBottomLeft(QPoint(qMin(to.x(), leftMost), qMax(to.y(), bottomMost)));
        break;
    }
    case GripPoint::BottomRight: {
        result.setBottomRight(QPoint(qMax(to.x(), rightMost), qMax(to.y(), bottomMost)));
        break;
    }
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    return result;
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
    QList<QGraphicsItem *> collidedItems =
            scene()->items(sceneBoundingRect().marginsAdded(graphicsviewutils::kContentMargins));
    std::for_each(collidedItems.begin(), collidedItems.end(), [this, &items](QGraphicsItem *item) {
        auto rectItem = qobject_cast<VERectGraphicsItem *>(item->toGraphicsObject());
        if (rectItem && item != this && item->parentItem() == parentItem())
            items.insert(rectItem);
    });
    QSet<VEInteractiveObject *> newItems(items);
    newItems.subtract(m_collidedItems);
    for (auto item : newItems)
        item->doHighlighting(Qt::red, true);

    QSet<VEInteractiveObject *> oldItems(m_collidedItems);
    oldItems.subtract(items);

    for (auto item : oldItems)
        item->doHighlighting(Qt::green, false);

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

        QRectF itemRect = QRectF(QPointF(0, 0), minimumSize());
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

} // namespace ui

} // namespace shared
