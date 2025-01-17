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

#include <ivcore/ivmodel.h>
#include <shared/qstringhash.h>
#include <unordered_map>
#include <vector>

namespace tmc {

/**
 * @brief   InterfaceView optimizer for model checking
 *
 * This optimizes given IV by marking listed functions as an environment and then
 * removes all 'dead' functions - environment functions that are connected only
 * to other environment functions
 */
class InterfaceViewOptimizer final
{
public:
    /// @brief   Mode in which optimizer should work
    enum class Mode
    {
        Environment,
        Keep,
        None
    };

public:
    /**
     * @brief   Deleted constructor
     */
    InterfaceViewOptimizer() = delete;
    /**
     * @brief   Deleted copy constructor
     */
    InterfaceViewOptimizer(const InterfaceViewOptimizer &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    InterfaceViewOptimizer(InterfaceViewOptimizer &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    InterfaceViewOptimizer operator=(const InterfaceViewOptimizer &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    InterfaceViewOptimizer operator=(InterfaceViewOptimizer &&) = delete;

public:
    /**
     * @brief   Optimize passed model
     *
     * @param   ivModel         Model to modify
     * @param   functionNames   Names of functions
     * @param   mode            Mode in which optimizer should work
     */
    static auto optimizeModel(ivm::IVModel *ivModel, const std::vector<QString> &functionNames, Mode mode) -> void;

private:
    static auto flattenModel(ivm::IVModel *ivModel) -> void;
    static auto findFunctionToFlatten(ivm::IVModel *ivModel) -> ivm::IVFunctionType *;
    static auto moveNestedFunctionsToRoot(ivm::IVModel *ivModel, ivm::IVFunctionType *function) -> void;

    static auto resolveFunctionNames(ivm::IVModel *ivModel, const std::vector<QString> &functionNames)
            -> std::vector<QString>;
    static auto getChildFunctionNames(ivm::IVFunctionType *function, std::vector<QString> &functionNames) -> void;
    static auto discardFunctions(ivm::IVModel *ivModel, const std::vector<QString> &functionNames) -> void;
    static auto keepFunctions(ivm::IVModel *ivModel, const std::vector<QString> &functionNames) -> void;

    static auto markAsEnvironment(ivm::IVFunction *function) -> void;

    static auto setGuiAsDefaultImplementation(ivm::IVFunction *function) -> void;
    static auto findImplementationType(const QString &implementationName, const ivm::IVFunction *function) -> QString;

    static auto removeDeadConnections(ivm::IVModel *ivModel) -> void;
    static auto removeDeadInterfaces(ivm::IVModel *ivModel) -> void;
    static auto removeDeadFunctions(ivm::IVModel *ivModel) -> void;
    static auto removeUnallowedInterfaces(ivm::IVFunction *function) -> void;

    static auto isFunction(const shared::VEObject *object) -> bool;
    static auto isFunctionType(const shared::VEObject *object) -> bool;
    static auto isConnectionDead(const ivm::IVConnection *connection) -> bool;
    static auto isInterfaceDead(const ivm::IVInterface *interface, const ivm::IVModel *ivModel) -> bool;
    static auto isFunctionDead(const ivm::IVFunction *function) -> bool;
    static auto isFunctionParent(const ivm::IVFunctionType *function) -> bool;
    static auto isSdlFunction(const ivm::IVFunctionType *functionType) -> bool;

    static inline const QString m_environmentImplementationName = "environment";
    static inline const QString m_environmentImplementationType = "GUI";
};

} // namespace tmc
