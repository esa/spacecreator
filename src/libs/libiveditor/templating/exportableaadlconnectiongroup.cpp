/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "exportableaadlconnectiongroup.h"

#include "aadlobjectconnectiongroup.h"

#include <QMetaEnum>

namespace aadlinterface {

ExportableAADLConnectionGroup::ExportableAADLConnectionGroup(const aadl::AADLObjectConnectionGroup *connection)
    : ExportableAADLObject(connection)
{
}

QString ExportableAADLConnectionGroup::connectionGroupName() const
{
    return exportedObject<aadl::AADLObjectConnectionGroup>()->title();
}

QString ExportableAADLConnectionGroup::sourceName() const
{
    return exportedObject<aadl::AADLObjectConnectionGroup>()->sourceName();
}

QString ExportableAADLConnectionGroup::targetName() const
{
    return exportedObject<aadl::AADLObjectConnectionGroup>()->targetName();
}

QVariantMap ExportableAADLConnectionGroup::sourceInterfaces() const
{
    static const QMetaEnum &typeMetaEnum = QMetaEnum::fromType<aadl::AADLObject::Type>();

    QVariantMap interfaceNames;
    for (const auto interface : exportedObject<aadl::AADLObjectConnectionGroup>()->groupedSourceInterfaces()) {
        const QString ifaceType = typeMetaEnum.valueToKey(static_cast<int>(interface->aadlType()));
        interfaceNames[ifaceType] = interface->title();
    }
    return interfaceNames;
}

QVariantMap ExportableAADLConnectionGroup::targetInterfaces() const
{
    static const QMetaEnum &typeMetaEnum = QMetaEnum::fromType<aadl::AADLObject::Type>();

    QVariantMap interfaceNames;
    for (const auto interface : exportedObject<aadl::AADLObjectConnectionGroup>()->groupedTargetInterfaces()) {
        const QString ifaceType = typeMetaEnum.valueToKey(static_cast<int>(interface->aadlType()));
        interfaceNames[ifaceType] = interface->title();
    }
    return interfaceNames;
}

} // namespace aadlinterface
