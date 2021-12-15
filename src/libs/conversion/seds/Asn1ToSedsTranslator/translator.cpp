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

#include "specialized/typetranslator.h"

using Asn1Acn::Asn1Model;
using conversion::translator::TranslationException;
using seds::model::SedsModel;

namespace conversion::seds::translator {

std::vector<std::unique_ptr<Model>> Asn1ToSedsTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    auto *asn1Model = getModel<Asn1Model>(sourceModels);

    return translateAsn1Model(asn1Model, options);
}

ModelType Asn1ToSedsTranslator::getSourceModelType() const
{
    return ModelType::Asn1;
}

ModelType Asn1ToSedsTranslator::getTargetModelType() const
{
    return ModelType::Seds;
}

std::set<ModelType> Asn1ToSedsTranslator::getDependencies() const
{
    static std::set<ModelType> dependencies { ModelType::Asn1 };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> Asn1ToSedsTranslator::translateAsn1Model(
        const Asn1Model *asn1Model, const Options &options)
{
    Q_UNUSED(options);
    std::vector<std::unique_ptr<Model>> resultModels;

    for (const auto &file : asn1Model->data()) {
        for (const auto &definitions : file->definitionsList()) {
            if (definitions->types().size() == 0) {
                continue;
            }
            auto sedsPackage = translateAsn1Definitions(asn1Model, definitions.get());

            ::seds::model::PackageFile sedsPackageFile;
            sedsPackageFile.setPackage(std::move(sedsPackage));
            auto sedsModel = std::make_unique<SedsModel>(std::move(sedsPackageFile));
            resultModels.push_back(std::move(sedsModel));
        }
    }

    return resultModels;
}

auto Asn1ToSedsTranslator::translateAsn1Definitions(const Asn1Model *asn1Model, const Asn1Acn::Definitions *definitions)
        -> ::seds::model::Package
{
    ::seds::model::Package package;
    package.setName(definitions->name());
    for (const auto &type : definitions->types()) {
        TypeTranslator::translateType(asn1Model, definitions, type.get(), &package);
    }

    return package;
}

} // namespace conversion::seds::translator
