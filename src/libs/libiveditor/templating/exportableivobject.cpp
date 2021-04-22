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

#include "exportableivobject.h"

#include "exportableivconnection.h"
#include "exportableivconnectiongroup.h"
#include "exportableivfunction.h"
#include "exportableivinterface.h"
#include "exportableproperty.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunctiontype.h"
#include "ivobject.h"

namespace ive {

ExportableIVObject::ExportableIVObject(const ivm::IVObject *ivObject)
    : AbstractExportableObject(ivObject)
{
}

QString ExportableIVObject::groupName() const
{
    const ivm::IVObject *ivObject = exportedObject<ivm::IVObject>();
    switch (ivObject->type()) {
    case ivm::IVObject::Type::InterfaceGroup:
        return {};
    case ivm::IVObject::Type::Function:
    case ivm::IVObject::Type::FunctionType:
        return QStringLiteral("Functions");
    case ivm::IVObject::Type::RequiredInterface:
    case ivm::IVObject::Type::ProvidedInterface:
        return QStringLiteral("Interfaces");
    case ivm::IVObject::Type::Comment:
        return QStringLiteral("Comments");
    case ivm::IVObject::Type::Connection:
        return QStringLiteral("Connections");
    case ivm::IVObject::Type::ConnectionGroup:
        return QStringLiteral("ConnectionGroups");
    default:
        Q_UNREACHABLE();
    }
    return QString();
}

QString ExportableIVObject::name() const
{
    return exportedObject<ivm::IVObject>()->title();
}

/**
 * @brief ExportableIVObject::createFrom creates appropriate exported class and casts to QVariant
 * @param ivObject exported object
 * @return created exported object as QVariant
 */
QVariant ExportableIVObject::createFrom(const ivm::IVObject *ivObject)
{
    switch (ivObject->type()) {
    case ivm::IVObject::Type::InterfaceGroup:
        return {};
    case ivm::IVObject::Type::Function:
    case ivm::IVObject::Type::FunctionType:
        return QVariant::fromValue(ExportableIVFunction(static_cast<const ivm::IVFunctionType *>(ivObject)));
    case ivm::IVObject::Type::RequiredInterface:
    case ivm::IVObject::Type::ProvidedInterface:
        return QVariant::fromValue(ExportableIVInterface(static_cast<const ivm::IVInterface *>(ivObject)));
    case ivm::IVObject::Type::Comment:
        return QVariant::fromValue(ExportableIVObject(ivObject));
    case ivm::IVObject::Type::Connection:
        return QVariant::fromValue(ExportableIVConnection(static_cast<const ivm::IVConnection *>(ivObject)));
    case ivm::IVObject::Type::ConnectionGroup:
        return QVariant::fromValue(ExportableIVConnectionGroup(static_cast<const ivm::IVConnectionGroup *>(ivObject)));
    default:
        Q_UNREACHABLE();
    }
    return QVariant();
}

/**
 * @brief ExportableIVObject::attributes returns list of attributes for using in string templates.
 * @return list of attributes.
 */
QVariantList ExportableIVObject::attributes() const
{
    return generateProperties(exportedObject<ivm::IVObject>()->entityAttributes(), false);
}

/**
 * @brief ExportableIVObject::properties returns list of properties for using in string templates.
 * @return list of properties.
 */
QVariantList ExportableIVObject::properties() const
{
    return generateProperties(exportedObject<ivm::IVObject>()->entityAttributes(), true);
}

QStringList ExportableIVObject::path() const
{
    return exportedObject<ivm::IVObject>()->path();
}

/**
 * @brief ExportableIVObject::generateProperties generates a variant list sorted by meta::Props::Token.
 * @param props can be hash of attributes or properties of IVObject.
 * @return sorted QVariantList which can be used in string templates
 */
QVariantList ExportableIVObject::generateProperties(const EntityAttributes &attributes, bool isProperty)
{
    QVariantList result;
    for (auto it = attributes.cbegin(); it != attributes.cend(); ++it) {
        if (it.value().isProperty() == isProperty) {
            result << QVariant::fromValue(templating::ExportableProperty(it.key(), it.value().value()));
        }
    }

    std::sort(result.begin(), result.end(), [](const QVariant &left_val, const QVariant &right_val) {
        const auto &r = right_val.value<templating::ExportableProperty>();
        const ivm::meta::Props::Token right_token = ivm::meta::Props::token(r.name());
        if (right_token == ivm::meta::Props::Token::Unknown)
            return true;

        const auto &l = left_val.value<templating::ExportableProperty>();
        const ivm::meta::Props::Token left_token = ivm::meta::Props::token(l.name());
        if (left_token == ivm::meta::Props::Token::Unknown)
            return false;

        return left_token < right_token;
    });

    return result;
}

}
