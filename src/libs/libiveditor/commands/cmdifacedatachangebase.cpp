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

#include "cmdifacedatachangebase.h"

#include "cmdentitiesremove.h"
#include "ivconnection.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"

namespace ive {
namespace cmd {

static inline EntityAttribute::Type entityType(
        shared::PropertyTemplateConfig *config, const ivm::IVObject *obj, const QString &attrName)
{
    if (!config || !obj) {
        return EntityAttribute::Type::Attribute;
    }

    if (auto propertyTemplate = config->propertyTemplateForObject(obj, attrName)) {
        return propertyTemplate->info() == shared::PropertyTemplate::Info::Attribute ? EntityAttribute::Type::Attribute
                                                                                     : EntityAttribute::Type::Property;
    }

    return EntityAttribute::Type::Attribute;
}

CmdIfaceDataChangeBase::CmdIfaceDataChangeBase(shared::PropertyTemplateConfig *config, ivm::IVInterface *iface,
        const QString &targetName, const QVariant &targetValue, const QVariant &prevValue, QUndoCommand *parent)
    : shared::cmd::CmdEntityAttributesChange(
            config, iface, { EntityAttribute { targetName, targetValue, entityType(config, iface, targetName) } })
    , m_iface(iface)
    , m_model(m_iface ? m_iface->model() : nullptr)
    , m_relatedConnections()
    , m_targetName(targetName)
    , m_targetToken(ivm::meta::Props::token(m_targetName))
    , m_oldValue(prevValue)
    , m_newValue(targetValue)
    , m_cmdRmConnection()
{
    if (m_iface) {
        setText(QObject::tr("Change %1's %2").arg(m_iface->titleUI(), m_targetName));
    }
}

CmdIfaceDataChangeBase::~CmdIfaceDataChangeBase()
{
    qDeleteAll(m_cmdRmConnection);
}

QVector<ivm::IVConnection *> CmdIfaceDataChangeBase::getRelatedConnections()
{
    if (!m_model) {
        return {};
    }

    return m_model->getRelatedConnections(m_iface);
}

ivm::IVInterface *CmdIfaceDataChangeBase::interface() const
{
    return m_iface;
}

ivm::IVInterface *CmdIfaceDataChangeBase::getConnectionOtherSide(
        const ivm::IVConnection *connection, ivm::IVInterface *changedIface)
{
    if (connection && changedIface) {
        switch (connection->connectionType()) {
        case ivm::IVConnection::ConnectionType::RI2PI: {
            return changedIface->isRequired() ? connection->targetInterface() : changedIface;
        }
        case ivm::IVConnection::ConnectionType::PI2RI: {
            return changedIface->isProvided() ? connection->targetInterface() : changedIface;
        }
        case ivm::IVConnection::ConnectionType::PI2PI:
        case ivm::IVConnection::ConnectionType::RI2RI: {
            if (connection->sourceInterface() == changedIface
                    || connection->sourceInterface()->cloneOf() == changedIface) {
                return connection->targetInterface();
            }

            if (connection->targetInterface() == changedIface
                    || connection->targetInterface()->cloneOf() == changedIface) {
                return connection->sourceInterface();
            }

            break;
        }
        default:
            break;
        }
    }

    Q_UNREACHABLE();

    return nullptr;
}

void CmdIfaceDataChangeBase::prepareRemoveConnectionCommands()
{
    QList<ivm::IVObject *> entities;
    for (const auto &connection : m_relatedConnections) {
        if (connectionMustDie(connection)) {
            entities.append(connection);
        }
    }
    if (!entities.isEmpty()) {
        auto cmdRm = new cmd::CmdEntitiesRemove(entities, m_model);
        m_cmdRmConnection.append(cmdRm);
    }
}

}
}
