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
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/types/arraydatatype.h>
#include <seds/SedsModel/types/containerdatatype.h>

using conversion::translator::NotDagException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredInterfaceException;
using conversion::translator::UndeclaredPackageReferenceException;

namespace conversion::asn1::translator {

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
        throw NotDagException(package->qualifiedName().pathStr());
    }

    markTemporary(package);

    std::set<Asn1Acn::ImportedType> importedTypes;

    handleDataTypes(package->dataTypes(), importedTypes);
    handleComponents(package->components(), package, importedTypes);

    markPermanent(package);

    m_result.push_back({ package, std::move(importedTypes) });
}

void PackagesDependencyResolver::handleDataTypes(
        const std::vector<seds::model::DataType> &dataTypes, std::set<Asn1Acn::ImportedType> &importedTypes)
{
    for (const auto &dataType : dataTypes) {
        if (const auto *arrayDataType = std::get_if<seds::model::ArrayDataType>(&dataType)) {
            auto importedType = handleArray(*arrayDataType);

            if (importedType) {
                importedTypes.insert(std::move(*importedType));
            }

            handleArrayDimensions(arrayDataType->dimensions(), importedTypes);
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
}

std::optional<Asn1Acn::ImportedType> PackagesDependencyResolver::handleArray(
        const seds::model::ArrayDataType &arrayDataType)
{
    const auto &typeRef = arrayDataType.type();

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
            const auto &typeRef = entry.type();

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

void PackagesDependencyResolver::handleComponents(const std::vector<seds::model::Component> &components,
        const seds::model::Package *package, std::set<Asn1Acn::ImportedType> &importedTypes)
{
    for (const auto &component : components) {
        handleDataTypes(component.dataTypes(), importedTypes);
        handleInterfaces(component.providedInterfaces(), component, package, importedTypes);
        handleInterfaces(component.requiredInterfaces(), component, package, importedTypes);
    }
}

void PackagesDependencyResolver::handleInterfaces(const std::vector<seds::model::Interface> &interfaces,
        const seds::model::Component &component, const seds::model::Package *package,
        std::set<Asn1Acn::ImportedType> &importedTypes)
{
    for (const auto &interface : interfaces) {
        handleTypeMapSet(interface.genericTypeMapSet(), importedTypes);

        const auto &interfaceDeclaration = findInterfaceDeclaration(interface.type().nameStr(), component, package);
        handleInterfaceDeclaration(interfaceDeclaration, importedTypes);
    }
}

void PackagesDependencyResolver::handleTypeMapSet(
        const std::optional<seds::model::GenericTypeMapSet> &typeMapSet, std::set<Asn1Acn::ImportedType> &importedTypes)
{
    if (!typeMapSet) {
        return;
    }

    for (const auto &typeMap : typeMapSet->genericTypeMaps()) {
        handleTypeMap(typeMap, importedTypes);
    }

    handleAlternateSet(typeMapSet->alternateSet(), importedTypes);
}

void PackagesDependencyResolver::handleTypeMap(
        const seds::model::GenericTypeMap &typeMap, std::set<Asn1Acn::ImportedType> &importedTypes)
{
    const auto concreteTypeRef = typeMap.type();

    if (concreteTypeRef.packageStr()) {
        auto importedType = createImportedType(concreteTypeRef);
        importedTypes.insert(std::move(importedType));
    }
}

void PackagesDependencyResolver::handleAlternateSet(const std::optional<seds::model::GenericAlternateSet> &alternateSet,
        std::set<Asn1Acn::ImportedType> &importedTypes)
{
    if (!alternateSet) {
        return;
    }

    for (const auto &alternate : alternateSet->alternates()) {
        for (const auto &typeMap : alternate.genericTypeMaps()) {
            handleTypeMap(typeMap, importedTypes);
        }
    }
}

void PackagesDependencyResolver::handleInterfaceDeclaration(
        const seds::model::InterfaceDeclaration &interfaceDeclaration, std::set<Asn1Acn::ImportedType> &importedTypes)
{
    handleParameters(interfaceDeclaration.parameters(), importedTypes);
    handleCommands(interfaceDeclaration.commands(), importedTypes);
}

void PackagesDependencyResolver::handleParameters(
        const std::vector<seds::model::InterfaceParameter> &parameters, std::set<Asn1Acn::ImportedType> &importedTypes)
{
    for (const auto &parameter : parameters) {
        const auto typeRef = parameter.type();

        if (typeRef.packageStr()) {
            auto importedType = createImportedType(typeRef);
            importedTypes.insert(std::move(importedType));
        }
    }
}

void PackagesDependencyResolver::handleCommands(
        const std::vector<seds::model::InterfaceCommand> &commands, std::set<Asn1Acn::ImportedType> &importedTypes)
{
    for (const auto &command : commands) {
        for (const auto &argument : command.arguments()) {
            const auto typeRef = argument.type();

            if (typeRef.packageStr()) {
                auto importedType = createImportedType(typeRef);
                importedTypes.insert(std::move(importedType));
            }
        }
    }
}

void PackagesDependencyResolver::handleArrayDimensions(
        const std::vector<seds::model::DimensionSize> &dimensions, std::set<Asn1Acn::ImportedType> &importedTypes)
{
    Q_UNUSED(importedTypes);

    for (const auto &dimension : dimensions) {
        Q_UNUSED(dimension);
    }
}

Asn1Acn::ImportedType PackagesDependencyResolver::createImportedType(const seds::model::DataTypeRef &typeRef)
{
    if (!typeRef.packageStr().has_value()) {
        auto errorMessage = QString("Failure in creation of an imported type \"%1\" because no package name is present "
                                    "in the data type reference")
                                    .arg(typeRef.nameStr());
        throw TranslationException(std::move(errorMessage));
    }

    const auto package = findPackage(*typeRef.packageStr());

    visit(package);

    const auto asn1ModuleName = Escaper::escapeAsn1PackageName(*typeRef.packageStr());
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

    throw UndeclaredPackageReferenceException(packageName);
    return nullptr;
}

const seds::model::InterfaceDeclaration &PackagesDependencyResolver::findInterfaceDeclaration(
        const QString &name, const seds::model::Component &sedsComponent, const seds::model::Package *sedsPackage)
{
    const auto &sedsComponentInterfaceDeclarations = sedsComponent.declaredInterfaces();
    auto found = std::find_if(sedsComponentInterfaceDeclarations.begin(), sedsComponentInterfaceDeclarations.end(),
            [&name](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                return interfaceDeclaration.nameStr() == name;
            });
    if (found != sedsComponentInterfaceDeclarations.end()) {
        return *found;
    }

    const auto &sedsPackageInterfaceDeclarations = sedsPackage->declaredInterfaces();
    found = std::find_if(sedsPackageInterfaceDeclarations.begin(), sedsPackageInterfaceDeclarations.end(),
            [&name](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                return interfaceDeclaration.nameStr() == name;
            });
    if (found != sedsPackageInterfaceDeclarations.end()) {
        return *found;
    }

    throw UndeclaredInterfaceException(name);
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
