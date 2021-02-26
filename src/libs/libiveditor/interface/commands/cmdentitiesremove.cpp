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

#include "cmdentitiesremove.h"

#include "commandids.h"

#include <aadlconnection.h>
#include <aadlconnectiongroup.h>
#include <aadlfunction.h>
#include <aadlfunctiontype.h>
#include <aadliface.h>
#include <aadlmodel.h>
#include <aadlobject.h>
namespace ive {
namespace cmd {

CmdEntitiesRemove::CmdEntitiesRemove(const QList<QPointer<ivm::AADLObject>> &entities, ivm::AADLModel *model)
    : shared::UndoCommand()
    , m_model(model)
    , m_entities(entities)
{
    setText(QObject::tr("Remove"));

    for (const QPointer<ivm::AADLObject> &entity : entities) {
        collectRelatedItems(entity);
    }
}

CmdEntitiesRemove::~CmdEntitiesRemove()
{
    const QVector<QPointer<ivm::AADLObject>> objects = m_relatedIfaces + m_relatedConnections + m_relatedEntities;
    for (ivm::AADLObject *obj : objects)
        if (obj && !obj->parent())
            delete obj;
}

ivm::AADLFunctionType *CmdEntitiesRemove::putParentFunctionFor(const ivm::AADLObject *obj)
{
    if (!obj || !obj->parentObject())
        return nullptr;

    const shared::Id &objId = obj->id();
    if (auto *fn = obj->parentObject()->as<ivm::AADLFunctionType *>())
        m_parentFunctions[objId] = fn;

    return m_parentFunctions[objId];
}

ivm::AADLFunctionType *CmdEntitiesRemove::popParentFunctionFor(const ivm::AADLObject *obj)
{
    return obj ? m_parentFunctions.take(obj->id()) : nullptr;
}

void CmdEntitiesRemove::advancedRemove(ivm::AADLObject *obj)
{
    if (!obj)
        return;

    auto fn = putParentFunctionFor(obj);
    if (m_model)
        m_model->removeObject(obj);

    if (fn)
        fn->removeChild(obj);
}

void CmdEntitiesRemove::advancedRestore(ivm::AADLObject *obj)
{
    if (!obj)
        return;

    if (auto *fn = popParentFunctionFor(obj))
        fn->addChild(obj);

    if (m_model)
        m_model->addObject(obj);
}

void CmdEntitiesRemove::redo()
{
    if (!m_model)
        return;

    auto removeAadlObjects = [this](const QVector<QPointer<ivm::AADLObject>> &collection) {
        for (auto it = collection.cbegin(); it != collection.cend(); ++it) {
            if ((*it)->aadlType() == ivm::AADLObject::Type::Connection) {
                if (auto *connection = qobject_cast<ivm::AADLConnection *>(*it)) {
                    connection->unsetInheritPI();
                }
            }
            advancedRemove(*it);
        }
    };

    removeAadlObjects(m_relatedConnections);
    removeAadlObjects(m_relatedIfaces);
    removeAadlObjects(m_relatedEntities);

    Q_EMIT entitiesRemoved(m_entities, this);
}

void CmdEntitiesRemove::undo()
{
    if (!m_model)
        return;

    auto restoreAadlObjects = [this](const QVector<QPointer<ivm::AADLObject>> &collection) {
        for (auto it = collection.crbegin(); it != collection.crend(); ++it) {
            if ((*it)->aadlType() == ivm::AADLObject::Type::Connection) {
                if (auto *connection = qobject_cast<ivm::AADLConnection *>(*it)) {
                    connection->setInheritPI();
                }
            }
            advancedRestore(*it);
        }
    };

    restoreAadlObjects(m_relatedEntities);
    restoreAadlObjects(m_relatedIfaces);
    restoreAadlObjects(m_relatedConnections);
}

bool CmdEntitiesRemove::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdEntitiesRemove::id() const
{
    return RemoveEntity;
}

void CmdEntitiesRemove::collectRelatedItems(ivm::AADLObject *toBeRemoved)
{
    if (!toBeRemoved)
        return;

    switch (toBeRemoved->aadlType()) {
    case ivm::AADLObject::Type::ConnectionGroup:
        if (auto connection = qobject_cast<ivm::AADLConnectionGroup *>(toBeRemoved)) {
            storeLinkedEntity(connection->sourceInterface());
            storeLinkedEntity(connection->targetInterface());
        }
        break;
    case ivm::AADLObject::Type::InterfaceGroup:
    case ivm::AADLObject::Type::RequiredInterface:
    case ivm::AADLObject::Type::ProvidedInterface: {
        if (auto *iface = qobject_cast<ivm::AADLIface *>(toBeRemoved)) {
            for (const auto &clone : iface->clones())
                collectRelatedItems(clone);
            for (const auto &connection : m_model->getConnectionsForIface(iface->id()))
                storeLinkedEntity(connection);
        }
        break;
    }
    case ivm::AADLObject::Type::Function:
    case ivm::AADLObject::Type::FunctionType: {
        if (auto *fnType = qobject_cast<ivm::AADLFunctionType *>(toBeRemoved)) {
            for (auto iface : fnType->interfaces())
                collectRelatedItems(iface);

            for (auto child : toBeRemoved->findChildren<ivm::AADLFunction *>(QString(), Qt::FindDirectChildrenOnly))
                collectRelatedItems(child);
        }
        break;
    }
    default: {
        for (auto child : toBeRemoved->findChildren<ivm::AADLObject *>(QString(), Qt::FindDirectChildrenOnly))
            collectRelatedItems(child);
        break;
    }
    }

    storeLinkedEntity(toBeRemoved);
}

void CmdEntitiesRemove::storeLinkedEntity(ivm::AADLObject *linkedEntity)
{
    auto storeObject = [](ivm::AADLObject *obj, QVector<QPointer<ivm::AADLObject>> *collection) {
        if (obj && !collection->contains(obj))
            collection->append(obj);
    };

    QVector<QPointer<ivm::AADLObject>> *pCollection { nullptr };
    switch (linkedEntity->aadlType()) {
    case ivm::AADLObject::Type::ConnectionGroup:
    case ivm::AADLObject::Type::Connection:
        pCollection = &m_relatedConnections;
        break;
    case ivm::AADLObject::Type::InterfaceGroup:
    case ivm::AADLObject::Type::RequiredInterface:
    case ivm::AADLObject::Type::ProvidedInterface:
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
