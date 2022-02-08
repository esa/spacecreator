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

#include <QDebug>
#include <exception>
#include <ivcore/ivobject.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <memory>
#include <shared/common.h>
#include <shared/parameter.h>
#include <shared/propertytemplateconfig.h>
#include <stdexcept>

namespace testgenerator {

const QString IvGenerator::startTestInterfaceName = "StartTest";
const QString IvGenerator::testDriverFunctionName = "TestDriver";

auto IvGenerator::generate(ivm::IVInterface *const interfaceUnderTest) -> std::unique_ptr<ivm::IVModel>
{
    auto *const config = ivm::IVPropertyTemplateConfig::instance();
    if (config == nullptr) {
        throw std::runtime_error("config is null");
    }

    if (interfaceUnderTest == nullptr) {
        return nullptr;
    }

    if (interfaceUnderTest->function() == nullptr) {
        throw std::runtime_error("Selected interface has no function specified");
    }

    const auto interfaceUnderTestOperationKind = interfaceUnderTest->kind();
    if (interfaceUnderTestOperationKind == ivm::IVInterface::OperationKind::Any
            || interfaceUnderTestOperationKind == ivm::IVInterface::OperationKind::Cyclic
            || interfaceUnderTestOperationKind == ivm::IVInterface::IVInterface::OperationKind::Sporadic) {
        throw std::runtime_error("Only Protected and Unprotected interfaces can be tested");
    }

    const auto interfaceUnderTestType = interfaceUnderTest->type();
    if (interfaceUnderTestType != ivm::IVObject::Type::ProvidedInterface) {
        throw std::runtime_error("Only Provided Interface can be tested");
    }

    auto ivModel = std::make_unique<ivm::IVModel>(config);
    ivModel->addObject(makeTestDriverFunction(ivModel.get(), interfaceUnderTest));
    ivModel->addObject(makeFunctionUnderTest(interfaceUnderTest));

    return ivModel;
}

auto IvGenerator::makeTestDriverFunction(ivm::IVModel *const model, ivm::IVInterface *const ifaceUnderTest)
        -> ivm::IVFunction *
{
    throwOnNullpointer(model);
    throwOnNullpointer(ifaceUnderTest);

    ivm::IVFunction *const function = new ivm::IVFunction;
    function->setTitle(testDriverFunctionName);
    function->setModel(model);
    function->addChild(makeTestDriverRequiredIface(ifaceUnderTest, function));
    function->addChild(makeStartTest(model, function));

    return function;
}

auto IvGenerator::makeFunctionUnderTest(ivm::IVInterface *const ifaceUnderTest) -> ivm::IVFunction *
{
    throwOnNullpointer(ifaceUnderTest);

    ivm::IVFunction *const function = new ivm::IVFunction;
    function->setTitle(ifaceUnderTest->function()->title());
    function->addChild(copyIface(ifaceUnderTest));

    return function;
}

auto IvGenerator::makeStartTest(ivm::IVModel *const model, ivm::IVFunction *const function) -> ivm::IVInterface *
{
    throwOnNullpointer(model);
    throwOnNullpointer(function);

    ivm::IVInterface::CreationInfo ci;
    ci.name = startTestInterfaceName;
    ci.model = model;
    ci.function = function;
    ci.kind = ivm::IVInterface::OperationKind::Cyclic;
    ci.type = ivm::IVInterface::InterfaceType::Provided;

    return ivm::IVInterface::createIface(ci);
}

auto IvGenerator::makeTestDriverRequiredIface(
        ivm::IVInterface *const ifaceUnderTest, ivm::IVFunction *const testDriverFunction) -> ivm::IVInterface *
{
    throwOnNullpointer(ifaceUnderTest);
    throwOnNullpointer(testDriverFunction);

    ivm::IVInterface::CreationInfo ci = ivm::IVInterface::CreationInfo::fromIface(ifaceUnderTest);
    ci.model = testDriverFunction->model();
    ci.function = testDriverFunction;
    ci.type = ivm::IVInterface::InterfaceType::Required;

    return ivm::IVInterface::createIface(ci);
}

auto IvGenerator::copyIface(ivm::IVInterface *const ifaceUnderTest) -> ivm::IVInterface *
{
    return ivm::IVInterface::createIface(ivm::IVInterface::CreationInfo::fromIface(ifaceUnderTest));
}

auto IvGenerator::throwOnNullpointer(void *const pointer) -> void
{
    if (pointer == nullptr) {
        throw std::invalid_argument("pointer cannot be null");
    }
}

} // namespace testgenerator
