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

#include "sedscontainerdatatypebuilder.h"

using namespace seds::model;

namespace tests::conversion::common {

SedsContainerDataTypeBuilder::SedsContainerDataTypeBuilder(QString name)
{
    m_dataType.setName(std::move(name));
}

seds::model::ContainerDataType SedsContainerDataTypeBuilder::build()
{
    return std::move(m_dataType);
}

SedsContainerDataTypeBuilder &SedsContainerDataTypeBuilder::withEntry(QString name, QString typeName)
{
    Entry entry;
    entry.setName(std::move(name));
    entry.setType(std::move(typeName));

    m_dataType.addEntry(std::move(entry));

    return *this;
}

} // namespace tests::conversion::common
