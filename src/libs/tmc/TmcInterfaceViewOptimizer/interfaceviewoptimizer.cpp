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
#include <QMap>
#include <QSaveFile>
#include <QString>
#include <algorithm>
#include <cassert>
#include <conversion/common/translation/exceptions.h>
#include <iostream>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivconnection.h>
#include <ivcore/ivconnectiongroup.h>
#include <ivcore/ivfunction.h>
#include <libiveditor/ivexporter.h>
#include <map>

using ivm::IVConnection;
using ivm::IVFunction;
using ivm::IVFunctionType;
using ivm::IVInterface;
using ivm::IVModel;
using ivm::IVObject;
using ivm::meta::Props;

using conversion::translator::TranslationException;

namespace tmc {

namespace {
struct ConnectionInfo {
    shared::Id sourceInterfaceName;
    shared::Id targetFunctionName;
    shared::Id targetInterfaceName;
};

using ConnectionInfoMap = QMap<shared::Id, ConnectionInfo>;
}

void InterfaceViewOptimizer::optimizeModel(
        IVModel *ivModel, const std::vector<QString> &functionNames, InterfaceViewOptimizer::Mode mode)
{
    flattenModel(ivModel);

    switch (mode) {
    case Mode::Environment:
        discardFunctions(ivModel, functionNames);
        break;
    case Mode::Keep:
        keepFunctions(ivModel, functionNames);
        break;
    case Mode::None:
        break;
    }

    removeDeadConnections(ivModel);
    removeDeadInterfaces(ivModel);
    removeDeadFunctions(ivModel);
}

void InterfaceViewOptimizer::flattenModel(ivm::IVModel *ivModel)
{
    while (flattenOneFunction(ivModel))
        ;
}

bool InterfaceViewOptimizer::flattenOneFunction(IVModel *ivModel)
{
    // find parent functions in the loop
    for (auto function : ivModel->allObjectsByType<IVFunctionType>()) {
        if (isFunctionParent(function)) {
            moveNestedFunctionsToRoot(ivModel, function);
            return true;
        }
    }

    return false;
}

void InterfaceViewOptimizer::moveNestedFunctionsToRoot(ivm::IVModel *ivModel, ivm::IVFunctionType *function)
{
    QVector<QString> nestedFunctionNames;
    for (const auto &child : function->functions()) {
        nestedFunctionNames.append(child->property("name").toString());
    }
    for (const auto &child : function->functionTypes()) {
        nestedFunctionNames.append(child->property("name").toString());
    }

    ConnectionInfoMap connectionsToRestore;
    const auto interfaces = function->interfaces();

    const auto connections = ivModel->getConnectionsForFunction(function->id());
    std::multimap<shared::Id, QPair<shared::Id, shared::Id>> innerInputConnections;
    std::multimap<shared::Id, QPair<shared::Id, shared::Id>> innerOutputConnections;
    std::multimap<shared::Id, QPair<shared::Id, shared::Id>> outerInputConnections;
    std::multimap<shared::Id, QPair<shared::Id, shared::Id>> outerOutputConnections;
    for (const auto &connection : connections) {
        if (connection->source()->id() == function->id()) {
            if (nestedFunctionNames.contains(connection->targetName())) {
                innerOutputConnections.emplace(connection->sourceInterface()->id(),
                        qMakePair(connection->target()->id(), connection->targetInterface()->id()));
            } else {
                outerOutputConnections.emplace(connection->sourceInterface()->id(),
                        qMakePair(connection->target()->id(), connection->targetInterface()->id()));
            }
        } else {
            if (nestedFunctionNames.contains(connection->targetName())) {
                innerInputConnections.emplace(connection->targetInterface()->id(),
                        qMakePair(connection->source()->id(), connection->sourceInterface()->id()));
            } else {
                outerInputConnections.emplace(connection->targetInterfaceName(),
                        qMakePair(connection->source()->id(), connection->sourceInterface()->id()));
            }
        }
    }

    for (auto infoIter = outerInputConnections.begin(); infoIter != outerInputConnections.end(); ++infoIter) {
        auto [begin, end] = innerOutputConnections.equal_range(infoIter->first);
        for (auto iter = begin; iter != end; ++iter) {
            connectionsToRestore.insert(infoIter->second.first,
                    ConnectionInfo { infoIter->second.second, iter->second.first, iter->second.second });
        }
    }

    for (auto infoIter = innerInputConnections.begin(); infoIter != innerInputConnections.end(); ++infoIter) {
        auto [begin, end] = outerOutputConnections.equal_range(infoIter->first);
        for (auto iter = begin; iter != end; ++iter) {
            connectionsToRestore.insert(infoIter->second.first,
                    ConnectionInfo { infoIter->second.second, iter->second.first, iter->second.second });
        }
    }

    // remove all connections
    for (const auto &connection : connections) {
        ivModel->removeObject(connection);
    }

    // reparent functions
    for (auto fn : function->functions()) {
        fn->setParent(ivModel);
    }
    for (auto fn : function->functionTypes()) {
        fn->setParent(ivModel);
    }
    // reparent connections
    for (auto conn : function->connections()) {
        conn->setParent(ivModel);
    }
    for (auto conn : function->connectionGroups()) {
        conn->setParent(ivModel);
    }

    // remove function
    ivModel->removeObject(function);

    // restore connections
    for (auto iter = connectionsToRestore.begin(); iter != connectionsToRestore.end(); ++iter) {
        IVInterface *source = dynamic_cast<IVInterface *>(ivModel->getObject(iter.value().sourceInterfaceName));
        IVInterface *target = dynamic_cast<IVInterface *>(ivModel->getObject(iter.value().targetInterfaceName));
        assert(source != nullptr);
        assert(target != nullptr);
        auto connection = new IVConnection(source, target, ivModel);
        ivModel->addObject(connection);
        source->function()->addChild(connection);
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

    // GUI functions cannot be an instance of function type
    function->setInstanceOf(nullptr);
    function->setEntityAttribute(Props::token(Props::Token::instance_of), "");

    // GUI functions cannot have context parameters and timers
    function->clearContextParams();

    // Set this implementation as default
    function->setDefaultImplementation(m_environmentImplementationName);

    // GUI functions cannot have any cyclic interfaces
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
        if (interface->kind() == IVInterface::OperationKind::Cyclic) {
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
