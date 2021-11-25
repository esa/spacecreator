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

namespace Asn1Acn {
class Asn1Model;
} // namespace Asn1Acn

namespace seds::model {
class SedsModel;
} // namespace seds::model

namespace conversion::seds::translator {

/**
 * @brief   Translator between ASN.1/ACN model and SEDS model
 */
class Asn1ToSedsTranslator final : public conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given ASN.1/ACN models into an SEDS model
     *
     * @param   sources     List of models
     *
     * @return  Created SEDS model
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

private:
    auto translateAsn1Model(const Asn1Acn::Asn1Model *asn1Model, const Options &options) const
            -> std::vector<std::unique_ptr<Model>>;
};

} // namespace conversion::iv::translator
