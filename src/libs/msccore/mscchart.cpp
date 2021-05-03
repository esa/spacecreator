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

#include "mscchart.h"

#include "cif/cifblockfactory.h"
#include "cif/ciflines.h"
#include "mscaction.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "mscdocument.h"
#include "mscgate.h"
#include "mscinstance.h"
#include "msctimer.h"

#include <QDebug>
#include <QVector>
#include <set>

namespace msc {

MscChart::MscChart(QObject *parent)
    : MscChart(DefaultName, parent)
{
}

MscChart::MscChart(const QString &name, QObject *parent)
    : MscEntity(name, parent)
{
}

MscChart::~MscChart()
{
    qDeleteAll(m_gates);
    m_gates.clear();
}

const QVector<MscInstance *> &MscChart::instances() const
{
    return m_instances;
}

/*!
   Returns the index of the given \p instance in the list of instances
   If the instance is not part of this chart, -1 is returned
 */
int MscChart::indexOfInstance(MscInstance *instance) const
{
    return m_instances.indexOf(instance);
}

QString MscChart::createUniqueInstanceName() const
{
    QString instanceName;
    int count = m_instances.size();
    do {
        instanceName = tr("Instance_%1").arg(++count);
    } while (instanceByName(instanceName));

    return instanceName;
}

MscInstance *MscChart::makeInstance(const QString &name, int index)
{
    QString instanceName(name);
    if (instanceName.isEmpty()) {
        int count = m_instances.size();
        do {
            instanceName = tr("Instance_%1").arg(++count);
        } while (instanceByName(instanceName));
    } else if (MscInstance *instance = instanceByName(instanceName)) {
        qWarning() << "Can't create. Instance with such name already exists:" << instanceName;
        return instance;
    }

    auto instance = new MscInstance(instanceName);
    addInstance(instance, index);
    return instance;
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

    if (!m_events.contains(instance)) {
        m_events[instance] = {};
    }
    for (MscInstanceEvent *ev : qAsConst(m_instanceEvents)) {
        if (relatedInstances(ev).contains(instance)) {
            if (m_events[instance].contains(ev)) {
                qFatal("Instance add failed - event already there!");
            }
            m_events[instance].append(ev);
        }
    }

    eventsCheck();

    Q_EMIT instanceAdded(instance, m_instances.indexOf(instance));
    Q_EMIT instancesChanged();
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

        m_events.remove(instance);

        Q_EMIT instanceRemoved(instance);
        Q_EMIT instancesChanged();
        Q_EMIT dataChanged();
    }
}

MscInstance *MscChart::instanceByName(const QString &name) const
{
    auto it = std::find_if(m_instances.constBegin(), m_instances.constEnd(),
            [name](MscInstance *instance) { return name == instance->name(); });

    if (it != m_instances.constEnd())
        return *it;

    return nullptr;
}

QVector<MscInstanceEvent *> MscChart::instanceEvents() const
{
    return allEvents();
}

QVector<MscInstanceEvent *> MscChart::eventsForInstance(const MscInstance *instance) const
{
    if (!instance || !m_events.contains(instance)) {
        return {};
    }

    return m_events.value(instance);
}

/*!
   Adds an instance event, and takes over parentship.
   @param instanceEvent The event (message/action/...) to add.
   @param eventIndex is the vertical position of the new event. If it is < 0, then the new event is appended.
   @return Returns the actual index, this event was inserted (might differ because of MSC constraints)
 */
