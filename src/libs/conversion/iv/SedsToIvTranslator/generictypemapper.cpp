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

#include "generictypemapper.h"

namespace conversion::iv::translator {

using seds::model::GenericTypeMap;
using seds::model::GenericTypeMapSet;

GenericTypeMapper::GenericTypeMapper(
        QString sedsInterfaceName, const std::optional<GenericTypeMapSet> &genericTypeMapSet)
    : m_sedsInterfaceName(std::move(sedsInterfaceName))
{
    if (!genericTypeMapSet.has_value()) {
        return;
    }

    const auto &typeMaps = genericTypeMapSet->genericTypeMaps();
    for (const auto &typeMap : typeMaps) {
        addSimpleMapping(typeMap);
    }
}

const TypeMapping *GenericTypeMapper::getMapping(const QString &typeName) const
{
    const auto foundMapping = std::find_if(m_mappings.begin(), m_mappings.end(),
            [&](const auto &mapping) { return mapping.genericTypeName == typeName; });

    if (foundMapping == m_mappings.end()) {
        return nullptr;
    } else {
        return &(*foundMapping);
    }
}

void GenericTypeMapper::addSimpleMapping(const GenericTypeMap &typeMap)
{
    TypeMapping::ConcreteType concreteType { typeMap.type().nameStr(), typeMap.fixedValue() };

    TypeMapping typeMapping;
    typeMapping.genericTypeName = typeMap.nameStr();
    typeMapping.concreteTypes.push_back(std::move(concreteType));

    m_mappings.push_back(std::move(typeMapping));
}

} // namespace conversion::iv::translator
