/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include <conversion/common/translation/exceptions.h>

using conversion::translator::TranslationException;
using seds::model::GenericAlternate;
using seds::model::GenericTypeMap;
using seds::model::GenericTypeMapSet;

namespace conversion::asn1::translator {

GenericTypeMapper::GenericTypeMapper(Context &context, QString interfaceName)
    : m_context(context)
    , m_interfaceName(std::move(interfaceName))
{
}

void GenericTypeMapper::addMappings(const seds::model::GenericTypeMapSet &typeMapSet)
{
    const auto &typeMaps = typeMapSet.genericTypeMaps();
    for (const auto &typeMap : typeMaps) {
        addSimpleMapping(typeMap);
    }

    if (typeMapSet.alternateSet()) {
        const auto &alternates = typeMapSet.alternateSet()->alternates();

        m_determinantName = findDeterminant(alternates);

        for (const auto &alternate : alternates) {
            addAlternateMapping(alternate);
        }
    }
}

const TypeMapping *GenericTypeMapper::getMapping(const QString &genericTypeName)
{
    if (m_mappings.count(genericTypeName) != 0) {
        return &(m_mappings.at(genericTypeName));
    } else {
        return nullptr;
    }
}

const std::optional<QString> &GenericTypeMapper::determinantName() const
{
    return m_determinantName;
}

void GenericTypeMapper::addSimpleMapping(const GenericTypeMap &typeMap)
{
    const auto &genericTypeName = typeMap.nameStr();
    const auto concreteType = m_context.findAsn1Type(typeMap.type());

    const TypeMapping::Concrete concreteMapping { concreteType, typeMap.fixedValue(), std::nullopt };

    TypeMapping mapping;
    mapping.concreteMappings.push_back(std::move(concreteMapping));

    m_mappings.insert({ genericTypeName, std::move(mapping) });
}

void GenericTypeMapper::addAlternateMapping(const GenericAlternate &alternate)
{
    const auto &typeMaps = alternate.genericTypeMaps();

    const auto foundDeterminant = std::find_if(typeMaps.begin(), typeMaps.end(),
            [&](const auto &typeMap) { return typeMap.nameStr() == *m_determinantName; });

    if (foundDeterminant == typeMaps.end()) {
        auto errorMessage = QString("Unable to find determinant \"%1\" for interface \"%2\"")
                                    .arg(*m_determinantName)
                                    .arg(m_interfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &determinantValue = foundDeterminant->fixedValue().value();

    for (const auto &typeMap : typeMaps) {
        const auto &genericTypeName = typeMap.nameStr();
        const auto &concreteType = m_context.findAsn1Type(typeMap.type());

        const TypeMapping::Concrete concreteMapping { concreteType, typeMap.fixedValue(), determinantValue };

        if (m_mappings.count(genericTypeName) == 0) {
            TypeMapping mapping;
            mapping.determinantTypeName = foundDeterminant->type().nameStr();
            mapping.concreteMappings.push_back(std::move(concreteMapping));

            m_mappings.insert({ genericTypeName, std::move(mapping) });
        } else {
            auto &mapping = m_mappings.at(genericTypeName);
            mapping.concreteMappings.push_back(std::move(concreteMapping));
        }
    }
}

QString GenericTypeMapper::findDeterminant(const std::vector<GenericAlternate> &alternates)
{
    const auto determinants = std::accumulate(std::next(alternates.begin()), alternates.end(),
            getPossibleDeterminants(alternates.front()), [&](const auto &acc, const auto &alternate) {
                std::vector<std::pair<QString, QString>> result;

                const auto possibleDeterminants = getPossibleDeterminants(alternate);

                std::set_intersection(acc.begin(), acc.end(), possibleDeterminants.begin(), possibleDeterminants.end(),
                        std::back_inserter(result), [&](const auto lhs, const auto rhs) { return lhs < rhs; });

                return result;
            });

    if (determinants.empty()) {
        auto errorMessage =
                QString("No alternate determinant could be found in the interface \"%1\"").arg(m_interfaceName);
        throw TranslationException(std::move(errorMessage));
    } else if (determinants.size() > 1) {
        auto errorMessage = QString("More than one possible alternate determinant was found in the interface \"%1\"")
                                    .arg(m_interfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    return determinants.front().first;
}

std::vector<std::pair<QString, QString>> GenericTypeMapper::getPossibleDeterminants(const GenericAlternate &alternate)
{
    std::vector<std::pair<QString, QString>> possibleDeterminants;

    for (const auto &typeMap : alternate.genericTypeMaps()) {
        if (typeMap.fixedValue().has_value()) {
            possibleDeterminants.push_back({ typeMap.nameStr(), typeMap.type().value().pathStr() });
        }
    }

    std::sort(possibleDeterminants.begin(), possibleDeterminants.end(),
            [&](const auto lhs, const auto rhs) { return lhs < rhs; });

    return possibleDeterminants;
}

} // namespace conversion::asn1::translator
