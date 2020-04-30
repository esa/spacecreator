/*
 Copyright (C) 2018-2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlrectgraphicsitem.h"

#include "commandsstack.h"
#include "baseitems/common/utils.h"
#include "baseitems/grippointshandler.h"
#include "aadlobject.h"
#include "interface/graphicsitemhelpers.h"

#include <QKeyEvent>
#include <QtDebug>

namespace aadlinterface {

static const qreal kOffset = 2.0;

AADLRectGraphicsItem::AADLRectGraphicsItem(aadl::AADLObject *entity, QGraphicsItem *parentGraphicsItem)
    : InteractiveObject(entity, parentGraphicsItem)
{
    connect(this, &InteractiveObject::relocated, this, &AADLRectGraphicsItem::onGeometryChanged);
    connect(this, &InteractiveObject::boundingBoxChanged, this, &AADLRectGraphicsItem::onGeometryChanged);
    setHighlightable(true);
}

QSizeF AADLRectGraphicsItem::minimalSize() const
{
    return QSizeF();
}

static QPointF shiftChildren(const QRectF &sourceRect, const QRectF &destRect)
{
    if (!sourceRect.isValid() || !destRect.isValid())
        return {};

    const QPointF shift = sourceRect.topLeft() - destRect.topLeft();
    if (destRect.translated(shift) == sourceRect)
        return {};

    return shift;
}

bool AADLRectGraphicsItem::setGeometry(const QRectF &sceneGeometry)
{
    if (sceneGeometry == sceneBoundingRect())
        return false;

    /// Silent geometry change without relayouting
    const QPointF shift = shiftChildren(sceneBoundingRect(), sceneGeometry);
    setPos(mapToParent(mapFromScene(sceneGeometry.topLeft())));
    if (!shift.isNull()) {
        for (QGraphicsItem *child : childItems()) {
            if (auto iObj = qobject_cast<AADLRectGraphicsItem *>(child->toGraphicsObject()))
                iObj->moveBy(shift.x(), shift.y());
        }
    }

    if (m_boundingRect.size() != sceneGeometry.size()) {
        prepareGeometryChange();
        m_boundingRect.setSize(sceneGeometry.size());
        Q_EMIT boundingBoxChanged();
    }

    updateGripPoints();
    return true;
}

void AADLRectGraphicsItem::setRect(const QRectF &geometry)
{
    if (setGeometry(geometry))
        instantLayoutUpdate();
}

void AADLRectGraphicsItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    const GripPoint::Locations locations {
        GripPoint::Location::Top, GripPoint::Location::Left,
        GripPoint::Location::Bottom, GripPoint::Location::Right,
        GripPoint::Location::TopLeft, GripPoint::Location::BottomLeft,
        GripPoint::Location::TopRight, GripPoint::Location::BottomRight };
    for (auto location : locations)
        m_gripPointsHandler->createGripPoint(location);
}

QRectF AADLRectGraphicsItem::adjustRectToParent(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QPointF shift = QPointF(to - from);
    QRectF rect = mapRectToParent(boundingRect());

    auto parentObj = qobject_cast<InteractiveObject *>(parentObject());
    const QRectF contentRect = parentObj
            ? parentObj->boundingRect().marginsRemoved(parentObj->aadlObject()->isRootObject() ? kRootMargins : kContentMargins)
            : QRectF();
    switch (grip->location()) {
    case GripPoint::Left: {
        const qreal left = rect.left() + shift.x();
        if (contentRect.isNull() || left >= contentRect.left())
            rect.setLeft(left);
    } break;
    case GripPoint::Top: {
        const qreal top = rect.top() + shift.y();
        if (contentRect.isNull() || top >= contentRect.top())
            rect.setTop(top);
    } break;
    case GripPoint::Right: {
        const qreal right = rect.right() + shift.x();
        if (contentRect.isNull() || right <= contentRect.right())
            rect.setRight(right);
    } break;
    case GripPoint::Bottom: {
        const qreal bottom = rect.bottom() + shift.y();
        if (contentRect.isNull() || bottom <= contentRect.bottom())
            rect.setBottom(bottom);
    } break;
    case GripPoint::TopLeft: {
        const QPointF topLeft = rect.topLeft() + shift;
        if (contentRect.isNull() || contentRect.contains(topLeft))
            rect.setTopLeft(topLeft);
    } break;
    case GripPoint::TopRight: {
        const QPointF topRight = rect.topRight() + shift;
        if (contentRect.isNull() || contentRect.contains(topRight))
            rect.setTopRight(topRight);
    } break;
    case GripPoint::BottomLeft: {
        const QPointF bottomLeft = rect.bottomLeft() + shift;
        if (contentRect.isNull() || contentRect.contains(bottomLeft))
            rect.setBottomLeft(bottomLeft);
    } break;
    case GripPoint::BottomRight: {
        const QPointF bottomRight = rect.bottomRight() + shift;
        if (contentRect.isNull() || contentRect.contains(bottomRight))
            rect.setBottomRight(bottomRight);
    } break;
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    return rect.normalized();
}

void AADLRectGraphicsItem::updateFromEntity()
{
    aadl::AADLObject *obj = aadlObject();
    Q_ASSERT(obj);
    if (!obj)
        return;

    const QRectF itemSceneRect { rect(obj->coordinates()) };
    if (!itemSceneRect.isValid())
        instantLayoutUpdate();
    else
        setRect(itemSceneRect);

    if (itemNeedsToBeRelayout())
        layout();
}

QList<QVariantList> AADLRectGraphicsItem::prepareChangeCoordinatesCommandParams() const
{
    QList<QVariantList> params;
    const QVector<QPointF> sceneGeometry { sceneBoundingRect().topLeft(), sceneBoundingRect().bottomRight() };
    params.append({ QVariant::fromValue(aadlObject()), QVariant::fromValue(sceneGeometry) });
    params.append(InteractiveObject::prepareChangeCoordinatesCommandParams());
    return params;
}

void AADLRectGraphicsItem::doRebuildLayout()
{
    const QRectF sceneRect = sceneBoundingRect();
    if (auto graphicsItemParent = parentItem()) {
        const QRectF parentRect = graphicsItemParent->sceneBoundingRect();
        setVisible(parentRect.contains(sceneRect));
    }
    for (auto child : childItems()) {
        if (auto rectItem = qobject_cast<AADLRectGraphicsItem *>(child->toGraphicsObject()))
            rectItem->setVisible(sceneRect.contains(rectItem->sceneBoundingRect()));
    }
    updateGripPoints();
    applyColorScheme();
}

bool AADLRectGraphicsItem::allowGeometryChange(const QPointF &from, const QPointF &to)
{
    const QPointF delta { to - from };
    if (delta.isNull())
        return false;

    const QRectF currentBounds = sceneBoundingRect().marginsAdded(kContentMargins);
    return gi::canPlaceRect(scene(), this, currentBounds, gi::RectOperation::Edit);
}

void AADLRectGraphicsItem::onManualMoveProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(grip)

    if (!scene())
        return;

    const QPointF shift = QPointF(to - from);
    QPointF newPos = scenePos() + shift;
    if (auto parentObj = qobject_cast<InteractiveObject *>(parentObject())) {
        const QRectF contentRect = parentObj->sceneBoundingRect().marginsRemoved(
                parentObj->aadlObject()->isRootObject() ? kRootMargins : kContentMargins);

        if (newPos.x() < contentRect.left())
            newPos.setX(contentRect.left());
        else if ((newPos.x() + m_boundingRect.width()) > contentRect.right())
            newPos.setX(contentRect.right() - m_boundingRect.width());

        if (newPos.y() < contentRect.top())
            newPos.setY(contentRect.top());
        else if ((newPos.y() + m_boundingRect.height()) > contentRect.bottom())
            newPos.setY(contentRect.bottom() - m_boundingRect.height());
    }
    setRect(QRectF(newPos, m_boundingRect.size()));
}

void AADLRectGraphicsItem::onManualResizeProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QRectF rect = adjustRectToParent(grip, from, to);
    if (rect.width() >= minimalSize().width() && rect.height() >= minimalSize().height())
        setRect(mapRectToScene(mapRectFromParent(rect)));
}

void AADLRectGraphicsItem::onManualResizeFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)
    handleGeometryChange(pressedAt, releasedAt);
}

void AADLRectGraphicsItem::onManualMoveFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)
    handleGeometryChange(pressedAt, releasedAt);
}

void AADLRectGraphicsItem::handleGeometryChange(const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt)
        return;

    if (allowGeometryChange(pressedAt, releasedAt))
        updateEntity();
    else // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
}

void AADLRectGraphicsItem::singleStepMove(MoveStep direction)
{
    static constexpr qreal delta = 1.;

    QPointF shift;
    switch (direction) {
    case MoveStep::Left: {
        shift.setX(-delta);
        break;
    }
    case Right: {
        shift.setX(delta);
        break;
    }
    case MoveStep::Up: {
        shift.setY(-delta);
        break;
    }
    case MoveStep::Down: {
        shift.setY(delta);
        break;
    }
    }

    shiftBy(shift);
}

QRectF AADLRectGraphicsItem::nestedItemsSceneBoundingRect() const
{
    QRectF nestedItemsBoundingRect;
    for (auto item : childItems()) {
        if (auto iObj = qobject_cast<AADLRectGraphicsItem *>(item->toGraphicsObject())) {
            const QRectF nestedRect = iObj->sceneBoundingRect();
            if (nestedRect.isValid())
                nestedItemsBoundingRect |= nestedRect;
        }
    }
    return nestedItemsBoundingRect;
}

void AADLRectGraphicsItem::shiftBy(const QPointF &shift)
{
    if (shift.isNull())
        return;

    const QPointF pressedAt = sceneBoundingRect().center();
    const QPointF releasedAt = pressedAt + shift;

    onManualMoveStart(nullptr, pressedAt);
    onManualMoveProgress(nullptr, pressedAt, releasedAt);
    onManualMoveFinish(nullptr, pressedAt, releasedAt);
}

void AADLRectGraphicsItem::onGeometryChanged()
{
    QSet<InteractiveObject *> items;
    QList<QGraphicsItem *> collidedItems = scene()->items(sceneBoundingRect().marginsAdded(kContentMargins));
    std::for_each(collidedItems.begin(), collidedItems.end(), [this, &items](QGraphicsItem *item) {
        auto rectItem = qobject_cast<AADLRectGraphicsItem *>(item->toGraphicsObject());
        if (rectItem && item != this && item->parentItem() == parentItem())
            items.insert(rectItem);
    });
    QSet<InteractiveObject *> newItems(items);
    newItems.subtract(m_collidedItems);
    for (auto item : newItems)
        item->doHighlighting(Qt::red, true);

    QSet<InteractiveObject *> oldItems(m_collidedItems);
    oldItems.subtract(items);

    for (auto item : oldItems)
        item->doHighlighting(Qt::green, false);

    m_collidedItems = items;
}

static inline QRectF collidingItemsBoundingRect(QGraphicsItem *item, const QRectF &rect)
{
    QList<QGraphicsItem *> collidingItems = item->scene()->items(rect);
    if (collidingItems.isEmpty())
        return {};

    QRectF br;
    for (auto collidingItem : collidingItems) {
        if (auto rectItem = qobject_cast<AADLRectGraphicsItem *>(collidingItem->toGraphicsObject())) {
            if (rectItem->parentItem() == item->parentItem() && rectItem != item) {
                br |= rectItem->sceneBoundingRect();
            }
        }
    }

    return br;
}

static inline void findGeometryForItem(QGraphicsItem *item, QRectF &itemRect, QRectF &boundedRect)
{
    const QRectF collidedRect = collidingItemsBoundingRect(item, itemRect);
    if (!collidedRect.isValid())
        return;

    if (boundedRect.right() - collidedRect.right() > itemRect.width()) {
        itemRect.moveLeft(collidedRect.right() + kOffset);
    } else if (boundedRect.bottom() - collidedRect.bottom() > itemRect.height()) {
        itemRect.moveLeft(boundedRect.left());
        itemRect.moveTop(collidedRect.bottom() + kOffset);
    } else if (boundedRect.width() <= boundedRect.height()) {
        itemRect.moveLeft(collidedRect.right() + kOffset);
        boundedRect.setRight(itemRect.right());
    } else {
        itemRect.moveLeft(boundedRect.left());
        itemRect.moveTop(collidedRect.bottom() + kOffset);
        boundedRect.setBottom(itemRect.bottom());
    }

    return findGeometryForItem(item, itemRect, boundedRect);
}

void AADLRectGraphicsItem::layout()
{
    const QRectF currentSceneRect = sceneBoundingRect();
    if (currentSceneRect.isValid()) {
        const QRectF collidingRect = collidingItemsBoundingRect(this, currentSceneRect);
        if (!collidingRect.isValid())
            return;
    }

    auto parentFunction = qobject_cast<AADLRectGraphicsItem *>(parentObject());
    const QMarginsF kCurrentMargins = parentFunction && parentFunction->aadlObject()->isRootObject() ? kRootMargins : kContentMargins;

    QRectF boundedRect = QRectF(parentFunction ? parentFunction->sceneBoundingRect().marginsRemoved(kCurrentMargins)
                                               : scene()->itemsBoundingRect());
    QRectF itemRect = QRectF(QPointF(0, 0), DefaultGraphicsItemSize).marginsAdded(kContentMargins);
    itemRect.moveTopLeft(boundedRect.topLeft());

    findGeometryForItem(this, itemRect, boundedRect);
    setRect(itemRect.marginsRemoved(kContentMargins));
    mergeGeometry();
}

bool AADLRectGraphicsItem::itemNeedsToBeRelayout() const
{
    const QRectF currentRect = sceneBoundingRect();
    if (!currentRect.isValid())
        return true;

    const QRectF currentExpandedRect = currentRect.marginsAdded(kContentMargins);
    for (auto item : scene()->items(currentExpandedRect)) {
        if (item == this || item->parentItem() != parentItem())
            continue;

        if (auto iObj = qobject_cast<AADLRectGraphicsItem *>(item->toGraphicsObject())) {
            const QRectF itemRect = iObj->sceneBoundingRect();
            if (!itemRect.isValid())
                continue;

            if (currentExpandedRect.intersects(itemRect))
                return true;
        }
    }

    return false;
}

}
