/*
   Copyright (C) 2020 Denis Gofman - <sendevent@gmail.com>

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
#include <QTimer>

namespace shared {
namespace ui {

/*!
 * \brief A "hash" holder for rendered scene state.
 *
 * While the QGraphicsScene::changed is emitted for *any* content change,
 * it's impossible to detect what the change actually was.
 * Thus, when a selection of an item shows grippoints, it triggers the QGraphicsScene::changed
 * (In our case, in IV even just a mouseover triggers it, I'm not sure why though).
 *
 * To minimize count of calls to QGraphicsScene::render here is some heuristic for filtering changes:
 *
 * Scene rect;
 * Items count;
 * Item geometry; // :(
 */
struct SceneHash {
    QRectF m_rect;
    int m_count;

    using ItemsGeoms = QMap<const QGraphicsItem *, QRectF>;
    ItemsGeoms m_itemsGeometry;

    void commitScene(const QGraphicsScene *const scene)
    {
        commitSceneRect(scene);
        commitSceneItemsCount(scene);
        commitSceneItems(scene);
    }

    void commitSceneRect(const QGraphicsScene *const scene) { m_rect = scene->sceneRect(); }

    void commitSceneItemsCount(const QGraphicsScene *const scene) { m_count = scene->items().size(); }

    void commitSceneItems(const QGraphicsScene *const scene)
    {
        m_itemsGeometry.clear();
        for (const QGraphicsItem *const item : scene->items())
            m_itemsGeometry[item] = item->sceneBoundingRect();
    }
};

struct MiniMapPrivate {
    MiniMapPrivate()
        : m_renderTimer(new QTimer)
    {
        static constexpr int renderDelayMs = 100;
        m_renderTimer->setInterval(renderDelayMs);
        m_renderTimer->setSingleShot(true);
    }

    ~MiniMapPrivate()
    {
        delete m_renderTimer;
        m_renderTimer = nullptr;
    }

    bool isReady() const { return m_view && m_scene; }

    bool ensureSceneChanged()
    {
        QRectF newSceneRect = m_scene->sceneRect();
        if (m_sceneHash.m_rect != newSceneRect) {
            m_sceneHash.commitScene(m_scene);
            return true;
        }

        const QList<QGraphicsItem *> items = m_scene->items();
        const int count = items.count();
        if (m_sceneHash.m_count != count) {
            m_sceneHash.commitScene(m_scene);
            return true;
        }

        SceneHash::ItemsGeoms itemGeoms;
        for (int i = 0; i < count; ++i) {
            const QGraphicsItem *const item = items.at(i);
            const QRectF &itemGeometry = item->sceneBoundingRect();
            const QRectF &hashedItemGeometry = m_sceneHash.m_itemsGeometry.value(item, {});
            if (itemGeometry != hashedItemGeometry) {
                m_sceneHash.commitScene(m_scene);
                return true;
            }
        }

        return false;
    }

    QPointer<QGraphicsView> m_view { nullptr };
    QPointer<QGraphicsScene> m_scene { nullptr };
    QTimer *m_renderTimer { nullptr };

    QPixmap m_sceneContent;
    QRect m_sceneViewport;

    QPixmap m_display;

    SceneHash m_sceneHash;
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

void MiniMap::setupSourceView(QGraphicsView *view)
{
    if (d->m_view) {
        if (d->m_scene) {
            disconnect(d->m_scene, &QGraphicsScene::changed, this, &MiniMap::onSceneUpdated);
            d->m_scene = nullptr;
        }

        //        disconnect(d->m_view);
        d->m_view = nullptr;
    }

    d->m_view = view;

    if (d->m_view) {
        //        connect(d->m_view,&QGraphicsView)
        if (auto *scene = d->m_view->scene()) {
            d->m_scene = scene;
            connect(d->m_scene, &QGraphicsScene::changed, this, &MiniMap::onSceneUpdated);
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
    if (!d->ensureSceneChanged()) {
        return;
    }

    d->m_sceneContent = QPixmap(d->m_scene->sceneRect().size().toSize());
    d->m_sceneContent.fill(Qt::transparent);
    QPainter p(&d->m_sceneContent);
    d->m_scene->render(&p);

    composeMap();
}

void MiniMap::updateViewportFrame()
{
    composeMap();
}

void MiniMap::composeMap()
{
    const QPixmap scaledContent = d->m_sceneContent.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    d->m_display = QPixmap(size());
    d->m_display.fill(Qt::white);

    QPainter painter(&d->m_display);
    painter.drawPixmap(d->m_display.rect(), scaledContent, scaledContent.rect());
    update();
}

} // namespace ui
} // namespace shared
