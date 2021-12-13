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

#include "exportabledvdevice.h"

#include "dvdevice.h"
#include "dvnode.h"

namespace dve {

ExportableDVDevice::ExportableDVDevice(const dvm::DVObject *dvObject)
    : ExportableDVObject(dvObject)
{
}

QString ExportableDVDevice::fullName() const
{
    return name() + "_" + portName();
}

QString ExportableDVDevice::portName() const
{
    return exportedObject<dvm::DVDevice>()->portName();
}

QString ExportableDVDevice::qualifier() const
{
    return exportedObject<dvm::DVDevice>()->qualifier();
}

QString ExportableDVDevice::nodeName() const
{
    dvm::DVNode *node = qobject_cast<dvm::DVNode *>(exportedObject<dvm::DVDevice>()->node());
    return node ? node->title() : "";
}

/*!
   Label ob the node this device belongs to
 */
QString ExportableDVDevice::nodeLabel() const
{
    dvm::DVNode *node = qobject_cast<dvm::DVNode *>(exportedObject<dvm::DVDevice>()->node());
    return node ? node->nodeLabel() : "";
}

} // namespace dve
