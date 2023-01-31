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

#include <conversion/common/import/modelimporter.h>
#include <functional>
#include <memory>
#include <simulink/SimulinkCommon/basetypesmappings.h>
#include <simulink/SimulinkModel/simulinkmodel.h>

class QXmlStreamReader;
class QXmlStreamAttribute;

namespace conversion {
class Model;
class Options;
} // namespace conversion

namespace simulink::model {
class Workspace;
class EnumDataType;
class EnumValue;
class AliasDataType;
class BusDataType;
class BusMember;
class Inport;
class Outport;
class NamedEntity;
} // namespace simulink::model

namespace simulink::importer {

/**
 * @brief   Importer that reads SIMULINK files
 */
class SimulinkXmlImporter final : public conversion::importer::ModelImporter
{
public:
    /**
     * @brief   Read given SIMULINK file and produce SIMULINK model
     *
     * @param   options  List of options
     *
     * @throws  conversion::importer::ImportException
     * @throws  msc::FileNotFoundException
     * @throws  simulink::importer::ParserException
     * @throws  simulink::importer::UnhandledElement
     * @throws  simulink::importer::UnhandledAttribute
     *
     * @return  Imported SIMULINK model
     */
    virtual auto importModel(const conversion::Options &options) const -> std::unique_ptr<conversion::Model> override;

private:
    static auto parse(const common::String &inputSimulinkFilename) -> std::unique_ptr<conversion::Model>;
    static auto readSimulinkModel(QXmlStreamReader &xmlReader, std::unique_ptr<model::SimulinkModel> &simulinkModel)
            -> void;
    static auto readDataTypes(QXmlStreamReader &xmlReader, std::unique_ptr<model::SimulinkModel> &simulinkModel)
            -> void;
    static auto readEnumDataType(QXmlStreamReader &xmlReader) -> model::EnumDataType;
    static auto readEnumValues(QXmlStreamReader &xmlReader, model::EnumDataType &enumDataType) -> void;
    static auto readEnumValue(QXmlStreamReader &xmlReader) -> model::EnumValue;
    static auto readAliasDataType(QXmlStreamReader &xmlReader) -> model::AliasDataType;
    static auto readBusDataType(QXmlStreamReader &xmlReader) -> model::BusDataType;
    static auto readBusMembers(QXmlStreamReader &xmlReader, model::BusDataType &busDataType) -> void;
    static auto readBusMember(QXmlStreamReader &xmlReader) -> model::BusMember;
    static auto readInports(QXmlStreamReader &xmlReader, std::unique_ptr<model::SimulinkModel> &simulinkModel) -> void;
    static auto readInport(QXmlStreamReader &xmlReader) -> model::Inport;
    static auto readOutports(QXmlStreamReader &xmlReader, std::unique_ptr<model::SimulinkModel> &simulinkModel) -> void;
    static auto readOutport(QXmlStreamReader &xmlReader) -> model::Outport;

    static auto parseBool(const QStringView &valueStr) -> bool;
    static auto parseInt64(const QStringView &valueStr) -> int64_t;
    static auto processForNamedEntity(model::NamedEntity &namedEntity, const QXmlStreamAttribute &attribute) -> bool;

    static auto parseDataScope(const QStringView &dataScopeStr) -> model::DataScope;
    static auto parseComplexity(const QStringView &complexityStr) -> model::Complexity;
    static auto parseDimensions(const QStringView &dimensionsStr) -> model::Dimensions;
    static auto parseDimensionsMode(const QStringView &dimensionsModeStr) -> model::DimensionsMode;
    static auto parsePortDimension(const QStringView &portDimensionStr) -> model::PortDimension;
    static auto parseSignalType(const QStringView &signalTypeStr) -> model::SignalType;
};

} // namespace seds::importer
