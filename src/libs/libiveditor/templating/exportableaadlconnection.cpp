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

#include "exportableaadlconnection.h"

#include "aadlobjectconnection.h"

namespace ive {

ExportableAADLConnection::ExportableAADLConnection(const ivm::AADLObjectConnection *connection)
    : ExportableAADLObject(connection)
{
}

QString ExportableAADLConnection::sourceName() const
{
    return exportedObject<ivm::AADLObjectConnection>()->sourceName();
}

QString ExportableAADLConnection::targetName() const
{
    return exportedObject<ivm::AADLObjectConnection>()->targetName();
}

QString ExportableAADLConnection::sourceInterfaceName() const
{
    return exportedObject<ivm::AADLObjectConnection>()->sourceInterfaceName();
}

QString ExportableAADLConnection::targetInterfaceName() const
{
    return exportedObject<ivm::AADLObjectConnection>()->targetInterfaceName();
}

bool ExportableAADLConnection::sourceInterfaceIsRequired() const
{
    auto o = exportedObject<ivm::AADLObjectConnection>();
    return o->sourceInterface() ? o->sourceInterface()->isRequired() : false;
}

bool ExportableAADLConnection::sourceInterfaceIsProvided() const
{
    auto o = exportedObject<ivm::AADLObjectConnection>();
    return o->sourceInterface() ? o->sourceInterface()->isProvided() : false;
}

bool ExportableAADLConnection::targetInterfaceIsRequired() const
{
    auto o = exportedObject<ivm::AADLObjectConnection>();
    return o->targetInterface() ? o->targetInterface()->isRequired() : false;
}

bool ExportableAADLConnection::targetInterfaceIsProvided() const
{
    auto o = exportedObject<ivm::AADLObjectConnection>();
    return o->targetInterface() ? o->targetInterface()->isProvided() : false;
}

}
