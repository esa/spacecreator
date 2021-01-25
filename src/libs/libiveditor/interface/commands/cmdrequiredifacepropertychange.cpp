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

#include "cmdrequiredifacepropertychange.h"

#include "aadlconnection.h"
#include "cmdentityremove.h"
#include "commandids.h"
#include "commandsfactory.h"

namespace ive {
namespace cmd {

QVariant getCurrentProperty(const ivm::AADLIfaceRequired *entity, const QString &name)
{
    return (entity && !name.isEmpty()) ? entity->prop(name) : QVariant();
}

CmdRequiredIfacePropertyChange::CmdRequiredIfacePropertyChange(
        ivm::AADLIfaceRequired *entity, const QString &propName, const QVariant &value)
    : CmdIfaceDataChangeBase(entity, propName, value, getCurrentProperty(entity, propName))
{
    if (m_targetToken == ivm::meta::Props::Token::InheritPI) {
        const bool lostInheritance = !m_newValue.toBool();
        if (lostInheritance) {
            m_relatedConnections = getRelatedConnections();
            prepareRemoveConnectionCommands();
        }
    }
}

void CmdRequiredIfacePropertyChange::redo()
{
    switch (m_targetToken) {
    case ivm::meta::Props::Token::InheritPI: {
        setInheritPI(m_newValue.toBool());
        break;
    }
    default:
        m_iface->setProp(m_targetName, m_newValue);
        break;
    }
}

void CmdRequiredIfacePropertyChange::undo()
{
    switch (m_targetToken) {
    case ivm::meta::Props::Token::InheritPI: {
        setInheritPI(m_oldValue.toBool());
        break;
    }
    default:
        m_iface->setProp(m_targetName, m_oldValue);
        break;
    }
}

bool CmdRequiredIfacePropertyChange::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdRequiredIfacePropertyChange::id() const
{
    return ChangeRequiredIfaceProperty;
}

void CmdRequiredIfacePropertyChange::setInheritPI(bool nowInherited)
{
    if (!m_iface || !m_model)
        return;

    if (nowInherited) {
        m_iface->setProp(m_targetName, nowInherited);

        for (auto cmd : m_cmdRmConnection)
            cmd->undo();
        for (auto connection : m_relatedConnections)
            connection->setInheritPI();
    } else {
        for (auto connection : m_relatedConnections)
            connection->unsetInheritPI();
        for (auto cmd : m_cmdRmConnection)
            cmd->redo();

        m_iface->setProp(m_targetName, nowInherited);
    }
}

bool CmdRequiredIfacePropertyChange::connectionMustDie(const ivm::AADLConnection *connection) const
{
    const ivm::AADLIface *otherIface = getConnectionOtherSide(connection, m_iface);
    if (!otherIface) {
        Q_UNREACHABLE();
        return true;
    }

    const bool sameParams = m_iface->originalParams() == otherIface->params();
    if (!sameParams)
        return true;

    const ivm::AADLIface::OperationKind newKind = m_iface->kindFromString(
            m_iface->originalAttr(ivm::meta::Props::token(ivm::meta::Props::Token::kind)).toString());
    if (ivm::AADLIface::OperationKind::Cyclic == newKind) {
        Q_UNREACHABLE(); // m_iface is a RI
        return true;
    }

    if (ivm::AADLIface::OperationKind::Any != newKind
            && ivm::AADLIface::OperationKind::Any != otherIface->kind()) {
        return otherIface->kind() == newKind;
    }

    return false;
}

}
}
