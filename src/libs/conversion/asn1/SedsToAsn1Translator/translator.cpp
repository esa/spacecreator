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

#include "translator.h"

#include "datatypesdependencyresolver.h"
#include "packagesdependencyresolver.h"
#include "specialized/datatypetranslatorvisitor.h"
#include "specialized/descriptiontranslator.h"

#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/sourcelocation.h>
#include <conversion/asn1/Asn1Options/options.h>
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

    std::optional<uint64_t> sequenceSizeThreshold = std::nullopt;
    if (options.isSet(Asn1Options::sequenceSizeThreshold)) {
        const auto thresholdStr = options.value(Asn1Options::sequenceSizeThreshold);
        sequenceSizeThreshold = thresholdStr->toLongLong();
    }

    return translateSedsModel(sedsModel, sequenceSizeThreshold);
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
        const SedsModel *sedsModel, const std::optional<uint64_t> &sequenceSizeThreshold) const
{
    std::vector<std::unique_ptr<Asn1Acn::File>> asn1Files;

    const auto &sedsModelData = sedsModel->data();
    if (std::holds_alternative<seds::model::PackageFile>(sedsModelData)) {
        const auto &sedsPackage = std::get<seds::model::PackageFile>(sedsModelData).package();

        auto packageFiles = translatePackage(sedsPackage, {}, asn1Files, {}, sequenceSizeThreshold);

        asn1Files.insert(asn1Files.end(), std::make_move_iterator(packageFiles.begin()),
                std::make_move_iterator(packageFiles.end()));
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &sedsPackages = std::get<seds::model::DataSheet>(sedsModelData).packages();

        PackagesDependencyResolver packagesDependencyResolver;
        const auto resolvedSedsPackages = packagesDependencyResolver.resolve(&sedsPackages);

        for (const auto &[sedsPackage, importedTypes] : resolvedSedsPackages) {
            auto packageFiles =
                    translatePackage(*sedsPackage, importedTypes, asn1Files, sedsPackages, sequenceSizeThreshold);
            asn1Files.insert(asn1Files.end(), std::make_move_iterator(packageFiles.begin()),
                    std::make_move_iterator(packageFiles.end()));
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    auto asn1Model = std::make_unique<Asn1Model>(std::move(asn1Files));

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(asn1Model));

    return result;
}

std::vector<std::unique_ptr<Asn1Acn::File>> SedsToAsn1Translator::translatePackage(
        const seds::model::Package &sedsPackage, const std::set<Asn1Acn::ImportedType> &importedTypes,
        const Asn1Model::Data &asn1Files, const std::vector<seds::model::Package> &sedsPackages,
        const std::optional<uint64_t> &sequenceSizeThreshold) const
{
    DataTypesDependencyResolver typesDependencyResolver;

    const auto packageDataTypes = collectDataTypes(sedsPackage);
    const auto resolvedPackageDataTypes = typesDependencyResolver.resolve(&packageDataTypes, nullptr);

    auto packageAsn1Definitions = std::make_unique<Asn1Acn::Definitions>(
            Escaper::escapeAsn1PackageName(sedsPackage.nameStr()), Asn1Acn::SourceLocation());
    DescriptionTranslator::translate(sedsPackage, packageAsn1Definitions.get());

    for (const auto &importedType : importedTypes) {
        packageAsn1Definitions->addImportedType(importedType);
    }

    translateDataTypes(resolvedPackageDataTypes, packageAsn1Definitions.get(), &sedsPackage, asn1Files, sedsPackages,
            sequenceSizeThreshold);

    std::vector<std::unique_ptr<Asn1Acn::File>> result;

    auto packageAsn1File = std::make_unique<Asn1Acn::File>(Escaper::escapeAsn1PackageName(sedsPackage.nameStr()));
    packageAsn1File->add(std::move(packageAsn1Definitions));
    result.push_back(std::move(packageAsn1File));

    for (const auto &sedsComponent : sedsPackage.components()) {
        if (sedsComponent.dataTypes().empty()) {
            // Don't generate ASN.1 package (and therefore a file) for a component without data type declarations
            continue;
        }

        const auto componentDataTypes = collectDataTypes(sedsComponent);
        const auto resolvedComponentDataTypes = typesDependencyResolver.resolve(&componentDataTypes, &packageDataTypes);

        const auto componentPackageName =
                Escaper::escapeAsn1PackageName(sedsPackage.nameStr() + "-" + sedsComponent.nameStr());
        auto componentAsn1Definitions =
                std::make_unique<Asn1Acn::Definitions>(componentPackageName, Asn1Acn::SourceLocation());

        for (const auto &importedType : importedTypes) {
            componentAsn1Definitions->addImportedType(importedType);
        }

        translateDataTypes(resolvedComponentDataTypes, componentAsn1Definitions.get(), &sedsPackage, asn1Files,
                sedsPackages, sequenceSizeThreshold);

        auto componentAsn1File = std::make_unique<Asn1Acn::File>(componentPackageName);
        componentAsn1File->add(std::move(componentAsn1Definitions));
        result.push_back(std::move(componentAsn1File));
    }

    return result;
}

void SedsToAsn1Translator::translateDataTypes(const std::list<const seds::model::DataType *> &sedsDataTypes,
        Asn1Acn::Definitions *asn1Definitions, const seds::model::Package *sedsPackage,
        const Asn1Model::Data &asn1Files, const std::vector<seds::model::Package> &sedsPackages,
        const std::optional<uint64_t> &sequenceSizeThreshold) const
{
    std::unique_ptr<Asn1Acn::Types::Type> asn1Type;
    DataTypeTranslatorVisitor dataTypeVisitor(
            asn1Type, asn1Definitions, sedsPackage, asn1Files, sedsPackages, sequenceSizeThreshold);

    for (const auto *sedsDataType : sedsDataTypes) {
        std::visit(dataTypeVisitor, *sedsDataType);

        const auto &asn1TypeIdentifier = asn1Type->identifier();
        auto asn1TypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
                asn1TypeIdentifier, asn1TypeIdentifier, Asn1Acn::SourceLocation(), std::move(asn1Type));

        DescriptionTranslator::translate(*sedsDataType, asn1TypeAssignment.get());

        asn1Definitions->addType(std::move(asn1TypeAssignment));
    }
}

