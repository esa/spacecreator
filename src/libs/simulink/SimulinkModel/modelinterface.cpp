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

#include "modelinterface.h"

namespace simulink::model {

const DataType *ModelInterface::dataType(const QString &name) const
{
    auto result = std::find_if(m_dataTypes.begin(), m_dataTypes.end(),
            [&name](const auto &dataType) { return dataTypeNameStr(dataType) == name; });

    if (result != m_dataTypes.end()) {
        return &(*result);
    }

    return nullptr;
}

const ModelInterface::DataTypes &ModelInterface::dataTypes() const
{
    return m_dataTypes;
}

void ModelInterface::addDataType(DataType dataType)
{
    m_dataTypes.push_back(std::move(dataType));
}

const ModelInterface::Inports &ModelInterface::inports() const
{
    return m_inports;
}

void ModelInterface::addInport(Inport inport)
{
    m_inports.push_back(std::move(inport));
}

const ModelInterface::Outports &ModelInterface::outports() const
{
    return m_outports;
}

void ModelInterface::addOutport(Outport outport)
{
    m_outports.push_back(std::move(outport));
}

} // namespace simulink::model
