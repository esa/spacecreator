#include "graphicsviewbase.h"

#include <QMouseEvent>

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

GraphicsViewBase::GraphicsViewBase(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent), d(new GraphicsViewBasePrivate)
{
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
}

GraphicsViewBase::GraphicsViewBase(QWidget* parent)
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

void GraphicsViewBase::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() == Qt::MidButton) {
        d->panning = true;
        d->lastMousePosition = event->localPos();
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsViewBase::mouseMoveEvent(QMouseEvent *event)
{
    if (d->panning) {
        QPointF translation = event->localPos() - d->lastMousePosition;
        translate(translation.x(), translation.y());
        d->lastMousePosition = event->localPos();
    }

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



}
}