int MscChart::addInstanceEvent(MscInstanceEvent *instanceEvent, int eventIndex)
{
    if (instanceEvent == nullptr) {
        return -1;
    }
    if (m_instanceEvents.contains(instanceEvent)) {
        return -1;
    }

    if (eventIndex < 0) {
        eventIndex = m_instanceEvents.size();
    }

    if (instanceEvent->entityType() == msc::MscEntity::EntityType::Create) {
        if (MscMessage *message = static_cast<MscMessage *>(instanceEvent)) {
            if (MscInstance *createdInstance = message->targetInstance()) {
                createdInstance->setExplicitCreator(message->sourceInstance());
                MscInstanceEvent *topEvent = firstEventOfInstance(createdInstance);
                int topIndex = m_instanceEvents.indexOf(topEvent);
                if (topIndex >= 0) {
                    eventIndex = std::min(topIndex, eventIndex);
                }
            }
        }
    }

    instanceEvent->setParent(this);
    if (eventIndex < 0 || eventIndex >= m_instanceEvents.size()) {
        m_instanceEvents.append(instanceEvent);
    } else {
        m_instanceEvents.insert(eventIndex, instanceEvent);
    }
    connect(instanceEvent, &MscInstanceEvent::dataChanged, this, &MscChart::dataChanged);

    QVector<MscInstance *> instances = relatedInstances(instanceEvent);
    for (MscInstance *inst : qAsConst(instances)) {
        if (!m_events.contains(inst)) {
            continue;
        }
        // Create messages havte to be the first one for instances created
        if (instanceEvent->entityType() == msc::MscEntity::EntityType::Create) {
            if (MscMessage *message = static_cast<MscMessage *>(instanceEvent)) {
                if (inst == message->targetInstance()) {
                    if (m_events[inst].contains(instanceEvent)) {
                        qFatal("event add failed - event already there! A");
                    }
                    m_events[inst].prepend(instanceEvent);
                    break;
                }
            }
        }

        // normal insert
        QVector<MscInstanceEvent *> &events = m_events[inst];
        int idx = 0;
        while (events.size() > idx && m_instanceEvents.indexOf(events[idx]) < eventIndex) {
            ++idx;
        }
        if (events.contains(instanceEvent)) {
            qFatal("event add failed - event already there! B");
        }
        events.insert(idx, instanceEvent);
    }
    if (instances.isEmpty()) {
        m_orphanEvents.append(instanceEvent);
    }
    connect(instanceEvent, &msc::MscInstanceEvent::instanceRelationChanged, this,
            [this](MscInstance *addedInstance, MscInstance *removedInstance) {
                if (auto ev = qobject_cast<MscInstanceEvent *>(sender())) {
                    msc::MscChart::eventInstanceChange(ev, addedInstance, removedInstance);
                }
            });

    if (instanceEvent->entityType() == MscEntity::EntityType::Timer) {
        MscTimer *timer = static_cast<MscTimer *>(instanceEvent);
        connect(timer, &MscTimer::instanceChanged, this, [this, timer]() { resetTimerRelations(timer); });
        connect(timer, &MscTimer::nameChanged, this, [this, timer]() { resetTimerRelations(timer); });
        resetTimerRelations(timer);
    }

    eventsCheck();

    Q_EMIT instanceEventAdded(instanceEvent);
    Q_EMIT instanceEventsChanged();
    Q_EMIT dataChanged();

    return m_instanceEvents.indexOf(instanceEvent);
}

/*!
   \brief MscChart::addInstanceEvent
   \param instanceEvent
   \param instance
   \param eventIndex
   \return
 */
