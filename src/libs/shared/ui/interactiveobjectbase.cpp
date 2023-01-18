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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "interactiveobjectbase.h"

#include "delayedsignal.h"
#include "grippointshandler.h"
#include "highlightrectitem.h"

#include <QDebug>
#include <QPainter>
#include <QPen>

namespace shared {
namespace ui {

struct InteractiveObjectBase::InteractiveObjectBasePrivate {
    InteractiveObjectBasePrivate()
        : selectedPen(Qt::black, 2)
    {
    }

    QPointer<shared::ui::GripPointsHandler> gripPointsHandler;
    QRectF boundingRect;
    QPointF prevPos;
    bool layoutDirty = false;

    bool highlightable = false;
    QPointer<HighlightRectItem> highlighter;
    QPen selectedPen;

    shared::DelayedSignal *rebuildLayoutSignal = nullptr;
};

InteractiveObjectBase::InteractiveObjectBase(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , d(new InteractiveObjectBasePrivate)
{
    d->rebuildLayoutSignal = new shared::DelayedSignal(this);
    connect(d->rebuildLayoutSignal, &shared::DelayedSignal::triggered, this,
            &InteractiveObjectBase::instantLayoutUpdate);
}

InteractiveObjectBase::~InteractiveObjectBase()
{
    delete d;
    d = nullptr;
}

void InteractiveObjectBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (isSelected()) {
        painter->setPen(d->selectedPen);
        painter->drawRect(d->boundingRect);
    }
}

QRectF InteractiveObjectBase::boundingRect() const
{
    return d->boundingRect;
}

void InteractiveObjectBase::setBoundingRect(const QRectF &newRect)
{
    if (newRect == d->boundingRect) {
        return;
    }
    prepareGeometryChange();

    d->boundingRect = newRect;
    updateGripPoints();
    Q_EMIT boundingBoxChanged();
}

GripPointsHandler *InteractiveObjectBase::gripPointsHandler() const
{
    return d->gripPointsHandler;
}

GripPoint *InteractiveObjectBase::gripPointItem(GripPoint::Location location) const
{
    if (!d->gripPointsHandler) {
        InteractiveObjectBase *me = const_cast<InteractiveObjectBase *>(this);
        me->initGripPoints();
    }
    return d->gripPointsHandler->gripPoint(location);
}

bool InteractiveObjectBase::isHovered() const
{
    if (!isUnderMouse())
        return false;

    return d->gripPointsHandler && d->gripPointsHandler->isVisible();
}

void InteractiveObjectBase::enableHighlight()
{
    doHighlighting(Qt::green, false, 1000);
}

void InteractiveObjectBase::disableHighlight()
{
    doHighlighting(Qt::red, false);
}

/*!
   \brief InteractiveObjectBase::doHighlighting
   \param color
   \param permanent if false, the highlight is shown as animation and fades out within a short time
   \param duration The time the highlight is visible
 */
void InteractiveObjectBase::doHighlighting(const QColor &color, bool permanent, int duration)
{
    if (!d->highlightable) {
        return;
    }

    // Delete the old one, if it is there
    clearHighlight();

    if (HighlightRectItem *highlighter = createHighlighter()) {
        QColor targetColor(color);
        QPen p(targetColor);
        p.setWidthF(1.);
        highlighter->setPen(p);
        targetColor.setAlphaF(0.25);
        highlighter->setBrush(targetColor);

        if (!permanent)
            highlighter->highlight(duration);
    }
}

void InteractiveObjectBase::clearHighlight()
{
    delete d->highlighter;
    d->highlighter = nullptr;
}

bool InteractiveObjectBase::isHighlighting() const
{
    return d->highlighter != nullptr;
}

QPointF InteractiveObjectBase::centerInScene() const
{
    return sceneBoundingRect().center();
}

bool InteractiveObjectBase::isHighlightable() const
{
    return d->highlightable;
}

void InteractiveObjectBase::setHighlightable(bool highlightable)
{
    if (highlightable != d->highlightable) {
        d->highlightable = highlightable;
        clearHighlight();
    }
}

/*!
  \brief shared::ui::VEInteractiveObject::postCreatePolishing
  Entry point for new (manually added) item fine customization - show configuration dialog.
  Designed to be called from a class derived from BaseCreatorTool.
  Default implementation tries to found a TextItem child. If found, current item
  set to be the only selected item, input focus forced to its first text field
  (to edit default name, etc).
*/
void InteractiveObjectBase::postCreatePolishing() { }

void InteractiveObjectBase::updateGripPoints()
{
    if (d->gripPointsHandler)
        d->gripPointsHandler->updateLayout();
}

QPen InteractiveObjectBase::selectedPen() const
{
    return d->selectedPen;
}

/*!
   \brief InteractiveObjectBase::scheduleLayoutUpdate
   Triggers a geometry update of that item. That might be needed if for example the underlying entity changes some of
   it's data.
   The actual update is done in the derived items in the virtual function \see
   shared::ui::VEInteractiveObject::rebuildLayout. To perform update instantly use the \see
   shared::ui::VEInteractiveObject::instantLayoutUpdate
 */
void InteractiveObjectBase::scheduleLayoutUpdate()
{
    if (d->layoutDirty) {
        return;
    }
    d->layoutDirty = true;
    d->rebuildLayoutSignal->triggerSignal();
}

/*!
   \brief InteractiveObjectBase::instantLayoutUpdate
   Triggers a geometry update of that item. That might be needed if for example the underlying entity changes some of
   it's data.
   Unlike the \see shared::ui::VEInteractiveObject::scheduleLayoutUpdate actual update is performed instantly
 */
void InteractiveObjectBase::instantLayoutUpdate()
{
    const QRectF oldBounds = boundingRect();

    rebuildLayout();
    d->layoutDirty = false;
    d->rebuildLayoutSignal->stop();

    if (oldBounds != boundingRect()) {
        Q_EMIT boundingBoxChanged();
    }
    update();
}

void InteractiveObjectBase::onManualMoveStart(GripPoint *, const QPointF &) { }

void InteractiveObjectBase::onManualMoveProgress(GripPoint *, const QPointF &, const QPointF &) { }

void InteractiveObjectBase::onManualMoveFinish(GripPoint *, const QPointF &, const QPointF &) { }

void InteractiveObjectBase::onManualGripPointAdd(GripPoint *) { }

void InteractiveObjectBase::onManualGripPointRemove(GripPoint *) { }

void InteractiveObjectBase::onManualResizeStart(GripPoint *, const QPointF &) { }

void InteractiveObjectBase::onManualResizeProgress(GripPoint *, const QPointF &, const QPointF &) { }

void InteractiveObjectBase::onManualResizeFinish(GripPoint *, const QPointF &, const QPointF &) { }

void InteractiveObjectBase::onSelectionChanged(bool) { }

void InteractiveObjectBase::initGripPoints()
{
    if (d->gripPointsHandler) {
        return;
    }

    d->gripPointsHandler = new GripPointsHandler(this);
    d->gripPointsHandler->setZValue(0);

    connect(d->gripPointsHandler, &GripPointsHandler::manualGeometryChangeStart, this,
            &InteractiveObjectBase::gripPointPressed);
    connect(d->gripPointsHandler, &GripPointsHandler::manualGeometryChangeProgress, this,
            &InteractiveObjectBase::gripPointMoved);
    connect(d->gripPointsHandler, &GripPointsHandler::manualGeometryChangeFinish, this,
            &InteractiveObjectBase::gripPointReleased);

    connect(d->gripPointsHandler, &GripPointsHandler::visibleChanged, this, [this]() {
        if (d->gripPointsHandler && !d->gripPointsHandler->isVisible()) {
            // it's not a thing directly added to the scene, so just delete is enough
            delete d->gripPointsHandler;
        }
    });
}

void InteractiveObjectBase::showGripPoints()
{
    initGripPoints();
    d->gripPointsHandler->showAnimated();
}

void InteractiveObjectBase::hideGripPoints()
{
    if (d->gripPointsHandler)
        d->gripPointsHandler->hideAnimated();
}


QVariant InteractiveObjectBase::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemSelectedChange:
        onSelectionChanged(value.toBool());
        break;
    case QGraphicsItem::ItemPositionChange:
        d->prevPos = pos();
        break;
    case QGraphicsItem::ItemPositionHasChanged:
        Q_EMIT relocated(d->prevPos, pos());
        break;
    default:
        break;
    }

