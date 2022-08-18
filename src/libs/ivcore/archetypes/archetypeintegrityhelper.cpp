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

#include "archetypeintegrityhelper.h"

#include "archetypelibrary.h"
#include "archetypemodel.h"
#include "archetypeobject.h"
#include "errorhub.h"
#include "functionarchetype.h"
#include "interfacearchetype.h"
#include "ivarchetypereference.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivobject.h"
#include "parameterarchetype.h"
#include "shared/parameter.h"
#include "shared/veobject.h"

namespace ivm {

void ArchetypeIntegrityHelper::checkArchetypeIntegrity(
        QList<shared::VEObject *> ivObjects, ivm::ArchetypeModel *archetypesModel)
{
    for (auto object : ivObjects) {
        if (auto functionType = object->as<ivm::IVFunctionType *>()) {
            checkFunctionArchetypeIntegrity(functionType, archetypesModel);
        }
    }
}

void ArchetypeIntegrityHelper::checkFunctionArchetypeIntegrity(
        ivm::IVFunctionType *function, ivm::ArchetypeModel *archetypesModel)
{
    for (auto reference : function->archetypeReferences()) {

        ivm::ArchetypeObject *archetypeObject = archetypesModel->getObjectByName(
                reference->getLibraryName(), ivm::ArchetypeObject::Type::ArchetypeLibrary);
        ivm::ArchetypeLibrary *archetypeLibrary = archetypeObject->as<ivm::ArchetypeLibrary *>();
        if (archetypeLibrary == nullptr) {
            continue;
        }

        ivm::FunctionArchetype *functionArchetype = archetypeLibrary->getFunctionByName(reference->getFunctionName());
        if (functionArchetype == nullptr) {
            continue;
        }

        for (auto interfaceArchetype : functionArchetype->getInterfaces()) {
            bool contains = false;
            for (auto interface : function->interfaces()) {
                if (checkInterfaceArchetypeIntegrity(interface, interfaceArchetype)) {
                    contains = true;
                }
            }
            if (!contains) {
                const QString msg("The archetype interface %1::%2 is not implemented in function %3");
                shared::ErrorHub::addError(shared::ErrorItem::Warning,
                        msg.arg(functionArchetype->title(), interfaceArchetype->title(), function->title()));
            }
        }
    }
}

bool ArchetypeIntegrityHelper::checkInterfaceArchetypeIntegrity(
        ivm::IVInterface *interface, ivm::InterfaceArchetype *interfaceArchetype)
{

    if (interface->title() != interfaceArchetype->title()) {
        return false;
    }

    switch (interfaceArchetype->getInterfaceType()) {
    case ivm::InterfaceArchetype::InterfaceType::PROVIDED:
        if (interface->direction() != ivm::IVInterface::InterfaceType::Provided) {
            return false;
        }
        break;
    case ivm::InterfaceArchetype::InterfaceType::REQUIRED:
        if (interface->direction() != ivm::IVInterface::InterfaceType::Required) {
            return false;
        }
        break;
    }

    switch (interfaceArchetype->getOperationKind()) {
    case ivm::InterfaceArchetype::OperationKind::CYCLIC:
        if (interface->kind() != ivm::IVInterface::OperationKind::Cyclic) {
            return false;
        }
        break;
    case ivm::InterfaceArchetype::OperationKind::SPORADIC:
        if (interface->kind() != ivm::IVInterface::OperationKind::Sporadic) {
            return false;
        }
        break;
    case ivm::InterfaceArchetype::OperationKind::PROTECTED:
        if (interface->kind() != ivm::IVInterface::OperationKind::Protected) {
            return false;
        }
        break;
    case ivm::InterfaceArchetype::OperationKind::UNPROTECTED:
        if (interface->kind() != ivm::IVInterface::OperationKind::Unprotected) {
            return false;
        }
        break;
    }

    if (interface->layerName() != interfaceArchetype->getLayer()) {
        return false;
    }

    for (auto parameterArchetype : interfaceArchetype->getParameters()) {
        bool contains = false;
        for (auto parameter : interface->params()) {
            if (checkParameterArchetypeIntegrity(parameter, parameterArchetype)) {
                contains = true;
            }
        }
        if (!contains) {
            return false;
        }
    }

    return true;
}

bool ArchetypeIntegrityHelper::checkParameterArchetypeIntegrity(
        const shared::InterfaceParameter &parameter, ivm::ParameterArchetype *parameterArchetype)
{
    if (parameter.name() != parameterArchetype->title()) {
        return false;
    }

    if (parameter.paramTypeName() != parameterArchetype->getType()) {
        return false;
    }

    switch (parameterArchetype->getDirection()) {
    case ivm::ParameterArchetype::ParameterDirection::IN:
        if (parameter.direction() != shared::InterfaceParameter::Direction::IN) {
            return false;
        }
        break;
    case ivm::ParameterArchetype::ParameterDirection::OUT:
        if (parameter.direction() != shared::InterfaceParameter::Direction::OUT) {
            return false;
        }
        break;
    }

    return true;
}
}
