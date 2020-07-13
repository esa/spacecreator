/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

   This application is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   This application is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
*/

#include "minimap.h"

#include <QCursor>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QPointer>
#include <QScrollBar>
#include <QTimer>
#include <QUndoStack>

#define LOG qDebug() << Q_FUNC_INFO

namespace shared {
namespace ui {

static const QPoint OutOfView { -1, -1 };

struct MiniMapPrivate {
    MiniMapPrivate()
        : m_renderTimer(new QTimer)
    {
        static constexpr int renderDelayMs = 10;
        m_renderTimer->setInterval(renderDelayMs);
        m_renderTimer->setSingleShot(true);

        m_dimColor.setAlphaF(0.5);
    }

    ~MiniMapPrivate()
    {
        delete m_renderTimer;
        m_renderTimer = nullptr;
    }

    enum class RequestedUpdate
    {
        None,
        Content,
        Viewport,
        Both
    };

    QPointer<QGraphicsView> m_view { nullptr };
    QPointer<QUndoStack> m_commandsStack { nullptr };
    QTimer *m_renderTimer { nullptr };

    QPixmap m_sceneContent;
    QRect m_sceneViewport;

    QPixmap m_display;

    QColor m_dimColor { Qt::black };

    RequestedUpdate m_scheduledUpdate { RequestedUpdate::Both };
    QRect m_mappedViewport;
    QRectF m_sceneRect;

    QPoint m_mouseStart = OutOfView;
    QPoint m_mouseFinish = OutOfView;
};

MiniMap::MiniMap(QWidget *parent)
    : QWidget(parent, Qt::Tool)
    , d(new MiniMapPrivate)
{
    connect(d->m_renderTimer, &QTimer::timeout, this, &MiniMap::delayedUpdate);

    static constexpr int minDimensionPix = 100;
    setMinimumSize(minDimensionPix, minDimensionPix);

    setMouseTracking(true);
}

MiniMap::~MiniMap() {}

void MiniMap::setupSourceView(QGraphicsView *view, QUndoStack *stack)
{
    if (d->m_view) {
        if (d->m_commandsStack) {
            disconnect(d->m_commandsStack, &QUndoStack::indexChanged, this, &MiniMap::onSceneUpdated);
        }

        for (auto scrollBar : { d->m_view->verticalScrollBar(), d->m_view->horizontalScrollBar() }) {
            disconnect(scrollBar, &QScrollBar::valueChanged, this, &MiniMap::onViewUpdated);
            disconnect(scrollBar, &QScrollBar::rangeChanged, this, &MiniMap::onSceneUpdated);
        }

        d->m_view = nullptr;
    }

    d->m_view = view;

    if (d->m_view) {

        for (auto scrollBar : { d->m_view->verticalScrollBar(), d->m_view->horizontalScrollBar() }) {
            connect(scrollBar, &QScrollBar::valueChanged, this, &MiniMap::onViewUpdated);
            connect(scrollBar, &QScrollBar::rangeChanged, this, &MiniMap::onSceneUpdated);
        }

        d->m_commandsStack = stack;

        if (d->m_commandsStack) {
            connect(d->m_commandsStack, &QUndoStack::indexChanged, this, &MiniMap::onSceneUpdated);
        }
    }
}

void MiniMap::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    const bool visible = isVisible();
    Q_EMIT visibilityChanged(visible);

    if (visible) {
        d->m_scheduledUpdate = MiniMapPrivate::RequestedUpdate::Both;
        delayedUpdate();
    }
}

void MiniMap::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
    Q_EMIT visibilityChanged(isVisible());
}

void MiniMap::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (!d->m_display.isNull()) {
        QPainter painter(this);
        painter.drawPixmap(rect(), d->m_display);
    }
}

void MiniMap::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    scheduleUpdateContent();
}

void MiniMap::onSceneUpdated()
{
    if (!isVisible()) {
        return;
    }

    scheduleUpdateContent();
}

void MiniMap::onViewUpdated()
{
    if (!isVisible()) {
        return;
    }

    scheduleUpdateViewport();
}

bool MiniMap::grabSceneContent()
{
    auto scene = d->m_view->scene();
    if (!scene) {
        return false;
    }

    d->m_sceneContent = QPixmap(size());
    d->m_sceneContent.fill(Qt::white);
    QPainter p(&d->m_sceneContent);
    scene->render(&p, d->m_sceneContent.rect(), QRect());

    return true;
}

bool MiniMap::grabViewportRect()
{
    auto scene = d->m_view->scene();
    if (!scene) {
        return false;
    }

    d->m_sceneRect = scene->sceneRect();
    const QRect &sceneRectPix = d->m_view->mapFromScene(d->m_sceneRect).boundingRect();

    d->m_sceneViewport = d->m_view->viewport()->rect();
    d->m_sceneViewport.translate(d->m_sceneViewport.topLeft() - sceneRectPix.topLeft());

    return true;
}

