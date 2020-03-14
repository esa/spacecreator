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

#include "cmdifaceattrchange.h"

#include "commandids.h"
#include "commandsfactory.h"

namespace taste3 {
namespace aadl {
namespace cmd {

static inline QVariant getCurrentAttribute(const AADLObjectIface *entity, const QString &name)
{
    return (entity && !name.isEmpty()) ? entity->attr(name) : QVariant();
}

CmdIfaceAttrChange::CmdIfaceAttrChange(AADLObjectIface *entity, const QString &attrName, const QVariant &value)
    : CmdIfaceDataChangeBase(entity, attrName, value, getCurrentAttribute(entity, attrName))
{
    if (m_targetToken == meta::Props::Token::kind) {
        m_relatedConnections = getRelatedConnections();
        prepareRemoveConnectionCommands();
    }
}

void CmdIfaceAttrChange::redo()
{
    switch (m_targetToken) {
    case meta::Props::Token::kind: {
        setKind(m_newValue);
        break;
    }
    default:
        m_iface->setAttr(m_targetName, m_newValue);
        break;
    }
}

void CmdIfaceAttrChange::undo()
{
    switch (m_targetToken) {
    case meta::Props::Token::kind: {
        setKind(m_oldValue);
        break;
    }
    default:
        m_iface->setAttr(m_targetName, m_oldValue);
        break;
    }
}

bool CmdIfaceAttrChange::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdIfaceAttrChange::id() const
{
    return ChangeIfaceAttribute;
}

void CmdIfaceAttrChange::setKind(const QVariant &kind)
{
    if (!m_iface || !m_model)
        return;

    if (kind == m_newValue)
        removeConnections();
    else
        restoreConnections();

    m_iface->setAttr(m_targetName, kind);
}

void CmdIfaceAttrChange::removeConnections()
{
    for (auto cmd : m_cmdRmConnection)
        cmd->redo();
}

void CmdIfaceAttrChange::restoreConnections()
{
    for (auto cmd : m_cmdRmConnection)
        cmd->undo();
}

bool CmdIfaceAttrChange::connectionMustDie(const AADLObjectConnection *connection) const
{
    const AADLObjectIface *otherIface = getConnectionOtherSide(connection, m_iface);
    if (!otherIface) {
        Q_UNREACHABLE();
        return false;
    }

    const AADLObjectIface::OperationKind newKind = m_iface->kindFromString(m_newValue.toString());
    if (AADLObjectIface::OperationKind::Cyclic == newKind)
        return true;
    if (AADLObjectIface::OperationKind::Any == newKind || AADLObjectIface::OperationKind::Any == otherIface->kind())
        return false;

    if (m_iface->direction() != otherIface->direction()) {
        auto isInheritsPI = [](const AADLObjectIface *iface) {
            if (iface && iface->isRequired())
                if (const auto ri = iface->as<const AADLObjectIfaceRequired *>())
                    return ri->isInheritPI();
            return false;
        };

        if (isInheritsPI(m_iface) || isInheritsPI(otherIface))
            return false;
    }

    return otherIface->kind() != newKind;
}

void CmdIfaceAttrChange::prepareRemoveConnectionCommands()
{
    for (const auto connection : m_relatedConnections) {
        if (connectionMustDie(connection)) {
            const QVariantList params = { QVariant::fromValue(connection), QVariant::fromValue(m_model.data()) };
            if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::RemoveEntity, params))
                m_cmdRmConnection.append(cmdRm);
        }
    }
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
