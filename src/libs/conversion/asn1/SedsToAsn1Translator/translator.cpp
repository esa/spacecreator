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
#include "visitors/datatypetranslatorvisitor.h"

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
        asn1Files.push_back(translatePackage(sedsPackage));
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &sedsPackages = std::get<seds::model::DataSheet>(sedsModelData).packages();
        for (const auto &sedsPackage : sedsPackages) {
            asn1Files.push_back(translatePackage(sedsPackage));
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    auto asn1Model = std::make_unique<Asn1Model>(std::move(asn1Files));

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(asn1Model));

    return result;
}

std::unique_ptr<Asn1Acn::File> SedsToAsn1Translator::translatePackage(const seds::model::Package &sedsPackage) const
{
    std::vector<const seds::model::DataType *> sedsDataTypes = collectDataTypes(sedsPackage);

    auto asn1Definitions = std::make_unique<Asn1Acn::Definitions>(
            Escaper::escapeAsn1PackageName(sedsPackage.nameStr()), Asn1Acn::SourceLocation());
    translateDataTypes(sedsDataTypes, asn1Definitions.get());

    auto asn1File = std::make_unique<Asn1Acn::File>(Escaper::escapeAsn1PackageName(sedsPackage.nameStr()));
    asn1File->add(std::move(asn1Definitions));

    return asn1File;
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
    for (const auto &sedsComponent : sedsPackage.components()) {
        std::transform(sedsComponent.dataTypes().begin(), sedsComponent.dataTypes().end(),
                std::back_inserter(sedsDataTypes), extractPointer);
    }

    return sedsDataTypes;
}

} // namespace conversion::asn1::translator
