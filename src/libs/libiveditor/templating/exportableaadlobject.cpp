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

#include "exportableaadlobject.h"

#include "aadlobject.h"
#include "aadlconnection.h"
#include "aadlconnectiongroup.h"
#include "aadlfunctiontype.h"
#include "exportableaadlconnection.h"
#include "exportableaadlconnectiongroup.h"
#include "exportableaadlfunction.h"
#include "exportableaadliface.h"
#include "exportableproperty.h"

namespace ive {

ExportableAADLObject::ExportableAADLObject(const ivm::AADLObject *aadlObject)
    : AbstractExportableObject(aadlObject)
{
}

QString ExportableAADLObject::groupName() const
{
    const ivm::AADLObject *aadlObject = exportedObject<ivm::AADLObject>();
    switch (aadlObject->aadlType()) {
    case ivm::AADLObject::Type::InterfaceGroup:
        return {};
    case ivm::AADLObject::Type::Function:
    case ivm::AADLObject::Type::FunctionType:
        return QStringLiteral("Functions");
    case ivm::AADLObject::Type::RequiredInterface:
    case ivm::AADLObject::Type::ProvidedInterface:
        return QStringLiteral("Interfaces");
    case ivm::AADLObject::Type::Comment:
        return QStringLiteral("Comments");
    case ivm::AADLObject::Type::Connection:
        return QStringLiteral("Connections");
    case ivm::AADLObject::Type::ConnectionGroup:
        return QStringLiteral("ConnectionGroups");
    default:
        Q_UNREACHABLE();
    }
    return QString();
}

QString ExportableAADLObject::name() const
{
    return exportedObject<ivm::AADLObject>()->title();
}

/**
 * @brief ExportableAADLObject::createFrom creates appropriate exported class and casts to QVariant
 * @param aadlObject exported object
 * @return created exported object as QVariant
 */
QVariant ExportableAADLObject::createFrom(const ivm::AADLObject *aadlObject)
{
    switch (aadlObject->aadlType()) {
    case ivm::AADLObject::Type::InterfaceGroup:
        return {};
    case ivm::AADLObject::Type::Function:
    case ivm::AADLObject::Type::FunctionType:
        return QVariant::fromValue(
                ExportableAADLFunction(static_cast<const ivm::AADLFunctionType *>(aadlObject)));
    case ivm::AADLObject::Type::RequiredInterface:
    case ivm::AADLObject::Type::ProvidedInterface:
        return QVariant::fromValue(ExportableAADLIface(static_cast<const ivm::AADLIface *>(aadlObject)));
    case ivm::AADLObject::Type::Comment:
        return QVariant::fromValue(ExportableAADLObject(aadlObject));
    case ivm::AADLObject::Type::Connection:
        return QVariant::fromValue(
                ExportableAADLConnection(static_cast<const ivm::AADLConnection *>(aadlObject)));
    case ivm::AADLObject::Type::ConnectionGroup:
        return QVariant::fromValue(
                ExportableAADLConnectionGroup(static_cast<const ivm::AADLConnectionGroup *>(aadlObject)));
    default:
        Q_UNREACHABLE();
    }
    return QVariant();
}

/**
 * @brief ExportableAADLObject::attributes returns list of attributes for using in string templates.
 * @return list of attributes.
 */
QVariantList ExportableAADLObject::attributes() const
{
    return generateProperties(exportedObject<ivm::AADLObject>()->attrs());
}

/**
 * @brief ExportableAADLObject::properties returns list of properties for using in string templates.
 * @return list of properties.
 */
QVariantList ExportableAADLObject::properties() const
{
    return generateProperties(exportedObject<ivm::AADLObject>()->props());
}

QStringList ExportableAADLObject::path() const
{
    return exportedObject<ivm::AADLObject>()->path();
}

/**
 * @brief ExportableAADLObject::generateProperties generates a variant list sorted by meta::Props::Token.
 * @param props can be hash of attributes or properties of AADLObject.
 * @return sorted QVariantList which can be used in string templates
 */
QVariantList ExportableAADLObject::generateProperties(const QHash<QString, QVariant> &props)
{
    QVariantList result;
    for (auto it = props.cbegin(); it != props.cend(); ++it)
        result << QVariant::fromValue(templating::ExportableProperty(it.key(), it.value()));

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
