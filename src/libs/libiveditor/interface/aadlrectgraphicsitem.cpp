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

#include "aadlobject.h"
#include "aadlfunctiontype.h"
#include "baseitems/common/aadlutils.h"
#include "commandsstack.h"
#include "interface/graphicsitemhelpers.h"
#include "ui/grippointshandler.h"

#include <QKeyEvent>
#include <QtDebug>

namespace ive {

static const qreal kOffset = 2.0;

AADLRectGraphicsItem::AADLRectGraphicsItem(ivm::AADLObject *entity, QGraphicsItem *parentGraphicsItem)
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

void AADLRectGraphicsItem::setRect(const QRectF &geometry)
{
    if (setGeometry(geometry))
        instantLayoutUpdate();
}

void AADLRectGraphicsItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    gripPointsHandler()->setUsedPoints({ shared::ui::GripPoint::Location::Top, shared::ui::GripPoint::Location::Left,
            shared::ui::GripPoint::Location::Bottom, shared::ui::GripPoint::Location::Right,
            shared::ui::GripPoint::Location::TopLeft, shared::ui::GripPoint::Location::BottomLeft,
            shared::ui::GripPoint::Location::TopRight, shared::ui::GripPoint::Location::BottomRight });
}

QRectF AADLRectGraphicsItem::adjustRectToParent(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QPointF shift = QPointF(to - from);
    QRectF rect = mapRectToParent(boundingRect());

    auto parentObj = qobject_cast<InteractiveObject *>(parentObject());
    const QRectF contentRect = parentObj ? parentObj->boundingRect().marginsRemoved(
                                       parentObj->aadlObject()->isRootObject() ? kRootMargins : kContentMargins)
                                         : QRectF();
    switch (grip->location()) {
    case shared::ui::GripPoint::Left: {
        const qreal left = rect.left() + shift.x();
        if (contentRect.isNull() || left >= contentRect.left())
            rect.setLeft(left);
    } break;
    case shared::ui::GripPoint::Top: {
        const qreal top = rect.top() + shift.y();
        if (contentRect.isNull() || top >= contentRect.top())
            rect.setTop(top);
    } break;
    case shared::ui::GripPoint::Right: {
        const qreal right = rect.right() + shift.x();
        if (contentRect.isNull() || right <= contentRect.right())
            rect.setRight(right);
    } break;
    case shared::ui::GripPoint::Bottom: {
        const qreal bottom = rect.bottom() + shift.y();
        if (contentRect.isNull() || bottom <= contentRect.bottom())
            rect.setBottom(bottom);
    } break;
    case shared::ui::GripPoint::TopLeft: {
        const QPointF topLeft = rect.topLeft() + shift;
        if (contentRect.isNull() || contentRect.contains(topLeft))
            rect.setTopLeft(topLeft);
    } break;
    case shared::ui::GripPoint::TopRight: {
        const QPointF topRight = rect.topRight() + shift;
        if (contentRect.isNull() || contentRect.contains(topRight))
            rect.setTopRight(topRight);
    } break;
    case shared::ui::GripPoint::BottomLeft: {
        const QPointF bottomLeft = rect.bottomLeft() + shift;
        if (contentRect.isNull() || contentRect.contains(bottomLeft))
            rect.setBottomLeft(bottomLeft);
    } break;
    case shared::ui::GripPoint::BottomRight: {
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
    ivm::AADLObject *obj = aadlObject();
    Q_ASSERT(obj);
    if (!obj)
        return;

    static const QList<ivm::meta::Props::Token> types { ivm::meta::Props::Token::coordinates,
        ivm::meta::Props::Token::InnerCoordinates, ivm::meta::Props::Token::RootCoordinates };

    QRectF itemSceneRect { ive::rect(obj->coordinates()) };
    int idx = 0;
    while (itemSceneRect.isNull() && idx < types.size()) {
        const ivm::meta::Props::Token token = types.at(idx);
        const QString strCoordinates = obj->prop(ivm::meta::Props::token(token)).toString();
        itemSceneRect = ive::rect(ivm::AADLObject::coordinatesFromString(strCoordinates));
        ++idx;
    }
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

void AADLRectGraphicsItem::rebuildLayout()
{
    const QRectF sceneRect = sceneBoundingRect();
    if (auto graphicsItemParent = parentItem()) {
        const QRectF parentRect = graphicsItemParent->sceneBoundingRect();
        setVisible(parentRect.contains(sceneRect) && aadlObject()->isVisible());
    } else {
        setVisible(aadlObject()->isVisible());
    }
    updateChildrenVisibility();
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

void AADLRectGraphicsItem::onManualMoveProgress(shared::ui::GripPoint *, const QPointF &from, const QPointF &to)
{
    if (!scene())
        return;

    const QPointF shift = QPointF(to - from);
    QPointF newPos = scenePos() + shift;
    if (auto parentObj = qobject_cast<InteractiveObject *>(parentObject())) {
        const QRectF contentRect = parentObj->sceneBoundingRect().marginsRemoved(
                parentObj->aadlObject()->isRootObject() ? kRootMargins : kContentMargins);

        if (newPos.x() < contentRect.left())
            newPos.setX(contentRect.left());
        else if ((newPos.x() + boundingRect().width()) > contentRect.right())
            newPos.setX(contentRect.right() - boundingRect().width());

        if (newPos.y() < contentRect.top())
            newPos.setY(contentRect.top());
        else if ((newPos.y() + boundingRect().height()) > contentRect.bottom())
            newPos.setY(contentRect.bottom() - boundingRect().height());
    }
    setRect(QRectF(newPos, boundingRect().size()));
}

void AADLRectGraphicsItem::onManualResizeProgress(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QRectF rect = adjustRectToParent(grip, from, to);
    if (rect.width() >= minimalSize().width() && rect.height() >= minimalSize().height())
        setRect(mapRectToScene(mapRectFromParent(rect)));
}

void AADLRectGraphicsItem::onManualResizeFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)
    handleGeometryChange(pressedAt, releasedAt);
}

void AADLRectGraphicsItem::onManualMoveFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
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

void AADLRectGraphicsItem::updateChildrenVisibility()
{
    const QRectF sceneRect = sceneBoundingRect();
    const bool visible = sceneRect.contains(nestedItemsSceneBoundingRect());
    for (auto child : childItems()) {
        if (auto rectItem = qobject_cast<AADLRectGraphicsItem *>(child->toGraphicsObject())) {
            rectItem->setVisible(visible && rectItem->aadlObject()->isVisible());
        }
    }
}

void AADLRectGraphicsItem::updateSiblingVisibility()
{
    if (auto parentRectObject = parentObject()) {
        if (auto parentRectItem = qobject_cast<AADLRectGraphicsItem *>(parentRectObject)) {
            parentRectItem->updateChildrenVisibility();
        }
    }
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
        item->doHighlighting(Qt::red, false);

    QSet<InteractiveObject *> oldItems(m_collidedItems);
    oldItems.subtract(items);

    for (auto item : oldItems)
        item->doHighlighting(Qt::green, false);

    m_collidedItems = items;

    if (m_collidedItems.isEmpty())
        clearHighlight();

    updateSiblingVisibility();
}

static inline QList<QRectF> siblingRectsForItem(
        QGraphicsItem *item, const QRectF &rect, const QMarginsF &margins = kContentMargins)
{
    QList<QGraphicsItem *> collidingItems = item->scene()->items(rect.marginsAdded(margins));
    if (collidingItems.isEmpty())
        return {};

    QList<QRectF> rects;
    for (auto collidingItem : collidingItems) {
        if (item == collidingItem || item->type() < QGraphicsItem::UserType)
            continue;

        if (auto rectItem = qobject_cast<AADLRectGraphicsItem *>(collidingItem->toGraphicsObject())) {
            if (rectItem->parentItem() == item->parentItem() && rectItem != item) {
                rects.append(rectItem->sceneBoundingRect());
            }
        }
    }

    return rects;
}

void AADLRectGraphicsItem::layout()
{
    const QRectF currentSceneRect = sceneBoundingRect();
    if (currentSceneRect.isValid() && siblingRectsForItem(this, currentSceneRect).isEmpty()) {
        return;
    }

    const auto parentFunction = qobject_cast<AADLRectGraphicsItem *>(parentObject());
    QRectF boundedRect = QRectF(parentFunction ? parentFunction->sceneBoundingRect() : scene()->itemsBoundingRect());
    QRectF itemRect = QRectF(QPointF(0, 0), DefaultGraphicsItemSize);
    itemRect.moveTopLeft(boundedRect.topLeft());
    findGeometryForRect(itemRect, boundedRect, siblingRectsForItem(this, boundedRect), kContentMargins);
    setRect(itemRect);
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
