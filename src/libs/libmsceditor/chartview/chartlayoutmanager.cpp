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

#include "chartlayoutmanager.h"

#include "actionitem.h"
#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/mscutils.h"
#include "baseitems/instanceheaditem.h"
#include "chartitem.h"
#include "chartverticalcheck.h"
#include "commands/cmdactionitemmove.h"
#include "commands/cmdconditionitemmove.h"
#include "commands/cmdcoregionitemmove.h"
#include "commands/cmdmessageitemresize.h"
#include "commands/cmdtimeritemmove.h"
#include "commentitem.h"
#include "conditionitem.h"
#include "coregionitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "msccommandsstack.h"
#include "msccomment.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "mscinstance.h"
#include "msctimer.h"
#include "timeritem.h"
#include "ui/graphicsscenebase.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QMap>
#include <QPointer>
#include <QTimer>
#include <QVector>
#include <algorithm>
#include <cmath>
#include <limits>

namespace msc {

/*!
   \class  ChartLayoutManager is the model containing the scene graph of the currently selected/visible
   MSC chart (showing instances, messages, ...). It is doing all the layout and reacts on changes in the chart to
   trigger a new layout.
 */

struct ChartViewLayoutInfo {
    ChartViewLayoutInfo() { }

    ~ChartViewLayoutInfo() { clear(); }

    void clear()
    {
        m_dynamicInstanceMarkers.clear();
        m_dynamicInstances.clear();
        m_pos = { 0., 0. };

        if (m_chartItem) {
            removeSceneItem(m_chartItem);
            delete m_chartItem;
            m_chartItem = nullptr;
        }

        m_instancesCommonAxisOffset = 0.;
    }

    QMap<MscInstance *, MessageItem *> m_dynamicInstances;
    QMap<MscInstance *, MessageItem *> m_dynamicInstanceMarkers;
    QSizeF m_preferredBox;
    QPointF m_pos;
    qreal m_instancesCommonAxisOffset = 0.;

    QPointer<ChartItem> m_chartItem = nullptr;

private:
};

struct ChartLayoutManagerPrivate {
    explicit ChartLayoutManagerPrivate(ChartLayoutManager *manager)
        : q(manager)
    {
    }

    ChartLayoutManager *q;
    shared::ui::GraphicsSceneBase m_scene;
    QHash<QUuid, msc::InstanceItem *> m_instanceItems;
    QVector<msc::InstanceItem *> m_instanceItemsSorted;
    QHash<QUuid, msc::InteractiveObject *> m_instanceEventItems;
    QVector<msc::InteractiveObject *> m_instanceEventItemsSorted;

    QPointer<msc::MscChart> m_currentChart = nullptr;

    QTimer m_layoutUpdateTimer;

    ChartViewLayoutInfo m_layoutInfo;
    ChartVerticalCheck m_verticalCheck;

