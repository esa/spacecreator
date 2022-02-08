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

#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <memory>

namespace testgenerator {

/**
 * @brief Test harness InterfaceView generator
 *
 */
class IvGenerator
{
public:
    /**
     * @brief name of an interface intended to start (trigger run) of a tests
     *
     */
    static const QString startTestInterfaceName;

    /**
     * @brief name of a function for a test driver (test driver not included)
     *
     */
    static const QString testDriverFunctionName;

    /**
     * @brief generate an IV model with a function under test and a test driver function
     *
     * @param interfaceUnderTest  interface which is tested
     * @return IV model with the function under test and the test driver function
     */
    static auto generate(ivm::IVInterface *interfaceUnderTest) -> std::unique_ptr<ivm::IVModel>;

private:
    static auto makeTestDriverFunction(ivm::IVModel *model, ivm::IVInterface *ifaceUnderTest) -> ivm::IVFunction *;
    static auto makeFunctionUnderTest(ivm::IVInterface *ifaceUnderTest) -> ivm::IVFunction *;
    static auto makeStartTest(ivm::IVModel *model, ivm::IVFunction *function) -> ivm::IVInterface *;
    static auto makeTestDriverRequiredIface(ivm::IVInterface *ifaceUnderTest, ivm::IVFunction *testDriverFunction)
            -> ivm::IVInterface *;
    static auto copyIface(ivm::IVInterface *ifaceUnderTest) -> ivm::IVInterface *;
    static auto throwOnNullpointer(void *pointer) -> void;
};

} // namespace testgenerator