void MscChart::addInstanceEvent(MscInstanceEvent *event, QHash<MscInstance *, int> instanceIndexes)
{
    if (!event) {
        return;
    }

    // Consistency check
    for (auto it = instanceIndexes.begin(); it != instanceIndexes.end(); ++it) {
        if (!event->relatesTo(it.key())) {
            qCritical("Instance for adding the event");
            return;
        }
        if (!m_events.contains(it.key())) {
            qCritical("Instance missing for adding the event there");
            return;
        }
    }

    // create messages are the first event of created instances
    if (event->entityType() == msc::MscEntity::EntityType::Create) {
        if (MscMessage *message = static_cast<MscMessage *>(event)) {
            if (MscInstance *createdInstance = message->targetInstance()) {
                createdInstance->setExplicitCreator(message->sourceInstance());
                instanceIndexes[createdInstance] = 0;
            }
            // check if source index needs to be adapted
            for (MscInstanceEvent *ev : m_events[message->sourceInstance()]) {
                if (MscMessage *msg = qobject_cast<MscMessage *>(ev)) {
                    if (msg->targetInstance() == message->targetInstance()) {
                        // put it before the message, as create has to be before first message to a created instance
                        instanceIndexes[msg->sourceInstance()] = m_events[msg->sourceInstance()].indexOf(msg);
                        break;
                    }
                }
            }
        }
    }

    // Add event
    for (auto it = instanceIndexes.begin(); it != instanceIndexes.end(); ++it) {
        int idx = it.value();
        QVector<MscInstanceEvent *> &list = m_events[it.key()];
        if (idx < 0 || idx >= list.size()) {
            idx = list.size();
        }
        list.insert(idx, event);
    }
    if (instanceIndexes.isEmpty()) {
        m_orphanEvents.append(event);
    }

    m_instanceEvents = allEvents();

    event->setParent(this);

    connect(event, &MscInstanceEvent::dataChanged, this, &MscChart::dataChanged);
    connect(event, &msc::MscInstanceEvent::instanceRelationChanged, this,
            [this](MscInstance *addedInstance, MscInstance *removedInstance) {
                if (auto ev = qobject_cast<MscInstanceEvent *>(sender())) {
                    msc::MscChart::eventInstanceChange(ev, addedInstance, removedInstance);
                }
            });

    if (event->entityType() == MscEntity::EntityType::Timer) {
        MscTimer *timer = static_cast<MscTimer *>(event);
        connect(timer, &MscTimer::instanceChanged, this, [this, timer]() { resetTimerRelations(timer); });
        connect(timer, &MscTimer::nameChanged, this, [this, timer]() { resetTimerRelations(timer); });
        resetTimerRelations(timer);
    }

    Q_EMIT instanceEventAdded(event);
    Q_EMIT instanceEventsChanged();
    Q_EMIT dataChanged();
}

void MscChart::setInstanceEvents(QHash<const MscInstance *, QVector<MscInstanceEvent *>> events)
{
    for (auto it = m_events.begin(); it != m_events.end(); ++it) {
        for (MscInstanceEvent *event : it.value()) {
            delete event;
        }
    }
    m_events.clear();

    m_events = events;
    m_instanceEvents = allEvents();

    for (auto it = m_events.begin(); it != m_events.end(); ++it) {
        for (MscInstanceEvent *event : it.value()) {
            event->setParent(this);

            connect(event, &MscInstanceEvent::dataChanged, this, &MscChart::dataChanged);
            connect(event, &msc::MscInstanceEvent::instanceRelationChanged, this,
                    [this](MscInstance *addedInstance, MscInstance *removedInstance) {
                        if (auto ev = qobject_cast<MscInstanceEvent *>(sender())) {
                            msc::MscChart::eventInstanceChange(ev, addedInstance, removedInstance);
                        }
                    });

            if (event->entityType() == MscEntity::EntityType::Timer) {
                MscTimer *timer = static_cast<MscTimer *>(event);
                connect(timer, &MscTimer::instanceChanged, this, [this, timer]() { resetTimerRelations(timer); });
                connect(timer, &MscTimer::nameChanged, this, [this, timer]() { resetTimerRelations(timer); });
                resetTimerRelations(timer);
            }
        }
    }

    Q_EMIT instanceEventsChanged();
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

    for (QVector<MscInstanceEvent *> &events : m_events) {
        events.removeOne(instanceEvent);
    }
    m_orphanEvents.removeAll(instanceEvent);

    if (!m_instanceEvents.contains(instanceEvent)) {
        return;
    }

    if (instanceEvent->entityType() == msc::MscEntity::EntityType::Create) {
        if (MscMessage *message = static_cast<MscMessage *>(instanceEvent)) {
            if (MscInstance *createdInstance = message->targetInstance())
                createdInstance->setExplicitCreator(nullptr);
        }
    }

    if (m_instanceEvents.removeAll(instanceEvent)) {
        if (instanceEvent->parent() == this) {
            instanceEvent->setParent(nullptr);
        }
        disconnect(instanceEvent, nullptr, this, nullptr);

        for (QVector<MscInstanceEvent *> evPerInstance : qAsConst(m_events)) {
            evPerInstance.removeOne(instanceEvent);
        }

        Q_EMIT instanceEventRemoved(instanceEvent);
        Q_EMIT instanceEventsChanged();
        Q_EMIT dataChanged();
    }
}