    qreal calcInstanceAxisHeight() const
    {
        if (m_instanceItems.isEmpty()) {
            static const qreal oneMessageHeight = CoordinatesConverter::heightInScene(100);
            const int eventsCount = qMax(1, m_currentChart->totalEventNumber());
            return eventsCount * (oneMessageHeight + q->interMessageSpan());
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
    int allItemsCount() const { return m_instanceItems.size() + m_instanceEventItems.size(); }
    QVector<InteractiveObject *> allItems() const
    {
        QVector<InteractiveObject *> items;
        items.reserve(allItemsCount());

        for (InstanceItem *item : m_instanceItemsSorted)
            items << item;

        for (InteractiveObject *item : m_instanceEventItemsSorted)
            items << item;

        return items;
    }

private:
    ChartLayoutManagerPrivate() { }
};

ChartLayoutManager::ChartLayoutManager(MscCommandsStack *undoStack, QObject *parent)
    : ChartLayoutManagerBase(undoStack, parent)
    , d(new ChartLayoutManagerPrivate(this))
{
    d->m_layoutUpdateTimer.setInterval(1);
    d->m_layoutUpdateTimer.setSingleShot(true);
    connect(&d->m_layoutUpdateTimer, &QTimer::timeout, this, &msc::ChartLayoutManager::doLayout);
}

ChartLayoutManager::~ChartLayoutManager()
{
    clearScene();
}

QGraphicsScene *ChartLayoutManager::graphicsScene() const
{
    return &(d->m_scene);
}

/*!
   Sets the chart to show.
 */
void ChartLayoutManager::setCurrentChart(MscChart *chart)
{
    if (chart == d->m_currentChart) {
        return;
    }

    if (d->m_currentChart) {
        disconnect(d->m_currentChart, nullptr, this, nullptr);
    }

    d->m_currentChart = chart;

    clearScene();

    if (!d->m_currentChart.isNull()) {
        if (!d->m_scene.views().isEmpty()) {
            prepareChartBoxItem();

            const QRectF &preferredRectCif = d->m_layoutInfo.m_chartItem->storedCustomRect();
            const QRectF &preferredRectDefault = QRectF({ 0., 0. }, preferredChartBoxSize());
            const bool emptyDoc =
                    d->m_currentChart->instances().isEmpty() && d->m_currentChart->totalEventNumber() == 0;
            applyContentRect(preferredRectCif.isNull() ? preferredRectDefault : preferredRectCif);
            if (preferredRectCif.isNull() && emptyDoc) {
                QSignalBlocker suppressCifChangeNotifycation(d->m_layoutInfo.m_chartItem);
                d->m_layoutInfo.m_chartItem->updateCif();
            }
        }
        doLayout();

        connect(d->m_currentChart, &msc::MscChart::instanceAdded, this, &ChartLayoutManager::updateLayout);
        connect(d->m_currentChart, &msc::MscChart::instanceRemoved, this,
                QOverload<msc::MscInstance *>::of(&ChartLayoutManager::removeInstanceItem));
        connect(d->m_currentChart, &msc::MscChart::instanceOrderChanged, this, &ChartLayoutManager::updateLayout);

        connect(d->m_currentChart, &msc::MscChart::instanceEventAdded, this, &ChartLayoutManager::updateLayout);
        connect(d->m_currentChart, &msc::MscChart::instanceEventRemoved, this, &ChartLayoutManager::removeEventItem);
        connect(d->m_currentChart, &msc::MscChart::eventMoved, this, &ChartLayoutManager::updateLayout);
        connect(d->m_currentChart, &msc::MscChart::messageRetargeted, this, &ChartLayoutManager::updateLayout);

        connect(d->m_currentChart, &msc::MscChart::dataChanged, this, &ChartLayoutManager::updateLayout);
    }

    Q_EMIT currentChartChanged(d->m_currentChart);
}

/*!
   \brief ChartLayoutManager::currentChart returns the currently shown chart
 */
MscChart *ChartLayoutManager::currentChart() const
{
    return d->m_currentChart;
}

void ChartLayoutManager::clearScene()
{
    qDeleteAll(d->m_instanceEventItems);
    d->m_instanceEventItems.clear();
    d->m_instanceEventItemsSorted.clear();

    qDeleteAll(d->m_instanceItems);
    d->m_instanceItems.clear();
    d->m_instanceItemsSorted.clear();
    setInstancesRect(QRectF());

    d->m_layoutInfo.clear();
    setInstancesRect(QRectF());

    d->m_scene.clear();
}

MessageItem *ChartLayoutManager::fillMessageItem(
        MscMessage *message, InstanceItem *sourceItem, InstanceItem *targetItem, qreal verticalOffset)
{
    MessageItem *item = itemForMessage(message);
    if (!item) {
        item = new MessageItem(message, this, sourceItem, targetItem);
        item->setChartItem(d->m_layoutInfo.m_chartItem);
        connect(item, &MessageItem::retargeted, this, &ChartLayoutManager::onMessageRetargeted, Qt::UniqueConnection);
        connect(d->m_layoutInfo.m_chartItem, &msc::ChartItem::contentRectChanged, item,
                &msc::MessageItem::onChartBoxChanged);

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

        QVector<QPointF> points { QPointF(0, 0), QPointF(0, 0) };
        if (item->geometryManagedByCif()) {
            bool converted = false;
            const QVector<QPointF> pointsScene =
                    CoordinatesConverter::cifToScene(item->modelItem()->cifPoints(), &converted);
            if (converted && points.size() == pointsScene.size())
                points = pointsScene;
        }

        // Alings point to the X of the given instance, and pushes the Y  if needed
        auto alignPoint = [&](const QPointF &defaultPos, InstanceItem *connectedItem) {
            QPointF pnt = defaultPos;
            if (connectedItem) {
                // make sure the message start/stop aligns with the instance
                pnt.setX(connectedItem->sceneBoundingRect().center().x());
            }
            pnt.setY(std::max(pnt.y(), newY));
            return pnt;
        };

        QPointF pntSource = alignPoint(points.front(), sourceItem);
        QPointF pntTarget = alignPoint(points.back(), targetItem);

        MessageItem::GeometryNotificationBlocker geometryNotificationBlocker(item);
        item->setMessagePoints({ pntSource, pntTarget });
        item->setInstances(sourceItem, targetItem);
        item->setAutoResizable(true);
    }

    if (item)
        item->instantLayoutUpdate();

    return item;
}

/*!
   Triggers an update. The update is scheduled, so multiple calls of this function trigger only one layout update.
   \sa doLayout()
 */
void ChartLayoutManager::updateLayout()
{
    if (d->m_scene.mousePressed()) {
        return; // Don't trigger re-layouts while the user interacts with the scene
    }

    d->m_layoutUpdateTimer.start();
}

/*!
   Updates the layout. In contrast to updateLayout(), it's done right away.
   For performance reasons prefer to use updateLayout().
   \sa updateLayout()
 */
void ChartLayoutManager::doLayout()
{
    d->m_layoutUpdateTimer.stop();

    d->m_layoutInfo.m_dynamicInstanceMarkers.clear();
    d->m_layoutInfo.m_pos = { 0., 0. };
    d->m_layoutInfo.m_instancesCommonAxisOffset = 0.;

    prepareChartBoxItem();
    if (d->m_layoutInfo.m_chartItem) {
        QSignalBlocker silently(d->m_layoutInfo.m_chartItem);
        d->m_layoutInfo.m_chartItem->applyCif();
    }

    // The calls order below DOES matter
    addInstanceItems(); // which are not highlightable now to avoid flickering
    addInstanceEventItems();
    disconnectItems();
    checkHorizontalConstraints();
    checkVerticalConstraints();

    qreal lastY = eventsBottom();
    actualizeInstancesHeights(lastY + interMessageSpan());
    updateChartboxToContent();
    connectItems();

    // make instance items be highlightable on message (dis-)connection
    if (d->m_currentChart) {
        for (MscInstance *instance : d->m_currentChart->instances()) {
            if (InstanceItem *item = itemForInstance(instance)) {
                item->setHighlightable(true);
            }
        }
    }

    forceCifForAll();

    Q_EMIT layoutComplete();
}

void ChartLayoutManager::addInstanceItems()
{
    if (!d->m_currentChart) {
        return;
    }

    QRectF newInstancesRect;

    InstanceItem *lastItem = nullptr;
    qreal maxHeight = 0.0;
    for (MscInstance *instance : d->m_currentChart->instances()) {
        InstanceItem *item = itemForInstance(instance);
        if (!item) {
            item = createDefaultInstanceItem(instance);
            if (lastItem) {
                item->setPos(lastItem->sceneBoundingRect().topRight() + QPointF(interInstanceSpan(), 0.0));
            }
            storeEntityItem(item);

            for (msc::InteractiveObject *instanceEventItem : qAsConst(d->m_instanceEventItems)) {
                if (instanceEventItem->modelEntity()->entityType() == msc::MscEntity::EntityType::Condition) {
                    if (auto condition = qobject_cast<MscCondition *>(instanceEventItem->modelEntity())) {
                        if (condition->shared()) {
                            item->stackBefore(instanceEventItem);
                        }
                    }
                }
            }
        }

        const bool geomByCif = item->geometryManagedByCif();
        if (geomByCif) {
            item->applyCif();
        }

        newInstancesRect |= item->sceneBoundingRect();

        if (!instance->explicitCreator()) {
            const qreal headBottom = item->axis().y1();
            d->m_layoutInfo.m_pos.setY(std::max(d->m_layoutInfo.m_pos.y(), headBottom));
            maxHeight = qMax(maxHeight, headBottom - item->pos().y());
        }

        lastItem = item;
    }

    for (InstanceItem *instanceItem : qAsConst(d->m_instanceItems)) {
        if (instanceItem->modelItem()->explicitCreator())
            continue;

        const qreal yOffset = maxHeight - instanceItem->axis().y1();
        instanceItem->moveSilentlyBy(QPointF(0, yOffset));
        if (instanceItem->geometryManagedByCif()) {
            instanceItem->updateCif();
            d->m_layoutInfo.m_instancesCommonAxisOffset = qMax(d->m_layoutInfo.m_instancesCommonAxisOffset, yOffset);
        }
    }

    d->m_layoutInfo.m_pos.setY(qMax(maxHeight, d->m_layoutInfo.m_pos.y()));

    Q_ASSERT(d->m_currentChart->instances().size() == d->m_instanceItems.size());
    Q_ASSERT(d->m_currentChart->instances().size() == d->m_instanceItemsSorted.size());

    setInstancesRect(newInstancesRect);
}

void ChartLayoutManager::addInstanceEventItems()
{
    if (!d->m_currentChart) {
        return;
    }

    const QVector<MscInstanceEvent *> chartEvents = d->m_currentChart->instanceEvents();
    auto it = chartEvents.begin();
    for (; it != chartEvents.end(); ++it) {
        MscInstanceEvent *instanceEvent = *it;
        Q_ASSERT(instanceEvent);

        switch (instanceEvent->entityType()) {
        case MscEntity::EntityType::Action: {
            addActionItem(static_cast<MscAction *>(instanceEvent));
            break;
        }
        case MscEntity::EntityType::Condition: {
            addConditionItem(static_cast<MscCondition *>(instanceEvent));
            break;
        }
        case MscEntity::EntityType::Comment: {
            addCommentItem(static_cast<MscComment *>(instanceEvent));
            break;
        }
        case MscEntity::EntityType::Create: {
            // TODO: rm MscCreate wrapper and use the MscMessage directly?
            addMessageItem(static_cast<MscMessage *>(instanceEvent));
            break;
        }
        case MscEntity::EntityType::Coregion: {
            addCoregionItem(static_cast<MscCoregion *>(instanceEvent));
            break;
        }
        case MscEntity::EntityType::Message: {
            addMessageItem(static_cast<MscMessage *>(instanceEvent));
            break;
        }
        case MscEntity::EntityType::Timer: {
            addTimerItem(static_cast<MscTimer *>(instanceEvent));
            break;
        }
        default: {
            break;
        }
        }
    }

    Q_ASSERT(d->m_instanceEventItems.size() == d->m_instanceEventItemsSorted.size());
}

QRectF shrinkChartMargins(const QRectF &from, bool left, bool right)
{
    const QMarginsF &chartMargins = ChartItem::chartMargins();
    QMarginsF toBeRemoved;
    if (left)
        toBeRemoved.setLeft(chartMargins.left());
    if (right)
        toBeRemoved.setRight(chartMargins.right());

    if (!toBeRemoved.isNull())
        return from.marginsRemoved(toBeRemoved);

    return from;
}

/*!
   Returns the scene rectangle of all instances and events
 */
QRectF ChartLayoutManager::minimalContentRect() const
{
    constexpr qreal minWidth { 200. };
    constexpr qreal minHeight { 200. };
    auto ensureMinSize = [&](QRectF r) {
        if (r.width() < minWidth)
            r.setWidth(minWidth);
        if (r.height() < minHeight)
            r.setHeight(minHeight);
        return r;
    };

    auto effectiveRectInstances = [&ensureMinSize](const QVector<InstanceItem *> &instances) {
        QRectF totalRect, cifsRect;
        for (InstanceItem *item : instances) {
            const QRectF &currRect = item->sceneBoundingRect();
            totalRect |= currRect;
            if (item->geometryManagedByCif())
                cifsRect |= currRect;
        }

        if (cifsRect.isNull())
            return ensureMinSize(totalRect);

        return totalRect;
    };

    const QRectF &currChartBox = CoordinatesConverter::currentChartItem()->contentRect();
    auto eventEffectiveRect = [&currChartBox](InteractiveObject *item) {
        if (!item)
            return QRectF();

        QRectF currRect = item->sceneBoundingRect();
        if (!item->modelEntity() || item->modelEntity()->entityType() != MscEntity::EntityType::Message)
            return currRect;

        MscMessage *message = static_cast<MscMessage *>(item->modelEntity());
        if (MessageItem *messageItem = dynamic_cast<MessageItem *>(item)) {
            if (message->isGlobal()) {
                // TODO: add support for complex global message
                // TODO: include name rect into min bounding box
                InstanceItem *instanceItem = messageItem->sourceInstanceItem() ? messageItem->sourceInstanceItem()
                                                                               : messageItem->targetInstanceItem();
                if (instanceItem)
                    currRect = instanceItem->sceneBoundingRect().intersected(currRect);
                else
                    currRect = currChartBox.intersected(currRect);
            }
        }
        return QRectF(currRect.toRect());
    };

    auto effectiveRectEvents = [&eventEffectiveRect](const QVector<InteractiveObject *> &events) {
        QRectF totalRect;
        for (InteractiveObject *item : events) {
            if (item->modelEntity() && item->modelEntity()->entityType() == MscEntity::EntityType::Comment)
                continue;

            const QRectF &currRect = eventEffectiveRect(item);
            if (!currRect.isNull())
                totalRect |= currRect;
        }
        return totalRect;
    };

    const QRectF &events =
            effectiveRectEvents(d->m_instanceEventItemsSorted).adjusted(0, interMessageSpan(), 0, interMessageSpan());
    QRectF instances = effectiveRectInstances(d->m_instanceItemsSorted);
    if (events.isNull()) {
        instances.setHeight(minHeight);
        instances.setTopLeft(QPointF(0., 0.));
        return instances;
    }

    QRectF contentRect = instances | events;
    contentRect.setBottom(events.bottom() /*+ InstanceEndItem::EndSymbolHeight*/);
    if (!contentRect.topLeft().isNull()) {
        if (contentRect.topLeft().x() < 0)
            contentRect.moveTopLeft(QPointF(0., 0.));
        else
            contentRect.setTopLeft(QPointF(0., 0.));
    }

    return contentRect;
}

QRectF ChartLayoutManager::actualContentRect() const
{
    QRectF r;
    bool globalToLeft(false), globalToRight(false);
    for (InteractiveObject *io : d->allItems()) {
        r |= io->sceneBoundingRect();
        if (!(globalToLeft || globalToRight) && !d->m_instanceEventItemsSorted.isEmpty()) {
            if (io->modelEntity() && io->modelEntity()->entityType() == MscEntity::EntityType::Message) {
                if (MessageItem *messageItem = qobject_cast<MessageItem *>(io)) {
                    if (messageItem->modelItem()->isGlobal()) {
                        globalToLeft = true;
                        globalToRight = true;
                    }
                }
            }
        }
    }

    r = shrinkChartMargins(r, globalToLeft, globalToRight);

    return r;
}

/**
   Returns all the instance items
 */
const QVector<InstanceItem *> &ChartLayoutManager::instanceItems() const
{
    return d->m_instanceItemsSorted;
}

/*!
   Returns all instance event items
   \note The sorting is not necessarily in the correct (vertical) order
 */
const QVector<InteractiveObject *> &ChartLayoutManager::allEventItems() const
{
    return d->m_instanceEventItemsSorted;
}

/*!
   Returns all action items belonging to the given instance
 */
const QVector<ActionItem *> ChartLayoutManager::actionsOfInstance(const msc::MscInstance *instance) const
{
    QVector<ActionItem *> actions;
    for (msc::InteractiveObject *event : qAsConst(d->m_instanceEventItems)) {
        if (auto actionItem = qobject_cast<msc::ActionItem *>(event)) {
            if (actionItem->modelItem()->instance() == instance) {
                actions.append(actionItem);
            }
        }
    }
    return actions;
}

/*!
   Returns all condition items belonging to the given instance.
   \note This includes shared conditions
 */
const QVector<ConditionItem *> ChartLayoutManager::conditionsOfInstance(const MscInstance *instance) const
{
    QVector<ConditionItem *> conditions;
    for (msc::InteractiveObject *event : qAsConst(d->m_instanceEventItems)) {
        if (auto conditionItem = qobject_cast<msc::ConditionItem *>(event)) {
            if (conditionItem->modelItem()->instance() == instance || conditionItem->modelItem()->shared()) {
                conditions.append(conditionItem);
            }
        }
    }
    return conditions;
}

/*!
   Returns all timer items belonging to the given instance
 */
const QVector<TimerItem *> ChartLayoutManager::timersOfInstance(const MscInstance *instance) const
{
    QVector<TimerItem *> timers;
    for (msc::InteractiveObject *event : qAsConst(d->m_instanceEventItems)) {
        if (auto timerItem = qobject_cast<msc::TimerItem *>(event)) {
            if (timerItem->modelItem()->instance() == instance) {
                timers.append(timerItem);
            }
        }
    }
    return timers;
}

QLineF ChartLayoutManager::commonAxis() const
{
    qreal commonAxisStart { 0 }, commonAxisStop { 0 };

    for (InstanceItem *gi : qAsConst(d->m_instanceItems)) {
        if (!gi->modelItem()->explicitCreator()) {
            commonAxisStart = qMax(commonAxisStart, gi->axis().p1().y());
        }
        if (!gi->modelItem()->explicitStop()) {
            commonAxisStop = qMax(commonAxisStop, gi->axis().p2().y());
        }
    }

    return QLineF(0., commonAxisStart, 0., commonAxisStop);
}

void ChartLayoutManager::updateChartboxToContent()
{
    if (!d->m_layoutInfo.m_chartItem)
        return;

    QRectF chartBox = actualContentRect();
    const QRectF &minRect = minimalContentRect();

    if (chartBox.isEmpty())
        chartBox = minRect;
    else
        chartBox.moveTopLeft(minRect.topLeft());

    auto extendRectTo = [](QRectF &rect, const QRectF &to) {
        if (rect.width() < to.width())
            rect.setWidth(to.width());
        if (rect.height() < to.height())
            rect.setHeight(to.height());
    };

    extendRectTo(chartBox, minRect);
    extendRectTo(chartBox, d->m_layoutInfo.m_chartItem->storedCustomRect());

    if (!chartBox.topLeft().isNull())
        chartBox.moveTopLeft({});
    const int totalItemsCount = d->allItemsCount();
    d->m_layoutInfo.m_chartItem->setZValue(-totalItemsCount);

    applyContentRect(chartBox);
    if (chartBox == d->m_layoutInfo.m_chartItem->storedCustomRect())
        d->m_currentChart->clearCifs();

    for (InstanceItem *instanceItem : d->m_instanceItemsSorted) {
        instanceItem->syncHeightToChartBox();
    }
}

void ChartLayoutManager::updateContentToChartbox(const QRectF &chartBoxRect)
{
    if (!d->m_layoutInfo.m_chartItem)
        return;

    QRectF chartBox = chartBoxRect;
    const QRectF &minRect = minimalContentRect();

    if (chartBox.isEmpty())
        chartBox = minRect;
    else
        chartBox.moveTopLeft(minRect.topLeft());

    if (chartBox.width() < minRect.width())
        chartBox.setWidth(minRect.width());
    if (chartBox.height() < minRect.height())
        chartBox.setHeight(minRect.height());

    const int totalItemsCount = d->allItemsCount();
    d->m_layoutInfo.m_chartItem->setZValue(-totalItemsCount);

    applyContentRect(chartBox);

    if (chartBox == actualContentRect())
        d->m_currentChart->clearCifs();
}

/*!
   Checks that no item is left of the chart box. And no items overlap horizontally.
 */
void ChartLayoutManager::checkHorizontalConstraints()
{
    if (!d->m_currentChart) {
        return;
    }

    qreal leftXLimit = 0.0;
    for (MscInstance *instance : d->m_currentChart->instances()) {

        InstanceItem *instanceItem = itemForInstance(instance);
        Q_ASSERT(instanceItem != nullptr);

        syncItemsPosToInstance(instanceItem);

        QRectF rect = instanceItem->sceneBoundingRect();
        // get width of instance + relevant events
        for (msc::ActionItem *actionItem : actionsOfInstance(instanceItem->modelItem())) {
            rect = rect.united(actionItem->sceneBoundingRect());
        }
        for (msc::ConditionItem *conditionItem : conditionsOfInstance(instanceItem->modelItem())) {
            if (!conditionItem->modelItem()->shared()) {
                rect = rect.united(conditionItem->sceneBoundingRect());
            }
        }
        for (msc::TimerItem *timerItem : timersOfInstance(instanceItem->modelItem())) {
            rect = rect.united(timerItem->sceneBoundingRect());
        }

        // check if left overlaps with last right limit
        const qreal offset = leftXLimit - rect.x();
        if (offset > 0.0) {
            instanceItem->moveBy(offset, 0.0);
            instanceItem->updateCif();
            rect.translate(offset, 0.0);
            syncItemsPosToInstance(instanceItem);
        }

        leftXLimit = rect.right() + 1.0;
    }
}

void ChartLayoutManager::checkVerticalConstraints()
{
    if (!d->m_currentChart) {
        return;
    }

    d->m_verticalCheck.reset(this, d->m_currentChart);
    d->m_verticalCheck.checkVerticalConstraints();
}

void ChartLayoutManager::actualizeInstancesHeights(qreal height) const
{
    for (InstanceItem *instanceItem : qAsConst(d->m_instanceItems)) {
        bool updated(false);
        if (instanceItem->modelItem()->explicitStop()) {
            updateStoppedInstanceHeight(instanceItem, height);
            updated = true;
        }
        if (instanceItem->modelItem()->explicitCreator()) {
            updateCreatedInstanceHeight(instanceItem, height);
            updated = true;
        }
        if (!updated)
            instanceItem->setAxisHeight(height);
    }
}

void ChartLayoutManager::updateStoppedInstanceHeight(InstanceItem *instanceItem, qreal totalH) const
{
    // update instance's end Y-position to the last message
    QVector<InteractiveObject *> events(instanceEventItems(instanceItem->modelItem()));
    if (!events.isEmpty()) {
        qreal bottomY = 0.0;
        for (InteractiveObject *item : events) {
            bottomY = std::max(bottomY, item->sceneBoundingRect().bottom());
        }

        QLineF axisLine(instanceItem->axis());
        axisLine.setP2({ axisLine.x2(), bottomY });
        instanceItem->setAxisHeight(axisLine.length());
    } else {
        updateCreatedInstanceHeight(instanceItem, totalH);
    }
}

void ChartLayoutManager::updateCreatedInstanceHeight(InstanceItem *instanceItem, qreal totalH) const
{
    MessageItem *createItem = creatorItemOfInstance(instanceItem->modelItem());
    if (!createItem) {
        return;
    }
    qreal currentTargetY = instanceItem->leftCreatorTarget().y();
    const qreal deltaY = createItem->head().y() - currentTargetY;
    instanceItem->moveSilentlyBy(QPointF(0.0, deltaY));
    const qreal height =
            std::max(totalH - instanceItem->headerItem()->sceneBoundingRect().bottom(), interMessageSpan());
    instanceItem->setAxisHeight(height);
}

InstanceItem *ChartLayoutManager::itemForInstance(msc::MscInstance *instance) const
{
    return instance ? d->m_instanceItems.value(instance->internalId()) : nullptr;
}

template<class ItemType>
ItemType *ChartLayoutManager::itemForInstanceEvent(MscInstanceEvent *event) const
{
    return event ? static_cast<ItemType *>(d->m_instanceEventItems.value(event->internalId())) : nullptr;
}

MessageItem *ChartLayoutManager::itemForMessage(MscMessage *message) const
{
    return itemForInstanceEvent<MessageItem>(message);
}

ConditionItem *ChartLayoutManager::itemForCondition(MscCondition *condition) const
{
    return itemForInstanceEvent<ConditionItem>(condition);
}

ActionItem *ChartLayoutManager::itemForAction(MscAction *action) const
{
    return itemForInstanceEvent<ActionItem>(action);
}

TimerItem *ChartLayoutManager::itemForTimer(MscTimer *timer) const
{
    return itemForInstanceEvent<TimerItem>(timer);
}

CoregionItem *ChartLayoutManager::itemForCoregion(MscCoregion *coregion) const
{
    if (!coregion)
        return nullptr;

    return qobject_cast<CoregionItem *>(d->m_instanceEventItems.value(coregion->internalId()));
}

InteractiveObject *ChartLayoutManager::itemForEntity(MscEntity *entity) const
{
    if (!entity)
        return nullptr;

    const bool isInstance = entity->entityType() == MscEntity::EntityType::Instance;
    return isInstance ? d->m_instanceItems.value(entity->internalId())
                      : d->m_instanceEventItems.value(entity->internalId());
}

/*!
   \brief ChartLayoutManager::nearestEntity
   Returns the entity that is close enough to be used by an event.
   \param pos
 */
MscEntity *ChartLayoutManager::nearestEntity(const QPointF &pos)
{
    static const qreal add_space = 10;
    static const QMarginsF extra_margin = QMarginsF(add_space, add_space, add_space, add_space);

    qreal distance = std::numeric_limits<int>::max();
    auto getNearest = [](qreal &distance, InteractiveObject *iObj, const QPointF &point) -> MscEntity * {
        MscEntity *entity = nullptr;
        const QRectF itemRect = iObj->sceneBoundingRect().marginsAdded(extra_margin);
        if (!itemRect.contains(point) || !iObj->modelEntity())
            return nullptr;

        qreal dist = std::abs(itemRect.center().x() - point.x());
        if (dist < distance) {
            distance = dist;
            entity = iObj->modelEntity();
        }
        return entity;
    };

    MscEntity *entity = nullptr;
    for (msc::InteractiveObject *item : d->m_instanceEventItemsSorted) {
        if ((entity = getNearest(distance, item, pos)))
            return entity;
    }
    for (msc::InteractiveObject *item : d->m_instanceItemsSorted) {
        if ((entity = getNearest(distance, item, pos)))
            return entity;
    }

    return nullptr;
}

/*!
   \brief ChartLayoutManager::nearestInstance
   Returns the instance that is close enough to be used by an event.
   \param pos
 */
MscInstance *ChartLayoutManager::nearestInstance(const QPointF &pos)
{
    static const qreal add_space = 30;
    static const QMarginsF extra_margin = QMarginsF(add_space, add_space, add_space, add_space);

    qreal distance = std::numeric_limits<int>::max();
    MscInstance *instance = nullptr;
    for (msc::InstanceItem *item : qAsConst(d->m_instanceItems)) {
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

/*!
   Returns the index an event would have if it was placed at Y-position \p y.
   @param y the Y-position in scene coordinates
 */
int ChartLayoutManager::eventIndex(const QPointF &pt, MscInstanceEvent *ignoreEvent)
{
    int idx = 0;
    for (msc::InteractiveObject *item : d->m_instanceEventItemsSorted) {
        if (item->modelEntity() == ignoreEvent) {
            continue;
        }
        auto event = dynamic_cast<MscInstanceEvent *>(item->modelEntity());
        if (!event) {
            continue;
        }
        auto messageItem = qobject_cast<MessageItem *>(item);
        if (messageItem) {
            const QPointF tailPos = messageItem->tail();
            const QPointF headPos = messageItem->head();
            const qreal tailDist = std::abs(tailPos.x() - pt.x());
            const qreal headDist = std::abs(headPos.x() - pt.x());
            if (headDist < tailDist) {
                if (headPos.y() < pt.y()) {
                    ++idx;
                }
            } else {
                if (tailPos.y() < pt.y()) {
                    ++idx;
                }
            }
        } else {
            const QRectF sceneBBox = item->sceneBoundingRect();
            if (sceneBBox.y() < pt.y()) {
                ++idx;
                if (auto coregionItem = qobject_cast<CoregionItem *>(item)) {
                    if (sceneBBox.bottom() < pt.y()) {
                        ++idx;
                    }
                }
            }
        }
    }
    return idx;
}

/*!
   Returns the index of event for the given \p instance for the given position \pt.
   It checks the existing the event items along the
 */
int ChartLayoutManager::eventInstanceIndex(
        const QPointF &pt, MscInstance *instance, MscInstanceEvent *ignoreEvent) const
{
    int idx = 0;
    for (msc::InteractiveObject *item : qAsConst(d->m_instanceEventItemsSorted)) {
        auto eventItem = qobject_cast<msc::EventItem *>(item);
        if (!eventItem) {
            continue;
        }
        if (!eventItem->eventEntity()->relatesTo(instance)) {
            continue;
        }
        if (eventItem->modelEntity() == ignoreEvent) {
            continue;
        }

        if (eventItem->eventEntity()->entityType() == MscEntity::EntityType::Coregion) {
            const qreal topY = eventItem->instanceTopArea(instance);
            if (topY < pt.y()) {
                ++idx;
            }
        }
        const qreal bottomY = eventItem->instanceBottomArea(instance);
        if (bottomY < pt.y()) {
            ++idx;
        }
    }

    return idx;
}

MscInstanceEvent *ChartLayoutManager::eventAtPosition(const QPointF &scenePos) const
{
    for (auto eventItem : qAsConst(d->m_instanceEventItems)) {
        if (eventItem->sceneBoundingRect().contains(scenePos))
            return qobject_cast<MscInstanceEvent *>(eventItem->modelEntity());
    }
    return nullptr;
}

InstanceItem *ChartLayoutManager::createDefaultInstanceItem(MscInstance *orphanInstance, const QPointF &pos)
{
    if (currentChart()) {
        if (!orphanInstance)
            orphanInstance = currentChart()->makeInstance();

        InstanceItem *instanceItem = InstanceItem::createDefaultItem(this, orphanInstance, currentChart(), pos);
        if (!instanceItem->geometryManagedByCif()) {
            const qreal axisHeight = d->calcInstanceAxisHeight();
            if (!qFuzzyIsNull(axisHeight))
                instanceItem->setAxisHeight(axisHeight);
        }
        connect(d->m_layoutInfo.m_chartItem, &msc::ChartItem::contentRectChanged, instanceItem,
                &msc::InstanceItem::syncHeightToChartBox);
        connect(instanceItem, &InstanceItem::relocated, this, [this]() {
            if (auto instanceI = qobject_cast<InstanceItem *>(sender())) {
                syncItemsPosToInstance(instanceI);
            }
        });
        connect(instanceItem, &InstanceItem::initialNameAccepted, this, &ChartLayoutManager::initialNameAccepted,
                Qt::UniqueConnection);

        return instanceItem;
    }
    return nullptr;
}

bool ChartLayoutManager::removeInstanceItem(msc::InstanceItem *item)
{
    if (item && removeSceneItem(item)) {
        delete item;
        return true;
    }

    return false;
}

msc::MessageItem *ChartLayoutManager::createDefaultMessageItem(msc::MscMessage *orphanMessage, const QPointF &pos)
{
    if (currentChart() && orphanMessage != nullptr) {
        return MessageItem::createDefaultItem(orphanMessage, this, pos);
    }
    return nullptr;
}

bool ChartLayoutManager::removeMessageItem(msc::MessageItem *item)
{
    if (item && removeSceneItem(item)) {
        delete item;
        return true;
    }

    return false;
}

ChartItem *ChartLayoutManager::itemForChart() const
{
    return d->m_layoutInfo.m_chartItem;
}

CommentItem *ChartLayoutManager::itemForComment(MscComment *comment) const
{
    return itemForInstanceEvent<CommentItem>(comment);
}

void ChartLayoutManager::removeInstanceItem(MscInstance *instance)
{
    if (!instance)
        return;

    if (msc::InstanceItem *item = d->m_instanceItems.take(instance->internalId())) {
        d->m_instanceItemsSorted.removeOne(item);
        removeSceneItem(item);
        delete item;

        clearScene();
        updateLayout();
    }
}

void ChartLayoutManager::removeEventItem(MscInstanceEvent *event)
{
    if (!event)
        return;

    if (msc::InteractiveObject *item = d->m_instanceEventItems.take(event->internalId())) {
        d->m_instanceEventItemsSorted.removeOne(item);
        removeSceneItem(item);
        delete item;
        updateLayout();
    }
}

void ChartLayoutManager::ensureInstanceCreationAdded(MscMessage *msgCreate, MscInstance *dynamicInstance)
{
    if (!d->m_layoutInfo.m_dynamicInstances.contains(dynamicInstance)) {
        MscInstance *creatorInstance = dynamicInstance->explicitCreator();
        if (MessageItem *item = fillMessageItem(
                    msgCreate, itemForInstance(creatorInstance), itemForInstance(dynamicInstance), 0.)) {
            d->m_layoutInfo.m_dynamicInstances.insert(dynamicInstance, item);
        }
    }

    if (d->m_layoutInfo.m_dynamicInstances.contains(dynamicInstance)
            && !d->m_layoutInfo.m_dynamicInstanceMarkers.contains(dynamicInstance)) {
        if (MessageItem *item = d->m_layoutInfo.m_dynamicInstances.value(dynamicInstance)) {
            d->m_layoutInfo.m_dynamicInstanceMarkers.insert(dynamicInstance, item);
        }
    }
}

CommentItem *ChartLayoutManager::addCommentItem(MscComment *comment)
{
    CommentItem *item = itemForComment(comment);
    if (comment->text().isEmpty()) {
        if (item) {
            item->setVisible(false);
        }
    } else {
        if (!item) {
            MscEntity *entity = comment->attachedEntity();
            if (!entity)
                return nullptr;

            item = new CommentItem(d->m_currentChart, this);
            auto iObj = entity->entityType() == MscEntity::EntityType::Chart ? d->m_layoutInfo.m_chartItem
                                                                             : itemForEntity(entity);
            item->attachTo(iObj);
            if (iObj) {
                connect(iObj, &InteractiveObject::needUpdateLayout, this, &ChartLayoutManager::updateLayout,
                        Qt::UniqueConnection);
            }
            storeEntityItem(item);
        }
        item->setText(comment->text());
        item->setVisible(true);
        item->instantLayoutUpdate();
    }

    return item;
};

MessageItem *ChartLayoutManager::addMessageItem(MscMessage *message)
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

    InstanceItem *sourceInstance = findInstanceItem(message->sourceInstance());
    InstanceItem *targetInstance = findInstanceItem(message->targetInstance());

    return fillMessageItem(message, sourceInstance, targetInstance, instanceVertiacalOffset);
}

ActionItem *ChartLayoutManager::addActionItem(MscAction *action)
{
    ActionItem *item = itemForAction(action);
    if (!item) {
        item = new ActionItem(action, this);
        storeEntityItem(item);
        if (item->geometryManagedByCif()) {
            item->applyCif();
            InstanceItem *instItem = itemForInstance(action->instance());
            if (instItem) {
                syncItemsPosToInstance(instItem);
            }
        }
        item->instantLayoutUpdate();
    }

    return item;
}

ConditionItem *ChartLayoutManager::addConditionItem(MscCondition *condition)
{
    auto *item = itemForCondition(condition);
    if (!item) {
        item = new ConditionItem(condition, this);
        connect(this, &msc::ChartLayoutManagerBase::instancesRectChanged, item, &msc::ConditionItem::setInstancesRect);
        storeEntityItem(item);
        if (item->geometryManagedByCif()) {
            item->applyCif();
            InstanceItem *instItem = itemForInstance(condition->instance());
            if (instItem) {
                syncItemsPosToInstance(instItem);
            }
        }
        item->setInstancesRect(instancesRect());
        item->instantLayoutUpdate();
    }

    return item;
}

TimerItem *ChartLayoutManager::addTimerItem(MscTimer *timer)
{
    TimerItem *item = itemForTimer(timer);
    if (!item) {
        item = new TimerItem(timer, this);
        storeEntityItem(item);
        if (item->geometryManagedByCif()) {
            item->applyCif();
            InstanceItem *instItem = itemForInstance(timer->instance());
            if (instItem) {
                syncItemsPosToInstance(instItem);
            }
        }
        item->instantLayoutUpdate();
    }

    return item;
}

CoregionItem *ChartLayoutManager::addCoregionItem(MscCoregion *coregion)
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
        MscCoregion *begin = currentChart()->lastBeginCoregion(coregion->instance());
        item = itemForCoregion(begin);
    }

    if (!item) {
        item = new CoregionItem(this);
        d->m_instanceEventItems.insert(coregion->internalId(), item);
        d->m_instanceEventItemsSorted.append(item);
        d->m_scene.addItem(item);
    }
    if (coregion->type() == MscCoregion::Type::Begin) {
        item->setBegin(coregion);
        item->setY(d->m_layoutInfo.m_pos.y() + instanceVertiacalOffset);
        item->setInstance(instance);
    } else {
        item->setEnd(coregion);
        item->instantLayoutUpdate();
    }

    return item;
}

/*!
   Returns the Message item that creates the given instance.
   If no create message was found a nullptr is returned
 */
MessageItem *ChartLayoutManager::creatorItemOfInstance(MscInstance *instance) const
{
    if (!instance->explicitCreator()) {
        return nullptr;
    }
    for (InteractiveObject *event : d->m_instanceEventItemsSorted) {
        if (event->modelEntity() && event->modelEntity()->entityType() == msc::MscEntity::EntityType::Create) {
            auto createItem = static_cast<msc::MessageItem *>(event);
            if (createItem->modelItem()->targetInstance() == instance) {
                return createItem;
            }
        }
    }
    return nullptr;
}

QVariantList ChartLayoutManager::prepareChangeOrderCommand(MscInstance *instance) const
{
    Q_ASSERT(instance != nullptr);

    const int currentIdx = d->m_currentChart->instances().indexOf(instance);
    Q_ASSERT(currentIdx >= 0);

    InstanceItem *instanceItem = itemForInstance(instance);
    Q_ASSERT(instanceItem != nullptr);

    int nextIdx = 0;
    for (InstanceItem *inst : d->m_instanceItemsSorted) {
        if (inst != instanceItem) {
            if (instanceItem->x() > inst->x()) {
                ++nextIdx;
            }
        }
    }

    if (currentIdx != nextIdx)
        return { QVariant::fromValue<MscInstance *>(instance), nextIdx,
            QVariant::fromValue<MscChart *>(d->m_currentChart.data()) };

    return {};
}

void ChartLayoutManager::onInstanceGeometryChanged()
{
    if (MscInstance *instance = qobject_cast<MscInstance *>(sender())) {
        if (InstanceItem *instanceItem = itemForInstance(instance)) {
            Q_ASSERT(instanceItem != nullptr);

            const int currentIdx = d->m_currentChart->instances().indexOf(instance);
            Q_ASSERT(currentIdx >= 0);
            int nextIdx = 0;
            for (InstanceItem *inst : d->m_instanceItemsSorted) {
                if (inst != instanceItem && instanceItem->x() > inst->x())
                    ++nextIdx;
            }

            if (currentIdx != nextIdx)
                d->m_currentChart->updateInstanceOrder(instance, nextIdx);
            else
                updateLayout();

            const QRectF newGeom = instanceItem->sceneBoundingRect();
            QPointF shiftToPositives;
            if (newGeom.topLeft().x() < 0)
                shiftToPositives.rx() = -newGeom.topLeft().x();
            if (newGeom.topLeft().y() < 0)
                shiftToPositives.ry() = -newGeom.topLeft().y();

            if (!shiftToPositives.isNull()) {
                for (InstanceItem *item : d->m_instanceItemsSorted) {
                    item->moveBy(shiftToPositives.x(), shiftToPositives.y());
                    if (item->geometryManagedByCif())
                        item->updateCif();
                }
            }
        }
    }
}

void ChartLayoutManager::onInstanceEventItemMoved(shared::ui::InteractiveObjectBase *item)
{
    updateLayout();

    if (auto activeItem = qobject_cast<InteractiveObject *>(item)) {
        activeItem->updateCif();
    }

    if (auto actionItem = qobject_cast<ActionItem *>(item)) {
        MscAction *action = actionItem->modelItem();
        MscInstance *newInstance = nearestInstance(actionItem->sceneBoundingRect().center());
        const int newIdx = eventInstanceIndex(item->pos(), newInstance, action);
        ChartIndex newChartIdx(newInstance, newIdx);
        undoStack()->push(new cmd::CmdActionItemMove(actionItem->modelItem(), newChartIdx, currentChart()));
        return;
    }

    if (auto conditionItem = qobject_cast<ConditionItem *>(item)) {
        MscCondition *condition = conditionItem->modelItem();
        MscInstance *newInstance = nearestInstance(conditionItem->sceneBoundingRect().center());
        if (newInstance == nullptr && conditionItem->modelItem()->shared()) {
            newInstance = conditionItem->modelItem()->instance();
        }
        const int newIdx = eventInstanceIndex(item->pos(), newInstance, condition);
        ChartIndex newChartIdx(newInstance, newIdx);
        undoStack()->push(new cmd::CmdConditionItemMove(conditionItem->modelItem(), newChartIdx, currentChart()));
        return;
    }

    if (auto timerItem = qobject_cast<TimerItem *>(item)) {
        const QRectF itemRect = timerItem->sceneBoundingRect();
        MscTimer *timer = timerItem->modelItem();
        MscInstance *newInstance = nearestInstance(QPointF(itemRect.left(), itemRect.center().y()));
        const int newIdx = eventInstanceIndex(item->pos(), newInstance, timer);
        ChartIndex newChartIdx(newInstance, newIdx);
        undoStack()->push(new cmd::CmdTimerItemMove(timerItem->modelItem(), newChartIdx, currentChart()));
        return;
    }

    if (auto coregionItem = qobject_cast<CoregionItem *>(item)) {
        MscInstance *newInstance = nearestInstance(coregionItem->sceneBoundingRect().center());
        MscCoregion *begin = coregionItem->begin();
        MscCoregion *end = coregionItem->end();
        const int newIdxBegin = eventInstanceIndex(item->sceneBoundingRect().topLeft(), newInstance, begin);
        const int newIdxEnd = eventInstanceIndex(item->sceneBoundingRect().bottomLeft(), newInstance, end);
        undoStack()->push(
                new cmd::CmdCoRegionItemMove(begin, end, newIdxBegin, newIdxEnd, newInstance, currentChart()));
        return;
    }
}

void ChartLayoutManager::onMessageRetargeted(MessageItem *item, const QPointF &pos, MscMessage::EndType endType)
{
    Q_ASSERT(item);
    MscMessage *message = item->modelItem();
    Q_ASSERT(message);

    MscInstance *newInstance = nearestInstance(pos);
    MscInstance *currentInstance = nullptr;
    MscInstance *otherInstance = nullptr;
    if (endType == MscMessage::EndType::SOURCE_TAIL) {
        currentInstance = message->sourceInstance();
        otherInstance = message->targetInstance();
    } else {
        currentInstance = message->targetInstance();
        otherInstance = message->sourceInstance();
    }
    const int currentIdx = d->m_currentChart->indexofEventAtInstance(message, currentInstance);
    const int newIdx = eventInstanceIndex(pos, newInstance, message);
    if (((newInstance != currentInstance && newInstance != otherInstance) || newIdx != currentIdx)
            && (newInstance || otherInstance)) {

        if (isHorizontal(item->messagePoints())) {
            if (item->geometryManagedByCif())
                item->modelItem()->clearCifs();
        } else {
            item->updateCif();
        }
        undoStack()->push(
                new cmd::CmdMessageItemResize(message, ChartIndex(newInstance, newIdx), endType, d->m_currentChart));
    } else {
        if (item->geometryManagedByCif())
            item->applyCif();

        item->instantLayoutUpdate();
        updateLayout();
    }
}

QSizeF ChartLayoutManager::preferredChartBoxSize() const
{
    return d->m_layoutInfo.m_preferredBox;
}

void ChartLayoutManager::setPreferredChartBoxSize(const QSizeF &size)
{
    d->m_layoutInfo.m_preferredBox = size;
}

void ChartLayoutManager::storeEntityItem(InteractiveObject *item)
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
        d->m_instanceEventItemsSorted.append(item);
        break;
    }
    }

    d->m_scene.addItem(item);
}

