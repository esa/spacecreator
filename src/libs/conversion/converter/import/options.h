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
#include <optional>
#include <unordered_map>
#include <vector>

namespace converter::import {

/**
 * @brief  Stores a map of importer options
 */
class Options final
{
public:
    /**
     * @brief   Possible options for importers
     */
    enum class Key
    {
        InputFile, ///< File to import
        PreprocessedFile, ///< Intermediate file that will be created during preprocessing
        SchemaFile, ///< Schema file that will be used during validation
        ExternalRef, ///< Declaration of an external reference
        ExternalRefFile, ///< File with external references declarations
        SkipSedsValidation, ///< Skip SEDS validation
        KeepIntermediateFiles, ///< Don't remove intermediate files after import
    };

public:
    /**
     * @brief   Adds new option without value
     *
     * @param   key     Option key
     */
    auto add(Options::Key key) -> void;
    /**
     * @brief   Adds new option with a value
     *
     * @param   key     Option key
     * @param   value   Option value
     */
    auto add(Options::Key key, QString value) -> void;

    /**
     * @brief   Checks whether given option is set
     *
     * @param   key     Option key
     *
     * @returns True of option was set, false otherwise
     */
    auto isSet(Options::Key key) const -> bool;
    /**
     * @brief   Returns value for first found key
     *
     * Use this if you expect that only one of such an option should be present
     *
     * @param   key     Option key
     *
     * @returns Optional option value
     */
    auto value(Options::Key key) const -> std::optional<QString>;
    /**
     * @brief   Returns vector of values for given key
     *
     * Use this if given option can have multiple values
     *
     * @param   key     Option key
     *
     * @returns Vector of option values
     */
    auto values(Options::Key key) const -> std::vector<QString>;

private:
    /** @brief  Options map type */
    std::unordered_multimap<Options::Key, std::optional<QString>> m_options;
};

} // namespace converter::import
