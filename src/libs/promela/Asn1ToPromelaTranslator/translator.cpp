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

#include <QDebug>
#include <algorithm>
#include <asn1library/asn1/asn1model.h>
#include <promela/PromelaOptions/options.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::File;
using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::promela::PromelaOptions;
using ivm::IVModel;

using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::PromelaDataModel;
using promela::model::Skip;

namespace promela::translator {
std::vector<std::unique_ptr<Model>> Asn1ToPromelaTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    const bool enhancedSpinSupport = options.isSet(PromelaOptions::enhancedSpinSupport);
    const bool ignoreSimulation = true;
    const bool asn1ValueGeneration = options.isSet(PromelaOptions::asn1ValueGeneration);
    const std::vector<QString> valueGeneration = options.values(PromelaOptions::asn1ValueGenerationForType);

    const auto *asn1Model = getModel<Asn1Model>(sourceModels);
    const auto *ivModel = getModel<IVModel>(sourceModels);

    if (asn1ValueGeneration) {
        QStringList typeNames;
        std::copy(valueGeneration.begin(), valueGeneration.end(), std::back_inserter(typeNames));

        const QString deltaStr = options.value(PromelaOptions::realGeneratorDelta).value_or("");
        const std::optional<float> delta =
                deltaStr.isEmpty() ? std::nullopt : std::optional(std::abs(deltaStr.toFloat()));

        return generateValueGenerationInlines(asn1Model, ivModel, typeNames, options, delta);
    } else {
        return translateAsn1Model(asn1Model, enhancedSpinSupport, ignoreSimulation);
    }
}

ModelType Asn1ToPromelaTranslator::getSourceModelType() const
{
    return ModelType::Asn1;
}

ModelType Asn1ToPromelaTranslator::getTargetModelType() const
{
    return ModelType::PromelaData;
}

std::set<ModelType> Asn1ToPromelaTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::Asn1, ModelType::InterfaceView };
}

std::vector<std::unique_ptr<Model>> Asn1ToPromelaTranslator::translateAsn1Model(
        const Asn1Model *model, bool enhancedSpinSupport, bool ignoreSimulation) const
{
    QVector<QString> initInlineNames;
    auto promelaModel = std::make_unique<PromelaDataModel>();
    for (const std::unique_ptr<File> &file : model->data()) {
        Asn1NodeVisitor visitor(*promelaModel, enhancedSpinSupport, ignoreSimulation);
        visitor.visit(*file);
        initInlineNames.append(visitor.getInitInlineNames());
    }

    PromelaTypeSorter typeSorter;
    typeSorter.sortTypeDefinitions(*promelaModel);

    createDataviewInitInline(*promelaModel, initInlineNames);

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(promelaModel));
    return result;
}

std::vector<std::unique_ptr<conversion::Model>> Asn1ToPromelaTranslator::generateValueGenerationInlines(
        const Asn1Acn::Asn1Model *asn1Model, const ivm::IVModel *ivModel, const QStringList &typeNames,
        const Options &options, const std::optional<float> &delta) const
{
    const auto subtypesFilepaths = options.values(PromelaOptions::subtypesFilepath);

    auto promelaModel = std::make_unique<PromelaDataModel>();
    for (const std::unique_ptr<File> &file : asn1Model->data()) {
        const auto subtypesFilepathFound = std::find_if(subtypesFilepaths.begin(), subtypesFilepaths.end(),
                [&](const auto &filepath) { return filepath == file->name(); });

        if (subtypesFilepathFound == subtypesFilepaths.end()) {
            Asn1NodeValueGeneratorVisitor visitor(*promelaModel, typeNames, delta);
            visitor.visit(*file);
        } else {
            Asn1NodeValueGeneratorVisitor visitor(*promelaModel, asn1Model, ivModel, delta);
            visitor.visit(*file);
        }
    }
    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(promelaModel));
    return result;
}

void Asn1ToPromelaTranslator::createDataviewInitInline(
        promela::model::PromelaDataModel &model, const QVector<QString> &initInlineNames) const
{
    promela::model::Sequence initSequence(promela::model::Sequence::Type::D_STEP);

    if (initInlineNames.isEmpty()) {
        initSequence.appendElement(Skip());
    } else {
        for (const QString &inlineName : initInlineNames) {
            initSequence.appendElement(InlineCall(inlineName, {}));
        }
    }

    std::unique_ptr<InlineDef> dataviewInitInline =
            std::make_unique<InlineDef>("global_dataview_init", QList<QString>(), std::move(initSequence));

    model.addInlineDef(std::move(dataviewInitInline));
}
}