void ChartLayoutManager::connectItems()
{
    for (InteractiveObject *instanceItem : qAsConst(d->m_instanceItems))
        connectInstanceItem(instanceItem);
    for (InteractiveObject *instanceEventItem : qAsConst(d->m_instanceEventItems))
        connectInstanceEventItem(instanceEventItem);
}

void ChartLayoutManager::connectInstanceItem(InteractiveObject *instanceItem)
{
    connect(instanceItem, &InteractiveObject::needUpdateLayout, this, &ChartLayoutManager::updateLayout,
            Qt::UniqueConnection);
    connect(instanceItem, &InteractiveObject::cifChanged, this, &ChartLayoutManager::cifDataChanged,
            Qt::UniqueConnection);

    if (MscInstance *instance = qobject_cast<MscInstance *>(instanceItem->modelEntity())) {
        connect(instance, &MscInstance::cifGeometryChanged, this, &ChartLayoutManager::onInstanceGeometryChanged,
                Qt::UniqueConnection);
        connect(instance, &MscInstance::explicitCreatorChanged, this, &ChartLayoutManager::onInstanceCreatorChanged,
                Qt::UniqueConnection);
    }
}

void ChartLayoutManager::connectInstanceEventItem(InteractiveObject *instanceEventItem)
{
    connect(instanceEventItem, &InteractiveObject::needUpdateLayout, this, &ChartLayoutManager::updateLayout,
            Qt::UniqueConnection);
    connect(instanceEventItem, &InteractiveObject::cifChanged, this, &ChartLayoutManager::cifDataChanged,
            Qt::UniqueConnection);
    connect(instanceEventItem, &InteractiveObject::moved, this, &ChartLayoutManager::onInstanceEventItemMoved,
            Qt::UniqueConnection);
}

