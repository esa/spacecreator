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

#include "specialized/componentstranslator.h"

#include <asn1library/asn1/asn1model.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <seds/SedsModel/sedsmodel.h>

using Asn1Acn::Asn1Model;
using conversion::Escaper;
using conversion::iv::IvOptions;
using conversion::translator::TranslationException;
using ivm::IVModel;
using seds::model::SedsModel;

namespace conversion::iv::translator {

std::vector<std::unique_ptr<Model>> SedsToIvTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    const auto *sedsModel = getModel<SedsModel>(sourceModels);
    auto *asn1Model = getModel<Asn1Model>(sourceModels);

    const auto ivConfigFilepath = options.value(IvOptions::configFilepath);
    if (!ivConfigFilepath) {
        throw TranslationException("InterfaceView configuration file wasn't specified");
    }

    ivm::IVPropertyTemplateConfig *ivConfig = ivm::IVPropertyTemplateConfig::instance();
    ivConfig->init(*ivConfigFilepath);

    return translateSedsModel(sedsModel, asn1Model, ivConfig, options);
}

ModelType SedsToIvTranslator::getSourceModelType() const
{
    return ModelType::Seds;
}

ModelType SedsToIvTranslator::getTargetModelType() const
{
    return ModelType::InterfaceView;
}

std::set<ModelType> SedsToIvTranslator::getDependencies() const
{
    static std::set<ModelType> dependencies { ModelType::Seds, ModelType::Asn1 };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> SedsToIvTranslator::translateSedsModel(const SedsModel *sedsModel,
        Asn1Model *asn1Model, ivm::IVPropertyTemplateConfig *ivConfig, const Options &options) const
{
    const auto generateFunctionsForPackages = options.isSet(IvOptions::generateFunctionsForPackages);

    auto ivModel = std::make_unique<IVModel>(ivConfig);

    const auto &sedsModelData = sedsModel->data();
    if (std::holds_alternative<seds::model::PackageFile>(sedsModelData)) {
        const auto &sedsPackage = std::get<seds::model::PackageFile>(sedsModelData).package();
        translatePackage(sedsPackage, asn1Model, ivModel.get(), generateFunctionsForPackages);
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &sedsPackages = std::get<seds::model::DataSheet>(sedsModelData).packages();
        for (const auto &sedsPackage : sedsPackages) {
            translatePackage(sedsPackage, asn1Model, ivModel.get(), generateFunctionsForPackages);
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    std::vector<std::unique_ptr<Model>> resultModels;
    resultModels.push_back(std::move(ivModel));

    return resultModels;
}

void SedsToIvTranslator::translatePackage(
        const seds::model::Package &sedsPackage, Asn1Model *asn1Model, IVModel *ivModel, bool generateFunction) const
{
    auto asn1Definitions = getAsn1Definitions(sedsPackage, asn1Model);

    ComponentsTranslator componentsTranslator(sedsPackage, asn1Definitions);
    auto ivFunctions = componentsTranslator.translateComponents();

    if (generateFunction) {
        const auto parentIvFunctionName = Escaper::escapeIvName(sedsPackage.nameStr());
        auto *parentIvFunction = new ivm::IVFunction();
        parentIvFunction->setEntityAttribute(
                ivm::meta::Props::token(ivm::meta::Props::Token::name), parentIvFunctionName);

        for (auto *ivFunction : ivFunctions) {
            parentIvFunction->addChild(ivFunction);
        }

        ivModel->addObject(parentIvFunction);
    } else {
        ivModel->addObjects(ivFunctions);
    }
}

Asn1Acn::Definitions *SedsToIvTranslator::getAsn1Definitions(
        const seds::model::Package &sedsPackage, Asn1Model *asn1Model) const
{
    const auto asn1FileName = Escaper::escapeAsn1PackageName(sedsPackage.nameStr());
    auto &asn1Files = asn1Model->data();
    auto asn1File = std::find_if(
            std::begin(asn1Files), std::end(asn1Files), [&](const auto &file) { return file->name() == asn1FileName; });
    if (asn1File == asn1Files.end()) {
        auto message = QString("Unable to find file %1 in the ASN.1 model").arg(asn1FileName);
        throw TranslationException(std::move(message));
    }

    const auto asn1DefinitionsName = Escaper::escapeAsn1PackageName(sedsPackage.nameStr());
    auto *asn1Definitions = (*asn1File)->definitions(asn1DefinitionsName);
    if (!asn1Definitions) {
        auto message =
                QString("ASN.1 file %1 doesn't have definitions named %2").arg(asn1FileName).arg(asn1DefinitionsName);
        throw TranslationException(std::move(message));
    }

    return asn1Definitions;
}

} // namespace conversion::iv::translator
