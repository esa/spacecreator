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

#include "exportableaadliface.h"

#include "ivinterface.h"
#include "ivinterfacechain.h"

namespace ive {

ExportableAADLIface::ExportableAADLIface(const ivm::IVInterface *iface)
    : ExportableAADLObject(iface)
{
}

bool ExportableAADLIface::isProvided() const
{
    return exportedObject<ivm::IVInterface>()->isProvided();
}

bool ExportableAADLIface::isRequired() const
{
    return exportedObject<ivm::IVInterface>()->isRequired();
}

QVariantList ExportableAADLIface::paramList() const
{
    QVariantList list;
    for (const ivm::InterfaceParameter &param : exportedObject<ivm::IVInterface>()->params())
        list << QVariant::fromValue(param);
    return list;
}

QVariantList ExportableAADLIface::connectedInterfaces() const
{
    QVariantList connectedInterfaces;
    auto iface = exportedObject<ivm::IVInterface>();
    const QList<ivm::IVInterfaceChain> chains = ivm::IVInterfaceChain::build(iface);
    for (const ivm::IVInterfaceChain &chain : chains) {
        connectedInterfaces << chain.targetEndPointPath();
    }
    return connectedInterfaces;
}

}
