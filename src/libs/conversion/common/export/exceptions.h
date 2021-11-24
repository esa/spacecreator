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

namespace conversion::exporter {

/**
 * @brief   Base class for all exceptions that occur while exporting
 */
class ExportException : public ConversionException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   message     Error message
     */
    explicit ExportException(QString message);
};

/**
 * @brief   Exception thrown when exporters get null model
 */
class NullModelException final : public ExportException
{
public:
    /**
     * @brief   Constructor
     */
    NullModelException();
};

/**
 * @brief   Exception thrown when exporters get incorrect model
 */
class IncorrectModelException final : public ExportException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   expectedModelType   Expected model type
     * @param   receivedModelType   Received model type
     */
    explicit IncorrectModelException(ModelType expectedModelType, ModelType receivedModelType);
};

/**
 * @brief   Exception thrown when output file name is missing
 */
class MissingOutputFilenameException final : public ExportException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   message     Model type
     */
    explicit MissingOutputFilenameException(ModelType modelType);
};

/**
 * @brief   Exception thrown when element is not supported
 */
class UnsupportedElementException final : public ExportException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   elementName     Element name
     */
    explicit UnsupportedElementException(QString elementName);
};

} // namespace conversion::exporter
