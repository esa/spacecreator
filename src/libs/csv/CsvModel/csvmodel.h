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

#include "row.h"

#include <memory>
#include <vector>

namespace csv {

/**
 * @brief   Represents CSV data model
 *
 * Based on RFC 4180: https://www.ietf.org/rfc/rfc4180.txt
 */
class CsvModel
{
public:
    /**
     * @brief   Getter for data header (column labels)
     *
     * @return  Header
     */
    auto header() -> Row;

    /**
     * @brief   Getter for data records
     *
     * @return  Data records
     */
    auto records() -> const std::vector<std::unique_ptr<Row>> &;

    /**
     * @brief   Set the data header
     *
     * @param   header row of labels
     */
    auto setHeader(const Row &header) -> void;

    /**
     * @brief   Set the data header
     *
     * @param   header list of labels
     */
    auto setHeader(const QStringList &header) -> void;

    /**
     * @brief   Add a data record to this model
     *
     * @param   record record with data
     */
    auto addRecord(std::unique_ptr<Row> record) -> void;

private:
    Row m_header;
    std::vector<std::unique_ptr<Row>> m_records;
};

} // namespace csv
