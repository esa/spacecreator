/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "exportableivconnection.h"

#include "ivconnection.h"

namespace ive {

ExportableIVConnection::ExportableIVConnection(const ivm::IVConnection *connection)
    : ExportableIVObject(connection)
{
}

QString ExportableIVConnection::sourceName() const
{
    return exportedObject<ivm::IVConnection>()->sourceName();
}

QString ExportableIVConnection::targetName() const
{
    return exportedObject<ivm::IVConnection>()->targetName();
}

QString ExportableIVConnection::sourceInterfaceName() const
{
    return exportedObject<ivm::IVConnection>()->sourceInterfaceName();
}

QString ExportableIVConnection::targetInterfaceName() const
{
    return exportedObject<ivm::IVConnection>()->targetInterfaceName();
}

bool ExportableIVConnection::sourceInterfaceIsRequired() const
{
    auto o = exportedObject<ivm::IVConnection>();
    return o->sourceInterface() ? o->sourceInterface()->isRequired() : false;
}

bool ExportableIVConnection::sourceInterfaceIsProvided() const
{
    auto o = exportedObject<ivm::IVConnection>();
    return o->sourceInterface() ? o->sourceInterface()->isProvided() : false;
}

bool ExportableIVConnection::targetInterfaceIsRequired() const
{
    auto o = exportedObject<ivm::IVConnection>();
    return o->targetInterface() ? o->targetInterface()->isRequired() : false;
}

bool ExportableIVConnection::targetInterfaceIsProvided() const
{
    auto o = exportedObject<ivm::IVConnection>();
    return o->targetInterface() ? o->targetInterface()->isProvided() : false;
}

}
