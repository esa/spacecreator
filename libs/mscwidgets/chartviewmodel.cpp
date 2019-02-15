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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "chartviewmodel.h"

#include "actionitem.h"
#include "baseitems/common/utils.h"
#include "baseitems/instanceheaditem.h"
#include "chartitem.h"
#include "commands/common/commandsstack.h"
#include "conditionitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccreate.h"
#include "mscinstance.h"
#include "msctimer.h"
#include "timeritem.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QMap>
#include <QPointer>
#include <QVector>
#include <cmath>
#include <limits>

namespace msc {

/*!
   \class  ChartViewModel is the model containing the scene graph of the currently selected/visible
   MSC chart (showing instances, messages, ...)
 */

struct ChartViewLayoutInfo {
    ChartViewLayoutInfo() {}

    ~ChartViewLayoutInfo() { clear(); }

    void clear()
    {
        m_dynamicInstanceMarkers.clear();
        m_dynamicInstances.clear();
        m_pos = { 0., 0. };
        m_instancesRect = QRectF();

        if (m_chartItem) {
            utils::removeSceneItem(m_chartItem);
            delete m_chartItem;
        }
    }

    QMap<MscInstance *, MessageItem *> m_dynamicInstances;
    QMap<MscInstance *, MessageItem *> m_dynamicInstanceMarkers;
    QRectF m_instancesRect;
    QSizeF m_preferredBox;
    QPointF m_pos;

    QPointer<ChartItem> m_chartItem = nullptr;

private:
};

struct ChartViewModelPrivate {
    ChartViewModelPrivate() {}

    QGraphicsScene m_scene;
    QVector<msc::InstanceItem *> m_instanceItems;
    QVector<msc::InteractiveObject *> m_instanceEventItems;
    QPointer<msc::MscChart> m_currentChart = nullptr;
    static constexpr qreal InterMessageSpan = 20.;
    static constexpr qreal InterInstanceSpan = 100.;
    bool m_layoutDirty = false;

    ChartViewLayoutInfo m_layoutInfo;

