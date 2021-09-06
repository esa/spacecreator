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

#include <QObject>
#include <QString>

namespace dve {

class ExportableBusAccess
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString deviceName READ deviceName)
    Q_PROPERTY(QString portName READ portName)
    Q_PROPERTY(QString busName READ busName)

public:
    QString name() const;

    const QString &deviceName() const;
    void setDeviceName(const QString &name);

    const QString &portName() const;
    void setPortName(const QString &name);

    const QString &busName() const;
    void setBusName(const QString &name);

private:
    QString m_deviceName;
    QString m_portName;
    QString m_busName;
};

} // namespace dve

Q_DECLARE_METATYPE(dve::ExportableBusAccess)
Q_DECLARE_TYPEINFO(dve::ExportableBusAccess, Q_MOVABLE_TYPE);
