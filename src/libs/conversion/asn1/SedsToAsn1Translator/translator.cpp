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
#include <seds/SedsModel/sedsmodel.h>

using conversion::translator::IncorrectSourceModelException;
using seds::model::SedsModel;

namespace conversion::asn1 {

std::unique_ptr<Model> SedsToAsn1Translator::translateModels(
        std::vector<const Model *> sourceModels, const Options &options) const
{
    for (const auto *sourceModel : sourceModels) {
        const auto *sedsModel = dynamic_cast<const SedsModel *>(sourceModel);
        if (sedsModel == nullptr) {
            throw IncorrectSourceModelException(ModelType::Seds);
        }

        translateSedsModel(sedsModel);
    }

    return nullptr;
}

std::set<ModelType> SedsToAsn1Translator::getDependencies() const
{
    return std::set<ModelType> { ModelType::Seds, ModelType::Asn1 };
}

void SedsToAsn1Translator::translateSedsModel(const SedsModel *sedsModel) const {}

} // namespace conversion::asn1
