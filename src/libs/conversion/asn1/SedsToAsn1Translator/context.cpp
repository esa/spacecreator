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

#include "context.h"

#include "specialized/descriptiontranslator.h"

#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <iostream>
#include <seds/SedsModel/package/package.h>

using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredDataTypeException;
using conversion::translator::UndeclaredInterfaceException;

namespace conversion::asn1::translator {

Context::Context(const seds::model::Package *sedsPackage, Asn1Acn::Definitions *definitions,
        Asn1Acn::Definitions *parentDefinitions, const seds::model::Component *component,
        const std::list<const seds::model::Package *> &sedsPackages,
        const std::vector<std::unique_ptr<Asn1Acn::File>> &asn1Files, const Options &options)
    : m_sedsPackage(sedsPackage)
    , m_definitions(definitions)
    , m_parentDefinitions(parentDefinitions)
    , m_component(component)
    , m_sedsPackages(sedsPackages)
    , m_asn1Files(asn1Files)
    , m_options(options)
{
}

void Context::addAsn1Type(
        std::unique_ptr<Asn1Acn::Types::Type> asn1Type, const seds::model::Description *sedsDescription)
{
    const auto &asn1TypeName = asn1Type->identifier();
    auto asn1TypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
            asn1TypeName, asn1TypeName, Asn1Acn::SourceLocation(), std::move(asn1Type));
    DescriptionTranslator::translate(sedsDescription, asn1TypeAssignment.get());

    m_definitions->addType(std::move(asn1TypeAssignment));
}

const seds::model::DataType *Context::findSedsType(const seds::model::DataTypeRef &typeRef)
{
    const auto package = typeRef.packageStr() ? getSedsPackage(*typeRef.packageStr()) : m_sedsPackage;
    const auto type = package->dataType(typeRef.nameStr());

    if (type == nullptr) {
        throw UndeclaredDataTypeException(typeRef.value().pathStr());
    }

    return type;
}

Asn1Acn::Types::Type *Context::findAsn1Type(const seds::model::DataTypeRef &typeRef)
{
    const auto &typeName = typeRef.nameStr();

    if (typeRef.packageStr()) {
        const auto asn1DefinitionsName = Escaper::escapeAsn1PackageName(*typeRef.packageStr());
        auto definitions = getAsn1Definitions(asn1DefinitionsName);
        auto typeAssignment = definitions->type(typeName);

        if (typeAssignment != nullptr) {
            Asn1Acn::ImportedType importedType(asn1DefinitionsName, typeName);
            m_definitions->addImportedType(importedType);

            return typeAssignment->type();
        }
    } else {
        auto *typeAssignment = m_definitions->type(typeName);
        if (typeAssignment != nullptr) {
            return typeAssignment->type();
        }

        if (m_parentDefinitions != nullptr) {
            typeAssignment = m_parentDefinitions->type(typeName);

            if (typeAssignment != nullptr) {
                Asn1Acn::ImportedType importedType(m_parentDefinitions->name(), typeName);
                m_definitions->addImportedType(importedType);

                return typeAssignment->type();
            }
        }
    }

    throw MissingAsn1TypeDefinitionException(typeRef.value().pathStr());
}

Asn1Acn::Definitions *Context::findAsn1TypeDefinitions(const seds::model::DataTypeRef &typeRef)
{
    const auto &typeName = typeRef.nameStr();

    if (typeRef.packageStr()) {
        const auto asn1DefinitionsName = Escaper::escapeAsn1PackageName(*typeRef.packageStr());
        auto definitions = getAsn1Definitions(asn1DefinitionsName);
        auto typeAssignment = definitions->type(typeName);

        if (typeAssignment != nullptr) {
            return definitions;
        }
    } else {

        auto *typeAssignment = m_definitions->type(typeName);
        if (typeAssignment != nullptr) {
            return m_definitions;
        }

        if (m_parentDefinitions != nullptr) {
            typeAssignment = m_parentDefinitions->type(typeName);
            if (typeAssignment != nullptr) {
                return m_parentDefinitions;
            }
        }
    }

    throw MissingAsn1TypeDefinitionException(typeRef.value().pathStr());
}

