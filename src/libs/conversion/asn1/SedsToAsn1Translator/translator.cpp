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

#include "visitors/datatypetranslatorvisitor.h"

#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/sourcelocation.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/sedsmodel.h>

using Asn1Acn::Asn1Model;
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
        const auto &package = std::get<seds::model::PackageFile>(sedsModelData).package();
        asn1Files.push_back(translatePackage(package));
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &packages = std::get<seds::model::DataSheet>(sedsModelData).packages();
        for (const auto &package : packages) {
            asn1Files.push_back(translatePackage(package));
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    auto asn1Model = std::make_unique<Asn1Model>(std::move(asn1Files));

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(asn1Model));

    return result;
}

std::unique_ptr<Asn1Acn::File> SedsToAsn1Translator::translatePackage(const seds::model::Package &package) const
{
    auto asn1File = std::make_unique<Asn1Acn::File>(package.nameStr());

    auto definitions = std::make_unique<Asn1Acn::Definitions>(package.asn1NameStr(), Asn1Acn::SourceLocation());
    translateDataTypes(package.dataTypes(), definitions.get());

    asn1File->add(std::move(definitions));

    return asn1File;
}

void SedsToAsn1Translator::translateDataTypes(
        const std::vector<seds::model::DataType> &dataTypes, Asn1Acn::Definitions *definitions) const
{
    std::unique_ptr<Asn1Acn::Types::Type> type;

    for (const auto &dataType : dataTypes) {
        std::visit(DataTypeTranslatorVisitor { type }, dataType);

        auto typeAssignment = std::make_unique<Asn1Acn::TypeAssignment>(
                type->identifier(), type->identifier(), Asn1Acn::SourceLocation(), std::move(type));
        definitions->addType(std::move(typeAssignment));
    }
}

} // namespace conversion::asn1::translator
