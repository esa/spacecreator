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
     * @param   ivModel                 Model to modify
     * @param   environmentFunctions    Names of functions that should be marked as environment
     */
    static auto optimizeModel(ivm::IVModel *ivModel, const std::vector<QString> &environmentFunctions) -> void;

private:
    static auto markAsEnvironment(const QString &functionName, ivm::IVModel *ivModel) -> void;
    static auto removeDeadFunctions(ivm::IVModel *ivModel) -> void;

    static auto setGuiAsDefaultImplementation(ivm::IVFunction *function) -> void;

    static auto findFunction(const QString &functionName, ivm::IVModel *ivModel) -> ivm::IVFunction *;
    static auto findImplementationType(const QString &implementationName, const ivm::IVFunction *function) -> QString;

    static auto removeUnallowedInterfaces(ivm::IVFunction *function) -> void;

    static auto isConnectionDead(const ivm::IVConnection *connection) -> bool;
    static auto isFunctionDead(const ivm::IVFunctionType *function) -> bool;
    static auto isSdlFunction(const ivm::IVFunction *function) -> bool;

    static inline const QString m_environmentImplementationName = "environment";
    static inline const QString m_environmentImplementationType = "GUI";
};

} // namespace tmc
