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

#include "ivconnectiongroup.h"

#include "ivfunctiontype.h"
#include "ivinterfacegroup.h"
#include "ivmodel.h"

#include <QtDebug>

namespace ivm {

IVConnectionGroup::IVConnectionGroup(const QString &name, IVInterfaceGroup *ifaceSource, IVInterfaceGroup *ifaceTarget,
        const QList<QPointer<IVConnection>> &connections, QObject *parent)
    : IVConnection(IVObject::Type::ConnectionGroup, ifaceSource, ifaceTarget, parent)
{
    setTitle(name);
    ifaceSource->setGroupName(name);
    ifaceTarget->setGroupName(name);

    for (auto connection : connections) {
        m_initConnections.append(connection->id());
    }
}

bool IVConnectionGroup::postInit(QString *warning)
{
    Q_UNUSED(warning);
    if (m_initConnections.isEmpty()) {
        return true;
    }

    m_connections.clear();

    for (const auto &id : m_initConnections) {
        addConnection(model()->getConnection(id));
    }

    m_initConnections.clear();

    return true;
}

bool IVConnectionGroup::aboutToBeRemoved()
{
    if (!m_connections.isEmpty()) {
        m_initConnections.clear();
    }
    while (!m_connections.isEmpty()) {
        const auto connection = m_connections.takeLast();
        removeConnection(connection);
        m_initConnections.append(connection->id());
    }
    return IVObject::aboutToBeRemoved();
}

QList<QPointer<IVConnection>> IVConnectionGroup::groupedConnections() const
{
    return m_connections;
}

QList<QPointer<IVInterface>> IVConnectionGroup::groupedSourceInterfaces() const
{
    if (auto ifaceGroup = sourceInterfaceGroup())
        return ifaceGroup->entities();

    return {};
}

QList<QPointer<IVInterface>> IVConnectionGroup::groupedTargetInterfaces() const
{
    if (auto ifaceGroup = targetInterfaceGroup())
        return ifaceGroup->entities();

    return {};
}

QList<IVInterface *> IVConnectionGroup::sourceFunctionInterfaces() const
{
    if (auto function = qobject_cast<IVFunctionType *>(source())) {
        return function->interfaces().toList();
    }
    return {};
}

QList<IVInterface *> IVConnectionGroup::targetFunctionInterfaces() const
{
    if (auto function = qobject_cast<IVFunctionType *>(target())) {
        return function->interfaces().toList();
    }
    return {};
}

IVInterfaceGroup *IVConnectionGroup::sourceInterfaceGroup() const
{
    return qobject_cast<IVInterfaceGroup *>(sourceInterface());
}

IVInterfaceGroup *IVConnectionGroup::targetInterfaceGroup() const
{
    return qobject_cast<IVInterfaceGroup *>(targetInterface());
}

/*!
   Adds a connection whithout removing it from a function.
   Is used for example when loading connections of a group
 */
void IVConnectionGroup::addGroupedConnection(const QPointer<IVConnection> &connection)
{
    connection->setParent(this);
    m_connections.append(connection);

    Q_EMIT connectionAdded(connection);
}

void IVConnectionGroup::addConnection(const QPointer<IVConnection> &connection)
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

    addGroupedConnection(connection);
}

void IVConnectionGroup::removeConnection(const QPointer<IVConnection> &connection)
{
    if (connection.isNull()) {
        return;
    }

    connection->setGroupName({});
    connection->setParent(parent());
    m_connections.removeAll(connection);
    bool removeSourceIface = true;
    bool removeTargetIface = true;

    std::for_each(
            m_connections.constBegin(), m_connections.constEnd(), [&](const QPointer<IVConnection> &connectionObj) {
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

QVariantList IVConnectionGroup::CreationInfo::toVarList() const
{
    return { QVariant::fromValue(*this) };
}

} // namespace ivm
