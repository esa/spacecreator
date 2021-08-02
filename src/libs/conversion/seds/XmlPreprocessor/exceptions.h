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
#include <converter/import/exceptions.h>

namespace seds::preprocessor {

/**
 * @brief   Exception thrown when an error was encountered during preprocessing
 */
class XmlPreprocessorException : public converter::import::ImportException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   filename    File that is being preprocessed
     * @param   message     Error message
     */
    XmlPreprocessorException(QString filename, QString message);
};

/**
 * @brief   Exception thrown when preprocessed file contains undefined external reference
 */
class UndefinedExternalReference : public XmlPreprocessorException
{
public:
    /**
     * @brief   Constructor
     *
     * @param   filename            File that is being preprocessed
     * @param   externalReference   Reference that wasn't defined
     */
    UndefinedExternalReference(QString filename, QString externalReference);
};

} // namespace seds::preprocessor
