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

#include "dvbinding.h"
#include "dvbus.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvnode.h"
#include "dvobject.h"
#include "dvpartition.h"
#include "dvprocessor.h"
#include "exportabledvbinding.h"
#include "exportabledvbus.h"
#include "exportabledvconnection.h"
#include "exportabledvdevice.h"
#include "exportabledvnode.h"
#include "exportabledvpartition.h"
#include "exportabledvprocessor.h"
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
    return generateProperties(exportedObject<dvm::DVObject>()->entityAttributes(), false);
}

/**
 * @brief ExportableDVObject::properties returns list of properties for using in string templates.
 * @return list of properties.
 */
QVariantList ExportableDVObject::properties() const
{
    return generateProperties(exportedObject<dvm::DVObject>()->entityAttributes(), true);
}

QVariant ExportableDVObject::createFrom(const dvm::DVObject *dvObject)
{
    switch (dvObject->type()) {
    case dvm::DVObject::Type::Unknown:
        break;
    case dvm::DVObject::Type::Node:
        return QVariant::fromValue(ExportableDVNode(static_cast<const dvm::DVNode *>(dvObject)));
    case dvm::DVObject::Type::Processor:
        return QVariant::fromValue(ExportableDVProcessor(static_cast<const dvm::DVProcessor *>(dvObject)));
    case dvm::DVObject::Type::Partition:
        return QVariant::fromValue(ExportableDVPartition(static_cast<const dvm::DVPartition *>(dvObject)));
    case dvm::DVObject::Type::Binding:
        return QVariant::fromValue(ExportableDVBinding(static_cast<const dvm::DVBinding *>(dvObject)));
    case dvm::DVObject::Type::Device:
        return QVariant::fromValue(ExportableDVDevice(static_cast<const dvm::DVDevice *>(dvObject)));
    case dvm::DVObject::Type::Bus:
        return QVariant::fromValue(ExportableDVBus(static_cast<const dvm::DVBus *>(dvObject)));
    case dvm::DVObject::Type::Connection:
        return QVariant::fromValue(ExportableDVConnection(static_cast<const dvm::DVConnection *>(dvObject)));
    default:
        break;
    }
    return QVariant();
}

/**
 * @brief ExportableDVObject::generateProperties generates a variant list sorted by meta::Props::Token.
 * @param props can be hash of attributes or properties of DVObject.
 * @return sorted QVariantList which can be used in string templates
 */
QVariantList ExportableDVObject::generateProperties(const EntityAttributes &attributes, bool isProperty)
{
    QVariantList result;
    for (auto it = attributes.cbegin(); it != attributes.cend(); ++it) {
        if (it.value().isProperty() == isProperty) {
            result << QVariant::fromValue(templating::ExportableProperty(it.key(), it.value().value()));
        }
    }

    std::sort(result.begin(), result.end(), [](const QVariant &left_val, const QVariant &right_val) {
        const auto &r = right_val.value<templating::ExportableProperty>();
        const dvm::meta::Props::Token right_token = dvm::meta::Props::token(r.name());
        if (right_token == dvm::meta::Props::Token::Unknown)
            return true;

        const auto &l = left_val.value<templating::ExportableProperty>();
        const dvm::meta::Props::Token left_token = dvm::meta::Props::token(l.name());
        if (left_token == dvm::meta::Props::Token::Unknown)
            return false;

        return left_token < right_token;
    });

    return result;
}

}
