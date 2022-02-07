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

#include "typemapping.h"

#include <QString>
#include <optional>
#include <seds/SedsModel/generics/generictypemap.h>
#include <seds/SedsModel/generics/generictypemapset.h>
#include <vector>

namespace conversion::iv::translator {

class GenericTypeMapper final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sedsInterfaceName   Parent interface name
     * @param   genericTypeMapSet   Generic type map set
     */
    GenericTypeMapper(
            QString sedsInterfaceName, const std::optional<seds::model::GenericTypeMapSet> &genericTypeMapSet);
    /**
     * @brief   Deleted copy constructor
     */
    GenericTypeMapper(const GenericTypeMapper &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    GenericTypeMapper(GenericTypeMapper &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    GenericTypeMapper &operator=(const GenericTypeMapper &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    GenericTypeMapper &operator=(GenericTypeMapper &&) = delete;

public:
    /**
     * @brief   Checks if given type name is mapped to any concrete type
     *
     * @param   typeName    Generic type name
     *
     * @return  Mapping information if type has mapping to at least one type, nullptr otherwise
     */
    auto getMapping(const QString &typeName) const -> const TypeMapping *;

private:
    auto addSimpleMapping(const seds::model::GenericTypeMap &typeMap) -> void;

private:
    QString m_sedsInterfaceName;

    std::vector<TypeMapping> m_mappings;
};

} // namespace conversion::iv::translator {
