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
#include "commands/cmdfunctionimplementationinsert.h"
#include "commands/cmdfunctionimplementationremove.h"
#include "commands/cmdfunctionimplementationupdate.h"
#include "commands/cmdifaceattrchange.h"
#include "ivfunction.h"
#include "ivobject.h"
#include "undocommand.h"

#include <QUndoStack>

namespace ive {
namespace cmd {

CommandsStack::CommandsStack(QObject *parent)
    : shared::cmd::CommandsStackBase(parent)
    , m_check(nullptr)
{
}

void CommandsStack::setAsn1Check(Asn1Acn::Asn1SystemChecks *check)
{
    m_check = check;
}

bool CommandsStack::push(QUndoCommand *command)
{
    if (!command) {
        return false;
    }

    if (auto fnCommand = dynamic_cast<CmdFunctionAttrChange *>(command)) {
        fnCommand->setAsn1SystemChecks(m_check);
        connect(fnCommand, &CmdFunctionAttrChange::asn1FilesImported, this, &CommandsStack::asn1FilesImported,
                Qt::UniqueConnection);
        connect(fnCommand, &CmdFunctionAttrChange::asn1FilesRemoved, this, &CommandsStack::asn1FileRemoved,
                Qt::UniqueConnection);
        connect(fnCommand, &CmdFunctionAttrChange::nameChanged, this, &CommandsStack::nameChanged,
                Qt::UniqueConnection);
        connect(fnCommand, &CmdFunctionAttrChange::defaultImplementationChanged, this,
                &CommandsStack::defaultImplementationChanged, Qt::UniqueConnection);
    }
    if (auto nameCommand = dynamic_cast<CmdIfaceAttrChange *>(command)) {
        connect(nameCommand, &CmdIfaceAttrChange::nameChanged, this, &CommandsStack::nameChanged, Qt::UniqueConnection);
    }
    if (auto nameCommand = dynamic_cast<CmdEntitiesRemove *>(command)) {
        connect(nameCommand, &CmdEntitiesRemove::entitiesRemoved, this, &CommandsStack::entitiesRemoved,
                Qt::UniqueConnection);
    }
    if (auto implCommand = dynamic_cast<CmdFunctionImplementationUpdate *>(command)) {
        connect(implCommand, &CmdFunctionImplementationUpdate::implementationChanged, this,
                &CommandsStack::implementationChanged, Qt::UniqueConnection);
    }
    if (auto implAdded = dynamic_cast<CmdFunctionImplementationInsert *>(command)) {
        connect(implAdded, &CmdFunctionImplementationInsert::implementationListChanged, this,
                &CommandsStack::implementationListChanged, Qt::UniqueConnection);
    }
    if (auto implAdded = dynamic_cast<CmdFunctionImplementationRemove *>(command)) {
        connect(implAdded, &CmdFunctionImplementationRemove::implementationListChanged, this,
                &CommandsStack::implementationListChanged, Qt::UniqueConnection);
    }
    if (auto attrCommand = dynamic_cast<shared::cmd::CmdEntityAttributesChange *>(command)) {
        connect(attrCommand, &shared::cmd::CmdEntityAttributesChange::attributeChanged, this,
                &CommandsStack::attributeChanged, Qt::UniqueConnection);
    }
    if (auto importCommand = dynamic_cast<ASN1ComponentsImport *>(command)) {
        connect(importCommand, &ASN1ComponentsImport::asn1FilesImported, this, &CommandsStack::asn1FilesImported,
                Qt::UniqueConnection);
        connect(importCommand, &ASN1ComponentsImport::asn1FilesRemoved, this, &CommandsStack::asn1FileRemoved,
                Qt::UniqueConnection);
    }
    m_undoStack->push(command);
    return true;
}

}
}
