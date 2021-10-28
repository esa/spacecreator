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

#include "visitors/asn1nodevisitor.h"

#include <asn1library/asn1/asn1model.h>
#include <conversion/common/translation/exceptions.h>
#include <tmc/PromelaModel/promelamodel.h>

using Asn1Acn::Asn1Model;
using conversion::translator::TranslationException;
using tmc::promela::model::PromelaModel;

namespace conversion::tmc::translator {
std::vector<std::unique_ptr<Model>> Asn1ToPromelaTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    Q_UNUSED(options);

    checkSourceModelCount(sourceModels);

    const auto *asn1Model = getModel<Asn1Model>(sourceModels);

    return translateAsn1Model(asn1Model);
}

ModelType Asn1ToPromelaTranslator::getSourceModelType() const
{
    return ModelType::Asn1;
}

ModelType Asn1ToPromelaTranslator::getTargetModelType() const
{
    return ModelType::Promela;
}

std::set<ModelType> Asn1ToPromelaTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::Asn1 };
}

std::vector<std::unique_ptr<Model>> Asn1ToPromelaTranslator::translateAsn1Model(const ::Asn1Acn::Asn1Model *model) const
{
    std::unique_ptr<PromelaModel> promelaModel = std::make_unique<PromelaModel>();
    for (const std::unique_ptr<Asn1Acn::File> &file : model->data()) {
        visitAsn1File(file.get(), *promelaModel);
    }

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(promelaModel));
    return result;
}

void Asn1ToPromelaTranslator::visitAsn1File(
        ::Asn1Acn::File *file, ::tmc::promela::model::PromelaModel &promelaModel) const
{
    Asn1NodeVisitor visitor(promelaModel);
    visitor.visit(*file);
}
}
