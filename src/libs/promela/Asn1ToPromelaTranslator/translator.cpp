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

#include "translator.h"

#include "visitors/asn1nodevisitor.h"

#include <algorithm>
#include <asn1library/asn1/asn1model.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/translation/exceptions.h>
#include <promela/PromelaModel/promelamodel.h>
#include <promela/PromelaOptions/options.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::File;
using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::promela::PromelaOptions;
using conversion::translator::TranslationException;
using promela::model::Declaration;
using promela::model::PromelaModel;
using promela::model::TypeAlias;
using promela::model::Utype;
using promela::model::UtypeRef;

namespace promela::translator {
std::vector<std::unique_ptr<Model>> Asn1ToPromelaTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    Q_UNUSED(options);

    checkSourceModelCount(sourceModels);

    bool enhancedSpinSupport = options.isSet(PromelaOptions::enhancedSpinSupport);

    const auto *asn1Model = getModel<Asn1Model>(sourceModels);

    return translateAsn1Model(asn1Model, enhancedSpinSupport);
}

ModelType Asn1ToPromelaTranslator::getSourceModelType() const
{
    return ModelType::Asn1;
}

ModelType Asn1ToPromelaTranslator::getTargetModelType() const
{
    return ModelType::Promela;
}

std::set<ModelType> Asn1ToPromelaTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::Asn1 };
}

void Asn1ToPromelaTranslator::sortTypeDefinitions(::promela::model::PromelaModel &promelaModel) const
{
    // this algorithm sorts the Utypes
    // that the dependencies of given utype are defined before.

    // this loop constructs a type alias map
    std::map<QString, QString> typeAliasMap;

    for (const TypeAlias &alias : promelaModel.getTypeAliases()) {
        if (std::holds_alternative<UtypeRef>(alias.getType())) {
            typeAliasMap.emplace(alias.getName(), std::get<UtypeRef>(alias.getType()).getName());
        }
    }

    // map of type dependencies
    std::map<QString, std::set<QString>> dependencies;
    // map of existing utpyes
    std::map<QString, Utype> types;

    // populate maps
    for (const Utype &utype : promelaModel.getUtypes()) {
        dependencies.emplace(utype.getName(), std::set<QString>());
        types.emplace(utype.getName(), utype);
    }

    // populate dependencies
    // this is required, because dependencies might be other objects
    for (const Utype &utype : promelaModel.getUtypes()) {
        for (const Declaration &declaration : utype.getFields()) {
            if (declaration.getType().isUtypeReference()) {
                // if field is another utype: add it to the dependencies set
                const QString &name = declaration.getType().getUtypeReference().getName();
                if (dependencies.find(name) != dependencies.end()) {
                    dependencies[utype.getName()].insert(name);
                } else {
                    // if field is a type alias which refers to utype, add it to dependencies set
                    auto iter = typeAliasMap.find(name);
                    if (iter != typeAliasMap.end() && dependencies.find(iter->second) != dependencies.end()) {
                        dependencies[utype.getName()].insert(iter->second);
                    }
                }
            }
        }
    }

    QList<Utype> sortedTypes;
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
                types.erase(iter->first);
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

std::vector<std::unique_ptr<Model>> Asn1ToPromelaTranslator::translateAsn1Model(
        const Asn1Model *model, bool enhancedSpinSupport) const
{
    std::unique_ptr<PromelaModel> promelaModel = std::make_unique<PromelaModel>();
    for (const std::unique_ptr<File> &file : model->data()) {
        visitAsn1File(file.get(), *promelaModel, enhancedSpinSupport);
    }

    sortTypeDefinitions(*promelaModel);

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(promelaModel));
    return result;
}

void Asn1ToPromelaTranslator::visitAsn1File(File *file, PromelaModel &promelaModel, bool enhancedSpinSupport) const
{
    Asn1NodeVisitor visitor(promelaModel, enhancedSpinSupport);
    visitor.visit(*file);
}
}
