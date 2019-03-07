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

#include "cmdmessageitemcreate.h"

#include "mscchart.h"
#include "mscmessage.h"

namespace msc {
namespace cmd {

CmdMessageItemCreate::CmdMessageItemCreate(msc::MscMessage *message, msc::MscChart *chart, int eventIndex)
    : BaseCommand(message)
    , m_message(message)
    , m_chart(chart)
    , m_eventIndex(eventIndex)
{
    Q_ASSERT(m_chart.data());

    setText(QObject::tr("Add message"));
}

void CmdMessageItemCreate::redo()
{
    Q_ASSERT(m_chart.data());

    if (!m_message) {
        m_message = new MscMessage(QObject::tr("Message_%1").arg(m_chart->instanceEvents().size()));
        m_modelItem = m_message;
    }

    // The chart takes over parent-/owner-ship
    m_chart->addInstanceEvent(m_message, m_eventIndex);
}

void CmdMessageItemCreate::undo()
{
    Q_ASSERT(m_chart.data());
    m_chart->removeInstanceEvent(m_message);

    // this command takes over ownership
    m_message->setParent(this);
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
