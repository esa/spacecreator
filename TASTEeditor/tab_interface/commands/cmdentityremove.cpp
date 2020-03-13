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

#include <tab_aadl/aadlobject.h>
#include <tab_aadl/aadlobjectconnection.h>
#include <tab_aadl/aadlobjectiface.h>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

CmdEntityRemove::CmdEntityRemove(AADLObject *entity, AADLObjectsModel *model)
    : QUndoCommand()
    , m_model(model)
    , m_entity(entity)
{
    setText(QObject::tr("Remove"));

    collectRelatedItems(m_entity);
}

CmdEntityRemove::~CmdEntityRemove()
{
    const QVector<QPointer<AADLObject>> &objects = m_relatedIfaces + m_relatedConnections + m_relatedEntities;
    for (AADLObject *obj : objects)
        if (obj && !obj->parent())
            delete obj;
}

AADLObjectFunctionType *CmdEntityRemove::putParentFunctionFor(const AADLObject *obj)
{
    if (!obj || !obj->parentObject())
        return nullptr;

    const common::Id &objId = obj->id();
    if (AADLObjectFunctionType *fn = obj->parentObject()->as<AADLObjectFunctionType *>())
        m_parentFunctions[objId] = fn;

    return m_parentFunctions[objId];
}

AADLObjectFunctionType *CmdEntityRemove::popParentFunctionFor(const AADLObject *obj)
{
    return obj ? m_parentFunctions.take(obj->id()) : nullptr;
}

void CmdEntityRemove::advancedRemove(AADLObject *obj)
{
    if (!obj)
        return;

    if (AADLObjectFunctionType *fn = putParentFunctionFor(obj))
        fn->removeChild(obj);

    if (m_model)
        m_model->removeObject(obj);
}

void CmdEntityRemove::advancedRestore(AADLObject *obj)
{
    if (!obj)
        return;

    if (AADLObjectFunctionType *fn = popParentFunctionFor(obj))
        fn->addChild(obj);

    if (m_model)
        m_model->addObject(obj);
}

void CmdEntityRemove::redo()
{
    if (!m_model)
        return;

    auto removeAadlObjects = [this](const QVector<QPointer<AADLObject>> &collection) {
        for (auto it = collection.cbegin(); it != collection.cend(); ++it)
            advancedRemove(*it);
    };

    removeAadlObjects(m_relatedIfaces);

    for (auto it = m_relatedConnections.cbegin(); it != m_relatedConnections.cend(); ++it) {
        if (auto *connection = qobject_cast<AADLObjectConnection *>(*it))
            connection->unsetInheritPI();

        advancedRemove(*it);
    }

    removeAadlObjects(m_relatedEntities);

    if (m_entity)
        advancedRemove(m_entity);
}

void CmdEntityRemove::undo()
{
    if (!m_model)
        return;

    if (m_entity)
        advancedRestore(m_entity);

    auto restoreAadlObjects = [this](const QVector<QPointer<AADLObject>> &collection) {
        for (auto it = collection.crbegin(); it != collection.crend(); ++it)
            advancedRestore(*it);
    };

    restoreAadlObjects(m_relatedEntities);
    restoreAadlObjects(m_relatedIfaces);

    for (auto it = m_relatedConnections.crbegin(); it != m_relatedConnections.crend(); ++it) {
        if (AADLObjectConnection *connection = qobject_cast<AADLObjectConnection *>(*it))
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

AADLObject *CmdEntityRemove::entity() const
{
    return m_entity;
}

void CmdEntityRemove::collectRelatedItems(AADLObject *toBeRemoved)
{
    if (!toBeRemoved)
        return;

    switch (toBeRemoved->aadlType()) {
    case AADLObject::Type::Interface: {
        if (AADLObjectIface *iface = qobject_cast<AADLObjectIface *>(toBeRemoved)) {
            for (auto clone : iface->clones())
                collectRelatedItems(clone);
            if (auto connection = m_model->getConnectionForIface(iface->id()))
                storeLinkedEntity(connection);
        }
        break;
    }
    case AADLObject::Type::Function:
    case AADLObject::Type::FunctionType: {
        if (AADLObjectFunctionType *fnType = qobject_cast<AADLObjectFunctionType *>(toBeRemoved)) {
            for (auto iface : fnType->interfaces())
                collectRelatedItems(iface);

            for (auto child : toBeRemoved->findChildren<AADLObjectFunction *>(QString(), Qt::FindDirectChildrenOnly))
                collectRelatedItems(child);
        }
        break;
    }
    default: {
        for (auto child : toBeRemoved->findChildren<AADLObject *>(QString(), Qt::FindDirectChildrenOnly))
            collectRelatedItems(child);
        break;
    }
    }

    storeLinkedEntity(toBeRemoved);
}

void CmdEntityRemove::storeLinkedEntity(AADLObject *linkedEntity)
{
    auto storeObject = [](AADLObject *obj, QVector<QPointer<AADLObject>> *collection) {
        if (obj && !collection->contains(obj))
            collection->append(obj);
    };

    QVector<QPointer<AADLObject>> *pCollection { nullptr };
    switch (linkedEntity->aadlType()) {
    case AADLObject::Type::Connection:
        pCollection = &m_relatedConnections;
        break;
    case AADLObject::Type::Interface:
        pCollection = &m_relatedIfaces;
        break;
    default:
        pCollection = &m_relatedEntities;
        break;
    }
    storeObject(linkedEntity, pCollection);
}

} // namespace cmd
} // namespace aadl
} // namespace taste3
