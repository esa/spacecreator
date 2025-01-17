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

#include <conversion/common/exceptions.h>
#include <conversion/common/modeltype.h>

namespace conversion {

/**
 * @brief   Exception that is thrown when Converter encounters an error
 */
class ConverterException : public ConversionException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   message     Error message
     */
    explicit ConverterException(QString message);
};

/**
 * @brief   Exception thrown when registering capabilities for model failed
 */
class RegistrationFailedException final : public ConverterException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   modelType       Type of model for which registration failed
     */
    explicit RegistrationFailedException(ModelType modelType);
};

/**
 * @brief   Exception throw when given string name doesn't correspond to any known model type
 */
class InvalidModelNameException final : public ConverterException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   modelName   Model name that is invalid
     */
    explicit InvalidModelNameException(const QString &modelName);
};

} // namespace conversion::importer
