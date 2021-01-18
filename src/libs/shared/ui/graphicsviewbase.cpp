/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "graphicsviewbase.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

namespace shared {
namespace ui {

struct GraphicsViewBase::GraphicsViewBasePrivate {
    qreal zoomPercent = 100.0;

    qreal minZoomPercent = 5.0;
    qreal zoomStepPercent = 25.0;
    qreal maxZoomPercent = 400.0;

    bool panning = false;
    QPointF lastMousePosition;
};

GraphicsViewBase::GraphicsViewBase(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
    , d(new GraphicsViewBasePrivate)
{
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    setDragMode(QGraphicsView::DragMode::RubberBandDrag);
    setRubberBandSelectionMode(Qt::IntersectsItemShape);
}

GraphicsViewBase::GraphicsViewBase(QWidget *parent)
    : GraphicsViewBase(nullptr, parent)
{
}

GraphicsViewBase::~GraphicsViewBase()
{
    delete d;
    d = nullptr;
}

/*!
\brief GraphicsViewBase::zoom() Get the current zoom percentage.
Returns the value of \c zoomPercent
*/
double GraphicsViewBase::zoom() const
{
    return d->zoomPercent;
}

qreal GraphicsViewBase::minZoomPercent() const
{
    return d->minZoomPercent;
}

qreal GraphicsViewBase::maxZoomPercent() const
{
    return d->maxZoomPercent;
}

qreal GraphicsViewBase::zoomStepPercent() const
{
    return d->zoomStepPercent;
}

/*!
 * \brief GraphicsViewBase::setZoom Set the current zoom percentage and update the view
 * \a percent
 */
void GraphicsViewBase::setZoom(double percent)
{
    percent = qBound(minZoomPercent(), percent, maxZoomPercent());
    if (qFuzzyCompare(d->zoomPercent, percent))
        return;

    d->zoomPercent = percent;

    resetTransform();
    scale(d->zoomPercent / 100.0, d->zoomPercent / 100.0);

    Q_EMIT zoomChanged(d->zoomPercent);
}

void GraphicsViewBase::setMinZoomPercent(qreal percent)
{
    d->minZoomPercent = percent;
}

void GraphicsViewBase::setMaxZoomPercent(qreal percent)
{
    d->maxZoomPercent = percent;
}

void GraphicsViewBase::setZoomStepPercent(qreal percent)
{
    d->zoomStepPercent = percent;
}

QList<QPair<QPointF, QString>> GraphicsViewBase::mouseMoveCoordinates(
        QGraphicsScene *, const QPoint &, const QPointF &) const
{
    return QList<QPair<QPointF, QString>>();
}

void GraphicsViewBase::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MidButton) {
        d->panning = true;
        d->lastMousePosition = event->localPos();
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsViewBase::mouseMoveEvent(QMouseEvent *event)
{
    // Handle panning
    if (d->panning) {
        QPointF translation = event->localPos() - d->lastMousePosition;
        translate(translation.x(), translation.y());
        d->lastMousePosition = event->localPos();
    }

    // Show the coordinates list in the statusbar
    const QPoint &screenPos(event->pos());
    const QPointF &scenePos(mapToScene(screenPos));
    const QString infoTemplate("%1: [%2;%3]; ");

    auto coordinatesInfo = [infoTemplate](const QPointF &point, const QString &name) {
        return infoTemplate.arg(name, QString::number(point.x()), QString::number(point.y()));
    };

    QString info = coordinatesInfo(screenPos, "Screen");
    info.append(coordinatesInfo(scenePos, "Scene"));

    // Handle the coordinates from the subclass
    auto coords = mouseMoveCoordinates(scene(), screenPos, scenePos);
    for (const auto &item : coords) {
        info.append(coordinatesInfo(item.first, item.second));
    }

    Q_EMIT mouseMoved(info);

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsViewBase::mouseReleaseEvent(QMouseEvent *event)
{
    d->panning = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsViewBase::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        QPointF oldPos = mapToScene(event->pos());

        setZoom(d->zoomPercent + (event->delta() > 0 ? zoomStepPercent() : -zoomStepPercent()));

        QPointF newPos = mapToScene(event->pos());
        QPointF delta = newPos - oldPos;

        translate(delta.x(), delta.y());
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void GraphicsViewBase::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier && (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Minus)) {
        setZoom(d->zoomPercent + (event->key() == Qt::Key_Plus ? zoomStepPercent() : -zoomStepPercent()));
        event->accept();
        return;
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphicsViewBase::drawBackground(QPainter *painter, const QRectF &rect)
{
    // Light grey background
    painter->fillRect(rect, QColor(231, 234, 237));

    // Draw the vertical lines
    painter->save();
    painter->setPen(QColor(238, 241, 241));

    // By doing this we counter the zooming
    const double zoomPercent = zoom();
    const QRectF scaleSceneRect { rect.topLeft() * zoomPercent / 100, rect.size() * zoomPercent / 100 };
    painter->scale(100 / zoomPercent, 100 / zoomPercent);

    // Now do the drawing
    const int y1 = static_cast<int>(scaleSceneRect.top());
    const int y2 = static_cast<int>((scaleSceneRect.bottom() + 1));
    const int x1 = static_cast<int>((scaleSceneRect.left()));
    const int x2 = static_cast<int>((scaleSceneRect.right() + 1));
    for (int x = x1 & ~15; x <= x2; x += 16) {
        painter->drawLine(x, y1, x, y2);
    }
    for (int y = y1 & ~15; y < y2; y += 16) {
        painter->drawLine(x1, y, x2, y);
    }

    painter->restore();
}

}
}
