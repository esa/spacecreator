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

#include "interfaceviewoptimizer.h"

#include <QBuffer>
#include <QSaveFile>
#include <conversion/common/translation/exceptions.h>
#include <iostream>
#include <ivcore/ivconnection.h>
#include <ivcore/ivfunction.h>
#include <libiveditor/ivexporter.h>

using ivm::IVConnection;
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVModel;
using ivm::IVObject;
using ivm::meta::Props;

using conversion::translator::TranslationException;

namespace tmc {

void InterfaceViewOptimizer::optimizeModel(
        IVModel *ivModel, const std::vector<QString> &functionNames, InterfaceViewOptimizer::Mode mode)
{
    for (auto obj : ivModel->objects()) {
        auto func = dynamic_cast<IVFunction *>(obj);

        if (func == nullptr) {
            continue;
        }

        const auto &funcFound = std::find(functionNames.begin(), functionNames.end(), func->title());

        switch (mode) {
        case Mode::Environment:
            if (funcFound != functionNames.end()) {
                markAsEnvironment(func);
            }
            break;
        case Mode::Keep:
            if (funcFound == functionNames.end()) {
                markAsEnvironment(func);
            }
            break;
        }
    }

    /* removeDeadConnections(ivModel); */
    /* removeDeadInterfaces(ivModel); */
    /* removeDeadFunctions(ivModel); */
}

void InterfaceViewOptimizer::markAsEnvironment(IVFunction *function)
{
    const auto &currentDefaultImplementationName = function->defaultImplementation();
    const auto &currentDefaultImplementationType = findImplementationType(currentDefaultImplementationName, function);

    if (currentDefaultImplementationType.toLower() == "sdl") {
        setGuiAsDefaultImplementation(function);
    }
}

void InterfaceViewOptimizer::setGuiAsDefaultImplementation(IVFunction *function)
{
    if (function->hasImplementationName(m_environmentImplementationName)) {
        const auto environmentImplementationType = findImplementationType(m_environmentImplementationName, function);

        if (environmentImplementationType.toUpper() != m_environmentImplementationType) {
            auto errorMessage = QString("Function '%1' already has implementation named '%2' but it's not of type '%3'")
                                        .arg(function->title())
                                        .arg(m_environmentImplementationName)
                                        .arg(m_environmentImplementationType);
            throw TranslationException(std::move(errorMessage));
        }
    } else {
        function->addImplementation(m_environmentImplementationName, m_environmentImplementationType);
    }

    function->setDefaultImplementation(m_environmentImplementationName);

    // GUI interfaces cannot have any cyclic interfaces
    removeUnallowedInterfaces(function);
}

QString InterfaceViewOptimizer::findImplementationType(const QString &implementationName, const IVFunction *function)
{
    for (const auto &impl : function->implementations()) {
        if (impl.name() == implementationName) {
            return impl.value().toString();
        }
    }

    return "";
}

void InterfaceViewOptimizer::removeDeadConnections(IVModel *ivModel)
{
    std::vector<IVObject *> objectsToRemove;

    for (auto obj : ivModel->objects()) {
        auto connection = dynamic_cast<IVConnection *>(obj);

        if (connection == nullptr) {
            continue;
        }

        if (isConnectionDead(connection)) {
            objectsToRemove.push_back(connection);
            objectsToRemove.push_back(connection->sourceInterface());
            objectsToRemove.push_back(connection->targetInterface());
        }
    }

    for (const auto obj : objectsToRemove) {
        ivModel->removeObject(obj);
    }
}

void InterfaceViewOptimizer::removeDeadInterfaces(IVModel *ivModel)
{
    std::vector<IVInterface *> interfacesToRemove;

    for (auto obj : ivModel->objects()) {
        auto interface = dynamic_cast<IVInterface *>(obj);

        if (interface == nullptr) {
            continue;
        }

        if (isInterfaceDead(interface, ivModel)) {
            interfacesToRemove.push_back(interface);
        }
    }

    for (const auto interface : interfacesToRemove) {
        ivModel->removeObject(interface);
    }
}

void InterfaceViewOptimizer::removeDeadFunctions(IVModel *ivModel)
{
    std::vector<IVFunction *> functionsToRemove;

    for (auto obj : ivModel->objects()) {
        auto func = dynamic_cast<IVFunction *>(obj);

        if (func == nullptr) {
            continue;
        }

        if (isFunctionDead(func)) {
            functionsToRemove.push_back(func);
        }
    }

    for (const auto func : functionsToRemove) {
        ivModel->removeObject(func);
    }
}

void InterfaceViewOptimizer::removeUnallowedInterfaces(IVFunction *function)
{
    std::vector<IVInterface *> interfacesToRemove;

    for (const auto interface : function->interfaces()) {
        if (interface->kind() != IVInterface::OperationKind::Sporadic) {
            interfacesToRemove.push_back(interface);
        }
    }

    for (const auto interface : interfacesToRemove) {
        function->removeChild(interface);
    }
}

bool InterfaceViewOptimizer::isConnectionDead(const IVConnection *connection)
{
    const auto source = connection->source();
    const auto target = connection->target();

    if (source == nullptr || target == nullptr) {
        return true;
    }

    if (source->type() != IVObject::Type::Function || target->type() != IVObject::Type::Function) {
        return false;
    }

    const auto sourceFunction = dynamic_cast<IVFunction *>(source);
    const auto targetFunction = dynamic_cast<IVFunction *>(target);

    return !isSdlFunction(sourceFunction) && !isSdlFunction(targetFunction);
}

bool InterfaceViewOptimizer::isInterfaceDead(const IVInterface *interface, const IVModel *ivModel)
{
    const auto connection = ivModel->getConnectionForIface(interface->id());

    if (connection == nullptr) {
        return true;
    } else {
        return false;
    }
}

bool InterfaceViewOptimizer::isFunctionDead(const IVFunction *function)
{
    // Function in considered 'dead' if it has no non-cyclic interfaces
    for (const auto interface : function->interfaces()) {
        if (interface->kind() != IVInterface::OperationKind::Cyclic) {
            return false;
        }
    }

    return true;
}

bool InterfaceViewOptimizer::isSdlFunction(const IVFunction *function)
{
    const QString defaultImplementation = function->defaultImplementation();
    for (const auto &impl : function->implementations()) { // NOLINT(readability-use-anyofallof)
        if (impl.name() == defaultImplementation && impl.value().type() == QVariant::Type::String
                && impl.value().toString().toLower() == "sdl") {
            return true;
        }
    }
    return false;
}

} // namespace tmc
