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

#include "cmdmessageitemresize.h"

#include "common/commandids.h"
#include "mscchart.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdMessageItemResize::CmdMessageItemResize(MscMessage *message, int newPos, MscInstance *newInsance,
        MscMessage::EndType endType, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(message, layoutManager)
    , m_message(message)
    , m_oldIndex(m_chart->instanceEvents().indexOf(message))
    , m_newIndex(newPos)
    , m_oldInstance(
              endType == msc::MscMessage::EndType::SOURCE_TAIL ? message->sourceInstance() : message->targetInstance())
    , m_newInstance(newInsance)
    , m_endType(endType)

{
    setText(QObject::tr("ReTarget message"));
}

void CmdMessageItemResize::redo()
{
    if (m_message && m_chart) {
        m_chart->updateMessageTarget(m_message, m_newInstance, m_newIndex, m_endType);
        checkVisualSorting();
    }
}

void CmdMessageItemResize::undo()
{
    if (m_message && m_chart) {
        m_chart->updateMessageTarget(m_message, m_oldInstance, m_oldIndex, m_endType);
        undoVisualSorting();
    }
}

bool CmdMessageItemResize::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdMessageItemResize::id() const
{
    return msc::cmd::Id::RetargetMessage;
}

} // ns cmd
} // ns msc
