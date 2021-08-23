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

#include "connectioncreationvalidator.h"

#include "dvconnection.h"
#include "dvmodel.h"
#include "dvnode.h"

namespace dvm {

ConnectionCreationValidator::FailReason ConnectionCreationValidator::canConnect(
        DVNode *sourceNode, DVNode *targetNode, DVDevice *sourceDevice, DVDevice *targetDevice)
{
    if (!sourceDevice)
        return ConnectionCreationValidator::FailReason::NoStartDevice;
    if (!targetDevice)
        return ConnectionCreationValidator::FailReason::NoEndDevice;
    if (!sourceNode)
        return ConnectionCreationValidator::FailReason::NoStartNode;
    if (!targetNode)
        return ConnectionCreationValidator::FailReason::NoEndNode;
    if (sourceDevice == targetDevice)
        return ConnectionCreationValidator::FailReason::SameDevice;
    if (sourceNode == targetNode)
        return ConnectionCreationValidator::FailReason::SameParent;
    if (!sourceNode->model() || !targetNode->model() || !sourceDevice || !targetDevice)
        return ConnectionCreationValidator::FailReason::NoScene;

    for (DVConnection *connection : sourceNode->model()->connections(sourceDevice)) {
        if ((connection->sourceDevice() == sourceDevice || connection->targetDevice() == sourceDevice)
                && (connection->sourceDevice() == targetDevice || connection->targetDevice() == targetDevice)) {
            return ConnectionCreationValidator::FailReason::AlreadyExists;
        }
    }

    return ConnectionCreationValidator::FailReason::NotFail;
}

}
