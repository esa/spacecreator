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

#include "ivgenerator.h"

#include <dvcore/dvcommonprops.h>
#include <dvcore/dvobject.h>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivobject.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <memory>
#include <shared/common.h>
#include <shared/parameter.h>
#include <shared/propertytemplateconfig.h>
#include <stdexcept>

namespace testgenerator {

const QVector<qint32> IvGenerator::Coordinates::Function::testDriver = { 75, 184, 275, 264 };
const QVector<qint32> IvGenerator::Coordinates::Function::functionUnderTest = { 462, 106, 662, 186 };

const QVector<qint32> IvGenerator::Coordinates::Interface::startTestIf = { 123, 184 };
const QVector<qint32> IvGenerator::Coordinates::Interface::interfaceUnderTestRi = { 275, 218 };
const QVector<qint32> IvGenerator::Coordinates::Interface::interfaceUnderTestPi = { 462, 142 };

const QVector<qint32> IvGenerator::Coordinates::connection = { 281, 218, 370, 218, 459, 142 };

const QString IvGenerator::startTestInterfaceName = "StartTest";
const QString IvGenerator::testDriverFunctionName = "TestDriver";

auto IvGenerator::generate(ivm::IVInterface *const interfaceUnderTest) -> std::unique_ptr<ivm::IVModel>
{
    checkInputArgument(interfaceUnderTest);

    auto *const config = ivm::IVPropertyTemplateConfig::instance();
    if (config == nullptr) {
        throw std::invalid_argument("config is null");
    }

    if (interfaceUnderTest == nullptr) {
        return nullptr;
    }

    auto ivModel = std::make_unique<ivm::IVModel>(config);

    auto *const testDriverFunction = makeTestDriverFunction(ivModel.get());
    auto *const testDriverRi = makeTestDriverRequiredIface(interfaceUnderTest, testDriverFunction);
    auto *const testDriverStartTestIf = makeStartTestIface(testDriverFunction);

    testDriverFunction->addChild(testDriverRi);
    testDriverFunction->addChild(testDriverStartTestIf);

    auto *const functionUnderTest = makeFunctionUnderTest(ivModel.get(), interfaceUnderTest);
    auto *const interfaceUnderTestPi = makeFunctionUnderTestProvidedInterface(interfaceUnderTest, functionUnderTest);

    functionUnderTest->addChild(interfaceUnderTestPi);

    auto *const connection = makeConnection(testDriverRi, interfaceUnderTestPi, ivModel.get());

    ivModel->addObject(testDriverFunction);
    ivModel->addObject(functionUnderTest);
    ivModel->addObject(connection);

    return ivModel;
}

auto IvGenerator::checkInputArgument(ivm::IVInterface *const iface) -> void
{
    if (iface->function() == nullptr) {
        throw std::invalid_argument("Selected interface has no function specified");
    }

    const auto ifaceOperationKind = iface->kind();
    if (ifaceOperationKind == ivm::IVInterface::OperationKind::Any
            || ifaceOperationKind == ivm::IVInterface::OperationKind::Cyclic
            || ifaceOperationKind == ivm::IVInterface::IVInterface::OperationKind::Sporadic) {
        throw std::invalid_argument("Only Protected and Unprotected interfaces can be tested");
    }

    if (iface->type() != ivm::IVObject::Type::ProvidedInterface) {
        throw std::invalid_argument("Only Provided Interface can be tested");
    }
}

auto IvGenerator::makeTestDriverFunction(ivm::IVModel *const model) -> ivm::IVFunction *
{
    throwOnNullpointer(model);

    ivm::IVFunction *const function = new ivm::IVFunction;
    function->setModel(model);
    function->setTitle(testDriverFunctionName);
    function->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::is_type), "NO");
    function->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::language), "C");
    function->setDefaultImplementation("default");

    setObjectCoordinates(function, Coordinates::Function::testDriver);

    return function;
}

