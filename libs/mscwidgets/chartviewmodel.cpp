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
#include "conditionitem.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "baseitems/common/utils.h"
#include "commands/common/commandsstack.h"

#include <mscaction.h>
#include <mscchart.h>
#include <msccondition.h>
#include <mscinstance.h>
#include <mscmessage.h>

#include <QGraphicsScene>
#include <QVector>
#include <QPointer>
#include <QDebug>

#include <cmath>
#include <limits>

namespace msc {

template<typename ItemType, typename MscEntityType>
ItemType *itemForEntity(MscEntityType *event, QGraphicsScene *scene)
{
    if (event)
        for (ItemType *item : utils::toplevelItems<ItemType>(scene))
            if (item && item->modelItem()->internalId() == event->internalId())
                return item;

    return nullptr;
}

/*!
   \class  ChartViewModel is the model containing the scene graph of the currently selected/visible
   MSC chart (showing instances, messages, ...)
 */

struct ChartViewModelPrivate {
    ChartViewModelPrivate() {}

    QGraphicsScene m_scene;
    QVector<msc::InstanceItem *> m_instanceItems;
    QVector<msc::InteractiveObject *> m_instanceEventItems;
    QPointer<msc::MscChart> m_currentChart = nullptr;
    static constexpr qreal InterMessageSpan = 20.;
    static constexpr qreal InterInstanceSpan = 100.;
    bool m_layoutDirty = false;

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

