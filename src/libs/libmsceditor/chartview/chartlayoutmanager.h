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

#include "chartlayoutmanagerbase.h"
#include "instanceitem.h"
#include "mscchart.h"
#include "mscentity.h"
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
class InteractiveObject;
class MessageItem;
class TimerItem;

class MscAction;
class MscCommandsStack;
class MscCondition;
class MscCoregion;
class MscInstance;
class MscInstanceEvent;
class MscTimer;

struct ChartLayoutManagerPrivate;

class ChartLayoutManager : public ChartLayoutManagerBase
{
    Q_OBJECT
    Q_PROPERTY(msc::MscChart *currentChart READ currentChart WRITE setCurrentChart NOTIFY currentChartChanged)

public:
    explicit ChartLayoutManager(MscCommandsStack *undoStack, QObject *parent = nullptr);
    ~ChartLayoutManager();

    QGraphicsScene *graphicsScene() const;

    void setCurrentChart(msc::MscChart *chart);
    msc::MscChart *currentChart() const override;

    void clearScene();

    msc::InstanceItem *createDefaultInstanceItem(
            msc::MscInstance *orphanInstance = nullptr, const QPointF &pos = QPointF());
    bool removeInstanceItem(msc::InstanceItem *item);
    msc::MessageItem *createDefaultMessageItem(
            msc::MscMessage *orphanMessage = nullptr, const QPointF &pos = QPointF());
    bool removeMessageItem(msc::MessageItem *item);

    ChartItem *itemForChart() const override;
    CommentItem *itemForComment(msc::MscComment *comment) const override;
    InstanceItem *itemForInstance(msc::MscInstance *instance) const override;
    MessageItem *itemForMessage(MscMessage *message) const override;
    ConditionItem *itemForCondition(MscCondition *condition) const override;
    ActionItem *itemForAction(MscAction *action) const override;
    TimerItem *itemForTimer(MscTimer *timer) const override;
    CoregionItem *itemForCoregion(MscCoregion *coregion) const override;
    InteractiveObject *itemForEntity(MscEntity *entity) const override;

    msc::MscEntity *nearestEntity(const QPointF &pos);
    msc::MscInstance *nearestInstance(const QPointF &pos);
    int eventIndex(const QPointF &pt, MscInstanceEvent *ignoreEvent = nullptr);
    int eventInstanceIndex(
            const QPointF &pt, MscInstance *instance, MscInstanceEvent *ignoreEvent = nullptr) const override;
    msc::MscInstanceEvent *eventAtPosition(const QPointF &scenePos) const override;

    QSizeF preferredChartBoxSize() const;
    void setPreferredChartBoxSize(const QSizeF &size);

    QRectF minimalContentRect() const override;
    QRectF actualContentRect() const override;

    const QVector<msc::InstanceItem *> &instanceItems() const override;

    const QVector<msc::InteractiveObject *> &allEventItems() const;

    const QVector<msc::ActionItem *> actionsOfInstance(const MscInstance *instance) const;
    const QVector<msc::ConditionItem *> conditionsOfInstance(const MscInstance *instance) const;
    const QVector<msc::TimerItem *> timersOfInstance(const MscInstance *instance) const;

    bool layoutUpdatePending() const override;

    msc::InteractiveObject *eventItem(const QUuid &id);

    QVector<MscInstanceEvent *> visibleEvents() const;

public Q_SLOTS:
    void updateLayout();
    void doLayout();
    void updateChartboxToContent();
    void updateContentToChartbox(const QRectF &chartBox);
    void removeInstanceItem(msc::MscInstance *instance);
    void removeEventItem(msc::MscInstanceEvent *event);

Q_SIGNALS:
    void currentChartChanged(msc::MscChart *chart);
    void layoutComplete();
    void cifDataChanged();
    void initialNameAccepted(msc::MscEntity *entity);

private Q_SLOTS:
    void onInstanceGeometryChanged();
    void onInstanceEventItemMoved(shared::ui::InteractiveObjectBase *item);
    void onMessageRetargeted(msc::MessageItem *item, const QPointF &pos, msc::MscMessage::EndType endType);
    void onInstanceCreatorChanged(msc::MscInstance *newCreator);

private:
    std::unique_ptr<ChartLayoutManagerPrivate> const d;

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
    ConditionItem *addConditionItem(MscCondition *condition);
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

    qreal eventsBottom() const;
};

} // namespace msc
