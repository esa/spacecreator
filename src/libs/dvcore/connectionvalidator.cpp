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

#include "connectionvalidator.h"

#include "dvconnection.h"
#include "dvdevice.h"
#include "dvmodel.h"
#include "dvnode.h"

namespace dvm {

ConnectionValidator::FailReason ConnectionValidator::canConnect(DVNode *sourceNode, DVNode *targetNode,
        DVDevice *sourceDevice, DVDevice *targetDevice, DVConnection *ignoreConnection)
{
    if (!sourceDevice)
        return ConnectionValidator::FailReason::NoStartDevice;
    if (!targetDevice)
        return ConnectionValidator::FailReason::NoEndDevice;
    if (!sourceNode)
        return ConnectionValidator::FailReason::NoStartNode;
    if (!targetNode)
        return ConnectionValidator::FailReason::NoEndNode;
    if (sourceDevice == targetDevice)
        return ConnectionValidator::FailReason::SameDevice;
    if (sourceNode == targetNode)
        return ConnectionValidator::FailReason::SameParent;
    if (!sourceNode->model() || !targetNode->model() || !sourceDevice || !targetDevice)
        return ConnectionValidator::FailReason::NoScene;

    for (DVConnection *connection : sourceNode->model()->connections(sourceDevice)) {
        if (connection == ignoreConnection) {
            continue;
        }
        if ((connection->sourceDevice() == sourceDevice || connection->targetDevice() == sourceDevice)
                && (connection->sourceDevice() == targetDevice || connection->targetDevice() == targetDevice)) {
            return ConnectionValidator::FailReason::AlreadyExists;
        }
    }

    // Check if the devices are compatible
    static const QString accessTocken = meta::Props::token(meta::Props::Token::requires_bus_access);
    if (sourceDevice->entityAttributeValue(accessTocken) != targetDevice->entityAttributeValue(accessTocken)) {
        return ConnectionValidator::FailReason::ParamsDiffer;
    }

    return ConnectionValidator::FailReason::NotFail;
}

/*!
   Checks if the connected devices exist and are are compatible
 */
ConnectionValidator::FailReason ConnectionValidator::check(DVConnection *connection)
{
    if (!connection) {
        return ConnectionValidator::FailReason::NoScene;
    }

    return canConnect(connection->sourceNode(), connection->targetNode(), connection->sourceDevice(),
            connection->targetDevice(), connection);
}

}
