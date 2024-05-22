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

#include "specialized/activitytranslator.h"
#include "specialized/common.h"
#include "specialized/splinecalibratortranslator.h"
#include "specialized/statemachinetranslator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivfunction.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <seds/SedsModel/sedsmodel.h>
#include <seds/SedsOptions/options.h>
#include <shared/parameter.h>

using Asn1Acn::Asn1Model;
using conversion::Escaper;
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

    return translateSedsModel(sedsModel, asn1Model, ivModel, options);
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
        const SedsModel *sedsModel, Asn1Acn::Asn1Model *asn1Model, ivm::IVModel *ivModel, const Options &options) const
{
    auto sdlModel = std::make_unique<SdlModel>();

    const auto &sedsModelData = sedsModel->data();
    if (std::holds_alternative<::seds::model::PackageFile>(sedsModelData)) {
        const auto &sedsPackage = std::get<::seds::model::PackageFile>(sedsModelData).package();
        translatePackage(sedsPackage, {}, asn1Model, ivModel, sdlModel.get(), options);
    } else if (std::holds_alternative<::seds::model::DataSheet>(sedsModelData)) {
        const auto &sedsPackages = std::get<::seds::model::DataSheet>(sedsModelData).packages();
        for (const auto &sedsPackage : sedsPackages) {
            translatePackage(sedsPackage, sedsPackages, asn1Model, ivModel, sdlModel.get(), options);
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(sdlModel));

    return result;
}

auto SedsToSdlTranslator::translatePackage(const ::seds::model::Package &sedsPackage,
        const std::vector<::seds::model::Package> &sedsPackages, Asn1Acn::Asn1Model *asn1Model, ivm::IVModel *ivModel,
        ::sdl::SdlModel *model, const Options &options) const -> void
{
    for (const auto &component : sedsPackage.components()) {
        translateComponent(sedsPackage, sedsPackages, component, asn1Model, ivModel, model, options);
    }
}

auto SedsToSdlTranslator::translateComponent(const ::seds::model::Package &sedsPackage,
        const std::vector<::seds::model::Package> &sedsPackages, const ::seds::model::Component &sedsComponent,
        Asn1Acn::Asn1Model *asn1Model, ivm::IVModel *ivModel, ::sdl::SdlModel *model, const Options &options) const
        -> void
{

    const auto &implementation = sedsComponent.implementation();
    const auto parameterMapCount = implementation.parameterMaps().size();
    const auto parameterActivityMapCount = implementation.parameterActivityMaps().size();
    const auto stateMachineCount = implementation.stateMachines().size();
    const auto activityCount = implementation.activities().size();
    const auto variableCount = implementation.variables().size();

    if (stateMachineCount > 1) {
        throw new TranslationException("Only a single state machine is supported per SEDS component");
    }

    if (parameterMapCount + parameterActivityMapCount + stateMachineCount + activityCount + variableCount > 0) {
        // There is at least one element in the implementation
        auto stateMachine = std::make_unique<::sdl::StateMachine>();
        ::sdl::Process process;
        process.setName(Escaper::escapeIvName(sedsComponent.nameStr()));
        const auto ivFunction = ivModel->getFunction(process.name(), Qt::CaseInsensitive);

        Context context(sedsPackage, sedsPackages, sedsComponent, asn1Model, ivFunction, &process, stateMachine.get());

        StateMachineTranslator::buildCommandMap(context);
        StateMachineTranslator::translateVariables(context, implementation.variables(), options);

        if (!options.isSet(conversion::seds::SedsOptions::tasteTranslation)) {
            StateMachineTranslator::createIoVariables(context);
        }

        StateMachineTranslator::createExternalProcedures(context);
        for (const auto &activity : implementation.activities()) {
            ActivityTranslator::translateActivity(context, activity, &process, options);
        }
        // TODO provide additional translation for parameter (activity) maps
        if (stateMachineCount == 1) {
            const auto &sedsStateMachine = implementation.stateMachines()[0];
            StateMachineTranslator::createTimerVariables(context, sedsStateMachine);
            StateMachineTranslator::translateStateMachine(context, sedsStateMachine, options);
            StateMachineTranslator::translateParameterMaps(
                    context, implementation.parameterMaps(), sedsStateMachine, options);
            StateMachineTranslator::translateParameterActivityMaps(
                    context, implementation.parameterActivityMaps(), sedsStateMachine, options);
        }
        StateMachineTranslator::ensureMinimalStateMachineExists(context);

        // Register all timers in the interface view
        for (const auto &timerName : process.timerNames()) {
            shared::ContextParameter timer(timerName, shared::BasicParameter::Type::Timer);
            ivFunction->addContextParam(timer);
        }
        if (context.maxSplinePointCount() > 0) {
            SplineCalibratorTranslator::buildSplineCalibratorBoilerplate(context);
        }

        // State machine needs to be moved after processing, because later it cannot be accessed for modification
        process.setStateMachine(std::move(stateMachine));
        model->addProcess(std::move(process));
    }
}

} // namespace conversion::sdl::translator
