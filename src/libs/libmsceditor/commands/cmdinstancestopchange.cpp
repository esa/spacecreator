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

#include "cmdinstancestopchange.h"

#include "commandids.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdInstanceStopChange::CmdInstanceStopChange(msc::MscInstance *item, bool newValue)
    : ChartBaseCommand(item, nullptr)
    , m_instance(item)
    , m_newExStop(newValue)
{
}

void CmdInstanceStopChange::redo()
{
    if (m_instance) {
        m_instance->setExplicitStop(m_newExStop);
    }
}

void CmdInstanceStopChange::undo()
{
    if (m_instance) {
        m_instance->setExplicitStop(!m_newExStop);
    }
}

bool CmdInstanceStopChange::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdInstanceStopChange::id() const
{
    return msc::cmd::Id::StopInstance;
}

} // ns cmd
} // ns msc
