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
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/type.h>
#include <asn1modelbuilder/asn1modelbuilder.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/SimulinkToIvTranslator/options.h>
#include <conversion/iv/SimulinkToIvTranslator/translator.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <libiveditor/iveditor.h>
#include <simulinkmodelbuilder/simulinkmodelbuilder.h>
#include <unittests/common/verifyexception.h>

using Asn1Acn::Types::Endianness;
using Asn1Acn::Types::IntegerEncoding;
using Asn1Acn::Types::RealEncoding;
using conversion::ModelType;
using conversion::iv::translator::SimulinkToIvTranslator;
using conversion::translator::TranslationException;
using ivm::IVModel;
using simulink::model::PortDimension;
using simulink::model::VectorDimensions;
using tests::conversion::common::Asn1ModelBuilder;
using tests::conversion::common::SimulinkModelBuilder;

namespace conversion::iv::test {

class MockModel final : public conversion::Model
{
    virtual auto modelType() const -> ModelType override { return ModelType::Unspecified; }
};

class tst_SimulinkToIvTranslator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testMissingModel();
    void testTooManyModels();
    void testWrongModel();

    void testTranslatePortsWithScalarDataTypes();
    void testTranslatePortsWithMultiDimensionalDataTypes();

private:
    static const std::array<const QString, 24> m_directoriesOfTests;

    static const QString m_interfaceXmlFilePath;
    static const QString m_outputIVXmlFilePath;
};

void tst_SimulinkToIvTranslator::initTestCase()
{
    ivm::initIVLibrary();
    ive::initIVEditor();
}

void tst_SimulinkToIvTranslator::testMissingModel()
{
    Options options;
    SimulinkToIvTranslator translator;

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({}, options), TranslationException,
            "No models passed for translation for SIMULINK to InterfaceView translation");
}

void tst_SimulinkToIvTranslator::testTooManyModels()
{
    Options options;
    SimulinkToIvTranslator translator;

    const auto simulinkModel1 = SimulinkModelBuilder("SimulinkModel").build();
    const auto simulinkModel2 = SimulinkModelBuilder("SimulinkModel").build();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            translator.translateModels({ simulinkModel1.get(), simulinkModel2.get() }, options), TranslationException,
            "Too many models passed for SIMULINK to InterfaceView translation");
}

void tst_SimulinkToIvTranslator::testWrongModel()
{
    Options options;
    options.add(IvOptions::configFilepath, "config.xml");
    SimulinkToIvTranslator translator;

    const auto mockModel = std::make_unique<MockModel>();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({ mockModel.get() }, options), TranslationException,
            "Missing source SIMULINK model");
}

