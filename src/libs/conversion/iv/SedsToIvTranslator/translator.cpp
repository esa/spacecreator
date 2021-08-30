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
#include <conversion/iv/IvOptions/options.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <seds/SedsModel/sedsmodel.h>

using conversion::iv::IvOptions;
using conversion::translator::IncorrectSourceModelException;
using conversion::translator::TranslationException;
using ivm::IVModel;
using seds::model::SedsModel;

namespace conversion::iv::translator {

std::unique_ptr<Model> SedsToIvTranslator::translateModels(
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

    const auto configFilename = options.value(IvOptions::configFilename);
    if (!configFilename) {
        throw TranslationException("InterfaceView configuration file wasn't specified");
    }

    ivm::IVPropertyTemplateConfig *config = ivm::IVPropertyTemplateConfig::instance();
    config->init(*configFilename);

    return translateSedsModel(sedsModel, config);
}

std::set<ModelType> SedsToIvTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::Seds };
}

std::unique_ptr<IVModel> SedsToIvTranslator::translateSedsModel(
        const SedsModel *sedsModel, ivm::IVPropertyTemplateConfig *config) const
{
    auto ivModel = std::make_unique<IVModel>(config);

    return ivModel;
}

} // namespace conversion::iv::translator
