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
class IncorrectSourceModelException : public ConversionException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   expectedModelType   Expected model type
     */
    explicit IncorrectSourceModelException(ModelType expectedModelType);
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
    explicit UnhandledValueException(QString valueTypeName);
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
    explicit UnsupportedValueException(QString valueTypeName);
};

} // namespace conversion::translator
