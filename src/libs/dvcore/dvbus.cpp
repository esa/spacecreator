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

#include "dvbus.h"

#include "dvconnection.h"
#include "dvdevice.h"

#include <QSet>

namespace dvm {

DVBus::DVBus(DVObject *parent)
    : DVObject(DVObject::Type::Bus, {}, parent)
{
}

void DVBus::setConnections(const QList<DVConnection *> &connections)
{
    QSet<DVDevice *> devices;
    for (DVConnection *connection : connections) {
        devices.insert(connection->sourceDevice());
        devices.insert(connection->targetDevice());
    }
    for (DVDevice *dev : devices) {
        m_devices << QPointer<DVDevice>(dev);
    }
}

QList<DVDevice *> DVBus::connectedDevices() const
{
    QList<DVDevice *> devices;
    for (const QPointer<DVDevice> &dev : m_devices) {
        if (!dev.isNull()) {
            devices << dev.data();
        }
    }
    return devices;
}

QString DVBus::qualifier() const
{
    if (!m_devices.isEmpty() && m_devices.first()) {
        return m_devices.first()->qualifier();
    }

    return "";
}

} // namespace dvm
