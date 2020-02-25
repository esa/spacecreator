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

static inline QVariant getCurrentProperty(const AADLObjectIfaceRequired *entity, const QString &name)
{
    return (entity && !name.isEmpty()) ? entity->prop(name) : QVariant();
}
static QVector<QPointer<AADLObjectIfaceRequired>> getRelatedIfaces(AADLObjectIfaceRequired *ri)
{
    QVector<QPointer<AADLObjectIfaceRequired>> ifaces;
    ifaces.append(ri);
    if (ri->isCloned())
        for (AADLObjectIface *iface : ri->clones())
            if (AADLObjectIfaceRequired *ifaceRequired = iface->as<AADLObjectIfaceRequired *>())
                ifaces.append(ifaceRequired);
    return ifaces;
}

static inline QVector<AADLObjectConnection *> getRelatedConnections(const QPointer<AADLObjectIfaceRequired> iface)
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
            prepareRemoveConnectionCommands();
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
        setPropLabelInherited(m_newValue);
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
        setPropLabelInherited(m_oldValue);
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

void CmdRequiredIfacePropertyChange::setPropLabelInherited(const QVariant &labelInherited)
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
    for (auto cmd : m_cmdRmConnection) {
        if (CmdEntityRemove *rmCmd = dynamic_cast<CmdEntityRemove *>(cmd))
            if (AADLObjectConnection *connection = rmCmd->entity()->as<AADLObjectConnection *>())
                connection->uninheritLabel();

        cmd->redo();
    }
}

void CmdRequiredIfacePropertyChange::restoreConnections()
{
    for (auto cmd : m_cmdRmConnection) {
        cmd->undo();

        if (CmdEntityRemove *rmCmd = dynamic_cast<CmdEntityRemove *>(cmd))
            if (AADLObjectConnection *connection = rmCmd->entity()->as<AADLObjectConnection *>())
                connection->inheritLabel();
    }
}

void CmdRequiredIfacePropertyChange::prepareRemoveConnectionCommands()
{
    for (AADLObjectConnection *connection : m_relatedConnections) {
        if (const AADLObjectIfaceRequired *ri = connection->selectIface<const AADLObjectIfaceRequired *>()) {
            if (const AADLObjectIfaceProvided *pi = connection->selectIface<const AADLObjectIfaceProvided *>()) {
                const QString riOriginalKind =
                        ri->originalAttr(meta::Props::token(meta::Props::Token::kind)).toString();
                if (ri->kindFromString(riOriginalKind) != pi->kind() || ri->originalParams() != pi->params()) {
                    const QVariantList params = { QVariant::fromValue(connection),
                                                  QVariant::fromValue(m_model.data()) };
                    if (QUndoCommand *cmdRm = cmd::CommandsFactory::create(cmd::RemoveEntity, params))
                        m_cmdRmConnection.append(cmdRm);
                }
            }
        }
    }
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
