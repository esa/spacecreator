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

#include "ui/grippoint.h"

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

namespace shared {
namespace ui {

static constexpr QPoint kOutOfView { -1, -1 };
static const QColor kDefaultDimColor { 0x00, 0x00, 0x00, 0x88 };
static constexpr qreal kDefaultScaleFactor { 6 };

struct MiniMapPrivate {
    QPointer<QGraphicsView> m_view;
    QColor m_dimColor { kDefaultDimColor };
    QPoint m_mouseStart { kOutOfView };
    QPoint m_mouseFinish { kOutOfView };
};

MiniMap::MiniMap(QWidget *parent)
    : QGraphicsView(parent)
    , d(new MiniMapPrivate)
{
    setOptimizationFlag(QGraphicsView::IndirectPainting);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setMouseTracking(true);
    setVisible(false);
}

MiniMap::~MiniMap() { }

void MiniMap::setupSourceView(QGraphicsView *view)
{
    Q_ASSERT(view);
    Q_ASSERT(view->scene());

    if (d->m_view) {
        d->m_view->removeEventFilter(this);
        for (auto scrollBar : { d->m_view->verticalScrollBar(), d->m_view->horizontalScrollBar() }) {
            disconnect(scrollBar, &QScrollBar::valueChanged, viewport(), qOverload<>(&QWidget::update));
            disconnect(scrollBar, &QScrollBar::rangeChanged, viewport(), qOverload<>(&QWidget::update));
        }
        d->m_view.clear();
    }

    d->m_view = view;
    setParent(view);

    if (d->m_view) {
        d->m_view->installEventFilter(this);
        for (auto scrollBar : { d->m_view->verticalScrollBar(), d->m_view->horizontalScrollBar() }) {
            connect(scrollBar, &QScrollBar::valueChanged, viewport(), qOverload<>(&QWidget::update));
            connect(scrollBar, &QScrollBar::rangeChanged, viewport(), qOverload<>(&QWidget::update));
        }
        setScene(view->scene());
        adjustGeometry();
        if (scene()) {
            connect(scene(), &QGraphicsScene::sceneRectChanged, this, &MiniMap::adjustGeometry);
        }
    }
}

void MiniMap::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    updateScaling();
}

void MiniMap::setDimColor(const QColor &to)
{
    if (d->m_dimColor != to) {
        d->m_dimColor = to;
        viewport()->update();
    }
}

QColor MiniMap::dimColor() const
{
    return d->m_dimColor;
}

bool MiniMap::checkMouseEvent(QMouseEvent *e, Qt::MouseButton current, Qt::MouseButton started) const
{
    return e->button() == current && e->buttons() == started && e->modifiers() == Qt::NoModifier;
}

void MiniMap::updateCursorInMappedViewport(const QPoint &pos, Qt::CursorShape targetShape)
{
    const auto viewportOnScene = d->m_view->mapToScene(d->m_view->viewport()->rect());
    const auto targetViewport = mapFromScene(viewportOnScene).boundingRect();
    const Qt::CursorShape cursorShape = targetViewport.contains(pos) ? targetShape : Qt::ArrowCursor;
    setCursor(cursorShape);
}

QRectF MiniMap::mappedViewportOnScene() const
{
    const auto viewportOnScene = d->m_view->mapToScene(d->m_view->viewport()->rect());
    return viewportOnScene.boundingRect();
}

void MiniMap::adjustGeometry()
{
    if (auto widget = parentWidget()) {
        const auto parentRect = widget->rect();
        const QSize sceneSize = scene()->sceneRect().size().toSize();
        QRect currentRect { QPoint(0, 0),
            sceneSize.scaled(parentRect.width() / kDefaultScaleFactor, parentRect.height(), Qt::KeepAspectRatio) };
        currentRect.moveTopRight(parentRect.topRight());
        setGeometry(currentRect);
    }
}

void MiniMap::updateScaling()
{
    if (const auto graphicsScene = scene()) {
        fitInView(graphicsScene->sceneRect());
        viewport()->update();
    }
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

    d->m_mouseStart = kOutOfView;
    d->m_mouseFinish = kOutOfView;
}

void MiniMap::processMouseInput()
{
    if (d->m_mouseFinish == kOutOfView) {
        return;
    }

    QPointF newCenter;
    if (d->m_mouseStart != d->m_mouseFinish) {
        // It's a drag. In general, we should shift the main viewport accordingly to the shift of the minimap's
        // preview. But let's just center the main view on the current mouse pos instead — the UX flow seems to be
        // smooth, but the code is way much simplier. This seems to be enough, at least for beginning.

        const auto sceneOffset = mapToScene(d->m_mouseFinish) - mapToScene(d->m_mouseStart);
        const auto viewportCenterOnScene = d->m_view->mapToScene(d->m_view->viewport()->rect().center());
        newCenter = viewportCenterOnScene + sceneOffset;
        d->m_mouseStart = d->m_mouseFinish;
    } else { // else it's a click - recenter the main view on it
        newCenter = mapToScene(d->m_mouseStart);
    }

    d->m_view->centerOn(newCenter);
}

void MiniMap::drawItems(QPainter *painter, int numItems, QGraphicsItem **items, const QStyleOptionGraphicsItem *options)
{
    for (int idx = 0; idx < numItems; ++idx) {
        if (items[idx]->type() == GripPoint::Type)
            continue;

        painter->save();
        painter->setMatrix(items[idx]->sceneMatrix(), true);
        items[idx]->paint(painter, &options[idx], this);
        painter->restore();
    }
}

void MiniMap::drawForeground(QPainter *painter, const QRectF &rect)
{
    QPainterPath dimmedOverlay;
    dimmedOverlay.addRect(rect);
    dimmedOverlay.addRect(mappedViewportOnScene());
    painter->setPen(Qt::NoPen);
    painter->fillPath(dimmedOverlay, dimColor());
}

bool MiniMap::eventFilter(QObject *object, QEvent *event)
{
    if (object == parentWidget() && event->type() == QEvent::Resize) {
        adjustGeometry();
    }
    return QGraphicsView::eventFilter(object, event);
}

} // namespace ui
} // namespace shared
