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

#include "exportedaadlobject.h"

#include "exportedaadlproperty.h"
#include "exportedaadlfunction.h"
#include "exportedaadliface.h"
#include "exportedaadlconnection.h"
#include "tab_aadl/aadlobject.h"

namespace taste3 {
namespace templating {

ExportedAADLObject::ExportedAADLObject(const aadl::AADLObject *aadlObject)
    : GenericExportedObject(aadlObject) {}

/**
 * @brief ExportedAADLObject::createFrom creates appropriate exported class and casts to QVariant
 * @param aadlObject exported object
 * @return created exported object as QVariant
 */
QVariant ExportedAADLObject::createFrom(const aadl::AADLObject *aadlObject)
{
    switch (aadlObject->aadlType()) {
    case aadl::AADLObject::Type::Function:
    case aadl::AADLObject::Type::FunctionType:
        return QVariant::fromValue(ExportedAADLFunction(aadlObject));
    case aadl::AADLObject::Type::RequiredInterface:
    case aadl::AADLObject::Type::ProvidedInterface:
        return QVariant::fromValue(TemplatedAADLIface(aadlObject));
    case aadl::AADLObject::Type::Comment:
        return QVariant::fromValue(ExportedAADLObject(aadlObject));
    case aadl::AADLObject::Type::Connection:
        return QVariant::fromValue(ExportedAADLConnection(aadlObject));
    default:
        Q_UNREACHABLE();
    }
    return QVariant();
}

/**
 * @brief ExportedAADLObject::attributes returns list of attribues for using in string templates.
 * @return list of attribues.
 */
QVariantList ExportedAADLObject::attributes() const
{
    return generateProperties(exportedObject<aadl::AADLObject>()->attrs());
}

/**
 * @brief ExportedAADLObject::properties returns list of properties for using in string templates.
 * @return list of properties.
 */
QVariantList ExportedAADLObject::properties() const
{
    return generateProperties(exportedObject<aadl::AADLObject>()->props());
}

/**
 * @brief ExportedAADLObject::generateProperties generates a variant list sorted by meta::Props::Token.
 * @param props can be hash of attributes or properties of AADLObject.
 * @return sorted QVariantList which can be used in string templates
 */
QVariantList ExportedAADLObject::generateProperties(const QHash<QString, QVariant> &props)
{
    QVariantList result;
    for (auto it = props.cbegin(); it != props.cend(); ++it)
        result << QVariant::fromValue(ExportedAADLProperty(it.key(), it.value()));

    std::sort(result.begin(), result.end(), [] (const QVariant &left_val, const QVariant &right_val) {
        const ExportedAADLProperty &r = right_val.value<ExportedAADLProperty>();
        const aadl::meta::Props::Token right_token = aadl::meta::Props::token(r.name());
        if (right_token == aadl::meta::Props::Token::Unknown)
            return true;

        const ExportedAADLProperty &l = left_val.value<ExportedAADLProperty>();
        const aadl::meta::Props::Token left_token = aadl::meta::Props::token(l.name());
        if (left_token == aadl::meta::Props::Token::Unknown)
            return false;

        return left_token < right_token;
    });

    return result;
}

} // ns templating
} // ns taste3
