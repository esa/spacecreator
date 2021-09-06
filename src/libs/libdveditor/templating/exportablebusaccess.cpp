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

#include "exportablebusaccess.h"

namespace dve {

QString ExportableBusAccess::name() const
{
    return m_deviceName + "_" + m_portName;
}

const QString &ExportableBusAccess::deviceName() const
{
    return m_deviceName;
}

void ExportableBusAccess::setDeviceName(const QString &name)
{
    m_deviceName = name;
}

const QString &ExportableBusAccess::portName() const
{
    return m_portName;
}

void ExportableBusAccess::setPortName(const QString &name)
{
    m_portName = name;
}

const QString &ExportableBusAccess::busName() const
{
    return m_busName;
}

void ExportableBusAccess::setBusName(const QString &name)
{
    m_busName = name;
}

} // namespace dve
