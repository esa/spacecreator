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
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/SedsToIvTranslator/translator.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <sedscomponentbuilder.h>
#include <sedsinterfacebuilder.h>
#include <sedsinterfacecommandbuilder.h>
#include <sedsinterfacedeclarationbuilder.h>
#include <sedsmodelbuilder.h>

using namespace ivm;
using namespace seds::model;

using conversion::ModelType;
using conversion::Options;
using conversion::iv::IvOptions;
using conversion::iv::translator::SedsToIvTranslator;

namespace tests::conversion::iv {

using common::SedsComponentBuilder;
using common::SedsInterfaceBuilder;
using common::SedsInterfaceCommandBuilder;
using common::SedsInterfaceDeclarationBuilder;
using common::SedsModelBuilder;

class tst_SedsToIvTranslator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testTranslateComponentWithProvidedInterface();
    void testTranslateComponentWithRequiredInterface();
};

void tst_SedsToIvTranslator::testTranslateComponentWithProvidedInterface()
{
    // clang-format off
    const std::unique_ptr<seds::model::SedsModel> sedsModel =
        SedsModelBuilder("Package")
            .withIntegerDataType("MyInteger")
            .withComponent(
                SedsComponentBuilder("Component")
                    .declaringInterface(
                        SedsInterfaceDeclarationBuilder("RequiredInterface")
                            .withCommand(SedsInterfaceCommandBuilder("ICommand", InterfaceCommandMode::Async)
                                .withArgument("CmdArg", "GenericType", CommandArgumentMode::In)))
                    .withProvidedInterface(
                        SedsInterfaceBuilder("Interface", "RequiredInterface")
                            .withMappings({{"GenericType", "MyInteger"}})));
    // clang-format on

    Options options;
    options.add(IvOptions::configFilename, "config.xml");

    SedsToIvTranslator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 2);

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
    QCOMPARE(param.paramTypeName(), "Interface_ICommand_Type");
    QCOMPARE(param.direction(), ivm::InterfaceParameter::Direction::IN);
}

void tst_SedsToIvTranslator::testTranslateComponentWithRequiredInterface()
{
    // clang-format off
    const std::unique_ptr<seds::model::SedsModel> sedsModel =
        SedsModelBuilder("Package")
            .withIntegerDataType("MyInteger")
            .withComponent(
                SedsComponentBuilder("Component")
                    .declaringInterface(
                        SedsInterfaceDeclarationBuilder("RequiredInterface")
                            .withCommand(SedsInterfaceCommandBuilder("ICommand", InterfaceCommandMode::Async)
                                .withArgument("CmdArg", "GenericType", CommandArgumentMode::In)))
                    .withRequiredInterface(
                        SedsInterfaceBuilder("Interface", "RequiredInterface")
                            .withMappings({{"GenericType", "MyInteger"}})));
    // clang-format on

    Options options;
    options.add(IvOptions::configFilename, "config.xml");

    SedsToIvTranslator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 2);

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
    QCOMPARE(param.paramTypeName(), "Interface_ICommand_Type");
    QCOMPARE(param.direction(), ivm::InterfaceParameter::Direction::IN);
}

} // namespace tests::conversion::iv

QTEST_MAIN(tests::conversion::iv::tst_SedsToIvTranslator)

#include "tst_sedstoivtranslator.moc"
