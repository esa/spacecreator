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

#include "modeltype.h"

#include <QString>

namespace conversion {

/**
 * @brief   Trait for properties of models
 *
 * @tparam  ModelType   Model
 */
template<typename Model>
struct ModelProperties {
    /// @brief  Model type
    static const ModelType type = ModelType::Unspecified;
    /// @brief  Model name
    static const inline QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extensions
    static const inline QStringList extensions = ModelTypeProperties<type>::extensions;
};

} // namespace conversion
