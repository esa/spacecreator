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

InteractiveObject::InteractiveObject(QGraphicsItem *parent) : QGraphicsObject(parent)
{

    setAcceptHoverEvents(true);

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

void InteractiveObject::gripPointMoved(GripPoint::Location gripPos, const QPointF &from, const QPointF &to)
{
    if (m_gripPoints)
        if (GripPoint *gripPnt = m_gripPoints->gripPoint(gripPos))
            handleGripPointMovement(gripPnt, from, to);
}

void InteractiveObject::handleGripPointMovement(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    if (grip->isMover())
        onMoveRequested(grip, from, to);
    else
        onResizeRequested(grip, from, to);
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
    HighlightRectItem *highlighter(new HighlightRectItem(this));
    connect(highlighter, &HighlightRectItem::highlighted, highlighter, &QObject::deleteLater);

    highlighter->setRect(m_boundingRect);

    return highlighter;
}

void InteractiveObject::doHighlighting(const QColor &color)
{
    if (HighlightRectItem *highlighter = createHighlighter()) {
        QColor targetColor(color);
        QPen p(targetColor);
        p.setWidthF(3.);
        highlighter->setPen(p);
        targetColor.setAlphaF(0.25);
        highlighter->setBrush(targetColor);

        highlighter->highlight();
    }
}

void InteractiveObject::highlightConnected()
{
    doHighlighting(Qt::green);
}

void InteractiveObject::highlightDisconnected()
{
    doHighlighting(Qt::red);
}

QPointF InteractiveObject::centerInScene() const
{
    return mapToScene(boundingRect().center());
}

void InteractiveObject::updateLayout()
{
    if (m_layoutDirty) {
        return;
    }

    m_layoutDirty = true;
    QMetaObject::invokeMethod(this, "rebuildLayout", Qt::QueuedConnection);
}

void InteractiveObject::prepareHoverMark()
{
    if (!m_gripPoints) {
        m_gripPoints = new GripPointsHandler(this);
        m_gripPoints->setZValue(0);

        connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeProgress, this,
                &InteractiveObject::gripPointMoved);
        connect(m_gripPoints, &GripPointsHandler::visibleChanged, this, [this]() {
            if (m_gripPoints && !m_gripPoints->isVisible())
                delete m_gripPoints; // it's not a thing directly added to the scene, so just delete is enough
        });
        if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::Location::Center))
            gp->setGripType(GripPoint::GripType::Mover);
    }

    m_gripPoints->showAnimated();
}

} // namespace msc
