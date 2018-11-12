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
#include "common/highlightrectitem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <QBrush>

namespace msc {

/*!
  \class msc::InteractiveObject
  \brief Movable/resizable QGraphicsObject with geometry change notifications.

  \inmodule MscWidgets

*/

InteractiveObject::InteractiveObject(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_gripPoints(new GripPointsHandler(this))
    , m_highlighter(new HighlightRectItem(this))

{
    m_highlighter->setVisible(false);

    connect(m_gripPoints, &GripPointsHandler::rectChanged, this,
            &InteractiveObject::gripPointMoved);

    setAcceptHoverEvents(true);

    m_gripPoints->setZValue(0);

    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges);

    setCursor(Qt::ArrowCursor);
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
        const bool isVertical = (grip->location() == GripPoint::Top || grip->location() == GripPoint::Bottom);
        const bool isHorizontal =
                !isVertical && (grip->location() == GripPoint::Right || grip->location() == GripPoint::Left);

        if (isVertical) {
            const bool isTop = grip->location() == GripPoint::Top;
            return prepareResizeY(r, d, isTop, !isTop);
        }

        if (isHorizontal) {
            const bool isLeft = grip->location() == GripPoint::Left;
            return prepareResizeX(r, d, isLeft, !isLeft);
        }

        return r;
    };

    auto justMoveMe = [this](const QPointF &d) { moveBy(d.x(), d.y()); };

    auto resizeInSingleDirection = [&grip, &targetBounds, &deltaCorrected, &prepareResizeXOrY, &prepareResizeXAndY]() {
        if (grip->location() == GripPoint::Top || grip->location() == GripPoint::Left)
            deltaCorrected *= -1.;

        if (grip->location() == GripPoint::Top || grip->location() == GripPoint::Bottom)
            deltaCorrected.rx() = 0;
        else if (grip->location() == GripPoint::Right || grip->location() == GripPoint::Left)
            deltaCorrected.ry() = 0;

        if (grip->isMover())
            targetBounds = prepareResizeXOrY(targetBounds, deltaCorrected);
        else
            targetBounds = prepareResizeXAndY(targetBounds, deltaCorrected);
    };

    switch (grip->location()) {
    case GripPoint::TopLeft:
    case GripPoint::TopRight:
    case GripPoint::BottomRight:
    case GripPoint::BottomLeft: {

        if (grip->isMover())
            resizeInSingleDirection();
        else {
            if (qFuzzyIsNull(deltaCorrected.x()))
                deltaCorrected.rx() = deltaCorrected.y();
            else
                deltaCorrected.ry() = deltaCorrected.x();

            if (GripPoint::BottomLeft == grip->location() || GripPoint::TopLeft == grip->location())
                deltaCorrected *= -1;

            targetBounds = prepareResizeXAndY(targetBounds, deltaCorrected);
        }

        break;
    }
    case GripPoint::Top:
    case GripPoint::Right:
    case GripPoint::Bottom:
    case GripPoint::Left: {
        resizeInSingleDirection();
        break;
    }
    case GripPoint::Center:
        justMoveMe(delta);
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

QVariant InteractiveObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemPositionChange:
        m_prevPos = pos();
        break;
    case QGraphicsItem::ItemPositionHasChanged:
        Q_EMIT relocated(m_prevPos, pos());
        break;
    default:
        break;
    }

    return QGraphicsObject::itemChange(change, value);
}

bool InteractiveObject::isHovered() const
{
    if (!isUnderMouse())
        return false;

    return m_gripPoints->isVisible();
}

void InteractiveObject::highlightConnected()
{
    m_highlighter->setRect(m_boundingRect);
    QColor color(Qt::green);
    QPen p(color);
    p.setWidthF(3.);
    m_highlighter->setPen(p);
    color.setAlphaF(0.25);
    m_highlighter->setBrush(color);

    m_highlighter->highlight();
}

void InteractiveObject::highlightDisconnected()
{
    m_highlighter->setRect(m_boundingRect);
    QColor color(Qt::red);
    QPen p(color);
    p.setWidthF(3.);
    m_highlighter->setPen(p);
    color.setAlphaF(0.25);
    m_highlighter->setBrush(color);

    m_highlighter->highlight();
}

} // namespace msc
