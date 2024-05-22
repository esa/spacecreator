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

#include <asn1library/asn1/asn1model.h>
#include <conversion/common/translation/exceptions.h>
#include <msccore/mscdocument.h>
#include <sdl/SdlModel/nextstate.h>

using Asn1Acn::Asn1Model;
using conversion::translator::TranslationException;
using ivm::IVModel;
using msc::MscModel;
using sdl::SdlModel;

namespace conversion::sdl::translator {

std::vector<std::unique_ptr<Model>> MscToSdlTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    checkSourceModelCount(sourceModels);

    const auto *mscModel = getModel<MscModel>(sourceModels);
    const auto *asn1Model = getModel<Asn1Model>(sourceModels);
    const auto *ivModel = getModel<IVModel>(sourceModels);

    if (asn1Model->data().empty()) {
        throw TranslationException("Missing observer.asn file in the MSC to SDL translation");
    } else if (asn1Model->data().size() > 1) {
        throw TranslationException("Only observer.asn file is allowed in the MSC to SDL translation");
    }

    const auto *asn1File = asn1Model->data().front().get();

    return translateMscModel(mscModel, asn1File, ivModel, options);
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
    static std::set<ModelType> dependencies { ModelType::Msc, ModelType::Asn1, ModelType::InterfaceView };
    return dependencies;
}

std::vector<std::unique_ptr<Model>> MscToSdlTranslator::translateMscModel(
        const MscModel *mscModel, const Asn1Acn::File *asn1File, const IVModel *ivModel, const Options &options) const
{
    auto sdlModel = std::make_unique<SdlModel>();

    DocumentTranslator documentTranslator(sdlModel.get(), asn1File, ivModel, options);
    for (const auto mscDocument : mscModel->allDocuments()) {
        documentTranslator.translateDocument(mscDocument);
    }

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(sdlModel));

    return result;
}

} // namespace conversion::sdl::translator
