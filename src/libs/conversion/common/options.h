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

#include "qstringhash.h"

#include <QString>
#include <optional>
#include <unordered_map>
#include <vector>

namespace conversion {

/**
 * @brief  Stores a map of importer options
 *
 * QString is used as a key, but it's recommended to use predeclared strings
 * that can be found in respective namespaces.
 */
class Options final
{
public:
    using Key = QString;

public:
    /**
     * @brief   Adds new option without value
     *
     * @param   key     Option key
     */
    auto add(Key key) -> void;
    /**
     * @brief   Adds new option with a value
     *
     * @param   key     Option key
     * @param   value   Option value
     */
    auto add(Key key, QString value) -> void;

    /**
     * @brief   Checks whether given option is set
     *
     * @param   key     Option key
     *
     * @returns True of option was set, false otherwise
     */
    auto isSet(const Key &key) const -> bool;
    /**
     * @brief   Returns value for first found key
     *
     * Use this if you expect that only one of such an option should be present
     *
     * @param   key     Option key
     *
     * @returns Optional option value
     */
    auto value(const Key &key) const -> std::optional<QString>;
    /**
     * @brief   Returns vector of values for given key
     *
     * Use this if given option can have multiple values
     *
     * @param   key     Option key
     *
     * @returns Vector of option values
     */
    auto values(const Key &key) const -> std::vector<QString>;

private:
    /** @brief  Options map type */
    std::unordered_multimap<Key, std::optional<QString>> m_options;
};

} // namespace conversion
