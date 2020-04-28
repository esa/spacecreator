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

namespace aadlinterface {
namespace cmd {

static inline QVariant getCurrentAttribute(const aadl::AADLObjectIface *entity, const QString &name)
{
    return (entity && !name.isEmpty()) ? entity->attr(name) : QVariant();
}

CmdIfaceAttrChange::CmdIfaceAttrChange(aadl::AADLObjectIface *entity, const QString &attrName, const QVariant &value)
    : CmdIfaceDataChangeBase(entity, attrName, value, getCurrentAttribute(entity, attrName))
{
    if (m_targetToken == aadl::meta::Props::Token::kind) {
        m_relatedConnections = getRelatedConnections();
        prepareRemoveConnectionCommands();
    }
}

void CmdIfaceAttrChange::redo()
{
    switch (m_targetToken) {
    case aadl::meta::Props::Token::kind: {
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
    case aadl::meta::Props::Token::kind: {
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

bool CmdIfaceAttrChange::connectionMustDie(const aadl::AADLObjectConnection *connection) const
{
    const aadl::AADLObjectIface *otherIface = getConnectionOtherSide(connection, m_iface);
    if (!otherIface) {
        Q_UNREACHABLE();
        return true;
    }

    const aadl::AADLObjectIface::OperationKind newKind = m_iface->kindFromString(m_newValue.toString());
    if (aadl::AADLObjectIface::OperationKind::Cyclic == newKind)
        return true;
    if (aadl::AADLObjectIface::OperationKind::Any == newKind || aadl::AADLObjectIface::OperationKind::Any == otherIface->kind())
        return false;

    if (!connection->isOneDirection()) {
        auto isInheritsPI = [](const aadl::AADLObjectIface *iface) {
            if (iface && iface->isRequired())
                if (const auto ri = iface->as<const aadl::AADLObjectIfaceRequired *>())
                    return ri->isInheritPI();
            return false;
        };

        if (isInheritsPI(m_iface) || isInheritsPI(otherIface))
            return false;
    }

    return otherIface->kind() != newKind;
}

}
}
