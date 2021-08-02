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

#include "device/metadata/metadatacategory.h"

namespace seds::model {

const MetadataValueSet &MetadataCategory::values() const
{
    return m_values;
}

void MetadataCategory::addValue(MetadataValueType value)
{
    m_values.push_back(std::move(value));
}

const MetadataCategory::Categories &MetadataCategory::subcategories() const
{
    return m_subcategories;
}

void MetadataCategory::addSubcategory(MetadataCategory subcategory)
{
    m_subcategories.push_back(std::move(subcategory));
}

} // namespace seds::model
