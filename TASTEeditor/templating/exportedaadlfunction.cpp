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

#include "tab_aadl/aadlobjectfunctiontype.h"
#include "tab_aadl/aadlobjectfunction.h"
#include "tab_aadl/aadlobjectiface.h"
#include "tab_aadl/aadlobjectcomment.h"
#include "tab_aadl/aadlobjectconnection.h"

namespace taste3 {
namespace templating {

ExportedAADLFunction::ExportedAADLFunction(const aadl::AADLObject *function)
    : ExportedAADLObject(function) {}

QVariantList ExportedAADLFunction::interfaces() const
{
    QVariantList ifaces;
    for (const auto iface : exportedObject<aadl::AADLObjectFunctionType>()->interfaces())
        ifaces << createFrom(iface);
    return ifaces;
}

QVariantList ExportedAADLFunction::functions() const
{
    QVariantList functions;
    const aadl::AADLObjectFunctionType *o = exportedObject<aadl::AADLObjectFunctionType>();
    for (const auto function : o->functionTypes())
        functions << createFrom(function);
    for (const auto function : o->functions())
        functions << createFrom(function);
    return functions;
}

QVariantList ExportedAADLFunction::comments() const
{
    QVariantList comments;
    for (const auto comment : exportedObject<aadl::AADLObjectFunctionType>()->comments())
        comments << createFrom(comment);
    return comments;
}

QVariantList ExportedAADLFunction::connections() const
{
    QVariantList connections;
    for (const auto connection : exportedObject<aadl::AADLObjectFunctionType>()->connections())
        connections << createFrom(connection);
    return connections;
}

} // ns templating
} // ns taste3