    Q_EMIT currentChartChanged(d->m_currentChart);
}

MessageItem *ChartViewModel::fillMessageItem(MscMessage *message, InstanceItem *sourceItem, InstanceItem *targetItem,
                                             qreal newY)
{
    MessageItem *item = itemForMessage(message);
    if (!item) {
        item = new MessageItem(message);

        if (item->isCreator() && targetItem && targetItem->modelItem() == message->targetInstance()) {
            QLineF axisLine(targetItem->axis());
            axisLine.setP1({ axisLine.x1(), newY + InstanceItem::StartSymbolHeight / 2. });
            const qreal deltaY = targetItem->axis().length() - axisLine.length();

            targetItem->setAxisHeight(axisLine.length());
            targetItem->moveBy(0., deltaY - InstanceItem::StartSymbolHeight * 2.);
            newY -= 2. * InstanceItem::StartSymbolHeight;
        } else if (targetItem && targetItem->modelItem()->explicitCreate()) {
            newY -= targetItem->axis().y1();
        } else if (sourceItem && sourceItem->modelItem()->explicitCreate()) {
            newY -= sourceItem->axis().y1();
        } else {
        }

        d->m_scene.addItem(item);
        d->m_instanceEventItems.append(item);

        item->connectObjects(sourceItem, targetItem, newY);

    } else {
        qDebug() << Q_FUNC_INFO << "The layout update is broken (not implemented yet) here :(";
    }

    return item;
}

MscInstance *ChartViewModel::nearestInstance(double x)
{
    double distance = std::numeric_limits<int>::max();
    MscInstance *instance = nullptr;
    for (auto item : d->m_instanceItems) {
        double dist = std::abs(item->x() - x);
        if (dist < distance) {
            distance = dist;
            instance = item->modelItem();
        }
    }
    return instance;
}

int ChartViewModel::eventIndex(double y)
{
    int idx = 0;
    for (auto item : d->m_instanceEventItems) {
        if (item->y() < y) {
            ++idx;
        }
    }
    return idx;
}

void ChartViewModel::relayout()
{
    double x = .0;

    QRectF totalRect;
    for (MscInstance *instance : d->m_currentChart->instances()) {
        InstanceItem *item = itemForInstance(instance);
        if (!item) {
            item = createDefaultInstanceItem(instance, QPointF());
            connect(item, &InstanceItem::moved, this, &ChartViewModel::onInstanceItemMoved, Qt::UniqueConnection);
            d->m_scene.addItem(item);
            d->m_instanceItems.append(item);
            item->setX(x);
        }

        item->setKind(instance->kind());

        x += d->InterInstanceSpan + item->boundingRect().width();

        if (!instance->explicitCreate()) {
            // move axis start to the scene's Y0:
            const QLineF &axisLine(item->axis());
            item->moveBy(0., -qMin(axisLine.y1(), axisLine.y2()));
        }

        totalRect = totalRect.united(item->boundingRect().translated(item->pos()));
    }

    qreal y(d->InterMessageSpan);
    auto instancesRect(totalRect);
    QPointer<ConditionItem> prevItem = nullptr;

    for (MscInstanceEvent *instanceEvent : d->m_currentChart->instanceEvents()) {
        // TODO: Handle timers and conditions
        if (instanceEvent->entityType() == MscEntity::EntityType::Message) {
            auto message = static_cast<MscMessage *>(instanceEvent);

            InstanceItem *sourceInstance(nullptr);
            qreal instanceVertiacalOffset(0);
            if (message->sourceInstance()) {
                sourceInstance = itemForInstance(message->sourceInstance());
                if (sourceInstance->modelItem()->explicitCreate())
                    instanceVertiacalOffset += sourceInstance->axis().p1().y();
            }
            InstanceItem *targetInstance(nullptr);
            if (message->targetInstance()) {
                targetInstance = itemForInstance(message->targetInstance());
                if (targetInstance->modelItem()->explicitCreate())
                    instanceVertiacalOffset += targetInstance->axis().p1().y();
            }

            if (MessageItem *item =
                        fillMessageItem(message, sourceInstance, targetInstance, y + instanceVertiacalOffset)) {

                y += item->boundingRect().height() + d->InterMessageSpan;

                totalRect = totalRect.united(item->boundingRect().translated(item->pos()));
            }
        }
        if (instanceEvent->entityType() == MscEntity::EntityType::Action) {
            auto action = static_cast<MscAction *>(instanceEvent);

            InstanceItem *instance(nullptr);
            qreal instanceVertiacalOffset(0);
            if (action->instance()) {
                instance = itemForInstance(action->instance());
                instanceVertiacalOffset = instance->axis().p1().y();
            }

            ActionItem *item = itemForAction(action);
            if (!item) {
                item = new ActionItem(action);
                connect(item, &ActionItem::moved, this, &ChartViewModel::onInstanceEventItemMoved,
                        Qt::UniqueConnection);

                d->m_scene.addItem(item);
                d->m_instanceEventItems.append(item);
            }
            item->connectObjects(instance, y + instanceVertiacalOffset);
            item->updateLayout();
            y += item->boundingRect().height() + d->InterMessageSpan;

            totalRect = totalRect.united(item->boundingRect().translated(item->pos()));
        }
        if (instanceEvent->entityType() == MscEntity::EntityType::Condition) {
            auto *condition = static_cast<MscCondition *>(instanceEvent);

            auto *item = itemForCondition(condition);
            if (!item) {
                item = new ConditionItem(condition);
                connect(item, &ConditionItem::moved, this, &ChartViewModel::onInstanceEventItemMoved,
                        Qt::UniqueConnection);

                d->m_scene.addItem(item);
                d->m_instanceEventItems.append(item);

                connect(item, &ConditionItem::needRelayout, this, &ChartViewModel::relayout);
            }

            if (prevItem
                && (prevItem->modelItem()->instance() == condition->instance() || prevItem->modelItem()->shared())) {
                y += prevItem->boundingRect().height() + d->InterMessageSpan;
            }

            InstanceItem *instance = itemForInstance(condition->instance());
            item->connectObjects(instance, y + instance->axis().p1().y(), instancesRect);
            item->updateLayout();

            prevItem = item;

            y += item->boundingRect().height() + d->InterMessageSpan;

            totalRect = totalRect.united(item->boundingRect().translated(item->pos()));
        } else {
            prevItem = nullptr;
        }
    }

    actualizeInstancesHeights(y);

    // actualize scene's rect to avoid flickering on first show:
    static constexpr qreal margin(50.);
    totalRect.adjust(-margin, -margin, margin, margin);
    d->m_scene.setSceneRect(totalRect);

    d->m_layoutDirty = false;
}

void ChartViewModel::actualizeInstancesHeights(qreal height) const
{
    for (MscInstance *instance : d->m_currentChart->instances()) {
        if (InstanceItem *instanceItem = itemForInstance(instance)) {

            if (instance->explicitCreate()) {
                updateCreatedInstanceHeight(instanceItem);
            }

            if (instance->explicitStop()) {
                updateStoppedInstanceHeight(instanceItem);
            } else {
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
            const qreal bottomY = bottommostEvent->boundingRect().translated(bottommostEvent->pos()).bottom();
            QLineF axisLine(instanceItem->axis());
            axisLine.setP2({ axisLine.x2(), bottomY });
            instanceItem->setAxisHeight(axisLine.length());
        }
    }
}

void ChartViewModel::updateCreatedInstanceHeight(InstanceItem *) const {}

InstanceItem *ChartViewModel::itemForInstance(msc::MscInstance *instance) const
{
    return itemForEntity<InstanceItem, MscInstance>(instance, &d->m_scene);
}

MessageItem *ChartViewModel::itemForMessage(MscMessage *message) const
{
    return itemForEntity<MessageItem, MscMessage>(message, &d->m_scene);
}

ConditionItem *ChartViewModel::itemForCondition(MscCondition *condition) const
{
    return itemForEntity<ConditionItem, MscCondition>(condition, &d->m_scene);
}

ActionItem *ChartViewModel::itemForAction(MscAction *action) const
{
    return itemForEntity<ActionItem, MscAction>(action, &d->m_scene);
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
            qDebug() << Q_FUNC_INFO << "ignored entity of type:" << event->entityType();
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
        connect(instanceItem, &InstanceItem::needRearrange, this, &ChartViewModel::rearrangeInstances);

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

void ChartViewModel::rearrangeInstances()
{
    QVector<InstanceItem *> instanceItems = { utils::toplevelItems<InstanceItem>(graphicsScene()).toVector() };

    std::sort(instanceItems.begin(), instanceItems.end(),
              [](const InstanceItem *const a, const InstanceItem *const b) { return a->pos().x() < b->pos().x(); });

    for (int i = 0; i < instanceItems.size(); ++i)
        currentChart()->updateInstancePos(instanceItems.at(i)->modelItem(), i);

    relayout();
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
    int idx = 0;
    for (msc::InteractiveObject *eitem : d->m_instanceEventItems) {
        if (eitem->modelEntity() == event) {
            item = eitem;
            break;
        }
        ++idx;
    }

    if (item) {
        utils::removeSceneItem(item);
        d->m_instanceEventItems.remove(idx);
        delete item;
        updateLayout();
    }
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
}

void ChartViewModel::onInstanceEventItemMoved(InteractiveObject *item)
{
    auto actionItem = qobject_cast<ActionItem *>(item);
    if (actionItem) {
        MscInstance *newInstance = nearestInstance(actionItem->x());
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
        MscInstance *newInstance = nearestInstance(conditionItem->x());
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
}

} // namespace msc
