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
#include "chartviewmodel.h"
#include "commands/common/commandsstack.h"
#include "instanceitem.h"
#include "msctimer.h"

#include <QPainter>

namespace msc {

/*!
   \class msc::TimerItem
   The graphical item to represent a timer in the graphicsview.
   \see msc::MscTimer
 */

static const QRectF symbolSize(0, 0, 50, 25);

TimerItem::TimerItem(msc::MscTimer *timer, ChartViewModel *model, QGraphicsItem *parent)
    : InteractiveObject(timer, parent)
    , m_timer(timer)
    , m_model(model)
    , m_textItem(new NameItem(this))
    , m_timerConnector(new QGraphicsLineItem(this))
{
    Q_ASSERT(m_timer != nullptr);
    Q_ASSERT(m_model != nullptr);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    m_textItem->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    m_textItem->setEditable(true);
    m_textItem->setPlainText(m_timer->name());

    connect(m_timer, &msc::MscTimer::nameChanged, this, &msc::TimerItem::setName);

    connect(m_textItem, &TextItem::edited, this, &TimerItem::onTextEdited, Qt::QueuedConnection);
    connect(m_textItem, &TextItem::keyPressed, this, &TimerItem::updateLayout);

    connect(m_timer, &msc::MscTimer::precedingTimerChanged, this, &TimerItem::rebuildLayout);
    m_textItem->setVisible(m_timer->precedingTimer() == nullptr);

    m_timerConnector->setVisible(m_timer->precedingTimer() != nullptr);
    m_timerConnector->setPen(QPen(Qt::black, 1));

    m_boundingRect = symbolSize;
    m_boundingRect.setWidth(symbolSize.width() + m_textItem->boundingRect().width());
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
    painter->save();

    QPen pen(Qt::black, 1);
    painter->setPen(pen);

    QPointF start(m_boundingRect.x(), m_boundingRect.center().y());
    QRectF symboxRect = symbolBox();

    if (m_timer->timerType() == MscTimer::TimerType::Start) {
        painter->drawLine(start, symboxRect.center());
        drawStartSymbol(painter, symboxRect);
    }
    if (m_timer->timerType() == MscTimer::TimerType::Stop) {
        if (m_timer->precedingTimer() != nullptr) {
            painter->drawLine(start, symboxRect.center());
            drawStopSymbol(painter, symboxRect);
        } else {
            start.setY(m_boundingRect.height() - symboxRect.height() / 2.);
            symboxRect.moveTop(m_boundingRect.height() - symboxRect.height());
            painter->drawLine(start, symboxRect.center());
            drawStopSymbol(painter, symboxRect);
            // draw orphan start symbol
            const QPointF endConnect = symboxRect.center();
            symboxRect.moveTop(0);
            QPointF startConnect(endConnect.x(), symboxRect.bottom());
            drawStartSymbol(painter, symboxRect);
            painter->drawLine(startConnect, endConnect);
        }
    }
    if (m_timer->timerType() == MscTimer::TimerType::Timeout) {
        painter->drawLine(start, symboxRect.center());
        drawStartSymbol(painter, symboxRect);
        drawTimeoutArrow(painter, start);
    }

    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

void TimerItem::setName(const QString &text)
{
    if (!m_timer) {
        return;
    }

    m_timer->setName(text);
    m_textItem->setPlainText(text);

    updateLayout();
}

void TimerItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    InteractiveObject::mouseDoubleClickEvent(event);
    m_textItem->enableEditMode();
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

void TimerItem::onTextEdited(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }

    using namespace msc::cmd;
    CommandsStack::push(RenameEntity, { QVariant::fromValue<MscEntity *>(this->modelItem()), text });
}

void TimerItem::rebuildLayout()
{
    if (!m_instance) {
        return;
    }

    prepareGeometryChange();

    m_textItem->setX(symbolSize.right());
    const qreal textOffset = (symbolSize.height() - m_textItem->boundingRect().height()) / 2.;
    m_textItem->setY(textOffset);

    m_boundingRect = symbolSize;

    if (m_timer->precedingTimer() == nullptr) {
        m_textItem->setVisible(true);
        m_timerConnector->setVisible(false);
        m_boundingRect.setWidth(symbolSize.width() + m_textItem->boundingRect().width());

        if (m_timer->timerType() == MscTimer::TimerType::Stop) {
            m_boundingRect.setHeight(2. * symbolSize.height() + 20.);
            const qreal textY = m_boundingRect.height() - symbolSize.height() - textOffset;
            m_textItem->setY(textY);
        }
    } else {
        m_textItem->setVisible(false);
        m_timerConnector->setVisible(true);
        TimerItem *preTimer = m_model->itemForTimer(m_timer->precedingTimer());
        if (preTimer) {
            const QRectF &symbolRect = symbolBox();
            const QPointF &boxCenter = symbolRect.center();
            const qreal extraY = preTimer->sceneBoundingRect().bottom() - scenePos().y();
            const qreal startY = m_timer->timerType() == MscTimer::TimerType::Stop ? boxCenter.y() : symbolRect.top();
            m_timerConnector->setLine(boxCenter.x(), startY, boxCenter.x(), extraY);
            connect(preTimer, &InteractiveObject::relocated, this, &TimerItem::rebuildLayout, Qt::UniqueConnection);
            connect(this, &InteractiveObject::relocated, this, &TimerItem::rebuildLayout, Qt::UniqueConnection);
        }
    }

    const qreal x = m_instance->centerInScene().x();
    setX(x);
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

QRectF TimerItem::symbolBox() const
{
    const qreal boxSize = symbolSize.height();
    const QPointF start(m_boundingRect.x(), m_boundingRect.center().y());
    const QPointF boxCenter(start.x() + symbolSize.width() - boxSize / 2, start.y());
    return QRectF(boxCenter.x() - boxSize / 2, boxCenter.y() - boxSize / 2, boxSize, boxSize);
}

} // namespace msc
