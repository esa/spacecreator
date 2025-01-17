/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

namespace simulink::importer {

ParserException::ParserException(common::String message)
    : ImportException(std::move(message))
{
}

UnhandledElement::UnhandledElement(const QStringView &encounteredType, const common::String &elementType)
    : ParserException(QString("Unhandled element <%1> in <%2>").arg(encounteredType).arg(elementType))
{
}

UnhandledAttribute::UnhandledAttribute(const QStringView &attributeName, const QStringView &elementType)
    : ParserException(QString("Unhandled attribute [%1] in element <%2>").arg(attributeName).arg(elementType))
{
}

UnexpectedEof::UnexpectedEof(const QStringView &elementType)
    : ParserException(QString("Unexpected EOF in <%1>").arg(elementType))
{
}

} // namespace simulink::importer
