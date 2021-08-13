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

#pragma once

class QString;

#include <set>

namespace conversion {

/**
 * @brief   All model types supported in conversion
 */
enum class ModelType
{
    Asn1,
    Aadl,
    Sdl,
    Seds
};

/**
 * @brief   Converts given model type to string
 *
 * @param   modelType   Model type to convert
 *
 * @param   String with model type name
 */
auto modelTypeToString(ModelType modelType) -> QString;
/**
 * @brief   Converts given set of model types to string
 *
 * @param   sourceModelsTypes       Set of model types
 *
 * @return  String with model types names separated with comma
 */
auto modelTypesToString(const std::set<ModelType> &modelsTypes) -> QString;

} // namespace conversion
