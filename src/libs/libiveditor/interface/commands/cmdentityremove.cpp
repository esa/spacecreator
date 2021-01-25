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
namespace ive {
namespace cmd {

CmdEntityRemove::CmdEntityRemove(ivm::AADLObject *entity, ivm::AADLObjectsModel *model)
    : shared::UndoCommand()
    , m_model(model)
    , m_entity(entity)
{
    setText(QObject::tr("Remove"));

    collectRelatedItems(m_entity);
}

CmdEntityRemove::~CmdEntityRemove()
{
    const QVector<QPointer<ivm::AADLObject>> objects = m_relatedIfaces + m_relatedConnections + m_relatedEntities;
    for (ivm::AADLObject *obj : objects)
        if (obj && !obj->parent())
            delete obj;
}

ivm::AADLObjectFunctionType *CmdEntityRemove::putParentFunctionFor(const ivm::AADLObject *obj)
{
    if (!obj || !obj->parentObject())
        return nullptr;

    const shared::Id &objId = obj->id();
    if (auto *fn = obj->parentObject()->as<ivm::AADLObjectFunctionType *>())
        m_parentFunctions[objId] = fn;

    return m_parentFunctions[objId];
}

ivm::AADLObjectFunctionType *CmdEntityRemove::popParentFunctionFor(const ivm::AADLObject *obj)
{
    return obj ? m_parentFunctions.take(obj->id()) : nullptr;
}

void CmdEntityRemove::advancedRemove(ivm::AADLObject *obj)
{
    if (!obj)
        return;

    auto fn = putParentFunctionFor(obj);
    if (m_model)
        m_model->removeObject(obj);

    if (fn)
        fn->removeChild(obj);
}

void CmdEntityRemove::advancedRestore(ivm::AADLObject *obj)
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

    auto removeAadlObjects = [this](const QVector<QPointer<ivm::AADLObject>> &collection) {
        for (auto it = collection.cbegin(); it != collection.cend(); ++it) {
            if ((*it)->aadlType() == ivm::AADLObject::Type::Connection) {
                if (auto *connection = qobject_cast<ivm::AADLObjectConnection *>(*it)) {
                    connection->unsetInheritPI();
                }
            }
            advancedRemove(*it);
        }
    };

    removeAadlObjects(m_relatedConnections);
    removeAadlObjects(m_relatedIfaces);
    removeAadlObjects(m_relatedEntities);

    Q_EMIT entityRemoved(m_entity, this);
}

void CmdEntityRemove::undo()
{
    if (!m_model)
        return;

    auto restoreAadlObjects = [this](const QVector<QPointer<ivm::AADLObject>> &collection) {
        for (auto it = collection.crbegin(); it != collection.crend(); ++it) {
            if ((*it)->aadlType() == ivm::AADLObject::Type::Connection) {
                if (auto *connection = qobject_cast<ivm::AADLObjectConnection *>(*it)) {
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

bool CmdEntityRemove::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command);
    return false;
}

int CmdEntityRemove::id() const
{
    return RemoveEntity;
}

ivm::AADLObject *CmdEntityRemove::entity() const
{
    return m_entity;
}

void CmdEntityRemove::collectRelatedItems(ivm::AADLObject *toBeRemoved)
{
    if (!toBeRemoved)
        return;

    switch (toBeRemoved->aadlType()) {
    case ivm::AADLObject::Type::ConnectionGroup:
        if (auto connection = qobject_cast<ivm::AADLObjectConnectionGroup *>(toBeRemoved)) {
            storeLinkedEntity(connection->sourceInterface());
            storeLinkedEntity(connection->targetInterface());
        }
        break;
    case ivm::AADLObject::Type::InterfaceGroup:
    case ivm::AADLObject::Type::RequiredInterface:
    case ivm::AADLObject::Type::ProvidedInterface: {
        if (auto *iface = qobject_cast<ivm::AADLObjectIface *>(toBeRemoved)) {
            for (const auto &clone : iface->clones())
                collectRelatedItems(clone);
            if (auto connection = m_model->getConnectionForIface(iface->id()))
                storeLinkedEntity(connection);
        }
        break;
    }
    case ivm::AADLObject::Type::Function:
    case ivm::AADLObject::Type::FunctionType: {
        if (auto *fnType = qobject_cast<ivm::AADLObjectFunctionType *>(toBeRemoved)) {
            for (auto iface : fnType->interfaces())
                collectRelatedItems(iface);

            for (auto child :
                    toBeRemoved->findChildren<ivm::AADLObjectFunction *>(QString(), Qt::FindDirectChildrenOnly))
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

void CmdEntityRemove::storeLinkedEntity(ivm::AADLObject *linkedEntity)
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
