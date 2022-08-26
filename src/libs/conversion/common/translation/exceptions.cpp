/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "translation/exceptions.h"

namespace conversion::translator {

TranslationException::TranslationException(QString message)
    : ConversionException(std::move(message))
{
}

UndeclaredPackageReferenceException::UndeclaredPackageReferenceException(const QString &packageName)
    : TranslationException(QString("Undeclared package '%1'").arg(packageName))
{
}

UndeclaredDataTypeException::UndeclaredDataTypeException(const QString &dataTypeName)
    : TranslationException(QString("Undeclared data type '%1'").arg(dataTypeName))
{
}

UnsupportedDataTypeException::UnsupportedDataTypeException(const QString &dataTypeName)
    : TranslationException(QString("Unsupported data type '%1'").arg(dataTypeName))
{
}

UndeclaredInterfaceException::UndeclaredInterfaceException(const QString &interfaceTypeName)
    : TranslationException(QString("Undeclared interface '%1'").arg(interfaceTypeName))
{
}

MissingGenericTypeMappingException::MissingGenericTypeMappingException(
        const QString &genericTypeName, const QString &interfaceName)
    : TranslationException(
            QString("Generic type '%1' wasn't mapped in the '%2' interface").arg(genericTypeName).arg(interfaceName))
{
}

MissingAsn1TypeDefinitionException::MissingAsn1TypeDefinitionException(const QString &asn1TypeName)
    : TranslationException(QString("Missing ASN.1 type definition '%1'").arg(asn1TypeName))
{
}

MissingInterfaceViewFunctionException::MissingInterfaceViewFunctionException(const QString &functionName)
    : TranslationException(QString("Function %1 not found in the InterfaceView").arg(functionName))
{
}

NotDagException::NotDagException(const QString &entityName)
    : TranslationException(QString("Detected cyclic dependency while handling entity \"%1\"").arg(entityName))
{
}

} // namespace conversion::translator
