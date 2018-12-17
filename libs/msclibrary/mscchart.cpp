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
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscgate.h"
#include "msctimer.h"

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

    m_instances.append(instance);
    Q_EMIT instanceAdded(instance);
}

void MscChart::removeInstance(MscInstance *instance)
{
    if (instance == nullptr) {
        return;
    }
    if (!m_instances.contains(instance)) {
        return;
    }

    if (m_instances.removeAll(instance))
        Q_EMIT instanceRemoved(instance);
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

void MscChart::addInstanceEvent(MscInstanceEvent *instanceEvent)
{
    if (instanceEvent == nullptr) {
        return;
    }
    if (m_instanceEvents.contains(instanceEvent)) {
        return;
    }

    m_instanceEvents.append(instanceEvent);
    Q_EMIT instanceEventAdded(instanceEvent);
}

void MscChart::removeInstanceEvent(MscInstanceEvent *instanceEvent)
{
    if (instanceEvent == nullptr) {
        return;
    }
    if (!m_instanceEvents.contains(instanceEvent)) {
        return;
    }

    if (m_instanceEvents.removeAll(instanceEvent))
        Q_EMIT instanceEventRemoved(instanceEvent);
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
    Q_EMIT gateAdded(gate);
}

void MscChart::removeGate(MscGate *gate)
{
    if (!gate)
        return;
    if (!m_gates.contains(gate))
        return;

    if (m_gates.removeAll(gate))
        Q_EMIT gateRemoved(gate);
}

/*!
  \brief MscChart::parentDocument
  \return The document that this chart is included in. Returns a nullptr if this chart is at the root
  of the model
*/
MscDocument *MscChart::parentDocument() const
{
    return dynamic_cast<MscDocument *>(parent());
}

MscEntity::EntityType MscChart::entityType() const
{
    return MscEntity::EntityType::Chart;
}

} // namespace msc
