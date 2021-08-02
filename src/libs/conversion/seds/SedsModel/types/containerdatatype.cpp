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

#include "types/containerdatatype.h"

namespace seds::model {

ContainerDataType::ContainerDataType() noexcept
    : m_abstract(false)
{
}

const ContainerDataType::ConstraintSet&
ContainerDataType::constraints() const
{
    return m_constraints;
}

void
ContainerDataType::addConstraint(ContainerDataType::ContainerConstraintType constraint)
{
    m_constraints.push_back(std::move(constraint));
}

const ContainerDataType::EntryList&
ContainerDataType::entries() const
{
    return m_entries;
}

void
ContainerDataType::addEntry(ContainerDataType::EntryListType entry)
{
    m_entries.push_back(std::move(entry));
}

const ContainerDataType::EntryList&
ContainerDataType::trailerEntries() const
{
    return m_trailerEntries;
}

void
ContainerDataType::addTrailerEntry(ContainerDataType::EntryListType entry)
{
    m_trailerEntries.push_back(std::move(entry));
}

const std::optional<DataTypeRef>&
ContainerDataType::baseType() const
{
    return m_baseType;
}

void
ContainerDataType::setBaseType(DataTypeRef baseType)
{
    m_baseType = std::move(baseType);
}

bool
ContainerDataType::isAbstract() const
{
    return m_abstract;
}

void
ContainerDataType::setAbstract(bool abstract)
{
    m_abstract = abstract;
}

} // namespace seds::model
