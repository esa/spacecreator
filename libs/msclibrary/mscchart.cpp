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

namespace msc {

MscChart::MscChart(QObject *parent)
    : MscElement(parent)
{
}

MscChart::MscChart(const QString &name, QObject *parent)
    : MscElement(name, parent)
{
}

MscChart::~MscChart()
{
    qDeleteAll(m_instances);
    m_instances.clear();

    qDeleteAll(m_messages);
    m_messages.clear();
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

    m_instances.removeAll(instance);
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

const QVector<MscMessage *> &MscChart::messages() const
{
    return m_messages;
}

void MscChart::addMessage(MscMessage *message)
{
    if (message == nullptr) {
        return;
    }
    if (m_messages.contains(message)) {
        return;
    }

    m_messages.append(message);
    Q_EMIT messageAdded(message);
}

void MscChart::removeMessage(MscMessage *message)
{
    if (message == nullptr) {
        return;
    }
    if (!m_messages.contains(message)) {
        return;
    }

    m_messages.removeAll(message);
    Q_EMIT messageRemoved(message);
}

MscMessage *MscChart::messageByName(const QString &name) const
{
    for (MscMessage *message : m_messages) {
        if (message->name() == name) {
            return message;
        }
    }
    return nullptr;
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

} // namespace msc
