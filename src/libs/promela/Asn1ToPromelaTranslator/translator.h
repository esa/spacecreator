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

#include <QStringList>
#include <conversion/common/translation/translator.h>
#include <ivcore/ivmodel.h>
#include <promela/PromelaModel/promeladatamodel.h>

namespace Asn1Acn {
class Asn1Model;
class Definitions;
class File;
}

namespace promela::translator {
/**
 * @brief Translate between ASN.1 model and PromelaDataModel
 */
class Asn1ToPromelaTranslator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given ASN.1 model into an Promela model
     *
     * @param   sourceModels    List of ASN.1 models
     * @param   options         List of options
     *
     * @return  Promela model translated from the ASN.1 models
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

private:
    auto translateAsn1Model(const Asn1Acn::Asn1Model *model, bool enhancedSpinSupport,
            bool ignoreSimulation = true) const -> std::vector<std::unique_ptr<conversion::Model>>;
    auto generateValueGenerationInlines(const Asn1Acn::Asn1Model *asn1Model, const ivm::IVModel *ivModel,
            const QStringList &typeNames, const conversion::Options &options, const std::optional<float> &delta) const
            -> std::vector<std::unique_ptr<conversion::Model>>;
    auto createDataviewInitInline(
            promela::model::PromelaDataModel &model, const QVector<QString> &initInlineNames) const -> void;
};
}
