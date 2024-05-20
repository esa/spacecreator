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

#include <asn1library/asn1/file.h>
#include <conversion/common/translation/translator.h>
#include <ivcore/ivmodel.h>
#include <msccore/mscmodel.h>
#include <sdl/SdlModel/sdlmodel.h>

namespace conversion::sdl::translator {

/**
 * @brief   Translator between MSC model and SDL model
 */
class MscToSdlTranslator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given MSC models into an SDL model
     *
     * @param   sourceModels    List of models
     * @param   options         List of options
     *
     * @return  SDL model translated from the MSC models
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
    /**
     * @brief   Provides a set of all source model types that are optional for the translation
     *
     * @return  Set of optional models
     */
    virtual auto getOptionalDependencies() const -> std::set<ModelType> override;

private:
    /**
     * @brief   Translate MSC model
     *
     * @param   mscModel    MSC model to translate
     * @param   asn1File    ASN.1 file
     * @param   options     List of options
     *
     * @return  Result SDL model
     */
    auto translateMscModel(const ::msc::MscModel *mscModel, const Asn1Acn::File *asn1File, const ivm::IVModel *ivModel,
            const Options &options) const -> std::vector<std::unique_ptr<Model>>;
};

} // namespace conversion::sdl::translator
