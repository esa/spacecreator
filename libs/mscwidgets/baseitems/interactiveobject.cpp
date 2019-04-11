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

#include "interactiveobject.h"

#include "baseitems/textitem.h"
#include "common/highlightrectitem.h"
#include "grippointshandler.h"
#include "mscentity.h"

#include <QBrush>
#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <functional>

namespace msc {

/*!
  \class msc::InteractiveObject
  \brief Movable/resizable QGraphicsObject with geometry change notifications.

  \inmodule MscWidgets

*/

InteractiveObject::InteractiveObject(msc::MscEntity *entity, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_entity(entity)
{
    setAcceptHoverEvents(true);

    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges);

    setCursor(Qt::ArrowCursor);
}

void InteractiveObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (isSelected()) {
        painter->save();
        QPen selectionPen(Qt::black, 3);
        painter->setPen(selectionPen);
        painter->drawRect(m_boundingRect);
        painter->restore();
    }
}

QRectF InteractiveObject::boundingRect() const
{
    static const QMarginsF selectionMargins(1, 1, 1, 1);

    if (isSelected()) {
        return m_boundingRect.marginsAdded(selectionMargins);
    }

    return m_boundingRect;
}

void InteractiveObject::gripPointMoved(GripPoint::Location gripPos, const QPointF &from, const QPointF &to)
{
    if (m_gripPoints)
        if (GripPoint *gripPnt = m_gripPoints->gripPoint(gripPos))
            handleGripPointMovement(gripPnt, from, to);
}

/*!
   \brief InteractiveObject::rebuildLayout
   In this function updates to the geometry, content etc. is done
   This is usually triggered by the funtion \see InteractiveObject::updateLayout
 */
void InteractiveObject::rebuildLayout() {}

void InteractiveObject::handleGripPointMovement(GripPoint *grip, const QPointF &from, const QPointF &to)
{
    if (grip->isMover())
        onMoveRequested(grip, from, to);
    else
        onResizeRequested(grip, from, to);
}

void InteractiveObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    m_storedZ = zValue();
    setZValue(m_storedZ + 1.);

    prepareHoverMark();
    updateGripPoints();
    QGraphicsObject::hoverEnterEvent(event);
}

void InteractiveObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    setZValue(m_storedZ);

    if (m_gripPoints)
        m_gripPoints->hideAnimated();

    QGraphicsObject::hoverLeaveEvent(event);
}

void InteractiveObject::updateGripPoints()
{
    if (m_gripPoints)
        m_gripPoints->updateLayout();
}

QVariant InteractiveObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case QGraphicsItem::ItemPositionChange:
        m_prevPos = pos();
        break;
    case QGraphicsItem::ItemPositionHasChanged:
        Q_EMIT relocated(m_prevPos, pos());
        break;
    default:
        break;
    }

    return QGraphicsObject::itemChange(change, value);
}

bool InteractiveObject::isHovered() const
{
    if (!isUnderMouse())
        return false;

    return m_gripPoints && m_gripPoints->isVisible();
}

HighlightRectItem *InteractiveObject::createHighlighter()
{
    if (m_highlighter)
        delete m_highlighter;
    m_highlighter = new HighlightRectItem(this);
    connect(m_highlighter, &HighlightRectItem::highlighted, m_highlighter, &QObject::deleteLater);
    connect(m_highlighter, &QObject::destroyed, this, [this]() { m_highlighter = nullptr; });

    m_highlighter->setRect(m_boundingRect);

    return m_highlighter;
}

bool InteractiveObject::isHighlightable() const
{
    return m_highlightable;
}

void InteractiveObject::setHighlightable(bool highlightable)
{
    m_highlightable = highlightable;
    clearHighlight();
}

QPair<QPointF, bool> InteractiveObject::commentPoint() const
{
    const QRectF br = sceneBoundingRect();
    return qMakePair(QPointF(br.right(), br.center().y()), false);
}

void InteractiveObject::highlightConnected()
{
    doHighlighting(Qt::green, false);
}

void InteractiveObject::highlightDisconnected()
{
    doHighlighting(Qt::red, false);
}

/*!
   \brief InteractiveObject::doHighlighting
   \param color
   \param permanent if false, the highlight is shown as animation and fades out within a short time
 */
void InteractiveObject::doHighlighting(const QColor &color, bool permanent)
{
    if (!m_highlightable) {
        return;
    }

    if (HighlightRectItem *highlighter = createHighlighter()) {
        QColor targetColor(color);
        QPen p(targetColor);
        p.setWidthF(3.);
        highlighter->setPen(p);
        targetColor.setAlphaF(0.25);
        highlighter->setBrush(targetColor);

        if (!permanent)
            highlighter->highlight();
    }
}

