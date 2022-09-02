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
#include <ivcore/ivcommonprops.h>
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
    auto parentFunctionsInfo = flattenModel(ivModel);

    auto allFunctionNames = resolveFunctionNames(functionNames, parentFunctionsInfo);

    switch (mode) {
    case Mode::Environment:
        discardFunctions(ivModel, allFunctionNames);
        break;
    case Mode::Keep:
        keepFunctions(ivModel, allFunctionNames);
        break;
    case Mode::None:
        break;
    }

    removeDeadConnections(ivModel);
    removeDeadInterfaces(ivModel);
    removeDeadFunctions(ivModel);
}

InterfaceViewOptimizer::ParentFunctionsInfo InterfaceViewOptimizer::flattenModel(IVModel *ivModel)
{
    ParentFunctionsInfo parentFunctionsInfo;

    std::vector<IVFunctionType *> functionsToRemove;
    std::vector<IVConnection *> connectionsToRemove;

    for (auto function : ivModel->allObjectsByType<IVFunctionType>()) {
        if (isFunctionParent(function)) {
            std::vector<QString> children;
            for (auto childFunction : function->functions()) {
                children.push_back(childFunction->title());
            }
            parentFunctionsInfo.insert({ function->title(), std::move(children) });

            functionsToRemove.push_back(function);
        } else {
            flattenConnections(function, connectionsToRemove, ivModel);
        }
    }

    for (auto connection : connectionsToRemove) {
        ivModel->removeObject(connection);
    }

    for (auto function : functionsToRemove) {
        const auto &connections = ivModel->getConnectionsForFunction(function->id());
        for (auto connection : connections) {
            ivModel->removeObject(connection);
        }

        ivModel->removeObject(function);
    }

    return parentFunctionsInfo;
}

void InterfaceViewOptimizer::flattenConnections(
        IVFunctionType *function, std::vector<IVConnection *> &connectionsToRemove, IVModel *ivModel)
{
    const auto &connections = ivModel->getConnectionsForFunction(function->id());

    for (auto connection : connections) {
        if (!shouldFlattenConnection(connection, function)) {
            continue;
        }

        const auto sourceInterface = connection->sourceInterface();
        auto lastConnection = findLastConnection(connection, ivModel);
        const auto targetInterface = lastConnection->targetInterface();

        connectionsToRemove.push_back(lastConnection);

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

void InterfaceViewOptimizer::discardFunctions(IVModel *ivModel, const std::vector<QString> &functionNames)
{
    for (const auto &functionName : functionNames) {
        auto function = ivModel->getFunction(functionName, Qt::CaseInsensitive);

        if (function == nullptr) {
            auto errorMessage = QString("InterfaceView optimizer cannot find function '%1'").arg(functionName);
            throw TranslationException(errorMessage);
        }

        markAsEnvironment(function);
    }
}

void InterfaceViewOptimizer::keepFunctions(IVModel *ivModel, const std::vector<QString> &functionNames)
{
    for (auto function : ivModel->allObjectsByType<IVFunction>()) {
        const auto functionFound = std::find(functionNames.begin(), functionNames.end(), function->title());
        if (functionFound == functionNames.end()) {
            markAsEnvironment(function);
        }
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

std::vector<QString> InterfaceViewOptimizer::resolveFunctionNames(
        const std::vector<QString> &functionNames, InterfaceViewOptimizer::ParentFunctionsInfo parentFunctionsInfo)
{
    std::vector<QString> allFunctionNames;

    for (const auto &functionName : functionNames) {
        auto resolvedFunctionNames = resolveFunctionName(functionName, parentFunctionsInfo);
        allFunctionNames.insert(allFunctionNames.end(), resolvedFunctionNames.begin(), resolvedFunctionNames.end());
    }

    return allFunctionNames;
}

std::vector<QString> InterfaceViewOptimizer::resolveFunctionName(
        const QString &functionName, InterfaceViewOptimizer::ParentFunctionsInfo parentFunctionsInfo)
{
    if (parentFunctionsInfo.count(functionName) == 0) {
        return { functionName };
    } else {
        const auto childrenFunctionsNames = parentFunctionsInfo.at(functionName);

        std::vector<QString> allFunctionNames;

        for (const auto &childrenFunctionName : childrenFunctionsNames) {
            auto resolvedChildrenFunctionNames = resolveFunctionName(childrenFunctionName, parentFunctionsInfo);
            allFunctionNames.insert(allFunctionNames.begin(), resolvedChildrenFunctionNames.begin(),
                    resolvedChildrenFunctionNames.end());
        }

        return allFunctionNames;
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

    function->setInstanceOf(nullptr);
    function->setEntityAttribute(Props::token(Props::Token::instance_of), "");

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

    for (auto connection : ivModel->allObjectsByType<IVConnection>()) {
        if (isConnectionDead(connection)) {
            objectsToRemove.push_back(connection);

            const auto sourceInterface = connection->sourceInterface();
            const auto connectionsForSourceInterface = ivModel->getConnectionsForIface(sourceInterface->id());
            if (connectionsForSourceInterface.size() <= 1) {
                objectsToRemove.push_back(sourceInterface);
            }

            const auto targetInterface = connection->targetInterface();
            const auto connectionsForTargetInterface = ivModel->getConnectionsForIface(targetInterface->id());
            if (connectionsForTargetInterface.size() <= 1) {
                objectsToRemove.push_back(targetInterface);
            }
        }
    }

    for (const auto obj : objectsToRemove) {
        ivModel->removeObject(obj);
    }
}

void InterfaceViewOptimizer::removeDeadInterfaces(IVModel *ivModel)
{
    std::vector<IVInterface *> interfacesToRemove;

    for (auto interface : ivModel->allObjectsByType<IVInterface>()) {
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

    for (auto function : ivModel->allObjectsByType<IVFunction>()) {
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
    if (interface->kind() == IVInterface::OperationKind::Cyclic) {
        return false;
    }

    const auto parentFunction = interface->function();

    if (parentFunction == nullptr) {
        return true;
    }

    if (parentFunction->type() == IVObject::Type::FunctionType) {
        return false;
    }

    const auto connection = ivModel->getConnectionForIface(interface->id());

    return connection == nullptr;
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

bool InterfaceViewOptimizer::isFunctionParent(const IVFunctionType *function)
{
    return !function->functions().empty() || !function->functionTypes().empty();
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