    return QGraphicsObject::itemChange(change, value);
}

/*!
   \brief InteractiveObjectBase::rebuildLayout
   In this function updates to the geometry, content etc. is done
   This is usually triggered by the function \see shared::ui::VEInteractiveObject::updateLayout
 */
void InteractiveObjectBase::rebuildLayout() { }

void InteractiveObjectBase::gripPointPressed(GripPoint *gp, const QPointF &at)
{
    if (gp->isMover()) {
        onManualMoveStart(gp, at);
    } else {
        onManualResizeStart(gp, at);
    }
}

void InteractiveObjectBase::gripPointMoved(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->isMover()) {
        onManualMoveProgress(gp, from, to);
    } else {
        onManualResizeProgress(gp, from, to);
    }
}

void InteractiveObjectBase::gripPointReleased(GripPoint *gp, const QPointF &pressedAt, const QPointF &releasedAt)
{
    if (gp->isMover()) {
        onManualMoveFinish(gp, pressedAt, releasedAt);
    } else {
        onManualResizeFinish(gp, pressedAt, releasedAt);
    }
}

HighlightRectItem *InteractiveObjectBase::createHighlighter()
{
    auto highlighter = new HighlightRectItem(this);
    highlighter->setPath(shape());
    connect(highlighter, &HighlightRectItem::highlighted, highlighter, &QObject::deleteLater);

    delete d->highlighter.data();
    d->highlighter = highlighter;

    return highlighter;
}

}
}