void tst_SimulinkToIvTranslator::testTranslatePortsWithScalarDataTypes()
{
    PortDimension inportVecDimensions(std::in_place_type<VectorDimensions>);
    auto &inportVecDimensions1x1 = std::get<VectorDimensions>(inportVecDimensions);
    inportVecDimensions1x1.addValue(1);

    PortDimension outportVecDimensions(std::in_place_type<VectorDimensions>);
    auto &outportVecDimensions1x1 = std::get<VectorDimensions>(outportVecDimensions);
    outportVecDimensions1x1.addValue(1);

    // clang-format off
    const auto simulinkModel = SimulinkModelBuilder("SimulinkInterface")
                                .withInport("Inport1", "", "double", "1", PortDimension(-1))
                                .withInport("Inport2", "", "int8", "2", PortDimension(1))
                                .withInport("Inport3", "", "Alias", "3", PortDimension(1))
                                .withInport("Inport4", "", "Alias", "4", std::move(inportVecDimensions))

                                .withOutport("Outport1", "", "double", "1", PortDimension(-1))
                                .withOutport("Outport2", "", "int8", "2", PortDimension(1))
                                .withOutport("Outport3", "", "Alias", "3", PortDimension(1))
                                .withOutport("Outport4", "", "Alias", "4", std::move(outportVecDimensions))
                                .build();
    // clang-format on

    Options options;
    options.add(IvOptions::configFilepath, "config.xml");
    options.add(SimulinkToIvOptions::ivFunctionName, "simulink");

    SimulinkToIvTranslator translator;

    const auto resultModels = translator.translateModels({ simulinkModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::InterfaceView);

    const auto *ivModel = dynamic_cast<IVModel *>(resultModel.get());
    QVERIFY(ivModel);

    const auto *function = ivModel->getFunction("simulink", Qt::CaseSensitive);
    QVERIFY(function);

    const auto interfaces = function->interfaces();
    QCOMPARE(interfaces.size(), 1);

    const auto *interface = interfaces[0];
    QVERIFY(interface);
    QVERIFY(interface->isProvided());
    QCOMPARE(interface->title(), "SimulinkInterface");

    const auto params = interface->params();
    QCOMPARE(params.size(), 8);

    const auto param = params[0];
    QCOMPARE(param.name(), "Inport1");
    QCOMPARE(param.paramTypeName(), "MatLab-Double");
    QCOMPARE(param.direction(), shared::InterfaceParameter::Direction::IN);
}

void tst_SimulinkToIvTranslator::testTranslatePortsWithMultiDimensionalDataTypes()
{
    PortDimension inport3VecDimensions(std::in_place_type<VectorDimensions>);
    auto &inport3VecDimensions1x2 = std::get<VectorDimensions>(inport3VecDimensions);
    inport3VecDimensions1x2.addValue(2);

    PortDimension inport4VecDimensions(std::in_place_type<VectorDimensions>);
    auto &inport4VecDimensions2x2 = std::get<VectorDimensions>(inport4VecDimensions);
    inport4VecDimensions2x2.addValue(2);
    inport4VecDimensions2x2.addValue(2);

    PortDimension outport3VecDimensions(std::in_place_type<VectorDimensions>);
    auto &outport3VecDimensions1x2 = std::get<VectorDimensions>(outport3VecDimensions);
    outport3VecDimensions1x2.addValue(2);

    PortDimension outport4VecDimensions(std::in_place_type<VectorDimensions>);
    auto &outport4VecDimensions2x2 = std::get<VectorDimensions>(outport4VecDimensions);
    outport4VecDimensions2x2.addValue(2);
    outport4VecDimensions2x2.addValue(2);

    // clang-format off
    const auto simulinkModel = SimulinkModelBuilder("SimulinkInterface")
                                .withInport("Inport1", "", "double", "1", PortDimension(2))
                                .withInport("Inport2", "", "int8", "2", PortDimension(2))
                                .withInport("Inport3", "", "Alias", "3", std::move(inport3VecDimensions))
                                .withInport("Inport4", "", "Alias", "4", std::move(inport4VecDimensions))

                                .withOutport("Outport1", "", "double", "1", PortDimension(2))
                                .withOutport("Outport2", "", "int8", "2", PortDimension(2))
                                .withOutport("Outport3", "", "Alias", "3", std::move(outport3VecDimensions1x2))
                                .withOutport("Outport4", "", "Alias", "4", std::move(outport4VecDimensions))
                                .build();
    // clang-format on

    Options options;
    options.add(IvOptions::configFilepath, "config.xml");
    options.add(SimulinkToIvOptions::ivFunctionName, "simulink");

    SimulinkToIvTranslator translator;

    const auto resultModels = translator.translateModels({ simulinkModel.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::InterfaceView);

    const auto *ivModel = dynamic_cast<IVModel *>(resultModel.get());
    QVERIFY(ivModel);

    const auto *function = ivModel->getFunction("simulink", Qt::CaseSensitive);
    QVERIFY(function);

    const auto interfaces = function->interfaces();
    QCOMPARE(interfaces.size(), 1);

    const auto *interface = interfaces[0];
    QVERIFY(interface);
    QVERIFY(interface->isProvided());
    QCOMPARE(interface->title(), "SimulinkInterface");

    const auto params = interface->params();
    QCOMPARE(params.size(), 8);

    const auto param = params[0];
    QCOMPARE(param.name(), "Inport1");
    QCOMPARE(param.paramTypeName(), "Inport1-Type");
    QCOMPARE(param.direction(), shared::InterfaceParameter::Direction::IN);
}

} // namespace conversion::iv::test

QTEST_MAIN(conversion::iv::test::tst_SimulinkToIvTranslator)

#include "tst_simulinktoivtranslator.moc"
