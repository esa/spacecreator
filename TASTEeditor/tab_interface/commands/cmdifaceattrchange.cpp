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
static QVector<QPointer<AADLObjectIface>> getRelatedIfaces(AADLObjectIface *iface)
{
    QVector<QPointer<AADLObjectIface>> ifaces;
    ifaces.append(iface);
    if (iface->isCloned())
        for (AADLObjectIface *clone : iface->clones())
            ifaces.append(clone);
    return ifaces;
}

static inline QVector<AADLObjectConnection *> getRelatedConnections(const QPointer<AADLObjectIface> iface)
{
    QVector<AADLObjectConnection *> affected;

    if (!iface)
        return affected;

    const AADLObjectsModel *model = iface->objectsModel();
    if (!model)
        return affected;

    for (AADLObjectIface *i : getRelatedIfaces(iface))
        affected += model->getConnectionsForIface(i->id());

    return affected;
}

CmdIfaceAttrChange::CmdIfaceAttrChange(AADLObjectIface *entity, const QString &attrName, const QVariant &value)
    : QUndoCommand()
    , m_iface(entity)
    , m_model(m_iface ? m_iface->objectsModel() : nullptr)
    , m_relatedConnections()
    , m_attrName(attrName)
    , m_attrToken(meta::Props::token(m_attrName))
    , m_oldValue(getCurrentAttribute(entity, m_attrName))
    , m_newValue(value)
{
    setText(QObject::tr("Change RI inheritance"));

    if (m_attrToken == meta::Props::Token::kind) {
        m_relatedConnections = getRelatedConnections(m_iface);
        prepareRemoveConnectionCommands();
    }
}

CmdIfaceAttrChange::~CmdIfaceAttrChange()
{
    qDeleteAll(m_cmdRmConnection);
}

void CmdIfaceAttrChange::redo()
{
    switch (m_attrToken) {
    case meta::Props::Token::kind: {
        setKind(m_newValue);
        break;
    }
    default:
        m_iface->setAttr(m_attrName, m_newValue);
        break;
    }
}

void CmdIfaceAttrChange::undo()
{
    switch (m_attrToken) {
    case meta::Props::Token::kind: {
        setKind(m_oldValue);
        break;
    }
    default:
        m_iface->setAttr(m_attrName, m_oldValue);
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

    m_iface->setAttr(m_attrName, kind);
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

void CmdIfaceAttrChange::prepareRemoveConnectionCommands()
{
    auto isInheritedRI = [](const AADLObjectIface *iface) {
        if (const AADLObjectIfaceRequired *ri = iface->as<const AADLObjectIfaceRequired *>())
            return ri->inheritPi();
        return false;
    };

    const AADLObjectIface::OperationKind k = AADLObjectIface::kindFromString(m_newValue.toString());
    for (AADLObjectConnection *connection : m_relatedConnections) {
        if (const AADLObjectIface *srci = connection->sourceInterface()) {
            if (const AADLObjectIface *dsti = connection->targetInterface()) {
                if (srci->isRequired() || dsti->isRequired()) {
                    const bool inheritPI = isInheritedRI(srci) || isInheritedRI(dsti);
                    if (inheritPI)
                        continue;
                }

                if (k != srci->kind() || k != dsti->kind()) {
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
