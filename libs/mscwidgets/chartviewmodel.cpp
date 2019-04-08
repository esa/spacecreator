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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "chartviewmodel.h"

#include "actionitem.h"
#include "baseitems/commentitem.h"
#include "baseitems/common/utils.h"
#include "baseitems/instanceheaditem.h"
#include "chartitem.h"
#include "commands/common/commandsstack.h"
#include "conditionitem.h"
#include "coregionitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccoregion.h"
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
    QHash<QUuid, msc::InteractiveObject *> m_instanceEventItemsHash;
    QHash<QUuid, CommentItem *> m_comments;
    QPointer<msc::MscChart> m_currentChart = nullptr;
    static constexpr qreal InterMessageSpan = 20.;
    static constexpr qreal InterInstanceSpan = 100.;
    int m_visibleItemLimit = -1;
    bool m_layoutDirty = false;

    ChartViewLayoutInfo m_layoutInfo;

    qreal calcInstanceAxisHeight() const
    {
        static constexpr qreal oneMessageHeight = 50.;
        const int eventsCount = qMax(1,
                                     m_visibleItemLimit == -1 ? m_currentChart->instanceEvents().size()
                                                              : qMin(m_visibleItemLimit, m_instanceEventItems.size()));
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
    d->m_instanceEventItemsHash.clear();

    qDeleteAll(d->m_instanceItems);
    d->m_instanceItems.clear();

    qDeleteAll(d->m_comments);
    d->m_comments.clear();

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
    connect(d->m_currentChart, &msc::MscChart::commentChanged, this, &ChartViewModel::onEntityCommentChanged);

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
        d->m_instanceEventItemsHash.insert(message->internalId(), item);

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
    updateCommentForMscChart();
    applyCif();
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
            connect(instance, &MscEntity::commentChanged, this, &ChartViewModel::onEntityCommentChanged,
                    Qt::UniqueConnection);
            d->m_scene.addItem(item);
            d->m_instanceItems.append(item);
        }

        item->setX(d->m_layoutInfo.m_pos.x());
        item->setHighlightable(false);
        item->setDenominatorAndKind(instance->denominatorAndKind());

        d->m_layoutInfo.m_pos.rx() += d->InterInstanceSpan + item->boundingRect().width();

        if (!instance->explicitCreator()) {
            // move axis start to the scene's Y0:
            const QLineF &axisLine(item->axis());
            item->moveBy(0., -qMin(axisLine.y1(), axisLine.y2()));
        }

        d->m_layoutInfo.m_instancesRect = d->m_layoutInfo.m_instancesRect.united(item->sceneBoundingRect());

        updateCommentForInteractiveObject(item);
        if (!instance->comment().isEmpty()) {
            msc::CommentItem *commentItem = d->m_comments.value(instance->internalId());
            if (commentItem)
                d->m_layoutInfo.m_pos.rx() += commentItem->boundingRect().width() - d->InterInstanceSpan;
        }

        if (d->m_visibleItemLimit != -1 && instance->explicitStop()) {
            const QVector<MscInstanceEvent *> instanceEvents = d->m_currentChart->eventsForInstance(instance);
            const QVector<MscInstanceEvent *> chartEvents = d->m_currentChart->instanceEvents();
            auto chartEvItEnd = chartEvents.crbegin();
            std::advance(chartEvItEnd, qMin(d->m_visibleItemLimit, chartEvents.size()));
            auto instEvItEnd = instanceEvents.crbegin();
            std::advance(instEvItEnd, qMin(d->m_visibleItemLimit, instanceEvents.size()));
            for (auto chartEvIt = chartEvents.crbegin(); chartEvIt != chartEvItEnd; ++chartEvIt) {
                auto it = std::find(instanceEvents.crbegin(), instEvItEnd, *chartEvIt);
                if (it != instEvItEnd) {
                    item->setVisible(true);
                    return;
                }
            }
            item->setVisible(false);
        }
    }
}

