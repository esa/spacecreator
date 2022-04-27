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

#include <algorithm>
#include <conversion/common/translation/exceptions.h>
#include <numeric>

using conversion::translator::TranslationException;

namespace conversion::iv::translator {

using seds::model::GenericAlternate;
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

    if (genericTypeMapSet->alternateSet().has_value()) {
        const auto &alternates = genericTypeMapSet->alternateSet()->alternates();

        const auto determinantName = findDeterminant(alternates);

        for (const auto &alternate : alternates) {
            addAlternateMapping(alternate, determinantName);
        }
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
    const auto &genericTypeName = typeMap.nameStr();
    const auto &concreteTypeRef = typeMap.type();

    TypeMapping::ConcreteType concreteType { concreteTypeRef, typeMap.fixedValue(), std::nullopt };

    TypeMapping typeMapping;
    typeMapping.genericTypeName = genericTypeName;
    typeMapping.concreteTypes.push_back(std::move(concreteType));

    m_mappings.push_back(std::move(typeMapping));
}

void GenericTypeMapper::addAlternateMapping(const GenericAlternate &alternate, const QString &determinantName)
{
    const auto &typeMaps = alternate.genericTypeMaps();

    const auto foundDeterminant = std::find_if(typeMaps.begin(), typeMaps.end(),
            [&](const auto &typeMap) { return typeMap.nameStr() == determinantName; });

    if (foundDeterminant == typeMaps.end()) {
        auto errorMessage = QString("Unable to find determinant \"%1\" for interface \"%2\"")
                                    .arg(determinantName)
                                    .arg(m_sedsInterfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &determinantTypeRef = foundDeterminant->type();
    const auto &determinantValue = foundDeterminant->fixedValue();

    for (const auto &typeMap : typeMaps) {
        const auto &genericTypeName = typeMap.nameStr();
        const auto &concreteTypeRef = typeMap.type();

        const auto foundMapping = std::find_if(m_mappings.begin(), m_mappings.end(),
                [&](const auto &mapping) { return mapping.genericTypeName == genericTypeName; });

        TypeMapping::ConcreteType concreteType { concreteTypeRef, typeMap.fixedValue(), determinantValue };

        if (foundMapping == m_mappings.end()) {
            TypeMapping typeMapping;
            typeMapping.genericTypeName = genericTypeName;
            typeMapping.determinantTypeRef = determinantTypeRef;
            typeMapping.concreteTypes.push_back(std::move(concreteType));

            m_mappings.push_back(std::move(typeMapping));
        } else {
            foundMapping->concreteTypes.push_back(std::move(concreteType));
        }
    }
}

QString GenericTypeMapper::findDeterminant(const std::vector<GenericAlternate> &alternates)
{
    const auto determinants = std::accumulate(std::next(alternates.begin()), alternates.end(),
            getPossibleDeterminants(alternates.front()), [&](const auto &acc, const auto &alternate) {
                std::vector<const GenericTypeMap *> result;

                const auto possibleDeterminants = getPossibleDeterminants(alternate);

                std::set_intersection(acc.begin(), acc.end(), possibleDeterminants.begin(), possibleDeterminants.end(),
                        std::back_inserter(result), [&](const auto lhs, const auto rhs) {
                            return lhs->nameStr() != rhs->nameStr() || lhs->type().nameStr() != rhs->type().nameStr();
                        });

                return result;
            });

    if (determinants.empty()) {
        auto errorMessage =
                QString("No alternate determinant could be found in the interface \"%1\"").arg(m_sedsInterfaceName);
        throw TranslationException(std::move(errorMessage));
    } else if (determinants.size() > 1) {
        auto errorMessage = QString("More than one possible alternate determinant was found in the interface \"%1\"")
                                    .arg(m_sedsInterfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    return determinants.front()->nameStr();
}

std::vector<const GenericTypeMap *> GenericTypeMapper::getPossibleDeterminants(const GenericAlternate &alternate)
{
    std::vector<const GenericTypeMap *> possibleDeterminants;

    for (const auto &typeMap : alternate.genericTypeMaps()) {
        if (typeMap.fixedValue().has_value()) {
            possibleDeterminants.push_back(&typeMap);
        }
    }

    std::sort(possibleDeterminants.begin(), possibleDeterminants.end(),
            [&](const auto lhs, const auto rhs) { return lhs < rhs; });

    return possibleDeterminants;
}

} // namespace conversion::iv::translator
