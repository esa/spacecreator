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

#include "exportabledvmessage.h"

#include "dvmessage.h"

namespace dve {

ExportableDVMessage::ExportableDVMessage(const dvm::DVObject *dvObject)
    : ExportableDVObject(dvObject)
{
}

QString ExportableDVMessage::fromFunction() const
{
    return exportedObject<dvm::DVMessage>()->fromFunction();
}

QStringList ExportableDVMessage::fromFunctionPath() const
{
    return exportedObject<dvm::DVMessage>()->fromFunctionPath();
}

QString ExportableDVMessage::fromInterface() const
{
    return exportedObject<dvm::DVMessage>()->fromInterface();
}

QString ExportableDVMessage::toFunction() const
{
    return exportedObject<dvm::DVMessage>()->toFunction();
}

QStringList ExportableDVMessage::toFunctionPath() const
{
    return exportedObject<dvm::DVMessage>()->toFunctionPath();
}

QString ExportableDVMessage::toInterface() const
{
    return exportedObject<dvm::DVMessage>()->toInterface();
}

QString ExportableDVMessage::resolvedTargetFunction() const
{
    return exportedObject<dvm::DVMessage>()->resolvedTargetFunction();
}

QString ExportableDVMessage::resolvedTargetInterface() const
{
    return exportedObject<dvm::DVMessage>()->resolvedTargetInterface();
}

} // namespace dve
