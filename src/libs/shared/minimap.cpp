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

#include <QDebug>
#include <QElapsedTimer>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QPointer>
#include <QScrollBar>
#include <QTimer>
#include <QUndoStack>

#define LOG qDebug() << Q_FUNC_INFO

namespace shared {
namespace ui {

struct MiniMapPrivate {
    MiniMapPrivate()
        : m_renderTimer(new QTimer)
    {
        static constexpr int renderDelayMs = 20;
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
};

MiniMap::MiniMap(QWidget *parent)
    : QWidget(parent, Qt::Tool)
    , d(new MiniMapPrivate)
{
    connect(d->m_renderTimer, &QTimer::timeout, this, &MiniMap::delayedUpdate);

    static constexpr int minDimensionPix = 100;
    setMinimumSize(minDimensionPix, minDimensionPix);
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

    QElapsedTimer et;
    et.start();

    d->m_sceneContent = QPixmap(size());
    d->m_sceneContent.fill(Qt::white);
    QPainter p(&d->m_sceneContent);
    scene->render(&p, d->m_sceneContent.rect(), QRect());

    d->m_renderTimer->setInterval(et.elapsed() * 2);

    return true;
}

bool MiniMap::grabViewportRect()
{
    auto scene = d->m_view->scene();
    if (!scene) {
        return false;
    }

    const QRectF &sceneRect = scene->sceneRect();
    const QRect &sceneRectPix = d->m_view->mapFromScene(sceneRect).boundingRect();

    d->m_sceneViewport = d->m_view->viewport()->rect();
    d->m_sceneViewport.translate(d->m_sceneViewport.topLeft() - sceneRectPix.topLeft());

    return true;
}

void MiniMap::composeMap()
{
    if (d->m_sceneContent.isNull()) {
        return;
    }

    const QRect &actualScene = d->m_view->mapFromScene(d->m_view->scene()->sceneRect()).boundingRect();
    const qreal scaleFactor = (actualScene.width() < actualScene.height())
            ? qreal(d->m_sceneContent.width()) / qreal(actualScene.width())
            : qreal(d->m_sceneContent.height()) / qreal(actualScene.height());

    const QTransform &t = QTransform::fromScale(scaleFactor, scaleFactor);
    const QRect &scaledViewport = t.mapRect(d->m_sceneViewport);
    const QRect drawnRect = QRect({ 0, 0 }, t.mapRect(actualScene).size());

    QPainterPath dimmedOverlay;
    dimmedOverlay.addRect(drawnRect);
    dimmedOverlay.addRect(scaledViewport);

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

} // namespace ui
} // namespace shared
