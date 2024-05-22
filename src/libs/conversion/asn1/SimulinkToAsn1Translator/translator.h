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

#include "context.h"

#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/importedtype.h>
#include <conversion/common/translation/translator.h>
#include <memory>
#include <set>
#include <simulink/SimulinkModel/datatypes/datatype.h>
#include <simulink/SimulinkModel/simulinkmodel.h>
#include <vector>

namespace conversion::asn1::translator::simulink {

/**
 * @brief   Translation between SIMULINK model and ASN.1 model
 */
class SimulinkToAsn1Translator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given SIMULINK models into an ASN.1 model
     *
     * @param   sourceModels    List of SIMULINK models
     * @param   options         List of options
     *
     * @return  ASN.1 model translated from the SIMULINK models
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
    auto translateSimulinkModel(const ::simulink::model::SimulinkModel &simulinkModel, const Options &options) const
            -> std::vector<std::unique_ptr<Model>>;

    auto addMatLabStandardDataTypes(Context &context) const -> void;
    auto translateDataTypesOfModel(Context &context, const ::simulink::model::SimulinkModel &simulinkModel) const
            -> void;
    auto translateDataTypesOfPorts(Context &context, const ::simulink::model::SimulinkModel &simulinkModel) const
            -> void;
};

} // namespace conversion::asn1::translator::simulink
