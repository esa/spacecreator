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

#include "exportableaadlfunction.h"

#include "aadlinterfacechain.h"
#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectconnectiongroup.h"
#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"

namespace aadlinterface {

ExportableAADLFunction::ExportableAADLFunction(const aadl::AADLObjectFunctionType *function)
    : ExportableAADLObject(function)
{
}

QVariantList ExportableAADLFunction::interfaces() const
{
    QVariantList ifaces;
    for (const auto iface : exportedObject<aadl::AADLObjectFunctionType>()->interfaces())
        ifaces << createFrom(iface);
    return ifaces;
}

QVariantList ExportableAADLFunction::functions() const
{
    QVariantList functions;
    const aadl::AADLObjectFunctionType *o = exportedObject<aadl::AADLObjectFunctionType>();
    for (const auto function : o->functionTypes())
        functions << createFrom(function);
    for (const auto function : o->functions())
        functions << createFrom(function);
    return functions;
}

QVariantList ExportableAADLFunction::comments() const
{
    QVariantList comments;
    for (const auto comment : exportedObject<aadl::AADLObjectFunctionType>()->comments())
        comments << createFrom(comment);
    return comments;
}

QVariantList ExportableAADLFunction::connections() const
{
    QVariantList connections;
    for (const auto connection : exportedObject<aadl::AADLObjectFunctionType>()->connections())
        connections << createFrom(connection);
    return connections;
}

QVariantList ExportableAADLFunction::connectionGroups() const
{
    QVariantList connectionGroups;
    for (const auto connectionGroup : exportedObject<aadl::AADLObjectFunctionType>()->connectionGroups())
        connectionGroups << createFrom(connectionGroup);
    return connectionGroups;
}

QVariantList ExportableAADLFunction::connectedFunctions() const
{
    const auto aadlFunction = exportedObject<aadl::AADLObjectFunction>();
    QVariantList list;
    for (auto chain : aadl::AADLInterfaceChain::linkedFunctions(aadlFunction)) {
        list << qVariantFromValue(chain);
    }
    return list;
}

}
