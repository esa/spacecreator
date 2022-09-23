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

#include <QFileInfo>
#include <asn1library/asn1/asn1model.h>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
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
using conversion::asn1::translator::seds::SedsToAsn1Translator;
using conversion::iv::IvOptions;
using conversion::translator::TranslationException;
using ivm::IVModel;
using seds::model::SedsModel;

namespace conversion::iv::translator::seds {

std::vector<std::unique_ptr<Model>> SedsToIvTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    const auto *sedsModel = getModel<SedsModel>(sourceModels);
    const auto *asn1Model = getModel<Asn1Model>(sourceModels);
    Q_UNUSED(asn1Model);

    const auto ivConfigFilepath = options.value(IvOptions::configFilepath);
    if (!ivConfigFilepath) {
        throw TranslationException("InterfaceView configuration file wasn't specified");
    }

    QFileInfo ivConfigFile(*ivConfigFilepath);
    if (!ivConfigFile.exists()) {
        auto errorMsg = QString("InterfaceView configuration file '%1' doesn't exist").arg(*ivConfigFilepath);
        throw TranslationException(std::move(errorMsg));
    }

    ivm::IVPropertyTemplateConfig *ivConfig = ivm::IVPropertyTemplateConfig::instance();
    ivConfig->init(*ivConfigFilepath);

    return translateSedsModel(sedsModel, ivConfig, options);
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

std::vector<std::unique_ptr<Model>> SedsToIvTranslator::translateSedsModel(
        const SedsModel *sedsModel, ivm::IVPropertyTemplateConfig *ivConfig, const Options &options) const
{
    const auto generateFunctionsForPackages = options.isSet(IvOptions::generateFunctionsForPackages);

    auto ivModel = std::make_unique<IVModel>(ivConfig);

    const auto &sedsModelData = sedsModel->data();
    if (std::holds_alternative<::seds::model::PackageFile>(sedsModelData)) {
        const auto &sedsPackage = std::get<::seds::model::PackageFile>(sedsModelData).package();
        translatePackage(sedsPackage, ivModel.get(), {}, generateFunctionsForPackages);
    } else if (std::holds_alternative<::seds::model::DataSheet>(sedsModelData)) {
        const auto &sedsPackages = std::get<::seds::model::DataSheet>(sedsModelData).packages();
        for (const auto &sedsPackage : sedsPackages) {
            translatePackage(sedsPackage, ivModel.get(), sedsPackages, generateFunctionsForPackages);
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    std::vector<std::unique_ptr<Model>> resultModels;
    resultModels.push_back(std::move(ivModel));

    return resultModels;
}

void SedsToIvTranslator::translatePackage(const ::seds::model::Package &sedsPackage, IVModel *ivModel,
        const std::vector<::seds::model::Package> &sedsPackages, bool generateFunction) const
{
    ComponentsTranslator componentsTranslator(&sedsPackage, sedsPackages);
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

const ::seds::model::Package *SedsToIvTranslator::getSedsPackage(
        const QString &packageName, const std::vector<::seds::model::Package> &sedsPackages)
{
    const auto sedsPackage = std::find_if(sedsPackages.begin(), sedsPackages.end(),
            [&](const auto &package) { return package.nameStr() == packageName; });
    if (sedsPackage == sedsPackages.end()) {
        auto message = QString("Unable to find package \"%1\"").arg(packageName);
        throw TranslationException(std::move(message));
    }

    return &(*sedsPackage);
}

} // namespace conversion::iv::translator::seds
