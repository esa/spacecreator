/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "interactiveobject.h"
#include "grippointshandler.h"

#include <QGraphicsScene>
#include <QPainter>

namespace msc {

InteractiveObject::InteractiveObject(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_gripPoints(new GripPointsHandler(this))
{
    setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);

    connect(m_gripPoints, &GripPointsHandler::rectChanged, this,
            &InteractiveObject::gripPointMoved);

    setAcceptHoverEvents(true);

    m_gripPoints->setZValue(0);
}

void InteractiveObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF InteractiveObject::boundingRect() const
{
    return m_boundingRect;
}
void InteractiveObject::gripPointMoved(GripPoint::Location gripPos,
                                       const QPointF &from, const QPointF &to)
{
    if (GripPoint *gripPnt = m_gripPoints->gripPoint(gripPos))
        handleGripPointMovement(gripPnt, from, to);
}

void InteractiveObject::handleGripPointMovement(GripPoint *grip, const QPointF &from, const QPointF &to)
{

    const QRectF sourceBounds = m_boundingRect;
    const qreal minSideAllowed = m_gripPoints->minSize().width();
    QRectF targetBounds(sourceBounds);
    const QPointF delta(to - from);
    QPointF deltaCorrected(delta);

    auto validateRectMinSize = [&minSideAllowed](QRectF &r) {
        r.setHeight(qMax(r.height(), minSideAllowed));
        r.setWidth(qMax(r.width(), minSideAllowed));
        return r;
    };
    auto prepareResizeX = [&validateRectMinSize](QRectF &r, const QPointF &d,
                                                 bool left, bool right) {
        QRectF newBounds =
                r.adjusted(left ? -d.x() : 0., -d.y(), right ? d.x() : 0, d.y());
        return validateRectMinSize(newBounds);
    };

    auto prepareResizeY = [&validateRectMinSize](QRectF &r, const QPointF &d, bool top,
                                                 bool bottom) {
        QRectF newBounds =
                r.adjusted(-d.x(), top ? -d.y() : 0., d.x(), bottom ? d.y() : 0.);
        return validateRectMinSize(newBounds);
    };

    auto prepareResizeXAndY = [&prepareResizeX,
                               &prepareResizeY](QRectF &r, const QPointF &d) {
        QRectF newBounds = prepareResizeX(r, d, true, true);
        return prepareResizeY(newBounds, d, true, true);
    };

    auto prepareResizeXOrY = [&grip, &prepareResizeX,
                              &prepareResizeY](QRectF &r, const QPointF &d) {
        const bool isVertical = (grip->location() == GripPoint::GPP_Top || grip->location() == GripPoint::GPP_Bottom);
        const bool isHorizontal =
                !isVertical && (grip->location() == GripPoint::GPP_Right || grip->location() == GripPoint::GPP_Left);

        if (isVertical) {
            const bool isTop = grip->location() == GripPoint::GPP_Top;
            return prepareResizeY(r, d, isTop, !isTop);
        }

        if (isHorizontal) {
            const bool isLeft = grip->location() == GripPoint::GPP_Left;
            return prepareResizeX(r, d, isLeft, !isLeft);
        }

        return r;
    };

    auto justMoveMe = [this](const QPointF &d) { moveBy(d.x(), d.y()); };

    auto resizeInSingleDirection = [&grip, &targetBounds, &deltaCorrected, &prepareResizeXOrY, &prepareResizeXAndY]() {
        if (grip->location() == GripPoint::GPP_Top || grip->location() == GripPoint::GPP_Left)
            deltaCorrected *= -1.;

        if (grip->location() == GripPoint::GPP_Top || grip->location() == GripPoint::GPP_Bottom)
            deltaCorrected.rx() = 0;
        else if (grip->location() == GripPoint::GPP_Right || grip->location() == GripPoint::GPP_Left)
            deltaCorrected.ry() = 0;

        if (grip->isAnchor())
            targetBounds = prepareResizeXOrY(targetBounds, deltaCorrected);
        else
            targetBounds = prepareResizeXAndY(targetBounds, deltaCorrected);
    };

    switch (grip->location()) {
    case GripPoint::GPP_TopLeft:
    case GripPoint::GPP_TopRight:
    case GripPoint::GPP_BottomRight:
    case GripPoint::GPP_BottomLeft: {

        if (grip->isAnchor())
            resizeInSingleDirection();
        else {
            if (qFuzzyIsNull(deltaCorrected.x()))
                deltaCorrected.rx() = deltaCorrected.y();
            else
                deltaCorrected.ry() = deltaCorrected.x();

            if (GripPoint::GPP_BottomLeft == grip->location() || GripPoint::GPP_TopLeft == grip->location())
                deltaCorrected *= -1;

            targetBounds = prepareResizeXAndY(targetBounds, deltaCorrected);
        }

        break;
    }
    case GripPoint::GPP_Top:
    case GripPoint::GPP_Right:
    case GripPoint::GPP_Bottom:
    case GripPoint::GPP_Left: {
        resizeInSingleDirection();
        break;
    }
    case GripPoint::GPP_Center:
        justMoveMe(delta);
        Q_EMIT relocated(delta);
        break;
    }

    if (targetBounds != m_boundingRect) {
        const QRectF prevRect = m_boundingRect;
        prepareGeometryChange();
        m_boundingRect = targetBounds;
        m_gripPoints->updateLayout();
        update();

        Q_EMIT resized(prevRect, m_boundingRect);
    }
}

void InteractiveObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    m_storedZ = zValue();
    setZValue(m_storedZ + 1.);

    m_gripPoints->showAnimated();
    QGraphicsObject::hoverEnterEvent(event);
}

void InteractiveObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    setZValue(m_storedZ);
    m_gripPoints->hideAnimated();
    QGraphicsObject::hoverLeaveEvent(event);
}

} // namespace msc
