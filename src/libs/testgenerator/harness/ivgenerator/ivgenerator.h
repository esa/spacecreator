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

#include <ivcore/ivconnection.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <memory>

namespace testgenerator {

/**
 * @brief  Constants with coordinates of the generated entities InterfaceView diagram
 *
 */
struct Coordinates {
    struct Function {
        /**
         * @brief  Coordinates of a test driver function
         *
         */
        static const QVector<qint32> testDriver;
        /**
         * @brief  Coordinates of a function under test (function to which the interface under test belongs to)
         *
         */
        static const QVector<qint32> functionUnderTest;
    };
    struct Interface {
        /**
         * @brief  Coordinates of start test cyclic interface
         *
         */
        static const QVector<qint32> startTestCi;
        /**
         * @brief  Coordinates of interface under test required interface (present in test driver function)
         *
         */
        static const QVector<qint32> interfaceUnderTestRi;
        /**
         * @brief  Coordinates of interface under test provided interface (present in function under test)
         *
         */
        static const QVector<qint32> interfaceUnderTestPi;
    };
    /**
     * @brief  Coordinates of connection between PI and RI
     *
     */
    static const QVector<qint32> connection;
};

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
    static auto makeTestDriverFunction(ivm::IVModel *model) -> ivm::IVFunction *;
    static auto makeFunctionUnderTest(ivm::IVModel *model, ivm::IVInterface *ifaceUnderTest) -> ivm::IVFunction *;
    static auto makeStartTestIface(ivm::IVModel *model, ivm::IVFunction *function) -> ivm::IVInterface *;
    static auto makeTestDriverRequiredIface(ivm::IVInterface *ifaceUnderTest, ivm::IVFunction *testDriverFunction)
            -> ivm::IVInterface *;
    static auto makeTestDriverProvidedInterface(ivm::IVInterface *ifaceUnderTest) -> ivm::IVInterface
            *; // TODO: this shall accept testDriverFunction as a parameter and write iface.function = function
    static auto makeConnection(ivm::IVInterface *required, ivm::IVInterface *provided, ivm::IVModel *model)
            -> ivm::IVConnection *;
    static auto throwOnNullpointer(void *pointer) -> void;
};

} // namespace testgenerator
