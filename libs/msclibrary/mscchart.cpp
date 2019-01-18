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
#include "msccondition.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscgate.h"
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

void MscChart::addInstance(MscInstance *instance)
{
    if (instance == nullptr) {
        return;
    }
    if (m_instances.contains(instance)) {
        return;
    }

    instance->setParent(this);
    m_instances.append(instance);
    connect(instance, &MscInstance::dataChanged, this, &MscChart::dataChanged);
    Q_EMIT instanceAdded(instance);
    Q_EMIT dataChanged();
}

void MscChart::removeInstance(MscInstance *instance)
{
    if (instance == nullptr) {
        return;
    }
    if (!m_instances.contains(instance)) {
        return;
    }

    if (m_instances.removeAll(instance)) {
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
                continue;
            case MscEntity::EntityType::Message: {
                auto message = static_cast<MscMessage *>(instanceEvent);
                if (message->sourceInstance() == instance || message->targetInstance() == instance)
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

void MscChart::addInstanceEvent(MscInstanceEvent *instanceEvent)
{
    if (instanceEvent == nullptr) {
        return;
    }
    if (m_instanceEvents.contains(instanceEvent)) {
        return;
    }

    instanceEvent->setParent(this);
    m_instanceEvents.append(instanceEvent);
    connect(instanceEvent, &MscInstanceEvent::dataChanged, this, &MscChart::dataChanged);
    Q_EMIT instanceEventAdded(instanceEvent);
    Q_EMIT dataChanged();
}

void MscChart::removeInstanceEvent(MscInstanceEvent *instanceEvent)
{
    if (instanceEvent == nullptr) {
        return;
    }
    if (!m_instanceEvents.contains(instanceEvent)) {
        return;
    }

    if (m_instanceEvents.removeAll(instanceEvent)) {
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

void MscChart::addGate(MscGate *gate)
{
    if (!gate)
        return;

    if (gates().contains(gate))
        return;

    m_gates.append(gate);
    connect(gate, &MscGate::dataChanged, this, &MscChart::dataChanged);
    Q_EMIT gateAdded(gate);
    Q_EMIT dataChanged();
}

void MscChart::removeGate(MscGate *gate)
{
    if (!gate)
        return;
    if (!m_gates.contains(gate))
        return;

    if (m_gates.removeAll(gate)) {
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
}

} // namespace msc
