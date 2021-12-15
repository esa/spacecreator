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
#include "specialized/datatypetranslatorvisitor.h"

#include <asn1library/asn1/asn1model.h>
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
    Q_UNUSED(options);

    checkSourceModelCount(sourceModels);

    const auto *sedsModel = getModel<SedsModel>(sourceModels);

    return translateSedsModel(sedsModel);
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

std::vector<std::unique_ptr<Model>> SedsToAsn1Translator::translateSedsModel(const SedsModel *sedsModel) const
{
    std::vector<std::unique_ptr<Asn1Acn::File>> asn1Files;

    const auto &sedsModelData = sedsModel->data();
    if (std::holds_alternative<seds::model::PackageFile>(sedsModelData)) {
        const auto &sedsPackage = std::get<seds::model::PackageFile>(sedsModelData).package();
        auto packageFiles = translatePackage(sedsPackage);
        asn1Files.insert(asn1Files.end(), std::make_move_iterator(packageFiles.begin()),
                std::make_move_iterator(packageFiles.end()));
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &sedsPackages = std::get<seds::model::DataSheet>(sedsModelData).packages();
        for (const auto &sedsPackage : sedsPackages) {
            auto packageFiles = translatePackage(sedsPackage);
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
        const seds::model::Package &sedsPackage) const
{
    std::vector<std::unique_ptr<Asn1Acn::File>> result;
    std::vector<const seds::model::DataType *> packageDataTypes = collectDataTypes(sedsPackage);

    auto packageAsn1Definitions = std::make_unique<Asn1Acn::Definitions>(
            Escaper::escapeAsn1PackageName(sedsPackage.nameStr()), Asn1Acn::SourceLocation());
    translateDataTypes(packageDataTypes, packageAsn1Definitions.get());

    auto packageAsn1File = std::make_unique<Asn1Acn::File>(Escaper::escapeAsn1PackageName(sedsPackage.nameStr()));
    packageAsn1File->add(std::move(packageAsn1Definitions));
    result.push_back(std::move(packageAsn1File));

    for (const auto &sedsComponent : sedsPackage.components()) {
        if (sedsComponent.dataTypes().size() == 0) {
            // Don't generate ASN.1 package (and therefore a file) for a component without data type declarations
            continue;
        }
        std::vector<const seds::model::DataType *> componentDataTypes = collectDataTypes(sedsComponent);

        const auto componentPackageName =
                Escaper::escapeAsn1PackageName(sedsPackage.nameStr() + "-" + sedsComponent.nameStr());
        auto componentAsn1Definitions =
                std::make_unique<Asn1Acn::Definitions>(componentPackageName, Asn1Acn::SourceLocation());
        translateDataTypes(componentDataTypes, componentAsn1Definitions.get());

        auto componentAsn1File = std::make_unique<Asn1Acn::File>(componentPackageName);
        componentAsn1File->add(std::move(componentAsn1Definitions));
        result.push_back(std::move(componentAsn1File));
    }

    return result;
}

void SedsToAsn1Translator::translateDataTypes(
        const std::vector<const seds::model::DataType *> &sedsDataTypes, Asn1Acn::Definitions *asn1Definitions) const
{
    DataTypesDependencyResolver dependencyResolver;
    const auto resolvedSedsDataTypes = dependencyResolver.resolve(&sedsDataTypes);

    std::unique_ptr<Asn1Acn::Types::Type> asn1Type;
    DataTypeTranslatorVisitor dataTypeVisitor { asn1Definitions, asn1Type };

    for (const auto *sedsDataType : resolvedSedsDataTypes) {
        std::visit(dataTypeVisitor, *sedsDataType);

        const auto &asn1TypeIdentifier = asn1Type->identifier();
        auto asn1TypeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
                asn1TypeIdentifier, asn1TypeIdentifier, Asn1Acn::SourceLocation(), std::move(asn1Type));
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
        const seds::model::Package &sedsPackage, Asn1Model *asn1Model)
{
    const auto asn1FileName = Escaper::escapeAsn1PackageName(sedsPackage.nameStr());
    auto &asn1Files = asn1Model->data();
    auto asn1File = std::find_if(
            std::begin(asn1Files), std::end(asn1Files), [&](const auto &file) { return file->name() == asn1FileName; });
    if (asn1File == asn1Files.end()) {
        auto message = QString("Unable to find file %1 in the ASN.1 model").arg(asn1FileName);
        throw TranslationException(std::move(message));
    }

    const auto asn1DefinitionsName = Escaper::escapeAsn1PackageName(sedsPackage.nameStr());
    auto *asn1Definitions = (*asn1File)->definitions(asn1DefinitionsName);
    if (!asn1Definitions) {
        auto message =
                QString("ASN.1 file %1 doesn't have definitions named %2").arg(asn1FileName).arg(asn1DefinitionsName);
        throw TranslationException(std::move(message));
    }

    return asn1Definitions;
}

} // namespace conversion::asn1::translator
