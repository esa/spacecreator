#include "interactiveobjectbase.h"
#include "grippointshandler.h"
#include "delayedsignal.h"
#include "highlightrectitem.h"

#include <QPainter>
#include <QPen>

namespace shared {
namespace ui {

struct InteractiveObjectBase::InteractiveObjectBasePrivate
{
    InteractiveObjectBasePrivate()
        : selectedPen(Qt::black, 2)
    {}

    QPointer<shared::ui::GripPointsHandler> gripPointsHandler;
    QRectF boundingRect;
    QPointF prevPos;
    bool layoutDirty = false;

    bool highlightable = false;
    QPointer<HighlightRectItem> highlighter;
    QPen selectedPen;

    utils::DelayedSignal* rebuildLayoutSignal;
};

InteractiveObjectBase::InteractiveObjectBase(QGraphicsItem* parent)
    : QGraphicsObject(parent), d(new InteractiveObjectBasePrivate)
{
    d->rebuildLayoutSignal = new utils::DelayedSignal(this);
    connect(d->rebuildLayoutSignal, &utils::DelayedSignal::triggered, this, &InteractiveObjectBase::instantLayoutUpdate);
}

void InteractiveObjectBase::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (isSelected()) {
        auto pen = painter->pen();
        painter->setPen(d->selectedPen);
        painter->drawRect(d->boundingRect);
    }
}

QRectF InteractiveObjectBase::boundingRect() const
{
    return d->boundingRect;
}

bool InteractiveObjectBase::isHovered() const
{
    if (!isUnderMouse())
        return false;

    return d->gripPointsHandler && d->gripPointsHandler->isVisible();
}

void InteractiveObjectBase::highlightConnected()
{
    doHighlighting(Qt::green, false);
}

void InteractiveObjectBase::highlightDisconnected()
{
    doHighlighting(Qt::red, false);
}

/*!
   \brief InteractiveObjectBase::doHighlighting
   \param color
   \param permanent if false, the highlight is shown as animation and fades out within a short time
 */
void InteractiveObjectBase::doHighlighting(const QColor &color, bool permanent)
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
            highlighter->highlight();
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
  \brief InteractiveObject::postCreatePolishing
  Entry point for new (manually added) item fine customization - show configuration dialog.
  Designed to be called from a class derived from BaseCreatorTool.
  Default implementation tries to found a TextItem child. If found, current item
  set to be the only selected item, input focus forced to its first text field
  (to edit default name, etc).
*/
void InteractiveObjectBase::postCreatePolishing() {
}

void InteractiveObjectBase::updateGripPoints()
{
    if (d->gripPointsHandler)
        d->gripPointsHandler->updateLayout();
}

/*!
   \brief InteractiveObjectBase::scheduleLayoutUpdate
   Triggers a gemoetry update of that item. That might be needed if for example the underlaying entity changes some of
   it's data.
   The actual update is done in the derived itmes in the virtual function \see InteractiveObject::rebuildLayout.
   To perform update instantly use the \see InteractiveObject::instantLayoutUpdate
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
   Triggers a gemoetry update of that item. That might be needed if for example the underlaying entity changes some of
   it's data.
   Unlike the \see InteractiveObject::scheduleLayoutUpdate actual update is performed instantly
 */
void InteractiveObjectBase::instantLayoutUpdate()
{
    const QRectF oldBounds = boundingRect();

    rebuildLayout();
    d->layoutDirty = false;

    if (oldBounds != boundingRect()) {
        Q_EMIT boundingBoxChanged();
    }
    update();
}

void InteractiveObjectBase::onManualMoveStart(GripPoint*, const QPointF&)
{
}

void InteractiveObjectBase::onManualMoveProgress(GripPoint*, const QPointF&, const QPointF&)
{
}

void InteractiveObjectBase::onManualMoveFinish(GripPoint*, const QPointF&, const QPointF&)
{
}

void InteractiveObjectBase::onManualGripPointAdd(GripPoint*)
{
}

void InteractiveObjectBase::onManualGripPointRemove(GripPoint*)
{
}

void InteractiveObjectBase::onManualResizeStart(GripPoint*, const QPointF&)
{
}

void InteractiveObjectBase::onManualResizeProgress(GripPoint*, const QPointF&, const QPointF&)
{
}

void InteractiveObjectBase::onManualResizeFinish(GripPoint*, const QPointF&, const QPointF&)
{
}

void InteractiveObjectBase::onSelectionChanged(bool)
{
}

void InteractiveObjectBase::initGripPoints()
{
    if (d->gripPointsHandler) {
        return;
    }

    d->gripPointsHandler = new GripPointsHandler(this);
    d->gripPointsHandler->setZValue(0);

    connect(d->gripPointsHandler, &GripPointsHandler::manualGeometryChangeStart, this, &InteractiveObjectBase::gripPointPressed);
    connect(d->gripPointsHandler, &GripPointsHandler::manualGeometryChangeProgress, this, &InteractiveObjectBase::gripPointMoved);
    connect(d->gripPointsHandler, &GripPointsHandler::manualGeometryChangeFinish, this, &InteractiveObjectBase::gripPointReleased);

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
   This is usually triggered by the funtion \see InteractiveObject::updateLayout
 */
void InteractiveObjectBase::rebuildLayout() {
}

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

HighlightRectItem* InteractiveObjectBase::createHighlighter()
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
