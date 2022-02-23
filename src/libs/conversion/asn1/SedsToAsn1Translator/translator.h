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
#include <asn1library/asn1/importedtype.h>
#include <conversion/common/translation/translator.h>
#include <list>
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

    /**
     * @brief   Gets ASN.1 definitions from given ASN.1 model
     *
     * @param   definitionsName     Definitions name
     * @param   asn1Files           ASN.1 files
     *
     * @return  ASN.1 definitions
     */
    static auto getAsn1Definitions(const QString &definitionsName, const Asn1Acn::Asn1Model::Data &asn1Files)
            -> Asn1Acn::Definitions *;
    /**
     * @brief   Gets SEDS package from given vector
     *
     * @param   packageName     Package name
     * @param   sedsPackages    SEDS packages
     *
     * @return  SEDS package
     */
    static auto getSedsPackage(const QString &packageName, const std::vector<seds::model::Package> &sedsPackages)
            -> const seds::model::Package *;

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
     * @param   importedTypes   Set of types that this package imports
     * @param   asn1Files       List of already translated ASN.1 files
     * @param   sedsPackages    List of SEDS packages
     *
     * @return  Result ASN.1 files
     */
    auto translatePackage(const seds::model::Package &sedsPackage, const std::set<Asn1Acn::ImportedType> &importedTypes,
            const Asn1Acn::Asn1Model::Data &asn1Files, const std::vector<seds::model::Package> &sedsPackages) const
            -> std::vector<std::unique_ptr<Asn1Acn::File>>;
    /**
     * @brief   Translate SEDS data types
     *
     * @param   sedsDataTypes       Data types to translate
     * @param   asn1Definitions     Where translated data types should be added
     * @param   sedsPackage         Parent SEDS package
     * @param   asn1Files           List of already translated ASN.1 files
     * @param   sedsPackages        List of SEDS packages
     */
    auto translateDataTypes(const std::list<const seds::model::DataType *> &sedsDataTypes,
            Asn1Acn::Definitions *asn1Definitions, const seds::model::Package *sedsPackage,
            const Asn1Acn::Asn1Model::Data &asn1Files, const std::vector<seds::model::Package> &sedsPackages) const
            -> void;

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
