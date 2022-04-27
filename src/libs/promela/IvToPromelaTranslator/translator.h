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

#include <conversion/common/translation/translator.h>
#include <ivcore/ivinterface.h>
#include <promela/PromelaModel/promelamodel.h>
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

private:
    auto generateInitProctype(const std::vector<QString> &modelFunctions, const ::ivm::IVModel *ivModel) const
            -> ::promela::model::InitProctype;
    auto generateProctype(::promela::model::PromelaModel *promelaModel, const QString &functionName,
            const QString &interfaceName, const QString &parameterType, size_t queueSize, size_t priority,
            bool environment) const -> std::unique_ptr<::promela::model::Proctype>;
    auto generateEnvironmentProctype(const QString &functionName, const QString &interfaceName,
            const QString &parameterType, const QString &sendInline) const
            -> std::unique_ptr<::promela::model::Proctype>;
    auto generateSendInline(const QString &functionName, const QString &interfaceName, const QString &parameterName,
            const QString &parameterType, const QString &sourceFunctionName, const QString &sourceInterfaceName) const
            -> std::unique_ptr<::promela::model::InlineDef>;
    auto createPromelaObjectsForFunction(::promela::model::PromelaModel *promelaModel, const ::ivm::IVModel *ivModel,
            ::ivm::IVFunction *ivFunction, const QString &functionName) const -> void;
    auto createPromelaObjectsForEnvironment(::promela::model::PromelaModel *promelaModel, const ::ivm::IVModel *ivModel,
            ::ivm::IVFunction *ivFunction, const QString &functionName) const -> void;
    auto createCheckQueueInline(::promela::model::PromelaModel *promelaModel, const QString &functionName,
            QList<QString> &channelNames) const -> void;
    auto createSystemState(::promela::model::PromelaModel *promelaModel, const ::ivm::IVModel *ivModel,
            const std::vector<QString> &modelFunctions) const -> void;

    auto containsContextVariables(const QVector<shared::ContextParameter> &parameters) const -> bool;
    auto constructChannelName(const QString &functionName, const QString &interfaceName) const -> QString;

    auto getInterfaceName(const ivm::IVInterface *interface) const -> QString;
    auto getInterfaceFunctionName(const ivm::IVInterface *interface) const -> QString;
    auto getInterfaceProperty(ivm::IVInterface *interface, const QString &name) const -> QVariant;
    auto getInterfaceParameter(const ivm::IVInterface *interface) const -> std::pair<QString, QString>;
    auto getInterfaceQueueSize(ivm::IVInterface *interface) const -> size_t;
    auto getInterfacePriority(ivm::IVInterface *interface) const -> size_t;
};
}
