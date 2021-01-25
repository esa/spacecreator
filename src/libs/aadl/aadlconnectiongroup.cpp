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

#include "aadlconnectiongroup.h"

#include "aadlfunctiontype.h"
#include "aadlifacegroup.h"
#include "aadlmodel.h"

#include <QtDebug>

namespace ivm {

AADLConnectionGroup::AADLConnectionGroup(const QString &name, AADLIfaceGroup *ifaceSource,
        AADLIfaceGroup *ifaceTarget, const QList<QPointer<AADLConnection>> &connections, QObject *parent)
    : AADLConnection(AADLObject::Type::ConnectionGroup, ifaceSource, ifaceTarget, parent)
{
    setTitle(name);
    ifaceSource->setGroupName(name);
    ifaceTarget->setGroupName(name);

    for (auto connection : connections) {
        m_initConnections.append(connection->id());
    }
}

bool AADLConnectionGroup::postInit()
{
    if (m_initConnections.isEmpty()) {
        return true;
    }

    m_connections.clear();

    for (const auto &id : m_initConnections) {
        addConnection(objectsModel()->getConnection(id));
    }

    m_initConnections.clear();

    return true;
}

bool AADLConnectionGroup::aboutToBeRemoved()
{
    if (!m_connections.isEmpty()) {
        m_initConnections.clear();
    }
    while (!m_connections.isEmpty()) {
        const auto connection = m_connections.takeLast();
        removeConnection(connection);
        m_initConnections.append(connection->id());
    }
    return true;
}

QList<QPointer<AADLConnection>> AADLConnectionGroup::groupedConnections() const
{
    return m_connections;
}

QList<QPointer<AADLIface>> AADLConnectionGroup::groupedSourceInterfaces() const
{
    if (auto ifaceGroup = sourceInterfaceGroup())
        return ifaceGroup->entities();

    return {};
}

QList<QPointer<AADLIface>> AADLConnectionGroup::groupedTargetInterfaces() const
{
    if (auto ifaceGroup = targetInterfaceGroup())
        return ifaceGroup->entities();

    return {};
}

QList<AADLIface *> AADLConnectionGroup::sourceFunctionInterfaces() const
{
    if (auto function = qobject_cast<AADLFunctionType *>(source())) {
        return function->interfaces().toList();
    }
    return {};
}

QList<AADLIface *> AADLConnectionGroup::targetFunctionInterfaces() const
{
    if (auto function = qobject_cast<AADLFunctionType *>(target())) {
        return function->interfaces().toList();
    }
    return {};
}

AADLIfaceGroup *AADLConnectionGroup::sourceInterfaceGroup() const
{
    return qobject_cast<AADLIfaceGroup *>(sourceInterface());
}

AADLIfaceGroup *AADLConnectionGroup::targetInterfaceGroup() const
{
    return qobject_cast<AADLIfaceGroup *>(targetInterface());
}

void AADLConnectionGroup::addConnection(const QPointer<AADLConnection> &connection)
{
    if (connection.isNull() || !connection->sourceInterface() || !connection->targetInterface()
            || !sourceInterfaceGroup() || !targetInterfaceGroup() || m_connections.contains(connection)) {
        return;
    }

    connection->setGroupName(title());

    if (sourceInterfaceGroup()->parentObject()->id() == connection->sourceInterface()->parentObject()->id()) {
        sourceInterfaceGroup()->addEntity(connection->sourceInterface());
    } else if (sourceInterfaceGroup()->parentObject()->id() == connection->targetInterface()->parentObject()->id()) {
        sourceInterfaceGroup()->addEntity(connection->targetInterface());
    } else {
        return;
    }

    if (targetInterfaceGroup()->parentObject()->id() == connection->sourceInterface()->parentObject()->id()) {
        targetInterfaceGroup()->addEntity(connection->sourceInterface());
    } else if (targetInterfaceGroup()->parentObject()->id() == connection->targetInterface()->parentObject()->id()) {
        targetInterfaceGroup()->addEntity(connection->targetInterface());
    } else {
        return;
    }

    connection->setParent(this);
    m_connections.append(connection);

    Q_EMIT connectionAdded(connection);
}

void AADLConnectionGroup::removeConnection(const QPointer<AADLConnection> &connection)
{
    if (connection.isNull()) {
        return;
    }

    connection->setGroupName({});
    connection->setParent(parent());
    m_connections.removeAll(connection);
    bool removeSourceIface = true;
    bool removeTargetIface = true;

    std::for_each(m_connections.constBegin(), m_connections.constEnd(),
            [&](const QPointer<AADLConnection> &connectionObj) {
                if (connectionObj.isNull()) {
                    return;
                }
                if ((connectionObj->sourceInterface() == connection->sourceInterface()
                            || connectionObj->targetInterface() == connection->sourceInterface())
                        && removeSourceIface) {
                    removeSourceIface = false;
                }
                if ((connectionObj->sourceInterface() == connection->targetInterface()
                            || connectionObj->targetInterface() == connection->targetInterface())
                        && removeTargetIface) {
                    removeTargetIface = false;
                }
            });
    if (removeSourceIface) {
        sourceInterfaceGroup()->removeEntity(connection->sourceInterface());
    }
    if (removeTargetIface) {
        targetInterfaceGroup()->removeEntity(connection->targetInterface());
    }

    Q_EMIT connectionRemoved(connection);
}

QVariantList AADLConnectionGroup::CreationInfo::toVarList() const
{
    return { QVariant::fromValue(*this) };
}

} // namespace ivm
