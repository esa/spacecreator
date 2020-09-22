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
#include "chartlayoutmanager.h"
#include "chartview/mscchartviewconstants.h"
#include "commands/common/commandsstack.h"
#include "instanceitem.h"
#include "msctimer.h"
#include "ui/grippointshandler.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

namespace msc {

/*!
   \class msc::TimerItem
   The graphical item to represent a timer in the graphicsview.
   \see msc::MscTimer
 */

TimerItem::TimerItem(msc::MscTimer *timer, ChartLayoutManager *model, QGraphicsItem *parent)
    : InteractiveObject(timer, parent)
    , m_timer(timer)
    , m_model(model)
    , m_textItem(new TextItem(this))
    , m_timerConnector(new QGraphicsLineItem(this))
{
    Q_ASSERT(m_timer != nullptr);
    Q_ASSERT(m_model != nullptr);

    setFlags(ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsSelectable);

    m_textItem->setMscValidationTest(QString("msc c1;instance i1;starttimer %1;endinstance;endmsc;"));
    m_textItem->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    m_textItem->setEditable(true);
    m_textItem->setPlainText(m_timer->name());

    connect(m_timer, &msc::MscTimer::nameChanged, this, &msc::TimerItem::setName);

    connect(m_textItem, &TextItem::edited, this, &TimerItem::onTextEdited, Qt::QueuedConnection);
    connect(m_textItem, &TextItem::textChanged, this, &TimerItem::scheduleLayoutUpdate);

    connect(m_timer, &msc::MscTimer::precedingTimerChanged, this, &TimerItem::scheduleLayoutUpdate);
    m_textItem->setVisible(m_timer->precedingTimer() == nullptr);

    connect(this, &TimerItem::relocated, this, &TimerItem::scheduleLayoutUpdate);

    m_timerConnector->setVisible(m_timer->precedingTimer() != nullptr);
    m_timerConnector->setPen(QPen(Qt::black, 1));

    QRectF br = TIMER_SYMBOL_SIZE;
    br.setWidth(TIMER_SYMBOL_SIZE.width() + m_textItem->boundingRect().width());
    setBoundingRect(br);
}

MscTimer *TimerItem::modelItem() const
{
    return m_timer;
}

void TimerItem::connectObjects(InstanceItem *instance, qreal y)
{
    setY(y);
    setInstance(instance);
    scheduleLayoutUpdate();
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

    const qreal x = m_instance->centerInScene().x();
    setX(x);

    scheduleLayoutUpdate();
}

void TimerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen pen(Qt::black, 1);
    painter->setPen(pen);

    const QRectF br = boundingRect();

    QPointF start(br.x(), br.center().y());
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
            start.setY(br.height() - symboxRect.height() / 2.);
            symboxRect.moveTop(br.height() - symboxRect.height());
            painter->drawLine(start, symboxRect.center());
            drawStopSymbol(painter, symboxRect);
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

    m_textItem->setPlainText(text);

    scheduleLayoutUpdate();
}

void TimerItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_timerConnector->setVisible(true);
    const QPointF start = symbolBox().center();
    const QPointF end(start.x(), event->pos().y());
    m_timerConnector->setLine(QLineF(start, end));

    InteractiveObject::mouseMoveEvent(event);
}

void TimerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    msc::MscInstanceEvent *instanceEvent = m_model->eventAtPosition(event->scenePos());
    if (instanceEvent) {
        auto timer = qobject_cast<msc::MscTimer *>(instanceEvent);
        if (timer) {
            if (canConnectTimers(timer, event->scenePos())) {
                using namespace msc::cmd;
                CommandsStack::push(RenameEntity, { QVariant::fromValue<MscEntity *>(timer), m_timer->name() });
            }
        }
    }

    updateConnectorLineVisibility();
    scheduleLayoutUpdate();

    InteractiveObject::mouseReleaseEvent(event);
}

void TimerItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    InteractiveObject::mouseDoubleClickEvent(event);
    m_textItem->enableEditMode();
}

void TimerItem::onManualMoveProgress(shared::ui::GripPoint *, const QPointF &from, const QPointF &to)
{
    const QPointF &newPos = pos() + (to - from);
    setPos(newPos);
}

void TimerItem::onManualMoveFinish(shared::ui::GripPoint *, const QPointF &, const QPointF &)
{
    Q_EMIT moved(this);
}

void TimerItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    gripPointsHandler()->setUsedPoints({ shared::ui::GripPoint::Location::Center });
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

    m_textItem->setX(TIMER_SYMBOL_SIZE.right());
    const qreal textOffset = (TIMER_SYMBOL_SIZE.height() - m_textItem->boundingRect().height()) / 2.;
    m_textItem->setY(textOffset);

    QRectF br = TIMER_SYMBOL_SIZE;

    if (m_timer->precedingTimer() == nullptr) {
        m_textItem->setVisible(true);
        br.setWidth(TIMER_SYMBOL_SIZE.width() + m_textItem->boundingRect().width());
    } else {
        m_textItem->setVisible(false);
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

    setBoundingRect(br);

    updateConnectorLineVisibility();

    const qreal x = m_instance->centerInScene().x();
    setX(x);
}

void TimerItem::onInstanceMoved(const QPointF &from, const QPointF &to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    scheduleLayoutUpdate();
}

void TimerItem::updateConnectorLineVisibility()
{
    TimerItem *preTimer = m_model->itemForTimer(m_timer->precedingTimer());
    m_timerConnector->setVisible(preTimer && preTimer->isVisible());
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
    const qreal boxSize = TIMER_SYMBOL_SIZE.height();
    const QPointF start(boundingRect().x(), boundingRect().center().y());
    const QPointF boxCenter(start.x() + TIMER_SYMBOL_SIZE.width() - boxSize / 2, start.y());
    return QRectF(boxCenter.x() - boxSize / 2, boxCenter.y() - boxSize / 2, boxSize, boxSize);
}

bool TimerItem::canConnectTimers(MscTimer *targetTimer, const QPointF &targetPos)
{
    if (targetTimer == nullptr || m_timer == nullptr)
        return false;

    if (m_timer->instance() != targetTimer->instance() || targetTimer->instance() == nullptr
            || m_timer->instance() == nullptr)
        return false;

    const bool upperItem = scenePos().y() < targetPos.y();
    MscTimer *start = upperItem ? m_timer.data() : targetTimer;
    return start->timerType() == MscTimer::TimerType::Start;
}

} // namespace msc
