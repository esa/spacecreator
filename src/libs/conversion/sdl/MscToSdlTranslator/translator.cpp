/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "specialized/documenttranslator.h"

#include <conversion/common/translation/exceptions.h>
#include <msccore/mscdocument.h>

using conversion::translator::TranslationException;
using msc::MscModel;
using sdl::SdlModel;

namespace conversion::sdl::translator {

std::vector<std::unique_ptr<Model>> MscToSdlTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    Q_UNUSED(options);

    checkSourceModelCount(sourceModels);

    const auto *mscModel = getModel<MscModel>(sourceModels);

    return translateMscModel(mscModel);
}

ModelType MscToSdlTranslator::getSourceModelType() const
{
    return ModelType::Msc;
}

ModelType MscToSdlTranslator::getTargetModelType() const
{
    return ModelType::Sdl;
}

std::set<ModelType> MscToSdlTranslator::getDependencies() const
{
    static std::set<ModelType> dependencies { ModelType::Msc };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> MscToSdlTranslator::translateMscModel(const MscModel *mscModel) const
{
    DocumentTranslator documentTranslator;
    for (const auto mscDocument : mscModel->allDocuments()) {
        documentTranslator.translateDocument(mscDocument);
    }

    auto sdlModel = std::make_unique<SdlModel>();

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(sdlModel));

    return result;
}

} // namespace conversion::sdl::translator
