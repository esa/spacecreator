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

QVector<QPointer<AADLObjectIfaceRequired>> getRelatedIfaces(AADLObjectIfaceRequired *ri)
{
    QVector<QPointer<AADLObjectIfaceRequired>> ifaces;
    ifaces.append(ri);
    if (ri->isCloned())
        for (AADLObjectIface *iface : ri->clones())
            if (AADLObjectIfaceRequired *ifaceRequired = iface->as<AADLObjectIfaceRequired *>())
                ifaces.append(ifaceRequired);
    return ifaces;
}

QVector<AADLObjectConnection *> getRelatedConnections(const QPointer<AADLObjectIfaceRequired> iface)
{
    QVector<AADLObjectConnection *> affected;

    if (!iface)
        return affected;

    const AADLObjectsModel *model = iface->objectsModel();
    if (!model)
        return affected;

    for (AADLObjectIfaceRequired *ri : getRelatedIfaces(iface))
        for (AADLObjectConnection *connection : model->getConnectionsForIface(ri->id()))
            if (const AADLObjectIfaceProvided *pi = connection->selectIface<const AADLObjectIfaceProvided *>())
                affected.append(connection);

    return affected;
}

QVector<QUndoCommand *> prepareRemoveConnectionCommands(const QVector<AADLObjectConnection *> &connections,
                                                        AADLObjectsModel *model)
{
    QVector<QUndoCommand *> commands;
    for (AADLObjectConnection *connection : connections) {
        if (const AADLObjectIfaceRequired *ri = connection->selectIface<const AADLObjectIfaceRequired *>()) {
            if (const AADLObjectIfaceProvided *pi = connection->selectIface<const AADLObjectIfaceProvided *>()) {
                const QString riOriginalKind =
                        ri->originalAttr(meta::Props::token(meta::Props::Token::kind)).toString();
                if (ri->kindFromString(riOriginalKind) != pi->kind() || ri->originalParams() != pi->params()) {
                    const QVariantList params = { QVariant::fromValue(connection), QVariant::fromValue(model) };
                    if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::RemoveEntity, params))
                        commands.append(cmdRm);
                }
            }
        }
    }
    return commands;
}

CmdRequiredIfacePropertyChange::CmdRequiredIfacePropertyChange(AADLObjectIfaceRequired *entity, const QString &propName,
                                                               const QVariant &value)
    : QUndoCommand()
    , m_ri(entity)
    , m_model(m_ri ? m_ri->objectsModel() : nullptr)
    , m_relatedConnections()
    , m_propertyName(propName)
    , m_propertyToken(meta::Props::token(m_propertyName))
    , m_oldValue(getCurrentProperty(entity, m_propertyName))
    , m_newValue(value)
{
    setText(QObject::tr("Change RI inheritance"));

    if (m_propertyToken == meta::Props::Token::labelInheritance) {
        m_relatedConnections = getRelatedConnections(m_ri);
        const bool lostInheritance = !m_newValue.toBool();
        if (lostInheritance)
            m_cmdRmConnection = prepareRemoveConnectionCommands(m_relatedConnections, m_model);
    }
}

CmdRequiredIfacePropertyChange::~CmdRequiredIfacePropertyChange()
{
    qDeleteAll(m_cmdRmConnection);
}

void CmdRequiredIfacePropertyChange::redo()
{
    switch (m_propertyToken) {
    case meta::Props::Token::labelInheritance: {
        updateLabelInheritance(m_newValue.toBool());
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
        updateLabelInheritance(m_oldValue.toBool());
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

void CmdRequiredIfacePropertyChange::updateLabelInheritance(bool nowInherited)
{
    if (!m_ri || !m_model)
        return;

    if (nowInherited) {
        m_ri->setProp(m_propertyName, nowInherited);

        for (auto cmd : m_cmdRmConnection)
            cmd->undo();
        for (auto connection : m_relatedConnections)
            connection->inheritLabel();
    } else {
        for (auto connection : m_relatedConnections)
            connection->uninheritLabel();
        for (auto cmd : m_cmdRmConnection)
            cmd->redo();

        m_ri->setProp(m_propertyName, nowInherited);
    }
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
