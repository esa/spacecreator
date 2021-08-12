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

#include "import/options.h"

namespace converter::import {

void Options::add(Options::Key key)
{
    m_options.insert({ key, std::nullopt });
}

void Options::add(Options::Key key, QString value)
{
    m_options.insert({ key, std::move(value) });
}

bool Options::isSet(Options::Key key) const
{
    const auto found = m_options.find(key);
    return found != std::end(m_options);
}

std::optional<QString> Options::value(Options::Key key) const
{
    const auto found = m_options.find(key);
    if (found != std::end(m_options)) {
        return found->second;
    } else {
        return std::nullopt;
    }
}

std::vector<QString> Options::values(Options::Key key) const
{
    std::vector<QString> values;

    const auto range = m_options.equal_range(key);
    std::for_each(range.first, range.second, [&values](auto &&option) {
        if (option.second) {
            values.push_back(std::move(*option.second));
        }
    });

    return values;
}

} // namespace converter::import
