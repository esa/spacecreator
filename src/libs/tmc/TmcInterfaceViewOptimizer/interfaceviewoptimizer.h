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

class InterfaceViewOptimizer final
{
public:
    InterfaceViewOptimizer() = delete;
    InterfaceViewOptimizer(const InterfaceViewOptimizer &) = delete;
    InterfaceViewOptimizer(InterfaceViewOptimizer &&) = delete;

    InterfaceViewOptimizer operator=(const InterfaceViewOptimizer &) = delete;
    InterfaceViewOptimizer operator=(InterfaceViewOptimizer &&) = delete;

public:
    static auto optimizeModel(ivm::IVModel *ivModel, const std::vector<QString> &environmentFunctions) -> void;

private:
    static auto makeEnvironment(const QString &functionName, ivm::IVModel *ivModel) -> void;
    static auto removeDeadFunctions(ivm::IVModel *ivModel) -> void;

    static auto findFunction(const QString &functionName, ivm::IVModel *ivModel) -> ivm::IVFunction *;
    static auto setGUIImplementation(ivm::IVFunction *function) -> void;

    static auto isConnectionDead(const ivm::IVConnection *connection) -> bool;
    static auto isFunctionDead(const ivm::IVFunctionType *function) -> bool;
    static auto isSdlFunction(const ivm::IVFunction *function) -> bool;
};

} // namespace tmc
