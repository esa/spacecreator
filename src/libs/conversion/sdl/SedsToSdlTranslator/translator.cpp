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

#include <conversion/common/translation/exceptions.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <seds/SedsModel/sedsmodel.h>

using conversion::translator::TranslationException;
using sdl::SdlModel;
using seds::model::SedsModel;

namespace conversion::sdl::translator {

std::vector<std::unique_ptr<Model>> SedsToSdlTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    Q_UNUSED(options);

    checkSourceModelCount(sourceModels);

    const auto *sedsModel = getModel<SedsModel>(sourceModels);

    return translateSedsModel(sedsModel);
}

ModelType SedsToSdlTranslator::getSourceModelType() const
{
    return ModelType::Seds;
}

ModelType SedsToSdlTranslator::getTargetModelType() const
{
    return ModelType::Sdl;
}

std::set<ModelType> SedsToSdlTranslator::getDependencies() const
{
    static std::set<ModelType> dependencies { ModelType::Seds, ModelType::Asn1, ModelType::InterfaceView };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> SedsToSdlTranslator::translateSedsModel(const SedsModel *sedsModel) const
{
    Q_UNUSED(sedsModel);
    auto sdlModel = std::make_unique<SdlModel>();

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(sdlModel));

    return result;
}

} // namespace conversion::sdl::translator
