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

#include <QObject>
#include <QtTest>

#include <simulink/SimulinkXMLImporter/importer.h>
#include <simulink/SimulinkOptions/options.h>
#include <simulink/SimulinkModel/simulinkmodel.h>

using simulink::importer::SimulinkXmlImporter;
using conversion::simulink::SimulinkOptions;
using simulink::model::SimulinkModel;

namespace simulink::test {

class tst_SimulinkXmlImporter : public QObject
{
    Q_OBJECT

public:
    virtual ~tst_SimulinkXmlImporter() = default;

private Q_SLOTS:
    void testImporter();
};

void tst_SimulinkXmlImporter::testImporter()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "Complex.xml");
    SimulinkXmlImporter importer;

    const auto expectedDataTypesAmount = 63;
    const auto expected11thBusMembersAmount = 33;
    const auto expected43thEnumValuesAmount = 2;
    const auto expectedInportsAmount = 78;
    const auto expectedOutportsAmount = 79;

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->modelInterface().dataTypes();
        QCOMPARE(dataTypes.size(), expectedDataTypesAmount);

        const auto &aliasType = std::get<model::AliasDataType>(dataTypes.at(0));
        QCOMPARE(aliasType.name(), "AliasToBooleanAliasBaseWorkspace");
        QCOMPARE(aliasType.baseType(), "BooleanAliasBaseWorkspace");
        QCOMPARE(aliasType.dataScope(), "Auto");
        QCOMPARE(aliasType.description(), "");
        QCOMPARE(aliasType.headerFile(), "");

        const auto &busType = std::get<model::BusDataType>(dataTypes.at(10));
        QCOMPARE(busType.name(), "Bus1BaseWorkspace");
        QCOMPARE(busType.dataScope(), "Auto");
        QCOMPARE(busType.description(), "");
        QCOMPARE(busType.headerFile(), "");
        QCOMPARE(busType.alignment(), "-1");
        QCOMPARE(busType.busMembers().size(), expected11thBusMembersAmount);

        const auto &busMember = busType.busMembers().at(0);
        QCOMPARE(busMember.name(), "boolSimulinkIntEnumTypeEnumFile");
        QCOMPARE(busMember.dataType(), "BoolSimulinkIntEnumType");
        QCOMPARE(busMember.complexity(), "real");
        QCOMPARE(busMember.description(), "");
        QCOMPARE(busMember.dimensions(), "1");
        QCOMPARE(busMember.dimensionsMode(), "Fixed");
        QCOMPARE(busMember.max(), "");
        QCOMPARE(busMember.min(), "");
        QCOMPARE(busMember.sampleTime(), "-1");
        QCOMPARE(busMember.unit(), "");

        const auto &enumType = std::get<model::EnumDataType>(dataTypes.at(42));
        QCOMPARE(enumType.name(), "Int16InMemoryEnumDataDictionary");
        QCOMPARE(enumType.headerFile(), "");
        QCOMPARE(enumType.description(), "");
        QCOMPARE(enumType.addClassNameToEnumNames(), "false");
        QCOMPARE(enumType.dataScope(), "Auto");
        QCOMPARE(enumType.defaultValue(), "ok");
        QCOMPARE(enumType.enumValues().size(), expected43thEnumValuesAmount);

        const auto &enumValue = enumType.enumValues().at(0);
        QCOMPARE(enumValue.name(), "ok");
        QCOMPARE(enumValue.value(), "1");
        QCOMPARE(enumValue.description(), "");
        QCOMPARE(enumValue.detailedDescription(), "");

        const auto &inports = simulinkModel->modelInterface().inports();
        QCOMPARE(inports.size(), expectedInportsAmount);

        const auto &inport = inports.at(0);
        QCOMPARE(inport.name(), "doubleIn");
        QCOMPARE(inport.outDataTypeStr(), "double");
        QCOMPARE(inport.busObject(), "BusObject");
        QCOMPARE(inport.busOutputAsStruct(), "off");
        QCOMPARE(inport.iconDisplay(), "Port number");
        QCOMPARE(inport.interpolate(), "on");
        QCOMPARE(inport.latchByDelayingOutsideSignal(), "off");
        QCOMPARE(inport.latchInputForFeedbackSignals(), "off");
        QCOMPARE(inport.lockScale(), "off");
        QCOMPARE(inport.outMax(), "[]");
        QCOMPARE(inport.outMin(), "[]");
        QCOMPARE(inport.outputSignalNames(), "doubleInSignal");
        QCOMPARE(inport.port(), "1");
        QCOMPARE(inport.portDimensions(), "-1");
        QCOMPARE(inport.sampleTime(), "-1");
        QCOMPARE(inport.signalType(), "auto");
        QCOMPARE(inport.unit(), "inherit");
        QCOMPARE(inport.unitNoProp(), "");
        QCOMPARE(inport.useBusObject(), "off");

        const auto &outports = simulinkModel->modelInterface().outports();
        QCOMPARE(outports.size(), expectedOutportsAmount);

        const auto &outport = outports.at(0);
        QCOMPARE(outport.name(), "doubleOut");
        QCOMPARE(outport.outDataTypeStr(), "double");
        QCOMPARE(outport.busObject(), "BusObject");
        QCOMPARE(outport.busOutputAsStruct(), "off");
        QCOMPARE(outport.iconDisplay(), "Port number");
        QCOMPARE(outport.initialOutput(), "[]");
        QCOMPARE(outport.inputSignalNames(), "doubleOutSignal");
        QCOMPARE(outport.lockScale(), "off");
        QCOMPARE(outport.mustResolveToSignalObject(), "off");
        QCOMPARE(outport.outMax(), "[]");
        QCOMPARE(outport.outMin(), "[]");
        QCOMPARE(outport.outputWhenDisabled(), "held");
        QCOMPARE(outport.port(), "1");
        QCOMPARE(outport.portDimensions(), "-1");
        QCOMPARE(outport.sampleTime(), "-1");
        QCOMPARE(outport.signalName(), "");
        QCOMPARE(outport.signalObject(), "");
        QCOMPARE(outport.signalType(), "auto");
        QCOMPARE(outport.storageClass(), "Auto");
        QCOMPARE(outport.unit(), "inherit");
        QCOMPARE(outport.unitNoProp(), "");
        QCOMPARE(outport.useBusObject(), "off");
        QCOMPARE(outport.varSizeSig(), "Inherit");

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

} // namespace simulink::test

QTEST_MAIN(simulink::test::tst_SimulinkXmlImporter)

#include "tst_simulinkxmlimporter.moc"
