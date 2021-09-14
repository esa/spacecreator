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

#include "export/exceptions.h"

namespace conversion::exporter {

ExportException::ExportException(QString message)
    : ConversionException(std::move(message))
{
}

IncorrectModelException::IncorrectModelException(ModelType expectedModelType, ModelType receivedModelType)
    : ExportException(QString("Incorrect model passed, expected %1 model, got %2 model")
                              .arg(modelTypeToString(expectedModelType))
                              .arg(modelTypeToString(receivedModelType)))
{
}

IncorrectSourceModelException::IncorrectSourceModelException(ModelType expectedModelType, ModelType receivedModelType)
    : ExportException(QString("Source model not supported, expected %1 model, got %2 model")
                              .arg(modelTypeToString(expectedModelType))
                              .arg(modelTypeToString(receivedModelType)))
{
}

} // namespace conversion::exporter
