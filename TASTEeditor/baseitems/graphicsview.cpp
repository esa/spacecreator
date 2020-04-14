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

#include "baseitems/common/utils.h"
#include "baseitems/interactiveobject.h"
#include "tab_aadl/aadlobject.h"
#include "tab_interface/aadlrectgraphicsitem.h"

#include <QGraphicsItem>
#include <QMouseEvent>

namespace taste3 {

/*!
  \class taste3::GraphicsView
  \brief The QGraphicsView wrapper for displaying the AADL.
*/

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    setDragMode(QGraphicsView::DragMode::RubberBandDrag);
    setRubberBandSelectionMode(Qt::IntersectsItemShape);

    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
}

/*!
\brief GraphicsView::zoom() Get the current zoom percentage.
Returns the value of \c m_zoomPercent
*/

double GraphicsView::zoom() const
{
    return m_zoomPercent;
}

/*!
 * \brief GraphicsView::setZoom Set the current zoom percentage and update the view
 * \a percent
 */
void GraphicsView::setZoom(double percent)
{
    percent = qBound(minZoomPercent(), percent, maxZoomPercent());
    if (qFuzzyCompare(m_zoomPercent, percent))
        return;

    m_zoomPercent = percent;

    resetTransform();
    scale(m_zoomPercent / 100.0, m_zoomPercent / 100.0);

    Q_EMIT zoomChanged(m_zoomPercent);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
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

    const QString infoTemplate("%1: [%2;%3]; ");
    auto coordinatesInfo = [infoTemplate](const QPointF &point, const QString &name) {
        return infoTemplate.arg(name, QString::number(point.x()), QString::number(point.y()));
    };

    QString info = coordinatesInfo(screenPos, "Screen");
    info.append(coordinatesInfo(scenePos, "Scene"));

    QList<QGraphicsItem *> itemsUnderCursor = items(screenPos);
    for (QGraphicsItem *item : itemsUnderCursor) {
        if (auto iObj = qobject_cast<aadl::InteractiveObject *>(item->toGraphicsObject())) {
            info.append(coordinatesInfo(item->mapFromScene(scenePos),
                                        iObj->aadlObject() ? iObj->aadlObject()->objectName() : QLatin1String("None")));
        }
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
        event->accept();
        return;
    }

    bool keyHandled(false);
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down: {
        for (QGraphicsItem *item : scene()->selectedItems()) {
            if (aadl::AADLRectGraphicsItem *rectItem =
                        // Not the qgraphicsitem_cast due its "successfull" cast of AADLFunctionName/Text Graphics Items
                qobject_cast<aadl::AADLRectGraphicsItem *>(item->toGraphicsObject())) {
                rectItem->singleStepMove(aadl::AADLRectGraphicsItem::MoveStep(event->key()));
                keyHandled = true;
            }
        }
        break;
    }
    default:
        break;
    }

    if (keyHandled)
        event->accept();
    else
        QGraphicsView::keyPressEvent(event);
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    static const QImage brushImage { QLatin1String(":/backgrounds/texture.png") };
    const QRectF scaleSceneRect { rect.topLeft() * m_zoomPercent / 100, rect.size() * m_zoomPercent / 100 };
    painter->save();
    painter->scale(100 / m_zoomPercent, 100 / m_zoomPercent);
    painter->setPen(Qt::NoPen);
    painter->setBrush(brushImage);
    painter->drawRect(scaleSceneRect);
    painter->restore();
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

} // namespace taste3
