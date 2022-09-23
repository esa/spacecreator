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

namespace ivm {
class IVModel;
class IVPropertyTemplateConfig;
} // namespace ivm

namespace seds::model {
class Package;
class SedsModel;
} // namespace seds::model

namespace conversion::iv::translator::seds {

/**
 * @brief   Translator between SEDS model and InterfaceView model
 */
class SedsToIvTranslator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given SEDS and ASN.1 models into an InterfaceView model
     *
     * @param   sources     List of models
     *
     * @return  Created InterfaceView model
     */
    virtual auto translateModels(std::vector<Model *> sourceModels, const Options &options) const
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
     * @brief   Gets SEDS package from given vector
     *
     * @param   packageName     Package name
     * @param   sedsPackages    SEDS packages
     *
     * @return  SEDS package
     */
    static auto getSedsPackage(const QString &packageName, const std::vector<::seds::model::Package> &sedsPackages)
            -> const ::seds::model::Package *;

private:
    /**
     * @brief   Translate SEDS model
     *
     * @param   sedsModel   SEDS model to translate
     * @param   ivConfig    IV model config
     * @param   options     List of options
     *
     * @return  Result IV model with auxiliary ASN.1 model
     */
    auto translateSedsModel(const ::seds::model::SedsModel *sedsModel, ivm::IVPropertyTemplateConfig *ivConfig,
            const Options &options) const -> std::vector<std::unique_ptr<Model>>;

private:
    /**
     * @brief   Translate SEDS package
     *
     * @param   sedsPackage         Package to translate
     * @param   ivModel             InterfaceView model to which should translated IV functions will be added
     * @param   sedsPackages        List of SEDS packages
     * @param   generateFunction    Generate parent function for the translated components
     * @param   options             Options
     */
    auto translatePackage(const ::seds::model::Package &sedsPackage, ivm::IVModel *ivModel,
            const std::vector<::seds::model::Package> &sedsPackages, bool generateFunction, const Options &options) const
            -> void;
};

} // namespace conversion::iv::translator::seds
