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

#include "translator.h"

#include "datatypesdependencyresolver.h"
#include "packagesdependencyresolver.h"
#include "specialized/datatypetranslatorvisitor.h"
#include "specialized/descriptiontranslator.h"
#include "specialized/interfacedeclarationtypecreator.h"

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/sourcelocation.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/sedsmodel.h>

using Asn1Acn::Asn1Model;
using conversion::Escaper;
using conversion::translator::TranslationException;
using seds::model::SedsModel;

namespace conversion::asn1::translator {

std::vector<std::unique_ptr<Model>> SedsToAsn1Translator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    const auto *sedsModel = getModel<SedsModel>(sourceModels);

    return translateSedsModel(sedsModel, options);
}

ModelType SedsToAsn1Translator::getSourceModelType() const
{
    return ModelType::Seds;
}

ModelType SedsToAsn1Translator::getTargetModelType() const
{
    return ModelType::Asn1;
}

std::set<ModelType> SedsToAsn1Translator::getDependencies() const
{
    static std::set<ModelType> dependencies { ModelType::Seds };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> SedsToAsn1Translator::translateSedsModel(
        const SedsModel *sedsModel, const Options &options) const
{
    auto asn1Model = std::make_unique<Asn1Model>();

    const auto &sedsModelData = sedsModel->data();

    if (std::holds_alternative<seds::model::PackageFile>(sedsModelData)) {
        const auto &sedsPackage = std::get<seds::model::PackageFile>(sedsModelData).package();
        translatePackage(&sedsPackage, asn1Model.get(), { &sedsPackage }, options);
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &sedsPackages = std::get<seds::model::DataSheet>(sedsModelData).packages();

        PackagesDependencyResolver packagesDependencyResolver;
        const auto resolvedSedsPackages = packagesDependencyResolver.resolve(&sedsPackages);

        for (const auto sedsPackage : resolvedSedsPackages) {
            translatePackage(sedsPackage, asn1Model.get(), resolvedSedsPackages, options);
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(asn1Model));

    return result;
}

void SedsToAsn1Translator::translatePackage(const seds::model::Package *sedsPackage, Asn1Acn::Asn1Model *asn1Model,
        const std::list<const seds::model::Package *> &sedsPackages, const Options &options) const
{
    DataTypesDependencyResolver typesDependencyResolver;

    // Create package ASN.1 definitions
    const auto packageAsn1DefinitionsName = Escaper::escapeAsn1PackageName(sedsPackage->nameStr());
    auto packageAsn1Definitions =
            std::make_unique<Asn1Acn::Definitions>(packageAsn1DefinitionsName, Asn1Acn::SourceLocation());
    auto packageAsn1DefinitionsPtr = packageAsn1Definitions.get();
    DescriptionTranslator::translate(sedsPackage, packageAsn1Definitions.get());

    // Create package context
    Context packageContext(sedsPackage, packageAsn1DefinitionsPtr, nullptr, sedsPackages, asn1Model->data(), options);

    // Translate package data types
    const auto packageSedsTypes = collectDataTypes(sedsPackage);
    const auto packageResolvedSedsTypes = typesDependencyResolver.resolve(&packageSedsTypes, nullptr);
    translateDataTypeSet(packageResolvedSedsTypes, packageContext);

    // Translate package interface declarations
    const auto &packageInterfaceDeclarations = sedsPackage->declaredInterfaces();
    translateInterfaceDeclarations(packageInterfaceDeclarations, packageContext);

    // Create and add package ASN.1 file
    auto packageAsn1File = std::make_unique<Asn1Acn::File>(packageAsn1DefinitionsName);
    packageAsn1File->add(std::move(packageAsn1Definitions));

    asn1Model->addAsn1File(std::move(packageAsn1File));

    for (const auto &sedsComponent : sedsPackage->components()) {
        // Create component ASN.1 definitions
        const auto componentAsn1DefinitionsName = QString("%1-%2")
                                                          .arg(packageAsn1DefinitionsName)
                                                          .arg(Escaper::escapeAsn1PackageName(sedsComponent.nameStr()));
        auto componentAsn1Definitions =
                std::make_unique<Asn1Acn::Definitions>(componentAsn1DefinitionsName, Asn1Acn::SourceLocation());
        DescriptionTranslator::translate(&sedsComponent, componentAsn1Definitions.get());

        // Create component context
        Context componentContext(sedsPackage, componentAsn1Definitions.get(), packageAsn1DefinitionsPtr, sedsPackages,
                asn1Model->data(), options);

        // Translate component data types
        const auto componentSedsTypes = collectDataTypes(sedsComponent);
        const auto componentResolvedSedsTypes = typesDependencyResolver.resolve(&componentSedsTypes, &packageSedsTypes);
        translateDataTypeSet(componentResolvedSedsTypes, componentContext);

        // Translate component interface declarations
        const auto &componentInterfaceDeclarations = sedsComponent.declaredInterfaces();
        translateInterfaceDeclarations(componentInterfaceDeclarations, componentContext);

        // Create and add component ASN.1 file
        auto componentAsn1File = std::make_unique<Asn1Acn::File>(componentAsn1DefinitionsName);
        componentAsn1File->add(std::move(componentAsn1Definitions));

        asn1Model->addAsn1File(std::move(componentAsn1File));
    }
}

void SedsToAsn1Translator::translateDataTypeSet(
        const std::list<const seds::model::DataType *> &dataTypes, Context &context) const
{
    DataTypeTranslatorVisitor dataTypeTranslator(context);

    for (const auto dataType : dataTypes) {
        std::visit(dataTypeTranslator, *dataType);
    }
}

void SedsToAsn1Translator::translateInterfaceDeclarations(
        const std::vector<seds::model::InterfaceDeclaration> &interfaceDeclarations, Context &context) const
{
    InterfaceDeclarationTypeCreator typeCreator(context);

    for (const auto &interfaceDeclaration : interfaceDeclarations) {
        typeCreator.createTypes(interfaceDeclaration);
    }
}

std::vector<const seds::model::DataType *> SedsToAsn1Translator::collectDataTypes(
        const seds::model::Package *package) const
{
    const auto extractPointer = [](const auto &dataType) { return &dataType; };

    std::vector<const seds::model::DataType *> sedsDataTypes;

    std::transform(package->dataTypes().begin(), package->dataTypes().end(), std::back_inserter(sedsDataTypes),
            extractPointer);

    return sedsDataTypes;
}

std::vector<const seds::model::DataType *> SedsToAsn1Translator::collectDataTypes(
        const seds::model::Component &component) const
{
    const auto extractPointer = [](const auto &dataType) { return &dataType; };

    std::vector<const seds::model::DataType *> sedsDataTypes;

    std::transform(component.dataTypes().begin(), component.dataTypes().end(), std::back_inserter(sedsDataTypes),
            extractPointer);

    return sedsDataTypes;
}

} // namespace conversion::asn1::translator
