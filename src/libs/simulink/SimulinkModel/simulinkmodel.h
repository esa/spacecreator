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

#include "datatypes/aliasdatatype.h"
#include "datatypes/busdatatype.h"
#include "datatypes/datatype.h"
#include "datatypes/enumdatatype.h"
#include "inport/inport.h"
#include "outport/outport.h"

#include <conversion/common/model.h>

namespace simulink::model {

/**
 * @brief   Represents a SIMULINK data model
 */
class SimulinkModel final : public conversion::Model
{
public:
    using DataTypes = std::vector<DataType>;
    using Inports = std::vector<Inport>;
    using Outports = std::vector<Outport>;

public:
    /**
     * @brief   Default constructor
     */
    SimulinkModel() = default;
    /**
     * @brief   Deleted copy constructor
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
     * @brief   Return model name
     *
     * @return  Reference to model name
     */
    auto name() const -> const common::String &;
    /**
     * @brief   Set model name
     *
     * @param   name     Model name
     */
    auto setName(const common::String name) -> void;

    /**
     * @brief   Return data type that has name equal to 'name' param
     *
     * @param   name     Data type name
     *
     * @return  Pointer to data type or nullptr if it does not exist
     */
    auto dataType(const QString &name) const -> const DataType *;
    /**
     * @brief   Return vector of all data types accessible in Simulink Model
     *
     * @return  Reference to vector containing all data type objects
     */
    auto dataTypes() const -> const DataTypes &;
    /**
     * @brief   Append data type to Simulink Model
     *
     * @param   dataType    Data type object
     */
    auto addDataType(DataType dataType) -> void;

    /**
     * @brief   Return vector of all inports accessible in Simulink Model
     *
     * @return  Reference to vector containing all inport objects
     */
    auto inports() const -> const Inports &;
    /**
     * @brief   Append inport to Simulink Model
     *
     * @param   inport    Inport object
     */
    auto addInport(Inport inport) -> void;

    /**
     * @brief   Return vector of all outports accessible in Simulink Model
     *
     * @return  Reference to vector containing all outport objects
     */
    auto outports() const -> const Outports &;
    /**
     * @brief   Append outport to Simulink Model
     *
     * @param   outport    outport object
     */
    auto addOutport(Outport outport) -> void;

private:
    /** @brief  Model name */
    common::String m_modelName;

    /** @brief  Data types using by model */
    DataTypes m_dataTypes;

    /** @brief  Model inports */
    Inports m_inports;

    /** @brief  Model outports */
    Outports m_outports;
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
