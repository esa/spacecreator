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
#include <simulink/SimulinkModel/simulinkmodel.h>
#include <simulink/SimulinkOptions/options.h>
#include <simulink/SimulinkXmlImporter/exceptions.h>
#include <simulink/SimulinkXmlImporter/importer.h>
#include <unittests/common/verifyexception.h>

using conversion::simulink::SimulinkOptions;
using simulink::importer::ParserException;
using simulink::importer::SimulinkXmlImporter;
using simulink::model::Complexity;
using simulink::model::DataScope;
using simulink::model::Dimensions;
using simulink::model::DimensionsMode;
using simulink::model::PortDimension;
using simulink::model::SignalType;
using simulink::model::SimulinkModel;
using simulink::model::VectorDimensions;

namespace simulink::test {

class tst_SimulinkXmlImporter : public QObject
{
    Q_OBJECT

public:
    virtual ~tst_SimulinkXmlImporter() = default;

private Q_SLOTS:
    void testElementsAmountInModel();
    void testEnumDataTypesParsing();
    void testAliasDataTypesParsing();
    void testBusDataTypesParsing();
    void testInportsParsing();
    void testOutportsParsing();

    void testDimensionsEmpty();
    void testDimensionsVecEmpty();
    void testDimensionsZero();
    void testDimensionsMinusOne();
    void testDimensionsTwo();
    void testDimensionsIntMax();
    void testDimensionsIntMaxPlusOne();
    void testDimensionsFloat6p1();
    void testDimensionsVariable();
    void testDimensionsVec2();
    void testDimensionsVecIntMax();
    void testDimensionsVec23();
    void testDimensionsVec2Float6p1();
    void testDimensionsVec20();
    void testDimensionsVec23Spaces();
    void testDimensionsVec2sc2();
    void testDimensionsVec22sc22();
    void testDimensionsVecIntMaxIntMaxIntMax();
    void testDimensionsVec23456();
    void testDimensionsVec2Var();
    void testDimensionsVec2sc2sc2();
    void testDimensionsCell234();
};

void tst_SimulinkXmlImporter::testElementsAmountInModel()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "Complex.xml");
    SimulinkXmlImporter importer;

    const auto expectedDataTypesAmount = 63;
    const auto expectedInportsAmount = 78;
    const auto expectedOutportsAmount = 79;

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), expectedDataTypesAmount);

        const auto &inports = simulinkModel->inports();
        QCOMPARE(inports.size(), expectedInportsAmount);

        const auto &outports = simulinkModel->outports();
        QCOMPARE(outports.size(), expectedOutportsAmount);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testEnumDataTypesParsing()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "Complex.xml");
    SimulinkXmlImporter importer;

    const auto enum43thInDataTypesIndex = 42;
    const auto expected43thEnumValuesAmount = 2;

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();

        const auto &enumType = std::get<model::EnumDataType>(dataTypes.at(enum43thInDataTypesIndex));
        QCOMPARE(enumType.name(), "Int16InMemoryEnumDataDictionary");
        QCOMPARE(enumType.headerFile(), "");
        QCOMPARE(enumType.description(), "");
        QCOMPARE(enumType.addClassNameToEnumNames(), false);
        QCOMPARE(enumType.dataScope(), DataScope::Auto);
        QCOMPARE(enumType.defaultValue(), "ok");
        QCOMPARE(enumType.enumValues().size(), expected43thEnumValuesAmount);

        const auto &firstEnumValue = enumType.enumValues().front();
        QCOMPARE(firstEnumValue.name(), "ok");
        QCOMPARE(firstEnumValue.value(), 1);
        QCOMPARE(firstEnumValue.description(), "");
        QCOMPARE(firstEnumValue.detailedDescription(), "");

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testAliasDataTypesParsing()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "Complex.xml");
    SimulinkXmlImporter importer;

    const auto alias1stInDataTypesIndex = 0;

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();

        const auto &aliasType = std::get<model::AliasDataType>(dataTypes.at(alias1stInDataTypesIndex));
        QCOMPARE(aliasType.name(), "AliasToBooleanAliasBaseWorkspace");
        QCOMPARE(aliasType.baseType(), "BooleanAliasBaseWorkspace");
        QCOMPARE(aliasType.dataScope(), DataScope::Auto);
        QCOMPARE(aliasType.description(), "");
        QCOMPARE(aliasType.headerFile(), "");

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testBusDataTypesParsing()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "Complex.xml");
    SimulinkXmlImporter importer;

    const auto bus11thInDataTypesIndex = 10;
    const auto expected11thBusMembersAmount = 33;

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();

        const auto &busType = std::get<model::BusDataType>(dataTypes.at(bus11thInDataTypesIndex));
        QCOMPARE(busType.name(), "Bus1BaseWorkspace");
        QCOMPARE(busType.dataScope(), DataScope::Auto);
        QCOMPARE(busType.description(), "");
        QCOMPARE(busType.headerFile(), "");
        QCOMPARE(busType.alignment(), "-1");
        QCOMPARE(busType.busMembers().size(), expected11thBusMembersAmount);

        const auto &firstBusMember = busType.busMembers().front();
        const auto *dimensions = std::get_if<int64_t>(&firstBusMember.dimensions());

        QCOMPARE(firstBusMember.name(), "boolSimulinkIntEnumTypeEnumFile");
        QCOMPARE(firstBusMember.dataType(), "BoolSimulinkIntEnumType");
        QCOMPARE(firstBusMember.complexity(), Complexity::Real);
        QCOMPARE(firstBusMember.description(), "");
        QCOMPARE(*dimensions, 1);
        QCOMPARE(firstBusMember.dimensionsMode(), DimensionsMode::Fixed);
        QCOMPARE(firstBusMember.max(), "");
        QCOMPARE(firstBusMember.min(), "");
        QCOMPARE(firstBusMember.sampleTime(), "-1");
        QCOMPARE(firstBusMember.unit(), "");

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testInportsParsing()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "Complex.xml");
    SimulinkXmlImporter importer;

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &inports = simulinkModel->inports();

        const auto &firstInport = inports.front();
        const auto *portDimension = std::get_if<int64_t>(&firstInport.portDimensions());

        QCOMPARE(firstInport.name(), "doubleIn");
        QCOMPARE(firstInport.outDataTypeStr(), "double");
        QCOMPARE(firstInport.busObject(), "BusObject");
        QCOMPARE(firstInport.busOutputAsStruct(), "off");
        QCOMPARE(firstInport.iconDisplay(), "Port number");
        QCOMPARE(firstInport.interpolate(), "on");
        QCOMPARE(firstInport.latchByDelayingOutsideSignal(), "off");
        QCOMPARE(firstInport.latchInputForFeedbackSignals(), "off");
        QCOMPARE(firstInport.lockScale(), "off");
        QCOMPARE(firstInport.outMax(), "[]");
        QCOMPARE(firstInport.outMin(), "[]");
        QCOMPARE(firstInport.outputSignalNames(), "doubleInSignal");
        QCOMPARE(firstInport.port(), "1");
        QCOMPARE(*portDimension, -1);
        QCOMPARE(firstInport.sampleTime(), "-1");
        QCOMPARE(firstInport.signalType(), SignalType::Auto);
        QCOMPARE(firstInport.unit(), "inherit");
        QCOMPARE(firstInport.unitNoProp(), "");
        QCOMPARE(firstInport.useBusObject(), "off");

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testOutportsParsing()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "Complex.xml");
    SimulinkXmlImporter importer;

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &outports = simulinkModel->outports();

        const auto &firstOutport = outports.front();
        const auto *portDimension = std::get_if<int64_t>(&firstOutport.portDimensions());

        QCOMPARE(firstOutport.name(), "doubleOut");
        QCOMPARE(firstOutport.outDataTypeStr(), "double");
        QCOMPARE(firstOutport.busObject(), "BusObject");
        QCOMPARE(firstOutport.busOutputAsStruct(), "off");
        QCOMPARE(firstOutport.iconDisplay(), "Port number");
        QCOMPARE(firstOutport.initialOutput(), "[]");
        QCOMPARE(firstOutport.inputSignalNames(), "doubleOutSignal");
        QCOMPARE(firstOutport.lockScale(), "off");
        QCOMPARE(firstOutport.mustResolveToSignalObject(), "off");
        QCOMPARE(firstOutport.outMax(), "[]");
        QCOMPARE(firstOutport.outMin(), "[]");
        QCOMPARE(firstOutport.outputWhenDisabled(), "held");
        QCOMPARE(firstOutport.port(), "1");
        QCOMPARE(*portDimension, -1);
        QCOMPARE(firstOutport.sampleTime(), "-1");
        QCOMPARE(firstOutport.signalName(), "");
        QCOMPARE(firstOutport.signalObject(), "");
        QCOMPARE(firstOutport.signalType(), SignalType::Auto);
        QCOMPARE(firstOutport.storageClass(), "Auto");
        QCOMPARE(firstOutport.unit(), "inherit");
        QCOMPARE(firstOutport.unitNoProp(), "");
        QCOMPARE(firstOutport.useBusObject(), "off");
        QCOMPARE(firstOutport.varSizeSig(), "Inherit");

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsEmpty()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsEmpty.xml");
    SimulinkXmlImporter importer;

    // Dimensions=""

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(importer.importModel(options), ParserException, "Dimensions is empty");
}

