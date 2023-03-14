/*
   Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "streaminglayoutmanager.h"

#include "actionitem.h"
#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/instanceheaditem.h"
#include "chartitem.h"
#include "conditionitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccreate.h"
#include "mscinstanceevent.h"
#include "mscmessage.h"
#include "msctimer.h"
#include "timeritem.h"

#include <QGraphicsScene>

namespace msc {

StreamingLayoutManager::StreamingLayoutManager(MscCommandsStack *undoStack, QObject *parent)
    : ChartLayoutManagerBase { undoStack, parent }
    , m_scene(new QGraphicsScene)
{
}

StreamingLayoutManager::~StreamingLayoutManager() { }

QGraphicsScene *StreamingLayoutManager::graphicsScene() const
{
    return m_scene.get();
}

void StreamingLayoutManager::setCurrentChart(MscChart *chart)
{
    if (chart == m_chart) {
        return;
    }

    if (m_chart) {
        disconnect(m_chart, nullptr, this, nullptr);
    }

    m_chart = chart;

    if (!m_chart) {
        return;
    }

    m_chartItem.reset(new ChartItem(m_chart, this));
    m_scene->addItem(m_chartItem.get());
    m_chartItem->setContentRect(minimalContentRect(), CifUpdatePolicy::ForceCreate);
    CoordinatesConverter::init(m_scene.get(), m_chartItem.get());

    connect(m_chart.get(), &MscChart::instanceAdded, this, &StreamingLayoutManager::instanceAdded);
    connect(m_chart.get(), &MscChart::instanceRemoved, this, &StreamingLayoutManager::instanceRemoved);
    connect(m_chart.get(), &MscChart::instanceEventAdded, this, &StreamingLayoutManager::eventAdded);
}

MscChart *StreamingLayoutManager::currentChart() const
{
    return m_chart;
}

void StreamingLayoutManager::setVisibleItemLimit(int number)
{
    m_visibleItems = number;
}

ChartItem *StreamingLayoutManager::itemForChart() const
{
    return m_chartItem.get();
}

CommentItem *StreamingLayoutManager::itemForComment(MscComment *comment) const
{
    return nullptr;
}

InstanceItem *StreamingLayoutManager::itemForInstance(MscInstance *instance) const
{
    if (!instance) {
        return nullptr;
    }

    for (InstanceItem *item : m_instanceItems) {
        if (item->modelItem() == instance) {
            return item;
        }
    }
    return nullptr;
}

MessageItem *StreamingLayoutManager::itemForMessage(MscMessage *message) const
{
    return itemForInstanceEvent<MessageItem>(message);
}

ConditionItem *StreamingLayoutManager::itemForCondition(MscCondition *condition) const
{
    return itemForInstanceEvent<ConditionItem>(condition);
}

ActionItem *StreamingLayoutManager::itemForAction(MscAction *action) const
{
    return itemForInstanceEvent<ActionItem>(action);
}

TimerItem *StreamingLayoutManager::itemForTimer(MscTimer *timer) const
{
    return itemForInstanceEvent<TimerItem>(timer);
}

CoregionItem *StreamingLayoutManager::itemForCoregion(MscCoregion *coregion) const
{
    qFatal("Coregions not supported by the MSC streamer");
    return nullptr;
}

InteractiveObject *StreamingLayoutManager::itemForEntity(MscEntity *entity) const
{
    if (!entity) {
        return nullptr;
    }

    if (entity->entityType() == MscEntity::EntityType::Instance) {
        return itemForInstance(static_cast<MscInstance *>(entity));
    } else {
        return itemForInstanceEvent<InteractiveObject>(dynamic_cast<MscInstanceEvent *>(entity));
    }
}

int StreamingLayoutManager::eventInstanceIndex(
        const QPointF &pt, MscInstance *instance, MscInstanceEvent *ignoreEvent) const
{
    return -1;
}

MscInstanceEvent *StreamingLayoutManager::eventAtPosition(const QPointF &scenePos) const
{
    for (auto eventItem : m_eventItems) {
        if (eventItem->sceneBoundingRect().contains(scenePos))
            return qobject_cast<MscInstanceEvent *>(eventItem->modelEntity());
    }
    return nullptr;
}

QRectF StreamingLayoutManager::minimalContentRect() const
{
    constexpr qreal minWidth { 200. };
    constexpr qreal minHeight { 200. };
    QRectF rect(0, 0, minWidth, minHeight);
    return rect;
}

QRectF StreamingLayoutManager::actualContentRect() const
{
    return m_chartItem->contentRect();
}

const QVector<InstanceItem *> &StreamingLayoutManager::instanceItems() const
{
    return m_instanceItems;
}

QPointF StreamingLayoutManager::instanceIndexToPosition(int index) const
{
    if (index <= 0) {
        return QPointF(10., 10.);
    }

    if (index >= m_instanceItems.size()) {
        index = m_instanceItems.size();
    }

    const InstanceItem *previouItem = m_instanceItems.at(index - 1);
    const QRectF previousBox = previouItem->sceneBoundingRect();
    return previousBox.topRight() + QPointF(interInstanceSpan(), 0.);
}

qreal StreamingLayoutManager::nextEventYPosition(MscInstance *instance) const
{
    if (m_nextYperInstance.contains(instance)) {
        return m_nextYperInstance[instance];
    }
    return 0.0;
}

void StreamingLayoutManager::instanceAdded(MscInstance *instance, int index)
{
    auto item = new InstanceItem(instance, this, m_chart.get(), m_chartItem.get());
    item->setHighlightable(false);
    connect(instance, &MscInstance::cifGeometryChanged, item, &InstanceItem::applyCif);
    connect(instance, &MscInstance::cifGeometryChanged, this, &StreamingLayoutManager::updateContentsRect);
    connect(m_chartItem.get(), &msc::ChartItem::contentRectChanged, item, &msc::InstanceItem::syncHeightToChartBox);
    if (index >= 0 && index < m_instanceItems.size()) {
        m_instanceItems.insert(index, item);
    } else {
        m_instanceItems.append(item);
    }
    m_nextYperInstance[instance] = item->headerItem()->sceneBoundingRect().bottom() + interMessageSpan() / 2.;
    updateInstancesRect();
    Q_EMIT instanceItemsChanged();
}

void StreamingLayoutManager::instanceRemoved(MscInstance *instance)
{
    InstanceItem *item = itemForInstance(instance);
    m_instanceItems.removeAll(item);
    delete item;
    m_nextYperInstance.remove(instance);
    updateInstancesRect();
    Q_EMIT instanceItemsChanged();
}

void StreamingLayoutManager::updateContentsRect()
{
    updateInstancesRect();
    const QRectF sceneRect = m_chartItem->sceneBoundingRect();
    const QRectF newChartRect = sceneRect | instancesRect();
    if (newChartRect != sceneRect) {
        m_chartItem->setContentRect(newChartRect);
    }

    updateInstancesRect();

    // update events
    if (auto instance = qobject_cast<MscInstance *>(sender())) {
        InstanceItem *instanceItem = itemForInstance(instance);
        syncItemsPosToInstance(instanceItem);
    }
}

void StreamingLayoutManager::eventAdded(MscInstanceEvent *event)
{
    switch (event->entityType()) {
    case MscEntity::EntityType::Action:
        addAction(static_cast<MscAction *>(event));
        break;
    case MscEntity::EntityType::Condition:
        addCondition(static_cast<MscCondition *>(event));
        break;
    case MscEntity::EntityType::Create:
        addCreateMessage(static_cast<MscCreate *>(event));
        break;
    case MscEntity::EntityType::Message:
        addMessage(static_cast<MscMessage *>(event));
        break;
    case MscEntity::EntityType::Timer:
        addTimer(static_cast<MscTimer *>(event));
        break;
    default:
        qFatal("Not supported");
        break;
    }
}

void StreamingLayoutManager::updateMessagePosition()
{
    auto message = dynamic_cast<MscMessage *>(sender());
    if (!message) {
        return;
    }

    MessageItem *item = itemForMessage(message);
    if (!item) {
        return;
    }

    if (message->sourceInstance()) {
        m_nextYperInstance[message->sourceInstance()] = item->tail().y() + interMessageSpan();
    }
    if (message->targetInstance()) {
        m_nextYperInstance[message->targetInstance()] = item->head().y() + interMessageSpan();
    }

    checkChartSize(item);

    item->updateCif();
}

void StreamingLayoutManager::addAction(MscAction *action)
{
    Q_ASSERT(action->instance());
    auto item = new ActionItem(action, this, m_chartItem.get());
    item->setChartItem(m_chartItem.get());
    m_eventItems[action->internalId()] = item;

    // set X
    InstanceItem *instanceItem = itemForInstance(action->instance());
    Q_ASSERT(instanceItem);
    syncItemPosToInstance(item, instanceItem);

    // set Y
    item->setY(m_nextYperInstance[action->instance()]);

    item->instantLayoutUpdate();
    m_nextYperInstance[action->instance()] = item->sceneBoundingRect().bottom() + interMessageSpan();

    checkChartSize(item);

    item->updateCif();
}

void StreamingLayoutManager::addCondition(MscCondition *condition)
{
    auto item = new ConditionItem(condition, this, m_chartItem.get());
    connect(this, &msc::ChartLayoutManagerBase::instancesRectChanged, item, &msc::ConditionItem::setInstancesRect);
    item->setChartItem(m_chartItem.get());
    m_eventItems[condition->internalId()] = item;

    // set X
    if (condition->shared()) {
        item->setInstancesRect(instancesRect());
    } else {
        InstanceItem *instanceItem = itemForInstance(condition->instance());
        Q_ASSERT(instanceItem);
        syncItemPosToInstance(item, instanceItem);
    }

    // set Y
    QVector<MscInstance *> instances;
    if (condition->shared()) {
        instances = m_chart->instances();
    } else {
        instances.append(condition->instance());
    }
    qreal y = -9e9;
    for (MscInstance *inst : instances) {
        y = std::max(y, m_nextYperInstance[inst]);
    }
    item->setY(y);
    item->instantLayoutUpdate();
    for (MscInstance *inst : instances) {
        m_nextYperInstance[inst] = item->sceneBoundingRect().bottom() + interMessageSpan();
    }

    checkChartSize(item);

    item->updateCif();
}

void StreamingLayoutManager::addCreateMessage(MscCreate *message)
{
    InstanceItem *sourceItem = itemForInstance(message->sourceInstance());
    InstanceItem *targetItem = itemForInstance(message->targetInstance());

    Q_ASSERT(message->sourceInstance());
    Q_ASSERT(message->targetInstance());
    Q_ASSERT(sourceItem);
    Q_ASSERT(targetItem);
    if (!message->sourceInstance() || !message->targetInstance() || !sourceItem || !targetItem) {
        return;
    }

    auto item = new MessageItem(message, this, sourceItem, targetItem, m_chartItem.get());
    item->disableSyntaxChecking();
    item->setChartItem(m_chartItem.get());
    m_eventItems[message->internalId()] = item;

    // set Y
    qreal y = interMessageSpan();
    if (message->sourceInstance()) {
        y = std::max(y, m_nextYperInstance[message->sourceInstance()]);
    }
    item->setY(y + interMessageSpan() * 0.5);
    qreal nextY = item->sceneBoundingRect().bottom() + interMessageSpan() * 0.5;
    m_nextYperInstance[message->sourceInstance()] = nextY;

    const qreal deltaY = item->head().y() - targetItem->leftCreatorTarget().y();
    targetItem->moveSilentlyBy(QPointF(0.0, deltaY));
    targetItem->syncHeightToChartBox();

    // set X
    syncItemPosToInstance(item, sourceItem);
    syncItemPosToInstance(item, targetItem);

    item->instantLayoutUpdate();
    m_nextYperInstance[message->targetInstance()] =
            targetItem->headerItem()->sceneBoundingRect().bottom() + interMessageSpan();

    checkChartSize(item);

    item->updateCif();
}

void StreamingLayoutManager::addMessage(MscMessage *message)
{
    InstanceItem *sourceItem = itemForInstance(message->sourceInstance());
    InstanceItem *targetItem = itemForInstance(message->targetInstance());
    auto item = new MessageItem(message, this, sourceItem, targetItem, m_chartItem.get());
    item->disableSyntaxChecking();
    item->setChartItem(m_chartItem.get());
    m_eventItems[message->internalId()] = item;

    // set Y
    qreal y = -9e-9;
    if (message->sourceInstance()) {
        y = std::max(y, m_nextYperInstance[message->sourceInstance()]);
    }
    if (message->targetInstance()) {
        y = std::max(y, m_nextYperInstance[message->targetInstance()]);
    }
    item->setY(y + interMessageSpan() * 0.5);

    // set X
    syncItemPosToInstance(item, sourceItem);
    syncItemPosToInstance(item, targetItem);

    item->instantLayoutUpdate();
    if (item->boundingRect().top() < -(interMessageSpan() * 0.5)) {
        // some messages are multi line, and therfore higher
        item->setY(item->y() - item->boundingRect().top());
        item->instantLayoutUpdate();
    }
    if (message->sourceInstance()) {
        m_nextYperInstance[message->sourceInstance()] = item->tail().y() + interMessageSpan();
    }
    if (message->targetInstance()) {
        m_nextYperInstance[message->targetInstance()] = item->head().y() + interMessageSpan();
    }

    checkChartSize(item);

    connect(message, &MscMessage::cifPointsChanged, this, &StreamingLayoutManager::updateMessagePosition);
    item->updateCif();
}

void StreamingLayoutManager::addTimer(MscTimer *timer)
{
    auto item = new TimerItem(timer, this, m_chartItem.get());
    item->setChartItem(m_chartItem.get());
    m_eventItems[timer->internalId()] = item;

    // set X
    InstanceItem *instanceItem = itemForInstance(timer->instance());
    Q_ASSERT(instanceItem);
    syncItemPosToInstance(item, instanceItem);

    // set Y
    item->setY(m_nextYperInstance[timer->instance()]);

    item->instantLayoutUpdate();
    m_nextYperInstance[timer->instance()] = item->sceneBoundingRect().bottom() + interMessageSpan();

    checkChartSize(item);

    item->updateCif();
}

void StreamingLayoutManager::updateInstancesRect()
{
    QRectF rect;
    for (msc::InstanceItem *item : m_instanceItems) {
        rect |= item->sceneBoundingRect();
    }
    setInstancesRect(rect);
}

void StreamingLayoutManager::checkChartSize(EventItem *item)
{
    QRectF chartRect = m_chartItem->contentRect();
    if (item->sceneBoundingRect().bottom() > chartRect.bottom()) {
        chartRect.setBottom(item->sceneBoundingRect().bottom() + ChartItem::chartMargins().bottom());
        m_chartItem->setContentRect(chartRect);
    }
    chartRect = m_chartItem->contentRect();
    if (item->sceneBoundingRect().right() > chartRect.right()) {
        chartRect.setRight(item->sceneBoundingRect().right() + ChartItem::chartMargins().right());
        m_chartItem->setContentRect(chartRect);
    }
}

} // namespace msc
