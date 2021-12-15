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
#include <seds/SedsModel/types/datatype.h>
#include <vector>

namespace Asn1Acn {
class Asn1Model;
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

    /**
     * @brief   Gets ASN.1 definitions for given SEDS package from given ASN.1 model
     *
     * @param   sedsPackage     SEDS package
     * @param   asn1Model       ASN.1 model
     *
     * @return  Asn1 definitions
     */
    static auto getAsn1Definitions(const seds::model::Package &sedsPackage, Asn1Acn::Asn1Model *asn1Model)
            -> Asn1Acn::Definitions *;

private:
    /**
     * @brief   Translate SEDS model
     *
     * @param   sedsModel   SEDS model to translate
     *
     * @return  Result ASN.1 model
     */
    auto translateSedsModel(const seds::model::SedsModel *sedsModel) const -> std::vector<std::unique_ptr<Model>>;

    /**
     * @brief   Translate SEDS package
     *
     * @param   sedsPackage     Package to translate
     *
     * @return  Result ASN.1 files
     */
    auto translatePackage(const seds::model::Package &sedsPackage) const -> std::vector<std::unique_ptr<Asn1Acn::File>>;
    /**
     * @brief   Translate SEDS data types
     *
     * @param   sedsDataTypes       Data types to translate
     * @param   asn1Definitions     Where translated data types should be added
     */
    auto translateDataTypes(const std::vector<const seds::model::DataType *> &sedsDataTypes,
            Asn1Acn::Definitions *asn1Definitions) const -> void;

    /**
     * @brief   Collects all data types declared directly in given package
     *
     * Gets data types from the package without those declared in the components
     *
     * @param   sedsPackage     SEDS package
     *
     * @return  Vector with all data types declared in the given package
     */
    std::vector<const seds::model::DataType *> collectDataTypes(const seds::model::Package &sedsPackage) const;

    /**
     * @brief   Collects all data types declared in given component
     *
     * Gets data types from the component
     *
     * @param   sedsComponent     SEDS component
     *
     * @return  Vector with all data types declared in the given component
     */
    std::vector<const seds::model::DataType *> collectDataTypes(const seds::model::Component &sedsComponent) const;
};

} // namespace conversion::asn1::translator
