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

#include "promelatypesorter.h"

#include <conversion/common/translation/exceptions.h>

using conversion::translator::TranslationException;
using promela::model::Declaration;
using promela::model::PromelaModel;
using promela::model::TypeAlias;
using promela::model::Utype;
using promela::model::UtypeRef;

namespace promela::translator {
auto PromelaTypeSorter::sortTypeDefinitions(PromelaModel &promelaModel) const -> void
{
    // this algorithm sorts the Utypes
    // that the dependencies of given utype are defined before.

    // this loop constructs a type alias map
    const std::map<QString, QString> typeAliasMap = getTypeAliasMap(promelaModel);

    // map of type dependencies
    std::map<QString, std::set<QString>> dependencies;
    // map of existing utypes, which will be inserted to sorted list
    std::map<QString, Utype> types;

    // populate maps
    for (const Utype &utype : promelaModel.getUtypes()) {
        dependencies.emplace(utype.getName(), std::set<QString>());
        types.emplace(utype.getName(), utype);
    }

    populateTypeDependencies(promelaModel, typeAliasMap, dependencies);

    // list for sorted types
    QList<Utype> sortedTypes;

    // auxilary set to check types existing on sortedTypes
    std::set<QString> insertedTypes;

    // Add types to sorted types when dependencies of the type
    // are subset of sorted types.
    while (!dependencies.empty()) {
        bool changed = false;

        auto iter = dependencies.begin();
        while (iter != dependencies.end()) {
            if (std::includes(insertedTypes.begin(), insertedTypes.end(), iter->second.begin(), iter->second.end())) {
                sortedTypes.append(types.at(iter->first));
                insertedTypes.insert(iter->first);
                iter = dependencies.erase(iter);
                changed = true;
            } else {
                ++iter;
            }
        }
        if (!changed) {
            throw conversion::ConversionException("Cyclic type dependencies in model.");
        }
    }

    promelaModel.setUtypes(sortedTypes);
}

std::map<QString, QString> PromelaTypeSorter::getTypeAliasMap(const PromelaModel &promelaModel) const
{
    // this loop constructs a type alias map
    std::map<QString, QString> typeAliasMap;

    for (const TypeAlias &alias : promelaModel.getTypeAliases()) {
        if (std::holds_alternative<UtypeRef>(alias.getType())) {
            typeAliasMap.emplace(alias.getName(), std::get<UtypeRef>(alias.getType()).getName());
        }
    }

    return typeAliasMap;
}

void PromelaTypeSorter::populateTypeDependencies(const PromelaModel &promelaModel,
        const std::map<QString, QString> &typeAliasMap, std::map<QString, std::set<QString>> &dependencies) const
{
    // populate dependencies
    // this is required, because dependencies might be other objects
    for (const Utype &utype : promelaModel.getUtypes()) {
        for (const Declaration &declaration : utype.getFields()) {
            QString name;
            if (declaration.getType().isUtypeReference()) {
                // if field is another utype: add it to the dependencies set
                name = declaration.getType().getUtypeReference().getName();
            } else if (declaration.getType().isArrayType()) {
                if (!std::holds_alternative<UtypeRef>(declaration.getType().getArrayType().getType())) {
                    continue;
                }
                name = std::get<UtypeRef>(declaration.getType().getArrayType().getType()).getName();
            }
            if (dependencies.find(name) != dependencies.end()) {
                dependencies[utype.getName()].insert(name);
            } else if (typeAliasMap.count(name)) {
                // if field is a type alias which refers to utype, add it to dependencies set
                const QString &targetType = typeAliasMap.at(name);
                if (dependencies.find(targetType) != dependencies.end()) {
                    dependencies[utype.getName()].insert(targetType);
                }
            }
        }
    }
}
}
