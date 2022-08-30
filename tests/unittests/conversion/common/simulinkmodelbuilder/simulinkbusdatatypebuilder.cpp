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

#include "simulinkbusdatatypebuilder.h"

using namespace simulink::model;

namespace tests::conversion::common {

SimulinkBusDataTypeBuilder::SimulinkBusDataTypeBuilder(
        QString name, DataScope dataScope, QString description, QString headerFile)
{
    m_busDataType.setName(name);
    m_busDataType.setDataScope(dataScope);
    m_busDataType.setDescription(std::move(description));
    m_busDataType.setHeaderFile(std::move(headerFile));
    m_busDataType.setAlignment("-1");
}

BusDataType SimulinkBusDataTypeBuilder::build()
{
    return std::move(m_busDataType);
}

SimulinkBusDataTypeBuilder &SimulinkBusDataTypeBuilder::withBusMember(
        QString name, QString dataType, QString description, Dimensions dimensions)
{
    BusMember busMember;
    busMember.setName(std::move(name));
    busMember.setDataType(std::move(dataType));
    busMember.setComplexity(Complexity::Real);
    busMember.setDescription(std::move(description));
    busMember.setDimensions(std::move(dimensions));
    busMember.setDimensionsMode(DimensionsMode::Fixed);
    busMember.setMax("");
    busMember.setMin("");
    busMember.setSampleTime("-1");
    busMember.setUnit("");

    m_busDataType.addBusMember(std::move(busMember));

    return *this;
}

} // namespace tests::conversion::common