void ChartViewModel::addInstanceEventItems()
{
    InteractiveObject *instanceEventItem(nullptr);
    const QVector<MscInstanceEvent *> &chartEvents = d->m_currentChart->instanceEvents();
    auto it = chartEvents.begin();
    if (d->m_visibleItemLimit != -1) {
        for (; it != chartEvents.end(); ++it) {
            if (std::distance(it, chartEvents.end()) <= d->m_visibleItemLimit)
                break;
            removeEventItem(*it);
        }
    }
    for (; it != chartEvents.end(); ++it) {
        MscInstanceEvent *instanceEvent = *it;
        Q_ASSERT(instanceEvent);

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
        case MscEntity::EntityType::Coregion: {
            instanceEventItem = addCoregionItem(static_cast<MscCoregion *>(instanceEvent));
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

        if (instanceEventItem) {
            polishAddedEventItem(instanceEvent, instanceEventItem);
            updateCommentForInteractiveObject(instanceEventItem);
        }
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
    case MscEntity::EntityType::Coregion: {
        MscCoregion *coregion = static_cast<MscCoregion *>(event);
        if (coregion->type() == MscCoregion::Type::Begin) {
            const qreal targetTop = d->m_layoutInfo.m_pos.y() + d->InterMessageSpan;
            item->setY(targetTop);
        } else {
            qobject_cast<CoregionItem *>(item)->updateLayout();
        }
        d->m_layoutInfo.m_pos.ry() += 2 * d->InterMessageSpan;
    } break;
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

void ChartViewModel::updateCommentForMscChart()
{
    updateComment(currentChart());
}

void ChartViewModel::updateCommentForInteractiveObject(InteractiveObject *iObj)
{
    if (!iObj)
        return;

    updateComment(iObj->modelEntity(), iObj);
}

void ChartViewModel::updateComment(msc::MscEntity *entity, msc::InteractiveObject *iObj)
{
    if (!entity)
        return;

    const QUuid id = entity->internalId();
    CommentItem *commentItem = d->m_comments.value(id, nullptr);
    if (!entity->comment().isEmpty()) {
        if (!commentItem) {
            commentItem = new CommentItem;
            commentItem->attachTo(iObj);
            if (iObj)
                connect(iObj, &InteractiveObject::needRelayout, this, &ChartViewModel::updateLayout,
                        Qt::UniqueConnection);
            connect(entity, &MscEntity::commentChanged, this, &ChartViewModel::onEntityCommentChanged,
                    Qt::UniqueConnection);
            connect(commentItem, &CommentItem::commentChanged, this,
                    [this, entity](const QString &comment) { onInteractiveObjectCommentChange(entity, comment); });

            d->m_scene.addItem(commentItem);
            d->m_comments.insert(id, commentItem);
        }
        commentItem->setVisible(true);
        commentItem->setText(entity->comment());
        if (!iObj && entity == currentChart()) {
            commentItem->setPos(d->m_layoutInfo.m_pos.x() + d->InterInstanceSpan / 4,
                                d->m_layoutInfo.m_instancesRect.y());
        }
    } else if (commentItem) {
        commentItem->setVisible(false);
    }
}

void ChartViewModel::updateContentBounds()
{
    QRectF totalRect;
    for (InstanceItem *gi : d->m_instanceItems)
        totalRect = totalRect.united(gi->sceneBoundingRect());

    for (InteractiveObject *gi : d->m_instanceEventItems) {
        if (gi->modelEntity() && gi->modelEntity()->entityType() == MscEntity::EntityType::Message) {
            MscMessage *message = static_cast<MscMessage *>(gi->modelEntity());
            if (message->isGlobal()) // ignore, it will be connected to the ChartItem edge lately
                continue;
        }
        totalRect = totalRect.united(gi->sceneBoundingRect());
    }
    for (CommentItem *commentItem : d->m_comments)
        totalRect = totalRect.united(commentItem->sceneBoundingRect());

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

    const int toplevelItemsCount = d->m_instanceItems.size() + d->m_instanceEventItems.size();
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
                updateStoppedInstanceHeight(instanceItem, height);
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

void ChartViewModel::updateStoppedInstanceHeight(InstanceItem *instanceItem, qreal totalH) const
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
    } else {
        updateCreatedInstanceHeight(instanceItem, totalH);
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
    if (!message)
        return nullptr;

    return qobject_cast<MessageItem *>(d->m_instanceEventItemsHash.value(message->internalId()));
}

ConditionItem *ChartViewModel::itemForCondition(MscCondition *condition) const
{
    if (!condition)
        return nullptr;

    return qobject_cast<ConditionItem *>(d->m_instanceEventItemsHash.value(condition->internalId()));
}

ActionItem *ChartViewModel::itemForAction(MscAction *action) const
{
    if (!action)
        return nullptr;

    return qobject_cast<ActionItem *>(d->m_instanceEventItemsHash.value(action->internalId()));
}

TimerItem *ChartViewModel::itemForTimer(MscTimer *timer) const
{
    if (!timer)
        return nullptr;

    return qobject_cast<TimerItem *>(d->m_instanceEventItemsHash.value(timer->internalId()));
}

CoregionItem *ChartViewModel::itemForCoregion(MscCoregion *coregion) const
{
    if (!coregion)
        return nullptr;

    return qobject_cast<CoregionItem *>(d->m_instanceEventItemsHash.value(coregion->internalId()));
}

InteractiveObject *ChartViewModel::itemForEntity(MscEntity *entity) const
{
    if (!entity)
        return nullptr;

    return qobject_cast<InteractiveObject *>(d->m_instanceEventItemsHash.value(entity->internalId()));
}

CommentItem *ChartViewModel::commentForEntity(MscEntity *entity)
{
    if (!entity)
        return nullptr;

    return d->m_comments.value(entity->internalId());
}

/*!
   \brief ChartViewModel::nearestEntity
   Returns the entity that is close enough to be used by an event.
   \param pos
 */
MscEntity *ChartViewModel::nearestEntity(const QPointF &pos)
{
    static const qreal add_space = 10;
    static const QMarginsF extra_margin = QMarginsF(add_space, add_space, add_space, add_space);

    qreal distance = std::numeric_limits<int>::max();
    auto getNearest = [](qreal &distance, InteractiveObject *iObj, const QPointF &pos) -> MscEntity * {
        MscEntity *entity = nullptr;
        const QRectF itemRect = iObj->sceneBoundingRect().marginsAdded(extra_margin);
        if (!itemRect.contains(pos) || !iObj->modelEntity())
            return nullptr;

        qreal dist = std::abs(itemRect.center().x() - pos.x());
        if (dist < distance) {
            distance = dist;
            entity = iObj->modelEntity();
        }
        return entity;
    };

    MscEntity *entity = nullptr;
    for (auto item : d->m_instanceItems)
        if ((entity = getNearest(distance, item, pos)))
            return entity;

    for (auto item : d->m_instanceEventItems)
        if ((entity = getNearest(distance, item, pos)))
            return entity;

    return nullptr;
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
            if (auto coregionItem = qobject_cast<CoregionItem *>(item)) {
                if (coregionItem->sceneBoundingRect().bottom() < y)
                    ++idx;
            }
        }
    }
    return idx;
}

MscInstanceEvent *ChartViewModel::eventAtPosition(const QPointF &scenePos)
{
    for (auto eventItem : d->m_instanceEventItems) {
        if (eventItem->sceneBoundingRect().contains(scenePos))
            return qobject_cast<MscInstanceEvent *>(eventItem->modelEntity());
    }
    return nullptr;
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
            if (auto item = itemForMessage(static_cast<MscMessage *>(event)))
                res.append(item);
            break;
        }
        case MscEntity::EntityType::Condition: {
            if (auto item = itemForCondition(static_cast<MscCondition *>(event)))
                res.append(item);
            break;
        }
        case MscEntity::EntityType::Action: {
            if (auto item = itemForAction(static_cast<MscAction *>(event)))
                res.append(item);
            break;
        }
        case MscEntity::EntityType::Timer: {
            if (auto item = itemForTimer(static_cast<MscTimer *>(event)))
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

        InstanceItem *instanceItem = InstanceItem::createDefaultItem(orphanInstance, currentChart(), pos);
        connect(instanceItem, &InteractiveObject::needRelayout, this, &ChartViewModel::updateLayout);

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
    msc::InteractiveObject *item = d->m_instanceEventItemsHash.take(event->internalId());
    if (item) {
        utils::removeSceneItem(item);
        d->m_instanceEventItems.removeOne(item);
        delete item;
        updateLayout();
    }
}

void ChartViewModel::onEntityCommentChanged(const QString &comment)
{
    Q_UNUSED(comment);

    msc::MscEntity *entity = qobject_cast<msc::MscEntity *>(sender());
    if (!entity)
        return;

    if (entity == d->m_currentChart) {
        updateCommentForMscChart();
    } else {
        updateCommentForInteractiveObject(
                utils::itemForEntity<msc::InteractiveObject, msc::MscEntity>(entity, &d->m_scene));
    }
}

void ChartViewModel::onInteractiveObjectCommentChange(MscEntity *entity, const QString &comment)
{
    msc::cmd::CommandsStack::push(msc::cmd::ChangeComment, { QVariant::fromValue<MscEntity *>(entity), comment });
    updateLayout();
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
        d->m_instanceEventItemsHash.insert(action->internalId(), item);
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
        d->m_instanceEventItemsHash.insert(condition->internalId(), item);

        connect(item, &InteractiveObject::needRelayout, this, &ChartViewModel::updateLayout);
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
        d->m_instanceEventItemsHash.insert(timer->internalId(), item);
    }
    item->connectObjects(instance, d->m_layoutInfo.m_pos.ry() + instanceVertiacalOffset);

    return item;
}

