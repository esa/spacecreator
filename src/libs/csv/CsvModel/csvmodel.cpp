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

#include "csvmodel.h"

#include <QStringList>
#include <memory>

namespace csv {

Row CsvModel::header() const
{
    return m_header;
}

const std::vector<std::unique_ptr<Row>> &CsvModel::records() const
{
    return m_records;
}

auto CsvModel::field(const unsigned int record, const unsigned int field) const -> QString
{
    if (record >= m_records.size() || m_records[record] == nullptr) {
        throw "Accessing non-existing record";
    }

    if (field >= m_records[record]->fields().size()) {
        throw "Accessing non-existing field";
    }

    return records()[record]->fields()[field];
}

void CsvModel::setHeader(const Row &header)
{
    m_header = header;
}

void CsvModel::setHeader(const QStringList &header)
{
    std::for_each(header.begin(), header.end(),
            [&](const auto &field) { //
                m_header.addField(field);
            });
}

void CsvModel::addRecord(std::unique_ptr<Row> record)
{
    m_records.push_back(std::move(record));
}

} // csv
