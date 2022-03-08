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

#include "dvnode.h"
#include "dvpartition.h"

#include <dvcore/dvboard.h>
#include <dvcore/dvdevice.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvobject.h>
#include <ivcore/ivfunction.h>
#include <memory>
#include <vector>

namespace testgenerator {

/**
 * @brief Test harness DeploymentView generator
 *
 */
class DvGenerator final
{
public:
    typedef QVector<qint32> Coordinates;

    struct DvCoordinates {
        static QVector<Coordinates> devices;
        static Coordinates node;
        static Coordinates partition;
    };

    static const QString X86_LINUX_CPP;
    static const QString SAM_V71_FREERTOS_N7S;
    static const QString X86_LINUX_POHIC;
    static const QString GR740_RTEMS_POHIC;
    static const QString RASPBERRY_PI_LINUX_POHIC;
    static const QString ZYNQ_ZC706_RTEMS_POHIC;
    static const QString BRAVE_LARGE_FREERTOS;
    static const QString LINUX_ARM_RUNTIME;

    static auto generate(const std::vector<ivm::IVFunction *> &functionsToBind,
            const QVector<dvm::DVObject *> &hwObjects, const QString &nodeName = "Node",
            const QString &nodeLabel = "Node_1", const QString &hostPartitionName = "hostPartition")
            -> std::unique_ptr<dvm::DVModel>;

private:
    static auto cloneDvObject(dvm::DVObject *object) -> dvm::DVObject *;
    static auto cloneDeviceAndAddToModelAndNode(dvm::DVObject *device, dvm::DVModel *model, dvm::DVObject *node)
            -> void;
    static auto cloneFunctionAndAddToModel(
            ivm::IVFunction *function, dvm::DVModel *model, dvm::DVObject *node, const QString &partitionTitle) -> void;

    static auto getBoard(const QVector<dvm::DVObject *> &objects) -> dvm::DVBoard *;
    static auto getDevices(const QVector<dvm::DVObject *> &objects) -> QVector<dvm::DVObject *>;

    static auto makeNodeAndAddToModel(const QString &nodeName, const QString &nodeLabel, dvm::DVModel *model,
            dvm::DVBoard *board) -> dvm::DVNode *;
    static auto makePartitionAndAddToNode(const QString &hostPartitionName, dvm::DVModel *model, dvm::DVNode *node)
            -> dvm::DVPartition *;

    static auto setDvObjectModelAndTitle(dvm::DVObject *object, dvm::DVModel *model, const QString &title) -> void;

    template<typename DVType>
    static auto makeDvObject(dvm::DVModel *const model, const QString &title) -> DVType *
    {
        DVType *const object = new DVType;
        object->setTitle(title);
        object->setModel(model);

        return object;
    }

    static const QString nodeLabelToken;
    static const QString nameToken;
    static const QString asn1moduleToken;
    static const QString devNamespaceToken;
    static const QString requiresBusAccessToken;
    static const QString requires_bus_accessToken;
    static const QString extendsToken;
    static const QString busNamespaceToken;
    static const QString asn1fileToken;
    static const QString asn1typeToken;
    static const QString implExtendsToken;
    static const QString portToken;
    static const QString typeToken;
    static const QString pathToken;
};

} // namespace testgenerator