void InteractiveObject::clearHighlight()
{
    if (m_highlighter) {
        delete m_highlighter;
        m_highlighter = nullptr;
    }
}

bool InteractiveObject::isHighlighting() const
{
    return m_highlighter != nullptr;
}

QPointF InteractiveObject::centerInScene() const
{
    return sceneBoundingRect().center();
}

MscEntity *InteractiveObject::modelEntity() const
{
    return m_entity.data();
}

/*!
   \brief InteractiveObject::scheduleLayoutUpdate
   Triggers a gemoetry update of that item. That might be needed if for example the underlaying entity changes some of
   it's data.
   The actual update is done in the derived itmes in the virtual function \see InteractiveObject::rebuildLayout.
   To perform update instantly use the \see InteractiveObject::instantLayoutUpdate
 */
void InteractiveObject::scheduleLayoutUpdate()
{
    if (m_layoutDirty)
        return;
    m_layoutDirty = true;
    QMetaObject::invokeMethod(this, "instantLayoutUpdate", Qt::QueuedConnection);
}

/*!
   \brief InteractiveObject::instantLayoutUpdate
   Triggers a gemoetry update of that item. That might be needed if for example the underlaying entity changes some of
   it's data.
   Unlike the \see InteractiveObject::scheduleLayoutUpdate actual update is performed instantly
 */
void InteractiveObject::instantLayoutUpdate()
{
    const QRectF oldBounds = boundingRect();

    rebuildLayout();
    m_layoutDirty = false;

    if (oldBounds != boundingRect()) {
        Q_EMIT boundingBoxChanged();
    }
    update();
}

void InteractiveObject::prepareHoverMark()
{
    if (!m_gripPoints) {
        m_gripPoints = new GripPointsHandler(this);
        m_gripPoints->setZValue(0);

        connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeProgress, this,
                &InteractiveObject::gripPointMoved);
        connect(m_gripPoints, &GripPointsHandler::visibleChanged, this, [this]() {
            if (m_gripPoints && !m_gripPoints->isVisible())
                delete m_gripPoints; // it's not a thing directly added to the scene, so just delete
                                     // is enough
        });
        if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::Location::Center))
            gp->setGripType(GripPoint::GripType::Mover);
    }

    m_gripPoints->showAnimated();
}

/*!
  \brief InteractiveObject::postCreatePolishing
  Entry point for new (manually added) item fine customization - show configuration dialog.
  Designed to be called from a class derived from BaseCreatorTool.
  Default implementation tries to found a TextItem child. If found, current item
  set to be the only selected item, input focus forced to its first text field
  (to edit default name, etc).
*/
void InteractiveObject::postCreatePolishing()
{
    std::function<bool(QGraphicsItem *)> findTextItem;
    findTextItem = [&](QGraphicsItem *where) {
        for (QGraphicsItem *child : where->childItems()) {
            if (TextItem *textItem = dynamic_cast<TextItem *>(child)) {
                scene()->clearSelection();
                setSelected(true);
                textItem->enableEditMode();
                return true;
            } else if (findTextItem(child)) {
                return true;
            }
        }
        return false;
    };

    findTextItem(this);
}

void InteractiveObject::applyCif()
{
    if (!modelEntity())
        return;

    const QVector<cif::CifBlockShared> &cifs = modelEntity()->cifs();
    if (cifs.size())
        qWarning() << Q_FUNC_INFO << "CIF data ignored for" << modelEntity()->name();
}

cif::CifBlockShared InteractiveObject::cifBlockByType(cif::CifLine::CifType type) const
{
    return modelEntity() ? modelEntity()->cifBlockByType(type) : cif::CifBlockShared();
}

/*!
  \brief InteractiveObject::mainCifType
  An item could have more than one CIF blocks that affects its geometry,
  say usually a Message has both the CifType::Message and CifType::Position.
  While the first one is about "how to draw message", the second is about its
  text/name position which is not so important for layouting process.
  Ancestors should override this to provide
  \return appropriate CIF type (CifType::Message in example above).
*/

cif::CifLine::CifType InteractiveObject::mainCifType() const
{
    return cif::CifLine::CifType::Unknown;
}

bool InteractiveObject::geometryManagedByCif() const
{
    const cif::CifLine::CifType targetType = mainCifType();
    return targetType == cif::CifLine::CifType::Unknown ? false : nullptr != cifBlockByType(targetType);
}

void InteractiveObject::updateCif() {}

void InteractiveObject::moveSilentlyBy(const QPointF &shift)
{
    if (shift.isNull())
        return;

    QSignalBlocker suppressMoved(this);
    moveBy(shift.x(), shift.y());
}

} // namespace msc
