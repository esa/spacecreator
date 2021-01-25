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

#include "exportedaadlfunction.h"

#include "tab_aadl/aadlcomment.h"
#include "tab_aadl/aadlconnection.h"
#include "tab_aadl/aadlfunction.h"
#include "tab_aadl/aadlfunctiontype.h"
#include "tab_aadl/aadliface.h"

namespace taste3 {
namespace templating {

ExportedAADLFunction::ExportedAADLFunction(const aadl::AADLFunctionType *function)
    : ExportedAADLObject(function)
{
}

QVariantList ExportedAADLFunction::interfaces() const
{
    QVariantList ifaces;
    for (const auto iface : exportedObject<aadl::AADLFunctionType>()->interfaces())
        ifaces << createFrom(iface);
    return ifaces;
}

QVariantList ExportedAADLFunction::functions() const
{
    QVariantList functions;
    const aadl::AADLFunctionType *o = exportedObject<aadl::AADLFunctionType>();
    for (const auto function : o->functionTypes())
        functions << createFrom(function);
    for (const auto function : o->functions())
        functions << createFrom(function);
    return functions;
}

QVariantList ExportedAADLFunction::comments() const
{
    QVariantList comments;
    for (const auto comment : exportedObject<aadl::AADLFunctionType>()->comments())
        comments << createFrom(comment);
    return comments;
}

QVariantList ExportedAADLFunction::connections() const
{
    QVariantList connections;
    for (const auto connection : exportedObject<aadl::AADLFunctionType>()->connections())
        connections << createFrom(connection);
    return connections;
}

} // ns templating
} // ns taste3
