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

#pragma once

#include "mscmessage.h"

#include <QObject>
#include <QPointF>
#include <QSizeF>
#include <memory>

class QGraphicsScene;
class QGraphicsObject;

namespace shared {
namespace ui {
class InteractiveObjectBase;
}
}

namespace msc {

class ActionItem;
class ChartItem;
class CommentItem;
class ConditionItem;
class CoregionItem;
class InstanceItem;
class InteractiveObject;
class MessageItem;
class SystemChecks;
class TimerItem;

class MscAction;
class MscChart;
class MscCommandsStack;
class MscCondition;
class MscCoregion;
class MscInstance;
class MscInstanceEvent;
class MscTimer;

struct ChartLayoutManagerPrivate;

class ChartLayoutManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(msc::MscChart *currentChart READ currentChart WRITE setCurrentChart NOTIFY currentChartChanged)
    Q_PROPERTY(QRectF instanceRect READ instancesRect NOTIFY instancesRectChanged)

public:
    explicit ChartLayoutManager(MscCommandsStack *undoStack, QObject *parent = nullptr);
    ~ChartLayoutManager();

    QGraphicsScene *graphicsScene() const;

    void setCurrentChart(msc::MscChart *chart);
    msc::MscChart *currentChart() const;

    void clearScene();

    msc::InstanceItem *createDefaultInstanceItem(
            msc::MscInstance *orphanInstance = nullptr, const QPointF &pos = QPointF());
    bool removeInstanceItem(msc::InstanceItem *item);
    msc::MessageItem *createDefaultMessageItem(
            msc::MscMessage *orphanMessage = nullptr, const QPointF &pos = QPointF());
    bool removeMessageItem(msc::MessageItem *item);

    ChartItem *itemForChart() const;
    CommentItem *itemForComment(msc::MscComment *comment) const;
    InstanceItem *itemForInstance(msc::MscInstance *instance) const;
    MessageItem *itemForMessage(MscMessage *message) const;
    ConditionItem *itemForCondition(MscCondition *condition) const;
    ActionItem *itemForAction(MscAction *action) const;
    TimerItem *itemForTimer(MscTimer *timer) const;
    CoregionItem *itemForCoregion(MscCoregion *coregion) const;
    InteractiveObject *itemForEntity(MscEntity *entity) const;

    msc::MscEntity *nearestEntity(const QPointF &pos);
    msc::MscInstance *nearestInstance(const QPointF &pos);
    int eventIndex(const QPointF &pt, MscInstanceEvent *ignoreEvent = nullptr);
    int eventInstanceIndex(const QPointF &pt, MscInstance *instance, MscInstanceEvent *ignoreEvent = nullptr);
    msc::MscInstanceEvent *eventAtPosition(const QPointF &pos);

    QSizeF preferredChartBoxSize() const;
    void setPreferredChartBoxSize(const QSizeF &size);

    void setVisibleItemLimit(int number);
    bool isStreamingModeEnabled() const;

    const QPointer<ChartItem> chartItem() const;
    QRectF minimalContentRect() const;
    QRectF actualContentRect() const;

    const QVector<msc::InstanceItem *> &instanceItems() const;

    const QVector<msc::InteractiveObject *> &instanceEventItems() const;

    const QVector<msc::ActionItem *> actionsOfInstance(const MscInstance *instance) const;
    const QVector<msc::ConditionItem *> conditionsOfInstance(const MscInstance *instance) const;
    const QVector<msc::TimerItem *> timersOfInstance(const MscInstance *instance) const;

    const QRectF &instancesRect() const;

    void setSystemChecker(msc::SystemChecks *checker);
    msc::SystemChecks *systemChecker() const;

    bool layoutUpdatePending() const;

    MscCommandsStack *undoStack() const;

    qreal interMessageSpan() const;
    msc::InteractiveObject *eventItem(const QUuid &id);

    QVector<MscInstanceEvent *> visibleEvents() const;

public Q_SLOTS:
    void updateLayout();
    void doLayout();
    void updateChartboxToContent();
    void updateContentToChartbox(const QRectF &chartBox);
    void removeInstanceItem(msc::MscInstance *instance);
    void removeEventItem(msc::MscInstanceEvent *event);
    void syncItemsPosToInstance(const InstanceItem *instanceItem);

Q_SIGNALS:
    void currentChartChanged(msc::MscChart *chart);
    void layoutComplete();
    void cifDataChanged();
    void instancesRectChanged(const QRectF &rect);
    void initialNameAccepted(MscEntity *entity);

private Q_SLOTS:
    void onInstanceGeometryChanged();
    void onInstanceEventItemMoved(shared::ui::InteractiveObjectBase *item);
    void onMessageRetargeted(msc::MessageItem *item, const QPointF &pos, msc::MscMessage::EndType endType);
    void onInstanceCreatorChanged(msc::MscInstance *newCreator);

private:
    std::unique_ptr<ChartLayoutManagerPrivate> const d;

    QVector<InteractiveObject *> instanceEventItems(MscInstance *instance) const;

    void checkHorizontalConstraints();
    void checkVerticalConstraints();
    void actualizeInstancesHeights(qreal height) const;
    void updateStoppedInstanceHeight(InstanceItem *instanceItem, qreal totalH) const;
    void updateCreatedInstanceHeight(InstanceItem *instanceItem, qreal totalH) const;
    MessageItem *fillMessageItem(MscMessage *message, InstanceItem *sourceItem, InstanceItem *targetItem, qreal y);
    void ensureInstanceCreationAdded(MscMessage *msgCreate, MscInstance *dynamicInstance);

    template<class ItemType>
    ItemType *itemForInstanceEvent(MscInstanceEvent *event) const;

    CommentItem *addCommentItem(MscComment *comment);
    MessageItem *addMessageItem(MscMessage *message);
    ActionItem *addActionItem(MscAction *action);
    ConditionItem *addConditionItem(MscCondition *condition, const QRectF &instancesRect);
    TimerItem *addTimerItem(MscTimer *timer);
    CoregionItem *addCoregionItem(MscCoregion *coregion);

    MessageItem *creatorItemOfInstance(MscInstance *instance) const;

    void addInstanceItems();
    void addInstanceEventItems();

    void storeEntityItem(InteractiveObject *item);

    void connectItems();
    void connectInstanceItem(InteractiveObject *instanceItem);
    void connectInstanceEventItem(InteractiveObject *instanceEventItem);
    void disconnectItems();

    QLineF commonAxis() const;

    void prepareChartBoxItem();

    void applyContentRect(const QRectF &newRect);

    QVariantList prepareChangeOrderCommand(MscInstance *instance) const;

    void forceCifForAll();

    void setInstancesRect(const QRectF &rect);

    qreal eventsBottom() const;
};

} // namespace msc
