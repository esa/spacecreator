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

#include <QStringList>
#include <memory>
#include <simulink/SimulinkModel/simulinkmodel.h>

namespace tests::conversion::common {

class SimulinkModelBuilder final
{
public:
    SimulinkModelBuilder(QString name);
    std::unique_ptr<simulink::model::SimulinkModel> build();

public:
    auto withEnumDataType(QString name, simulink::model::DataScope dataScope, QString description, QString headerFile,
            QStringList elems) -> SimulinkModelBuilder &;
    auto withAliasDataType(QString name, QString baseType, simulink::model::DataScope dataScope, QString description,
            QString headerFile) -> SimulinkModelBuilder &;
    auto withBusDataType(simulink::model::BusDataType busDataType) -> SimulinkModelBuilder &;
    auto withInport(QString name, QString outputSignalNames, QString outDataTypeStr, QString port,
            simulink::model::PortDimension portDimension) -> SimulinkModelBuilder &;
    auto withOutport(QString name, QString inputSignalNames, QString outDataTypeStr, QString port,
            simulink::model::PortDimension portDimension) -> SimulinkModelBuilder &;

private:
    simulink::model::SimulinkModel m_simulinkModel;
};

} // namespace tests::conversion::common
