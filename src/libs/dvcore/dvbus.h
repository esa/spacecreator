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

#include "dvdevice.h"
#include "dvobject.h"

#include <QPointer>

namespace dvm {
class DVConnection;

class DVBus : public DVObject
{
    Q_OBJECT
    Q_PROPERTY(QList<dvm::DVDevice *> connectedDevices READ connectedDevices)
    Q_PROPERTY(QString qualifier READ qualifier)

public:
    explicit DVBus(DVObject *parent = nullptr);

    void setConnections(const QList<DVConnection *> &connections);

    QList<DVDevice *> connectedDevices() const;
    QString qualifier() const;

private:
    QList<QPointer<DVDevice>> m_devices;
};

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVBus *)
