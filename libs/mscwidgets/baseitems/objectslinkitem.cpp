/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "objectslinkitem.h"
#include "common/objectslink.h"
#include "common/objectanchor.h"
#include "common/utils.h"

#include <QEvent>
#include <QGraphicsSceneMoveEvent>
#include <QMetaObject>
#include <QPainter>

namespace msc {

/*!
  \class msc::ObjectsLinkItem
  \brief Base class for visual representation of items connections.

  \inmodule MscWidgets

*/

QPainterPath ObjectsLinkItem::hoverableLine(const QLineF &line)
{
    return utils::lineShape(line, utils::LineHoverTolerance);
}

ObjectsLinkItem::ObjectsLinkItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_link(new ObjectsLink(this))
{
    connect(m_link, &ObjectsLink::sourceAnchorChanged, this, &ObjectsLinkItem::onSourceAnchorChanged);
    connect(m_link, &ObjectsLink::targetAnchorChanged, this, &ObjectsLinkItem::onTargetAnchorChanged);
}

void ObjectsLinkItem::onSourceAnchorChanged(const QPointF &scenePos)
{
    Q_UNUSED(scenePos);
    rebuildLayout();
}

void ObjectsLinkItem::onTargetAnchorChanged(const QPointF &scenePos)
{
    Q_UNUSED(scenePos);
    rebuildLayout();
}

ObjectsLink *ObjectsLinkItem::link() const
{
    return m_link;
}

template<class T>
void ObjectsLinkItem::updateDrawingTool(T &existent, const T &requested)
{
    if (requested == existent)
        return;

    existent = requested;
    update();
}

void ObjectsLinkItem::setBodyBrush(const QBrush &brush)
{
    updateDrawingTool<QBrush>(m_bodyBrush, brush);
}

QBrush ObjectsLinkItem::bodyBrush() const
{
    return m_bodyBrush;
}

void ObjectsLinkItem::setBodyPen(const QPen &pen)
{
    updateDrawingTool<QPen>(m_bodyPen, pen);
}

QPen ObjectsLinkItem::bodyPen() const
{
    return m_bodyPen;
}

void ObjectsLinkItem::setSourceBrush(const QBrush &brush)
{
    updateDrawingTool<QBrush>(m_itemsBrush.Source, brush);
}

QBrush ObjectsLinkItem::sourceBrush() const
{
    return m_itemsBrush.Source;
}

void ObjectsLinkItem::setSourcePen(const QPen &pen)
{
    updateDrawingTool<QPen>(m_itemsPen.Source, pen);
}

QPen ObjectsLinkItem::sourcePen() const
{
    return m_itemsPen.Source;
}

void ObjectsLinkItem::setTargetBrush(const QBrush &brush)
{
    updateDrawingTool<QBrush>(m_itemsBrush.Target, brush);
}

QBrush ObjectsLinkItem::targetBrush() const
{
    return m_itemsBrush.Target;
}

void ObjectsLinkItem::setTargetPen(const QPen &pen)
{
    updateDrawingTool<QPen>(m_itemsPen.Target, pen);
}

QPen ObjectsLinkItem::targetPen() const
{
    return m_itemsPen.Target;
}

void ObjectsLinkItem::setStartSignShown(bool shown)
{
    if (shown == m_symbolShown.Source)
        return;

    m_symbolShown.Source = shown;
    update();
}

bool ObjectsLinkItem::startSignShown() const
{
    return m_symbolShown.Source;
}

void ObjectsLinkItem::setEndSignShown(bool shown)
{
    if (shown == m_symbolShown.Target)
        return;

    m_symbolShown.Target = shown;
    update();
}

bool ObjectsLinkItem::endSignShown() const
{
    return m_symbolShown.Target;
}

QPointF ObjectsLinkItem::anchorPointSource() const
{
    return m_link->source()->point();
}

QPointF ObjectsLinkItem::anchorPointTarget() const
{
    return m_link->target()->point();
}

void ObjectsLinkItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    drawBody(painter);
    if (startSignShown())
        drawStartSign(painter);
    if (endSignShown())
        drawEndSign(painter);
}

void ObjectsLinkItem::updateLayout()
{
    if (m_dirty)
        return;
    m_dirty = true;

    QMetaObject::invokeMethod(this, "buildLayout", Qt::QueuedConnection);
}

void ObjectsLinkItem::rebuildLayout()
{
    buildLayout();
}

QPainterPath ObjectsLinkItem::shape() const
{
    return bodyPath();
}

QRectF ObjectsLinkItem::boundingRect() const
{
    QRectF r = shape().boundingRect();
    r.moveCenter(QPointF(0., 0.));
    return r;
}

void ObjectsLinkItem::drawBody(QPainter *painter)
{
    drawPath(painter, m_bodyBrush, m_bodyPen, bodyPath());
}

void ObjectsLinkItem::drawStartSign(QPainter *painter)
{
    drawPath(painter, m_itemsBrush.Source, m_itemsPen.Source, m_symbols.Source);
}

void ObjectsLinkItem::drawEndSign(QPainter *painter)
{
    drawPath(painter, m_itemsBrush.Target, m_itemsPen.Target, m_symbols.Target);
}

void ObjectsLinkItem::drawPath(QPainter *painter, const QBrush &brush,
                               const QPen &pen, const QPainterPath &path)
{
    painter->save();

    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawPath(path);

    painter->restore();
}

} // namespace msc
