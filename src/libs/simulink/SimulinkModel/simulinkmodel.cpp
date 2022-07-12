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

#include "simulinkmodel.h"

namespace simulink::model {

conversion::ModelType SimulinkModel::modelType() const
{
    return conversion::ModelProperties<SimulinkModel>::type;
}

const DataType *SimulinkModel::dataType(const QString &name) const
{
    auto result = std::find_if(m_dataTypes.begin(), m_dataTypes.end(),
            [&name](const auto &dataType) { return dataTypeNameStr(dataType) == name; });

    if (result != m_dataTypes.end()) {
        return &(*result);
    }

    return nullptr;
}

const SimulinkModel::DataTypes &SimulinkModel::dataTypes() const
{
    return m_dataTypes;
}

void SimulinkModel::addDataType(DataType dataType)
{
    m_dataTypes.push_back(std::move(dataType));
}

const SimulinkModel::Inports &SimulinkModel::inports() const
{
    return m_inports;
}

void SimulinkModel::addInport(Inport inport)
{
    m_inports.push_back(std::move(inport));
}

const SimulinkModel::Outports &SimulinkModel::outports() const
{
    return m_outports;
}

void SimulinkModel::addOutport(Outport outport)
{
    m_outports.push_back(std::move(outport));
}

} // namespace simulink::model
