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

#pragma once

#include "ivtopromelatranslatorcontext.h"
#include "systeminfo.h"

#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/definitions.h>
#include <conversion/common/translation/translator.h>
#include <ivcore/ivinterface.h>
#include <promela/PromelaModel/promelasystemmodel.h>
#include <shared/veobject.h>

namespace promela::translator {
/**
 * @brief Translate between InterfaceView model and Promela Model
 */
class IvToPromelaTranslator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given InterfaceView model into an Promela model
     *
     * @param   sourceModels    List of models
     * @param   options         List of options
     *
     * @return  Promela model translated from the InterfaceView model
     */
    auto translateModels(std::vector<conversion::Model *> sourceModels, const conversion::Options &options) const
            -> std::vector<std::unique_ptr<conversion::Model>> override;

    /**
     * @brief   Return a model type that is a source of this translator
     *
     * @return  Source model type
     */
    auto getSourceModelType() const -> conversion::ModelType override;
    /**
     * @brief   Returns a model type that is a target of this translator
     *
     * @return  Target model type
     */
    auto getTargetModelType() const -> conversion::ModelType override;
    /**
     * @brief   Provides a set of all source model types that are required for the translation
     *
     * @return  Set of required models
     */
    auto getDependencies() const -> std::set<conversion::ModelType> override;
    /**
     * @brief   Provides a set of all source model types that are optional for the translation
     *
     * @return  Set of optional models
     */
    auto getOptionalDependencies() const -> std::set<conversion::ModelType> override;

    /**
     * @brief Prepare System Information based on model and conversion options.
     *
     * @param model input IVModel
     * @param options conversion options
     * @return system information
     */
    std::unique_ptr<SystemInfo> prepareSystemInfo(const ivm::IVModel *model, const conversion::Options &options) const;

private:
    auto constructChannelName(const QString &functionName, const QString &interfaceName) const -> QString;

    auto getSubtypesDefinitions(const Asn1Acn::Asn1Model *asn1Model, const conversion::Options &options) const
            -> std::vector<const Asn1Acn::Definitions *>;
    auto getInterfaceName(const ivm::IVInterface *interface) const -> QString;
    auto getInterfaceFunctionName(const ivm::IVInterface *interface) const -> QString;
    auto getInterfaceProperty(const ivm::IVInterface *interface, const QString &name) const -> QVariant;
    auto getInterfaceParameter(const ivm::IVInterface *interface) const -> std::pair<QString, QString>;
    auto getInterfaceQueueSize(const ivm::IVInterface *interface) const -> size_t;
    auto getInterfacePriority(const ivm::IVInterface *interface) const -> size_t;

    auto findRequiredInterface(const ivm::IVModel *model, const QString &functionName,
            const QString &interfaceName) const -> const ivm::IVInterface *;
    auto findTimerSignal(const ivm::IVModel *model, const QString &functionName, const QString &signalName) const
            -> QString;

    auto observerChannelName(const ObserverAttachment &attachment, const QString &toFunction) const -> QString;
    auto getAttachmentToFunctions(const ivm::IVModel *model, const ObserverAttachment &attachment) const -> QStringList;

    auto getObserverAttachments(IvToPromelaTranslatorContext &context, const QString &function,
            const QString &interface, const ObserverAttachment::Kind kind) const -> ObserverAttachments;

    auto prepareFunctionInfo(IvToPromelaTranslatorContext &context, const ::ivm::IVFunction *ivFunction,
            const QString &functionName, FunctionInfo &functionInfo) const -> void;
    auto prepareEnvironmentFunctionInfo(IvToPromelaTranslatorContext &context, const ::ivm::IVFunction *ivFunction,
            const QString &functionName, FunctionInfo &functionInfo) const -> void;
    auto prepareProctypeInfo(IvToPromelaTranslatorContext &context, const ivm::IVInterface *providedInterface,
            const QString &functionName) const -> std::unique_ptr<ProctypeInfo>;
    auto prepareEnvProctypeInfo(IvToPromelaTranslatorContext &context, const ivm::IVInterface *requiredInterface,
            const QString &functionName) const -> std::unique_ptr<EnvProctypeInfo>;
    auto prepareSynchronousCallInfo(IvToPromelaTranslatorContext &context, const QString &functionName,
            const ivm::IVInterface *requiredInterface, FunctionInfo &functionInfo) const -> void;
    auto prepareSporadicCallInfo(IvToPromelaTranslatorContext &context, const QString &functionName,
            const ivm::IVInterface *requiredInterface, FunctionInfo &functionInfo) const -> void;
    auto prepareCallInfo(RequiredCallInfo &info, IvToPromelaTranslatorContext &context, const QString &functionName,
            const ivm::IVInterface *requiredInterface) const -> QString;
};
}
