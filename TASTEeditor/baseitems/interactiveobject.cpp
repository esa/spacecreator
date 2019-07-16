/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "baseitems/common/utils.h"
#include "common/highlightrectitem.h"
#include "grippointshandler.h"

#include <QBrush>
#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <functional>

namespace taste3 {

InteractiveObject::InteractiveObject(QObject *entity, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_entity(entity)
    , m_selectedPen(Qt::black, 4, Qt::DotLine)
{
    setAcceptHoverEvents(true);

    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges
             /*| QGraphicsItem::ItemIsMovable*/);

    setCursor(Qt::ArrowCursor);
}

QObject *InteractiveObject::modelEntity() const
{
    return m_entity;
}

void InteractiveObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (isSelected()) {
        painter->save();
        painter->setPen(m_selectedPen);

        painter->drawRect(m_boundingRect);
        painter->restore();
    }
}

QRectF InteractiveObject::boundingRect() const
{
    return m_boundingRect;
}

void InteractiveObject::gripPointPressed(GripPoint::Location gp, const QPointF &at)
{
    if (m_gripPoints) {
        if (GripPoint *gripPnt = m_gripPoints->gripPoint(gp)) {
            if (gripPnt->isMover())
                onManualMoveStart(gripPnt->location(), at);
            else
                onManualResizeStart(gripPnt->location(), at);
        }
    }
}

void InteractiveObject::gripPointMoved(GripPoint::Location gripPos, const QPointF &from, const QPointF &to)
{
    if (m_gripPoints) {
        if (GripPoint *gripPnt = m_gripPoints->gripPoint(gripPos)) {
            if (gripPnt->isMover())
                onManualMoveProgress(gripPnt->location(), from, to);
            else
                onManualResizeProgress(gripPnt->location(), from, to);
        }
    }
}

void InteractiveObject::gripPointReleased(GripPoint::Location gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (m_gripPoints) {
        if (GripPoint *gripPnt = m_gripPoints->gripPoint(gp)) {
            if (gripPnt->isMover())
                onManualMoveFinish(gripPnt->location(), pressedAt, releasedAt);
            else
                onManualResizeFinish(gripPnt->location(), pressedAt, releasedAt);
        }
    }
}

void InteractiveObject::rebuildLayout()
{
    for (auto item : childItems()) {
        if (auto iObj = dynamic_cast<InteractiveObject *>(item))
            iObj->instantLayoutUpdate();
    }
}

QFont InteractiveObject::font() const
{
    return m_font;
}

void InteractiveObject::setFont(const QFont &font)
{
    m_font = font;
}

QBrush InteractiveObject::brush() const
{
    return m_brush;
}

void InteractiveObject::setBrush(const QBrush &brush)
{
    m_brush = brush;
}

QPen InteractiveObject::pen() const
{
    return m_pen;
}

void InteractiveObject::setPen(const QPen &pen)
{
    m_pen = pen;
}

void InteractiveObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    m_storedZ = zValue();
    setZValue(m_storedZ + 1.);

    prepareHoverMark();
    updateGripPoints();
    QGraphicsObject::hoverEnterEvent(event);
}

void InteractiveObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    setZValue(m_storedZ);

    if (m_gripPoints)
        m_gripPoints->hideAnimated();

    QGraphicsObject::hoverLeaveEvent(event);
}

void InteractiveObject::onManualMoveStart(GripPoint::Location grip, const QPointF &at)
{
    Q_UNUSED(grip);
    Q_UNUSED(at);
}

void InteractiveObject::onManualMoveProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    if (grip != GripPoint::Location::Center)
        return;

    const QPointF delta { to - from };
    QPointF newPos = pos() + delta;
    if (parentItem()) {
        const QRectF contentRect = parentItem()->boundingRect();
        if (newPos.x() < contentRect.left())
            newPos.setX(contentRect.left());
        else if ((newPos.x() + m_boundingRect.width()) > contentRect.right())
            newPos.setX(contentRect.right() - m_boundingRect.width());
        if (newPos.y() < contentRect.top())
            newPos.setY(contentRect.top());
        else if ((newPos.y() + m_boundingRect.height()) > contentRect.bottom())
            newPos.setY(contentRect.bottom() - m_boundingRect.height());
    } else {
        const QRectF newGeometry { newPos, boundingRect().size() };
        const QList<QGraphicsItem *> collidedItems = scene()->items(newGeometry);
        auto it = std::find_if(collidedItems.constBegin(), collidedItems.constEnd(), [this](const QGraphicsItem *item) {
            return dynamic_cast<const InteractiveObject *>(item) && item != this && !item->parentItem();
        });
        if (it != collidedItems.constEnd())
            return;
    }
    setPos(newPos);

    rebuildLayout();
    updateGripPoints();

    Q_EMIT needUpdateLayout();
}

void InteractiveObject::onManualMoveFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                           const QPointF &releasedAt)
{
    Q_UNUSED(grip);
    Q_UNUSED(pressedAt);
    Q_UNUSED(releasedAt);
}

void InteractiveObject::onManualResizeStart(GripPoint::Location grip, const QPointF &at)
{
    Q_UNUSED(grip);
    Q_UNUSED(at);
}

