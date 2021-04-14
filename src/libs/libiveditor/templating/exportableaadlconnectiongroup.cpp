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

#include "ivconnectiongroup.h"

#include <QMetaEnum>

namespace ive {

ExportableAADLConnectionGroup::ExportableAADLConnectionGroup(const ivm::IVConnectionGroup *connection)
    : ExportableAADLObject(connection)
{
}

QString ExportableAADLConnectionGroup::connectionGroupName() const
{
    return exportedObject<ivm::IVConnectionGroup>()->title();
}

QString ExportableAADLConnectionGroup::sourceName() const
{
    return exportedObject<ivm::IVConnectionGroup>()->sourceName();
}

QString ExportableAADLConnectionGroup::targetName() const
{
    return exportedObject<ivm::IVConnectionGroup>()->targetName();
}

QVariantMap ExportableAADLConnectionGroup::sourceInterfaces() const
{
    static const QMetaEnum &typeMetaEnum = QMetaEnum::fromType<ivm::IVObject::Type>();

    QVariantMap interfaceNames;
    for (const auto interface : exportedObject<ivm::IVConnectionGroup>()->groupedSourceInterfaces()) {
        const QString ifaceType = typeMetaEnum.valueToKey(static_cast<int>(interface->type()));
        interfaceNames[ifaceType] = interface->title();
    }
    return interfaceNames;
}

QVariantMap ExportableAADLConnectionGroup::targetInterfaces() const
{
    static const QMetaEnum &typeMetaEnum = QMetaEnum::fromType<ivm::IVObject::Type>();

    QVariantMap interfaceNames;
    for (const auto interface : exportedObject<ivm::IVConnectionGroup>()->groupedTargetInterfaces()) {
        const QString ifaceType = typeMetaEnum.valueToKey(static_cast<int>(interface->type()));
        interfaceNames[ifaceType] = interface->title();
    }
    return interfaceNames;
}

} // namespace ive
