/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "timeritem.h"

#include "baseitems/textitem.h"
#include "instanceitem.h"

#include <QPainter>
#include <msctimer.h>

namespace msc {

/*!
   \class msc::TimerItem
   The graphical item to represent a timer in the graphicsview.
   \see msc::MscTimer
 */

static const QRectF symbolSize(0, 0, 50, 25);

TimerItem::TimerItem(msc::MscTimer *timer, QGraphicsItem *parent)
    : InteractiveObject(timer, parent)
    , m_timer(timer)
    , m_textItem(new TextItem(this))
{
    Q_ASSERT(m_timer != nullptr);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    m_textItem->setPlainText(m_timer->name());
}

MscTimer *TimerItem::modelItem() const
{
    return m_timer;
}

void TimerItem::connectObjects(InstanceItem *instance, qreal y)
{
    setY(y);
    setInstance(instance);
}

void TimerItem::setInstance(InstanceItem *instance)
{
    if (instance == m_instance) {
        return;
    }

    if (m_instance) {
        disconnect(m_instance, nullptr, this, nullptr);
    }

    m_instance = instance;
    if (m_instance) {
        connect(m_instance, &InteractiveObject::relocated, this, &TimerItem::onInstanceMoved, Qt::DirectConnection);
        m_timer->setInstance(m_instance->modelItem());
    } else {
        m_timer->setInstance(nullptr);
    }

    updateLayout();
}

void TimerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const qreal boxSize = symbolSize.height();

    painter->save();

    QPen pen(Qt::black, 1);
    painter->setPen(pen);

    QPointF start(m_boundingRect.x(), m_boundingRect.center().y());
    QPointF boxCenter(start.x() + symbolSize.width() - boxSize / 2, start.y());
    painter->drawLine(start, boxCenter);

    QRectF symboxRect(boxCenter.x() - boxSize / 2, boxCenter.y() - boxSize / 2, boxSize, boxSize);

    if (m_timer->timerType() == MscTimer::TimerType::Start) {
        drawStartSymbol(painter, symboxRect);
    }
    if (m_timer->timerType() == MscTimer::TimerType::Stop) {
        drawStopSymbol(painter, symboxRect);
    }
    if (m_timer->timerType() == MscTimer::TimerType::Timeout) {
        drawStartSymbol(painter, symboxRect);
        drawTimeoutArrow(painter, start);
    }

    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

void TimerItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() == GripPoint::Location::Center) {
        const QPointF &delta = to - from;
        setPos(pos() + delta);
    }
}

void TimerItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

void TimerItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints({ GripPoint::Location::Center });

    connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeFinish, this,
            &TimerItem::onManualGeometryChangeFinished, Qt::UniqueConnection);
}

void TimerItem::rebuildLayout()
{
    if (!m_instance) {
        return;
    }

    prepareGeometryChange();

    m_textItem->setPlainText(m_timer->name());
    m_textItem->setX(symbolSize.right());
    m_textItem->setY((symbolSize.height() - m_textItem->boundingRect().height()) / 2);

    m_boundingRect = symbolSize;
    const double x = m_instance->centerInScene().x();
    setX(x);

    m_layoutDirty = false;
}

void TimerItem::onInstanceMoved(const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    updateLayout();
}

void TimerItem::onManualGeometryChangeFinished(GripPoint::Location pos, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    if (pos == GripPoint::Location::Center) {
        Q_EMIT moved(this);
    }
}

void TimerItem::drawStartSymbol(QPainter *painter, const QRectF &rect)
{
    QPolygonF poly;
    poly.append(rect.topRight());
    poly.append(rect.topLeft());
    poly.append(rect.bottomRight());
    poly.append(rect.bottomLeft());
    poly.append(rect.topRight());
    painter->drawPolyline(poly);
}

void TimerItem::drawStopSymbol(QPainter *painter, const QRectF &rect)
{
    painter->drawLine(rect.topLeft(), rect.bottomRight());
    painter->drawLine(rect.topRight(), rect.bottomLeft());
}

void TimerItem::drawTimeoutArrow(QPainter *painter, const QPointF &pt)
{
    QPainterPath path;
    path.moveTo(pt);
    path.lineTo(pt - QPointF(-10, -5));
    path.lineTo(pt - QPointF(-10, 5));
    path.closeSubpath();
    painter->fillPath(path, Qt::black);
}

} // namespace msc
