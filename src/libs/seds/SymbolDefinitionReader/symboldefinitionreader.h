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

#include <seds/SedsCommon/basetypesmappings.h>
#include <unordered_map>

namespace seds::symbolreader {

/**
 * Reads symbol definitions (external references) that will be used during preprocessing.
 *
 * External references are stored in toml file.
 * External reference value has to be a string.
 *
 * Reader will throw an exception in case of an error
 */
class SymbolDefinitionReader final
{
public:
    /** @brief  Type of map with external references */
    using ExternalReferencesMap = std::unordered_map<QString, QString>;

public:
    /**
     * @brief   Reads symbols from given file
     *
     * @param   filename    File to read
     *
     * @throws  SymbolDefinitionReaderException
     * @throws  converter::import::FileNotFound
     *
     * @returns Map with read external references
     */
    static auto readSymbols(QString filename) -> ExternalReferencesMap;
};

} // namespace seds::symbolreader
