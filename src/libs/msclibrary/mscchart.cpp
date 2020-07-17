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
            case MscEntity::EntityType::Action: {
                auto action = static_cast<MscAction *>(instanceEvent);
                if (action->instance() == instance)
                    events.append(instanceEvent);
                break;
            }
            case MscEntity::EntityType::Timer: {
                auto timer = static_cast<MscTimer *>(instanceEvent);
                if (timer->instance() == instance)
                    events.append(instanceEvent);
                break;
            }
            case MscEntity::EntityType::Coregion: {
                auto coregion = static_cast<MscCoregion *>(instanceEvent);
                if (coregion->relatesTo(instance))
                    events.append(instanceEvent);
                break;
            }
            case MscEntity::EntityType::Comment:
                break;
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
   @param instanceEvent The event (message/action/...) to add.
   @param eventIndex is the vertical position of the new event. If it tis < 0, then the new event is appended.
 */
void MscChart::addInstanceEvent(MscInstanceEvent *instanceEvent, int eventIndex)
{
    if (instanceEvent == nullptr) {
        return;
    }
    if (m_instanceEvents.contains(instanceEvent)) {
        return;
    }

    if (instanceEvent->entityType() == msc::MscEntity::EntityType::Create) {
        if (MscMessage *message = static_cast<MscMessage *>(instanceEvent)) {
            if (MscInstance *createdInstance = message->targetInstance())
                createdInstance->setExplicitCreator(message->sourceInstance());
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

    Q_EMIT instanceEventAdded(instanceEvent);
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
    const int currentPos = m_instanceEvents.indexOf(event);
    if (newIndex != currentPos && currentPos >= 0 && newIndex >= 0 && newIndex < m_instanceEvents.size()) {
        m_instanceEvents.removeAt(currentPos);
        m_instanceEvents.insert(newIndex, event);
        return true;
    }
    return false;
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

cif::CifBlockShared MscChart::cifMscDoc() const
{
    return parentDocument() ? parentDocument()->cifBlockByType(cif::CifLine::CifType::MscDocument)
                            : cif::CifBlockShared();
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