/*!
   Return the index (vertical) of the given event
 */
int MscChart::indexofEvent(MscInstanceEvent *instanceEvent) const
{
    return allEvents().indexOf(instanceEvent);
}

/*!
   Returns the psotion of an event for a given instance
   If the event is not part of the instance, -1 is returned
 */
int MscChart::indexofEventAtInstance(MscInstanceEvent *instanceEvent, MscInstance *instance) const
{
    if (!m_events.contains(instance)) {
        return -1;
    }

    return m_events[instance].indexOf(instanceEvent);
}

/*!
   Returns the first message with the given name
 */
MscMessage *MscChart::messageByName(const QString &name) const
{
    for (MscMessage *message : allEventsOfType<msc::MscMessage>()) {
        if (message->name() == name) {
            return message;
        }
    }
    return nullptr;
}

/*!
   Returns the first/top event of the given instance.
   Returns nullptr inca case the instance is invalid, or has no event
 */
MscInstanceEvent *MscChart::firstEventOfInstance(MscInstance *instance) const
{
    if (!instance) {
        return nullptr;
    }

    if (m_events.contains(instance)) {
        if (!m_events[instance].isEmpty()) {
            return m_events[instance].first();
        }
    }

    return nullptr;
}

/*!
   Returns the total number of events in this chart
 */
int MscChart::totalEventNumber() const
{
    return allEvents().size();
}

/*!
   Returns all messages of this chart
 */
