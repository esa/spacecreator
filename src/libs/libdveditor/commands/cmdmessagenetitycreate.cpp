/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdmessagenetitycreate.h"

#include "commandids.h"
#include "dvconnection.h"
#include "dvmessage.h"
#include "dvmodel.h"

namespace dve {
namespace cmd {

CmdMessageEntityCreate::CmdMessageEntityCreate(
        dvm::DVConnection *connection, const QString &messageName, const QString &from, const QString &to)
    : shared::UndoCommand()
    , m_connection(connection)
    , m_message(new dvm::DVMessage(connection))
{
    m_message->setTitle(messageName);
    m_message->setFromFunction(from);
    m_message->setToFunction(to);
}

void CmdMessageEntityCreate::redo()
{
    m_message->setParent(m_connection);
    if (auto model = m_connection->model()) {
        model->addObject(m_message);
    }
}

void CmdMessageEntityCreate::undo()
{
    if (auto model = m_connection->model()) {
        model->removeObject(m_message);
    }
    m_message->setParent(this);
}

int CmdMessageEntityCreate::id() const
{
    return CreateMessageEntity;
}

} // namespace cmd
} // namespace dve
