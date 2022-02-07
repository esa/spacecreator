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
#include <ivcore/ivfunction.h>
#include <ivcore/ivobject.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <memory>
#include <shared/common.h>
#include <shared/parameter.h>
#include <shared/propertytemplateconfig.h>
#include <stdexcept>

namespace testgenerator {

auto IvGenerator::generate(ivm::IVInterface *const interfaceUnderTest) -> std::unique_ptr<ivm::IVModel>
{
    auto *const config = ivm::IVPropertyTemplateConfig::instance();
    if (config == nullptr) {
        throw std::runtime_error("config is null");
    }
    auto ivModel = std::make_unique<ivm::IVModel>(config);

    if (interfaceUnderTest == nullptr) {
        return ivModel;
    }

    if (interfaceUnderTest->function() == nullptr) {
        throw std::runtime_error("Selected interface has no function specified");
    }

    const QString interfaceUnderTestName = interfaceUnderTest->title();
    const QString functionUnderTestName = interfaceUnderTest->function()->title();

    ivm::IVFunction *const testDriverFunction = new ivm::IVFunction;
    testDriverFunction->setTitle("TestDriver");

    // add interface parameters
    QVector<shared::InterfaceParameter> ifParams;
    ivm::IVInterface::CreationInfo ifCreationInfo;
    ifCreationInfo.model = ivModel.get();
    ifCreationInfo.function = testDriverFunction;
    ifCreationInfo.name = interfaceUnderTestName;
    ifCreationInfo.type = ivm::IVInterface::InterfaceType::Required;
    ifCreationInfo.kind = ivm::IVInterface::OperationKind::Protected;
    ifCreationInfo.parameters = ifParams;
    testDriverFunction->addChild(ivm::IVInterface::createIface(ifCreationInfo));

    ivModel->addObject(testDriverFunction);

    ivm::IVFunction *const functionUnderTest = new ivm::IVFunction;
    functionUnderTest->setTitle(functionUnderTestName);
    ivModel->addObject(functionUnderTest);

    return ivModel;
}

} // namespace testgenerator
