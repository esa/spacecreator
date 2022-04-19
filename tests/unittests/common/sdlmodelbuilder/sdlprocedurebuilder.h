/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <memory>
#include <sdl/SdlModel/procedure.h>
#include <sdl/SdlModel/procedureparameter.h>
#include <sdl/SdlModel/transition.h>
#include <sdl/SdlModel/variabledeclaration.h>
#include <sdl/SdlModel/variablereference.h>

using sdl::Procedure;
using sdl::ProcedureParameter;
using sdl::Transition;
using sdl::VariableDeclaration;
using sdl::VariableReference;

namespace tests::common {

class SdlProcedureBuilder final
{
public:
    SdlProcedureBuilder();

    auto build() -> std::unique_ptr<Procedure>;

    auto withName(QString name) -> SdlProcedureBuilder &;

    auto withTransition(std::unique_ptr<Transition> transition) -> SdlProcedureBuilder &;

    auto withVariable(std::unique_ptr<VariableDeclaration> variable) -> SdlProcedureBuilder &;

    auto withParameter(std::unique_ptr<ProcedureParameter> parameter) -> SdlProcedureBuilder &;

    auto withReturnType(QString returnType) -> SdlProcedureBuilder &;

private:
    std::unique_ptr<Procedure> m_procedure;
};

} // namespace tests::common
