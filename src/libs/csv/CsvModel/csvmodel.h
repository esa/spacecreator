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

#include <conversion/common/model.h>
#include <memory>
#include <vector>

namespace csv {

/**
 * @brief   Represents CSV data model
 *
 */
class CsvModel : public conversion::Model
{
public:
    /**
     * @brief   Getter for this model type
     *
     * @return  Model type
     */
    virtual auto modelType() const -> conversion::ModelType override;

    auto header() -> Row;

    auto records() -> const std::vector<std::unique_ptr<Row>> &;

    auto separator() -> QString;

    auto setHeader(const Row &header) -> void;

    auto setHeader(const QStringList &header) -> void;

    auto addRecord(std::unique_ptr<Row> record) -> void;

    auto setSeparator(const QString &separator) -> void;

private:
    Row m_header;
    std::vector<std::unique_ptr<Row>> m_records;
    QString m_separator = ",";
};

} // namespace csv