void tst_SimulinkXmlImporter::testDimensionsVecEmpty()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVecEmpty.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[  ]"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(importer.importModel(options), ParserException, "Dimensions Array is empty");
}

void tst_SimulinkXmlImporter::testDimensionsZero()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsZero.xml");
    SimulinkXmlImporter importer;

    // Dimensions="0"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            importer.importModel(options), ParserException, "Value of dimension should be -1 or be greater than 0 '0'");
}

void tst_SimulinkXmlImporter::testDimensionsMinusOne()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsMinusOne.xml");
    SimulinkXmlImporter importer;

    // Dimensions="-1"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValue = std::get_if<int64_t>(&dimensions);
        QCOMPARE(*dimensionsValue, -1);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsTwo()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsTwo.xml");
    SimulinkXmlImporter importer;

    // Dimensions="2"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValue = std::get_if<int64_t>(&dimensions);
        QCOMPARE(*dimensionsValue, 2);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsIntMax()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsIntMax.xml");
    SimulinkXmlImporter importer;

    // Dimensions="2147483647"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValue = std::get_if<int64_t>(&dimensions);
        QCOMPARE(*dimensionsValue, 2147483647);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsIntMaxPlusOne()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsIntMaxPlusOne.xml");
    SimulinkXmlImporter importer;

    // Dimensions="2147483648"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValue = std::get_if<int64_t>(&dimensions);
        QCOMPARE(*dimensionsValue, 2147483648);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsFloat6p1()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsFloat6p1.xml");
    SimulinkXmlImporter importer;

    // Dimensions="6.1"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            importer.importModel(options), ParserException, "Unable to parse dimension number '6.1'");
}

