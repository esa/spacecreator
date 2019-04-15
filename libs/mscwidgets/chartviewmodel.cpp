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
#include "baseitems/common/coordinatesconverter.h"
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

        m_instancesCommonAxisStart = 0.;
    }

    QMap<MscInstance *, MessageItem *> m_dynamicInstances;
    QMap<MscInstance *, MessageItem *> m_dynamicInstanceMarkers;
    QRectF m_instancesRect;
    QSizeF m_preferredBox;
    QPointF m_pos;
    qreal m_instancesCommonAxisStart = 0.;

    QPointer<ChartItem> m_chartItem = nullptr;

private:
};

struct ChartViewModelPrivate {
    ChartViewModelPrivate() {}

    QGraphicsScene m_scene;
    QHash<QUuid, msc::InstanceItem *> m_instanceItems;
    QVector<msc::InstanceItem *> m_instanceItemsSorted;
    QHash<QUuid, msc::InteractiveObject *> m_instanceEventItems;
    QVector<msc::InteractiveObject *> m_instanceEventItemSorted;

    QHash<QUuid, CommentItem *> m_comments;
    QPointer<msc::MscChart> m_currentChart = nullptr;
    static constexpr qreal InterMessageSpan = 20.;
    static constexpr qreal InterInstanceSpan = 100.;
    int m_visibleItemLimit = -1;
    bool m_layoutDirty = false;

    ChartViewLayoutInfo m_layoutInfo;

    qreal calcInstanceAxisHeight() const
    {
        if (m_instanceItems.isEmpty()) {
            static constexpr qreal oneMessageHeight = 50.;
            const int eventsCount =
                    qMax(1,
                         m_visibleItemLimit == -1 ? m_currentChart->instanceEvents().size()
                                                  : qMin(m_visibleItemLimit, m_instanceEventItems.size()));
            return eventsCount * (oneMessageHeight + ChartViewModelPrivate::InterMessageSpan);
        }

        qreal height(0);
        for (InstanceItem *item : m_instanceItemsSorted)
            if (item->axisHeight() > height)
                height = item->axisHeight();
        return height;
    }

    // returns a single collection of items in:
    // m_instanceItems
    // m_instanceEventItems
    // m_comments
    int allItemsCount() const { return m_instanceItems.size() + m_instanceEventItems.size() + m_comments.size(); }
    QVector<InteractiveObject *> allItems() const
    {
        QVector<InteractiveObject *> items;
        items.reserve(allItemsCount());

        for (InstanceItem *item : m_instanceItemsSorted)
            items << item;

        for (InteractiveObject *item : m_instanceEventItemSorted)
            items << item;

        for (CommentItem *item : m_comments)
            items << item;

        return items;
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
    d->m_instanceEventItemSorted.clear();

    qDeleteAll(d->m_instanceItems);
    d->m_instanceItems.clear();
    d->m_instanceItemsSorted.clear();

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

    prepareChartBoxItem();

    const QRectF &initialChartRect = d->m_layoutInfo.m_chartItem->storedCustomRect();
    if (d->m_currentChart)
        doLayout();

    // restore initial chart box (stored in CIF), because it's overriden by actual content bounds
    if (!initialChartRect.isNull())
        d->m_layoutInfo.m_chartItem->setBox(initialChartRect);
    else if (!d->m_layoutInfo.m_preferredBox.isEmpty()) {
        const QRectF prefferedBox { d->m_layoutInfo.m_chartItem->box().topLeft(), d->m_layoutInfo.m_preferredBox };
        d->m_layoutInfo.m_chartItem->setBox(prefferedBox);
    }

    connect(d->m_currentChart, &msc::MscChart::instanceAdded, this, &ChartViewModel::updateLayout);
    connect(d->m_currentChart, &msc::MscChart::instanceRemoved, this,
            QOverload<msc::MscInstance *>::of(&ChartViewModel::removeInstanceItem));
    connect(d->m_currentChart, &msc::MscChart::instanceMoved, this,
            [this](MscInstance *instance, int, int) { onInstanceItemMoved(itemForInstance(instance)); });

    connect(d->m_currentChart, &msc::MscChart::instanceEventAdded, this, &ChartViewModel::updateLayout);
    connect(d->m_currentChart, &msc::MscChart::instanceEventRemoved, this, &ChartViewModel::removeEventItem);
    connect(d->m_currentChart, &msc::MscChart::eventMoved, this, &ChartViewModel::updateLayout);
    connect(d->m_currentChart, &msc::MscChart::messageRetargeted, this, &ChartViewModel::updateLayout);
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

        storeEntityItem(item);

        qreal deltaY(0.);
        if (isCreateMsg && !targetItem->geometryManagedByCif()) {
            QLineF axisLine(targetItem->axis());
            axisLine.setP1({ axisLine.x1(), newY + InstanceHeadItem::StartSymbolHeight / 2. });

            deltaY = targetItem->axis().length() - axisLine.length();

            if (!targetItem->modelItem()->explicitStop()) {
                axisLine.setP2({ axisLine.x2(), axisLine.y2() - axisLine.y1() });
            }

            targetItem->setAxisHeight(axisLine.length());
            targetItem->moveSilentlyBy({ 0., deltaY });
        }

        QPointF pntSource = sourceItem ? sourceItem->sceneBoundingRect().center() : d->m_layoutInfo.m_pos;
        pntSource.setY(newY);
        QPointF pntTarget = targetItem ? targetItem->sceneBoundingRect().center() : d->m_layoutInfo.m_pos;
        pntTarget.setY(newY);

        MessageItem::GeometryNotificationBlocker geometryNotificationBlocker(item);
        item->setInstances(sourceItem, targetItem);
        item->setMessagePoints({ pntSource, pntTarget }, MessageItem::CifUpdatePolicy::DontChange);
    }

