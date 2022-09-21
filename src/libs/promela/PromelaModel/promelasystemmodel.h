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

#pragma once

#include "promelamodel.h"

namespace promela::model {

/**
 * @brief   Representation of Promela system model.
 */
class PromelaSystemModel final : public PromelaModel
{
public:
    /// @brief Default constructor.
    PromelaSystemModel() = default;

    /// @brief Deleted copy constructor.
    PromelaSystemModel(const PromelaSystemModel &) = delete;
    /// @brief Default move constructor.
    PromelaSystemModel(PromelaSystemModel &&) = default;

    /// @brief Deleted copy assignment operator.
    PromelaSystemModel &operator=(const PromelaSystemModel &) = delete;
    /// @brief Default move assignment operator.
    PromelaSystemModel &operator=(PromelaSystemModel &&) = default;

public:
    /**
     * @brief   Get this model type
     *
     * @return  Model type
     */
    auto modelType() const -> conversion::ModelType override;
};

} // namespace promela::model

namespace conversion {

/**
 * @brief   Specialization for Promela system model
 */
template<>
struct ModelProperties<::promela::model::PromelaSystemModel> {
    /// @brief  Model type
    static const ModelType type = ModelType::PromelaSystem;
    /// @brief  Model name
    static inline const QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extension
    static inline const QStringList extensions = ModelTypeProperties<type>::extensions;
};

} // namespace conversion
