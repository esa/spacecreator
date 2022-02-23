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

#include "packagesdependencyresolver.h"

#include <algorithm>
#include <conversion/common/escaper/escaper.h>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/containerdatatype.h>

namespace conversion::asn1::translator {

PackagesDependencyResolver::NotDagException::NotDagException()
    : ConversionException("Packages doesn't make a DAG")
{
}

PackagesDependencyResolver::UnknownPackageReferenceException::UnknownPackageReferenceException(
        const QString &packageName)
    : ConversionException(QString("Reference to unknown package '%1'").arg(packageName))
{
}

PackagesDependencyResolver::ResultList PackagesDependencyResolver::resolve(
        const std::vector<seds::model::Package> *packages)
{
    m_marks.clear();
    m_result.clear();

    m_packages = packages;

    for (const auto &package : *m_packages) {
        visit(&package);
    }

    return m_result;
}

void PackagesDependencyResolver::visit(const seds::model::Package *package)
{
    if (isPermanentlyMarked(package)) {
        return;
    }

    if (isTemporarilyMarked(package)) {
        throw NotDagException();
    }

    markTemporary(package);

    std::set<Asn1Acn::ImportedType> importedTypes;

    for (const auto &dataType : package->dataTypes()) {
        if (const auto *arrayDataType = std::get_if<seds::model::ArrayDataType>(&dataType)) {
            auto importedType = handleArray(*arrayDataType);

            if (importedType) {
                importedTypes.insert(std::move(*importedType));
            }
        } else if (const auto *containerDataType = std::get_if<seds::model::ContainerDataType>(&dataType)) {
            auto containerImportedTypes = handleContainer(*containerDataType);
            std::copy(containerImportedTypes.begin(), containerImportedTypes.end(),
                    std::inserter(importedTypes, importedTypes.end()));
        } else if (const auto *subRangeDataType = std::get_if<seds::model::SubRangeDataType>(&dataType)) {
            auto importedType = handleSubRangeDataType(*subRangeDataType);

            if (importedType) {
                importedTypes.insert(std::move(*importedType));
            }
        }
    }

    markPermanent(package);

    m_result.push_back({ package, std::move(importedTypes) });
}

std::optional<Asn1Acn::ImportedType> PackagesDependencyResolver::handleArray(
        const seds::model::ArrayDataType &arrayDataType)
{
    const auto &typeRef = arrayDataType.typeRef();

    if (typeRef.packageStr()) {
        return createImportedType(typeRef);
    }

    return std::nullopt;
}

std::set<Asn1Acn::ImportedType> PackagesDependencyResolver::handleContainer(
        const seds::model::ContainerDataType &containerDataType)
{
    std::set<Asn1Acn::ImportedType> importedTypes;

    const auto visitor = [&](auto &&entry) {
        using T = std::decay_t<decltype(entry)>;
        if constexpr (std::is_same_v<T, seds::model::PaddingEntry>) {
            return;
        } else {
            const auto &typeRef = entry.typeRef();

            if (typeRef.packageStr()) {
                auto importedType = createImportedType(typeRef);
                importedTypes.insert(std::move(importedType));
            }
        }
    };

    for (const auto &containerEntry : containerDataType.entries()) {
        std::visit(visitor, containerEntry);
    }
    for (const auto &containerTrailerEntry : containerDataType.trailerEntries()) {
        std::visit(visitor, containerTrailerEntry);
    }

    for (const auto &containerConstraint : containerDataType.constraints()) {
        if (auto typeConstraint = std::get_if<seds::model::ContainerTypeConstraint>(&containerConstraint)) {
            const auto &typeRef = typeConstraint->type();

            if (typeRef.packageStr()) {
                auto importedType = createImportedType(typeRef);
                importedTypes.insert(std::move(importedType));
            }
        }
    }

    return importedTypes;
}

std::optional<Asn1Acn::ImportedType> PackagesDependencyResolver::handleSubRangeDataType(
        const seds::model::SubRangeDataType &subRangeDataType)
{
    const auto &typeRef = subRangeDataType.type();

    if (typeRef.packageStr()) {
        return createImportedType(typeRef);
    }

    return std::nullopt;
}

Asn1Acn::ImportedType PackagesDependencyResolver::createImportedType(const seds::model::DataTypeRef &typeRef)
{
    const auto &packageName = *typeRef.packageStr();
    const auto package = findPackage(packageName);

    visit(package);

    const auto asn1ModuleName = Escaper::escapeAsn1PackageName(packageName);
    const auto asn1TypeName = Escaper::escapeAsn1TypeName(typeRef.nameStr());

    return Asn1Acn::ImportedType(asn1ModuleName, asn1TypeName);
}

const seds::model::Package *PackagesDependencyResolver::findPackage(const QString &packageName) const
{
    auto foundPackage = std::find_if(m_packages->begin(), m_packages->end(),
            [&](const auto &package) { return package.nameStr() == packageName; });

    if (foundPackage != m_packages->end()) {
        return &(*foundPackage);
    }

    throw UnknownPackageReferenceException(packageName);
    return nullptr;
}

void PackagesDependencyResolver::markTemporary(const seds::model::Package *package)
{
    m_marks.insert({ package, MarkType::Temporary });
}

void PackagesDependencyResolver::markPermanent(const seds::model::Package *package)
{
    m_marks[package] = MarkType::Permanent;
}

bool PackagesDependencyResolver::isTemporarilyMarked(const seds::model::Package *package)
{
    if (m_marks.count(package) == 0) {
        return false;
    }

    return m_marks.at(package) == MarkType::Temporary;
}

bool PackagesDependencyResolver::isPermanentlyMarked(const seds::model::Package *package)
{
    if (m_marks.count(package) == 0) {
        return false;
    }

    return m_marks.at(package) == MarkType::Permanent;
}

} // namespace conversion::asn1::translator {
