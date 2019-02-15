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

#include "mscchart.h"

#include "mscaction.h"
#include "msccondition.h"
#include "mscdocument.h"
#include "mscgate.h"
#include "mscinstance.h"
#include "msctimer.h"

#include <QDebug>

namespace msc {

MscChart::MscChart(QObject *parent)
    : MscEntity(parent)
{
}

MscChart::MscChart(const QString &name, QObject *parent)
    : MscEntity(name, parent)
{
}

MscChart::~MscChart()
{
    qDeleteAll(m_instances);
    m_instances.clear();

    qDeleteAll(m_instanceEvents);
    m_instanceEvents.clear();

    qDeleteAll(m_gates);
    m_gates.clear();
}

const QVector<MscInstance *> &MscChart::instances() const
{
    return m_instances;
}

/*!
   Adds an instance, and takes over parentship.
 */
void MscChart::addInstance(MscInstance *instance, int index)
{
    if (instance == nullptr) {
        return;
    }
    if (m_instances.contains(instance)) {
        return;
    }

    instance->setParent(this);
    if (index < 0 || index >= m_instances.size()) {
        m_instances.append(instance);
    } else {
        m_instances.insert(index, instance);
    }
    connect(instance, &MscInstance::dataChanged, this, &MscChart::dataChanged);
    Q_EMIT instanceAdded(instance);
    Q_EMIT dataChanged();
}

/*!
   Removes the instance, but does not delete it.
   Removes the parentship of this chart.
 */
void MscChart::removeInstance(MscInstance *instance)
{
    if (instance == nullptr) {
        return;
    }
    if (!m_instances.contains(instance)) {
        return;
    }

    if (m_instances.removeAll(instance)) {
        if (instance->parent() == this) {
            instance->setParent(nullptr);
        }
        Q_EMIT instanceRemoved(instance);
        Q_EMIT dataChanged();
    }
}

MscInstance *MscChart::instanceByName(const QString &name) const
{
    for (MscInstance *instance : m_instances) {
        if (instance->name() == name) {
            return instance;
        }
    }
    return nullptr;
}

const QVector<MscInstanceEvent *> &MscChart::instanceEvents() const
{
    return m_instanceEvents;
}

QVector<MscInstanceEvent *> MscChart::eventsForInstance(MscInstance *instance) const
{
    QVector<MscInstanceEvent *> events;
    if (instance)
        for (MscInstanceEvent *instanceEvent : instanceEvents()) {
            switch (instanceEvent->entityType()) {
            case MscEntity::EntityType::Document:
            case MscEntity::EntityType::Chart:
            case MscEntity::EntityType::Instance:
            case MscEntity::EntityType::Create: // A synthetic MscMessage is used instead
                continue;
            case MscEntity::EntityType::Message: {
                auto message = static_cast<MscMessage *>(instanceEvent);
                if (message->relatesTo(instance))
                    events.append(instanceEvent);
                break;
            }
            case MscEntity::EntityType::Condition: {
                auto condition = static_cast<MscCondition *>(instanceEvent);
                if (condition->instance() == instance)
                    events.append(instanceEvent);
                break;
            }
            case MscEntity::EntityType::Gate: {
                auto gate = static_cast<MscGate *>(instanceEvent);
                if (gate->instance() == instance)
                    events.append(instanceEvent);
                break;
            }
            default: {
                qWarning() << Q_FUNC_INFO << "ignored type:" << instanceEvent->entityType();
                break;
            }
            }
        }

    return events;
}

/*!
   Adds an instance event, and takes over parentship.
   @param eventIdex is the vertical position of the new event. If it tis < 0, then the new event is appended.
 */
void MscChart::addInstanceEvent(MscInstanceEvent *instanceEvent, int eventIndex)
{
    if (instanceEvent == nullptr) {
        return;
    }
    if (m_instanceEvents.contains(instanceEvent)) {
        return;
    }

    instanceEvent->setParent(this);
    if (eventIndex < 0 || eventIndex >= m_instanceEvents.size()) {
        m_instanceEvents.append(instanceEvent);
    } else {
        m_instanceEvents.insert(eventIndex, instanceEvent);
    }
    connect(instanceEvent, &MscInstanceEvent::dataChanged, this, &MscChart::dataChanged);

    if (instanceEvent->entityType() == MscEntity::EntityType::Timer) {
        MscTimer *timer = static_cast<MscTimer *>(instanceEvent);
        connect(timer, &MscInstanceEvent::nameChanged, this, &MscChart::checkTimerRelations);
        connect(timer, &MscTimer::instanceChanged, this, &MscChart::checkTimerRelations);
    }

    checkTimerRelations();

    Q_EMIT instanceEventAdded(instanceEvent);
    Q_EMIT dataChanged();
}

/*!
   Removes the instance event, but does not delete it.
   Removes the parentship of this chart.
 */
void MscChart::removeInstanceEvent(MscInstanceEvent *instanceEvent)
{
    if (instanceEvent == nullptr) {
        return;
    }
    if (!m_instanceEvents.contains(instanceEvent)) {
        return;
    }

    if (m_instanceEvents.removeAll(instanceEvent)) {
        if (instanceEvent->parent() == this) {
            instanceEvent->setParent(nullptr);
        }
        disconnect(instanceEvent, nullptr, this, nullptr);

        Q_EMIT instanceEventRemoved(instanceEvent);
        Q_EMIT dataChanged();
    }
}

MscMessage *MscChart::messageByName(const QString &name) const
{
    for (MscInstanceEvent *message : m_instanceEvents) {
        if (message->entityType() == MscEntity::EntityType::Message && message->name() == name) {
            return static_cast<MscMessage *>(message);
        }
    }
    return nullptr;
}

const QVector<MscGate *> &MscChart::gates() const
{
    return m_gates;
}

/*!
   Adds a gate, and takes over parentship.
 */
void MscChart::addGate(MscGate *gate)
{
    if (!gate)
        return;

    if (gates().contains(gate))
        return;

    gate->setParent(this);
    m_gates.append(gate);
    connect(gate, &MscGate::dataChanged, this, &MscChart::dataChanged);
    Q_EMIT gateAdded(gate);
    Q_EMIT dataChanged();
}

/*!
   Removes the gate, but does not delete it.
   Removes the parentship of this chart.
 */
void MscChart::removeGate(MscGate *gate)
{
    if (!gate)
        return;
    if (!m_gates.contains(gate))
        return;

    if (m_gates.removeAll(gate)) {
        if (gate->parent() == this) {
            gate->setParent(nullptr);
        }
        Q_EMIT gateRemoved(gate);
        Q_EMIT dataChanged();
    }
}

/*!
  \brief MscChart::parentDocument
  \return The document that this chart is included in. Returns a nullptr if this chart is at the
  root of the model
*/
MscDocument *MscChart::parentDocument() const
{
    return dynamic_cast<MscDocument *>(parent());
}

MscEntity::EntityType MscChart::entityType() const
{
    return MscEntity::EntityType::Chart;
}

void MscChart::updateInstancePos(MscInstance *instance, int pos)
{
    Q_ASSERT(instance);

    const int currPos = m_instances.indexOf(instance);
    Q_ASSERT(currPos != -1);

    if (currPos == pos)
        return;

    m_instances.takeAt(currPos);
    m_instances.insert(pos, instance);

    Q_EMIT instanceMoved(instance, currPos, pos);
    Q_EMIT dataChanged();
}

template<class T>
bool setEventInstance(T *event, MscInstance *newInstance)
{
    Q_ASSERT(event);
    Q_ASSERT(newInstance);
    if (event->instance() != newInstance) {
        event->setInstance(newInstance);
        return true;
    }
    return false;
}

void MscChart::updateActionPos(MscAction *action, MscInstance *newInstance, int eventPos)
{
    bool changed = setEventInstance(action, newInstance);
    changed |= moveEvent(action, eventPos);

    if (changed) {
        Q_EMIT eventMoved();
        Q_EMIT dataChanged();
    }
}

void MscChart::updateConditionPos(MscCondition *condition, MscInstance *newInstance, int eventPos)
{
    bool changed = setEventInstance(condition, newInstance);
    changed |= moveEvent(condition, eventPos);

    if (changed) {
        Q_EMIT eventMoved();
        Q_EMIT dataChanged();
    }
}

void MscChart::updateTimerPos(MscTimer *timer, MscInstance *newInstance, int eventPos)
{
    bool changed = setEventInstance(timer, newInstance);
    changed |= moveEvent(timer, eventPos);

    if (changed) {
        Q_EMIT eventMoved();
        Q_EMIT dataChanged();
    }
}

/*!
   Move the given event vertically to the given position in the event sequence.
   Return true, if the event was moved.
 */
bool MscChart::moveEvent(MscInstanceEvent *event, int newIndex)
{
    const int currentPos = m_instanceEvents.indexOf(event);
    if (newIndex != currentPos && currentPos >= 0 && newIndex >= 0 && newIndex < m_instanceEvents.size()) {
        m_instanceEvents.takeAt(currentPos);
        m_instanceEvents.insert(newIndex, event);
        return true;
    }
    return false;
}

void MscChart::updateMessageTarget(MscMessage *message, MscInstance *newInstance, int eventPos,
                                   msc::MscMessage::EndType endType)
{
    Q_ASSERT(message);

    bool changed = false;
    if (!message->relatesTo(newInstance)) {
        if (endType == msc::MscMessage::EndType::SOURCE_TAIL) {
            message->setSourceInstance(newInstance);
        } else {
            message->setTargetInstance(newInstance);
        }
        changed = true;
    }

    changed |= moveEvent(message, eventPos);

    if (changed) {
        Q_EMIT messageRetargeted();
        Q_EMIT dataChanged();
    }
}

void MscChart::checkTimerRelations()
{
    QVector<MscTimer *> timers;
    for (MscInstanceEvent *event : instanceEvents()) {
        if (event->entityType() == MscEntity::EntityType::Timer) {
            auto timer = static_cast<MscTimer *>(event);
            timers.append(timer);
        }
    }

    for (auto it = timers.begin(); it != timers.end(); ++it) {
        Q_ASSERT(*it != nullptr);
        const QString &name = (*it)->name();
        auto it2 = it;
        ++it2;
        while (it2 != timers.end()) {
            if ((*it2)->name() == name && (*it2)->instance() == (*it)->instance()) {
                (*it)->setFollowingTimer(*it2);
                (*it2)->setPrecedingTimer(*it);
                break;
            }
            ++it2;
        }
        if (it2 == timers.end()) {
            (*it)->setFollowingTimer(nullptr);
        }
        if (MscTimer *oldPre = (*it)->precedingTimer()) {
            if (oldPre->name() != name || oldPre->instance() != (*it)->instance()) {
                (*it)->setPrecedingTimer(nullptr);
            }
        }
    }
}

} // namespace msc
