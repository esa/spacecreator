/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "exportabledvnode.h"

#include "dvcommonprops.h"
#include "dvdevice.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "dvpartition.h"
#include "dvsystemfunction.h"

namespace dve {

ExportableDVNode::ExportableDVNode(const dvm::DVObject *dvObject)
    : ExportableDVObject(dvObject)
{
}

QVariantList ExportableDVNode::partitions() const
{
    QVariantList partitions;
    for (const dvm::DVPartition *partition : exportedObject<dvm::DVNode>()->partitions())
        partitions << createFrom(partition);
    return partitions;
}

QVariantList ExportableDVNode::devices() const
{
    QVariantList devices;
    for (const dvm::DVDevice *device : exportedObject<dvm::DVNode>()->devices())
        devices << createFrom(device);
    return devices;
}

QVariantList ExportableDVNode::requiredBusAccesses() const
{
    QVariantList devices;
    auto node = exportedObject<dvm::DVNode>();
    if (!node) {
        return devices;
    }
    dvm::DVModel *model = node->model();
    if (!model) {
        return devices;
    }

    for (const dvm::DVDevice *device : node->devices()) {
        if (model->isUsed(device)) {
            devices << createFrom(device);
        }
    }

    return devices;
}

QVariantList ExportableDVNode::systemFunctions() const
{
    QVariantList functions;
    for (dvm::DVSystemFunction *function : exportedObject<dvm::DVNode>()->systemFunctions()) {
        functions << createFrom(function);
    }
    return functions;
}

QString ExportableDVNode::nodeLabel() const
{
    auto node = exportedObject<dvm::DVNode>();
    if (!node) {
        return {};
    }
    return node->nodeLabel();
}

} // namespace dve
