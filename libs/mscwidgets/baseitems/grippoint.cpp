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

#include "grippoint.h"
#include "interactiveobject.h"
#include "grippointshandler.h"

#include <QColor>
#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

namespace msc {

/*!
  \class msc::GripPoint
  \brief Handler for resizing/moving item by mouse.

  \inmodule MscWidgets

*/

DrawRectInfo GripPoint::m_uiDescr = DrawRectInfo();

void GripPoint::setSideSize(qreal sz)
{
    m_uiDescr.setRectSize(QSizeF(sz, sz));
}

qreal GripPoint::sideSize()
{
    return m_uiDescr.rectSize().width();
}

void GripPoint::setBorderWidth(qreal w)
{
    m_uiDescr.setBorderWidth(w);
}

qreal GripPoint::borderWidth()
{
    return m_uiDescr.borderWidth();
}

void GripPoint::setBorderColor(const QColor &color)
{
    m_uiDescr.setBorderColor(color);
}

QColor GripPoint::borderColor()
{
    return m_uiDescr.borderColor();
}

void GripPoint::setBodyColor(const QColor &color)
{
    m_uiDescr.setBodyColor(color);
}

QColor GripPoint::bodyColor()
{
    return m_uiDescr.bodyColor();
}

GripPoint::GripPoint(Location pos, GripPointsHandler *parent, GripPoint::GripType gpType)
    : QGraphicsItem(parent)
    , m_listener(parent)
    , m_location(pos)
    , m_type(gpType)
    , m_boundRect(QPointF(-m_uiDescr.rectSize().width() / 2., -m_uiDescr.rectSize().height() / 2.),
                  m_uiDescr.rectSize())
{
    setFlags(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setVisible(true);
};

void GripPoint::updateLayout()
{
    if (QGraphicsItem *targetObj = parentItem()) {
        QCursor c;

        const QRectF targetRect = targetObj->boundingRect();
        QPointF destination;
        switch (m_location) {
        case TopLeft:
            destination = targetRect.topLeft();
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeFDiagCursor;
            break;
        case Top:
            destination = { targetRect.center().x(), targetRect.top() };
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeVerCursor;
            break;
        case TopRight:
            destination = targetRect.topRight();
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeBDiagCursor;
            break;
        case Right:
            destination = { targetRect.right(), targetRect.center().y() };
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeHorCursor;
            break;
        case BottomRight:
            destination = targetRect.bottomRight();
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeFDiagCursor;
            break;
        case Bottom:
            destination = { targetRect.center().x(), targetRect.bottom() };
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeVerCursor;
            break;
        case BottomLeft:
            destination = targetRect.bottomLeft();
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeBDiagCursor;
            break;
        case Left:
            destination = { targetRect.left(), targetRect.center().y() };
            c = isMover() ? Qt::SizeAllCursor : Qt::SizeHorCursor;
            break;
        case Center:
            destination = targetRect.center();
            c = Qt::SizeAllCursor;
            break;
        }

        if (!isMover() || m_location == Center) {
            QRectF r(m_boundRect.translated(pos()));
            r.moveCenter(destination);
            setPos(r.center());
        }
        setCursor(c);
    }

    m_path = QPainterPath();
    m_path.addRect(m_boundRect);
}

QRectF GripPoint::boundingRect() const
{
    return m_boundRect;
}

bool GripPoint::isMover() const
{
    return gripType() == GripPoint::GripType::Mover;
}

GripPoint::GripType GripPoint::gripType() const
{
    return m_type;
}

void GripPoint::setGripType(GripPoint::GripType gpType)
{
    if (gripType() == gpType)
        return;

    m_type = gpType;
    updateLayout();
}

bool GripPoint::isUsed() const
{
    return m_used;
}
void GripPoint::setIsUsed(bool used)
{
    if (m_used == used)
        return;

    m_used = used;
    if (!m_used)
        setVisible(false);
}

void GripPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->fillPath(m_path, m_uiDescr.body());
    painter->strokePath(m_path, m_uiDescr.border());
}

void GripPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_listener)
        m_listener->handleGripPointMove(this, event->lastScenePos(), event->scenePos());
}

void GripPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_listener)
        m_listener->handleGripPointPress(this, event->lastScenePos(), event->scenePos());
}

void GripPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_listener)
        m_listener->handleGripPointRelease(this, event->lastScenePos(), event->scenePos());
}

} // namespace msc
