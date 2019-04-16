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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "graphicsview.h"

#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/utils.h"
#include "baseitems/interactiveobject.h"

#include <QGraphicsItem>
#include <QMouseEvent>
#include <QUndoStack>
#include <mscentity.h>

namespace msc {

/*!
  \class msc::GraphicsView
  \brief Basic view for an MSC diagram

  \inmodule MscWidgets

  * Handles the middle mouse button to pan the view.
  * Provides zoom functionality.
  * Reports the current mouse position as signal
*/

/*!
  Constructs a MSV view object with the parent \a parent.
*/
GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , m_undoStack(new QUndoStack(this))
{
    setBackgroundBrush(QImage(":/resources/resources/texture.png"));

    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);

    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
}

double GraphicsView::zoom() const
{
    return m_zoomPercent;
}

void GraphicsView::setZoom(double percent)
{
    if (percent < minZoomPercent() || percent > maxZoomPercent())
        return;

    m_zoomPercent = percent;

    resetTransform();
    scale(m_zoomPercent / 100.0, m_zoomPercent / 100.0);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier) {
        Q_EMIT createMessageToolRequested();
        return;
    }

    if (event->buttons() == Qt::MidButton) {
        m_panning = true;
        m_lastMousePosition = event->localPos();
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint &screenPos(event->pos());
    const QPointF &scenePos(mapToScene(screenPos));

    auto coordinatesInfo = [](const QPointF &point, const QString &name) {
        static const QString infoTemplate("%1: [%2;%3]; ");
        return infoTemplate.arg(name, QString::number(point.x()), QString::number(point.y()));
    };

    QString info = coordinatesInfo(screenPos, "Screen");
    info.append(coordinatesInfo(scenePos, "Scene"));
    info.append(coordinatesInfo(utils::CoordinatesConverter::sceneToCif(scenePos), "CIF"));

    for (InteractiveObject *item : utils::itemByPos<InteractiveObject>(scene(), scenePos)) {
        info.append(coordinatesInfo(item->mapFromScene(scenePos),
                                    item->modelEntity() ? item->modelEntity()->name() : QLatin1String("None")));
    }

    Q_EMIT mouseMoved(info);

    if (m_panning) {
        QPointF translation = event->localPos() - m_lastMousePosition;
        translate(translation.x(), translation.y());
        m_lastMousePosition = event->localPos();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    m_panning = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        QPointF oldPos = mapToScene(event->pos());

        setZoom(m_zoomPercent + (event->delta() > 0 ? zoomStepPercent() : -zoomStepPercent()));

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
        setZoom(m_zoomPercent + (event->key() == Qt::Key_Plus ? zoomStepPercent() : -zoomStepPercent()));
        Q_EMIT zoomChanged(m_zoomPercent);
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}

QUndoStack *GraphicsView::undoStack() const
{
    return m_undoStack;
}

qreal GraphicsView::minZoomPercent() const
{
    return m_minZoomPercent;
}

void GraphicsView::setMinZoomPercent(qreal percent)
{
    m_minZoomPercent = percent;
}

qreal GraphicsView::maxZoomPercent() const
{
    return m_maxZoomPercent;
}

void GraphicsView::setMaxZoomPercent(qreal percent)
{
    m_maxZoomPercent = percent;
}

qreal GraphicsView::zoomStepPercent() const
{
    return m_zoomStepPercent;
}

void GraphicsView::setZoomStepPercent(qreal percent)
{
    m_zoomStepPercent = percent;
}

} // namespace msc
