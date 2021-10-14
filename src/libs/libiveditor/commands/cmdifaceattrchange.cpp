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
#include "ivconnection.h"

namespace ive {
namespace cmd {

static inline QVariant getCurrentAttribute(const ivm::IVInterface *entity, const QString &name)
{
    return (entity && !name.isEmpty()) ? entity->entityAttributeValue(name) : QVariant();
}

CmdIfaceAttrChange::CmdIfaceAttrChange(shared::PropertyTemplateConfig *config, ivm::IVInterface *entity,
        const QString &attrName, const QVariant &value)
    : CmdIfaceDataChangeBase(config, entity, attrName, value, getCurrentAttribute(entity, attrName))
{
    if (m_targetToken == ivm::meta::Props::Token::kind) {
        m_relatedConnections = getRelatedConnections();
        prepareRemoveConnectionCommands();
    }
}

void CmdIfaceAttrChange::redo()
{
    switch (m_targetToken) {
    case ivm::meta::Props::Token::kind: {
        setKind(m_newValue);
        break;
    }
    default:
        break;
    }

    CmdIfaceDataChangeBase::redo();

    if (m_targetToken == ivm::meta::Props::Token::name) {
        Q_EMIT nameChanged(m_iface, m_oldValue.toString(), this);
    }
    m_firstRedo = false;
}

void CmdIfaceAttrChange::undo()
{
    CmdIfaceDataChangeBase::undo();

    switch (m_targetToken) {
    case ivm::meta::Props::Token::kind: {
        setKind(m_oldValue);
        break;
    }
    default:
        break;
    }

    if (m_targetToken == ivm::meta::Props::Token::name) {
        Q_EMIT nameChanged(m_iface, m_newValue.toString(), this);
    }
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
}

void CmdIfaceAttrChange::removeConnections()
{
    for (auto cmd : qAsConst(m_cmdRmConnection))
        cmd->redo();
}

void CmdIfaceAttrChange::restoreConnections()
{
    for (auto cmd : qAsConst(m_cmdRmConnection))
        cmd->undo();
}

bool CmdIfaceAttrChange::connectionMustDie(const ivm::IVConnection *connection) const
{
    const ivm::IVInterface *otherIface = getConnectionOtherSide(connection, m_iface);
    if (!otherIface) {
        Q_UNREACHABLE();
        return true;
    }

    const ivm::IVInterface::OperationKind newKind = m_iface->kindFromString(m_newValue.toString());
    return ivm::IVInterface::OperationKind::Cyclic == newKind;
}

} // namespace cmd
} // namespace ive