void ChartLayoutManager::disconnectItems()
{
    for (InteractiveObject *instanceItem : qAsConst(d->m_instanceItems)) {
        disconnect(instanceItem, &InteractiveObject::needUpdateLayout, this, &ChartLayoutManager::updateLayout);
        disconnect(instanceItem, &InteractiveObject::cifChanged, this, &ChartLayoutManager::cifDataChanged);
        if (MscInstance *instance = qobject_cast<MscInstance *>(instanceItem->modelEntity())) {
            disconnect(
                    instance, &MscInstance::cifGeometryChanged, this, &ChartLayoutManager::onInstanceGeometryChanged);
            disconnect(instance, &MscInstance::explicitCreatorChanged, this,
                    &ChartLayoutManager::onInstanceCreatorChanged);
        }
    }
    for (InteractiveObject *instanceEventItem : qAsConst(d->m_instanceEventItems)) {
        disconnect(instanceEventItem, &InteractiveObject::needUpdateLayout, this, &ChartLayoutManager::updateLayout);
        disconnect(instanceEventItem, &InteractiveObject::cifChanged, this, &ChartLayoutManager::cifDataChanged);
        disconnect(instanceEventItem, &InteractiveObject::moved, this, &ChartLayoutManager::onInstanceEventItemMoved);
    }
}

