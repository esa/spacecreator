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

#include "types/compositedatatype.h"
#include "types/constraints/containerrangeconstraint.h"
#include "types/constraints/containertypeconstraint.h"
#include "types/constraints/containervalueconstraint.h"
#include "types/datatyperef.h"
#include "types/entries/entry.h"
#include "types/entries/errorcontrolentry.h"
#include "types/entries/fixedvalueentry.h"
#include "types/entries/lengthentry.h"
#include "types/entries/listentry.h"
#include "types/entries/paddingentry.h"

#include <optional>
#include <variant>

namespace seds::model {

class ContainerDataType final : public CompositeDataType
{
public:
    using EntryListType = std::variant<Entry, ErrorControlEntry, FixedValueEntry, LengthEntry, ListEntry, PaddingEntry>;
    using EntryList = std::vector<EntryListType>;
    using ContainerConstraintType =
            std::variant<ContainerRangeConstraint, ContainerTypeConstraint, ContainerValueConstraint>;
    using ConstraintSet = std::vector<ContainerConstraintType>;

public:
    ContainerDataType() noexcept;
    ContainerDataType(ContainerDataType &&) = default;
    ContainerDataType &operator=(ContainerDataType &&) = default;

public:
    auto constraints() const -> const ConstraintSet &;
    auto addConstraint(ContainerConstraintType constraint) -> void;

    auto entries() const -> const EntryList &;
    auto addEntry(EntryListType entry) -> void;

    auto trailerEntries() const -> const EntryList &;
    auto addTrailerEntry(EntryListType entry) -> void;

    auto baseType() const -> const std::optional<DataTypeRef> &;
    auto setBaseType(DataTypeRef baseType) -> void;

    auto isAbstract() const -> bool;
    auto setAbstract(bool abstract) -> void;

private:
    ConstraintSet m_constraints;
    EntryList m_entries;
    EntryList m_trailerEntries;

    std::optional<DataTypeRef> m_baseType;
    bool m_abstract;
};

} // namespace seds::model
