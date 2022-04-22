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
    Q_UNUSED(options);

    std::vector<std::unique_ptr<Asn1Acn::File>> asn1Files;

    const auto &sedsModelData = sedsModel->data();

    if (std::holds_alternative<seds::model::PackageFile>(sedsModelData)) {
        const auto &sedsPackage = std::get<seds::model::PackageFile>(sedsModelData).package();

        translatePackage(&sedsPackage);
    } else if (std::holds_alternative<seds::model::DataSheet>(sedsModelData)) {
        const auto &sedsPackages = std::get<seds::model::DataSheet>(sedsModelData).packages();

        PackagesDependencyResolver packagesDependencyResolver;
        const auto resolvedSedsPackages = packagesDependencyResolver.resolve(&sedsPackages);

        for (const auto sedsPackage : resolvedSedsPackages) {
            translatePackage(sedsPackage);
        }
    } else {
        throw TranslationException("Unhandled SEDS model data type");
    }

    auto asn1Model = std::make_unique<Asn1Model>(std::move(asn1Files));

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(asn1Model));

    return result;
}

void SedsToAsn1Translator::translatePackage(const seds::model::Package *sedsPackage) const
{
    Q_UNUSED(sedsPackage);
}

} // namespace conversion::asn1::translator
