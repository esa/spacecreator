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

#include "specialized/generictypemapper.h"

namespace conversion::iv::translator {

GenericTypeMapper::GenericTypeMapper(const std::optional<seds::model::GenericTypeMapSet> &genericTypeMapSet)
    : m_genericTypeMapSet(genericTypeMapSet)
{
}

const QString &GenericTypeMapper::findMappedType(const QString &genericTypeName) const
{
    if (!m_genericTypeMapSet) {
        return genericTypeName;
    }

    const auto &genericTypeMaps = m_genericTypeMapSet->genericTypeMaps();

    if (genericTypeMaps.empty()) {
        return genericTypeName;
    }

    const auto foundMapping = std::find_if(genericTypeMaps.begin(), genericTypeMaps.end(),
            [&genericTypeName](
                    const seds::model::GenericTypeMap &typeMap) { return typeMap.nameStr() == genericTypeName; });

    if (foundMapping != genericTypeMaps.end()) {
        return foundMapping->type().nameStr();
    } else {
        return genericTypeName;
    }
}

} // namespace conversion::iv::translator
