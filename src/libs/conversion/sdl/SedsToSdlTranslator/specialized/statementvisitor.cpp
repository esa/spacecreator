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

#include "statementvisitor.h"

#include "translation/exceptions.h"

#include <conversion/common/escaper/escaper.h>

using conversion::Escaper;
using conversion::translator::TranslationException;

namespace conversion::sdl::translator {

StatementVisitor::StatementVisitor(const seds::model::Package &sedsPackage, Asn1Acn::Asn1Model *asn1Model,
        ::sdl::Process *sdlProcess, ::sdl::Procedure *sdlProcedure, ::sdl::Transition *sdlTransition)
    : m_sedsPackage(sedsPackage)
    , m_asn1Model(asn1Model)
    , m_sdlProcess(sdlProcess)
    , m_sdlProcedure(sdlProcedure)
    , m_sdlTransition(sdlTransition)
{
}

auto StatementVisitor::operator()(const seds::model::ActivityInvocation &invocation) -> void
{
    auto task = translateActivityCall(m_sdlProcess, invocation);
    m_sdlTransition->addAction(std::move(task));
}

auto StatementVisitor::operator()(const seds::model::Assignment &assignment) -> void
{
    const auto targetName = Escaper::escapeAsn1FieldName(assignment.outputVariableRef().value().value());
    const auto &element = assignment.element();
    if (std::holds_alternative<seds::model::VariableRef>(element)) {
        const auto action =
                QString("%1 := %2").arg(targetName, std::get<seds::model::VariableRef>(element).value().value());
        m_sdlTransition->addAction(std::make_unique<::sdl::Task>("", action));
    } else if (std::holds_alternative<seds::model::ValueOperand>(element)) {
        const auto action =
                QString("%1 := %2").arg(targetName, std::get<seds::model::ValueOperand>(element).value().value());
        m_sdlTransition->addAction(std::make_unique<::sdl::Task>("", action));
    }
    throw TranslationException("Not implemented");
}

auto StatementVisitor::operator()(const seds::model::Calibration &calibration) -> void
{
    Q_UNUSED(calibration);
}

auto StatementVisitor::operator()(const seds::model::Conditional &conditional) -> void
{
    Q_UNUSED(conditional);
}

auto StatementVisitor::operator()(const seds::model::Iteration &iteration) -> void
{
    Q_UNUSED(iteration);
}

auto StatementVisitor::operator()(const seds::model::MathOperation &operation) -> void
{
    Q_UNUSED(operation);
}

auto StatementVisitor::operator()(const seds::model::SendCommandPrimitive &sendCommand) -> void
{
    Q_UNUSED(sendCommand);
}

auto StatementVisitor::operator()(const seds::model::SendParameterPrimitive &sendPrimitive) -> void
{
    Q_UNUSED(sendPrimitive);
}

auto StatementVisitor::translateActivityCall(::sdl::Process *process, const seds::model::ActivityInvocation &invocation)
        -> std::unique_ptr<::sdl::ProcedureCall>
{
    auto call = std::make_unique<::sdl::ProcedureCall>();
    call->setName(Escaper::escapeSdlName(invocation.activity().value()));
    for (const auto &argument : invocation.argumentValues()) {
        if (std::holds_alternative<seds::model::ValueOperand>(argument.value())) {
            const auto &value = std::get<seds::model::ValueOperand>(argument.value());
            call->addArgument(std::make_unique<::sdl::VariableLiteral>(value.value().value()));
        }
        if (std::holds_alternative<seds::model::VariableRefOperand>(argument.value())) {
            const auto &value = std::get<seds::model::VariableRefOperand>(argument.value());
            const auto variableName = Escaper::escapeAsn1FieldName(value.variableRef().value().value());
            const auto variableDeclaration = std::find_if(process->variables().begin(), process->variables().end(),
                    [variableName](const auto &variable) { return variable->name() == variableName; });
            if (variableDeclaration == process->variables().end()) {
                throw TranslationException(QString("Variable %1 not found").arg(variableName));
            }
            call->addArgument(std::make_unique<::sdl::VariableReference>((*variableDeclaration).get()));
        }
    }
    return std::move(call);
}

}
