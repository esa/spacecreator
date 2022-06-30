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

#include "modelinterface.h"

#include <conversion/common/model.h>

namespace simulink::model {

/**
 * @brief   Represents a SIMULINK data model
 */
class SimulinkModel final : public conversion::Model
{
public:
    /**
     * @brief   Constructor
     *
     * @param   modelInterface    Model interface
     */
    explicit SimulinkModel(ModelInterface modelInterface);
    /**
     * @brief   Deleted copy constructor/
     */
    SimulinkModel(const SimulinkModel &) = delete;
    /**
     * @brief   Default move constructor
     */
    SimulinkModel(SimulinkModel &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    SimulinkModel &operator=(const SimulinkModel &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    SimulinkModel &operator=(SimulinkModel &&) = default;

public:
    /**
     * @brief   Get this model type
     *
     * @return  Model type
     */
    virtual auto modelType() const -> conversion::ModelType override;

public:
    /**
     * @brief   Getter for model interface
     *
     * @returns Model interface
     */
    auto modelInterface() const -> const ModelInterface &;

private:
    /** @brief  Model interface */
    ModelInterface m_modelInterface;
};

} // namespace simulink::model

namespace conversion {

/**
 * @brief   Specialization for SIMULINK model
 */
template<>
struct ModelProperties<::simulink::model::SimulinkModel> {
    /// @brief  Model type
    static const ModelType type = ModelType::Simulink;
    /// @brief  Model name
    static inline const QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extension
    static inline const QStringList extensions = ModelTypeProperties<type>::extensions;
};

} // namespace conversion
