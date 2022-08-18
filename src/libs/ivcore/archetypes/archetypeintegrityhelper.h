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

#include <QObject>

namespace shared {
class VEObject;
class InterfaceParameter;
}

namespace ivm {

class ArchetypeModel;
class InterfaceArchetype;
class ParameterArchetype;
class IVFunctionType;
class IVInterface;

class ArchetypeIntegrityHelper : public QObject
{
    Q_OBJECT
public:
    ArchetypeIntegrityHelper() = delete;

    static void checkArchetypeIntegrity(QList<shared::VEObject *> ivObjects, ivm::ArchetypeModel *archetypesModel);

private:
    static void checkFunctionArchetypeIntegrity(ivm::IVFunctionType *function, ivm::ArchetypeModel *archetypesModel);
    static bool checkInterfaceArchetypeIntegrity(
            ivm::IVInterface *interface, ivm::InterfaceArchetype *interfaceArchetype);
    static bool checkParameterArchetypeIntegrity(
            const shared::InterfaceParameter &parameter, ivm::ParameterArchetype *parameterArchetype);
};

}
