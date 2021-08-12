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

#include <QStringList>

namespace seds::parser {

ParserException::ParserException(QString message)
    : ImportException(std::move(message))
{
}

UnhandledElement::UnhandledElement(const QStringRef &encounteredType, const QString &elementType)
    : ParserException(QString("Unhandled element <%1> in <%2>").arg(encounteredType).arg(elementType))
{
}

UnhandledAttribute::UnhandledAttribute(const QStringRef &attributeName, const QStringRef &elementType)
    : ParserException(QString("Unhandled attribute [%1] in element <%2>").arg(attributeName).arg(elementType))
{
}

UnexpectedEOF::UnexpectedEOF(const QStringRef &elementType)
    : ParserException(QString("Unexpected EOF in <%1>").arg(elementType))
{
}

} // namespace seds::parser
