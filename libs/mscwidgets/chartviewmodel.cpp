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

#include <mscaction.h>
#include <mscchart.h>
#include <msccondition.h>
#include <mscinstance.h>
#include <mscmessage.h>

#include <QGraphicsScene>
#include <QVector>
#include <QPointer>
#include <QDebug>

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
    ChartViewModelPrivate(ChartViewModel *parent)
        : m_currentChart(new msc::MscChart(parent))
    {
    }

    QGraphicsScene m_scene;
    QVector<msc::InstanceItem *> m_instanceItems;
    QVector<msc::InteractiveObject *> m_instanceEventItems;
    QPointer<msc::MscChart> m_currentChart = nullptr;
    static constexpr qreal InterMessageSpan = 20.;
    static constexpr qreal InterInstanceSpan = 100.;

    qreal calcInstanceAxisHeight() const
    {
        static constexpr qreal oneMessageHeight = 50.;
        const int eventsCount = qMax(1, m_currentChart->instanceEvents().size());
        return eventsCount * (oneMessageHeight + ChartViewModelPrivate::InterMessageSpan);
    }
};

ChartViewModel::ChartViewModel(QObject *parent)
    : QObject(parent)
    , d(new ChartViewModelPrivate(this))
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

    d->m_currentChart = chart;

    clearScene();

    if (d->m_currentChart)
        relayout();

    Q_EMIT currentChartChanged(d->m_currentChart);
}

void ChartViewModel::relayout()
{
    double x = .0;

    QRectF totalRect;
    for (MscInstance *instance : d->m_currentChart->instances()) {
        InstanceItem *item = itemForInstance(instance);
        if (!item) {
            item = createDefaultInstanceItem(instance, QPointF());
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
    double instancesWidth(x - d->InterInstanceSpan);

    auto relayoutCondition = [&](const QString &messageName) {
        QPointer<ConditionItem> item = nullptr;
        QPointer<ConditionItem> prevItem = nullptr;

        for (MscInstanceEvent *instanceEvent : d->m_currentChart->instanceEvents()) {
            MscCondition *condition = nullptr;

            if (instanceEvent->entityType() == MscEntity::EntityType::Condition
                && (condition = dynamic_cast<MscCondition *>(instanceEvent))
                && condition->messageName() == messageName) {
                item = itemForCondition(condition);

                if (!item) {
                    item = new ConditionItem(condition);

                    d->m_scene.addItem(item);
                    d->m_instanceEventItems.append(item);
                }

                InstanceItem *instance = itemForInstance(condition->instance());
                item->buildLayout(condition->shared() ? instancesWidth : instance->boundingRect().width());

                if (prevItem
                    && (prevItem->modelItem()->instance() == condition->instance()
                        || prevItem->modelItem()->shared())) {
                    y += -prevItem->boundingRect().y();
                    y += prevItem->boundingRect().height() + d->InterMessageSpan;
                }

                item->setPos(condition->shared() ? 0 : (instance->axis().p1().x() - item->boundingRect().width() / 2),
                             y + instance->axis().p1().y());

                prevItem = item;
            }
        }

        if (item) {
            y += -item->boundingRect().y();
            y += item->boundingRect().height() + d->InterMessageSpan;
        }
    };

    // conditions with empty messageName
    relayoutCondition("");

    for (MscInstanceEvent *instanceEvent : d->m_currentChart->instanceEvents()) {
        // TODO: Handle timers and conditions
        if (instanceEvent->entityType() == MscEntity::EntityType::Message) {
            auto message = static_cast<MscMessage *>(instanceEvent);

            InstanceItem *sourceInstance(nullptr);
            qreal instanceVertiacalOffset(0);
            if (message->sourceInstance()) {
                sourceInstance = itemForInstance(message->sourceInstance());
                instanceVertiacalOffset = sourceInstance->axis().p1().y();
            }
            InstanceItem *targetInstance(nullptr);
            if (message->targetInstance()) {
                targetInstance = itemForInstance(message->targetInstance());
                if (qFuzzyIsNull(instanceVertiacalOffset))
                    instanceVertiacalOffset = targetInstance->axis().p1().y();
            }

            MessageItem *item = itemForMessage(message);
            if (!item) {
                item = new MessageItem(message);

                d->m_scene.addItem(item);
                d->m_instanceEventItems.append(item);
            }
            const QRectF boundingRect = item->boundingRect();
            y += -boundingRect.y();
            item->connectObjects(sourceInstance, targetInstance, y + instanceVertiacalOffset);
            y += boundingRect.height() + d->InterMessageSpan;

            totalRect = totalRect.united(item->boundingRect().translated(item->pos()));

            // condition after message
            relayoutCondition(item->name());
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

                d->m_scene.addItem(item);
                d->m_instanceEventItems.append(item);
            }
            item->connectObjects(instance, y + instanceVertiacalOffset);
            y += item->boundingRect().height() + d->InterMessageSpan;

            totalRect = totalRect.united(item->boundingRect().translated(item->pos()));

            // condition after message
            //            relayoutCondition(item->name());
        }
    }

    actualizeInstancesHeights(y);

    // actualize scene's rect to avoid flickering on first show:
    static constexpr qreal margin(50.);
    totalRect.adjust(-margin, -margin, margin, margin);
    d->m_scene.setSceneRect(totalRect);
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

        if (QGraphicsObject *bottomostEvent = events.last()) {
            const qreal bottomY = bottomostEvent->boundingRect().translated(bottomostEvent->pos()).bottom();
            QLineF axisLine(instanceItem->axis());
            axisLine.setP2({ axisLine.x2(), bottomY });
            instanceItem->setAxisHeight(axisLine.length());
        }
    }
}

void ChartViewModel::updateCreatedInstanceHeight(InstanceItem * /*instanceItem*/) const {}

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
        if (MscInstance *instance = item->modelItem()) {
            currentChart()->removeInstance(instance);
            delete instance;
        }
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
        if (MscMessage *message = item->modelItem()) {
            currentChart()->removeInstanceEvent(message);
            delete message;
        }
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

} // namespace msc
