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

#include "aadlcomment.h"
#include "aadlconnection.h"
#include "aadlconnectiongroup.h"
#include "aadlfunction.h"
#include "aadlfunctiontype.h"
#include "aadliface.h"
#include "aadlinterfacechain.h"
#include "aadlmodel.h"

namespace ive {

ExportableAADLFunction::ExportableAADLFunction(const ivm::AADLFunctionType *function)
    : ExportableAADLObject(function)
{
}

QVariantList ExportableAADLFunction::interfaces() const
{
    QVariantList ifaces;
    for (const auto iface : exportedObject<ivm::AADLFunctionType>()->interfaces())
        ifaces << createFrom(iface);
    return ifaces;
}

QVariantList ExportableAADLFunction::functions() const
{
    QVariantList functions;
    const ivm::AADLFunctionType *o = exportedObject<ivm::AADLFunctionType>();
    for (const auto function : o->functionTypes())
        functions << createFrom(function);
    for (const auto function : o->functions())
        functions << createFrom(function);
    return functions;
}

QVariantList ExportableAADLFunction::comments() const
{
    QVariantList comments;
    for (const auto comment : exportedObject<ivm::AADLFunctionType>()->comments())
        comments << createFrom(comment);
    return comments;
}

QVariantList ExportableAADLFunction::connections() const
{
    QVariantList connections;
    for (const auto connection : exportedObject<ivm::AADLFunctionType>()->connections())
        connections << createFrom(connection);
    return connections;
}

QVariantList ExportableAADLFunction::connectionGroups() const
{
    QVariantList connectionGroups;
    for (const auto connectionGroup : exportedObject<ivm::AADLFunctionType>()->connectionGroups())
        connectionGroups << createFrom(connectionGroup);
    return connectionGroups;
}

QVariantList ExportableAADLFunction::connectedFunctions() const
{
    const auto aadlFunction = exportedObject<ivm::AADLFunctionType>();
    QVariantList list;
    for (auto chain : ivm::AADLInterfaceChain::linkedFunctions(aadlFunction)) {
        list << qVariantFromValue(chain);
    }
    return list;
}

QVariantList ExportableAADLFunction::contextParameters() const
{
    QVariantList parameters;
    for (const ivm::ContextParameter &param : exportedObject<ivm::AADLFunctionType>()->contextParams()) {
        parameters << qVariantFromValue(param);
    }
    return parameters;
}

}
