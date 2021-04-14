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

#include <ivconnection.h>
#include <ivconnectiongroup.h>
#include <ivfunction.h>
#include <ivfunctiontype.h>
#include <ivinterface.h>
#include <ivmodel.h>
#include <ivobject.h>
namespace ive {
namespace cmd {

CmdEntitiesRemove::CmdEntitiesRemove(const QList<QPointer<ivm::IVObject>> &entities, ivm::IVModel *model)
    : shared::UndoCommand()
    , m_model(model)
    , m_entities(entities)
{
    setText(QObject::tr("Remove"));

    for (const QPointer<ivm::IVObject> &entity : entities) {
        collectRelatedItems(entity);
    }
}

CmdEntitiesRemove::~CmdEntitiesRemove()
{
    const QVector<QPointer<ivm::IVObject>> objects = m_relatedIfaces + m_relatedConnections + m_relatedEntities;
    for (ivm::IVObject *obj : objects)
        if (obj && !obj->parent())
            delete obj;
}

ivm::IVFunctionType *CmdEntitiesRemove::putParentFunctionFor(const ivm::IVObject *obj)
{
    if (!obj || !obj->parentObject())
        return nullptr;

    const shared::Id &objId = obj->id();
    if (auto *fn = obj->parentObject()->as<ivm::IVFunctionType *>())
        m_parentFunctions[objId] = fn;

    return m_parentFunctions[objId];
}

ivm::IVFunctionType *CmdEntitiesRemove::popParentFunctionFor(const ivm::IVObject *obj)
{
    return obj ? m_parentFunctions.take(obj->id()) : nullptr;
}

void CmdEntitiesRemove::advancedRemove(ivm::IVObject *obj)
{
    if (!obj)
        return;

    auto fn = putParentFunctionFor(obj);
    if (m_model)
        m_model->removeObject(obj);

    if (fn)
        fn->removeChild(obj);
}

void CmdEntitiesRemove::advancedRestore(ivm::IVObject *obj)
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

    auto removeIVObjects = [this](const QVector<QPointer<ivm::IVObject>> &collection) {
        for (auto it = collection.cbegin(); it != collection.cend(); ++it) {
            if ((*it)->type() == ivm::IVObject::Type::Connection) {
                if (auto *connection = qobject_cast<ivm::IVConnection *>(*it)) {
                    connection->unsetInheritPI();
                }
            }
            advancedRemove(*it);
        }
    };

    removeIVObjects(m_relatedConnections);
    removeIVObjects(m_relatedIfaces);
    removeIVObjects(m_relatedEntities);

    Q_EMIT entitiesRemoved(m_entities, this);
}

void CmdEntitiesRemove::undo()
{
    if (!m_model)
        return;

    auto restoreIVObjects = [this](const QVector<QPointer<ivm::IVObject>> &collection) {
        for (auto it = collection.crbegin(); it != collection.crend(); ++it) {
            if ((*it)->type() == ivm::IVObject::Type::Connection) {
                if (auto *connection = qobject_cast<ivm::IVConnection *>(*it)) {
                    connection->setInheritPI();
                }
            }
            advancedRestore(*it);
        }
    };

    restoreIVObjects(m_relatedEntities);
    restoreIVObjects(m_relatedIfaces);
    restoreIVObjects(m_relatedConnections);
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

void CmdEntitiesRemove::collectRelatedItems(ivm::IVObject *toBeRemoved)
{
    if (!toBeRemoved)
        return;

    switch (toBeRemoved->type()) {
    case ivm::IVObject::Type::ConnectionGroup:
        if (auto connection = qobject_cast<ivm::IVConnectionGroup *>(toBeRemoved)) {
            storeLinkedEntity(connection->sourceInterface());
            storeLinkedEntity(connection->targetInterface());
        }
        break;
    case ivm::IVObject::Type::InterfaceGroup:
    case ivm::IVObject::Type::RequiredInterface:
    case ivm::IVObject::Type::ProvidedInterface: {
        if (auto *iface = qobject_cast<ivm::IVInterface *>(toBeRemoved)) {
            for (const auto &clone : iface->clones())
                collectRelatedItems(clone);
            for (const auto &connection : m_model->getConnectionsForIface(iface->id()))
                storeLinkedEntity(connection);
        }
        break;
    }
    case ivm::IVObject::Type::Function:
    case ivm::IVObject::Type::FunctionType: {
        if (auto *fnType = qobject_cast<ivm::IVFunctionType *>(toBeRemoved)) {
            for (auto iface : fnType->interfaces())
                collectRelatedItems(iface);

            for (auto child : toBeRemoved->findChildren<ivm::IVFunction *>(QString(), Qt::FindDirectChildrenOnly))
                collectRelatedItems(child);
        }
        break;
    }
    default: {
        for (auto child : toBeRemoved->findChildren<ivm::IVObject *>(QString(), Qt::FindDirectChildrenOnly))
            collectRelatedItems(child);
        break;
    }
    }

    storeLinkedEntity(toBeRemoved);
}

void CmdEntitiesRemove::storeLinkedEntity(ivm::IVObject *linkedEntity)
{
    auto storeObject = [](ivm::IVObject *obj, QVector<QPointer<ivm::IVObject>> *collection) {
        if (obj && !collection->contains(obj))
            collection->append(obj);
    };

    QVector<QPointer<ivm::IVObject>> *pCollection { nullptr };
    switch (linkedEntity->type()) {
    case ivm::IVObject::Type::ConnectionGroup:
    case ivm::IVObject::Type::Connection:
        pCollection = &m_relatedConnections;
        break;
    case ivm::IVObject::Type::InterfaceGroup:
    case ivm::IVObject::Type::RequiredInterface:
    case ivm::IVObject::Type::ProvidedInterface:
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
