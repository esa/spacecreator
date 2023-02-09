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

#include "chartlayoutmanagerbase.h"

#include "actionitem.h"
#include "baseitems/common/coordinatesconverter.h"
#include "conditionitem.h"
#include "coregionitem.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "mscmessage.h"
#include "msctimer.h"
#include "timeritem.h"

namespace msc {

ChartLayoutManagerBase::ChartLayoutManagerBase(MscCommandsStack *undoStack, QObject *parent)
    : QObject { parent }
    , m_undoStack(undoStack)
{
    Q_ASSERT(undoStack != nullptr);
}

/**
   Returns the undo stack used for the modles displayed by this ChartLayoutManager
   \note The MscCommandsStack object is owned by some other class
 */
MscCommandsStack *ChartLayoutManagerBase::undoStack() const
{
    return m_undoStack;
}

/**
   Sets the object to check if the msc entities correspond to the iv model
 */
void ChartLayoutManagerBase::setSystemChecker(SystemChecks *checker)
{
    if (checker == m_systemChecker) {
        return;
    }
    m_systemChecker = checker;
    Q_EMIT systemCheckerChanged(m_systemChecker);
}

/**
   Returns the object to check if msc entities correspond to the iv model
 */
SystemChecks *ChartLayoutManagerBase::systemChecker() const
{
    return m_systemChecker;
}

/**
 * Returns if an update/recalculation is pending
 */
bool ChartLayoutManagerBase::layoutUpdatePending() const
{
    return false;
}

/*!
    Returns the bounding box of all instances
 */
const QRectF &ChartLayoutManagerBase::instancesRect() const
{
    return m_instancesRect;
}

/**
 * Returns the default distance/space between instances in scene pixel
 */
qreal ChartLayoutManagerBase::interInstanceSpan()
{
    static constexpr int interInstanceSpanCIF { 500 };
    static qreal interInstanceSpanScene { 0. };
    if (qFuzzyIsNull(interInstanceSpanScene)) {
        interInstanceSpanScene = CoordinatesConverter::heightInScene(interInstanceSpanCIF);
    }
    return interInstanceSpanScene;
}

/**
 * Returns the default vertical distance/space between events in scene pixel
 */
qreal ChartLayoutManagerBase::interMessageSpan()
{
    static constexpr int interMessageSpanCIF { 40 };
    static qreal interMessageSpanScene { 0. };
    if (qFuzzyIsNull(interMessageSpanScene)) {
        interMessageSpanScene = CoordinatesConverter::heightInScene(interMessageSpanCIF);
    }
    return interMessageSpanScene;
}

/**
 * Horizontally aligns all events related to the instance \p instanceItem
 */
void ChartLayoutManagerBase::syncItemsPosToInstance(const InstanceItem *instanceItem)
{
    if (!instanceItem) {
        return;
    }

    QVector<InteractiveObject *> events = instanceEventItems(instanceItem->modelItem());
    for (InteractiveObject *item : qAsConst(events)) {
        EventItem *eventItem = qobject_cast<EventItem *>(item);
        syncItemPosToInstance(eventItem, instanceItem);
    }
}

void ChartLayoutManagerBase::syncItemPosToInstance(EventItem *eventItem, const InstanceItem *instanceItem)
{
    if (!instanceItem) {
        return;
    }

    const qreal instanceCenter = instanceItem->centerInScene().x();

    switch (eventItem->modelEntity()->entityType()) {
    case MscEntity::EntityType::Action:
    case MscEntity::EntityType::Coregion: {
        eventItem->setTargetHCenter(instanceCenter);
        break;
    }
    case MscEntity::EntityType::Condition: {
        auto condition = static_cast<MscCondition *>(eventItem->modelEntity());
        if (!condition->shared()) {
            eventItem->setTargetHCenter(instanceCenter);
        }
        break;
    }
    case MscEntity::EntityType::Message: {
        auto messageItem = static_cast<MessageItem *>(eventItem);
        auto message = static_cast<MscMessage *>(eventItem->modelEntity());
        if (message->sourceInstance() == instanceItem->modelItem()) {
            QPointF sourcePt = messageItem->tail();
            sourcePt.setX(instanceCenter);
            messageItem->setTailPosition(sourcePt);
        }
        if (message->targetInstance() == instanceItem->modelItem()) {
            QPointF targetPt = messageItem->head();
            targetPt.setX(instanceCenter);
            messageItem->setHeadPosition(targetPt);
        }
        break;
    }
    case MscEntity::EntityType::Create: {
        auto messageItem = static_cast<MessageItem *>(eventItem);
        auto message = static_cast<MscMessage *>(eventItem->modelEntity());
        if (message->sourceInstance() == instanceItem->modelItem()) {
            QPointF sourcePt = messageItem->tail();
            sourcePt.setX(instanceCenter);
            messageItem->setTailPosition(sourcePt);
        }
        if (messageItem->targetInstanceItem() && messageItem->sourceInstanceItem()) {
            QPointF targetPt = messageItem->head();
            InstanceItem *targetItem = messageItem->targetInstanceItem();
            const qreal targetCenter = targetItem->centerInScene().x();
            const qreal sourceX = messageItem->sourceInstanceItem()->centerInScene().x();
            if (sourceX < targetCenter) {
                targetPt.setX(targetItem->sceneBoundingRect().left());
            } else {
                targetPt.setX(targetItem->sceneBoundingRect().right());
            }
            messageItem->setHeadPosition(targetPt);
        }
        break;
    }
    case MscEntity::EntityType::Timer: {
        eventItem->setX(instanceCenter);
        break;
    }
    default:
        break;
    }
}

/**
 * Returns all graphics items of events that relate to the given instance \p instance
 */
QVector<InteractiveObject *> ChartLayoutManagerBase::instanceEventItems(MscInstance *instance) const
{
    if (!currentChart()) {
        return {};
    }

    QVector<InteractiveObject *> res;

    const QVector<MscInstanceEvent *> events = currentChart()->eventsForInstance(instance);
    for (MscInstanceEvent *event : events) {
        switch (event->entityType()) {
        case MscEntity::EntityType::Create:
        case MscEntity::EntityType::Message: {
            if (auto item = itemForMessage(static_cast<MscMessage *>(event))) {
                res.append(item);
            }
            break;
        }
        case MscEntity::EntityType::Condition: {
            if (auto item = itemForCondition(static_cast<MscCondition *>(event))) {
                res.append(item);
            }
            break;
        }
        case MscEntity::EntityType::Action: {
            if (auto item = itemForAction(static_cast<MscAction *>(event))) {
                res.append(item);
            }
            break;
        }
        case MscEntity::EntityType::Timer: {
            if (auto item = itemForTimer(static_cast<MscTimer *>(event))) {
                res.append(item);
            }
            break;
        }
        case MscEntity::EntityType::Coregion: {
            if (auto item = itemForCoregion(static_cast<MscCoregion *>(event))) {
                res.append(item);
            }
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

void ChartLayoutManagerBase::setInstancesRect(const QRectF &rect)
{
    if (rect == m_instancesRect) {
        return;
    }

    m_instancesRect = rect;
    Q_EMIT instancesRectChanged(m_instancesRect);
}

} // namespace msc
