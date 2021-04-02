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

const QVector<MscInstanceEvent *> &MscChart::instanceEvents() const
{
    return m_instanceEvents;
}

QVector<MscInstanceEvent *> MscChart::eventsForInstance(const MscInstance *instance) const
{
    if (!instance) {
        return {};
    }

    QVector<MscInstanceEvent *> events;
    for (MscInstanceEvent *instanceEvent : instanceEvents()) {
        if (auto event = qobject_cast<MscInstanceEvent *>(instanceEvent)) {
            if (event->relatesTo(instance)) {
                events.append(instanceEvent);
            }
        }
    }

    if (events != m_events.value(instance)) {
        qWarning() << "Missmatch in events per instance! :(";
        qFatal("Missmatch in events");
    }

    return events;
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

    if (instanceEvent->entityType() == MscEntity::EntityType::Timer) {
        MscTimer *timer = static_cast<MscTimer *>(instanceEvent);
        connect(timer, &MscTimer::instanceChanged, this, [this, timer]() { resetTimerRelations(timer); });
        connect(timer, &MscTimer::nameChanged, this, [this, timer]() { resetTimerRelations(timer); });
        resetTimerRelations(timer);
    }

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
    connect(instanceEvent, &msc::MscInstanceEvent::instanceRelationChanged, this,
            [this](MscInstance *addedInstance, MscInstance *removedInstance) {
                if (auto ev = qobject_cast<MscInstanceEvent *>(sender())) {
                    msc::MscChart::eventInstanceChange(ev, addedInstance, removedInstance);
                }
            });

    eventsCheck();

    Q_EMIT instanceEventAdded(instanceEvent);
    Q_EMIT instanceEventsChanged();
    Q_EMIT dataChanged();

    return m_instanceEvents.indexOf(instanceEvent);
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
    return m_instanceEvents.indexOf(instanceEvent);
}

/*!
   Returns the first message with the given name
 */
MscMessage *MscChart::messageByName(const QString &name) const
{
    for (MscInstanceEvent *message : m_instanceEvents) {
        if (message->entityType() == MscEntity::EntityType::Message && message->name() == name) {
            return static_cast<MscMessage *>(message);
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

    for (MscInstanceEvent *event : m_instanceEvents) {
        if (event->relatesTo(instance)) {
            return event;
        }
    }

    return nullptr;
}

/*!
   Returns all messages of this chart
 */
QVector<MscMessage *> MscChart::messages() const
{
    QVector<MscMessage *> result;
    for (MscInstanceEvent *event : m_instanceEvents) {
        if (event->entityType() == msc::MscEntity::EntityType::Message) {
            result.append(static_cast<msc::MscMessage *>(event));
        }
    }
    return result;
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
    return m_instances.isEmpty() && m_instanceEvents.isEmpty();
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
    if (sortedEvents == m_instanceEvents) {
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
    for (const MscInstanceEvent *event : m_instanceEvents) {
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
    QVector<MscMessage *> messages = allEvents<MscMessage>();
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

    const int idx = m_instanceEvents.indexOf(timer);
    if (idx == -1)
        return;

    // Limit thes connection of stop/start timers to appropriate pairs only,
    // to be able to load that do not use timerInstanceName properly

    updateFollowingTimer(timer, idx);
    updatePrecedingTimer(timer, idx);
}

void MscChart::updatePrecedingTimer(MscTimer *timer, int idx)
{
    if (idx == -1)
        idx = m_instanceEvents.indexOf(timer);

    if (idx != -1) {
        for (int timerIdx = idx - 1; timerIdx >= 0; --timerIdx) {
            MscInstanceEvent *event = m_instanceEvents.value(timerIdx);
            if (event->entityType() != MscEntity::EntityType::Timer)
                continue;

            MscTimer *mscTimer = static_cast<MscTimer *>(event);
            if (mscTimer->fullName() == timer->fullName() && mscTimer->instance() == timer->instance()) {
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
    if (idx == -1)
        idx = m_instanceEvents.indexOf(timer);

    if (idx != -1) {
        for (int timerIdx = idx + 1; timerIdx < m_instanceEvents.size(); ++timerIdx) {
            MscInstanceEvent *event = m_instanceEvents.value(timerIdx);
            if (event->entityType() != MscEntity::EntityType::Timer)
                continue;

            MscTimer *mscTimer = static_cast<MscTimer *>(event);
            if (mscTimer->fullName() == timer->fullName() && mscTimer->instance() == timer->instance()) {
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
