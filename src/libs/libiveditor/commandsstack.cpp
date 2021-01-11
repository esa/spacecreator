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

#include "commandsstack.h"

#include "aadlobject.h"
#include "interface/commands/cmdentityattributechange.h"
#include "interface/commands/cmdentityremove.h"
#include "interface/commands/cmdifaceattrchange.h"
#include "undocommand.h"

#include <QUndoStack>

namespace aadlinterface {
namespace cmd {

bool CommandsStack::push(QUndoCommand *command)
{
    if (command && CommandsStack::current()) {
        if (auto nameCommand = dynamic_cast<CmdEntityAttributeChange *>(command)) {
            connect(nameCommand, &CmdEntityAttributeChange::nameChanged, static_cast<CommandsStack *>(instance()),
                    &CommandsStack::nameChanged, Qt::UniqueConnection);
        }
        if (auto nameCommand = dynamic_cast<CmdIfaceAttrChange *>(command)) {
            connect(nameCommand, &CmdIfaceAttrChange::nameChanged, static_cast<CommandsStack *>(instance()),
                    &CommandsStack::nameChanged, Qt::UniqueConnection);
        }
        if (auto nameCommand = dynamic_cast<CmdEntityRemove *>(command)) {
            connect(nameCommand, &CmdEntityRemove::entityRemoved, static_cast<CommandsStack *>(instance()),
                    &CommandsStack::entityRemoved, Qt::UniqueConnection);
        }
        CommandsStack::current()->push(command);
        return true;
    }

    return false;
}

}
}
