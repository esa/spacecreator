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

    handleDataTypes(package->dataTypes());
    handleComponents(package->components(), package);

    markPermanent(package);

    m_result.push_back(package);
}

void PackagesDependencyResolver::handleDataTypes(const std::vector<seds::model::DataType> &dataTypes)
{
    for (const auto &dataType : dataTypes) {
        if (const auto *arrayDataType = std::get_if<seds::model::ArrayDataType>(&dataType)) {
            handleArray(*arrayDataType);
            handleArrayDimensions(arrayDataType->dimensions());
        } else if (const auto *containerDataType = std::get_if<seds::model::ContainerDataType>(&dataType)) {
            handleContainer(*containerDataType);
        } else if (const auto *subRangeDataType = std::get_if<seds::model::SubRangeDataType>(&dataType)) {
            handleSubRangeDataType(*subRangeDataType);
        }
    }
}

void PackagesDependencyResolver::handleDataTypeRef(const seds::model::DataTypeRef &dataTypeRef)
{
    if (dataTypeRef.packageStr()) {
        const auto package = findPackage(*dataTypeRef.packageStr());
        visit(package);
    }
}

void PackagesDependencyResolver::handleArray(const seds::model::ArrayDataType &arrayDataType)
{
    handleDataTypeRef(arrayDataType.type());
}

void PackagesDependencyResolver::handleArrayDimensions(const std::vector<seds::model::DimensionSize> &dimensions)
{
    for (const auto &dimension : dimensions) {
        if (dimension.indexTypeRef()) {
            handleDataTypeRef(*dimension.indexTypeRef());
        }
    }
}

void PackagesDependencyResolver::handleContainer(const seds::model::ContainerDataType &containerDataType)
{
    const auto visitor = [&](auto &&entry) {
        using T = std::decay_t<decltype(entry)>;
        if constexpr (std::is_same_v<T, seds::model::PaddingEntry>) {
            return;
        } else {
            handleDataTypeRef(entry.type());
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
            handleDataTypeRef(typeConstraint->type());
        }
    }
}

void PackagesDependencyResolver::handleSubRangeDataType(const seds::model::SubRangeDataType &subRangeDataType)
{
    handleDataTypeRef(subRangeDataType.type());
}

void PackagesDependencyResolver::handleComponents(
        const std::vector<seds::model::Component> &components, const seds::model::Package *package)
{
    for (const auto &component : components) {
        handleDataTypes(component.dataTypes());
        handleInterfaces(component.providedInterfaces(), component, package);
        handleInterfaces(component.requiredInterfaces(), component, package);
    }
}

void PackagesDependencyResolver::handleInterfaces(const std::vector<seds::model::Interface> &interfaces,
        const seds::model::Component &component, const seds::model::Package *package)
{
    for (const auto &interface : interfaces) {
        const auto &interfaceDeclarationRef = interface.type();
        if (interfaceDeclarationRef.packageStr()) {
            const auto otherPackage = findPackage(*interfaceDeclarationRef.packageStr());
            visit(otherPackage);
            continue;
        }

        const auto &interfaceDeclaration =
                findInterfaceDeclaration(interfaceDeclarationRef.nameStr(), component, package);
        handleInterfaceDeclaration(interfaceDeclaration);

        handleTypeMapSet(interface.genericTypeMapSet());
    }
}

void PackagesDependencyResolver::handleInterfaceDeclaration(
        const seds::model::InterfaceDeclaration &interfaceDeclaration)
{
    for (const auto &baseInterface : interfaceDeclaration.baseInterfaces()) {
        const auto &baseInterfaceDeclarationRef = baseInterface.type();
        if (baseInterfaceDeclarationRef.packageStr()) {
            const auto otherPackage = findPackage(*baseInterfaceDeclarationRef.packageStr());
            visit(otherPackage);
            continue;
        }
    }

    handleParameters(interfaceDeclaration.parameters());
    handleCommands(interfaceDeclaration.commands());
}

void PackagesDependencyResolver::handleParameters(const std::vector<seds::model::InterfaceParameter> &parameters)
{
    for (const auto &parameter : parameters) {
        handleDataTypeRef(parameter.type());
    }
}

void PackagesDependencyResolver::handleCommands(const std::vector<seds::model::InterfaceCommand> &commands)
{
    for (const auto &command : commands) {
        for (const auto &argument : command.arguments()) {
            handleDataTypeRef(argument.type());
        }
    }
}

void PackagesDependencyResolver::handleTypeMapSet(const std::optional<seds::model::GenericTypeMapSet> &typeMapSet)
{
    if (!typeMapSet) {
        return;
    }

    for (const auto &typeMap : typeMapSet->genericTypeMaps()) {
        handleDataTypeRef(typeMap.type());
    }

    handleAlternateSet(typeMapSet->alternateSet());
}

void PackagesDependencyResolver::handleAlternateSet(const std::optional<seds::model::GenericAlternateSet> &alternateSet)
{
    if (!alternateSet) {
        return;
    }

    for (const auto &alternate : alternateSet->alternates()) {
        for (const auto &typeMap : alternate.genericTypeMaps()) {
            handleDataTypeRef(typeMap.type());
        }
    }
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
