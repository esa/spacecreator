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
 * @brief   Exception thrown when translator gets unsupported model
 */
class IncorrectSourceModelException : public TranslationException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   expectedModelType   Expected model type
     * @param   receivedModelType   Received model type
     */
    explicit IncorrectSourceModelException(ModelType expectedModelType, ModelType receivedModelType);
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
     * @param   dataTypeName    Name of undeclared the type
     */
    explicit UndeclaredDataTypeException(const QString &dataTypeName);
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
 * @brief   Exception thrown when translator encounters unhandled value in source model
 */
class UnhandledValueException : public TranslationException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   valueTypeName   Name of the unhandled value type
     */
    explicit UnhandledValueException(const QString &valueTypeName);
};

/**
 * @brief   Exception thrown when translator encounters unsupported value in source model
 */
class UnsupportedValueException : public TranslationException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   valueTypeName   Name of the unsupported value type
     */
    explicit UnsupportedValueException(const QString &valueTypeName);
};

} // namespace conversion::translator
