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

#include "specialized/functionstranslator.h"

#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <seds/SedsModel/sedsmodel.h>

using conversion::translator::TranslationException;
using ivm::IVModel;
using seds::model::SedsModel;

namespace conversion::seds::translator {

std::vector<std::unique_ptr<Model>> IvToSedsTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    const auto *ivModel = getModel<IVModel>(sourceModels);

    return translateIvModel(ivModel, options);
}

ModelType IvToSedsTranslator::getSourceModelType() const
{
    return ModelType::InterfaceView;
}

ModelType IvToSedsTranslator::getTargetModelType() const
{
    return ModelType::Seds;
}

std::set<ModelType> IvToSedsTranslator::getDependencies() const
{
    static std::set<ModelType> dependencies { ModelType::InterfaceView };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> IvToSedsTranslator::translateIvModel(
        const IVModel *ivModel, const Options &options) const
{
    Q_UNUSED(options);

    ::seds::model::Package sedsPackage;
    sedsPackage.setName("InterfaceView");

    auto ivFunctions = ivModel->allObjectsByType<ivm::IVFunction>();

    const auto comparator = [](auto lhs, auto rhs) { return lhs->title() < rhs->title(); };
    std::sort(std::begin(ivFunctions), std::end(ivFunctions), comparator);

    for (const auto ivFunction : ivFunctions) {
        FunctionsTranslator::translateFunction(ivFunction, sedsPackage);
    }

    ::seds::model::PackageFile sedsPackageFile;
    sedsPackageFile.setPackage(std::move(sedsPackage));

    auto sedsModel = std::make_unique<SedsModel>(std::move(sedsPackageFile));

    std::vector<std::unique_ptr<Model>> resultModels;
    resultModels.push_back(std::move(sedsModel));

    return resultModels;
}

} // namespace conversion::seds::translator
