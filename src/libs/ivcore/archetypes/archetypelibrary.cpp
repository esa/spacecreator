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

#include "archetypelibrary.h"

namespace ivm {

ArchetypeLibrary::ArchetypeLibrary(const QString &title, QObject *parent)
    : ArchetypeObject(title, ArchetypeObject::Type::ArchetypeLibrary, parent)
{
}

ArchetypeLibrary::~ArchetypeLibrary() = default;

QVector<FunctionArchetype *> ArchetypeLibrary::getFunctions() const
{
    return m_functions;
}

bool ArchetypeLibrary::aboutToBeRemoved()
{
    for (auto function : m_functions) {
        function->setParentObject(nullptr);
    }
    m_functions.clear();
    return true;
}

void ArchetypeLibrary::addFunction(FunctionArchetype *functionArchetype)
{
    if (!functionArchetype) {
        return;
    }
    functionArchetype->setParentObject(this);
    m_functions.append(functionArchetype);
}

void ArchetypeLibrary::removeFunction(FunctionArchetype *functionArchetype)
{
    if (!functionArchetype) {
        return;
    }
    functionArchetype->setParentObject(nullptr);
    m_functions.removeAll(functionArchetype);
}

QStringList ArchetypeLibrary::getFunctionsNames()
{
    QStringList functionsNames;

    for (auto function : getFunctions()) {
        functionsNames.append(function->title());
    }

    return functionsNames;
}
}
