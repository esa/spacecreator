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

#include <QVector>
#include <dvcore/dvobject.h>
#include <ivcore/ivfunction.h>
#include <memory>
#include <sstream>

namespace testgenerator {

/**
 * @brief  Constants with coordinates of generated entities on DeploymentView diagram
 *
 */
struct DvCoordinates final {
    struct Node final {
        static const QVector<qint32> node;
        struct Partition final {
            static const QVector<qint32> partition;
            struct Eth0 final {
                static const QVector<qint32> device;
            };
            struct Uart0 final {
                static const QVector<qint32> device;
            };
        };
    };
};

/**
 * @brief Test harness DeploymentView generator
 *
 */
class DvGenerator final
{
public:
    static const QString startTestInterfaceName;
    static const QString testDriverFunctionName;

    static auto generate(const std::vector<ivm::IVFunction *> &functionsToBind) -> std::unique_ptr<dvm::DVModel>;

private:
    static auto makeNode() -> void;
    static auto makePartition() -> void;
    static auto makeDevice() -> void;
    static auto setObjectCoordinates(dvm::DVObject *object, const QVector<qint32> &coordinates) -> void;
    static auto throwOnNullpointer(void *pointer) -> void;
};

} // namespace testgenerator
