/*
  Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "functionarchetype.h"

namespace ivm {

struct FunctionArchetypePrivate {
    explicit FunctionArchetypePrivate() {}
    QVector<InterfaceArchetype *> m_interfaces;
};

FunctionArchetype::FunctionArchetype(const QString &title, QObject *parent)
    : ArchetypeObject(title, ArchetypeObject::Type::FunctionArchetype, parent)
    , m_functionPrivate(std::make_unique<FunctionArchetypePrivate>())
{
}

FunctionArchetype::~FunctionArchetype() = default;

QVector<InterfaceArchetype *> FunctionArchetype::getInterfaces() const
{
    return m_functionPrivate->m_interfaces;
}

void FunctionArchetype::addInterface(InterfaceArchetype *interfaceArchetype)
{
    if (!interfaceArchetype) {
        return;
    }
    interfaceArchetype->setParentObject(this);
    m_functionPrivate->m_interfaces.append(interfaceArchetype);
}

void FunctionArchetype::removeInterface(InterfaceArchetype *interfaceArchetype)
{
    if (!interfaceArchetype) {
        return;
    }
    interfaceArchetype->setParentObject(nullptr);
    m_functionPrivate->m_interfaces.removeAll(interfaceArchetype);
}
}
