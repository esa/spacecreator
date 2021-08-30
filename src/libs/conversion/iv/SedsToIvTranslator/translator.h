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
class SedsModel;
class Package;
} // namespace seds::model

namespace conversion::iv::translator {

class SedsToIvTranslator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given SEDS models into an ASN.1 model
     *
     * @param   sources     List of SEDS models
     *
     * @return  ASN.1 model translated from SEDS models
     */
    virtual auto translateModels(std::vector<const Model *> sourceModels, const Options &options) const
            -> std::unique_ptr<Model> override;
    /**
     * @brief   Provides a set of all source model types that are required for the translation
     *
     * @return  Set of required models
     */
    virtual auto getDependencies() const -> std::set<ModelType> override;

private:
    auto translateSedsModel(const seds::model::SedsModel *sedsModel, ivm::IVPropertyTemplateConfig *config) const
            -> std::unique_ptr<ivm::IVModel>;
};

} // namespace conversion::iv::translator
