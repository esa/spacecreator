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

#include <asn1library/asn1/file.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <seds/SedsModel/sedsmodel.h>

using conversion::iv::IvOptions;
using conversion::translator::IncorrectSourceModelException;
using conversion::translator::TranslationException;
using ivm::IVModel;
using seds::model::SedsModel;

namespace conversion::iv::translator {

std::vector<std::unique_ptr<Model>> SedsToIvTranslator::translateModels(
        std::vector<const Model *> sourceModels, const Options &options) const
{
    if (sourceModels.empty()) {
        throw TranslationException("No models passed for translation");
    } else if (sourceModels.size() > 1) {
        throw TranslationException("Only one model is allowed for translation");
    }

    const auto *sourceModel = sourceModels[0];
    if (sourceModel == nullptr) {
        throw TranslationException("Source model is null");
    }

    if (sourceModel->modelType() != ModelType::Seds) {
        throw IncorrectSourceModelException(ModelType::Seds, sourceModel->modelType());
    }

    const auto *sedsModel = dynamic_cast<const SedsModel *>(sourceModel);

    const auto configFilename = options.value(IvOptions::configFilename);
    if (!configFilename) {
        throw TranslationException("InterfaceView configuration file wasn't specified");
    }

    ivm::IVPropertyTemplateConfig *config = ivm::IVPropertyTemplateConfig::instance();
    config->init(*configFilename);

    return translateSedsModel(sedsModel, config, options);
}

std::set<ModelType> SedsToIvTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::Seds };
}

std::vector<std::unique_ptr<Model>> SedsToIvTranslator::translateSedsModel(
        const SedsModel *sedsModel, ivm::IVPropertyTemplateConfig *config, const Options &options) const
{
    const auto generateFunctionsForPackages = options.isSet(IvOptions::generateFunctionsForPackages);

    auto ivModel = std::make_unique<IVModel>(config);
    Asn1Acn::File asn1File("interfaceview.asn");

    const auto &sedsModelData = sedsModel->data();
    if (std::holds_alternative<seds::model::PackageFile>(sedsModelData)) {
        const auto &package = std::get<seds::model::PackageFile>(sedsModelData).package();
        translatePackage(package, ivModel.get(), asn1File, generateFunctionsForPackages);
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &packages = std::get<seds::model::DataSheet>(sedsModelData).packages();
        for (const auto &package : packages) {
            translatePackage(package, ivModel.get(), asn1File, generateFunctionsForPackages);
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(ivModel));

    return result;
}

void SedsToIvTranslator::translatePackage(
        const seds::model::Package &package, IVModel *model, Asn1Acn::File &asn1File, bool generateFunction) const
{
    ComponentsTranslator componentsTranslator(package, asn1File);
    auto components = componentsTranslator.translateComponents();

    if (generateFunction) {
        auto *ivFunction = new ivm::IVFunction();
        ivFunction->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name), package.nameStr());

        for (auto *component : components) {
            ivFunction->addChild(component);
        }

        model->addObject(ivFunction);
    } else {
        model->addObjects(components);
    }
}

} // namespace conversion::iv::translator
