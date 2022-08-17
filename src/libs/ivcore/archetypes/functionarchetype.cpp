/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "functionarchetype.h"

namespace ivm {

FunctionArchetype::FunctionArchetype(const QString &title, QObject *parent)
    : ArchetypeObject(title, ArchetypeObject::Type::FunctionArchetype, parent)
{
}

FunctionArchetype::~FunctionArchetype() = default;

bool FunctionArchetype::aboutToBeRemoved()
{
    for (auto interface : m_interfaces) {
        interface->setParentObject(nullptr);
    }
    m_interfaces.clear();

    return true;
}

QVector<InterfaceArchetype *> FunctionArchetype::getInterfaces() const
{
    return m_interfaces;
}

void FunctionArchetype::addInterface(InterfaceArchetype *interfaceArchetype)
{
    if (!interfaceArchetype) {
        return;
    }
    interfaceArchetype->setParentObject(this);
    m_interfaces.append(interfaceArchetype);
}

void FunctionArchetype::removeInterface(InterfaceArchetype *interfaceArchetype)
{
    if (!interfaceArchetype) {
        return;
    }
    interfaceArchetype->setParentObject(nullptr);
    m_interfaces.removeAll(interfaceArchetype);
}
}
