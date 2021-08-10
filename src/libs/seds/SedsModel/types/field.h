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

#include "base/namedentity.h"
#include "types/datatyperef.h"
#include "types/dimensionsize.h"
#include "types/encodings/booleandataencoding.h"
#include "types/encodings/floatdataencoding.h"
#include "types/encodings/integerdataencoding.h"
#include "types/encodings/stringdataencoding.h"
#include "types/ranges/derivedtyperange.h"

#include <optional>
#include <variant>
#include <vector>

namespace seds::model {

class Field : public NamedEntity
{
public:
    using ArrayDimensions = std::vector<DimensionSize>;
    using FieldEncoding = std::variant<std::monostate, BooleanDataEncoding, FloatDataEncoding, IntegerDataEncoding,
            StringDataEncoding>;

public:
    Field() = default;
    virtual ~Field() = 0;
    Field(Field &&) = default;
    Field &operator=(Field &&) = default;

public:
    auto type() const -> const DataTypeRef &;
    auto setType(DataTypeRef type) -> void;

    auto encoding() const -> const FieldEncoding &;
    auto setEncoding(FieldEncoding encoding) -> void;

    auto arrayDimensions() const -> const ArrayDimensions &;
    auto addArrayDimension(DimensionSize arrayDimension) -> void;

    auto validRange() const -> const DerivedTypeRange &;
    auto setValidRange(DerivedTypeRange validRange) -> void;

private:
    DataTypeRef m_type;
    FieldEncoding m_encoding;
    ArrayDimensions m_arrayDimensions;
    DerivedTypeRange m_validRange;
};

} // namespace seds::model
