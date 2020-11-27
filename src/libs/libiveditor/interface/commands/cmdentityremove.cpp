/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdentityremove.h"

#include "commandids.h"

#include <aadlobject.h>
#include <aadlobjectconnection.h>
#include <aadlobjectconnectiongroup.h>
#include <aadlobjectfunction.h>
#include <aadlobjectfunctiontype.h>
#include <aadlobjectiface.h>
#include <aadlobjectsmodel.h>
namespace aadlinterface {
namespace cmd {

CmdEntityRemove::CmdEntityRemove(aadl::AADLObject *entity, aadl::AADLObjectsModel *model)
    : shared::UndoCommand()
    , m_model(model)
    , m_entity(entity)
{
    setText(QObject::tr("Remove"));

    collectRelatedItems(m_entity);
}

CmdEntityRemove::~CmdEntityRemove()
{
    const QVector<QPointer<aadl::AADLObject>> objects = m_relatedIfaces + m_relatedConnections + m_relatedEntities;
    for (aadl::AADLObject *obj : objects)
        if (obj && !obj->parent())
            delete obj;
}

aadl::AADLObjectFunctionType *CmdEntityRemove::putParentFunctionFor(const aadl::AADLObject *obj)
{
    if (!obj || !obj->parentObject())
        return nullptr;

    const shared::Id &objId = obj->id();
    if (auto *fn = obj->parentObject()->as<aadl::AADLObjectFunctionType *>())
        m_parentFunctions[objId] = fn;

    return m_parentFunctions[objId];
}

aadl::AADLObjectFunctionType *CmdEntityRemove::popParentFunctionFor(const aadl::AADLObject *obj)
{
    return obj ? m_parentFunctions.take(obj->id()) : nullptr;
}

void CmdEntityRemove::advancedRemove(aadl::AADLObject *obj)
{
    if (!obj)
        return;

    auto fn = putParentFunctionFor(obj);
    if (m_model)
        m_model->removeObject(obj);

    if (fn)
        fn->removeChild(obj);
}

void CmdEntityRemove::advancedRestore(aadl::AADLObject *obj)
{
    if (!obj)
        return;

    if (auto *fn = popParentFunctionFor(obj))
        fn->addChild(obj);

    if (m_model)
        m_model->addObject(obj);
}

void CmdEntityRemove::redo()
{
    if (!m_model)
        return;

    auto removeAadlObjects = [this](const QVector<QPointer<aadl::AADLObject>> &collection) {
        for (auto it = collection.cbegin(); it != collection.cend(); ++it)
            advancedRemove(*it);
    };

    removeAadlObjects(m_relatedIfaces);

    for (auto it = m_relatedConnections.cbegin(); it != m_relatedConnections.cend(); ++it) {
        if (auto *connection = qobject_cast<aadl::AADLObjectConnection *>(*it))
            connection->unsetInheritPI();

        advancedRemove(*it);
    }

    removeAadlObjects(m_relatedEntities);

    Q_EMIT entityRemoved(m_entity, this);
}

void CmdEntityRemove::undo()
{
    if (!m_model)
        return;

    auto restoreAadlObjects = [this](const QVector<QPointer<aadl::AADLObject>> &collection) {
        for (auto it = collection.crbegin(); it != collection.crend(); ++it)
            advancedRestore(*it);
    };

    restoreAadlObjects(m_relatedEntities);
    restoreAadlObjects(m_relatedIfaces);

    for (auto it = m_relatedConnections.crbegin(); it != m_relatedConnections.crend(); ++it) {
        if (auto *connection = qobject_cast<aadl::AADLObjectConnection *>(*it))
            connection->setInheritPI();

        advancedRestore(*it);
    }
}

bool CmdEntityRemove::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdEntityRemove::id() const
{
    return RemoveEntity;
}

aadl::AADLObject *CmdEntityRemove::entity() const
{
    return m_entity;
}

void CmdEntityRemove::collectRelatedItems(aadl::AADLObject *toBeRemoved)
{
    if (!toBeRemoved)
        return;

    switch (toBeRemoved->aadlType()) {
    case aadl::AADLObject::Type::ConnectionGroup:
        if (auto connection = qobject_cast<aadl::AADLObjectConnectionGroup *>(toBeRemoved)) {
            storeLinkedEntity(connection->sourceInterface());
            storeLinkedEntity(connection->targetInterface());
        }
        break;
    case aadl::AADLObject::Type::InterfaceGroup:
    case aadl::AADLObject::Type::RequiredInterface:
    case aadl::AADLObject::Type::ProvidedInterface: {
        if (auto *iface = qobject_cast<aadl::AADLObjectIface *>(toBeRemoved)) {
            for (const auto &clone : iface->clones())
                collectRelatedItems(clone);
            if (auto connection = m_model->getConnectionForIface(iface->id()))
                storeLinkedEntity(connection);
        }
        break;
    }
    case aadl::AADLObject::Type::Function:
    case aadl::AADLObject::Type::FunctionType: {
        if (auto *fnType = qobject_cast<aadl::AADLObjectFunctionType *>(toBeRemoved)) {
            for (auto iface : fnType->interfaces())
                collectRelatedItems(iface);

            for (auto child :
                    toBeRemoved->findChildren<aadl::AADLObjectFunction *>(QString(), Qt::FindDirectChildrenOnly))
                collectRelatedItems(child);
        }
        break;
    }
    default: {
        for (auto child : toBeRemoved->findChildren<aadl::AADLObject *>(QString(), Qt::FindDirectChildrenOnly))
            collectRelatedItems(child);
        break;
    }
    }

    storeLinkedEntity(toBeRemoved);
}

void CmdEntityRemove::storeLinkedEntity(aadl::AADLObject *linkedEntity)
{
    auto storeObject = [](aadl::AADLObject *obj, QVector<QPointer<aadl::AADLObject>> *collection) {
        if (obj && !collection->contains(obj))
            collection->append(obj);
    };

    QVector<QPointer<aadl::AADLObject>> *pCollection { nullptr };
    switch (linkedEntity->aadlType()) {
    case aadl::AADLObject::Type::ConnectionGroup:
    case aadl::AADLObject::Type::Connection:
        pCollection = &m_relatedConnections;
        break;
    case aadl::AADLObject::Type::InterfaceGroup:
    case aadl::AADLObject::Type::RequiredInterface:
    case aadl::AADLObject::Type::ProvidedInterface:
        pCollection = &m_relatedIfaces;
        break;
    default:
        pCollection = &m_relatedEntities;
        break;
    }
    storeObject(linkedEntity, pCollection);
}

}
}
