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

#include "exceptions.h"

#include "modeltype.h"

namespace conversion {

ConversionException::ConversionException(QString message)
    : m_message(std::move(message))
{
}

const char *ConversionException::what() const noexcept
{
    return m_message.toUtf8().constData();
}

const QString &ConversionException::errorMessage() const noexcept
{
    return m_message;
}

FileNotFoundException::FileNotFoundException(const QString &filename, const QString &message)
    : ConversionException(QString("File '%1' not found: '%2'").arg(filename).arg(message))
{
}

RegistrationFailedException::RegistrationFailedException(ModelType modelType)
    : ConversionException(QString("Unable to register for %1 model").arg(modelTypeToString(modelType)))
{
}

ImporterNotRegisteredException::ImporterNotRegisteredException(ModelType modelType)
    : ConversionException(QString("No importer for %1 model registered").arg(modelTypeToString(modelType)))
{
}

TranslatorNotRegisteredException::TranslatorNotRegisteredException(
        const std::set<ModelType> &sourceModelsTypes, ModelType targetModelType)
    : ConversionException(QString("No translator from { %1 } models to %2 model registered")
                                  .arg(modelTypesToString(sourceModelsTypes))
                                  .arg(modelTypeToString(targetModelType)))
{
}

ExporterNotRegisteredException::ExporterNotRegisteredException(ModelType modelType)
    : ConversionException(QString("No exporter for %1 model registered").arg(modelTypeToString(modelType)))
{
}

InvalidModelNameException::InvalidModelNameException(const QString &modelName)
    : ConversionException(QString("Model name '%1' is not valid").arg(modelName))
{
}

} // namespace conversion
