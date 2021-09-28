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

#include "translation/translator.h"

namespace conversion::translator {

void Translator::checkSourceModelCount(const std::vector<Model *> &models) const
{
    const auto requiredModelsCount = getDependencies().size();

    if (models.empty()) {
        auto message = QString("No models passed for translation for %1 to %2 translation")
                               .arg(modelTypeToString(getSourceModelType()))
                               .arg(modelTypeToString(getTargetModelType()));
        throw TranslationException(std::move(message));
    } else if (models.size() < requiredModelsCount) {
        auto message = QString("Not enough models passed for %1 to %2 translation")
                               .arg(modelTypeToString(getSourceModelType()))
                               .arg(modelTypeToString(getTargetModelType()));
        throw TranslationException(std::move(message));
    } else if (models.size() > requiredModelsCount) {
        auto message = QString("Too many models passed for %1 to %2 translation")
                               .arg(modelTypeToString(getSourceModelType()))
                               .arg(modelTypeToString(getTargetModelType()));
        throw TranslationException(std::move(message));
    }
}

} // namespace conversion::translator
