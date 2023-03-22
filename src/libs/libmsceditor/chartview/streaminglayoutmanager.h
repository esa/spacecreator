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

#pragma once

#include "chartlayoutmanagerbase.h"
#include "instanceitem.h"
#include "mscinstanceevent.h"

#include <QHash>
#include <QObject>
#include <QPointer>
#include <QUuid>
#include <memory>

class QGraphicsScene;

namespace msc {

class MscCreate;

/**
 * A non editable manager for one MSC chart.
 * It does react on model changes
 */
class StreamingLayoutManager : public ChartLayoutManagerBase
{
    Q_OBJECT
    Q_PROPERTY(QVector<msc::InstanceItem *> instanceItems READ instanceItems NOTIFY instanceItemsChanged)
public:
    explicit StreamingLayoutManager(MscCommandsStack *undoStack, QObject *parent = nullptr);
    ~StreamingLayoutManager();

    QGraphicsScene *graphicsScene() const;

    void setCurrentChart(MscChart *chart);
    MscChart *currentChart() const override;

    void setVisibleItemLimit(int number);

    ChartItem *itemForChart() const override;
    CommentItem *itemForComment(MscComment *comment) const override;
    InstanceItem *itemForInstance(MscInstance *instance) const override;
    MessageItem *itemForMessage(MscMessage *message) const override;
    ConditionItem *itemForCondition(MscCondition *condition) const override;
    ActionItem *itemForAction(MscAction *action) const override;
    TimerItem *itemForTimer(MscTimer *timer) const override;
    CoregionItem *itemForCoregion(MscCoregion *coregion) const override;
    InteractiveObject *itemForEntity(MscEntity *entity) const override;
    template<class ItemType>
    ItemType *itemForInstanceEvent(MscInstanceEvent *event) const
    {
        return event ? static_cast<ItemType *>(m_eventItems.value(event->internalId())) : nullptr;
    }

    int eventInstanceIndex(const QPointF &pt, MscInstance *instance, MscInstanceEvent *ignoreEvent) const override;
    MscInstanceEvent *eventAtPosition(const QPointF &scenePos) const override;
    QRectF minimalContentRect() const override;
    QRectF actualContentRect() const override;

    const QVector<msc::InstanceItem *> &instanceItems() const override;

    //
    QPointF instanceIndexToPosition(int index) const;

    qreal nextEventYPosition(MscInstance *instance) const;

Q_SIGNALS:
    void instanceItemsChanged();

private Q_SLOTS:
    void instanceAdded(msc::MscInstance *instance, int index);
    void instanceRemoved(msc::MscInstance *instance);
    void updateContentsRect();
    void eventAdded(msc::MscInstanceEvent *event);
    void eventRemoved(msc::MscInstanceEvent *event);
    void updateMessagePosition();

private:
    void addAction(msc::MscAction *action);
    void addCondition(msc::MscCondition *condition);
    void addCreateMessage(msc::MscCreate *message);
    void addMessage(msc::MscMessage *message);
    void addTimer(msc::MscTimer *timer);
    void updateInstancesRect();
    void checkChartSize(EventItem *item);

    QPointer<MscChart> m_chart;
    std::unique_ptr<QGraphicsScene> m_scene;
    int m_visibleItems = -1;

    std::unique_ptr<ChartItem> m_chartItem;
    QVector<msc::InstanceItem *> m_instanceItems;
    QHash<QUuid, msc::InteractiveObject *> m_eventItems;
    QMap<MscInstance *, qreal> m_nextYperInstance;
};

} // namespace msc