void ChartLayoutManager::prepareChartBoxItem()
{
    if (!d->m_layoutInfo.m_chartItem) {
        d->m_layoutInfo.m_chartItem = new ChartItem(d->m_currentChart, this);
        connect(d->m_layoutInfo.m_chartItem, &ChartItem::contentRectChanged, this,
                &ChartLayoutManager::updateContentToChartbox);
        connect(d->m_layoutInfo.m_chartItem, &InteractiveObject::cifChanged, this, &ChartLayoutManager::cifDataChanged);

        d->m_scene.addItem(d->m_layoutInfo.m_chartItem);
    }

    CoordinatesConverter::init(&d->m_scene, d->m_layoutInfo.m_chartItem);
}

void ChartLayoutManager::applyContentRect(const QRectF &newRect)
{
    d->m_layoutInfo.m_chartItem->setContentRect(newRect);
}

void ChartLayoutManager::onInstanceCreatorChanged(MscInstance *newCreator)
{
    if (!newCreator)
        if (MscInstance *created = qobject_cast<MscInstance *>(sender())) {
            if (InstanceItem *item = itemForInstance(created)) {
                item->moveSilentlyBy({ 0., -item->y() });
            }
        }
    updateLayout();
}

void ChartLayoutManager::forceCifForAll()
{
    for (InteractiveObject *item : d->allItems()) {
        if (!item->geometryManagedByCif()) {
            QSignalBlocker suppressItemCifAdded(item->modelEntity());
            item->updateCif();
        }
    }

    QSignalBlocker silently(d->m_layoutInfo.m_chartItem);
    d->m_layoutInfo.m_chartItem->updateCif();
}