    if (item)
        item->instantLayoutUpdate();

    return item;
}

void ChartViewModel::updateLayout()
{
    if (d->m_layoutDirty)
        return;

    d->m_layoutDirty = true;
    QMetaObject::invokeMethod(this, "doLayout", Qt::QueuedConnection);
}

void ChartViewModel::doLayout()
{
    d->m_layoutInfo.m_dynamicInstanceMarkers.clear();
    d->m_layoutInfo.m_pos = { 0., 0. };
    d->m_layoutInfo.m_instancesRect = QRectF();
    d->m_layoutInfo.m_instancesCommonAxisStart = 0.;

    prepareChartBoxItem();
    if (d->m_layoutInfo.m_chartItem)
        d->m_layoutInfo.m_chartItem->applyCif();

    // The calls order below DOES matter
    addInstanceItems(); // which are not highlightable now to avoid flickering
    addInstanceEventItems();
    disconnectItems();
    actualizeInstancesHeights(d->m_layoutInfo.m_pos.y() + d->InterMessageSpan);
    updateCommentForMscChart();
    updateContentBounds();
    connectItems();

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
            storeEntityItem(item);
        }

        item->setHighlightable(false);
        item->setDenominatorAndKind(instance->denominatorAndKind());

        const bool geomByCif = item->geometryManagedByCif();
        if (geomByCif)
            item->applyCif();
        else
            item->setX(d->m_layoutInfo.m_pos.x() + d->InterInstanceSpan);

        d->m_layoutInfo.m_pos.rx() = item->sceneBoundingRect().right();

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

    Q_ASSERT(d->m_currentChart->instances().size() == d->m_instanceItems.size());
    Q_ASSERT(d->m_currentChart->instances().size() == d->m_instanceItemsSorted.size());
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

    Q_ASSERT(d->m_currentChart->instanceEvents().size() == d->m_instanceEventItems.size());
    Q_ASSERT(d->m_currentChart->instanceEvents().size() == d->m_instanceEventItemSorted.size());
}