std::vector<const seds::model::DataType *> SedsToAsn1Translator::collectDataTypes(
        const seds::model::Package &sedsPackage) const
{
    const auto extractPointer = [](const auto &dataType) { return &dataType; };

    std::vector<const seds::model::DataType *> sedsDataTypes;

    std::transform(sedsPackage.dataTypes().begin(), sedsPackage.dataTypes().end(), std::back_inserter(sedsDataTypes),
            extractPointer);

    return sedsDataTypes;
}

std::vector<const seds::model::DataType *> SedsToAsn1Translator::collectDataTypes(
        const seds::model::Component &sedsComponent) const
{
    const auto extractPointer = [](const auto &dataType) { return &dataType; };

    std::vector<const seds::model::DataType *> sedsDataTypes;

    std::transform(sedsComponent.dataTypes().begin(), sedsComponent.dataTypes().end(),
            std::back_inserter(sedsDataTypes), extractPointer);

    return sedsDataTypes;
}

Asn1Acn::Definitions *SedsToAsn1Translator::getAsn1Definitions(
        const QString &definitionsName, const Asn1Model::Data &asn1Files)
{
    const auto asn1FileName = Escaper::escapeAsn1PackageName(definitionsName);

    auto asn1File = std::find_if(
            std::begin(asn1Files), std::end(asn1Files), [&](const auto &file) { return file->name() == asn1FileName; });
    if (asn1File == asn1Files.end()) {
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

const seds::model::Package *SedsToAsn1Translator::getSedsPackage(
        const QString &packageName, const std::vector<seds::model::Package> &sedsPackages)
{
    const auto sedsPackage = std::find_if(sedsPackages.begin(), sedsPackages.end(),
            [&](const auto &package) { return package.nameStr() == packageName; });
    if (sedsPackage == sedsPackages.end()) {
        auto message = QString("Unable to find package \"%1\"").arg(packageName);
        throw TranslationException(std::move(message));
    }

    return &(*sedsPackage);
}

} // namespace conversion::asn1::translator
