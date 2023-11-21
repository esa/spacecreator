/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdupdateentityrequirements.h"

#include "commandids.h"
#include "mscentity.h"

namespace msc {
namespace cmd {

/*!
 * \param item The entity the requirements are linked to
 * \param requirements A list of IDs of the requirements to be linked to the entity
 */
CmdUpdateEntityRequirements::CmdUpdateEntityRequirements(MscEntity *item, const QByteArrayList &requirements)
    : msc::cmd::BaseCommand(item)
    , m_newRequirements(requirements)
    , m_oldRequirements(item->requirements())
{
}

void CmdUpdateEntityRequirements::redo()
{
    m_modelItem->setRequirements(m_newRequirements);
}

void CmdUpdateEntityRequirements::undo()
{
    m_modelItem->setRequirements(m_oldRequirements);
}

bool CmdUpdateEntityRequirements::mergeWith(const QUndoCommand *command)
{
    if (id() == command->id()) {
        auto other = static_cast<const CmdUpdateEntityRequirements *>(command);
        m_oldRequirements = other->m_oldRequirements;
        return true;
    }

    return false;
}

int CmdUpdateEntityRequirements::id() const
{
    return Id::UpdateEntityRequirements;
}

} // namespace cmd
} // namespace msc
