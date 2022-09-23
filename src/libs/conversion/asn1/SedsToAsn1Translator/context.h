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

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/types/type.h>
#include <conversion/common/options.h>
#include <cstdint>
#include <list>
#include <memory>
#include <optional>
#include <seds/SedsModel/base/description.h>
#include <seds/SedsModel/components/component.h>
#include <seds/SedsModel/types/datatype.h>
#include <seds/SedsModel/types/datatyperef.h>
#include <vector>

namespace seds::model {
class Package;
} // namespace seds::model

namespace conversion::asn1::translator::seds {

/**
 * @brief   Context with current SEDS to ASN.1 translation state
 */
class Context final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   sedsPackage         Current SEDS package
     * @param   definitions         Current ASN.1 definitions
     * @param   parentDefinitions   Parent ASN.1 definitions (if any)
     * @param   component           Parent SEDS component (if any)
     * @param   sedsPackages        List of all SEDS packages
     * @param   asn1Files           List of all already translated ASN.1 files
     * @param   options             Translation options
     */
    Context(const ::seds::model::Package *sedsPackage, Asn1Acn::Definitions *definitions,
            Asn1Acn::Definitions *parentDefinitions, const ::seds::model::Component *component,
            const std::list<const ::seds::model::Package *> &sedsPackages,
            const std::vector<std::unique_ptr<Asn1Acn::File>> &asn1Files, const Options &options);
    /**
     * @brief   Deleted copy constructor
     */
    Context(const Context &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    Context(Context &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    Context &operator=(const Context &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    Context &operator=(Context &&) = delete;

public:
    /**
     * @brief   Add a type to current ASN.1 definitions
     *
     * @param   asn1Type            Type to add
     * @param   sedsDescription     Description that should be applied to this type
     */
    auto addAsn1Type(std::unique_ptr<Asn1Acn::Types::Type> asn1Type, const ::seds::model::Description *sedsDescription)
            -> void;

    /**
     * @brief   Find SEDS data type by SEDS type reference
     *
     * @param   typeRef     SEDS type ref
     *
     * @return  Pointer to the found type, nullptr otherwise
     */
    auto findSedsType(const ::seds::model::DataTypeRef &typeRef) -> const ::seds::model::DataType *;
    /**
     * @brief   Find ASN.1 data type by SEDS type reference
     *
     * @param   typeRef     SEDS type ref
     *
     * @return  Pointer to the found type, nullptr otherwise
     */
    auto findAsn1Type(const ::seds::model::DataTypeRef &typeRef) -> Asn1Acn::Types::Type *;
    /**
     * @brief   Find ASN.1 definitions where ASN.1 type is present
     *
     * @param   typeRef     SEDS type ref
     *
     * @return  Pointer to the found definitions, nullptr otherwise
     */
    auto findAsn1TypeDefinitions(const ::seds::model::DataTypeRef &typeRef) -> Asn1Acn::Definitions *;
    /**
     * @brief   Find SEDS interface declaration by SEDS interface reference
     *
     * @param   interfaceRef    SEDS interface ref
     *
     * @return  Pointer to the found interface declaration, nullptr otherwise
     */
    auto findInterfaceDeclaration(const ::seds::model::InterfaceDeclarationRef &interfaceRef)
            -> const ::seds::model::InterfaceDeclaration *;

    /**
     * @brief   Check if given type is known
     *
     * @param   typeRef     SEDS type ref
     *
     * @return  True if type exists, false otherwise
     */
    auto hasAsn1Type(const ::seds::model::DataTypeRef &typeRef) const -> bool;

    /**
     * @brief   Getter for current SEDS package
     *
     * @return  Pointer to the package
     */
    auto getSedsPackage() const -> const ::seds::model::Package *;
    /**
     * @brief   Getter for SEDS package with given name
     *
     * @param   packageName     Name of the package to find
     *
     * @return  Pointer to the found package, nullptr otherwise
     */
    auto getSedsPackage(const QString &packageName) const -> const ::seds::model::Package *;
    /**
     * @brief   Getter for current ASN.1 definitions
     *
     * @return  Pointer to hte current ASN.1 definitions
     */
    auto getAsn1Definitions() const -> Asn1Acn::Definitions *;
    /**
     * @brief   Getter for ASN.1 definitions with given name
     *
     * @param   asn1FileName    Name of the ASN.1 file to find
     *
     * @return  Pointer to the found file, nullptr otherwise
     */
    auto getAsn1Definitions(const QString &asn1FileName) const -> Asn1Acn::Definitions *;

    /**
     * @brief   Add import of the given type
     *
     * @param   packageName     Name of the package where type is present
     * @param   typeName        Name of the imported type
     */
    auto importType(const QString &packageName, const QString &typeName) -> void;

    /**
     * @brief   Getter for name of the current package
     *
     * @return  Current package name
     */
    auto packageName() const -> const QString &;
    /**
     * @brief   Getter for name of the current definitions
     *
     * @return  Current definitions name
     */
    auto definitionsName() const -> const QString &;
    /**
     * @brief   Getter for name of the current component
     *
     * @return  Current component name
     */
    auto componentName() const -> const QString &;
    /**
     * @brief   Getter for array size threshold
     *
     * @return  Array size threshold, nullopt otherwise
     */
    auto arraySizeThreshold() const -> std::optional<uint64_t>;

public:
    /**
     * @brief   Clone this context for given package name
     *
     * @param   packageName     Package name
     *
     * @return  New context
     */
    auto cloneForPackage(const QString &packageName) -> Context;

private:
    const ::seds::model::Package *m_sedsPackage;
    Asn1Acn::Definitions *m_definitions;
    Asn1Acn::Definitions *m_parentDefinitions;
    const ::seds::model::Component *m_component;

    const std::list<const ::seds::model::Package *> &m_sedsPackages;
    const std::vector<std::unique_ptr<Asn1Acn::File>> &m_asn1Files;

    const Options &m_options;
};

} // namespace conversion::asn1::translator::seds
