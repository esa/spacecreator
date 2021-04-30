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

#include "ivrectgraphicsitem.h"

#include "baseitems/common/ivutils.h"
#include "commandsstack.h"
#include "interface/graphicsitemhelpers.h"
#include "ivfunctiontype.h"
#include "ivobject.h"
#include "ui/grippointshandler.h"

#include <QGraphicsScene>
#include <QtDebug>

namespace ive {

IVRectGraphicsItem::IVRectGraphicsItem(ivm::IVObject *entity, QGraphicsItem *parentGraphicsItem)
    : InteractiveObject(entity, parentGraphicsItem)
{
    connect(this, &InteractiveObject::relocated, this, &IVRectGraphicsItem::onGeometryChanged);
    connect(this, &InteractiveObject::boundingBoxChanged, this, &IVRectGraphicsItem::onGeometryChanged);
    setHighlightable(true);
}

QSizeF IVRectGraphicsItem::minimalSize() const
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

bool IVRectGraphicsItem::setGeometry(const QRectF &sceneGeometry)
{
    if (sceneGeometry == sceneBoundingRect())
        return false;

    /// Silent geometry change without relayouting
    const QPointF shift = shiftChildren(sceneBoundingRect(), sceneGeometry);
    setPos(mapToParent(mapFromScene(sceneGeometry.topLeft())));
    if (!shift.isNull()) {
        for (QGraphicsItem *child : childItems()) {
            if (auto iObj = qobject_cast<IVRectGraphicsItem *>(child->toGraphicsObject()))
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

void IVRectGraphicsItem::setRect(const QRectF &geometry)
{
    if (setGeometry(geometry))
        instantLayoutUpdate();
}

void IVRectGraphicsItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    gripPointsHandler()->setUsedPoints({ shared::ui::GripPoint::Location::Top, shared::ui::GripPoint::Location::Left,
            shared::ui::GripPoint::Location::Bottom, shared::ui::GripPoint::Location::Right,
            shared::ui::GripPoint::Location::TopLeft, shared::ui::GripPoint::Location::BottomLeft,
            shared::ui::GripPoint::Location::TopRight, shared::ui::GripPoint::Location::BottomRight });
}

void IVRectGraphicsItem::updateFromEntity()
{
    ivm::IVObject *obj = entity();
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

QList<QPair<ivm::IVObject *, QVector<QPointF>>> IVRectGraphicsItem::prepareChangeCoordinatesCommandParams() const
{
    QList<QPair<ivm::IVObject *, QVector<QPointF>>> params;
    const QVector<QPointF> sceneGeometry { sceneBoundingRect().topLeft(), sceneBoundingRect().bottomRight() };
    params.append({ entity(), sceneGeometry });
    params.append(InteractiveObject::prepareChangeCoordinatesCommandParams());
    return params;
}

void IVRectGraphicsItem::rebuildLayout()
{
    InteractiveObject::rebuildLayout();

    updateGripPoints();
    applyColorScheme();
}

void IVRectGraphicsItem::onManualMoveProgress(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    handleGeometryChanging(grip, pressedAt, releasedAt);
}

void IVRectGraphicsItem::onManualResizeProgress(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    handleGeometryChanging(grip, pressedAt, releasedAt);
}

void IVRectGraphicsItem::onManualResizeFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    handleGeometryChanged(grip, pressedAt, releasedAt);
}

void IVRectGraphicsItem::onManualMoveFinish(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    handleGeometryChanged(grip, pressedAt, releasedAt);
}

QRectF IVRectGraphicsItem::transformedRect(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to)
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

void IVRectGraphicsItem::handleGeometryChanged(
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

void IVRectGraphicsItem::handleGeometryChanging(
        shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (pressedAt == releasedAt)
        return;

    const QRectF rect = transformedRect(grip, pressedAt, releasedAt);
    if (rect.width() >= minimalSize().width() && rect.height() >= minimalSize().height() && gi::isBounded(this, rect)) {
        setRect(rect);
    }
}

QRectF IVRectGraphicsItem::nestedItemsSceneBoundingRect() const
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

void IVRectGraphicsItem::onGeometryChanged()
{
    QSet<InteractiveObject *> items;
    QList<QGraphicsItem *> collidedItems = scene()->items(sceneBoundingRect().marginsAdded(kContentMargins));
    std::for_each(collidedItems.begin(), collidedItems.end(), [this, &items](QGraphicsItem *item) {
        auto rectItem = qobject_cast<IVRectGraphicsItem *>(item->toGraphicsObject());
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

void IVRectGraphicsItem::layout()
{
    if (itemNeedsToBeRelayout()) {
        const auto parentFunction = qobject_cast<IVRectGraphicsItem *>(parentObject());
        QRectF boundedRect =
                QRectF(parentFunction ? parentFunction->sceneBoundingRect() : scene()->itemsBoundingRect());
        QRectF itemRect = QRectF(QPointF(0, 0), DefaultGraphicsItemSize);
        itemRect.moveTopLeft(boundedRect.topLeft());
        findGeometryForRect(itemRect, boundedRect, siblingItemsRects(this, gi::rectangularTypes()), kContentMargins);
        setRect(itemRect);
        mergeGeometry();
    }
}

bool IVRectGraphicsItem::itemNeedsToBeRelayout() const
{
    const QRectF currentRect = sceneBoundingRect();
    if (!currentRect.isValid())
        return true;

    return gi::isCollided(this, currentRect) || !gi::isBounded(this, currentRect);
}

}
