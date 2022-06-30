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

#include "datatypes/datatype.h"
#include "datatypes/enumdatatype.h"
#include "datatypes/aliasdatatype.h"
#include "datatypes/busdatatype.h"
#include "inport/inport.h"
#include "outport/outport.h"

namespace simulink::model {

class ModelInterface final
{
public:
    using DataTypes = std::vector<DataType>;
    using Inports = std::vector<Inport>;
    using Outports = std::vector<Outport>;

public:
    ModelInterface() = default;
    ModelInterface(ModelInterface &&) = default;
    ModelInterface &operator=(ModelInterface &&) = default;

public:
    auto dataType(const QString &name) const -> const DataType *;
    auto dataTypes() const -> const DataTypes &;
    auto addDataType(DataType dataType) -> void;

    auto inports() const -> const Inports &;
    auto addInport(Inport inport) -> void;

    auto outports() const -> const Outports &;
    auto addOutport(Outport outport) -> void;

private:
    DataTypes m_dataTypes;
    Inports m_inports;
    Outports m_outports;
};

} // namespace simulink::model