void MiniMap::composeMap()
{
    if (d->m_sceneContent.isNull()) {
        return;
    }

    const QRect &actualScene = d->m_view->mapFromScene(d->m_sceneRect).boundingRect();
    const qreal scaleFactor = (actualScene.width() < actualScene.height())
            ? qreal(d->m_sceneContent.width()) / qreal(actualScene.width())
            : qreal(d->m_sceneContent.height()) / qreal(actualScene.height());

    const QTransform &t = QTransform::fromScale(scaleFactor, scaleFactor);
    d->m_mappedViewport = t.mapRect(d->m_sceneViewport);
    const QRect drawnRect = QRect({ 0, 0 }, t.mapRect(actualScene).size());

    QPainterPath dimmedOverlay;
    dimmedOverlay.addRect(drawnRect);
    dimmedOverlay.addRect(d->m_mappedViewport);

    QPixmap composedPreview(d->m_sceneContent);
    QPainter painter(&composedPreview);
    painter.fillPath(dimmedOverlay, dimColor());
    d->m_display = composedPreview;

    setFixedSize(drawnRect.size());

    update();
}

void MiniMap::setDimColor(const QColor &to)
{
    if (d->m_dimColor != to) {
        d->m_dimColor = to;

        composeMap();
    }
}

QColor MiniMap::dimColor() const
{
    return d->m_dimColor;
}

void MiniMap::scheduleUpdateContent()
{
    if (d->m_renderTimer->isActive())
        d->m_renderTimer->stop();

    switch (d->m_scheduledUpdate) {
    case MiniMapPrivate::RequestedUpdate::None:
        d->m_scheduledUpdate = MiniMapPrivate::RequestedUpdate::Content;
        break;
    default:
        d->m_scheduledUpdate = MiniMapPrivate::RequestedUpdate::Both;
        break;
    }

    d->m_renderTimer->start();
}

void MiniMap::scheduleUpdateViewport()
{
    if (d->m_renderTimer->isActive())
        d->m_renderTimer->stop();

    switch (d->m_scheduledUpdate) {
    case MiniMapPrivate::RequestedUpdate::None:
        d->m_scheduledUpdate = MiniMapPrivate::RequestedUpdate::Viewport;
        break;
    default:
        d->m_scheduledUpdate = MiniMapPrivate::RequestedUpdate::Both;
        break;
    }

    d->m_renderTimer->start();
}

void MiniMap::delayedUpdate()
{
    switch (d->m_scheduledUpdate) {
    case MiniMapPrivate::RequestedUpdate::Content:
        grabSceneContent();
        break;
    case MiniMapPrivate::RequestedUpdate::Viewport:
        grabViewportRect();
        break;
    default: {
        grabSceneContent();
        grabViewportRect();
        break;
    }
    }

    d->m_scheduledUpdate = MiniMapPrivate::RequestedUpdate::None;

    composeMap();
}

bool MiniMap::checkMouseEvent(QMouseEvent *e, Qt::MouseButton current, Qt::MouseButton started) const
{
    return e->button() == current && e->buttons() == started && e->modifiers() == Qt::NoModifier;
}

void MiniMap::updateCursorInMappedViewport(const QPoint &pos, Qt::CursorShape targetShape)
{
    const Qt::CursorShape cursorShape = d->m_mappedViewport.contains(pos) ? targetShape : Qt::ArrowCursor;
    setCursor(cursorShape);
}

void MiniMap::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if (checkMouseEvent(event, Qt::LeftButton, Qt::LeftButton)) {
        updateCursorInMappedViewport(event->pos(), Qt::ClosedHandCursor);

        d->m_mouseStart = event->pos();
    }
}

void MiniMap::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    if (checkMouseEvent(event, Qt::NoButton, Qt::NoButton)) {
        updateCursorInMappedViewport(event->pos(), Qt::OpenHandCursor);
    } else if (checkMouseEvent(event, Qt::NoButton, Qt::LeftButton)) {
        d->m_mouseFinish = event->pos();
        processMouseInput();
    }
}

void MiniMap::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if (checkMouseEvent(event, Qt::LeftButton, Qt::NoButton)) {
        d->m_mouseFinish = event->pos();
        processMouseInput();
    }

    updateCursorInMappedViewport(event->pos(), Qt::OpenHandCursor);

    d->m_mouseStart = OutOfView;
    d->m_mouseFinish = OutOfView;
}

void MiniMap::processMouseInput()
{
    if (d->m_mouseFinish == OutOfView) {
        return;
    }

    QPointF newCenter;
    if (d->m_mouseStart != d->m_mouseFinish) {
        // It's a drag. In general, we should shift the main viewport accordingly to the shift of the minimap's preview.
        // But let's just center the main view on the current mouse pos instead — the UX flow seems to be smooth,
        // but the code is way much simplier.
        // This seems to be enough, at least for beginning.
        d->m_mouseStart = d->m_mouseFinish;
    } // else it's a click - recenter the main view on it

    newCenter = pixelToScene(d->m_mouseStart);

    d->m_view->centerOn(newCenter);
}

QPointF MiniMap::pixelToScene(const QPoint &pixel) const
{
    const qreal scaleFactor = d->m_sceneRect.width() / d->m_sceneContent.rect().width();
    const QTransform t = QTransform::fromScale(scaleFactor, scaleFactor);

    const QPointF scenePoint = t.map(pixel) + d->m_sceneRect.topLeft();
    return scenePoint;
}

} // namespace ui
} // namespace shared
