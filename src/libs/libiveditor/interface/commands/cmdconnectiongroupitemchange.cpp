/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdconnectiongroupitemchange.h"
#include "commandids.h"
#include "aadlobjectconnection.h"
#include "aadlobjectconnectiongroup.h"

namespace ive {
namespace cmd {


CmdConnectionGroupItemChange::CmdConnectionGroupItemChange(aadl::AADLObjectConnectionGroup *group,
                                                           aadl::AADLObjectConnection *connection,
                                                           bool add)
    : QUndoCommand()
    , m_group(group)
    , m_connection(connection)
    , m_addConnection(add)
{
    Q_ASSERT(connection);
    Q_ASSERT(group);
}

void CmdConnectionGroupItemChange::redo()
{
    if (m_addConnection) {
        m_group->addConnection(m_connection);
    } else {
        m_group->removeConnection(m_connection);
    }
}

void CmdConnectionGroupItemChange::undo()
{
    if (m_addConnection) {
        m_group->removeConnection(m_connection);
    } else {
        m_group->addConnection(m_connection);
    }
}

int CmdConnectionGroupItemChange::id() const
{
    return CreateConnectionGroupEntity;
}

} // namespace ive
} // namespace cmd
