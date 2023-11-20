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

#include "settingsmanager.h"
#include "ui/grippoint.h"

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QPointer>
#include <QScopedPointer>
#include <QScrollBar>
#include <QTimer>
#include <QUndoStack>

namespace shared {
namespace ui {

#define LOG qDebug() << Q_FUNC_INFO

static constexpr QPoint kOutOfView { -1, -1 };
static const QColor kDefaultDimColor { 0x00, 0x00, 0x00, 0x88 };
static constexpr qreal kDefaultScaleFactor { 6 };
static const qreal kOpacityRegular { 0.5 };
static const qreal kOpacityHovered { 0.95 };
static const qreal kOpacityRelocating { 0.5 };
static const qreal kOpacityStickable { 0.75 };
static const int kRelocatingThreshold { 5 };
static const MiniMap::Location kDefaultLocation { MiniMap::Location::NorthEast };

struct MiniMapPrivate {
    QPointer<QGraphicsView> m_view;
    QColor m_dimColor { kDefaultDimColor };
    QPoint m_mouseStart { kOutOfView };
    QPoint m_mouseFinish { kOutOfView };
    MiniMap::Location m_location { kDefaultLocation };
    bool m_relocating = false;
    QScopedPointer<QGraphicsOpacityEffect> m_opacity;
};

MiniMap::MiniMap(QWidget *parent)
    : QGraphicsView(parent)
    , d(new MiniMapPrivate)
{
    setOptimizationFlag(QGraphicsView::IndirectPainting);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setMouseTracking(true);

    const int location = shared::SettingsManager::load<int>(
            shared::SettingsManager::Common::MinimapLocation, static_cast<int>(kDefaultLocation));
    setLocation(static_cast<MiniMap::Location>(location));

    d->m_opacity.reset(new QGraphicsOpacityEffect(this));

    setOpacity(kOpacityRegular);
    setGraphicsEffect(d->m_opacity.data());
}

MiniMap::~MiniMap() { }

void MiniMap::setupSourceView(QGraphicsView *view)
{
    Q_ASSERT(view && view->scene());

    if (d->m_view) {
        d->m_view->viewport()->removeEventFilter(this);
        for (auto scrollBar : { d->m_view->verticalScrollBar(), d->m_view->horizontalScrollBar() }) {
            disconnect(scrollBar, &QScrollBar::valueChanged, viewport(), qOverload<>(&QWidget::update));
            disconnect(scrollBar, &QScrollBar::rangeChanged, viewport(), qOverload<>(&QWidget::update));
        }
        d->m_view.clear();
    }

    d->m_view = view;
    setParent(view);

    if (d->m_view) {
        d->m_view->viewport()->installEventFilter(this);
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

bool MiniMap::checkMouseEvent(
        QMouseEvent *e, Qt::MouseButton current, Qt::MouseButton started, Qt::KeyboardModifier mod) const
{
    return e->button() == current && e->buttons() == started && e->modifiers() == mod;
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
    if (d->m_view) {
        const auto parentRect = d->m_view->rect();
        const QSize sceneSize = scene()->sceneRect().size().toSize();
        QRect currentRect { QPoint(0, 0),
            sceneSize.scaled(parentRect.size() / kDefaultScaleFactor, Qt::KeepAspectRatio) };
        currentRect = stickToEdge(location(), currentRect, MiniMap::Stickiness::Strict);
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

    d->m_relocating = checkMouseEvent(event, Qt::LeftButton, Qt::LeftButton, Qt::ShiftModifier);

    if (checkMouseEvent(event, Qt::LeftButton, Qt::LeftButton)) {
        updateCursorInMappedViewport(event->pos(), Qt::ClosedHandCursor);
        d->m_mouseStart = event->pos();
    } else if (d->m_relocating) {
        qApp->setOverrideCursor(Qt::ClosedHandCursor);
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
    } else if (d->m_relocating) {
        followMouse(event->globalPosition());
    }
}

void MiniMap::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    if (checkMouseEvent(event, Qt::LeftButton, Qt::NoButton)) {
        d->m_mouseFinish = event->pos();
        if (!d->m_relocating) {
            processMouseInput();
        }
    }

    updateCursorInMappedViewport(event->pos(), Qt::OpenHandCursor);

    d->m_mouseStart = kOutOfView;
    d->m_mouseFinish = kOutOfView;

    adjustGeometry();

    if (d->m_relocating) {
        qApp->restoreOverrideCursor();
        d->m_relocating = false;
        setOpacity(kOpacityRegular);
    }
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
        if (items[idx]->type() == GripPoint::Type || !items[idx]->isVisible()) {
            continue;
        }

        painter->save();
        painter->setTransform(items[idx]->sceneTransform(), true);
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
    const bool hasViewport = d->m_view && d->m_view->viewport();
    if (hasViewport && object == d->m_view->viewport() && event->type() == QEvent::Resize) {
        QTimer::singleShot(0, this, &MiniMap::adjustGeometry);
    }
    return QGraphicsView::eventFilter(object, event);
}

MiniMap::Location MiniMap::posToLocation(const QPointF &pos, const QRectF &vpr) const
{
    auto lineLength = [&pos](const QPointF &end) { return QLineF(pos, end).length(); };

    typedef qreal Distance;
    typedef QPair<MiniMap::Location, Distance> PosInfo;
    QVector<PosInfo> distances {
        { MiniMap::Location::North, lineLength(QPointF(vpr.center().x(), vpr.top())) },
        { MiniMap::Location::NorthEast, lineLength(vpr.topRight()) },
        { MiniMap::Location::East, lineLength(QPointF(vpr.right(), vpr.center().y())) },
        { MiniMap::Location::SouthEast, lineLength(vpr.bottomRight()) },
        { MiniMap::Location::South, lineLength(QPointF(vpr.center().x(), vpr.bottom())) },
        { MiniMap::Location::SouthWest, lineLength(vpr.bottomLeft()) },
        { MiniMap::Location::West, lineLength(QPointF(vpr.left(), vpr.center().y())) },
        { MiniMap::Location::NorthWest, lineLength(vpr.topLeft()) },
    };

    std::sort(std::begin(distances), std::end(distances),
            [](const PosInfo &lhs, const PosInfo &rhs) { return lhs.second < rhs.second; });

    return distances.constFirst().first;
}

void MiniMap::followMouse(const QPointF &globalMouse)
{
    auto viewport = d->m_view;
    Q_ASSERT(viewport != nullptr);

    const QPointF &localMouse = viewport->mapFromGlobal(globalMouse);
    const QRectF &viewportRect = viewport->viewport()->rect();

    const MiniMap::Location &newLocation = posToLocation(localMouse, viewportRect);
    setLocation(newLocation);

    const QRect &geom = geometry();
    QRect shiftedGeom = stickToEdge(newLocation, geom, MiniMap::Stickiness::Dynamic);
    const QPoint &shiftedCenter = shiftedGeom.center();
    QPoint freeCenter;

    switch (location()) {
    case MiniMap::Location::North:
    case MiniMap::Location::South: {
        freeCenter = QPoint(localMouse.x(), shiftedCenter.y());
        break;
    }
    case MiniMap::Location::West:
    case MiniMap::Location::East: {
        freeCenter = QPoint(shiftedCenter.x(), localMouse.y());
        break;
    }
    default:
        break;
    }

    if (!freeCenter.isNull()) {
        shiftedGeom.moveCenter(freeCenter);
    }

    setGeometry(shiftedGeom);

    const auto &targetRect =
            stickToEdge(newLocation, geom, MiniMap::Stickiness::Strict)
                    .adjusted(-kRelocatingThreshold, -kRelocatingThreshold, kRelocatingThreshold, kRelocatingThreshold);
    const qreal opacity = targetRect.contains(shiftedGeom) ? kOpacityStickable : kOpacityRelocating;
    setOpacity(opacity);
}

QRect MiniMap::stickToEdge(MiniMap::Location edge, const QRect &srcGeometry, MiniMap::Stickiness flow) const
{
    auto viewport = d->m_view;
    Q_ASSERT(viewport != nullptr);

    const QRectF &vpr = viewport->viewport()->rect();
    const QPoint &vpc = vpr.center().toPoint();
    const bool isStrict = flow == MiniMap::Stickiness::Strict;

    QRect shiftedRect(srcGeometry);
    QPoint shiftedCenter;

    switch (edge) {
    case MiniMap::Location::North: {
        shiftedRect.moveTop(vpr.top());
        shiftedCenter = QPoint(vpc.x(), shiftedRect.center().y());
        break;
    }
    case MiniMap::Location::NorthEast: {
        shiftedRect.moveTopRight(vpr.topRight().toPoint());
        break;
    }
    case MiniMap::Location::East: {
        shiftedRect.moveRight(vpr.right());
        shiftedCenter = QPoint(shiftedRect.center().x(), vpc.y());
        break;
    }
    case MiniMap::Location::SouthEast: {
        shiftedRect.moveBottomRight(vpr.bottomRight().toPoint());
        break;
    }
    case MiniMap::Location::South: {
        shiftedRect.moveBottom(vpr.bottom());
        shiftedCenter = QPoint(vpc.x(), shiftedRect.center().y());
        break;
    }
    case MiniMap::Location::SouthWest: {
        shiftedRect.moveBottomLeft(vpr.bottomLeft().toPoint());
        break;
    }
    case MiniMap::Location::West: {
        shiftedRect.moveLeft(vpr.left());
        shiftedCenter = QPoint(shiftedRect.center().x(), vpc.y());
        break;
    }
    case MiniMap::Location::NorthWest: {
        shiftedRect.moveTopLeft(vpr.topLeft().toPoint());
        break;
    }
    }

    if (!shiftedCenter.isNull() && isStrict) {
        shiftedRect.moveCenter(shiftedCenter);
    }

    return shiftedRect;
}

MiniMap::Location MiniMap::location() const
{
    return d->m_location;
}

void MiniMap::setLocation(MiniMap::Location location)
{
    if (this->location() != location) {
        d->m_location = location;
        shared::SettingsManager::store<int>(
                shared::SettingsManager::Common::MinimapLocation, static_cast<int>(this->location()));
    }
}

void MiniMap::enterEvent(QEnterEvent *event)
{
    QGraphicsView::enterEvent(event);
    setOpacity(kOpacityHovered);
}

void MiniMap::leaveEvent(QEvent *event)
{
    QGraphicsView::leaveEvent(event);
    setOpacity(kOpacityRegular);
}

void MiniMap::setOpacity(qreal opacity)
{
    if (d->m_opacity->opacity() != opacity) {
        d->m_opacity->setOpacity(opacity);
    }
}

} // namespace ui
} // namespace shared
