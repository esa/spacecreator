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

#include "exportabledvobject.h"

#include "dvbus.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvmessage.h"
#include "dvnode.h"
#include "dvobject.h"
#include "dvpartition.h"
#include "dvsystemfunction.h"
#include "dvsysteminterface.h"
#include "exportabledvbus.h"
#include "exportabledvconnection.h"
#include "exportabledvdevice.h"
#include "exportabledvmessage.h"
#include "exportabledvnode.h"
#include "exportabledvpartition.h"
#include "exportableproperty.h"

namespace dve {

ExportableDVObject::ExportableDVObject(const dvm::DVObject *dvObject)
    : templating::AbstractExportableObject(dvObject)
{
}

QString ExportableDVObject::name() const
{
    return exportedObject<dvm::DVObject>()->title();
}

/**
 * @brief ExportableDVObject::attributes returns list of attributes for using in string templates.
 * @return list of attributes.
 */
QVariantList ExportableDVObject::attributes() const
{
    return exportedObject<dvm::DVObject>()->generateProperties(false);
}

/**
 * @brief ExportableDVObject::properties returns list of properties for using in string templates.
 * @return list of properties.
 */
QVariantList ExportableDVObject::properties() const
{
    return exportedObject<dvm::DVObject>()->generateProperties(true);
}

QVariant ExportableDVObject::createFrom(const dvm::DVObject *dvObject)
{
    switch (dvObject->type()) {
    case dvm::DVObject::Type::Unknown:
        break;
    case dvm::DVObject::Type::Node:
        return QVariant::fromValue(ExportableDVNode(static_cast<const dvm::DVNode *>(dvObject)));
    case dvm::DVObject::Type::Partition:
        return QVariant::fromValue(ExportableDVPartition(static_cast<const dvm::DVPartition *>(dvObject)));
    case dvm::DVObject::Type::Device:
        return QVariant::fromValue(ExportableDVDevice(static_cast<const dvm::DVDevice *>(dvObject)));
    case dvm::DVObject::Type::Connection:
        return QVariant::fromValue(ExportableDVConnection(static_cast<const dvm::DVConnection *>(dvObject)));
    case dvm::DVObject::Type::Message:
        return QVariant::fromValue(ExportableDVMessage(static_cast<const dvm::DVMessage *>(dvObject)));
    case dvm::DVObject::Type::Bus:
        return QVariant::fromValue(ExportableDVBus(static_cast<const dvm::DVBus *>(dvObject)));
    case dvm::DVObject::Type::SystemFunction:
        return QVariant::fromValue(
                const_cast<dvm::DVSystemFunction *>(static_cast<const dvm::DVSystemFunction *>(dvObject)));
    case dvm::DVObject::Type::SystemInterface:
        return QVariant::fromValue(
                const_cast<dvm::DVSystemInterface *>(static_cast<const dvm::DVSystemInterface *>(dvObject)));
    default:
        break;
    }
    return QVariant();
}

}