void tst_SimulinkXmlImporter::testDimensionsVariable()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVariable.xml");
    SimulinkXmlImporter importer;

    // Dimensions="Variable"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            importer.importModel(options), ParserException, "Importer doesn't handle variables 'Variable'");
}

void tst_SimulinkXmlImporter::testDimensionsVec2()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec2.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2]"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValues = std::get_if<VectorDimensions>(&dimensions);
        QCOMPARE(dimensionsValues->values().size(), 1);
        QCOMPARE(dimensionsValues->values().front(), 2);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsVecIntMax()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVecIntMax.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2147483647]"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValues = std::get_if<VectorDimensions>(&dimensions);
        QCOMPARE(dimensionsValues->values().size(), 1);
        QCOMPARE(dimensionsValues->values().front(), 2147483647);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsVec23()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec23.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2 3]"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValues = std::get_if<VectorDimensions>(&dimensions);
        QCOMPARE(dimensionsValues->values().size(), 2);
        QCOMPARE(dimensionsValues->values()[0], 2);
        QCOMPARE(dimensionsValues->values()[1], 3);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsVec2Float6p1()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec2Float6p1.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2 6.1]"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            importer.importModel(options), ParserException, "Unable to parse one of array value '6.1'");
}

void tst_SimulinkXmlImporter::testDimensionsVec20()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec20.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2 0]"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            importer.importModel(options), ParserException, "Value of dimension array should be greater than 0 '0'");
}

