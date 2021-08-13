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
#include <conversion/common/import/exceptions.h>

namespace seds::parser {

/**
 * @brief   Exception thrown when an error was encountered during parsing
 */
class ParserException : public conversion::importer::ImportException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   message     Error message
     */
    ParserException(QString message);
};

/**
 * @brief   Exception thrown when parser encounters an element that isn't handled in current element
 */
class UnhandledElement : public ParserException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   encounteredType     Type of the encountered element
     * @param   elementType         Type of the element in which unhandled element was encountered in
     */
    UnhandledElement(const QStringRef &encounteredType, const QString &elementType);
};

/**
 * @brief   Exception thrown when parser encounters an attribute that isn't handled in current element
 */
class UnhandledAttribute : public ParserException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   attributeName   Encountered attribute
     * @param   elementType     Type of the element in which unhandled attribute was encountered in
     */
    UnhandledAttribute(const QStringRef &attributeName, const QStringRef &elementType);
};

/**
 * @brief   Exception thrown when unexpected EOF occured
 */
class UnexpectedEOF : public ParserException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   elementType     Type of the element in which EOF was encountered in
     */
    UnexpectedEOF(const QStringRef &elementType);
};

} // namespace seds::parser
