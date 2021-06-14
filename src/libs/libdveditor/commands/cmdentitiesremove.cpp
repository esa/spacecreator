/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvfunction.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "dvobject.h"
#include "dvpartition.h"

namespace dve {
namespace cmd {

CmdEntitiesRemove::CmdEntitiesRemove(const QList<QPointer<dvm::DVObject>> &entities, dvm::DVModel *model)
    : shared::UndoCommand()
    , m_model(model)
    , m_entities(entities)
{
    setText(tr("Remove selected item(s)"));

    for (const QPointer<dvm::DVObject> &entity : entities) {
        collectRelatedItems(entity);
    }
}

CmdEntitiesRemove::~CmdEntitiesRemove()
{
    qDeleteAll(m_subCommands);
    m_subCommands.clear();
}

void CmdEntitiesRemove::advancedRemove(dvm::DVObject *obj)
{
    if (!obj)
        return;

    dvm::DVObject *parent = putParentFor(obj);
    if (m_model)
        m_model->removeObject(obj);

    if (parent) {
        switch (parent->type()) {
        case dvm::DVObject::Type::Device:
            if (auto node = parent->as<dvm::DVNode *>()) {
                node->removeDevice(obj->as<dvm::DVDevice *>());
            }
            break;
        case dvm::DVObject::Type::Partition:
            if (auto node = parent->as<dvm::DVNode *>()) {
                node->removePartition(obj->as<dvm::DVPartition *>());
            }
            break;
        case dvm::DVObject::Type::Function:
            if (auto partition = parent->as<dvm::DVPartition *>()) {
                partition->removeFunction(obj->as<dvm::DVFunction *>());
            }
            break;
        default:
            break;
        }
    }
    obj->setParent(this);
}

void CmdEntitiesRemove::advancedRestore(dvm::DVObject *obj)
{
    if (!obj)
        return;

    dvm::DVObject *parent = popParentFor(obj);
    if (parent) {
        switch (parent->type()) {
        case dvm::DVObject::Type::Device:
            if (auto node = parent->as<dvm::DVNode *>()) {
                node->addDevice(obj->as<dvm::DVDevice *>());
            }
            break;
        case dvm::DVObject::Type::Partition:
            if (auto node = parent->as<dvm::DVNode *>()) {
                node->addPartition(obj->as<dvm::DVPartition *>());
            }
            break;
        case dvm::DVObject::Type::Function:
            if (auto partition = parent->as<dvm::DVPartition *>()) {
                partition->addFunction(obj->as<dvm::DVFunction *>());
            }
            break;
        default:
            break;
        }
    }
    obj->setParentObject(parent);

    if (m_model)
        m_model->addObject(obj);
}

void CmdEntitiesRemove::redo()
{
    if (!m_model)
        return;

    auto removeDVObjects = [this](const QVector<QPointer<dvm::DVObject>> &collection) {
        for (auto it = collection.cbegin(); it != collection.cend(); ++it) {
            advancedRemove(*it);
        }
    };

    for (auto it = m_subCommands.begin(); it != m_subCommands.end(); ++it) {
        (*it)->redo();
    }

    removeDVObjects(m_relatedEntities);
}

void CmdEntitiesRemove::undo()
{
    if (!m_model)
        return;

    auto restoreDVObjects = [this](const QVector<QPointer<dvm::DVObject>> &collection) {
        for (auto it = collection.crbegin(); it != collection.crend(); ++it) {
            advancedRestore(*it);
        }
    };

    restoreDVObjects(m_relatedEntities);
    for (auto it = m_subCommands.rbegin(); it != m_subCommands.rend(); ++it) {
        (*it)->undo();
    }
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

dvm::DVObject *CmdEntitiesRemove::putParentFor(const dvm::DVObject *obj)
{
    if (!obj || !obj->parentObject())
        return nullptr;

    const shared::Id &objId = obj->id();
    if (auto *fn = obj->parentObject()->as<dvm::DVObject *>())
        m_parentObjects[objId] = fn;

    return m_parentObjects[objId];
}

dvm::DVObject *CmdEntitiesRemove::popParentFor(const dvm::DVObject *obj)
{
    return obj ? m_parentObjects.take(obj->id()) : nullptr;
}

void CmdEntitiesRemove::collectRelatedItems(dvm::DVObject *toBeRemoved)
{
    if (!toBeRemoved)
        return;

    switch (toBeRemoved->type()) {
    case dvm::DVObject::Type::Board:
    case dvm::DVObject::Type::Function:
    case dvm::DVObject::Type::Connection:
    case dvm::DVObject::Type::Unknown:
        break;
    case dvm::DVObject::Type::Node:
        if (auto node = toBeRemoved->as<dvm::DVNode *>()) {
            for (auto partition : node->partitions()) {
                collectRelatedItems(partition);
            }
            for (auto device : node->devices()) {
                collectRelatedItems(device);
            }
        }
        break;
    case dvm::DVObject::Type::Partition:
        if (auto partition = toBeRemoved->as<dvm::DVPartition *>()) {
            for (auto function : partition->functions()) {
                collectRelatedItems(function);
            }
        }
        break;
    case dvm::DVObject::Type::Device:
        for (auto connection : m_model->allObjectsByType<dvm::DVConnection>()) {
            if ((connection->sourceDevice() && connection->sourceDevice()->id() == toBeRemoved->id())
                    || (connection->targetDevice() && connection->targetDevice()->id() == toBeRemoved->id())) {
                collectRelatedItems(connection);
            }
        }
        break;
    default:
        for (auto child : toBeRemoved->findChildren<dvm::DVObject *>(QString(), Qt::FindDirectChildrenOnly))
            collectRelatedItems(child);
        break;
    }
    m_relatedEntities.append(toBeRemoved);
}

} // namespace cmd
} // namespace dve
