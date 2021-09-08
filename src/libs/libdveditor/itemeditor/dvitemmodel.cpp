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

#include "dvitemmodel.h"

#include "dvconnection.h"
#include "dvconnectiongraphicsitem.h"
#include "dvdevice.h"
#include "dvdevicegraphicsitem.h"
#include "dvnode.h"
#include "dvnodegraphicsitem.h"
#include "dvpartition.h"
#include "dvpartitiongraphicsitem.h"
#include "interface/veitemmodel.h"
#include "ui/veinteractiveobject.h"

#include <QtDebug>

namespace dve {

DVItemModel::DVItemModel(dvm::DVModel *model, shared::cmd::CommandsStackBase *commandsStack, QObject *parent)
    : shared::ui::VEItemModel(model, commandsStack, parent)
{
}

DVItemModel::~DVItemModel() { }

dvm::DVModel *DVItemModel::objectsModel() const
{
    return qobject_cast<dvm::DVModel *>(m_model);
}

shared::ui::VEInteractiveObject *DVItemModel::createItem(shared::Id objectId)
{
    auto obj = objectsModel()->getObject(objectId);
    Q_ASSERT(obj);
    if (!obj) {
        return nullptr;
    }

    QGraphicsItem *parentItem = obj->parentObject() ? m_items.value(obj->parentObject()->id()) : nullptr;

    switch (obj->type()) {
    case dvm::DVObject::Type::Function: {
        if (dvm::DVObject *parent = obj->parentObject()) {
            if (QGraphicsItem *gi = getItem(parent->id())) {
                gi->update();
            }
        }
    } break;
    case dvm::DVObject::Type::Node: {
        return new DVNodeGraphicsItem(obj->as<dvm::DVNode *>(), parentItem);
    } break;
    case dvm::DVObject::Type::Partition: {
        return new DVPartitionGraphicsItem(obj->as<dvm::DVPartition *>(), parentItem);
    } break;
    case dvm::DVObject::Type::Device: {
        return new DVDeviceGraphicsItem(obj->as<dvm::DVDevice *>(), parentItem);
    } break;
    case dvm::DVObject::Type::Connection: {
        if (auto connectionEntity = obj->as<dvm::DVConnection *>()) {
            DVDeviceGraphicsItem *startItem = getItem<DVDeviceGraphicsItem *>(connectionEntity->sourceDevice()->id());
            DVDeviceGraphicsItem *endItem = getItem<DVDeviceGraphicsItem *>(connectionEntity->targetDevice()->id());
            return new DVConnectionGraphicsItem(connectionEntity, startItem, endItem, parentItem);
        }
    } break;
    case dvm::DVObject::Type::Bus:
    case dvm::DVObject::Type::Message: {
        break;
    }
    default: {
        qCritical() << "Unknown object type:" << obj->type();
        break;
    }
    }

    return {};
}

} // namespace dve
