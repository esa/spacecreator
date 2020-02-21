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

#include "commandids.h"
#include "commandsfactory.h"

namespace taste3 {
namespace aadl {
namespace cmd {

static inline QVariant getCurrentProperty(AADLObjectIfaceRequired *entity, const QString &name)
{
    return (entity && !name.isEmpty()) ? entity->prop(name) : QVariant();
}

static inline QVector<AADLObjectConnection *> getAffectedConnections(const AADLObjectIfaceRequired *ri)
{
    QVector<AADLObjectConnection *> affected;

    if (!ri)
        return affected;

    const AADLObjectsModel *model = ri->objectsModel();
    if (!model)
        return affected;

    for (AADLObjectConnection *connection : model->getConnectionsForIface(ri->id()))
        if (const AADLObjectIfaceProvided *pi = connection->selectIface<const AADLObjectIfaceProvided *>())
            affected.append(connection);

    return affected;
}

CmdRequiredIfacePropertyChange::CmdRequiredIfacePropertyChange(AADLObjectIfaceRequired *entity, const QString &propName,
                                                               const QVariant &value)
    : QUndoCommand()
    , m_ri(entity)
    , m_model(m_ri ? m_ri->objectsModel() : nullptr)
    , m_affectedConenctions(getAffectedConnections(m_ri))
    , m_propertyName(propName)
    , m_propertyToken(meta::Props::token(m_propertyName))
    , m_oldValue(getCurrentProperty(entity, m_propertyName))
    , m_newValue(value)
{
    setText(QObject::tr("Change RI inheritance"));

    const bool lostInheritance = !m_newValue.toBool();
    if (lostInheritance)
        prepareRemoveConnectionCommands();
}

CmdRequiredIfacePropertyChange::~CmdRequiredIfacePropertyChange()
{
    qDeleteAll(m_cmdRmConnection);
}

void CmdRequiredIfacePropertyChange::redo()
{
    switch (m_propertyToken) {
    case meta::Props::Token::labelInheritance: {
        updateConnections(m_newValue);
        break;
    }
    default:
        m_ri->setProp(m_propertyName, m_newValue);
        break;
    }
}

void CmdRequiredIfacePropertyChange::undo()
{
    switch (m_propertyToken) {
    case meta::Props::Token::labelInheritance: {
        updateConnections(m_oldValue);
        break;
    }
    default:
        m_ri->setProp(m_propertyName, m_oldValue);
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

void CmdRequiredIfacePropertyChange::updateConnections(const QVariant &labelInherited)
{
    if (!m_ri || !m_model)
        return;

    const bool nowInherited = labelInherited.toBool();
    if (nowInherited) {
        m_ri->setProp(m_propertyName, labelInherited);
        restoreConnections();
    } else {
        removeConnections();
        m_ri->setProp(m_propertyName, labelInherited);
    }
}

void CmdRequiredIfacePropertyChange::removeConnections()
{
    for (auto cmd : m_cmdRmConnection)
        cmd->redo();
}

void CmdRequiredIfacePropertyChange::restoreConnections()
{
    for (auto cmd : m_cmdRmConnection)
        cmd->undo();
}

void CmdRequiredIfacePropertyChange::prepareRemoveConnectionCommands()
{
    const QString riOriginalKind = m_ri->originalAttr(meta::Props::token(meta::Props::Token::kind)).toString();

    for (AADLObjectConnection *connection : m_affectedConenctions) {
        if (const AADLObjectIfaceProvided *pi = connection->selectIface<const AADLObjectIfaceProvided *>()) {

            if (m_ri->kindFromString(riOriginalKind) != pi->kind() || m_ri->originalParams() != pi->params()) {
                const QVariantList params = { QVariant::fromValue(connection), QVariant::fromValue(m_model.data()) };
                if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::RemoveEntity, params))
                    m_cmdRmConnection.append(cmdRm);
            }
        }
    }
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
