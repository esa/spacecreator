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

#include "commands/cmdentitiesimport.h"
#include "commands/cmdentitiesremove.h"
#include "commands/cmdfunctionattrchange.h"
#include "commands/cmdfunctionlanguageinsert.h"
#include "commands/cmdfunctionlanguageremove.h"
#include "commands/cmdfunctionlanguageupdate.h"
#include "commands/cmdifaceattrchange.h"
#include "ivfunction.h"
#include "ivobject.h"
#include "undocommand.h"

#include <QUndoStack>

namespace ive {
namespace cmd {

CommandsStack::CommandsStack(QObject *parent)
    : shared::cmd::CommandsStackBase(parent)
{
}

bool CommandsStack::push(QUndoCommand *command)
{
    if (!command) {
        return false;
    }

    if (auto nameCommand = dynamic_cast<CmdFunctionAttrChange *>(command)) {
        connect(nameCommand, &CmdFunctionAttrChange::nameChanged, this, &CommandsStack::nameChanged,
                Qt::UniqueConnection);
        connect(nameCommand, &CmdFunctionAttrChange::defaultImplementationChanged, this,
                &CommandsStack::defaultImplementationChanged, Qt::UniqueConnection);
    }
    if (auto nameCommand = dynamic_cast<CmdIfaceAttrChange *>(command)) {
        connect(nameCommand, &CmdIfaceAttrChange::nameChanged, this, &CommandsStack::nameChanged, Qt::UniqueConnection);
    }
    if (auto nameCommand = dynamic_cast<CmdEntitiesRemove *>(command)) {
        connect(nameCommand, &CmdEntitiesRemove::entitiesRemoved, this, &CommandsStack::entitiesRemoved,
                Qt::UniqueConnection);
    }
    if (auto implCommand = dynamic_cast<CmdFunctionLanguageUpdate *>(command)) {
        connect(implCommand, &CmdFunctionLanguageUpdate::implementationChanged, this,
                &CommandsStack::implementationChanged, Qt::UniqueConnection);
    }
    if (auto implAdded = dynamic_cast<CmdFunctionLanguageInsert *>(command)) {
        connect(implAdded, &CmdFunctionLanguageInsert::implementationListChanged, this,
                &CommandsStack::implementationListChanged, Qt::UniqueConnection);
    }
    if (auto implAdded = dynamic_cast<CmdFunctionLanguageRemove *>(command)) {
        connect(implAdded, &CmdFunctionLanguageRemove::implementationListChanged, this,
                &CommandsStack::implementationListChanged, Qt::UniqueConnection);
    }
    if (auto attrCommand = dynamic_cast<shared::cmd::CmdEntityAttributesChange *>(command)) {
        connect(attrCommand, &shared::cmd::CmdEntityAttributesChange::attributeChanged, this,
                &CommandsStack::attributeChanged, Qt::UniqueConnection);
    }
    if (auto importCommand = dynamic_cast<CmdEntitiesImport *>(command)) {
        connect(importCommand, &CmdEntitiesImport::asn1FilesImported, this, &CommandsStack::asn1FilesImported,
                Qt::UniqueConnection);
        connect(importCommand, &CmdEntitiesImport::asn1FileRemoved, this, &CommandsStack::asn1FileRemoved,
                Qt::UniqueConnection);
    }

    m_undoStack->push(command);
    return true;
}

}
}
