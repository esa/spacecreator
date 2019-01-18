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

#include "cmdmessageitemcreate.h"

#include <mscchart.h>
#include <mscmessage.h>

namespace msc {
namespace cmd {

CmdMessageItemCreate::CmdMessageItemCreate(msc::MscMessage *message, msc::MscChart *chart)
    : BaseCommand(message)
    , m_message(message)
    , m_chart(chart)
{
    Q_ASSERT(m_chart.data());

    setText(QObject::tr("Add message"));
}

CmdMessageItemCreate::~CmdMessageItemCreate()
{
    // Delete the message item if we are the owner
    if (m_message && m_message->parent() != nullptr) {
        delete m_message;
        m_message = nullptr;
    }
}

void CmdMessageItemCreate::redo()
{
    Q_ASSERT(m_chart.data());

    if (!m_message) {
        m_message = new MscMessage(QObject::tr("Message_%1").arg(m_chart->instanceEvents().size()));
        m_modelItem = m_message;
    }
    if (!m_message->sourceInstance() && !m_message->targetInstance() && !m_chart->instances().empty()) {
        m_message->setSourceInstance(m_chart->instances().at(0));
    }
    m_chart->addInstanceEvent(m_message);
}

void CmdMessageItemCreate::undo()
{
    Q_ASSERT(m_chart.data());
    m_chart->removeInstanceEvent(m_message);

    // Having to parent means, this command takes over ownership
    m_message->setParent(nullptr);
}

bool CmdMessageItemCreate::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdMessageItemCreate::id() const
{
    return msc::cmd::Id::CreateMessage;
}

} // ns cmd
} // ns msc
