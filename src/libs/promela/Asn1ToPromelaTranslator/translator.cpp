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

#include "promelatypesorter.h"
#include "visitors/asn1nodevaluegeneratorvisitor.h"
#include "visitors/asn1nodevisitor.h"

#include <algorithm>
#include <asn1library/asn1/asn1model.h>
#include <promela/PromelaModel/promelamodel.h>
#include <promela/PromelaOptions/options.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::File;
using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::promela::PromelaOptions;

using promela::model::PromelaModel;

namespace promela::translator {
std::vector<std::unique_ptr<Model>> Asn1ToPromelaTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    const bool enhancedSpinSupport = options.isSet(PromelaOptions::enhancedSpinSupport);
    const bool asn1ValueGeneration = options.isSet(PromelaOptions::asn1ValueGeneration);
    const std::vector<QString> valueGeneration = options.values(PromelaOptions::asn1ValueGenerationForType);

    const auto *asn1Model = getModel<Asn1Model>(sourceModels);

    if (asn1ValueGeneration) {
        QStringList typeNames;
        std::copy(valueGeneration.begin(), valueGeneration.end(), std::back_inserter(typeNames));
        return generateValueGenerationInlines(asn1Model, typeNames);
    } else {
        return translateAsn1Model(asn1Model, enhancedSpinSupport);
    }
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

std::vector<std::unique_ptr<Model>> Asn1ToPromelaTranslator::translateAsn1Model(
        const Asn1Model *model, bool enhancedSpinSupport) const
{
    std::unique_ptr<PromelaModel> promelaModel = std::make_unique<PromelaModel>();
    for (const std::unique_ptr<File> &file : model->data()) {
        visitAsn1File(file.get(), *promelaModel, enhancedSpinSupport);
    }

    PromelaTypeSorter typeSorter;
    typeSorter.sortTypeDefinitions(*promelaModel);

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(promelaModel));
    return result;
}

std::vector<std::unique_ptr<conversion::Model>> Asn1ToPromelaTranslator::generateValueGenerationInlines(
        const Asn1Acn::Asn1Model *model, const QStringList &typeNames) const
{
    std::unique_ptr<PromelaModel> promelaModel = std::make_unique<PromelaModel>();
    for (const std::unique_ptr<File> &file : model->data()) {
        visitAsn1FileGenerate(file.get(), *promelaModel, typeNames);
    }
    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(promelaModel));
    return result;
}

void Asn1ToPromelaTranslator::visitAsn1File(File *file, PromelaModel &promelaModel, bool enhancedSpinSupport) const
{
    Asn1NodeVisitor visitor(promelaModel, enhancedSpinSupport);
    visitor.visit(*file);
}

void Asn1ToPromelaTranslator::visitAsn1FileGenerate(
        File *file, PromelaModel &promelaModel, const QStringList &typeNames) const
{
    Asn1NodeValueGeneratorVisitor visitor(promelaModel, typeNames);
    visitor.visit(*file);
}
}
