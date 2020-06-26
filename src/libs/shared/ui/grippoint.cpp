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

#include "grippoint.h"

#include "grippointshandler.h"
#include "interactiveobjectbase.h"

#include <QColor>
#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

static shared::ui::DrawRectInfo uiDescr;

namespace shared {
namespace ui {

GripPoint::GripPoint(Location pos, GripPointsHandler *parent)
    : QGraphicsItem(parent)
    , m_listener(parent)
    , m_location(pos)
    , m_boundRect(
              QPointF(-::uiDescr.rectSize().width() / 2., -::uiDescr.rectSize().height() / 2.), ::uiDescr.rectSize())
{
    setFlags(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setVisible(true);
}

QRectF GripPoint::boundingRect() const
{
    return m_boundRect;
}

void GripPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->fillPath(m_path, ::uiDescr.body());
    painter->strokePath(m_path, ::uiDescr.border());
}

void GripPoint::setSideSize(qreal sz)
{
    ::uiDescr.setRectSize(QSizeF(sz, sz));
}

qreal GripPoint::sideSize()
{
    return ::uiDescr.rectSize().width();
}

void GripPoint::setBorderWidth(qreal w)
{
    ::uiDescr.setBorderWidth(w);
}

qreal GripPoint::borderWidth()
{
    return ::uiDescr.borderWidth();
}

void GripPoint::setBorderColor(const QColor &color)
{
    ::uiDescr.setBorderColor(color);
}

QColor GripPoint::borderColor()
{
    return ::uiDescr.borderColor();
}

void GripPoint::setBodyColor(const QColor &color)
{
    ::uiDescr.setBodyColor(color);
}

QColor GripPoint::bodyColor()
{
    return ::uiDescr.bodyColor();
}

GripPoint::GripType GripPoint::gripType() const
{
    return m_location == Center || m_location == Absolute ? GripType::Mover : GripType::Resizer;
}

bool GripPoint::isMover() const
{
    return gripType() == GripPoint::GripType::Mover;
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

void GripPoint::updateLayout()
{
    if (QGraphicsItem *targetObj = parentItem()) {
        QCursor c;

        const QRectF targetRect = targetObj->boundingRect();
        QPointF destination;
        switch (m_location) {
        case TopLeft:
            destination = targetRect.topLeft();
            c = Qt::SizeFDiagCursor;
            break;
        case Top:
            destination = { targetRect.center().x(), targetRect.top() };
            c = Qt::SizeVerCursor;
            break;
        case TopRight:
            destination = targetRect.topRight();
            c = Qt::SizeBDiagCursor;
            break;
        case Right:
            destination = { targetRect.right(), targetRect.center().y() };
            c = Qt::SizeHorCursor;
            break;
        case BottomRight:
            destination = targetRect.bottomRight();
            c = Qt::SizeFDiagCursor;
            break;
        case Bottom:
            destination = { targetRect.center().x(), targetRect.bottom() };
            c = Qt::SizeVerCursor;
            break;
        case BottomLeft:
            destination = targetRect.bottomLeft();
            c = Qt::SizeBDiagCursor;
            break;
        case Left:
            destination = { targetRect.left(), targetRect.center().y() };
            c = Qt::SizeHorCursor;
            break;
        case Center:
            destination = targetRect.center();
            c = Qt::SizeAllCursor;
            break;
        case Absolute:
            c = Qt::SizeAllCursor;
            break;
        }

        if (m_location != Absolute) {
            QRectF r(m_boundRect.translated(pos()));
            r.moveCenter(destination);
            setPos(r.center());
        }
        setCursor(c);
    }

    m_path = QPainterPath();
    m_path.addRect(m_boundRect);
}

void GripPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_listener) {
        Q_EMIT m_listener->manualGeometryChangeProgress(this, event->lastScenePos(), event->scenePos());
        event->accept();
    }
}

void GripPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_listener) {
            Q_EMIT m_listener->manualGeometryChangeStart(this, event->scenePos());
        }
        event->accept();
    } else {
        event->ignore();
    }
}

void GripPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_listener) {
        Q_EMIT m_listener->manualGeometryChangeFinish(
                this, event->buttonDownScenePos(event->button()), event->scenePos());
        event->accept();
    }
}

}
}
