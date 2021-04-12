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

#include "aadlfunctiontype.h"
#include "aadlobject.h"
#include "baseitems/common/aadlutils.h"
#include "commandsstack.h"
#include "interface/graphicsitemhelpers.h"
#include "ui/grippointshandler.h"

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QtDebug>

namespace ive {

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

void AADLRectGraphicsItem::updateFromEntity()
{
    ivm::AADLObject *obj = aadlObject();
    Q_ASSERT(obj);
    if (!obj)
        return;

    const QRectF itemSceneRect { ive::rect(obj->coordinates()) };
    if (!itemSceneRect.isValid())
        layout();
    else
        setRect(itemSceneRect);

    if (itemNeedsToBeRelayout())
        layout();
}

QList<QPair<ivm::AADLObject *, QVector<QPointF>>> AADLRectGraphicsItem::prepareChangeCoordinatesCommandParams() const
{
    QList<QPair<ivm::AADLObject *, QVector<QPointF>>> params;
    const QVector<QPointF> sceneGeometry { sceneBoundingRect().topLeft(), sceneBoundingRect().bottomRight() };
    params.append({ aadlObject(), sceneGeometry });
    params.append(InteractiveObject::prepareChangeCoordinatesCommandParams());
    return params;
}

void AADLRectGraphicsItem::rebuildLayout()
{
    InteractiveObject::rebuildLayout();

    updateGripPoints();
    applyColorScheme();
}

void AADLRectGraphicsItem::onManualMoveProgress(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    handleGeometryChanging(grip, pressedAt, releasedAt);
}

void AADLRectGraphicsItem::onManualResizeProgress(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    handleGeometryChanging(grip, pressedAt, releasedAt);
}

void AADLRectGraphicsItem::onManualResizeFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    handleGeometryChanged(grip, pressedAt, releasedAt);
}

void AADLRectGraphicsItem::onManualMoveFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    handleGeometryChanged(grip, pressedAt, releasedAt);
}

QRectF AADLRectGraphicsItem::transformedRect(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QPointF shift = QPointF(to - from);
    QRectF rect = sceneBoundingRect();
    if (shift.isNull()) {
        return rect;
    }
    if (!grip) {
        return rect.translated(shift);
    }

    switch (grip->location()) {
    case shared::ui::GripPoint::Left: {
        const qreal left = rect.left() + shift.x();
        rect.setLeft(left);
    } break;
    case shared::ui::GripPoint::Top: {
        const qreal top = rect.top() + shift.y();
        rect.setTop(top);
    } break;
    case shared::ui::GripPoint::Right: {
        const qreal right = rect.right() + shift.x();
        rect.setRight(right);
    } break;
    case shared::ui::GripPoint::Bottom: {
        const qreal bottom = rect.bottom() + shift.y();
        rect.setBottom(bottom);
    } break;
    case shared::ui::GripPoint::TopLeft: {
        const QPointF topLeft = rect.topLeft() + shift;
        rect.setTopLeft(topLeft);
    } break;
    case shared::ui::GripPoint::TopRight: {
        const QPointF topRight = rect.topRight() + shift;
        rect.setTopRight(topRight);
    } break;
    case shared::ui::GripPoint::BottomLeft: {
        const QPointF bottomLeft = rect.bottomLeft() + shift;
        rect.setBottomLeft(bottomLeft);
    } break;
    case shared::ui::GripPoint::BottomRight: {
        const QPointF bottomRight = rect.bottomRight() + shift;
        rect.setBottomRight(bottomRight);
    } break;
    case shared::ui::GripPoint::Center: {
        rect.translate(shift);
    } break;
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    return rect.normalized();
}

void AADLRectGraphicsItem::handleGeometryChanged(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (pressedAt == releasedAt)
        return;

    if (gi::isBounded(this, sceneBoundingRect()) && !gi::isCollided(this, sceneBoundingRect()))
        updateEntity();
    else // Fallback to previous geometry in case colliding with items at the same level
        updateFromEntity();
}

void AADLRectGraphicsItem::handleGeometryChanging(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt)
        return;

    const QRectF rect = transformedRect(grip, pressedAt, releasedAt);
    if (rect.width() >= minimalSize().width() && rect.height() >= minimalSize().height() && gi::isBounded(this, rect)) {
        setRect(rect);
    }
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
    for (const QGraphicsItem *item : childItems()) {
        if (gi::rectangularTypes().contains(item->type())) {
            const QRectF nestedRect = item->sceneBoundingRect();
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

    if (auto grip = gripPointItem(shared::ui::GripPoint::Location::Center)) {
        const QPointF pressedAt = sceneBoundingRect().center();
        const QPointF releasedAt = pressedAt + shift;

        onManualMoveStart(grip, pressedAt);
        onManualMoveProgress(grip, pressedAt, releasedAt);
        onManualMoveFinish(grip, pressedAt, releasedAt);
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
}

void AADLRectGraphicsItem::layout()
{
    if (itemNeedsToBeRelayout()) {
        const auto parentFunction = qobject_cast<AADLRectGraphicsItem *>(parentObject());
        QRectF boundedRect =
                QRectF(parentFunction ? parentFunction->sceneBoundingRect() : scene()->itemsBoundingRect());
        QRectF itemRect = QRectF(QPointF(0, 0), DefaultGraphicsItemSize);
        itemRect.moveTopLeft(boundedRect.topLeft());
        findGeometryForRect(itemRect, boundedRect, siblingItemsRects(this, gi::rectangularTypes()), kContentMargins);
        setRect(itemRect);
        mergeGeometry();
    }
}

bool AADLRectGraphicsItem::itemNeedsToBeRelayout() const
{
    const QRectF currentRect = sceneBoundingRect();
    if (!currentRect.isValid())
        return true;

    return gi::isCollided(this, currentRect) || !gi::isBounded(this, currentRect);
}

}