void InteractiveObject::onManualResizeProgress(GripPoint::Location grip, const QPointF &from, const QPointF &to)
{
    const QPointF shift = QPointF(to - from);
    QRectF rect = mapRectToParent(boundingRect());
    switch (grip) {
    case GripPoint::Left: {
        const qreal left = rect.left() + shift.x();
        if (!parentItem() || left >= 0)
            rect.setLeft(left);
    } break;
    case GripPoint::Top: {
        const qreal top = rect.top() + shift.y();
        if (!parentItem() || top >= 0)
            rect.setTop(top);
    } break;
    case GripPoint::Right: {
        const qreal right = rect.right() + shift.x();
        if (!parentItem() || right <= parentItem()->boundingRect().right())
            rect.setRight(right);
    } break;
    case GripPoint::Bottom: {
        const qreal bottom = rect.bottom() + shift.y();
        if (!parentItem() || bottom <= parentItem()->boundingRect().bottom())
            rect.setBottom(bottom);
    } break;
    case GripPoint::TopLeft: {
        const QPointF topLeft = rect.topLeft() + shift;
        if (!parentItem() || parentItem()->boundingRect().contains(topLeft))
            rect.setTopLeft(topLeft);
    } break;
    case GripPoint::TopRight: {
        const QPointF topRight = rect.topRight() + shift;
        if (!parentItem() || parentItem()->boundingRect().contains(topRight))
            rect.setTopRight(topRight);
    } break;
    case GripPoint::BottomLeft: {
        const QPointF bottomLeft = rect.bottomLeft() + shift;
        if (!parentItem() || parentItem()->boundingRect().contains(bottomLeft))
            rect.setBottomLeft(bottomLeft);
    } break;
    case GripPoint::BottomRight: {
        const QPointF bottomRight = rect.bottomRight() + shift;
        if (!parentItem() || parentItem()->boundingRect().contains(bottomRight))
            rect.setBottomRight(bottomRight);
    } break;
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    if (!parentItem()) {
        const QList<QGraphicsItem *> collidedItems = scene()->items(rect.normalized());
        auto it = std::find_if(collidedItems.constBegin(), collidedItems.constEnd(), [this](const QGraphicsItem *item) {
            return dynamic_cast<const InteractiveObject *>(item) && item != this && !item->parentItem();
        });
        if (it != collidedItems.constEnd())
            return;
    }
    setRect(rect.normalized());

    rebuildLayout();
    updateGripPoints();

    Q_EMIT needUpdateLayout();
}

void InteractiveObject::onManualResizeFinish(GripPoint::Location grip, const QPointF &pressedAt,
                                             const QPointF &releasedAt)
{
    Q_UNUSED(grip);
    Q_UNUSED(pressedAt);
    Q_UNUSED(releasedAt);
}

void InteractiveObject::updateGripPoints()
{
    if (m_gripPoints)
        m_gripPoints->updateLayout();
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

    return m_gripPoints && m_gripPoints->isVisible();
}

HighlightRectItem *InteractiveObject::createHighlighter()
{
    if (m_highlighter)
        delete m_highlighter;
    m_highlighter = new HighlightRectItem(this);
    connect(m_highlighter, &HighlightRectItem::highlighted, m_highlighter, &QObject::deleteLater);
    connect(m_highlighter, &QObject::destroyed, this, [this]() { m_highlighter = nullptr; });

    m_highlighter->setRect(m_boundingRect);

    return m_highlighter;
}

bool InteractiveObject::isHighlightable() const
{
    return m_highlightable;
}

void InteractiveObject::setHighlightable(bool highlightable)
{
    m_highlightable = highlightable;
    clearHighlight();
}

void InteractiveObject::highlightConnected()
{
    doHighlighting(Qt::green, false);
}

void InteractiveObject::highlightDisconnected()
{
    doHighlighting(Qt::red, false);
}

void InteractiveObject::doHighlighting(const QColor &color, bool permanent)
{
    if (!m_highlightable) {
        return;
    }

    if (HighlightRectItem *highlighter = createHighlighter()) {
        QColor targetColor(color);
        QPen p(targetColor);
        p.setWidthF(1.);
        highlighter->setPen(p);
        targetColor.setAlphaF(0.25);
        highlighter->setBrush(targetColor);

        if (!permanent)
            highlighter->highlight();
    }
}

void InteractiveObject::clearHighlight()
{
    if (m_highlighter) {
        delete m_highlighter;
        m_highlighter = nullptr;
    }
}

bool InteractiveObject::isHighlighting() const
{
    return m_highlighter != nullptr;
}

QPointF InteractiveObject::centerInScene() const
{
    return sceneBoundingRect().center();
}

void InteractiveObject::scheduleLayoutUpdate()
{
    if (m_layoutDirty)
        return;

    m_layoutDirty = true;
    QMetaObject::invokeMethod(this, "instantLayoutUpdate", Qt::QueuedConnection);
}

void InteractiveObject::instantLayoutUpdate()
{
    const QRectF oldBounds = boundingRect();

    rebuildLayout();
    m_layoutDirty = false;

    if (oldBounds != boundingRect())
        Q_EMIT boundingBoxChanged();

    update();
}

void InteractiveObject::setRect(const QRectF &geometry)
{
    setPos(geometry.topLeft());
    prepareGeometryChange();
    m_boundingRect = { QPointF(0, 0), geometry.size() };
    instantLayoutUpdate();
}

void InteractiveObject::prepareHoverMark()
{
    if (!m_gripPoints) {
        m_gripPoints = new GripPointsHandler(this);
        m_gripPoints->setZValue(0);

        connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeStart, this,
                &InteractiveObject::gripPointPressed);
        connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeProgress, this,
                &InteractiveObject::gripPointMoved);
        connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeFinish, this,
                &InteractiveObject::gripPointReleased);

        connect(m_gripPoints, &GripPointsHandler::visibleChanged, this, [this]() {
            if (m_gripPoints && !m_gripPoints->isVisible())
                delete m_gripPoints; // it's not a thing directly added to the scene, so just delete
                                     // is enough
        });
        if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::Location::Center))
            gp->setGripType(GripPoint::GripType::Mover);
    }

    m_gripPoints->showAnimated();
}

} // namespace taste3
