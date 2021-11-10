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

#include <asn1library/asn1/asn1model.h>
#include <conversion/common/translation/translator.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <seds/SedsModel/sedsmodel.h>

namespace conversion::sdl::translator {

/**
 * @brief   Translator between SEDS model and SDL model
 */
class SedsToSdlTranslator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given SEDS models into an SDL model
     *
     * @param   sourceModels    List of SEDS models
     * @param   options         List of options
     *
     * @return  ASN.1 model translated from the SEDS models
     */
    virtual auto translateModels(std::vector<Model *> sourceModels, const Options &options) const
            -> std::vector<std::unique_ptr<Model>> override;

    /**
     * @brief   Return a model type that is a source of this translator
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

private:
    /**
     * @brief   Translate SEDS model
     *
     * @param   sedsModel   SEDS model to translate
     *
     * @return  Result SDL model
     */
    auto translateSedsModel(const seds::model::SedsModel *sedsModel, Asn1Acn::Asn1Model *asn1Model,
            ivm::IVModel *ivModel) const -> std::vector<std::unique_ptr<Model>>;

    /**
     * @brief   Translate SEDS package
     *
     * @param   package             Package to translate
     * @param   asn1Model           Relevant ASN.1/ACN model
     * @param   ivModel             Relevant Interface View model
     * @param   model               Target SDL model to add the translated component implementations
     */
    auto translatePackage(const seds::model::Package &sedsPackage, Asn1Acn::Asn1Model *asn1Model, ivm::IVModel *ivModel,
            ::sdl::SdlModel *model) const -> void;

    /**
     * @brief   Translate SEDS component
     *
     * @param   component           Component to translate
     * @param   asn1Model           Relevant ASN.1/ACN model
     * @param   ivModel             Relevant Interface View model
     * @param   model               Target SDL model to add the translated component implementation
     */
    auto translateComponent(const seds::model::Component &sedsComponent, Asn1Acn::Asn1Model *asn1Model,
            ivm::IVModel *ivModel, ::sdl::SdlModel *model) const -> void;
};

} // namespace conversion::sdl::translator