CoregionItem *ChartViewModel::addCoregionItem(MscCoregion *coregion)
{
    InstanceItem *instance(nullptr);
    qreal instanceVertiacalOffset(0);
    if (coregion->instance()) {
        instance = itemForInstance(coregion->instance());
        instanceVertiacalOffset = instance->axis().p1().y();
    }

    CoregionItem *item = nullptr;
    if (coregion->type() == MscCoregion::Type::Begin) {
        item = itemForCoregion(coregion);
    } else {
        auto isCoregionBegin = [coregion](MscInstanceEvent *event) {
            if (event->entityType() != MscEntity::EntityType::Coregion)
                return false;

            if (!event->relatesTo(coregion->instance()))
                return false;

            return static_cast<MscCoregion *>(event)->type() == MscCoregion::Type::Begin;
        };

        const QVector<MscInstanceEvent *> &events = currentChart()->instanceEvents();
        auto it = std::find(events.rbegin(), events.rend(), coregion);
        auto res = std::find_if(it, events.rend(), isCoregionBegin);
        if (res == events.rend())
            return nullptr;

        item = itemForCoregion(qobject_cast<MscCoregion *>(*res));
    }

    if (!item) {
        item = new CoregionItem(this);

        connect(item, &TimerItem::moved, this, &ChartViewModel::onInstanceEventItemMoved, Qt::UniqueConnection);

        d->m_scene.addItem(item);
        d->m_instanceEventItems.append(item);
        d->m_instanceEventItemsHash.insert(coregion->internalId(), item);
    }
    if (coregion->type() == MscCoregion::Type::Begin) {
        item->setBegin(coregion);
        item->connectObjects(instance, d->m_layoutInfo.m_pos.ry() + instanceVertiacalOffset);
    } else {
        item->setEnd(coregion);
    }

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
    MscMessage *message = item->modelItem();
    Q_ASSERT(message);

    MscInstance *newInstance = nearestInstance(pos);
    MscInstance *currentInstance =
            endType == MscMessage::EndType::SOURCE_TAIL ? message->sourceInstance() : message->targetInstance();
    MscInstance *otherInstance =
            endType == MscMessage::EndType::SOURCE_TAIL ? message->targetInstance() : message->sourceInstance();
    const int currentIdx = d->m_currentChart->instanceEvents().indexOf(message);
    const int newIdx = eventIndex(pos.y());
    if ((newInstance != currentInstance && newInstance != otherInstance) || newIdx != currentIdx) {
        msc::cmd::CommandsStack::push(msc::cmd::RetargetMessage,
                                      { QVariant::fromValue<MscMessage *>(message), newIdx,
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

int ChartViewModel::instanceOrderFromPos(const QPointF &scenePos)
{
    if (d->m_instanceItems.isEmpty())
        return -1;

    QMap<qreal, QPair<QLineF, InstanceItem *>> instanceByDistance;
    for (InstanceItem *instanceItem : d->m_instanceItems) {
        const QRectF &currInstanceRect = instanceItem->sceneBoundingRect();
        const QLineF line(currInstanceRect.center(), scenePos);
        instanceByDistance.insert(line.length(), qMakePair(line, instanceItem));
    }

    const QPair<QLineF, InstanceItem *> &nearestPair = instanceByDistance.first();

    const QLineF distance = nearestPair.first;
    const int existentId = currentChart()->instances().indexOf(nearestPair.second->modelItem());
    if (-1 == existentId)
        return -1; // otherwise it could be prepended

    return existentId + (distance.dx() <= 0. ? 0 : 1);
}

void ChartViewModel::setVisibleItemLimit(int number)
{
    d->m_visibleItemLimit = number;
    updateLayout();
}

void ChartViewModel::applyCif()
{
    // While CIF support is not complete it breaks almost all examples.
    // To keep the master in a usable state use this locally:
    /*
    const QList<InteractiveObject *> &toplevelItems = utils::toplevelItems<InteractiveObject>(graphicsScene());
    const int toplevelItemsCount = toplevelItems.size();
    for (int i = 0; i < toplevelItemsCount; ++i)
        if (InteractiveObject *gi = toplevelItems.at(i))
            gi->applyCif();
    */
}

} // namespace msc