QVector<MscMessage *> MscChart::messages() const
{
    return allEventsOfType<msc::MscMessage>();
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

bool MscChart::isEmpty() const
{
    return m_instances.isEmpty() && m_events.isEmpty() && m_orphanEvents.isEmpty();
}

void MscChart::updateInstanceOrder(MscInstance *instance, int pos)
{
    Q_ASSERT(instance);

    const int currPos = m_instances.indexOf(instance);
    Q_ASSERT(currPos != -1);

    if (currPos == pos)
        return;

    m_instances.move(currPos, pos);

    Q_EMIT instanceOrderChanged(instance, currPos, pos);
    Q_EMIT instancesChanged();
    Q_EMIT dataChanged();
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

void MscChart::updateCoregionPos(
        MscCoregion *regionBegin, MscCoregion *regionEnd, MscInstance *newInstance, int beginPos, int endPos)
{
    if (beginPos >= endPos || !regionBegin || !regionEnd || !newInstance) {
        return;
    }

    bool changed = setEventInstance(regionBegin, newInstance);
    changed |= setEventInstance(regionEnd, newInstance);
    changed |= moveEvent(regionBegin, beginPos);
    changed |= moveEvent(regionEnd, endPos);

    if (changed) {
        Q_EMIT eventMoved();
        Q_EMIT dataChanged();
    }
}

void MscChart::updateConditionPos(MscCondition *condition, MscInstance *newInstance, int eventPos)
{
    bool changed = false;
    if (!condition->shared()) {
        changed = setEventInstance(condition, newInstance);
    }
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
        resetTimerRelations(timer);
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
    if (event == nullptr || newIndex < 0 || newIndex >= m_instanceEvents.size()) {
        return false;
    }

    const int currentPos = m_instanceEvents.indexOf(event);
    if (newIndex == currentPos || currentPos < 0) {
        return false;
    }

    m_instanceEvents.removeAt(currentPos);

    // Move in list per instance
    for (MscInstance *instance : relatedInstances(event)) {
        Q_ASSERT(m_events.contains(instance));
        int newIdx = 0;
        m_events[instance].removeOne(event);
        bool inserted = false;
        for (MscInstanceEvent *ev : m_events[instance]) {
            const int otherIdx = m_instanceEvents.indexOf(ev);
            if (otherIdx >= newIndex) {
                m_events[instance].insert(newIdx, event);
                inserted = true;
                break;
            }
            newIdx++;
        }
        if (!inserted) {
            m_events[instance].append(event);
        }
    }

    // Move in global list
    m_instanceEvents.insert(newIndex, event);

    eventsCheck();

    return true;
}

/*!
   Does set the sorting of the events of this chart at once. Be careful to have the same set of events, as already
   available
 */
void MscChart::rearrangeEvents(const QVector<MscInstanceEvent *> &sortedEvents)
{
    if (sortedEvents == allEvents()) {
        return;
    }

    int idx = 0;
    bool changed = false;
    for (MscInstanceEvent *event : sortedEvents) {
        changed |= moveEvent(event, idx);
        idx++;
    }

    if (changed) {
        Q_EMIT eventMoved();
        Q_EMIT dataChanged();
    }
}

void MscChart::updateMessageTarget(
        MscMessage *message, MscInstance *newInstance, int eventPos, msc::MscMessage::EndType endType)
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

/*!
   Looks for messageInstanceName and timerInstanceName and if they are numbers, returns the highest number
   Returns 1 as a minimum. Even if nothing was found.
 */
int MscChart::maxInstanceNameNumber() const
{
    int num = 1;
    for (const MscInstanceEvent *event : allEvents()) {
        int nameNum = -1;
        if (event->entityType() == MscEntity::EntityType::Message) {
            auto msg = static_cast<const MscMessage *>(event);
            nameNum = msg->messageInstanceName().toInt();
        }
        if (event->entityType() == MscEntity::EntityType::Timer) {
            auto timer = static_cast<const MscTimer *>(event);
            nameNum = timer->timerInstanceName().toInt();
        }
        if (nameNum > num)
            num = nameNum;
    }
    return num;
}

/*!
   Sets instance names if needed, starting with the given number
   @return the next free number to be used for instance names

   For more documentation see \see MscModel::checkInstanceNames
 */
int MscChart::setInstanceNameNumbers(int nextNumber)
{
    QVector<MscMessage *> messages = allEventsOfType<MscMessage>();
    for (auto msg = messages.begin(); msg != messages.end(); ++msg) {
        for (auto msg2 = msg + 1; msg2 != messages.end(); ++msg2) {
            // now check if they are not unique
            if ((*msg)->name() == (*msg2)->name() && (*msg)->sourceInstance() == (*msg2)->sourceInstance()
                    && (*msg)->targetInstance() == (*msg2)->targetInstance()) {
                if ((*msg)->messageInstanceName().isEmpty())
                    (*msg)->setMessageInstanceName(QString::number(nextNumber++));
                if ((*msg2)->messageInstanceName().isEmpty())
                    (*msg2)->setMessageInstanceName(QString::number(nextNumber++));
            }
        }
    }
    //@todo check timer too ?
    return nextNumber;
}

void MscChart::resetTimerRelations(MscTimer *timer)
{
    MscTimer *precedingTimer = timer->precedingTimer();
    MscTimer *followingTimer = timer->followingTimer();
    if (precedingTimer)
        updateFollowingTimer(precedingTimer);
    if (followingTimer)
        updatePrecedingTimer(followingTimer);
    timer->setFollowingTimer(nullptr);
    timer->setPrecedingTimer(nullptr);

    const int idx = indexofEventAtInstance(timer, timer->instance());
    if (idx == -1)
        return;

    // Limit thes connection of stop/start timers to appropriate pairs only,
    // to be able to load that do not use timerInstanceName properly

    updateFollowingTimer(timer, idx);
    updatePrecedingTimer(timer, idx);
}

void MscChart::updatePrecedingTimer(MscTimer *timer, int idx)
{
    if (!m_events.contains(timer->instance())) {
        return;
    }

    const QVector<MscInstanceEvent *> events = m_events[timer->instance()];
    if (idx == -1) {
        idx = events.indexOf(timer);
    }

    if (idx != -1) {
        for (int timerIdx = idx - 1; timerIdx >= 0; --timerIdx) {
            MscInstanceEvent *event = events.value(timerIdx);
            if (event->entityType() != MscEntity::EntityType::Timer) {
                continue;
            }

            MscTimer *mscTimer = static_cast<MscTimer *>(event);
            if (mscTimer->fullName() == timer->fullName()) {
                timer->setPrecedingTimer(mscTimer);
                mscTimer->setFollowingTimer(timer);
                return;
            }
        }
    }

    timer->setPrecedingTimer(nullptr);
}

void MscChart::updateFollowingTimer(MscTimer *timer, int idx)
{
    if (!m_events.contains(timer->instance())) {
        return;
    }

    const QVector<MscInstanceEvent *> events = m_events[timer->instance()];
    if (idx == -1) {
        idx = events.indexOf(timer);
    }

    if (idx != -1) {
        for (int timerIdx = idx + 1; timerIdx < events.size(); ++timerIdx) {
            MscInstanceEvent *event = events.value(timerIdx);
            if (event->entityType() != MscEntity::EntityType::Timer) {
                continue;
            }

            MscTimer *mscTimer = static_cast<MscTimer *>(event);
            if (mscTimer->fullName() == timer->fullName()) {
                timer->setFollowingTimer(mscTimer);
                mscTimer->setPrecedingTimer(timer);
                return;
            }
        }
    }

    timer->setFollowingTimer(nullptr);
}

/*!
   \brief msc::MscChart::eventInstanceChange
   \param event
   \param addedInstance
   \param removedInstance
 */
void msc::MscChart::eventInstanceChange(
        msc::MscInstanceEvent *event, msc::MscInstance *addedInstance, msc::MscInstance *removedInstance)
{
    if (removedInstance) {
        if (m_events.contains(removedInstance)) {
            m_events[removedInstance].removeOne(event);
        }
    }

    if (addedInstance) {
        m_events[addedInstance].append(event);
    }
}

cif::CifBlockShared MscChart::cifMscDoc() const
{
    return parentDocument() ? parentDocument()->cifBlockByType(cif::CifLine::CifType::MscDocument)
                            : cif::CifBlockShared();
}

QVector<MscInstance *> MscChart::relatedInstances(MscInstanceEvent *event) const
{
    QVector<MscInstance *> result;
    switch (event->entityType()) {
    case MscEntity::EntityType::Action: {
        auto action = static_cast<MscAction *>(event);
        if (action->instance()) {
            result.append(action->instance());
        }
        break;
    }
    case MscEntity::EntityType::Condition: {
        auto condition = static_cast<MscCondition *>(event);
        if (condition->shared()) {
            return m_instances;
        } else {
            if (condition->instance()) {
                result.append(condition->instance());
            }
        }
        break;
    }
    case MscEntity::EntityType::Coregion: {
        auto coregion = static_cast<MscCoregion *>(event);
        if (coregion->instance()) {
            result.append(coregion->instance());
        }
        break;
    }
    case MscEntity::EntityType::Create:
    case MscEntity::EntityType::Message: {
        auto message = static_cast<MscMessage *>(event);
        if (message->sourceInstance()) {
            result.append(message->sourceInstance());
        }
        if (message->targetInstance()) {
            result.append(message->targetInstance());
        }
        break;
    }
    case MscEntity::EntityType::Timer: {
        auto timer = static_cast<MscTimer *>(event);
        if (timer->instance()) {
            result.append(timer->instance());
        }
        break;
    }
    default:
        break;
    }

    return result;
}

bool MscChart::eventsCheck() const
{
    for (MscInstance *inst : m_instances) {
        if (m_events[inst] != eventsForInstance(inst)) {
            return false;
        }
    }
    return true;
}

// Used for function MscChart::allEvents()
void addTopUntil(MscInstance *instance, MscInstanceEvent *untilEvent, const QVector<MscInstance *> &instances,
        const QHash<const MscInstance *, QVector<MscInstanceEvent *>> &events, QVector<MscInstanceEvent *> &result,
        MscInstanceEvent *noRecurEvent)
{
    if (!events.contains(instance)) {
        return;
    }

    for (MscInstanceEvent *event : events[instance]) {
        if (!result.contains(event)) {
            QVector<MscInstance *> otherInstances = instances;
            otherInstances.removeAll(instance);
            // check depending on type if other stacks need to be added before
            if (event->entityType() == MscEntity::EntityType::Message) {
                auto message = static_cast<MscMessage *>(event);
                if (message->sourceInstance() != nullptr && message->sourceInstance() != instance
                        && instances.contains(message->sourceInstance()) && event != noRecurEvent) {
                    addTopUntil(message->sourceInstance(), event, otherInstances, events, result, event);
                }
                if (message->targetInstance() != nullptr && message->targetInstance() != instance
                        && instances.contains(message->targetInstance()) && event != noRecurEvent) {
                    addTopUntil(message->targetInstance(), event, otherInstances, events, result, event);
                }
            }
            if (event->entityType() == MscEntity::EntityType::Condition) {
                auto condition = static_cast<MscCondition *>(event);
                if (condition->shared()) {
                    for (MscInstance *inst : qAsConst(otherInstances)) {
                        addTopUntil(inst, event, otherInstances, events, result, noRecurEvent);
                    }
                }
            }
            if (event->entityType() == MscEntity::EntityType::Create) {
                auto create = static_cast<MscMessage *>(event);
                if (create->targetInstance() != nullptr && create->targetInstance() == instance) {
                    addTopUntil(create->targetInstance(), event, otherInstances, events, result, event);
                }
            }

            if (!result.contains(event)) {
                result.append(event);
            }
            if (event == untilEvent) {
                return;
            }
        }
    }
};

/*!
   Creates a sorted list of events from the event list per instance
 */
QVector<MscInstanceEvent *> MscChart::allEvents() const
{
    if (m_instances.isEmpty()) {
        return m_orphanEvents;
    }

    QVector<MscInstanceEvent *> result;
    for (MscInstance *instance : m_instances) {
        addTopUntil(instance, nullptr, m_instances, m_events, result, nullptr);
    }
    result.append(m_orphanEvents);

    return result;
}

QRect MscChart::cifRect() const
{
    using namespace cif;

    if (const cif::CifBlockShared &cif = cifMscDoc())
        if (cif->hasPayloadFor(CifLine::CifType::MscDocument)) {
            const QVector<QPoint> &points = cif->payload(CifLine::CifType::MscDocument).value<QVector<QPoint>>();
            if (points.size() == 2) {
                const QPoint &pos = points.first();
                const QPoint &size = points.last();
                return { pos.x(), pos.y(), size.x(), size.y() };
            }
        }
    return {};
}

void MscChart::setCifRect(const QRect &rect)
{
    using namespace cif;

    if (MscDocument *pDoc = parentDocument()) {

        CifBlockShared mscDocCif = cifMscDoc();
        if (!mscDocCif) {
            mscDocCif = CifBlockFactory::createBlockMscDocument();
            pDoc->addCif(mscDocCif);
        }

        if (!mscDocCif->hasPayloadFor(CifLine::CifType::MscDocument)) {
            mscDocCif->addLine(CifLineShared(new CifLineMscDocument()));
        }

        const QVector<QPoint> points { rect.topLeft(), { rect.width(), rect.height() } };
        mscDocCif->setPayload(QVariant::fromValue(points), CifLine::CifType::MscDocument);
        Q_EMIT rectChanged();
        Q_EMIT dataChanged(); // update MscTextView
    } else {
        qWarning() << "CIF is not supported for \"orphan\" MSC (one that is not enclosed in MSCDOCUMENT)";
    }
}

} // namespace msc
