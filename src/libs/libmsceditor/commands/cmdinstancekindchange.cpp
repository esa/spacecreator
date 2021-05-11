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

#include "cmdinstancekindchange.h"

#include "commandids.h"
#include "mscinstance.h"

namespace msc {
namespace cmd {

CmdInstanceKindChange::CmdInstanceKindChange(
        MscInstance *item, const QString &newKind, ChartLayoutManager *layoutManager)
    : ChartBaseCommand(item, layoutManager)
    , m_instance(item)
    , m_oldValue(item->denominatorAndKind())
    , m_newValue(newKind)
{
}

void CmdInstanceKindChange::redo()
{
    if (m_instance) {
        m_instance->setDenominatorAndKind(m_newValue);
    }
}

void CmdInstanceKindChange::undo()
{
    if (m_instance) {
        m_instance->setDenominatorAndKind(m_oldValue);
    }
}

bool CmdInstanceKindChange::mergeWith(const QUndoCommand *command)
{
    auto other = dynamic_cast<const CmdInstanceKindChange *>(command);
    if (canMergeWith(other)) {
        m_newValue = other->m_newValue;
        return true;
    }

    return false;
}

int CmdInstanceKindChange::id() const
{
    return msc::cmd::Id::RenameInstanceKind;
}

} // namespace cmd
} // namespace msc
