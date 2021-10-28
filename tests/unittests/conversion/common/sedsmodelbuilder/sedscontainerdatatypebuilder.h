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

#include <seds/SedsModel/types/containerdatatype.h>

namespace tests::conversion::common {

class SedsContainerDataTypeBuilder final
{
public:
    SedsContainerDataTypeBuilder(QString name);
    auto build() -> seds::model::ContainerDataType;

public:
    auto withEntry(QString name, QString typeName) -> SedsContainerDataTypeBuilder &;
    auto withErrorControlEntry(QString name, QString typeName, seds::model::CoreErrorControl errorControl)
            -> SedsContainerDataTypeBuilder &;
    auto withFixedValueEntry(QString name, QString typeName, QString value) -> SedsContainerDataTypeBuilder &;

private:
    seds::model::ContainerDataType m_dataType;
};

} // namespace tests::conversion::common {
