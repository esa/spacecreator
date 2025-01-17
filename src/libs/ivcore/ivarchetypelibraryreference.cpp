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

#include "ivarchetypelibraryreference.h"

namespace ivm {

IVArchetypeLibraryReference::IVArchetypeLibraryReference(
        const QString &archetypeLibraryName, const QString &archetypeLibraryPath, QObject *parent)
    : IVObject(IVObject::Type::ArchetypeLibraryReference, parent)
    , m_archetypeLibraryName(archetypeLibraryName)
    , m_archetypeLibraryPath(archetypeLibraryPath)
{
}

QString IVArchetypeLibraryReference::getLibraryName() const
{
    return m_archetypeLibraryName;
}

void IVArchetypeLibraryReference::setLibraryName(const QString &libraryName)
{
    m_archetypeLibraryName = libraryName;
}

QString IVArchetypeLibraryReference::getLibraryPath() const
{
    return m_archetypeLibraryPath;
}

void IVArchetypeLibraryReference::setLibraryPath(const QString &libraryPath)
{
    m_archetypeLibraryPath = libraryPath;
}

bool IVArchetypeLibraryReference::operator==(const IVArchetypeLibraryReference &other) const
{
    return m_archetypeLibraryName == other.getLibraryName()
            || m_archetypeLibraryPath == other.getLibraryPath();
}
}
