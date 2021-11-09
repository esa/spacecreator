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

#include "specialized/statemachinetranslator.h"

#include <conversion/common/translation/exceptions.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <seds/SedsModel/sedsmodel.h>

using Asn1Acn::Asn1Model;
using conversion::translator::TranslationException;
using ivm::IVModel;
using sdl::SdlModel;
using seds::model::SedsModel;

namespace conversion::sdl::translator {

std::vector<std::unique_ptr<Model>> SedsToSdlTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    Q_UNUSED(options);

    checkSourceModelCount(sourceModels);

    const auto *sedsModel = getModel<SedsModel>(sourceModels);
    auto *asn1Model = getModel<Asn1Model>(sourceModels);
    auto *ivModel = getModel<IVModel>(sourceModels);

    return translateSedsModel(sedsModel, asn1Model, ivModel);
}

ModelType SedsToSdlTranslator::getSourceModelType() const
{
    return ModelType::Seds;
}

ModelType SedsToSdlTranslator::getTargetModelType() const
{
    return ModelType::Sdl;
}

std::set<ModelType> SedsToSdlTranslator::getDependencies() const
{
    static std::set<ModelType> dependencies { ModelType::Seds, ModelType::Asn1, ModelType::InterfaceView };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> SedsToSdlTranslator::translateSedsModel(
        const SedsModel *sedsModel, Asn1Acn::Asn1Model *asn1Model, ivm::IVModel *ivModel) const
{
    auto sdlModel = std::make_unique<SdlModel>();

    const auto &sedsModelData = sedsModel->data();
    if (std::holds_alternative<seds::model::PackageFile>(sedsModelData)) {
        const auto &sedsPackage = std::get<seds::model::PackageFile>(sedsModelData).package();
        translatePackage(sedsPackage, asn1Model, ivModel, sdlModel.get());
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &sedsPackages = std::get<seds::model::DataSheet>(sedsModelData).packages();
        for (const auto &sedsPackage : sedsPackages) {
            translatePackage(sedsPackage, asn1Model, ivModel, sdlModel.get());
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(sdlModel));

    return result;
}

auto SedsToSdlTranslator::translatePackage(const seds::model::Package &sedsPackage, Asn1Acn::Asn1Model *asn1Model,
        ivm::IVModel *ivModel, ::sdl::SdlModel *model) const -> void
{
    for (const auto &component : sedsPackage.components()) {
        translateComponent(component, asn1Model, ivModel, model);
    }
}

auto SedsToSdlTranslator::translateComponent(const seds::model::Component &sedsComponent, Asn1Acn::Asn1Model *asn1Model,
        ivm::IVModel *ivModel, ::sdl::SdlModel *model) const -> void
{
    Q_UNUSED(asn1Model);
    Q_UNUSED(ivModel);

    const auto &implementation = sedsComponent.implementation();
    if (implementation.stateMachines().size() > 1) {
        throw new TranslationException("Only a single state machine is supported per SEDS component");
    }

    if (implementation.parameterMaps().size() + implementation.parameterActivityMaps().size()
                    + implementation.stateMachines().size()
            > 0) {
        // There is at least one active element in the implementation
        auto stateMachine = std::make_unique<::sdl::StateMachine>();
        ::sdl::Process process;
        process.setName(sedsComponent.nameStr()); // TODO -> mangle identifier
        // TODO translate variable declarations
        // TODO translate procedures (activities?)
        // TODO provide additional translation for parameter (activity) maps
        if (implementation.stateMachines().size() == 1) {
            StateMachineTranslator::translateStateMachine(
                    implementation.stateMachines()[0], &process, stateMachine.get());
        }
        // State machine needs to be moved after processing, because later it cannot be accessed for modification
        process.setStateMachine(std::move(stateMachine));
        model->addProcess(std::move(process));
    }
}

} // namespace conversion::sdl::translator
