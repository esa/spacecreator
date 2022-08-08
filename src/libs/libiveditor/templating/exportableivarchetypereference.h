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

#pragma once

#include "exportableivobject.h"

namespace ivm {
class IVArchetypeReference;
}

namespace ive {

/**
 * @brief The IVArchetypeReference is a class to export IVArchetypeReference
 */
class ExportableIVArchetypeReference : public ExportableIVObject
{
    Q_GADGET
    Q_PROPERTY(QString archetypeLibrary READ archetypeLibraryName)
    Q_PROPERTY(QString archetypeFunction READ archetypeFunctionName)

public:
    explicit ExportableIVArchetypeReference(const ivm::IVArchetypeReference *archetypeReference = nullptr);
    QString archetypeLibraryName() const;
    QString archetypeFunctionName() const;
};

}

Q_DECLARE_METATYPE(ive::ExportableIVArchetypeReference)
Q_DECLARE_TYPEINFO(ive::ExportableIVArchetypeReference, Q_MOVABLE_TYPE);