void insertEntity(QMap<int, msc::MscInstanceEvent *> &events, int key, msc::MscInstanceEvent *value,
        const QVector<MscInstanceEvent *> &originalEvents)
{
    if (events.contains(key)) {
        // if on same position, preserve sorting of the existing
        msc::MscInstanceEvent *oldValue = events[key];
        if (originalEvents.indexOf(value) > originalEvents.indexOf(oldValue)) {
            insertEntity(events, ++key, value, originalEvents); // insert afer oldvalue
        } else {
            events[key] = value;
            insertEntity(events, ++key, oldValue, originalEvents); // move old value
        }
    } else {
        events[key] = value;
    }
}

/*!
   Returns if a layout update is still pending
 */
bool ChartLayoutManager::layoutUpdatePending() const
{
    return d->m_layoutUpdateTimer.isActive();
}

InteractiveObject *ChartLayoutManager::eventItem(const QUuid &id)
{
    return d->m_instanceEventItems.value(id);
}

/*!
  Returns the msc events that are to be shown.
  This depends on the visible item limit.
 */
QVector<MscInstanceEvent *> ChartLayoutManager::visibleEvents() const
{
    if (!d->m_currentChart) {
        return {};
    }

    return d->m_currentChart->chronologicalEvents();
}

qreal ChartLayoutManager::eventsBottom() const
{
    qreal bottom = 0;
    for (msc::InteractiveObject *eventItem : d->m_instanceEventItemsSorted) {
        bottom = std::max(eventItem->sceneBoundingRect().y(), bottom);
    }
    return bottom;
}

} // namespace msc
