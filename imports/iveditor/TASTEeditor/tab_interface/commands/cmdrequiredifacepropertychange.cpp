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

#include "cmdentityremove.h"
#include "commandids.h"
#include "commandsfactory.h"

namespace taste3 {
namespace aadl {
namespace cmd {

QVariant getCurrentProperty(const AADLObjectIfaceRequired *entity, const QString &name)
{
    return (entity && !name.isEmpty()) ? entity->prop(name) : QVariant();
}

CmdRequiredIfacePropertyChange::CmdRequiredIfacePropertyChange(AADLObjectIfaceRequired *entity, const QString &propName,
                                                               const QVariant &value)
    : CmdIfaceDataChangeBase(entity, propName, value, getCurrentProperty(entity, propName))
{
    if (m_targetToken == meta::Props::Token::InheritPI) {
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
    case meta::Props::Token::InheritPI: {
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
    case meta::Props::Token::InheritPI: {
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

bool CmdRequiredIfacePropertyChange::connectionMustDie(const AADLObjectConnection *connection) const
{
    const AADLObjectIface *otherIface = getConnectionOtherSide(connection, m_iface);
    if (!otherIface) {
        Q_UNREACHABLE();
        return true;
    }

    const bool sameParams = m_iface->originalParams() == otherIface->params();
    if (!sameParams)
        return true;

    const AADLObjectIface::OperationKind newKind =
            m_iface->kindFromString(m_iface->originalAttr(meta::Props::token(meta::Props::Token::kind)).toString());
    if (AADLObjectIface::OperationKind::Cyclic == newKind) {
        Q_UNREACHABLE(); // m_iface is a RI
        return true;
    }

    if (AADLObjectIface::OperationKind::Any != newKind && AADLObjectIface::OperationKind::Any != otherIface->kind()) {
        return otherIface->kind() == newKind;
    }

    return false;
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