void tst_SimulinkXmlImporter::testDimensionsVec23Spaces()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec23Spaces.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[    2     3   ]"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValues = std::get_if<VectorDimensions>(&dimensions);
        QCOMPARE(dimensionsValues->values().size(), 2);
        QCOMPARE(dimensionsValues->values()[0], 2);
        QCOMPARE(dimensionsValues->values()[1], 3);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsVec2sc2()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec2sc2.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2; 2]"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValues = std::get_if<VectorDimensions>(&dimensions);
        QCOMPARE(dimensionsValues->values().size(), 2);
        QCOMPARE(dimensionsValues->values()[0], 2);
        QCOMPARE(dimensionsValues->values()[1], 2);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsVec22sc22()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec22sc22.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2 2; 2 2]"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(importer.importModel(options), ParserException,
            "Dimensions Array has '4' dimension, but it should be max 2-dimensional");
}

void tst_SimulinkXmlImporter::testDimensionsVecIntMaxIntMaxIntMax()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVecIntMaxIntMax.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2147483647 2147483647]"

    try {
        const auto model = importer.importModel(options);
        const auto simulinkModel = dynamic_cast<SimulinkModel *>(model.get());

        const auto &dataTypes = simulinkModel->dataTypes();
        QCOMPARE(dataTypes.size(), 1);

        const auto *busDataType = std::get_if<model::BusDataType>(&dataTypes.front());
        QCOMPARE(busDataType->busMembers().size(), 1);

        const auto &dimensions = busDataType->busMembers().front().dimensions();
        const auto *dimensionsValues = std::get_if<VectorDimensions>(&dimensions);
        QCOMPARE(dimensionsValues->values().size(), 2);
        QCOMPARE(dimensionsValues->values()[0], 2147483647);
        QCOMPARE(dimensionsValues->values()[1], 2147483647);

    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SimulinkXmlImporter::testDimensionsVec23456()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec23456.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2 3 4 5 6]"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(importer.importModel(options), ParserException,
            "Dimensions Array has '5' dimension, but it should be max 2-dimensional");
}

void tst_SimulinkXmlImporter::testDimensionsVec2Var()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec2Var.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2 Variable]"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            importer.importModel(options), ParserException, "Importer doesn't handle variables 'Variable'");
}

void tst_SimulinkXmlImporter::testDimensionsVec2sc2sc2()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsVec2sc2sc2.xml");
    SimulinkXmlImporter importer;

    // Dimensions="[2; 2; 2]"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(importer.importModel(options), ParserException,
            "Dimensions Array has '3' dimension, but it should be max 2-dimensional");
}

void tst_SimulinkXmlImporter::testDimensionsCell234()
{
    conversion::Options options;
    options.add(SimulinkOptions::inputFilepath, "DimensionsCell234.xml");
    SimulinkXmlImporter importer;

    // Dimensions="{2 3 4}"

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            importer.importModel(options), ParserException, "Importer doesn't handle cell array '{2 3 4}'");
}

} // namespace simulink::test

QTEST_MAIN(simulink::test::tst_SimulinkXmlImporter)

#include "tst_simulinkxmlimporter.moc"
