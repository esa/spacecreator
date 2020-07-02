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
        static constexpr int renderDelayMs = 100;
        m_renderTimer->setInterval(renderDelayMs);
        m_renderTimer->setSingleShot(true);

        m_dimColor.setAlphaF(0.5);
    }

    ~MiniMapPrivate()
    {
        delete m_renderTimer;
        m_renderTimer = nullptr;
    }

    QPointer<QGraphicsView> m_view { nullptr };
    QPointer<QUndoStack> m_commandsStack { nullptr };
    QTimer *m_renderTimer { nullptr };

    QPixmap m_sceneContent;
    QRect m_sceneViewport;

    QPixmap m_display;

    QColor m_dimColor { Qt::gray };
};

MiniMap::MiniMap(QWidget *parent)
    : QWidget(parent, Qt::Tool)
    , d(new MiniMapPrivate)
{
    qDebug() << Q_FUNC_INFO << parent;
    connect(d->m_renderTimer, &QTimer::timeout, this, &MiniMap::updateSceneContent);

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

        for (auto scrollBar : { d->m_view->verticalScrollBar(), d->m_view->horizontalScrollBar() })
            disconnect(scrollBar, &QScrollBar::valueChanged, this, &MiniMap::onViewUpdated);

        d->m_view = nullptr;
    }

    d->m_view = view;

    if (d->m_view) {

        for (auto scrollBar : { d->m_view->verticalScrollBar(), d->m_view->horizontalScrollBar() })
            connect(scrollBar, &QScrollBar::valueChanged, this, &MiniMap::onViewUpdated);

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
        updateSceneContent();
        updateViewportFrame();
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

    composeMap();
}

void MiniMap::onSceneUpdated()
{
    // Depending on the actual scene content/size,
    // the rendering process might be a resources consuming task.
    // Do it only when really necessary.
    if (!isVisible()) {
        return;
    }

    if (d->m_renderTimer->isActive())
        d->m_renderTimer->stop();
    d->m_renderTimer->start();
}

void MiniMap::onViewUpdated()
{
    // Just to keep it consistent with the scene's content
    if (!isVisible()) {
        return;
    }

    updateViewportFrame();
}

void MiniMap::updateSceneContent()
{
    if (auto scene = d->m_view->scene()) {
        const QRect &sceneRectPix = d->m_view->mapFromScene(scene->sceneRect()).boundingRect();
        d->m_sceneContent = QPixmap(sceneRectPix.size());
        d->m_sceneContent.fill(Qt::transparent);
        QPainter p(&d->m_sceneContent);
        scene->render(&p);

        composeMap();
    }
}

void MiniMap::updateViewportFrame()
{
    if (auto scene = d->m_view->scene()) {

        const QRectF &sceneRect = scene->sceneRect();
        const QRect &sceneRectPix = d->m_view->mapFromScene(sceneRect).boundingRect();

        d->m_sceneViewport = d->m_view->viewport()->rect();
        d->m_sceneViewport.translate(d->m_sceneViewport.topLeft() - sceneRectPix.topLeft());

        composeMap();
    }
}

void MiniMap::composeMap()
{
    QPixmap holder(d->m_sceneContent);

    QPainterPath dimmedOverlay;
    dimmedOverlay.addRect(holder.rect());
    dimmedOverlay.addRect(d->m_sceneViewport);

    QPainter painter(&holder);
    painter.fillPath(dimmedOverlay, dimColor());

    d->m_display = holder.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

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

} // namespace ui
} // namespace shared
