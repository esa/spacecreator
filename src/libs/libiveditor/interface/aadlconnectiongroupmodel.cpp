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

#include "aadlconnectiongroupmodel.h"

#include "aadlnamevalidator.h"
#include "aadlobjectconnection.h"
#include "aadlobjectconnectiongroup.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "commandsstack.h"
#include "interface/commands/cmdconnectiongroupitemchange.h"
#include "interface/commands/commandsfactory.h"

#include <QSet>

namespace ive {

AADLConnectionGroupModel::AADLConnectionGroupModel(
        ivm::AADLObjectConnectionGroup *connectionGroup, cmd::CommandsStack::Macro *macro, QObject *parent)
    : QAbstractListModel(parent)
    , m_connectionGroup(connectionGroup)
    , m_cmdMacro(macro)
{
    const auto groupedConnections = connectionGroup->groupedConnections();
    std::for_each(groupedConnections.constBegin(), groupedConnections.constEnd(),
            [this](ivm::AADLObjectConnection *c) { m_groupedConnetions.insert(c->id()); });

    if (auto model = connectionGroup->objectsModel()) {
        const QList<ivm::AADLObjectIface *> targetIfaces = connectionGroup->targetFunctionInterfaces();
        const QList<ivm::AADLObjectIface *> sourceIfaces = connectionGroup->sourceFunctionInterfaces();

        for (auto iface : sourceIfaces) {
            const auto ifaceConnections = model->getConnectionsForIface(iface->id());

            std::copy_if(ifaceConnections.constBegin(), ifaceConnections.constEnd(),
                    std::back_inserter(m_allConnections), [&](const ivm::AADLObjectConnection *connection) {
                        if (sourceIfaces.contains(connection->sourceInterface()))
                            return targetIfaces.contains(connection->targetInterface());
                        else if (sourceIfaces.contains(connection->targetInterface()))
                            return targetIfaces.contains(connection->sourceInterface());
                        return false;
                    });
        }
    }

    auto updateEnableState = [this]() {
        if (m_groupedConnetions.size() == 1) {
            if (auto model = m_connectionGroup->objectsModel()) {
                ivm::AADLObjectConnection *connection = model->getConnection(*m_groupedConnetions.begin());
                const QModelIndex idx = index(m_allConnections.indexOf(connection));
                Q_EMIT dataChanged(idx, idx);
            }
        }
    };

    connect(connectionGroup, &ivm::AADLObjectConnectionGroup::connectionAdded, this,
            [this, updateEnableState](ivm::AADLObjectConnection *connection) {
                updateEnableState();
                m_groupedConnetions.insert(connection->id());

                const QModelIndex idx = index(m_allConnections.indexOf(connection));
                Q_EMIT dataChanged(idx, idx, { Qt::CheckStateRole });
            });
    connect(connectionGroup, &ivm::AADLObjectConnectionGroup::connectionRemoved, this,
            [this, updateEnableState](ivm::AADLObjectConnection *connection) {
                m_groupedConnetions.remove(connection->id());
                updateEnableState();

                const QModelIndex idx = index(m_allConnections.indexOf(connection));
                Q_EMIT dataChanged(idx, idx, { Qt::CheckStateRole });
            });
}

QVariant AADLConnectionGroupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= m_allConnections.size() || index.row() < 0) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return ivm::AADLNameValidator::nextNameFor(m_allConnections.at(index.row()));
    }

    if (role == Qt::CheckStateRole) {
        return m_groupedConnetions.contains(m_allConnections.at(index.row())->id()) ? Qt::CheckState::Checked
                                                                                    : Qt::CheckState::Unchecked;
    }

    return QVariant();
}

int AADLConnectionGroupModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_allConnections.size();
}

bool AADLConnectionGroupModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }
    if (role != Qt::CheckStateRole) {
        return false;
    }

    const auto state = value.value<Qt::CheckState>();
    const auto connection = m_allConnections.at(index.row());

    const QVariantList params { QVariant::fromValue(m_connectionGroup), QVariant::fromValue(connection.data()),
        state == Qt::Checked };
    if (const auto propsCmd = cmd::CommandsFactory::create(cmd::ChangeConnectionGroupEntity, params)) {
        m_cmdMacro->push(propsCmd);
        return true;
    }
    return false;
}

Qt::ItemFlags AADLConnectionGroupModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsUserCheckable | (isEnabled(index) ? Qt::ItemIsEnabled : Qt::NoItemFlags);
}

bool AADLConnectionGroupModel::isEnabled(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return false;
    }
    if (m_groupedConnetions.size() != 1) {
        return true;
    }
    return !m_groupedConnetions.contains(m_allConnections.at(index.row())->id());
}

} // namespace ive
