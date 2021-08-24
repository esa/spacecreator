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

#include <QString>
#include <set>
#include <stdexcept>

class QString;

namespace conversion {

enum class ModelType;

/**
 * @brief   Base class for conversion exceptions
 */
class ConversionException : public std::exception
{
public:
    /**
     * @brief   Constructor
     *
     * @param   message     Error message
     */
    explicit ConversionException(QString message);

public:
    /**
     * @brief   Returns error message as const char*
     *
     * @returns Error message
     */
    virtual auto what() const noexcept -> const char * override;
    /**
     * @brief   Returns error message as QString
     *
     * @returns Error message
     */
    auto errorMessage() const noexcept -> const QString &;

protected:
    /**
     * @brief   Default constructor, only for inheriting classes
     */
    ConversionException() = default;

protected:
    /** @brief  Error message */
    QString m_message;
};

/**
 * @brief   Exception thrown when required file doesn't exist
 */
class FileNotFoundException final : public ConversionException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   filename    File that is missing
     * @param   message     Additional error message
     */
    FileNotFoundException(const QString &filename, const QString &message);
};

/**
 * @brief   Exception thrown when registering capabilities for model failed
 */
class RegistrationFailedException : public ConversionException
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
 * @brief   Exception thrown when importer for given model type wasn't registered
 */
class ImporterNotRegisteredException : public ConversionException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   modelType       Model type
     */
    explicit ImporterNotRegisteredException(ModelType modelType);
};

/**
 * @brief   Exception thrown when translator for given model type wasn't registered
 */
class TranslatorNotRegisteredException : public ConversionException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sourceModelsTypes   Set of source models types
     * @param   targetModelType     Target model type
     */
    TranslatorNotRegisteredException(const std::set<ModelType> &sourceModelTypes, ModelType targetModelType);

private:
    auto sourceModelsTypesToString(const std::set<ModelType> &sourceModelsTypes) -> QString;
};

/**
 * @brief   Exception thrown when exporter for given model type wasn't registered
 */
class ExporterNotRegisteredException : public ConversionException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   modelType       Model type
     */
    explicit ExporterNotRegisteredException(ModelType modelType);
};

} // namespace conversion
