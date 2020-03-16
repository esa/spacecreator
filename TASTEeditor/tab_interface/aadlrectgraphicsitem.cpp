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

#include "app/commandsstack.h"
#include "baseitems/common/utils.h"
#include "baseitems/grippointshandler.h"
#include "tab_aadl/aadlobject.h"

#include <QKeyEvent>
#include <QtDebug>

namespace taste3 {
namespace aadl {

AADLRectGraphicsItem::AADLRectGraphicsItem(AADLObject *entity, QGraphicsItem *parentItem)
    : InteractiveObject(entity, parentItem)
{
    connect(this, &InteractiveObject::relocated, this, &AADLRectGraphicsItem::onGeometryChanged);
    connect(this, &InteractiveObject::boundingBoxChanged, this, &AADLRectGraphicsItem::onGeometryChanged);
    setHighlightable(true);
}

QSizeF AADLRectGraphicsItem::minimalSize() const
{
    return QSizeF();
}

void AADLRectGraphicsItem::setRect(const QRectF &geometry)
{
    const QPointF geometryPos = parentItem() ? parentItem()->mapFromScene(geometry.topLeft()) : geometry.topLeft();
    setPos(geometryPos);
    prepareGeometryChange();
    m_boundingRect = { QPointF(0, 0), geometry.size() };
    instantLayoutUpdate();
    updateGripPoints();
}

void AADLRectGraphicsItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    static const GripPoint::Locations locations { GripPoint::Location::Top,      GripPoint::Location::Left,
                                                  GripPoint::Location::Bottom,   GripPoint::Location::Right,
                                                  GripPoint::Location::TopLeft,  GripPoint::Location::BottomLeft,
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
            ? parentObj->boundingRect().marginsRemoved(parentObj->aadlObject()->isRootObject() ? utils::kRootMargins
                                                                                               : utils::kContentMargins)
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

    const QRectF itemSceneRect { utils::rect(obj->coordinates()) };
    if (!itemSceneRect.isValid())
        instantLayoutUpdate();
    else
        setRect(itemSceneRect);

    updateGripPoints();
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
    if (auto graphicsItemParent = parentItem()) {
        const QRectF parentRect = graphicsItemParent->sceneBoundingRect();
        setVisible(parentRect.contains(sceneBoundingRect()));
    }
    InteractiveObject::rebuildLayout();
    updateGripPoints();
}

bool AADLRectGraphicsItem::allowGeometryChange(const QPointF &from, const QPointF &to)
{
    const QPointF delta { to - from };
    if (delta.isNull())
        return false;

    const QList<QGraphicsItem *> collidedItems =
            scene()->items(sceneBoundingRect().marginsAdded(2 * utils::kContentMargins));
    auto it = std::find_if(collidedItems.constBegin(), collidedItems.constEnd(), [this](const QGraphicsItem *item) {
        return qobject_cast<const AADLRectGraphicsItem *>(item->toGraphicsObject()) && item != this
                && item->parentItem() == parentItem();
    });
    // Fallback to previous geometry in case colliding with items at the same level
    if (it != collidedItems.constEnd()) {
        updateFromEntity();
        return false;
    }

    return true;
}

void AADLRectGraphicsItem::onManualMoveProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(grip)
    Q_UNUSED(from)

    if (!scene() || m_clickPos.isNull())
        return;

    QPointF newPos = mapToParent(mapFromScene(to) - m_clickPos);
    if (auto parentObj = qobject_cast<InteractiveObject *>(parentObject())) {
        const QRectF contentRect = parentObj->boundingRect().marginsRemoved(
                parentObj->aadlObject()->isRootObject() ? utils::kRootMargins : utils::kContentMargins);

        if (newPos.x() < contentRect.left())
            newPos.setX(contentRect.left());
        else if ((newPos.x() + m_boundingRect.width()) > contentRect.right())
            newPos.setX(contentRect.right() - m_boundingRect.width());

        if (newPos.y() < contentRect.top())
            newPos.setY(contentRect.top());
        else if ((newPos.y() + m_boundingRect.height()) > contentRect.bottom())
            newPos.setY(contentRect.bottom() - m_boundingRect.height());
    }
    setPos(newPos);

    //    rebuildLayout();
    updateGripPoints();

    //    Q_EMIT needUpdateLayout();
}

void AADLRectGraphicsItem::onManualResizeProgress(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    const QRectF rect = adjustRectToParent(grip, from, to);
    if (rect.width() >= minimalSize().width() && rect.height() >= minimalSize().height())
        setRect(parentItem() ? parentItem()->mapRectToScene(rect) : rect);

    //    rebuildLayout();
    updateGripPoints();

    //    Q_EMIT needUpdateLayout();
}

void AADLRectGraphicsItem::onManualResizeFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (allowGeometryChange(pressedAt, releasedAt))
        updateEntity();
    else
        updateFromEntity();
}

void AADLRectGraphicsItem::onManualMoveFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt)
{
    Q_UNUSED(grip)

    if (allowGeometryChange(pressedAt, releasedAt))
        updateEntity();
    else
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
    default: {
        return;
    }
    }

    shiftBy(shift);
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
    QList<QGraphicsItem *> collidedItems = scene()->items(sceneBoundingRect().marginsAdded(2 * utils::kContentMargins));
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

} // namespace aadl
} // namespace taste3