const seds::model::InterfaceDeclaration *Context::findInterfaceDeclaration(
        const seds::model::InterfaceDeclarationRef &interfaceRef)
{
    const auto &name = interfaceRef.nameStr();

    const auto namesEqual = [&name](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
        return interfaceDeclaration.nameStr() == name;
    };

    if (interfaceRef.packageStr()) {
        const auto package = getSedsPackage(*interfaceRef.packageStr());
        const auto &interfaceDeclarations = package->declaredInterfaces();

        const auto found = std::find_if(interfaceDeclarations.begin(), interfaceDeclarations.end(), namesEqual);
        if (found != interfaceDeclarations.end()) {
            return &(*found);
        }
    } else {
        if (m_component != nullptr) {
            const auto &componentInterfaceDeclarations = m_component->declaredInterfaces();

            const auto found = std::find_if(
                    componentInterfaceDeclarations.begin(), componentInterfaceDeclarations.end(), namesEqual);
            if (found != componentInterfaceDeclarations.end()) {
                return &(*found);
            }
        }

        const auto &packageInterfaceDeclarations = m_sedsPackage->declaredInterfaces();
        const auto found =
                std::find_if(packageInterfaceDeclarations.begin(), packageInterfaceDeclarations.end(), namesEqual);
        if (found != packageInterfaceDeclarations.end()) {
            return &(*found);
        }
    }

    throw UndeclaredInterfaceException(interfaceRef.value().pathStr());
}

bool Context::hasAsn1Type(const seds::model::DataTypeRef &typeRef) const
{
    const auto &typeName = typeRef.nameStr();

    if (typeRef.packageStr()) {
        const auto asn1DefinitionsName = Escaper::escapeAsn1PackageName(*typeRef.packageStr());
        auto definitions = getAsn1Definitions(asn1DefinitionsName);
        auto typeAssignment = definitions->type(typeName);

        if (typeAssignment != nullptr) {
            return true;
        }
    } else {
        auto *typeAssignment = m_definitions->type(typeName);
        if (typeAssignment != nullptr) {
            return true;
        }

        if (m_parentDefinitions != nullptr) {
            typeAssignment = m_parentDefinitions->type(typeName);

            if (typeAssignment != nullptr) {
                return true;
            }
        }
    }

    return false;
}

const seds::model::Package *Context::getSedsPackage() const
{
    return m_sedsPackage;
}

const seds::model::Package *Context::getSedsPackage(const QString &packageName) const
{
    const auto sedsPackage = std::find_if(m_sedsPackages.begin(), m_sedsPackages.end(),
            [&](const auto &package) { return package->nameStr() == packageName; });

    if (sedsPackage == m_sedsPackages.end()) {
        auto message = QString("Unable to find package \"%1\"").arg(packageName);
        throw TranslationException(std::move(message));
    }

    return *sedsPackage;
}

Asn1Acn::Definitions *Context::getAsn1Definitions() const
{
    return m_definitions;
}

Asn1Acn::Definitions *Context::getAsn1Definitions(const QString &asn1FileName) const
{
    auto asn1File = std::find_if(std::begin(m_asn1Files), std::end(m_asn1Files),
            [&](const auto &file) { return file->name() == asn1FileName; });
    if (asn1File == m_asn1Files.end()) {
        auto message = QString("Unable to find file %1 in the ASN.1 model").arg(asn1FileName);
        throw TranslationException(std::move(message));
    }

    auto *asn1Definitions = (*asn1File)->definitions(asn1FileName);
    if (!asn1Definitions) {
        auto message = QString("ASN.1 file %1 doesn't have definitions named %1").arg(asn1FileName);
        throw TranslationException(std::move(message));
    }

    return asn1Definitions;
}

void Context::importType(const QString &packageName, const QString &typeName)
{
    const auto definitionsName = Escaper::escapeAsn1PackageName(packageName);

    if (definitionsName == m_definitions->name()) {
        return;
    }

    const auto asn1TypeName = Escaper::escapeAsn1TypeName(typeName);

    Asn1Acn::ImportedType importedType(definitionsName, asn1TypeName);
    m_definitions->addImportedType(importedType);
}

const QString &Context::packageName() const
{
    return m_sedsPackage->nameStr();
}

const QString &Context::definitionsName() const
{
    return m_definitions->name();
}

const QString &Context::componentName() const
{
    return m_component->nameStr();
}

std::optional<uint64_t> Context::arraySizeThreshold() const
{
    if (m_options.isSet(Asn1Options::sequenceSizeThreshold)) {
        return m_options.value(Asn1Options::sequenceSizeThreshold)->toLongLong();
    } else {
        return std::nullopt;
    }
}

Context Context::cloneForPackage(const QString &packageName)
{
    auto package = getSedsPackage(packageName);

    const auto definitionsName = Escaper::escapeAsn1PackageName(packageName);
    auto definitions = getAsn1Definitions(definitionsName);

    return Context(package, definitions, nullptr, nullptr, m_sedsPackages, m_asn1Files, m_options);
}

} // namespace conversion::asn1::translator
