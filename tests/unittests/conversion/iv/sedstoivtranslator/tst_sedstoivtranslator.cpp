/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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
#include <asn1modelbuilder/asn1modelbuilder.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/SedsToIvTranslator/translator.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <sedsmodelbuilder/sedscomponentbuilder.h>
#include <sedsmodelbuilder/sedsinterfacebuilder.h>
#include <sedsmodelbuilder/sedsinterfacecommandbuilder.h>
#include <sedsmodelbuilder/sedsinterfacedeclarationbuilder.h>
#include <sedsmodelbuilder/sedsmodelbuilder.h>
#include <unittests/common/verifyexception.h>

using namespace ivm;
using namespace seds::model;

using conversion::Options;
using conversion::iv::IvOptions;
using conversion::iv::translator::SedsToIvTranslator;
using conversion::translator::TranslationException;

using tests::conversion::common::Asn1ModelBuilder;
using tests::conversion::common::SedsComponentBuilder;
using tests::conversion::common::SedsInterfaceBuilder;
using tests::conversion::common::SedsInterfaceCommandBuilder;
using tests::conversion::common::SedsInterfaceDeclarationBuilder;
using tests::conversion::common::SedsModelBuilder;

namespace conversion::iv::test {

class MockModel final : public conversion::Model
{
    virtual auto modelType() const -> conversion::ModelType override { return conversion::ModelType::Unspecified; }
};

class tst_SedsToIvTranslator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testMissingModel();
    void testNotEnoughModels();
    void testTooManyModels();
    void testWrongModel();

    void testTranslateComponentWithProvidedInterface();
    void testTranslateComponentWithRequiredInterface();
};

void tst_SedsToIvTranslator::testMissingModel()
{
    Options options;
    SedsToIvTranslator translator;

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({}, options), TranslationException,
            "No models passed for translation for SEDS to InterfaceView translation");
}

void tst_SedsToIvTranslator::testNotEnoughModels()
{
    Options options;
    SedsToIvTranslator translator;

    const auto sedsModel = SedsModelBuilder("Package").build();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({ sedsModel.get() }, options), TranslationException,
            "Not enough models passed for SEDS to InterfaceView translation");
}

void tst_SedsToIvTranslator::testTooManyModels()
{
    Options options;
    SedsToIvTranslator translator;

    const auto sedsModel1 = SedsModelBuilder("Package").build();
    const auto sedsModel2 = SedsModelBuilder("Package").build();
    const auto sedsModel3 = SedsModelBuilder("Package").build();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            translator.translateModels({ sedsModel1.get(), sedsModel2.get(), sedsModel3.get() }, options),
            TranslationException, "Too many models passed for SEDS to InterfaceView translation");
}

void tst_SedsToIvTranslator::testWrongModel()
{
    Options options;
    SedsToIvTranslator translator;

    const auto sedsModel = SedsModelBuilder("Package").build();
    const auto mockModel = std::make_unique<MockModel>();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({ sedsModel.get(), mockModel.get() }, options),
            TranslationException, "Missing source ASN.1 model");
}

void tst_SedsToIvTranslator::testTranslateComponentWithProvidedInterface()
{
    // clang-format off
    const auto sedsModel =
        SedsModelBuilder("Package")
            .withIntegerDataType("MyInteger")
            .withComponent(
                SedsComponentBuilder("Component")
                    .declaringInterface(
                        SedsInterfaceDeclarationBuilder("RequiredInterface")
                            .withCommand(
                                SedsInterfaceCommandBuilder("ICommand", InterfaceCommandMode::Async)
                                    .withArgument("CmdArg", "GenericType", CommandArgumentMode::In)
                                    .build())
                        .build())
                    .withProvidedInterface(
                        SedsInterfaceBuilder("Interface", "RequiredInterface")
                            .withMappings({{"GenericType", "MyInteger"}})
                            .build())
                    .build())
            .build();
    // clang-format on

    // clang-format off
    const auto asn1Model =
        Asn1ModelBuilder("PACKAGE")
            .withIntegerDataType("MyInteger")
        .build();
    // clang-format on

    Options options;
    options.add(IvOptions::configFilepath, "config.xml");

    SedsToIvTranslator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get(), asn1Model.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::InterfaceView);

    const auto *ivModel = dynamic_cast<IVModel *>(resultModel.get());
    QVERIFY(ivModel);

    const auto *function = ivModel->getFunction("Component", Qt::CaseSensitive);
    QVERIFY(function);

    const auto interfaces = function->interfaces();
    QCOMPARE(interfaces.size(), 1);

    const auto *interface = interfaces[0];
    QVERIFY(interface);
    QVERIFY(interface->isProvided());
    QCOMPARE(interface->title(), "Interface_ICommand_Pi");

    const auto params = interface->params();
    QCOMPARE(params.size(), 1);

    const auto param = params[0];
    QCOMPARE(param.name(), "InputParam");
    QCOMPARE(param.paramTypeName(), "ICommand_Type");
    QCOMPARE(param.direction(), shared::InterfaceParameter::Direction::IN);
}

void tst_SedsToIvTranslator::testTranslateComponentWithRequiredInterface()
{
    // clang-format off
    const auto sedsModel =
        SedsModelBuilder("Package")
            .withIntegerDataType("MyInteger")
            .withComponent(
                SedsComponentBuilder("Component")
                    .declaringInterface(
                        SedsInterfaceDeclarationBuilder("RequiredInterface")
                            .withCommand(SedsInterfaceCommandBuilder("ICommand", InterfaceCommandMode::Async)
                                .withArgument("CmdArg", "GenericType", CommandArgumentMode::In)
                                .build())
                            .build())
                    .withRequiredInterface(
                        SedsInterfaceBuilder("Interface", "RequiredInterface")
                            .withMappings({{"GenericType", "MyInteger"}})
                            .build())
                    .build())
            .build();
    // clang-format on

    // clang-format off
    const auto asn1Model =
        Asn1ModelBuilder("PACKAGE")
            .withIntegerDataType("MyInteger")
        .build();
    // clang-format on

    Options options;
    options.add(IvOptions::configFilepath, "config.xml");

    SedsToIvTranslator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get(), asn1Model.get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::InterfaceView);

    const auto *ivModel = dynamic_cast<IVModel *>(resultModel.get());
    QVERIFY(ivModel);

    const auto *function = ivModel->getFunction("Component", Qt::CaseSensitive);
    QVERIFY(function);

    const auto interfaces = function->interfaces();
    QCOMPARE(interfaces.size(), 1);

    const auto *interface = interfaces[0];
    QVERIFY(interface);
    QVERIFY(interface->isRequired());
    QCOMPARE(interface->title(), "Interface_ICommand_Ri");

    const auto params = interface->params();
    QCOMPARE(params.size(), 1);

    const auto param = params[0];
    QCOMPARE(param.name(), "InputParam");
    QCOMPARE(param.paramTypeName(), "ICommand_Type");
    QCOMPARE(param.direction(), shared::InterfaceParameter::Direction::IN);
}

} // namespace conversion::iv::test

QTEST_MAIN(conversion::iv::test::tst_SedsToIvTranslator)

#include "tst_sedstoivtranslator.moc"
