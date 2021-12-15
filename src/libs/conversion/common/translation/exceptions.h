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

#pragma once

#include "../exceptions.h"
#include "modeltype.h"

#include <set>

namespace conversion::translator {

/**
 * @brief   Base class for all exceptions that occur while importing
 */
class TranslationException : public ConversionException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   message     Error message
     */
    explicit TranslationException(QString message);
};

/**
 * @brief   Exception thrown when translator encounters a data type that wasn't declared
 */
class UndeclaredDataTypeException : public TranslationException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   dataTypeName    Name of the undeclared type
     */
    explicit UndeclaredDataTypeException(const QString &dataTypeName);
};

/**
 * @brief   Exception thrown when translator encounters a data type that is not supported
 */
class UnsupportedDataTypeException : public TranslationException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   dataTypeName    Name of the unsupported type
     */
    explicit UnsupportedDataTypeException(const QString &dataTypeName);
};

/**
 * @brief   Exception thrown when translator encounters an interface that wasn't declared
 */
class UndeclaredInterfaceException : public TranslationException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   interfaceTypeName   Name of the type of the undeclared interface
     */
    explicit UndeclaredInterfaceException(const QString &interfaceTypeName);
};

/**
 * @brief   Exception thrown when interface parameter type wasn't mapped for given interface
 */
class MissingGenericTypeMappingException : public TranslationException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   genericTypeName     Type that wasn't mapped
     * @param   interfaceName       Name of interface with the unmapped type
     */
    explicit MissingGenericTypeMappingException(const QString &genericTypeName, const QString &interfaceName);
};

/**
 * @brief   Thrown when SEDS references an ASN.1 type (ie. as an entry type) but the definition is not present
 *          in the ASN.1 model
 */
class MissingAsn1TypeDefinitionException : public TranslationException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   asn1TypeName    Name of the missing ASN.1 type
     */
    explicit MissingAsn1TypeDefinitionException(const QString &asn1TypeName);
};

/**
 * @brief   Thrown when SEDS references a Function which is not found in the InterfaceView
 */
class MissingInterfaceViewFunctionException : public TranslationException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   functionName    Name of the missing Function
     */
    explicit MissingInterfaceViewFunctionException(const QString &functionName);
};

} // namespace conversion::translator
