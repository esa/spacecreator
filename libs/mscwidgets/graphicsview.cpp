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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "graphicsview.h"

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QTimeLine>
#include <QUndoStack>

namespace msc {

qreal GraphicsView::MinZoom = 50.0;
qreal GraphicsView::ZoomStep = 25.0;
qreal GraphicsView::MaxZoom = 400.0;

/*!
  \class msc::GraphicsView
  \brief Basic view for an MSC diagram

  \inmodule MscWidgets

  Some documentation
*/

/*!
  Constructs a MSV view object with the parent \a parent.
*/
GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent), m_undoStack(new QUndoStack(this))
{
    setBackgroundBrush(QImage(":/resources/resources/texture.png"));

    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);
}

void GraphicsView::setZoom(double percent)
{
    if (percent < GraphicsView::MinZoom || percent > GraphicsView::MaxZoom)
        return;

    m_zoomPercent = percent;

    resetTransform();
    scale(m_zoomPercent / 100.0, m_zoomPercent / 100.0);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint &screenPos(event->pos());
    const QPointF &scenePos(mapToScene(screenPos));

    QPointF itemPos(-1, -1);
    if (QGraphicsItem *item = itemAt(screenPos))
        itemPos = item->mapFromScene(scenePos);

    Q_EMIT mouseMoved(screenPos, scenePos, itemPos);

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        QPointF oldPos = mapToScene(event->pos());

        setZoom(m_zoomPercent + (event->delta() > 0 ? ZoomStep : -ZoomStep));

        QPointF newPos = mapToScene(event->pos());
        QPointF delta = newPos - oldPos;

        translate(delta.x(), delta.y());

        Q_EMIT zoomChanged(m_zoomPercent);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier && (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Minus)) {
        setZoom(m_zoomPercent + (event->key() == Qt::Key_Plus ? ZoomStep : -ZoomStep));
        Q_EMIT zoomChanged(m_zoomPercent);
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}

QUndoStack *GraphicsView::undoStack() const
{
    return m_undoStack;
}

} // namespace msc
