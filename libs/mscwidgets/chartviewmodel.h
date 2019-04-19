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

namespace msc {

class ActionItem;
class ChartItem;
class CommentItem;
class ConditionItem;
class CoregionItem;
class InstanceItem;
class InteractiveObject;
class MessageItem;
class TimerItem;

class MscAction;
class MscChart;
class MscCondition;
class MscCoregion;
class MscInstance;
class MscInstanceEvent;
class MscTimer;

struct ChartViewModelPrivate;
struct ChartViewLayoutInfo;
class ChartViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ChartViewModel(QObject *parent = nullptr);
    ~ChartViewModel();

    QGraphicsScene *graphicsScene() const;

    msc::MscChart *currentChart() const;

    void clearScene();

    msc::InstanceItem *createDefaultInstanceItem(msc::MscInstance *orphanInstance = nullptr,
                                                 const QPointF &pos = QPointF());
    bool removeInstanceItem(msc::InstanceItem *item);
    msc::MessageItem *createDefaultMessageItem(msc::MscMessage *orphanMessage = nullptr,
                                               const QPointF &pos = QPointF());
    bool removeMessageItem(msc::MessageItem *item);

    InstanceItem *itemForInstance(msc::MscInstance *instance) const;
    MessageItem *itemForMessage(MscMessage *message) const;
    ConditionItem *itemForCondition(MscCondition *condition) const;
    ActionItem *itemForAction(MscAction *action) const;
    TimerItem *itemForTimer(MscTimer *timer) const;
    CoregionItem *itemForCoregion(MscCoregion *coregion) const;
    InteractiveObject *itemForEntity(MscEntity *entity) const;

    CommentItem *commentForEntity(msc::MscEntity *entity);
    msc::MscEntity *nearestEntity(const QPointF &pos);
    msc::MscInstance *nearestInstance(const QPointF &pos);
    int eventIndex(qreal y);
    msc::MscInstanceEvent *eventAtPosition(const QPointF &pos);

    QSizeF preferredChartBoxSize() const;
    void setPreferredChartBoxSize(const QSizeF &size);

    int instanceOrderFromPos(const QPointF &scenePos);

    void setVisibleItemLimit(int number);

    const QPointer<ChartItem> chartItem() const;

public Q_SLOTS:
    void updateLayout();
    void fillView(msc::MscChart *chart);
    void updateContentBounds();
    void removeInstanceItem(msc::MscInstance *instance);
    void removeEventItem(msc::MscInstanceEvent *event);

Q_SIGNALS:
    void currentChartChanged(msc::MscChart *chart);
    void layoutComplete();
    void cifDataChanged();

private Q_SLOTS:
    void onEntityCommentChanged(const QString &comment = QString());
    void onInteractiveObjectCommentChange(msc::MscEntity *entity, const QString &comment);
    void onInstanceItemMoved(InteractiveObject *instanceItem);
    void onInstanceEventItemMoved(InteractiveObject *item);
    void onMessageRetargeted(MessageItem *item, const QPointF &pos, msc::MscMessage::EndType endType);
    void advanceItemsToChartBox();

private:
    std::unique_ptr<ChartViewModelPrivate> const d;

    Q_INVOKABLE void doLayout();

    QVector<QGraphicsObject *> instanceEventItems(MscInstance *instance) const;

    void actualizeInstancesHeights(qreal height) const;
    void updateStoppedInstanceHeight(InstanceItem *instanceItem, qreal totalH) const;
    void updateCreatedInstanceHeight(InstanceItem *instanceItem, qreal totalH) const;
    MessageItem *fillMessageItem(MscMessage *message, InstanceItem *sourceItem, InstanceItem *targetItem, qreal y);
    void ensureInstanceCreationAdded(MscMessage *msgCreate, MscInstance *dynamicInstance);

    template<class ItemType>
    ItemType *itemForInstanceEvent(MscInstanceEvent *event) const;

    MessageItem *addMessageItem(MscMessage *message);
    ActionItem *addActionItem(MscAction *action);
    ConditionItem *addConditionItem(MscCondition *condition, ConditionItem *prevItem, QRectF &instancesRect);
    TimerItem *addTimerItem(MscTimer *timer);
    CoregionItem *addCoregionItem(MscCoregion *coregion);

    void polishAddedEventItem(MscInstanceEvent *event, InteractiveObject *item);
    void updateComment(msc::MscEntity *entity, msc::InteractiveObject *iObj = nullptr);
    void updateCommentForMscChart();
    void updateCommentForInteractiveObject(InteractiveObject *iObj);

    void addInstanceItems();
    void addInstanceEventItems();

    void storeEntityItem(InteractiveObject *item);

    QRectF minimalContentRect() const;

    void connectItems();
    void connectInstanceItem(InteractiveObject *instanceItem);
    void connectInstanceEventItem(InteractiveObject *instanceEventItem);
    void disconnectItems();

    QLineF commonAxis() const;

    void prepareChartBoxItem();
};

} // namespace msc
