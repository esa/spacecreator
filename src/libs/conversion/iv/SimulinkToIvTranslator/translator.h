/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include <conversion/common/options.h>
#include <conversion/common/translation/translator.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <simulink/SimulinkModel/simulinkmodel.h>

namespace conversion::iv::translator::simulink {

/**
 * @brief   Translator between SIMULINK model to InterfaceView model
 */
class SimulinkToIvTranslator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given SIMULINK model into an InterfaceView model
     *
     * @param   sources     List of models
     *
     * @return  Created InterfaceView model
     */
    virtual auto translateModels(std::vector<Model *> sourceModels, const conversion::Options &options) const
            -> std::vector<std::unique_ptr<Model>> override;

    /**
     * @brief   Returns a model type that is a source of this translator
     *
     * @return  Source model type
     */
    virtual auto getSourceModelType() const -> ModelType override;
    /**
     * @brief   Returns a model type that is a target of this translator
     *
     * @return  Target model type
     */
    virtual auto getTargetModelType() const -> ModelType override;
    /**
     * @brief   Provides a set of all source model types that are required for the translation
     *
     * @return  Set of required models
     */
    virtual auto getDependencies() const -> std::set<ModelType> override;
    /**
     * @brief   Provides a set of all source model types that are optional for the translation
     *
     * @return  Set of optional models
     */
    virtual auto getOptionalDependencies() const -> std::set<ModelType> override;

private:
    auto translateSimulinkModel(const ::simulink::model::SimulinkModel &simulinkModel,
            ivm::IVPropertyTemplateConfig &ivConfig, const Options &options) const
            -> std::vector<std::unique_ptr<Model>>;

    static auto isIVFunctionImplementationSupported(const QString &ivFunctionImplementation) -> bool;

    static auto getIVConfigFilePathIfExists(const conversion::Options &options) -> QString;
    static auto createIVFunction(const conversion::Options &options) -> ivm::IVFunction *;
    static auto createIVInterface(const QString &interfaceName, ivm::IVFunction *ivFunction) -> ivm::IVInterface *;

private:
    static const QStringList m_supportedIVFunctionImplementations;
};

} // namespace conversion::iv::translator::simulink
