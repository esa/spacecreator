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
#include <seds/SedsModel/sedsmodel.h>

using namespace ivm;
using namespace seds::model;

using conversion::Options;
using conversion::iv::IvOptions;
using conversion::iv::translator::SedsToIvTranslator;

namespace seds::test {

class tst_SedsToIvTranslator : public QObject
{
    Q_OBJECT

public:
    virtual ~tst_SedsToIvTranslator() = default;

private Q_SLOTS:
    void testTranslateComponentWithProvidedInterface();
    void testTranslateComponentWithRequiredInterface();

private:
    std::unique_ptr<SedsModel> createComponentWithProvidedInterface();
    std::unique_ptr<SedsModel> createComponentWithRequiredInterface();
};

void tst_SedsToIvTranslator::testTranslateComponentWithProvidedInterface()
{
    const auto sedsModel = createComponentWithProvidedInterface();

    Options options;
    options.add(IvOptions::configFilename, "config.xml");

    SedsToIvTranslator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 2);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), conversion::ModelType::InterfaceView);

    const auto *ivModel = dynamic_cast<IVModel *>(resultModel.get());
    QVERIFY(ivModel);

    const auto *function = ivModel->getFunction("Component", Qt::CaseSensitive);
    QVERIFY(function);

    const auto interfaces = function->interfaces();
    QCOMPARE(interfaces.size(), 1);

    const auto *interface = interfaces[0];
    QVERIFY(interface);
    QVERIFY(interface->isProvided());
    QCOMPARE(interface->title(), "ICommand");
}

void tst_SedsToIvTranslator::testTranslateComponentWithRequiredInterface()
{
    const auto sedsModel = createComponentWithRequiredInterface();

    Options options;
    options.add(IvOptions::configFilename, "config.xml");

    SedsToIvTranslator translator;

    const auto resultModels = translator.translateModels({ sedsModel.get() }, options);
    QCOMPARE(resultModels.size(), 2);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), conversion::ModelType::InterfaceView);

    const auto *ivModel = dynamic_cast<IVModel *>(resultModel.get());
    QVERIFY(ivModel);

    const auto *function = ivModel->getFunction("Component", Qt::CaseSensitive);
    QVERIFY(function);

    const auto interfaces = function->interfaces();
    QCOMPARE(interfaces.size(), 1);

    const auto *interface = interfaces[0];
    QVERIFY(interface);
    QVERIFY(interface->isRequired());
    QCOMPARE(interface->title(), "ICommand");
}

std::unique_ptr<SedsModel> tst_SedsToIvTranslator::createComponentWithProvidedInterface()
{
    CommandArgument argument;
    argument.setName(QStringLiteral("CmdArg"));
    argument.setType(QStringLiteral("GenericType"));
    argument.setMode(CommandArgumentMode::In);

    InterfaceCommand command;
    command.setName(QStringLiteral("ICommand"));
    command.setMode(InterfaceCommandMode::Async);
    command.addArgument(std::move(argument));

    InterfaceDeclaration interfaceDeclaration;
    interfaceDeclaration.setName(QStringLiteral("ProvidedInterface"));
    interfaceDeclaration.addCommand(std::move(command));

    GenericTypeMap genericTypeMap;
    genericTypeMap.setName(QStringLiteral("GenericType"));
    genericTypeMap.setType(QStringLiteral("MyInteger"));

    GenericTypeMapSet genericTypeMapSet;
    genericTypeMapSet.addGenericTypeMap(std::move(genericTypeMap));

    Interface interface;
    interface.setName(QStringLiteral("Interface"));
    interface.setType(QStringLiteral("ProvidedInterface"));
    interface.setGenericTypeMapSet(std::move(genericTypeMapSet));

    Component component;
    component.setName(QStringLiteral("Component"));
    component.addInterfaceDeclaration(std::move(interfaceDeclaration));
    component.addProvidedInterface(std::move(interface));

    IntegerDataType integerDataType;
    integerDataType.setName(QStringLiteral("MyInteger"));

    Package package;
    package.setName(QStringLiteral("Package"));
    package.addComponent(std::move(component));
    package.addDataType(std::move(integerDataType));

    PackageFile packageFile;
    packageFile.setPackage(std::move(package));

    return std::make_unique<SedsModel>(std::move(packageFile));
}

std::unique_ptr<SedsModel> tst_SedsToIvTranslator::createComponentWithRequiredInterface()
{
    CommandArgument argument;
    argument.setName(QStringLiteral("CmdArg"));
    argument.setType(QStringLiteral("GenericType"));
    argument.setMode(CommandArgumentMode::In);

    InterfaceCommand command;
    command.setName(QStringLiteral("ICommand"));
    command.setMode(InterfaceCommandMode::Async);
    command.addArgument(std::move(argument));

    InterfaceDeclaration interfaceDeclaration;
    interfaceDeclaration.setName(QStringLiteral("RequiredInterface"));
    interfaceDeclaration.addCommand(std::move(command));

    GenericTypeMap genericTypeMap;
    genericTypeMap.setName(QStringLiteral("GenericType"));
    genericTypeMap.setType(QStringLiteral("MyInteger"));

    GenericTypeMapSet genericTypeMapSet;
    genericTypeMapSet.addGenericTypeMap(std::move(genericTypeMap));

    Interface interface;
    interface.setName(QStringLiteral("Interface"));
    interface.setType(QStringLiteral("RequiredInterface"));
    interface.setGenericTypeMapSet(std::move(genericTypeMapSet));

    Component component;
    component.setName(QStringLiteral("Component"));
    component.addInterfaceDeclaration(std::move(interfaceDeclaration));
    component.addRequiredInterface(std::move(interface));

    IntegerDataType integerDataType;
    integerDataType.setName(QStringLiteral("MyInteger"));

    Package package;
    package.setName(QStringLiteral("Package"));
    package.addComponent(std::move(component));
    package.addDataType(std::move(integerDataType));

    PackageFile packageFile;
    packageFile.setPackage(std::move(package));

    return std::make_unique<SedsModel>(std::move(packageFile));
}

} // namespace seds::test

QTEST_MAIN(seds::test::tst_SedsToIvTranslator)

#include "tst_sedstoivtranslator.moc"