    qreal calcInstanceAxisHeight() const
    {
        static constexpr qreal oneMessageHeight = 50.;
        const int eventsCount = qMax(1, m_currentChart->instanceEvents().size());
        return eventsCount * (oneMessageHeight + ChartViewModelPrivate::InterMessageSpan);
    }
};

ChartViewModel::ChartViewModel(QObject *parent)
    : QObject(parent)
    , d(new ChartViewModelPrivate())
{
}

ChartViewModel::~ChartViewModel()
{
    clearScene();
}

QGraphicsScene *ChartViewModel::graphicsScene() const
{
    return &(d->m_scene);
}

MscChart *ChartViewModel::currentChart() const
{
    return d->m_currentChart;
}

void ChartViewModel::clearScene()
{
    qDeleteAll(d->m_instanceEventItems);
    d->m_instanceEventItems.clear();

    qDeleteAll(d->m_instanceItems);
    d->m_instanceItems.clear();

    d->m_layoutInfo.clear();

    d->m_scene.clear();
}

void ChartViewModel::fillView(MscChart *chart)
{
    if (chart == d->m_currentChart) {
        return;
    }

    if (d->m_currentChart) {
        disconnect(d->m_currentChart, nullptr, this, nullptr);
    }

    d->m_currentChart = chart;

    clearScene();

    if (d->m_currentChart)
        relayout();

    connect(d->m_currentChart, &msc::MscChart::instanceAdded, this, &ChartViewModel::updateLayout);
    connect(d->m_currentChart, &msc::MscChart::instanceRemoved, this,
            QOverload<msc::MscInstance *>::of(&ChartViewModel::removeInstanceItem));
    connect(d->m_currentChart, &msc::MscChart::instanceMoved, this, [&]() {
        this->clearScene();
        this->updateLayout();
    });

    connect(d->m_currentChart, &msc::MscChart::instanceEventAdded, this, &ChartViewModel::updateLayout);
    connect(d->m_currentChart, &msc::MscChart::instanceEventRemoved, this, &ChartViewModel::removeEventItem);
    connect(d->m_currentChart, &msc::MscChart::eventMoved, this, &ChartViewModel::updateLayout);
    connect(d->m_currentChart, &msc::MscChart::messageRetargeted, this, [&]() {
        this->clearScene();
        this->updateLayout();
    });

    Q_EMIT currentChartChanged(d->m_currentChart);
}

MessageItem *ChartViewModel::fillMessageItem(MscMessage *message, InstanceItem *sourceItem, InstanceItem *targetItem,
                                             qreal verticalOffset)
{
    MessageItem *item = itemForMessage(message);
    if (!item) {
        item = new MessageItem(message);
        connect(item, &MessageItem::retargeted, this, &ChartViewModel::onMessageRetargeted, Qt::UniqueConnection);

        const bool isCreateMsg =
                item->isCreator() && targetItem && targetItem->modelItem() == message->targetInstance();

        qreal newY = d->m_layoutInfo.m_pos.y() + verticalOffset;

        if (!isCreateMsg) {
            if (targetItem && targetItem->modelItem()->explicitCreator()) {
                newY -= targetItem->axis().y1();
            } else if (sourceItem && sourceItem->modelItem()->explicitCreator()) {
                newY -= sourceItem->axis().y1();
            }
        }

        d->m_scene.addItem(item);
        d->m_instanceEventItems.append(item);

        if (isCreateMsg) {
            QLineF axisLine(targetItem->axis());
            axisLine.setP1({ axisLine.x1(), newY + InstanceHeadItem::StartSymbolHeight / 2. });

            const qreal deltaY = targetItem->axis().length() - axisLine.length();

            if (!targetItem->modelItem()->explicitStop()) {
                axisLine.setP2({ axisLine.x2(), axisLine.y2() - axisLine.y1() });
            }

            targetItem->setAxisHeight(axisLine.length());
            targetItem->moveBy(0., deltaY);
        }

        item->connectObjects(sourceItem, targetItem, newY);
    }

    if (item)
        item->updateLayout();

    return item;
}

void ChartViewModel::relayout()
{
    d->m_layoutInfo.m_dynamicInstanceMarkers.clear();
    d->m_layoutInfo.m_pos = { 0., 0. };
    d->m_layoutInfo.m_instancesRect = QRectF();

    // The calls order below DOES matter
    addInstanceItems(); // which are not highlightable now to avoid flickering
    addInstanceEventItems();
    actualizeInstancesHeights(d->m_layoutInfo.m_pos.y() + d->InterMessageSpan);
    updateContentBounds();

    // make instance items be highlightable on message (dis-)connection
    for (MscInstance *instance : d->m_currentChart->instances())
        if (InstanceItem *item = itemForInstance(instance))
            item->setHighlightable(true);

    d->m_layoutDirty = false;

    Q_EMIT layoutComplete();
}

void ChartViewModel::addInstanceItems()
{
    for (MscInstance *instance : d->m_currentChart->instances()) {
        InstanceItem *item = itemForInstance(instance);
        if (!item) {
            item = createDefaultInstanceItem(instance, QPointF());
            connect(item, &InstanceItem::moved, this, &ChartViewModel::onInstanceItemMoved, Qt::UniqueConnection);
            d->m_scene.addItem(item);
            d->m_instanceItems.append(item);
            item->setX(d->m_layoutInfo.m_pos.x());
        }

        item->setHighlightable(false);
        item->setKind(instance->kind());

        d->m_layoutInfo.m_pos.rx() += d->InterInstanceSpan + item->boundingRect().width();

        if (!instance->explicitCreator()) {
            // move axis start to the scene's Y0:
            const QLineF &axisLine(item->axis());
            item->moveBy(0., -qMin(axisLine.y1(), axisLine.y2()));
        }

        d->m_layoutInfo.m_instancesRect = d->m_layoutInfo.m_instancesRect.united(item->sceneBoundingRect());
    }
}

void ChartViewModel::addInstanceEventItems()
{
    QGraphicsObject *instanceEventItem(nullptr);
    for (MscInstanceEvent *instanceEvent : d->m_currentChart->instanceEvents()) {
        switch (instanceEvent->entityType()) {
        case MscEntity::EntityType::Message: {
            instanceEventItem = addMessageItem(static_cast<MscMessage *>(instanceEvent));
            break;
        }
        case MscEntity::EntityType::Create: {
            // TODO: rm MscCreate wrapper and use the MscMessage directly?
            instanceEventItem = addMessageItem(static_cast<MscMessage *>(instanceEvent));
            break;
        }
        case MscEntity::EntityType::Action: {
            instanceEventItem = addActionItem(static_cast<MscAction *>(instanceEvent));
            break;
        }
        case MscEntity::EntityType::Condition: {
            ConditionItem *prevItem = qobject_cast<ConditionItem *>(instanceEventItem);
            instanceEventItem = addConditionItem(static_cast<MscCondition *>(instanceEvent), prevItem,
                                                 d->m_layoutInfo.m_instancesRect);
            break;
        }
        case MscEntity::EntityType::Timer: {
            instanceEventItem = addTimerItem(static_cast<MscTimer *>(instanceEvent));
            break;
        }
        default: {
            instanceEventItem = nullptr;
            break;
        }
        }

        if (instanceEventItem)
            polishAddedEventItem(instanceEvent, instanceEventItem);
    }
}

void ChartViewModel::polishAddedEventItem(MscInstanceEvent *event, QGraphicsObject *item)
{
    auto moveNewItem = [&]() {
        qreal deltaY = 0.;

        const QRectF srcRect = item->sceneBoundingRect();
        const qreal targetTop = d->m_layoutInfo.m_pos.y() + d->InterMessageSpan;
        if (!qFuzzyCompare(srcRect.top() + 1., targetTop + 1.)) {
            deltaY = targetTop - srcRect.top();
            item->moveBy(0., deltaY);
        }

        d->m_layoutInfo.m_pos.ry() = targetTop + srcRect.height();

        return deltaY;
    };

    switch (event->entityType()) {
    case MscEntity::EntityType::Message:
    case MscEntity::EntityType::Create: {
        MscMessage *message = static_cast<MscMessage *>(event);
        MessageItem *messageItem = dynamic_cast<MessageItem *>(item);
        Q_ASSERT(messageItem != nullptr);
        messageItem->setPositionChangeIgnored(true);

        const bool relatedToDynamicInstance =
                (message->sourceInstance() && message->sourceInstance()->explicitCreator())
                || (message->targetInstance() && message->targetInstance()->explicitCreator());
        const qreal deltaY = moveNewItem();
        if (!qFuzzyIsNull(deltaY) && relatedToDynamicInstance) {
            // After a message has been moved its connection (an arrow to an instance) is broken
            InstanceItem *creatorInstanceItem = itemForInstance(message->sourceInstance());
            InstanceItem *createdInstanceItem = itemForInstance(message->targetInstance());
            if (createdInstanceItem && creatorInstanceItem && messageItem) {
                if (event->entityType() == MscEntity::EntityType::Create) {
                    createdInstanceItem->moveBy(0., deltaY);
                }
                messageItem->connectObjects(creatorInstanceItem, createdInstanceItem, messageItem->y());
            }
        }

        messageItem->setPositionChangeIgnored(false);

        break;
    }
    default: {
        moveNewItem();
        break;
    }
    }
}

void ChartViewModel::updateContentBounds()
{
    QRectF totalRect;
    const QList<InteractiveObject *> &toplevelItems = utils::toplevelItems<InteractiveObject>(graphicsScene());
    const int toplevelItemsCount = toplevelItems.size();
    for (int i = 0; i < toplevelItemsCount; ++i) {
        if (InteractiveObject *gi = toplevelItems.at(i)) {
            if (gi->modelEntity()->entityType() == MscEntity::EntityType::Message) {
                MscMessage *message = static_cast<MscMessage *>(gi->modelEntity());
                if (message->isGlobal()) // ignore, it will be connected to the ChartItem edge lately
                    continue;
            }
            totalRect = totalRect.united(gi->sceneBoundingRect());
        }
    }

    const QSizeF &preferredSize = preferredChartBoxSize();
    if (!preferredSize.isEmpty()) {
        const qreal widthShiftHalf =
                totalRect.width() <= preferredSize.width() ? (preferredSize.width() - totalRect.width()) / 2. : 0.;
        const qreal heightShiftHalf =
                totalRect.height() <= preferredSize.height() ? (preferredSize.height() - totalRect.height()) / 2. : 0.;
        if (!qFuzzyIsNull(widthShiftHalf) || !qFuzzyIsNull(heightShiftHalf)) {
            totalRect.adjust(-widthShiftHalf, -heightShiftHalf, widthShiftHalf, heightShiftHalf);
        }
    }

    if (!d->m_layoutInfo.m_chartItem) {
        d->m_layoutInfo.m_chartItem = new ChartItem(d->m_currentChart);
        d->m_scene.addItem(d->m_layoutInfo.m_chartItem);
    }

    d->m_layoutInfo.m_chartItem->setZValue(-toplevelItemsCount);
    d->m_layoutInfo.m_chartItem->setBox(totalRect);

    d->m_scene.setSceneRect(d->m_layoutInfo.m_chartItem->sceneBoundingRect());

    // polish events
    for (MscInstanceEvent *event : d->m_currentChart->instanceEvents()) {
        switch (event->entityType()) {
        case MscEntity::EntityType::Message: {
            MscMessage *message = static_cast<MscMessage *>(event);
            if (message->isGlobal()) {
                if (MessageItem *item = itemForMessage(message)) {
                    item->updateLayout(); // place it on the correct box edge
                }
            }
            break;
        }
        default: {
            break;
        }
        }
    }
}

void ChartViewModel::actualizeInstancesHeights(qreal height) const
{
    for (MscInstance *instance : d->m_currentChart->instances()) {
        if (InstanceItem *instanceItem = itemForInstance(instance)) {

            bool updated(false);
            if (instance->explicitStop()) {
                updateStoppedInstanceHeight(instanceItem);
                updated = true;
            }

            if (instance->explicitCreator() && !instance->explicitStop()) {
                updateCreatedInstanceHeight(instanceItem, height);
                updated = true;
            }

            if (!updated) {
                instanceItem->setAxisHeight(height);
            }
        }
    }
}

void ChartViewModel::updateStoppedInstanceHeight(InstanceItem *instanceItem) const
{
    // update instance's end Y-postion to the last message
    QVector<QGraphicsObject *> events(instanceEventItems(instanceItem->modelItem()));
    if (!events.isEmpty()) {
        std::sort(events.begin(), events.end(), [](const QGraphicsObject *const a, const QGraphicsObject *const b) {
            return a->pos().y() < b->pos().y();
        });

        if (QGraphicsObject *bottommostEvent = events.last()) {
            const qreal bottomY = bottommostEvent->sceneBoundingRect().bottom();
            QLineF axisLine(instanceItem->axis());
            axisLine.setP2({ axisLine.x2(), bottomY });
            instanceItem->setAxisHeight(axisLine.length());
        }
    }
}

void ChartViewModel::updateCreatedInstanceHeight(InstanceItem *instanceItem, qreal totalH) const
{
    QLineF axisLine(instanceItem->axis());
    axisLine.setP2({ axisLine.x2(), totalH });

    instanceItem->setAxisHeight(axisLine.length());
}

InstanceItem *ChartViewModel::itemForInstance(msc::MscInstance *instance) const
{
    return utils::itemForEntity<InstanceItem, MscInstance>(instance, &d->m_scene);
}

MessageItem *ChartViewModel::itemForMessage(MscMessage *message) const
{
    return utils::itemForEntity<MessageItem, MscMessage>(message, &d->m_scene);
}

ConditionItem *ChartViewModel::itemForCondition(MscCondition *condition) const
{
    return utils::itemForEntity<ConditionItem, MscCondition>(condition, &d->m_scene);
}

ActionItem *ChartViewModel::itemForAction(MscAction *action) const
{
    return utils::itemForEntity<ActionItem, MscAction>(action, &d->m_scene);
}

TimerItem *ChartViewModel::itemForTimer(MscTimer *timer) const
{
    return utils::itemForEntity<TimerItem, MscTimer>(timer, &d->m_scene);
}

/*!
   \brief ChartViewModel::nearestInstance
   Returns the instance that is close enough to be used by an event.
   \param pos
 */
MscInstance *ChartViewModel::nearestInstance(const QPointF &pos)
{
    static const qreal add_space = 30;
    static const QMarginsF extra_margin = QMarginsF(add_space, add_space, add_space, add_space);

    qreal distance = std::numeric_limits<int>::max();
    MscInstance *instance = nullptr;
    for (auto item : d->m_instanceItems) {
        const QRectF itemRect = item->sceneBoundingRect().marginsAdded(extra_margin);
        if (!itemRect.contains(pos)) {
            continue;
        }
        qreal dist = std::abs(itemRect.center().x() - pos.x());
        if (dist < distance) {
            distance = dist;
            instance = item->modelItem();
        }
    }
    return instance;
}

int ChartViewModel::eventIndex(qreal y)
{
    int idx = 0;
    for (auto item : d->m_instanceEventItems) {
        if (item->y() < y) {
            ++idx;
        }
    }
    return idx;
}

void ChartViewModel::updateLayout()
{
    if (d->m_layoutDirty) {
        return;
    }

    d->m_layoutDirty = true;
    QMetaObject::invokeMethod(this, "relayout", Qt::QueuedConnection);
}

QVector<QGraphicsObject *> ChartViewModel::instanceEventItems(MscInstance *instance) const
{
    QVector<QGraphicsObject *> res;

    const QVector<MscInstanceEvent *> &events = currentChart()->eventsForInstance(instance);
    for (MscInstanceEvent *event : events) {
        switch (event->entityType()) {
        case MscEntity::EntityType::Message: {
            if (MessageItem *item = itemForMessage(static_cast<MscMessage *>(event)))
                res.append(item);
            break;
        }
        case MscEntity::EntityType::Condition: {
            if (ConditionItem *item = itemForCondition(static_cast<MscCondition *>(event)))
                res.append(item);
            break;
        }
        default: {
            qWarning() << Q_FUNC_INFO << "ignored entity of type:" << event->entityType();
            break;
        }
        }
    }

    return res;
}

InstanceItem *ChartViewModel::createDefaultInstanceItem(MscInstance *orphanInstance, const QPointF &pos)
{
    if (currentChart()) {
        if (!orphanInstance) {
            orphanInstance = new MscInstance(tr("Instance_%1").arg(currentChart()->instances().size()));
            currentChart()->addInstance(orphanInstance);
        }

        InstanceItem *instanceItem = InstanceItem::createDefaultItem(orphanInstance, pos);
        connect(instanceItem, &InstanceItem::needRelayout, this, &ChartViewModel::relayout);

        const qreal axisHeight = d->calcInstanceAxisHeight();
        if (!qFuzzyIsNull(axisHeight))
            instanceItem->setAxisHeight(axisHeight);
        return instanceItem;
    }
    return nullptr;
}

bool ChartViewModel::removeInstanceItem(msc::InstanceItem *item)
{
    if (item && utils::removeSceneItem(item)) {
        delete item;
        return true;
    }

    return false;
}

msc::MessageItem *ChartViewModel::createDefaultMessageItem(msc::MscMessage *orphanMessage, const QPointF &pos)
{
    if (currentChart()) {
        if (!orphanMessage) {
            orphanMessage = new MscMessage(tr("Message_%1").arg(currentChart()->instanceEvents().size()));
            currentChart()->addInstanceEvent(orphanMessage);
        }

        return MessageItem::createDefaultItem(orphanMessage, pos);
    }
    return nullptr;
}

bool ChartViewModel::removeMessageItem(msc::MessageItem *item)
{
    if (item && utils::removeSceneItem(item)) {
        delete item;
        return true;
    }

    return false;
}

void ChartViewModel::removeInstanceItem(MscInstance *instance)
{
    msc::InstanceItem *item = itemForInstance(instance);
    if (item) {
        const int idx = d->m_instanceItems.indexOf(item);
        utils::removeSceneItem(item);
        d->m_instanceItems.remove(idx);
        delete item;
        updateLayout();
    }
}

void ChartViewModel::removeEventItem(MscInstanceEvent *event)
{
    msc::InteractiveObject *item = nullptr;
    int idx = -1;
    for (msc::InteractiveObject *eitem : d->m_instanceEventItems) {
        ++idx;
        if (eitem->modelEntity() == event) {
            item = eitem;
            break;
        }
    }

    if (item) {
        utils::removeSceneItem(item);
        if (idx >= 0) {
            d->m_instanceEventItems.remove(idx);
        }
        delete item;
        updateLayout();
    }
}

void ChartViewModel::ensureInstanceCreationAdded(MscMessage *msgCreate, MscInstance *dynamicInstance)
{
    if (!d->m_layoutInfo.m_dynamicInstances.contains(dynamicInstance)) {
        MscInstance *creatorInstance = dynamicInstance->explicitCreator();
        if (MessageItem *item = fillMessageItem(msgCreate, itemForInstance(creatorInstance),
                                                itemForInstance(dynamicInstance), 0.)) {
            d->m_layoutInfo.m_dynamicInstances.insert(dynamicInstance, item);
        }
    }

    if (d->m_layoutInfo.m_dynamicInstances.contains(dynamicInstance)
        && !d->m_layoutInfo.m_dynamicInstanceMarkers.contains(dynamicInstance)) {
        if (MessageItem *item = d->m_layoutInfo.m_dynamicInstances.value(dynamicInstance)) {
            d->m_layoutInfo.m_dynamicInstanceMarkers.insert(dynamicInstance, item);
        }
    }
};

MessageItem *ChartViewModel::addMessageItem(MscMessage *message)
{
    qreal instanceVertiacalOffset(0);
    auto findInstanceItem = [&](MscInstance *instance) {
        InstanceItem *res(nullptr);

        if (instance) {
            res = itemForInstance(instance);
            if (res->modelItem()->explicitCreator()) {
                ensureInstanceCreationAdded(message, res->modelItem());
                instanceVertiacalOffset += res->axis().p1().y();
            }
        }

        return res;
    };

    InstanceItem *sourceInstance(findInstanceItem(message->sourceInstance()));
    InstanceItem *targetInstance(findInstanceItem(message->targetInstance()));

    return fillMessageItem(message, sourceInstance, targetInstance, instanceVertiacalOffset);
}

ActionItem *ChartViewModel::addActionItem(MscAction *action)
{
    InstanceItem *instance(nullptr);
    qreal instanceVertiacalOffset(0);
    if (action->instance()) {
        instance = itemForInstance(action->instance());
        instanceVertiacalOffset = instance->axis().p1().y();
    }

    ActionItem *item = itemForAction(action);
    if (!item) {
        item = new ActionItem(action);
        connect(item, &ActionItem::moved, this, &ChartViewModel::onInstanceEventItemMoved, Qt::UniqueConnection);

        d->m_scene.addItem(item);
        d->m_instanceEventItems.append(item);
    }
    item->connectObjects(instance, d->m_layoutInfo.m_pos.y() + instanceVertiacalOffset);
    item->updateLayout();

    return item;
}

ConditionItem *ChartViewModel::addConditionItem(MscCondition *condition, ConditionItem *prevItem, QRectF &instancesRect)
{
    auto *item = itemForCondition(condition);
    if (!item) {
        item = new ConditionItem(condition);
        connect(item, &ConditionItem::moved, this, &ChartViewModel::onInstanceEventItemMoved, Qt::UniqueConnection);

        d->m_scene.addItem(item);
        d->m_instanceEventItems.append(item);

        connect(item, &ConditionItem::needRelayout, this, &ChartViewModel::relayout);
    }

    InstanceItem *instance = itemForInstance(condition->instance());
    qreal verticalOffset = instance->axis().p1().y();
    if (prevItem && (prevItem->modelItem()->instance() == condition->instance() || prevItem->modelItem()->shared())) {
        verticalOffset += prevItem->boundingRect().height() + d->InterMessageSpan;
    }

    item->connectObjects(instance, d->m_layoutInfo.m_pos.y() + verticalOffset, instancesRect);
    item->updateLayout();

    return item;
}

TimerItem *ChartViewModel::addTimerItem(MscTimer *timer)
{
    InstanceItem *instance(nullptr);
    qreal instanceVertiacalOffset(0);
    if (timer->instance()) {
        instance = itemForInstance(timer->instance());
        instanceVertiacalOffset = instance->axis().p1().y();
    }

    TimerItem *item = itemForTimer(timer);
    if (!item) {
        item = new TimerItem(timer, this);
        connect(item, &TimerItem::moved, this, &ChartViewModel::onInstanceEventItemMoved, Qt::UniqueConnection);

        d->m_scene.addItem(item);
        d->m_instanceEventItems.append(item);
    }
    item->connectObjects(instance, d->m_layoutInfo.m_pos.ry() + instanceVertiacalOffset);
    item->updateLayout();

    return item;
}

void ChartViewModel::onInstanceItemMoved(InstanceItem *instanceItem)
{
    const int currentIdx = d->m_currentChart->instances().indexOf(instanceItem->modelItem());
    Q_ASSERT(currentIdx >= 0);

    int nextIdx = 0;
    for (auto inst : d->m_instanceItems) {
        if (inst != instanceItem) {
            if (instanceItem->x() > inst->x()) {
                ++nextIdx;
            }
        }
    }

    if (currentIdx != nextIdx) {
        msc::cmd::CommandsStack::push(msc::cmd::MoveInstance,
                                      { QVariant::fromValue<MscInstance *>(instanceItem->modelItem()), nextIdx,
                                        QVariant::fromValue<MscChart *>(d->m_currentChart) });
    }

    updateContentBounds();
}

void ChartViewModel::onInstanceEventItemMoved(InteractiveObject *item)
{
    auto actionItem = qobject_cast<ActionItem *>(item);
    if (actionItem) {
        MscInstance *newInstance = nearestInstance(actionItem->sceneBoundingRect().center());
        const int currentIdx = d->m_currentChart->instanceEvents().indexOf(actionItem->modelItem());
        const int newIdx = eventIndex(item->y());
        if (newInstance != actionItem->modelItem()->instance() || newIdx != currentIdx) {
            msc::cmd::CommandsStack::push(msc::cmd::MoveAction,
                                          { QVariant::fromValue<MscAction *>(actionItem->modelItem()), newIdx,
                                            QVariant::fromValue<MscInstance *>(newInstance),
                                            QVariant::fromValue<MscChart *>(d->m_currentChart) });
        } else {
            updateLayout();
        }
    }

    auto conditionItem = qobject_cast<ConditionItem *>(item);
    if (conditionItem) {
        MscInstance *newInstance = nearestInstance(conditionItem->sceneBoundingRect().center());
        const int currentIdx = d->m_currentChart->instanceEvents().indexOf(conditionItem->modelItem());
        const int newIdx = eventIndex(item->y());
        if (newInstance != conditionItem->modelItem()->instance() || newIdx != currentIdx) {
            msc::cmd::CommandsStack::push(msc::cmd::MoveCondition,
                                          { QVariant::fromValue<MscCondition *>(conditionItem->modelItem()), newIdx,
                                            QVariant::fromValue<MscInstance *>(newInstance),
                                            QVariant::fromValue<MscChart *>(d->m_currentChart) });
        } else {
            updateLayout();
        }
    }

    auto timerItem = qobject_cast<TimerItem *>(item);
    if (timerItem) {
        MscInstance *newInstance = nearestInstance(timerItem->sceneBoundingRect().center());
        const int currentIdx = d->m_currentChart->instanceEvents().indexOf(timerItem->modelItem());
        const int newIdx = eventIndex(item->y());
        if (newInstance != timerItem->modelItem()->instance() || newIdx != currentIdx) {
            msc::cmd::CommandsStack::push(msc::cmd::MoveTimer,
                                          { QVariant::fromValue<MscTimer *>(timerItem->modelItem()), newIdx,
                                            QVariant::fromValue<MscInstance *>(newInstance),
                                            QVariant::fromValue<MscChart *>(d->m_currentChart) });
        } else {
            updateLayout();
        }
    }
}

void ChartViewModel::onMessageRetargeted(MessageItem *item, const QPointF &pos, MscMessage::EndType endType)
{
    Q_ASSERT(item);
    Q_ASSERT(item->modelItem());

    MscInstance *newInstance = nearestInstance(pos);
    MscInstance *currentInstance = endType == MscMessage::EndType::SOURCE_TAIL ? item->modelItem()->sourceInstance()
                                                                               : item->modelItem()->targetInstance();
    MscInstance *otherInstance = endType == MscMessage::EndType::SOURCE_TAIL ? item->modelItem()->targetInstance()
                                                                             : item->modelItem()->sourceInstance();
    const int currentIdx = d->m_currentChart->instanceEvents().indexOf(item->modelItem());
    const int newIdx = eventIndex(pos.y());
    if ((newInstance != currentInstance && newInstance != otherInstance) || newIdx != currentIdx) {
        msc::cmd::CommandsStack::push(msc::cmd::RetargetMessage,
                                      { QVariant::fromValue<MscMessage *>(item->modelItem()), newIdx,
                                        QVariant::fromValue<MscInstance *>(newInstance),
                                        QVariant::fromValue<MscMessage::EndType>(endType),
                                        QVariant::fromValue<MscChart *>(d->m_currentChart) });
    } else {
        clearScene();
        updateLayout();
    }
}

QSizeF ChartViewModel::preferredChartBoxSize() const
{
    return d->m_layoutInfo.m_preferredBox;
}

void ChartViewModel::setPreferredChartBoxSize(const QSizeF &size)
{
    d->m_layoutInfo.m_preferredBox = size;
}

} // namespace msc
