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

#pragma once

#include "common.h"

#include <QMetaEnum>
#include <QPointF>
#include <QVector>

class QGraphicsScene;
class QGraphicsItem;

namespace dvm {
class DVNode;
class DVDevice;

class ConnectionCreationValidator
{
    Q_GADGET

public:
    enum class FailReason
    {
        NotFail = 0,
        ParamsDiffer,
        SameParent,
        SameDevice,
        NoScene,
        NoStartNode,
        NoEndNode,
        NoStartDevice,
        NoEndDevice,
        AlreadyExists,
    };
    Q_ENUM(FailReason)

    static ConnectionCreationValidator::FailReason canConnect(
            DVNode *sourceNode, DVNode *targetNode, DVDevice *sourceDevice, DVDevice *targetDevice);

private:
    ConnectionCreationValidator();
};

struct ValidationResult {
    shared::Id startDeviceId;
    shared::Id endDeviceId;

    QPointF startPointAdjusted;
    QPointF endPointAdjusted;

    QVector<QPointF> connectionPoints;

    ConnectionCreationValidator::FailReason status { ConnectionCreationValidator::FailReason::NotFail };

    inline bool failed() const { return status != ConnectionCreationValidator::FailReason::NotFail; }
    inline bool setFailed(ConnectionCreationValidator::FailReason s)
    {
        status = s;
        return failed();
    }
};

}
