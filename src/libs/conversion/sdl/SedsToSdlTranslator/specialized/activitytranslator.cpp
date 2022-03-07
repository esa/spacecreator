/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "activitytranslator.h"

#include "statementtranslatorvisitor.h"

#include <conversion/common/escaper/escaper.h>
#include <sdl/SdlModel/task.h>

using conversion::Escaper;

namespace conversion::sdl::translator {

auto ActivityTranslator::translateActivity(
        Context &context, const seds::model::Activity &sedsActivity, ::sdl::Process *sdlProcess) -> void
{

    const auto name = Escaper::escapeSdlName(sedsActivity.nameStr());
    auto procedure = std::make_unique<::sdl::Procedure>(name);
    for (const auto &argument : sedsActivity.arguments()) {
        const auto parameterName = Escaper::escapeSdlVariableName(argument.nameStr());
        const auto parameterType = Escaper::escapeAsn1TypeName(argument.typeRef().nameStr());
        auto parameter = std::make_unique<::sdl::ProcedureParameter>(parameterName, parameterType, "in");
        procedure->addParameter(std::move(parameter));
    }
    translateBody(context, sedsActivity, sdlProcess, procedure.get());
    sdlProcess->addProcedure(std::move(procedure));
}

auto ActivityTranslator::translateBody(Context &context, const seds::model::Activity &sedsActivity,
        ::sdl::Process *sdlProcess, ::sdl::Procedure *procedure) -> void
{
    auto transition = std::make_unique<::sdl::Transition>();
    StatementTranslatorVisitor::StatementContext statementContext(context, sdlProcess, procedure);
    StatementTranslatorVisitor visitor(statementContext, transition.get());
    for (const auto &statement : sedsActivity.body()->statements()) {
        std::visit(visitor, statement);
    }

    procedure->setTransition(std::move(transition));
}

} // namespace conversion::sdl::translator
