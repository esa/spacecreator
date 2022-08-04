/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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
#include <list>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>
#include <seds/SedsModel/types/datatype.h>
#include <vector>

namespace Asn1Acn {
class Definitions;
class File;
} // namespace Asn1Acn

namespace seds::model {
class SedsModel;
class Package;
class Component;
} // namespace seds::model

namespace conversion::asn1::translator {

/**
 * @brief   Translator between SEDS model and ASN.1 model
 */
class SedsToAsn1Translator final : public ::conversion::translator::Translator
{
public:
    /**
     * @brief   Translate given SEDS models into an ASN.1 model
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
    auto translateSedsModel(const seds::model::SedsModel *sedsModel, const Options &options) const
            -> std::vector<std::unique_ptr<Model>>;

    auto translatePackage(const seds::model::Package *sedsPackage, Asn1Acn::Asn1Model *asn1Model,
            const std::list<const seds::model::Package *> &sedsPackages, const Options &options) const -> void;

    auto translateComponent(const seds::model::Component &sedsComponent, const seds::model::Package *sedsPackage,
            Asn1Acn::Definitions *packageAsn1Definitions, Asn1Acn::Asn1Model *asn1Model,
            const std::list<const seds::model::Package *> &sedsPackages, const Options &options) const -> void;

    auto translateDataTypeSet(const std::list<const seds::model::DataType *> &sedsDataTypes, Context &context) const
            -> void;
    auto translateInterfaceDeclarations(const std::vector<seds::model::InterfaceDeclaration> &interfaceDeclarations,
            Context &context) const -> void;
    auto translateInterfaceImplementations(
            const std::vector<seds::model::Interface> &providedInterfaces, Context &context) const -> void;
    auto translateGenericTypes(const seds::model::Interface &interface,
            const seds::model::InterfaceDeclaration *interfaceDeclaration, const seds::model::Component &component,
            Context &context, Context &interfaceContext) const -> void;

private:
    auto collectDataTypes(const seds::model::Package *package) const -> std::vector<const seds::model::DataType *>;
    auto collectDataTypes(const seds::model::Component &component) const -> std::vector<const seds::model::DataType *>;
};

} // namespace conversion::asn1::translator
