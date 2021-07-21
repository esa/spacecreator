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

#include "graphicsitemhelpers.h"

#include "connectioncreationvalidator.h"
#include "dvdevicegraphicsitem.h"
#include "graphicsviewutils.h"

namespace dve {
namespace gi {

dvm::ValidationResult validateConnectionCreate(QGraphicsScene *scene, const QVector<QPointF> &points)
{
    dvm::ValidationResult result;
    result.startPointAdjusted = points.front();
    result.endPointAdjusted = points.back();
    auto startDeviceItem = shared::graphicsviewutils::itemAt<DVDeviceGraphicsItem>(scene, result.startPointAdjusted);
    if (!startDeviceItem || !startDeviceItem->entity()) {
        result.status = dvm::ConnectionCreationValidator::FailReason::NoStartDevice;
        return result;
    }
    auto endDeviceItem = shared::graphicsviewutils::itemAt<DVDeviceGraphicsItem>(scene, result.endPointAdjusted);
    if (!endDeviceItem || !endDeviceItem->entity()) {
        result.status = dvm::ConnectionCreationValidator::FailReason::NoEndDevice;
        return result;
    }

    result.startDeviceId = startDeviceItem->entity()->id();
    result.endDeviceId = endDeviceItem->entity()->id();

    result.status = dvm::ConnectionCreationValidator::canConnect(startDeviceItem->entity()->node(),
            endDeviceItem->entity()->node(), startDeviceItem->entity(), endDeviceItem->entity());

    /// TODO:
    return result;
}

} // namespace gi
} // namespace dve