auto IvGenerator::makeFunctionUnderTest(ivm::IVModel *const model, ivm::IVInterface *const ifaceUnderTest)
        -> ivm::IVFunction *
{
    throwOnNullpointer(model);
    throwOnNullpointer(ifaceUnderTest);

    ivm::IVFunction *const function = new ivm::IVFunction;
    function->setModel(model);
    function->setTitle(ifaceUnderTest->function()->title());
    for (const auto &entityAttribute : ifaceUnderTest->function()->entityAttributes()) {
        function->setEntityAttribute(entityAttribute.name(), entityAttribute.value().toString());
    }
    setObjectCoordinates(function, Coordinates::Function::functionUnderTest);

    return function;
}

auto IvGenerator::makeStartTestIface(ivm::IVFunction *const testDriverFunction) -> ivm::IVInterface *
{
    throwOnNullpointer(testDriverFunction);

    ivm::IVInterface::CreationInfo ci;
    ci.name = startTestInterfaceName;
    ci.model = testDriverFunction->model();
    ci.function = testDriverFunction;
    ci.kind = ivm::IVInterface::OperationKind::Cyclic;
    ci.type = ivm::IVInterface::InterfaceType::Provided;

    auto *const iface = ivm::IVInterface::createIface(ci);
    iface->setEntityAttribute("period", "999");
    iface->setEntityAttribute("stack_size", "5000");
    iface->setEntityAttribute("priority", "1");
    iface->setEntityAttribute("dispatch_offset", "0");
    iface->setEntityAttribute("wcet", "0");
    setObjectCoordinates(iface, Coordinates::Interface::startTestIf);

    return iface;
}

auto IvGenerator::makeTestDriverRequiredIface(
        ivm::IVInterface *const ifaceUnderTest, ivm::IVFunction *const testDriverFunction) -> ivm::IVInterface *
{
    throwOnNullpointer(ifaceUnderTest);
    throwOnNullpointer(testDriverFunction);

    ivm::IVInterface::CreationInfo ci =
            ivm::IVInterface::CreationInfo::initFromIface(ifaceUnderTest, ivm::IVInterface::CreationInfo::Policy::Init);

    ci.model = testDriverFunction->model();
    ci.function = testDriverFunction;
    ci.type = ivm::IVInterface::InterfaceType::Required;

    auto *const iface = ivm::IVInterface::createIface(ci);
    iface->setEntityProperty(ivm::meta::Props::token(ivm::meta::Props::Token::Autonamed), "true");
    iface->setEntityProperty(ivm::meta::Props::token(ivm::meta::Props::Token::InheritPI), "true");
    iface->setEntityAttribute("wcet", "0");
    setObjectCoordinates(iface, Coordinates::Interface::interfaceUnderTestRi);

    return iface;
}

auto IvGenerator::makeFunctionUnderTestProvidedInterface(
        ivm::IVInterface *const ifaceUnderTest, ivm::IVFunction *const functionUnderTest) -> ivm::IVInterface *
{
    ivm::IVInterface::CreationInfo ci =
            ivm::IVInterface::CreationInfo::initFromIface(ifaceUnderTest, ivm::IVInterface::CreationInfo::Policy::Init);

    ci.function = functionUnderTest;

    auto *const iface = ivm::IVInterface::createIface(ci);
    iface->setEntityAttribute("wcet", 0);
    setObjectCoordinates(iface, Coordinates::Interface::interfaceUnderTestPi);

    return iface;
}

auto IvGenerator::makeConnection(ivm::IVInterface *const required, ivm::IVInterface *const provided,
        ivm::IVModel *const model) -> ivm::IVConnection *
{
    throwOnNullpointer(required);
    throwOnNullpointer(provided);

    ivm::IVConnection *const connection = new ivm::IVConnection(required, provided, model);

    setObjectCoordinates(connection, Coordinates::connection);

    return connection;
}

auto IvGenerator::setObjectCoordinates(ivm::IVObject *const object, const QVector<qint32> &coordinates) -> void
{
    object->setEntityProperty(ivm::meta::Props::token(ivm::meta::Props::Token::coordinates),
            dvm::DVObject::coordinatesToString(coordinates));
}

auto IvGenerator::throwOnNullpointer(void *const pointer) -> void
{
    if (pointer == nullptr) {
        throw std::invalid_argument("pointer cannot be null");
    }
}

} // namespace testgenerator
