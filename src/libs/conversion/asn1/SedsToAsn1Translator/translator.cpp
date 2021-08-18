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

#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/file.h>
#include <conversion/common/translation/exceptions.h>
#include <seds/SedsModel/sedsmodel.h>

using Asn1Acn::Asn1Model;
using conversion::translator::IncorrectSourceModelException;
using conversion::translator::TranslationException;
using seds::model::SedsModel;

namespace conversion::asn1 {

std::unique_ptr<Model> SedsToAsn1Translator::translateModels(
        std::vector<const Model *> sourceModels, const Options &options) const
{
    if (sourceModels.empty()) {
        throw TranslationException("No models passed for translation");
    } else if (sourceModels.size() > 1) {
        throw TranslationException("Only one model is allowed for translation");
    }

    const auto *sourceModel = sourceModels[0];
    if (sourceModel == nullptr) {
        throw TranslationException("Source model is null");
    }

    const auto *sedsModel = dynamic_cast<const SedsModel *>(sourceModel);
    if (sedsModel == nullptr) {
        throw IncorrectSourceModelException(ModelType::Seds);
    }

    translateSedsModel(sedsModel);

    return nullptr;
}

std::set<ModelType> SedsToAsn1Translator::getDependencies() const
{
    return std::set<ModelType> { ModelType::Seds, ModelType::Asn1 };
}

Asn1Model SedsToAsn1Translator::translateSedsModel(const SedsModel *sedsModel) const
{
    std::vector<Asn1Acn::File> asn1Files;

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
        throw TranslationException("Unhandled SEDS model data");
    }

    return Asn1Model(std::move(asn1Files));
}

Asn1Acn::File SedsToAsn1Translator::translatePackage(const seds::model::Package &package) const
{
    Asn1Acn::File asn1File(package.qualifiedName().name().value());

    return asn1File;
}

} // namespace conversion::asn1
