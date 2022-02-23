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

#include <asn1library/asn1/importedtype.h>
#include <conversion/common/exceptions.h>
#include <list>
#include <seds/SedsModel/package/package.h>
#include <set>
#include <unordered_map>
#include <vector>

namespace conversion::asn1::translator {

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
public:
    /**
     * @brief   Exception thrown when data types to sort doesn't create a DAG
     *
     * Exception for cyclic dependency
     */
    class NotDagException final : public ConversionException
    {
    public:
        /**
         * @brief   Constructor
         */
        NotDagException();
    };

    /**
     * @brief   Exception thrown when can find referenced package
     */
    class UnknownPackageReferenceException final : public ConversionException
    {
    public:
        /**
         * @brief   Constructor
         *
         * @param   packageName     Name of the referenced package
         */
        UnknownPackageReferenceException(const QString &dataTypeName);
    };

private:
    /// @brief  Type of the mark for the soring
    enum class MarkType
    {
        Temporary,
        Permanent
    };

    /// @brief  Package with types that it imports
    struct Result final {
        const seds::model::Package *package;
        std::set<Asn1Acn::ImportedType> importedTypes;
    };

    using MarksMap = std::unordered_map<const seds::model::Package *, MarkType>;
    using ResultList = std::list<Result>;

public:
    /**
     * @brief   Resolve dependencies between given packages
     *
     * @param   packages    Data types to sort
     *
     * @return  Sorted list of packages
     */
    auto resolve(const std::vector<seds::model::Package> *packages) -> ResultList;

private:
    auto visit(const seds::model::Package *package) -> void;

    auto handleArray(const seds::model::ArrayDataType &arrayDataType) -> std::optional<Asn1Acn::ImportedType>;
    auto handleContainer(const seds::model::ContainerDataType &containerDataType) -> std::set<Asn1Acn::ImportedType>;
    auto handleSubRangeDataType(const seds::model::SubRangeDataType &subRangeDataType)
            -> std::optional<Asn1Acn::ImportedType>;

    auto createImportedType(const seds::model::DataTypeRef &typeRef) -> Asn1Acn::ImportedType;

    auto findPackage(const QString &packageName) const -> const seds::model::Package *;

    auto markTemporary(const seds::model::Package *package) -> void;
    auto markPermanent(const seds::model::Package *package) -> void;
    auto isTemporarilyMarked(const seds::model::Package *package) -> bool;
    auto isPermanentlyMarked(const seds::model::Package *package) -> bool;

private:
    const std::vector<seds::model::Package> *m_packages;

    MarksMap m_marks;
    ResultList m_result;
};

} // namespace conversion::asn1::translator {
