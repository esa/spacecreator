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

#include "aadlobjectconnectiongroup.h"

#include "aadlnamevalidator.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectifacegroup.h"
#include "aadlobjectsmodel.h"

#include <QtDebug>

namespace aadl {

AADLObjectConnectionGroup::AADLObjectConnectionGroup(const QString &name, AADLObjectIfaceGroup *ifaceSource,
        AADLObjectIfaceGroup *ifaceTarget, const QList<QPointer<AADLObjectConnection>> &connections, QObject *parent)
    : AADLObjectConnection(AADLObject::Type::ConnectionGroup, ifaceSource ? ifaceSource->function() : nullptr,
            ifaceTarget ? ifaceTarget->function() : nullptr, ifaceSource, ifaceTarget, parent)
{
    setTitle(name);
    ifaceSource->setGroupName(name);
    ifaceTarget->setGroupName(name);
    for (auto connection : connections) {
        addConnection(connection);
    }
}

bool AADLObjectConnectionGroup::postInit()
{
    if (m_initConnections.isEmpty()) {
        return true;
    }

    m_connections.clear();

    for (const auto id : m_initConnections) {
        addConnection(objectsModel()->getConnection(id));
    }

    m_initConnections.clear();

    return true;
}

QList<QPointer<AADLObjectConnection>> AADLObjectConnectionGroup::groupedConnections() const
{
    return m_connections;
}

QList<QPointer<AADLObjectIface>> AADLObjectConnectionGroup::groupedSourceInterfaces() const
{
    if (auto ifaceGroup = sourceInterfaceGroup())
        return ifaceGroup->entities();

    return {};
}

QList<QPointer<AADLObjectIface>> AADLObjectConnectionGroup::groupedTargetInterfaces() const
{
    if (auto ifaceGroup = targetInterfaceGroup())
        return ifaceGroup->entities();

    return {};
}

QList<AADLObjectIface *> AADLObjectConnectionGroup::sourceFunctionInterfaces() const
{
    if (auto function = qobject_cast<AADLObjectFunctionType *>(source())) {
        return function->interfaces().toList();
    }
    return {};
}

QList<AADLObjectIface *> AADLObjectConnectionGroup::targetFunctionInterfaces() const
{
    if (auto function = qobject_cast<AADLObjectFunctionType *>(target())) {
        return function->interfaces().toList();
    }
    return {};
}

AADLObjectIfaceGroup *AADLObjectConnectionGroup::sourceInterfaceGroup() const
{
    return qobject_cast<AADLObjectIfaceGroup *>(sourceInterface());
}

AADLObjectIfaceGroup *AADLObjectConnectionGroup::targetInterfaceGroup() const
{
    return qobject_cast<AADLObjectIfaceGroup *>(targetInterface());
}

void AADLObjectConnectionGroup::addConnection(const QPointer<AADLObjectConnection> &connection)
{
    if (connection.isNull() || !connection->sourceInterface() || !connection->targetInterface()
            || !sourceInterfaceGroup() || !targetInterfaceGroup()) {
        return;
    }

    const QString encodedName = AADLNameValidator::encodeName(aadlType(), title());
    connection->setGroupName(encodedName);

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

void AADLObjectConnectionGroup::removeConnection(const QPointer<AADLObjectConnection> &connection)
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
            [&](const QPointer<AADLObjectConnection> &connectionObj) {
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

QVariantList AADLObjectConnectionGroup::CreationInfo::toVarList() const
{
    return { QVariant::fromValue(*this) };
}

} // namespace aadl
