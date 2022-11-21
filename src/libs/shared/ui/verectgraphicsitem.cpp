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
    if (sceneGeometryIsResize)
    {
        for (QGraphicsItem *child : childItems())
        {
            auto iObj = qobject_cast<VERectGraphicsItem *>(child->toGraphicsObject());
            if (iObj)
            {
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
    if (setGeometry(geometry))
        instantLayoutUpdate();
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
        if (layoutShouldBeChanged() && doLayout()) {
            mergeGeometry();
        }
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

    const QRectF rect = transformedRect(grip, pressedAt, releasedAt);
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
    if (from == to)
    {
        return;
    }

    // calculate new rect for this item, given that 'grip' was moved as descriped by 'from' and 'to'
    QRectF newRect = transformedRect(grip, from, to);
    // Enforce minimal size
    setGeometry(newRect);

    const QRectF entityRect = graphicsviewutils::rect(entity()->coordinates());
    for (auto child : childItems())
    {
        auto iface = qobject_cast<VEConnectionEndPointGraphicsItem *>(child->toGraphicsObject());
        if (iface)
        {
            const VEObject *obj = iface->entity();
            Q_ASSERT(obj);
            if (!obj) {
                return;
            }

            const QPointF storedPos = graphicsviewutils::pos(obj->coordinates());
            if (storedPos.isNull() || !grip) // how can grip be null?
            {
                iface->instantLayoutUpdate();
                continue;
            }

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

void VERectGraphicsItem::onManualResizeFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt)
        return;

    const QRectF rect = sceneBoundingRect();
    bool isBounded = shared::graphicsviewutils::isBounded(this, rect);
    bool noCollision = !shared::graphicsviewutils::isCollided(this, rect);
    if (isBounded && noCollision) {
        layoutInterfaces();
        layoutConnectionsOnResize(shared::ui::VEConnectionGraphicsItem::CollisionsPolicy::PartialRebuild);
        updateEntity();
    } else { // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
        for (auto child : childItems()) {
            if (auto iface = qobject_cast<VEConnectionEndPointGraphicsItem *>(child->toGraphicsObject())) {
                iface->updateFromEntity();
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

QRectF VERectGraphicsItem::transformedRect(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QPointF shift = QPointF(to - from);
    QRectF rect = sceneBoundingRect();
    if (shift.isNull()) {
        return rect;
    }
    if (!grip) // A move operation
    {
        return rect.translated(shift);
    }

    if (!grip->isUnderMouse())
    {
        return rect;
    }

    QSizeF minSize = minimumSize();

    switch (grip->location()) {
    case GripPoint::Left:
    {
        auto newRect = rect.adjusted(shift.x(), 0, 0, 0);
        if (newRect.width() < minSize.width())
        {
            rect.setWidth(minSize.width());
        }
        else
        {
            rect = newRect;
        }

    } break;
    case GripPoint::Top: {
        const qreal top = rect.top() + shift.y();
        rect.setTop(top);
    } break;
    case GripPoint::Right: {
        const qreal right = rect.right() + shift.x();
        rect.setRight(right);
    } break;
    case GripPoint::Bottom: {
        const qreal bottom = rect.bottom() + shift.y();
        rect.setBottom(bottom);
    } break;
    case GripPoint::TopLeft: {
        const QPointF topLeft = rect.topLeft() + shift;
        rect.setTopLeft(topLeft);
    } break;
    case GripPoint::TopRight: {
        const QPointF topRight = rect.topRight() + shift;
        rect.setTopRight(topRight);
    } break;
    case GripPoint::BottomLeft: {
        const QPointF bottomLeft = rect.bottomLeft() + shift;
        rect.setBottomLeft(bottomLeft);
    } break;
    case GripPoint::BottomRight: {
        const QPointF bottomRight = rect.bottomRight() + shift;
        rect.setBottomRight(bottomRight);
    } break;
    case GripPoint::Center: {
        rect.translate(shift);
    } break;
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    return rect.normalized();
}

QRectF VERectGraphicsItem::resizedRect(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QPointF shift = QPointF(to - from);
    QRectF rect = sceneBoundingRect();
    if (shift.isNull()) {
        return rect;
    }

    QSizeF minSize = minimumSize();

    switch (grip->location()) {
    case GripPoint::Left:
    {
        auto newRect = rect.adjusted(shift.x(), 0, 0, 0);
        if (newRect.width() < minSize.width())
        {
            rect.setWidth(minSize.width());
        }
        else
        {
            rect = newRect;
        }

    } break;
    case GripPoint::Top: {
        const qreal top = rect.top() + shift.y();
        rect.setTop(top);
    } break;
    case GripPoint::Right: {
        const qreal right = rect.right() + shift.x();
        rect.setRight(right);
    } break;
    case GripPoint::Bottom: {
        const qreal bottom = rect.bottom() + shift.y();
        rect.setBottom(bottom);
    } break;
    case GripPoint::TopLeft: {
        const QPointF topLeft = rect.topLeft() + shift;
        rect.setTopLeft(topLeft);
    } break;
    case GripPoint::TopRight: {
        const QPointF topRight = rect.topRight() + shift;
        rect.setTopRight(topRight);
    } break;
    case GripPoint::BottomLeft: {
        const QPointF bottomLeft = rect.bottomLeft() + shift;
        rect.setBottomLeft(bottomLeft);
    } break;
    case GripPoint::BottomRight: {
        const QPointF bottomRight = rect.bottomRight() + shift;
        rect.setBottomRight(bottomRight);
    } break;
    case GripPoint::Center: {
        rect.translate(shift);
    } break;
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    // Check rect for collisions

    return rect.normalized();
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
    if (layoutShouldBeChanged())
    {
        const auto parentFunction = qobject_cast<VERectGraphicsItem *>(parentObject());

        QRectF boundedRect;
        QMarginsF margins;
        if (parentFunction)
        {
            boundedRect = parentFunction->sceneBoundingRect();
            margins = graphicsviewutils::kContentMargins;
        }
        else
        {
            boundedRect = scene()->itemsBoundingRect();
            margins = graphicsviewutils::kRootMargins;
        }

        QRectF itemRect = QRectF(QPointF(0, 0), minimumSize());
        itemRect.moveTopLeft(boundedRect.topLeft());

        graphicsviewutils::findGeometryForRect(itemRect, boundedRect, graphicsviewutils::siblingItemsRects(this), margins);
        if (itemRect != sceneBoundingRect())
        {
            setRect(itemRect);
            return true;
        }
    }
    return false;
}

bool VERectGraphicsItem::layoutShouldBeChanged() const
{
    const QRectF currentRect = sceneBoundingRect();
    if (!currentRect.isValid())
        return true;

    return graphicsviewutils::isCollided(this, currentRect) || !graphicsviewutils::isBounded(this, currentRect);
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
