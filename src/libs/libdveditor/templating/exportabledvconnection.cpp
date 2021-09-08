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

#include "exportabledvconnection.h"

#include "dvconnection.h"
#include "dvdevice.h"
#include "dvmessage.h"
#include "dvmodel.h"
#include "dvnode.h"

#include <algorithm>

namespace dve {

ExportableDVConnection::ExportableDVConnection(const dvm::DVObject *dvObject)
    : ExportableDVObject(dvObject)
{
}

QVariantList ExportableDVConnection::messages() const
{
    QVariantList messages;
    const dvm::DVConnection *connection = exportedObject<dvm::DVConnection>();
    for (const dvm::DVMessage *message : connection->messages()) {
        messages << createFrom(message);
    }
    return messages;
}

QString ExportableDVConnection::fromNode() const
{
    dvm::DVNode *node = exportedObject<dvm::DVConnection>()->sourceNode();
    return node ? node->title() : "";
}

QString ExportableDVConnection::fromDevice() const
{
    dvm::DVDevice *device = exportedObject<dvm::DVConnection>()->sourceDevice();
    return device ? device->title() : "";
}

QString ExportableDVConnection::fromPort() const
{
    dvm::DVDevice *device = exportedObject<dvm::DVConnection>()->sourceDevice();
    return device ? device->portName() : "";
}

QString ExportableDVConnection::toNode() const
{
    dvm::DVNode *node = exportedObject<dvm::DVConnection>()->targetNode();
    return node ? node->title() : "";
}

QString ExportableDVConnection::toDevice() const
{
    dvm::DVDevice *device = exportedObject<dvm::DVConnection>()->targetDevice();
    return device ? device->title() : "";
}

QString ExportableDVConnection::toPort() const
{
    dvm::DVDevice *device = exportedObject<dvm::DVConnection>()->targetDevice();
    return device ? device->portName() : "";
}

QString ExportableDVConnection::busName() const
{
    dvm::DVConnection *connection = const_cast<dvm::DVConnection *>(exportedObject<dvm::DVConnection>());
    dvm::DVDevice *device = connection->sourceDevice();
    dvm::DVModel *model = device ? device->model() : nullptr;
    if (!model) {
        return {};
    }

    QList<QList<dvm::DVConnection *>> clusters = model->connectionClusters();
    auto it = std::find_if(clusters.begin(), clusters.end(),
            [&connection](QList<dvm::DVConnection *> cluster) { return cluster.contains(connection); });
    if (it == clusters.end()) {
        return {};
    }

    return it->first()->sourceNode()->title() + "_" + it->first()->sourceDevice()->title();
}

} // namespace dve
