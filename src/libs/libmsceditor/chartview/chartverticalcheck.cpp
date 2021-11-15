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

#include "chartverticalcheck.h"

#include "baseitems/instanceheaditem.h"
#include "chartlayoutmanager.h"
#include "commentitem.h"
#include "coregionitem.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscchart.h"
#include "msccoregion.h"
#include "mscinstance.h"
#include "mscmessage.h"

#include <QDebug>

namespace msc {

ChartVerticalCheck::ChartVerticalCheck() {}

void ChartVerticalCheck::reset(ChartLayoutManager *manager, MscChart *chart)
{
    m_manager = manager;
    m_currentChart = chart;

    minYperInstance.clear();
    for (MscInstance *instance : m_currentChart->instances()) {
        InstanceItem *instanceItem = m_manager->itemForInstance(instance);
        Q_ASSERT(instanceItem);
        minYperInstance[instance] = instanceItem->headerItem()->sceneBoundingRect().bottom() + m_minSpace;
    }

    m_overlapEvents.clear();
    m_processedEvents.clear();
    m_activeCoregionItem = nullptr;
}

void ChartVerticalCheck::checkVerticalConstraints()
{
    for (MscInstanceEvent *event : m_currentChart->instanceEvents()) {
        checkEvent(event);
    }
}

void ChartVerticalCheck::checkEvent(MscInstanceEvent *event)
{
    auto eventItem = qobject_cast<msc::EventItem *>(m_manager->eventItem(event->internalId()));
    if (!eventItem) {
        // Check if this is the end of a coregion
        if (m_activeCoregionItem != nullptr && m_activeCoregionItem->end() == event) {
            eventItem = m_activeCoregionItem;
        } else {
            return;
        }
    }

    if (m_processedEvents.contains(event)) {
        return;
    }

    if (eventItem->geometryManagedByCif()) {
        eventItem->applyCif();
    }

    const QVector<MscInstance *> instances = m_currentChart->relatedInstances(event);

    qreal minY = 0.0;
    for (MscInstance *inst : instances) {
        minY = std::max(minY, minYperInstance[inst]);
    }

    switch (event->entityType()) {
    case MscEntity::EntityType::Action:
    case MscEntity::EntityType::Condition:
    case MscEntity::EntityType::Timer: {
        if (eventItem->instancesTopArea(instances) < minY) {
            eventItem->setY(minY + m_manager->interMessageSpan());
        }
        // Check for overlaps (crossing messages)
        if (!m_overlapEvents.contains(eventItem)) {
            for (const msc::EventItem *ev : m_overlapEvents) {
                if (eventItem->sceneBoundingRect().intersects(ev->sceneBoundingRect())) {
                    eventItem->setY(ev->sceneBoundingRect().bottom() + 2);
                }
            }
            m_overlapEvents.append(eventItem);
        }
        minY = eventItem->instancesBottomArea(instances) + m_minSpace;
        for (MscInstance *inst : instances) {
            minYperInstance[inst] = minY;
        }
        break;
    }
    case MscEntity::EntityType::Create:
    case MscEntity::EntityType::Message: {
        auto messageItem = static_cast<MessageItem *>(eventItem);
        bool isCrossing = m_currentChart->isCrossingMessage(messageItem->modelItem());
        checkMessageTail(messageItem, isCrossing);
        m_processedEvents.insert(event);
        if (isCrossing) {
            checkPrecedingEvents(event, messageItem->modelItem()->targetInstance());
            checkMessageHead(messageItem, isCrossing);
        } else {
            checkMessageHead(messageItem, isCrossing);
        }

        // Only check horizontal messages for overlaps
        if (!m_overlapEvents.contains(messageItem) && messageItem->isHorizontal() && !isCrossing) {
            if (event->entityType() == MscEntity::EntityType::Message) {
                for (const msc::EventItem *ev : m_overlapEvents) {
                    if (messageItem->sceneBoundingRect().intersects(ev->sceneBoundingRect())) {
                        messageItem->moveToYPosition(ev->sceneBoundingRect().bottom() + m_messageOffset);
                    }
                }
            }
            m_overlapEvents.append(messageItem);
            if (messageItem->modelItem()->targetInstance()) {
                minYperInstance[messageItem->modelItem()->targetInstance()] =
                        messageItem->head().y() + m_minSpace + m_messageOffset;
            }
            if (messageItem->modelItem()->sourceInstance()) {
                minYperInstance[messageItem->modelItem()->sourceInstance()] =
                        messageItem->tail().y() + m_minSpace + m_messageOffset;
            }
        }
        break;
    }
    case MscEntity::EntityType::Coregion: {
        auto coregion = static_cast<MscCoregion *>(event);
        MscInstance *instance = coregion->instance();
        if (coregion->type() == MscCoregion::Type::Begin) {
            Q_ASSERT(m_activeCoregionItem == nullptr);
            m_activeCoregionItem = static_cast<CoregionItem *>(eventItem);
            if (eventItem->instanceTopArea(instance) < minY) {
                eventItem->setY(minY + m_manager->interMessageSpan());
            }
            minY = eventItem->instanceTopArea(instance) + m_minSpace;
            minYperInstance[instance] = minY;
        } else {
            Q_ASSERT(m_activeCoregionItem != nullptr);
            // check if end is below last included item
            if (m_activeCoregionItem->instanceBottomArea(instance) < minY) {
                QRectF bbox = m_activeCoregionItem->boundingRect();
                bbox.setHeight((minY + m_manager->interMessageSpan()) - m_activeCoregionItem->y());
                m_activeCoregionItem->setBoundingRect(bbox);
            }
            minY = m_activeCoregionItem->instanceBottomArea(instance) + m_minSpace;
            minYperInstance[instance] = minY;
            m_activeCoregionItem = nullptr;
        }
        break;
    }
    default:
        qDebug() << "Type" << event->entityType() << "unhandled yet" << Q_FUNC_INFO;
        break;
    }
    m_processedEvents.insert(event);

    if (event->comment()) {
        if (CommentItem *commentItem = m_manager->itemForComment(event->comment())) {
            commentItem->instantLayoutUpdate();
        }
    }
}

/*!
   Simpler vertical layout of the events in streaming mode
 */
void ChartVerticalCheck::checkStreamingVerticalConstraints()
{
    if (!m_currentChart || !m_manager) {
        return;
    }

    double yPos = 0;
    for (InstanceItem *item : m_manager->instanceItems()) {
        if (!item->modelItem()->isCreated()) {
            yPos = std::max(yPos, item->headerItem()->sceneBoundingRect().bottom() + m_manager->interMessageSpan());
        }
    }

    for (MscInstanceEvent *event : m_manager->visibleEvents()) {
        auto eventItem = qobject_cast<msc::EventItem *>(m_manager->eventItem(event->internalId()));
        if (!eventItem) {
            continue;
        }

        switch (event->entityType()) {
        case MscEntity::EntityType::Action:
        case MscEntity::EntityType::Condition:
        case MscEntity::EntityType::Timer: {
            eventItem->setY(yPos);
            yPos = eventItem->sceneBoundingRect().bottom() + m_manager->interMessageSpan();
            break;
        }
        case MscEntity::EntityType::Create:
        case MscEntity::EntityType::Message: {
            auto messageItem = static_cast<MessageItem *>(eventItem);
            messageItem->moveToYPosition(yPos);
            yPos = eventItem->sceneBoundingRect().bottom() + m_manager->interMessageSpan();
            break;
        }
        case MscEntity::EntityType::Coregion: {
            // Coregions are not handled in RemoteControlWebServer / RemoteControlHandler
            break;
        }
        default:
            break;
        }
    }
}

void ChartVerticalCheck::checkMessageHead(MessageItem *messageItem, bool isCrossing)
{
    if (messageItem->modelItem()->targetInstance()) {
        const qreal messageMinY = minYperInstance[messageItem->modelItem()->targetInstance()] + m_messageOffset;
        if (messageItem->head().y() < messageMinY) {
            messageItem->setHeadPosition(
                    QPointF(messageItem->head().x(), messageMinY + m_manager->interMessageSpan()), isCrossing);
        }
        minYperInstance[messageItem->modelItem()->targetInstance()] =
                messageItem->head().y() + m_minSpace + m_messageOffset;
    }
}

void ChartVerticalCheck::checkMessageTail(MessageItem *messageItem, bool isCrossing)
{
    if (messageItem->modelItem()->sourceInstance()) {
        const qreal messageMinY = minYperInstance[messageItem->modelItem()->sourceInstance()] + m_messageOffset;
        if (messageItem->tail().y() < messageMinY) {
            messageItem->setTailPosition(
                    QPointF(messageItem->tail().x(), messageMinY + m_manager->interMessageSpan()), isCrossing);
        }
        minYperInstance[messageItem->modelItem()->sourceInstance()] =
                messageItem->tail().y() + m_minSpace + m_messageOffset;
    }
}

/*!
   Checks all eventsbefore the given event for one instance
 */
void ChartVerticalCheck::checkPrecedingEvents(MscInstanceEvent *event, MscInstance *instance)
{
    QVector<MscInstanceEvent *> events = m_currentChart->eventsForInstance(instance);
    for (MscInstanceEvent *ev : events) {
        if (ev == event) {
            return;
        }
        checkEvent(ev);
    }
}

}
