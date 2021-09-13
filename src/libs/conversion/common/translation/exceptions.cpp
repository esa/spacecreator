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

IncorrectSourceModelException::IncorrectSourceModelException(ModelType expectedModelType, ModelType receivedModelType)
    : TranslationException(QString("Source model not supported, expected %1 model, got %2 model")
                                   .arg(modelTypeToString(expectedModelType))
                                   .arg(modelTypeToString(receivedModelType)))
{
}

UndeclaredDataTypeException::UndeclaredDataTypeException(const QString &dataTypeName)
    : TranslationException(QString("Undeclared data type '%1'").arg(dataTypeName))
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

UnhandledValueException::UnhandledValueException(const QString &valueTypeName)
    : TranslationException(QString("Translator encountered unhandled value for %1").arg(valueTypeName))
{
}

UnsupportedValueException::UnsupportedValueException(const QString &valueTypeName)
    : TranslationException(QString("Translator encountered unsupported value for %1").arg(valueTypeName))
{
}

} // namespace conversion::translator