void ChartViewModel::polishAddedEventItem(MscInstanceEvent *event, InteractiveObject *item)
{
    auto moveNewItem = [&]() {
        qreal deltaY = 0.;
        const bool geometryFromCif = item->geometryManagedByCif();
        if (geometryFromCif)
            item->applyCif();
        else {
            const QRectF srcRect = item->sceneBoundingRect();
            const qreal targetTop = d->m_layoutInfo.m_pos.y() + d->InterMessageSpan;
            deltaY = targetTop - srcRect.top();
            item->moveSilentlyBy({ 0., deltaY });
        }

        d->m_layoutInfo.m_pos.ry() = item->sceneBoundingRect().bottom();

        return deltaY;
    };

    switch (event->entityType()) {
    case MscEntity::EntityType::Coregion: {
        MscCoregion *coregion = static_cast<MscCoregion *>(event);
        if (coregion->type() == MscCoregion::Type::Begin) {
            const qreal targetTop = d->m_layoutInfo.m_pos.y() + d->InterMessageSpan;
            item->setY(targetTop);
        } else {
            qobject_cast<CoregionItem *>(item)->scheduleLayoutUpdate();
        }
        d->m_layoutInfo.m_pos.ry() += 2 * d->InterMessageSpan;
    } break;
    case MscEntity::EntityType::Message:
    case MscEntity::EntityType::Create: {
        MscMessage *message = static_cast<MscMessage *>(event);
        MessageItem *messageItem = dynamic_cast<MessageItem *>(item);
        Q_ASSERT(messageItem != nullptr);
        MessageItem::GeometryNotificationBlocker geometryNotificationBlocker(messageItem);

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
                    if (!createdInstanceItem->geometryManagedByCif())
                        createdInstanceItem->moveBy(0., deltaY);

                    if (!messageItem->geometryManagedByCif()) {
                        // move arrow head to the created instance, shift other points accordingly:
                        QVector<QPointF> messagePoints = messageItem->messagePoints();
                        const QPointF newStart = { messagePoints.first().x(), messagePoints.first().y() + deltaY };
                        const QPointF delta = newStart - messagePoints.first();
                        for (int i = 0; i < messagePoints.size() - 1; ++i)
                            messagePoints[i] += delta;
                        messageItem->setMessagePoints(
                                messagePoints,
                                MessageItem::CifUpdatePolicy::DontChange); // changes its pos to the source point
                    }
                }
                messageItem->setInstances(creatorInstanceItem, createdInstanceItem);
            }
        }

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
                connect(iObj, &InteractiveObject::needUpdateLayout, this, &ChartViewModel::updateLayout,
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

/*!
   Returns the scene rectangle of all instances and events
 */
QRectF ChartViewModel::prepareContentRect() const
{
    QRectF totalRect;
    for (InteractiveObject *gi : d->allItems()) {
        QRectF currRect = gi->sceneBoundingRect();
        if (gi->modelEntity()
            && gi->modelEntity()->entityType() == MscEntity::EntityType::Message) // TODO: same for Create?
        {
            MscMessage *message = static_cast<MscMessage *>(gi->modelEntity());
            if (message->isGlobal()) {
                if (MessageItem *messageItem = dynamic_cast<MessageItem *>(gi)) {
                    // if it's a simple (two point) horizontal message only its height
                    // accounted since it will be extended to the chart edge afterwhile
                    // TODO: add support for complex global message
                    const QLineF arrowLine(messageItem->tail(), messageItem->head());
                    if (qFuzzyIsNull(arrowLine.dy())) {
                        const QPointF &currCenter = currRect.center();
                        currRect.setWidth(1.);
                        currRect.moveCenter(currCenter);
                    }
                }
            }
        }
        totalRect = totalRect.united(currRect);
    }
    return totalRect;
}

QLineF ChartViewModel::commonAxis() const
{
    qreal commonAxisStart { 0 }, commonAxisStop { 0 };

    for (InstanceItem *gi : d->m_instanceItems) {
        if (!gi->modelItem()->explicitCreator()) {
            commonAxisStart = qMax(commonAxisStart, gi->axis().p1().y());
        }
        if (!gi->modelItem()->explicitStop()) {
            commonAxisStop = qMax(commonAxisStop, gi->axis().p2().y());
        }
    }

    return QLineF(0., commonAxisStart, 0., commonAxisStop);
}

void ChartViewModel::onChartBoxChanged()
{
    const QRectF &contentRect = prepareContentRect();
    QRectF currentChartBox = d->m_layoutInfo.m_chartItem->box();

    if (currentChartBox.width() < contentRect.width())
        currentChartBox.setWidth(contentRect.width());
    if (currentChartBox.height() < contentRect.height())
        currentChartBox.setHeight(contentRect.height());
    /*
        // expand non stopped instances to the bottom
        for (InstanceItem *instanceItem : d->m_instanceItemsSorted) {
            if (instanceItem->modelItem()->explicitStop())
                continue;
            if (!qFuzzyCompare(1. + instanceItem->sceneBoundingRect().bottom(), 1. + currentChartBox.bottom())) {
                const qreal deltaH = currentChartBox.bottom() - instanceItem->sceneBoundingRect().bottom();
                qDebug() << "updating instance height:" << instanceItem->axisHeight() << deltaH
                         << (instanceItem->axisHeight() + deltaH);
                instanceItem->setAxisHeight(instanceItem->axisHeight() + deltaH);
            }
        }
    */
    // expand global messages
    for (InteractiveObject *io : d->m_instanceEventItemSorted)
        if (io->modelEntity()->entityType() == MscEntity::EntityType::Message)
            if (MessageItem *messageItem = qobject_cast<MessageItem *>(io))
                messageItem->onChartBoxChanged();

    if (currentChartBox == d->m_layoutInfo.m_chartItem->box())
        return;
    d->m_layoutInfo.m_chartItem->setBox(currentChartBox);
}

void ChartViewModel::updateContentBounds()
{
    if (!d->m_layoutInfo.m_chartItem)
        return;

    const bool customBoxUsed = d->m_layoutInfo.m_chartItem->geometryManagedByCif();
    QRectF chartBox =
            customBoxUsed ? d->m_layoutInfo.m_chartItem->storedCustomRect() : d->m_layoutInfo.m_chartItem->box();
    const QRectF &contentRect =
            prepareContentRect().marginsAdded(customBoxUsed ? QMarginsF() : ChartItem::chartMargins());
    if (chartBox.isEmpty())
        chartBox = contentRect;
    else {
        if (chartBox.width() < contentRect.width())
            chartBox.setWidth(contentRect.width());
        if (chartBox.height() < contentRect.height())
            chartBox.setHeight(contentRect.height());
    }
    d->m_layoutInfo.m_chartItem->setBox(chartBox);

    const int totalItemsCount = d->allItemsCount();
    d->m_layoutInfo.m_chartItem->setZValue(-totalItemsCount);
}

void ChartViewModel::actualizeInstancesHeights(qreal height) const
{
    for (InstanceItem *instanceItem : d->m_instanceItems) {
        bool updated(false);
        if (instanceItem->modelItem()->explicitStop()) {
            updateStoppedInstanceHeight(instanceItem, height);
            updated = true;
        } else if (instanceItem->modelItem()->explicitCreator()) {
            updateCreatedInstanceHeight(instanceItem, height);
            updated = true;
        }

        if (!updated) {
            instanceItem->setAxisHeight(height);
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
    return instance ? d->m_instanceItems.value(instance->internalId()) : nullptr;
}

template<class ItemType>
ItemType *ChartViewModel::itemForInstanceEvent(MscInstanceEvent *event) const
{
    return event ? static_cast<ItemType *>(d->m_instanceEventItems.value(event->internalId())) : nullptr;
}

MessageItem *ChartViewModel::itemForMessage(MscMessage *message) const
{
    return itemForInstanceEvent<MessageItem>(message);
}

ConditionItem *ChartViewModel::itemForCondition(MscCondition *condition) const
{
    return itemForInstanceEvent<ConditionItem>(condition);
}

ActionItem *ChartViewModel::itemForAction(MscAction *action) const
{
    return itemForInstanceEvent<ActionItem>(action);
}

TimerItem *ChartViewModel::itemForTimer(MscTimer *timer) const
{
    return itemForInstanceEvent<TimerItem>(timer);
}

CoregionItem *ChartViewModel::itemForCoregion(MscCoregion *coregion) const
{
    if (!coregion)
        return nullptr;

    return qobject_cast<CoregionItem *>(d->m_instanceEventItems.value(coregion->internalId()));
}

InteractiveObject *ChartViewModel::itemForEntity(MscEntity *entity) const
{
    if (!entity)
        return nullptr;

    const bool isInstance = entity->entityType() == MscEntity::EntityType::Instance;
    return isInstance ? d->m_instanceItems.value(entity->internalId())
                      : d->m_instanceEventItems.value(entity->internalId());
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
    for (auto item : d->allItems())
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
    for (auto item : d->m_instanceEventItemSorted) {
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
        if (!orphanInstance)
            orphanInstance = currentChart()->makeInstance();

        InstanceItem *instanceItem = InstanceItem::createDefaultItem(orphanInstance, currentChart(), pos);
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
    if (!instance)
        return;

    if (msc::InstanceItem *item = d->m_instanceItems.take(instance->internalId())) {
        d->m_instanceItemsSorted.removeOne(item);
        utils::removeSceneItem(item);
        delete item;
        updateLayout();
    }
}

void ChartViewModel::removeEventItem(MscInstanceEvent *event)
{
    if (!event)
        return;

    if (msc::InteractiveObject *item = d->m_instanceEventItems.take(event->internalId())) {
        d->m_instanceEventItemSorted.removeOne(item);
        utils::removeSceneItem(item);
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
            if (res && res->modelItem()->explicitCreator()) {
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
        if (instance)
            instanceVertiacalOffset = instance->axis().p1().y();
    }

    ActionItem *item = itemForAction(action);
    if (!item) {
        item = new ActionItem(action);
        storeEntityItem(item);
    }
    item->connectObjects(instance, d->m_layoutInfo.m_pos.y() + instanceVertiacalOffset);
    item->instantLayoutUpdate();

    return item;
}

ConditionItem *ChartViewModel::addConditionItem(MscCondition *condition, ConditionItem *prevItem, QRectF &instancesRect)
{
    auto *item = itemForCondition(condition);
    if (!item) {
        item = new ConditionItem(condition);
        storeEntityItem(item);
    }

    InstanceItem *instance = itemForInstance(condition->instance());
    if (instance) {
        qreal verticalOffset = instance->axis().p1().y();
        if (prevItem
            && (prevItem->modelItem()->instance() == condition->instance() || prevItem->modelItem()->shared())) {
            verticalOffset += prevItem->boundingRect().height() + d->InterMessageSpan;
        }
        item->connectObjects(instance, d->m_layoutInfo.m_pos.y() + verticalOffset, instancesRect);
        item->instantLayoutUpdate();
    }

    return item;
}

TimerItem *ChartViewModel::addTimerItem(MscTimer *timer)
{
    InstanceItem *instance(nullptr);
    qreal instanceVertiacalOffset(0);
    if (timer->instance()) {
        instance = itemForInstance(timer->instance());
        if (instance)
            instanceVertiacalOffset = instance->axis().p1().y();
    }

    TimerItem *item = itemForTimer(timer);
    if (!item) {
        item = new TimerItem(timer, this);
        storeEntityItem(item);
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
        if (instance)
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
        d->m_instanceEventItems.insert(coregion->internalId(), item);
        d->m_instanceEventItemSorted.append(item);
        d->m_scene.addItem(item);
    }
    if (coregion->type() == MscCoregion::Type::Begin) {
        item->setBegin(coregion);
        item->connectObjects(instance, d->m_layoutInfo.m_pos.ry() + instanceVertiacalOffset);
    } else {
        item->setEnd(coregion);
        item->instantLayoutUpdate();
    }

    return item;
}

void ChartViewModel::onInstanceItemMoved(InteractiveObject *instanceItem)
{
    MscInstance *instance = dynamic_cast<MscInstance *>(instanceItem->modelEntity());
    Q_ASSERT(instance != nullptr);

    const int currentIdx = d->m_currentChart->instances().indexOf(instance);
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
                                      { QVariant::fromValue<MscInstance *>(instance), nextIdx,
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
    if (d->m_instanceItemsSorted.isEmpty())
        return -1;

    QMap<qreal, QPair<QLineF, InstanceItem *>> instanceByDistance;
    for (InstanceItem *instanceItem : d->m_instanceItemsSorted) {
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

void ChartViewModel::storeEntityItem(InteractiveObject *item)
{
    if (!item)
        return;

    MscEntity *entity = item->modelEntity();
    if (!entity)
        return;

    const MscEntity::EntityType entityType = entity->entityType();
    switch (entityType) {
    case MscEntity::EntityType::Instance: {
        InstanceItem *instanceItem = static_cast<InstanceItem *>(item);
        d->m_instanceItems.insert(entity->internalId(), instanceItem);
        d->m_instanceItemsSorted.append(instanceItem);
        break;
    }
    default: {
        d->m_instanceEventItems.insert(entity->internalId(), item);
        d->m_instanceEventItemSorted.append(item);
        break;
    }
    }

    d->m_scene.addItem(item);
}

void ChartViewModel::connectItems()
{
    for (InteractiveObject *instanceItem : d->m_instanceItems)
        connectInstanceItem(instanceItem);
    for (InteractiveObject *instanceEventItem : d->m_instanceEventItems)
        connectInstanceEventItem(instanceEventItem);
}

void ChartViewModel::connectInstanceItem(InteractiveObject *instanceItem)
{
    connect(instanceItem, &InteractiveObject::needUpdateLayout, this, &ChartViewModel::updateLayout,
            Qt::UniqueConnection);
    connect(instanceItem, &InteractiveObject::cifChanged, this, &ChartViewModel::cifDataChanged, Qt::UniqueConnection);
    connect(instanceItem->modelEntity(), &MscEntity::commentChanged, this, &ChartViewModel::onEntityCommentChanged,
            Qt::UniqueConnection);
    connect(instanceItem, &InteractiveObject::moved, this, &ChartViewModel::onInstanceItemMoved, Qt::UniqueConnection);
}

void ChartViewModel::connectInstanceEventItem(InteractiveObject *instanceEventItem)
{
    connect(instanceEventItem, &InteractiveObject::needUpdateLayout, this, &ChartViewModel::updateLayout,
            Qt::UniqueConnection);
    connect(instanceEventItem, &InteractiveObject::cifChanged, this, &ChartViewModel::cifDataChanged,
            Qt::UniqueConnection);
    connect(instanceEventItem, &InteractiveObject::moved, this, &ChartViewModel::onInstanceEventItemMoved,
            Qt::UniqueConnection);
}

void ChartViewModel::disconnectItems()
{
    for (InteractiveObject *instanceItem : d->m_instanceItems) {
        disconnect(instanceItem, &InteractiveObject::needUpdateLayout, this, &ChartViewModel::updateLayout);
        disconnect(instanceItem, &InteractiveObject::cifChanged, this, &ChartViewModel::cifDataChanged);
        disconnect(instanceItem->modelEntity(), &MscEntity::commentChanged, this,
                   &ChartViewModel::onEntityCommentChanged);
        disconnect(instanceItem, &InteractiveObject::moved, this, &ChartViewModel::onInstanceItemMoved);
    }
    for (InteractiveObject *instanceEventItem : d->m_instanceEventItems) {
        disconnect(instanceEventItem, &InteractiveObject::needUpdateLayout, this, &ChartViewModel::updateLayout);
        disconnect(instanceEventItem, &InteractiveObject::cifChanged, this, &ChartViewModel::cifDataChanged);
        disconnect(instanceEventItem, &InteractiveObject::moved, this, &ChartViewModel::onInstanceEventItemMoved);
    }
}

void ChartViewModel::prepareChartBoxItem()
{
    if (!d->m_layoutInfo.m_chartItem) {
        d->m_layoutInfo.m_chartItem = new ChartItem(d->m_currentChart);
        connect(d->m_layoutInfo.m_chartItem, &ChartItem::cifChanged, this, [this]() { updateContentBounds(); });
        connect(d->m_layoutInfo.m_chartItem, &ChartItem::chartBoxChanged, this, &ChartViewModel::onChartBoxChanged);

        d->m_scene.addItem(d->m_layoutInfo.m_chartItem);
    }

    utils::CoordinatesConverter::init(&d->m_scene, d->m_layoutInfo.m_chartItem);
}

} // namespace msc
