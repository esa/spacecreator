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

#include <conversion/common/exceptions.h>
#include <list>
#include <seds/SedsModel/package/package.h>
#include <set>
#include <unordered_map>
#include <vector>

namespace conversion::asn1::translator::seds {

/**
 * @brief   Utility used for resolving dependencies between SEDS packages
 *
 * SEDS allows to reference a type from other package before it's defined.
 * ASN.1 model on the other hand requires Type object to be passed while creating a UserdefinedType
 * Because of that we have to sort packages topologically so that we are sure that references
 * are already valid
 */
class PackagesDependencyResolver final
{
private:
    /// @brief  Type of the mark for the sorting
    enum class MarkType
    {
        Temporary,
        Permanent
    };

    using MarksMap = std::unordered_map<const ::seds::model::Package *, MarkType>;
    using ResultList = std::list<const ::seds::model::Package *>;

public:
    /**
     * @brief   Resolve dependencies between given packages
     *
     * @param   packages    Data types to sort
     *
     * @return  Sorted list of packages
     */
    auto resolve(const std::vector<::seds::model::Package> *packages) -> ResultList;

private:
    auto visit(const ::seds::model::Package *package) -> void;

    auto handleDataTypes(const std::vector<::seds::model::DataType> &dataTypes) -> void;
    auto handleArray(const ::seds::model::ArrayDataType &arrayDataType) -> void;
    auto handleContainer(const ::seds::model::ContainerDataType &containerDataType) -> void;
    auto handleSubRangeDataType(const ::seds::model::SubRangeDataType &subRangeDataType) -> void;

    auto handleInterfaceDeclarations(const std::vector<::seds::model::InterfaceDeclaration> &interfaceDeclarations)
            -> void;
    auto handleInterfaceDeclaration(const ::seds::model::InterfaceDeclaration &interfaceDeclaration) -> void;
    auto handleParameters(const std::vector<::seds::model::InterfaceParameter> &parameters) -> void;
    auto handleCommands(const std::vector<::seds::model::InterfaceCommand> &commands) -> void;

    auto handleComponents(
            const std::vector<::seds::model::Component> &components, const ::seds::model::Package *package) -> void;
    auto handleInterfaces(const std::vector<::seds::model::Interface> &interfaces,
            const ::seds::model::Component &component, const ::seds::model::Package *package) -> void;

    auto handleTypeMapSet(const std::optional<::seds::model::GenericTypeMapSet> &typeMapSet) -> void;
    auto handleAlternateSet(const std::optional<::seds::model::GenericAlternateSet> &alternateSet) -> void;

    auto handleArrayDimensions(const std::vector<::seds::model::DimensionSize> &dimensions) -> void;

    auto handleDataTypeRef(const ::seds::model::DataTypeRef &typeRef) -> void;

    auto findPackage(const QString &packageName) const -> const ::seds::model::Package *;
    auto findInterfaceDeclaration(const QString &name, const ::seds::model::Component &sedsComponent,
            const ::seds::model::Package *sedsPackage) -> const ::seds::model::InterfaceDeclaration &;

    auto markTemporary(const ::seds::model::Package *package) -> void;
    auto markPermanent(const ::seds::model::Package *package) -> void;
    auto isTemporarilyMarked(const ::seds::model::Package *package) -> bool;
    auto isPermanentlyMarked(const ::seds::model::Package *package) -> bool;

private:
    const std::vector<::seds::model::Package> *m_packages;

    MarksMap m_marks;
    ResultList m_result;
};

} // namespace conversion::asn1::translator::seds {
