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

#pragma once

#include <QString>
#include <vector>

namespace csv {

typedef QString Field;

/**
 * @brief   Represents CSV row
 *
 * Could store either data or header (labels)
 */
class Row
{
public:
    /**
     * @brief   Construct a new Row object
     *
     */
    Row();

    /**
     * @brief   Construct a new Row object
     *
     * @param   fields fields for this Row
     */
    Row(const QStringList &fields);

    /**
     * @brief   Getter for the fields
     *
     * @return  Fields
     */
    auto fields() const -> std::vector<Field>;

    /**
     * @brief   Add a given field
     *
     * @param   field a field to be added
     */
    auto addField(const Field &field) -> void;

private:
    std::vector<Field> m_fields;
};

} // namespace csv
