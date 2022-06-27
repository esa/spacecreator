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
using ivm::IVFunctionType;
using ivm::IVInterface;
using ivm::IVModel;
using ivm::IVObject;
using ivm::meta::Props;

using conversion::translator::TranslationException;

namespace tmc {

void InterfaceViewOptimizer::optimizeModel(
        IVModel *ivModel, const std::vector<QString> &functionNames, InterfaceViewOptimizer::Mode mode)
{
    flattenModel(ivModel);

    for (auto obj : ivModel->objects()) {
        if (!isFunction(obj)) {
            continue;
        }

        auto function = obj->as<IVFunction *>();

        const auto &functionFound = std::find(functionNames.begin(), functionNames.end(), function->title());

        switch (mode) {
        case Mode::Environment:
            if (functionFound != functionNames.end()) {
                markAsEnvironment(function);
            }
            break;
        case Mode::Keep:
            if (functionFound == functionNames.end()) {
                markAsEnvironment(function);
            }
            break;
        case Mode::None:
            break;
        }
    }

    removeDeadConnections(ivModel);
    removeDeadInterfaces(ivModel);
    removeDeadFunctions(ivModel);
}

void InterfaceViewOptimizer::flattenModel(IVModel *ivModel)
{
    auto objects = ivModel->objects();
    std::vector<IVFunctionType *> functionsToRemove;

    for (auto obj : objects) {
        if (!isFunctionType(obj)) {
            continue;
        }

        auto function = obj->as<IVFunctionType *>();

        if (isFunctionParent(function)) {
            functionsToRemove.push_back(function);
        } else {
            flattenConnections(function, ivModel);
        }
    }

    for (auto function : functionsToRemove) {
        const auto &connections = ivModel->getConnectionsForFunction(function->id());
        for (auto connection : connections) {
            ivModel->removeObject(connection);
        }

        ivModel->removeObject(function);
    }
}

void InterfaceViewOptimizer::flattenConnections(IVFunctionType *function, IVModel *ivModel)
{
    const auto &connections = ivModel->getConnectionsForFunction(function->id());

    for (auto connection : connections) {
        if (!shouldFlattenConnection(connection, function)) {
            continue;
        }

        const auto sourceInterface = connection->sourceInterface();
        auto lastConnection = findLastConnection(connection, ivModel);
        const auto targetInterface = lastConnection->targetInterface();
        ivModel->removeObject(lastConnection);

        auto newConnection = new IVConnection(sourceInterface, targetInterface);
        ivModel->addObject(newConnection);
    }
}

bool InterfaceViewOptimizer::shouldFlattenConnection(IVConnection *connection, IVFunctionType *function)
{
    if (connection == nullptr) {
        return false;
    }

    if (connection->sourceName() != function->title()) {
        return false;
    }

    const auto target = connection->target();
    if (isFunctionType(target)) {
        const auto targetFunc = target->as<const IVFunctionType *>();
        if (isFunctionParent(targetFunc)) {
            return true;
        }
    }

    return false;
}

IVConnection *InterfaceViewOptimizer::findLastConnection(IVConnection *connection, IVModel *ivModel)
{
    auto target = connection->target()->as<IVFunctionType *>();

    if (isFunctionParent(target)) {
        const auto &targetName = connection->targetName();
        const auto &targetConnections = ivModel->getConnectionsForIface(connection->targetInterface()->id());

        const auto foundNextConnection = std::find_if(targetConnections.begin(), targetConnections.end(),
                [&](const auto conn) { return conn->sourceName() == targetName; });

        ivModel->removeObject(connection);

        return findLastConnection(*foundNextConnection, ivModel);
    } else {
        return connection;
    }
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
        if (obj->as<IVObject *>()->type() != IVObject::Type::Connection) {
            continue;
        }

        auto connection = obj->as<IVConnection *>();

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
        if (obj->as<IVObject *>()->type() != IVObject::Type::ProvidedInterface
                && obj->as<IVObject *>()->type() != IVObject::Type::RequiredInterface) {
            continue;
        }

        auto interface = obj->as<IVInterface *>();

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
    std::vector<IVFunctionType *> functionsToRemove;

    for (auto obj : ivModel->objects()) {
        if (!isFunctionType(obj)) {
            continue;
        }

        auto function = obj->as<IVFunctionType *>();

        if (isFunctionDead(function)) {
            functionsToRemove.push_back(function);
        }
    }

    for (const auto function : functionsToRemove) {
        ivModel->removeObject(function);
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

bool InterfaceViewOptimizer::isFunction(const shared::VEObject *object)
{
    auto ivObject = object->as<const IVObject *>();

    return ivObject->type() == IVObject::Type::Function;
}

bool InterfaceViewOptimizer::isFunctionType(const shared::VEObject *object)
{
    auto ivObject = object->as<const IVObject *>();

    return ivObject->type() == IVObject::Type::Function || ivObject->type() == IVObject::Type::FunctionType;
}

bool InterfaceViewOptimizer::isConnectionDead(const IVConnection *connection)
{
    const auto source = connection->source();
    const auto target = connection->target();

    if (source == nullptr || target == nullptr) {
        return true;
    }

    if (!isFunctionType(source)) {
        return true;
    }
    const auto sourceFunction = source->as<IVFunctionType *>();

    if (!isFunctionType(target)) {
        return true;
    }
    const auto targetFunction = target->as<IVFunctionType *>();

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

bool InterfaceViewOptimizer::isFunctionDead(const IVFunctionType *function)
{
    // Function in considered 'dead' if it has no non-cyclic interfaces
    for (const auto interface : function->interfaces()) {
        if (interface->kind() != IVInterface::OperationKind::Cyclic) {
            return false;
        }
    }

    return true;
}

bool InterfaceViewOptimizer::isFunctionParent(const IVFunctionType *function)
{
    return !function->functions().empty();
}

bool InterfaceViewOptimizer::isSdlFunction(const IVFunctionType *functionType)
{
    if (functionType->type() != IVObject::Type::Function) {
        return false;
    }

    const auto function = functionType->as<const IVFunction *>();

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
