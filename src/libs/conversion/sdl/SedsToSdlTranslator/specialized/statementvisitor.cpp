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
#include <conversion/iv/SedsToIvTranslator/specialized/interfacecommandtranslator.h>

using conversion::Escaper;
using conversion::iv::translator::InterfaceCommandTranslator;
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
        return;
    } else if (std::holds_alternative<seds::model::ValueOperand>(element)) {
        const auto action =
                QString("%1 := %2").arg(targetName, std::get<seds::model::ValueOperand>(element).value().value());
        m_sdlTransition->addAction(std::make_unique<::sdl::Task>("", action));
        return;
    }
    throw TranslationException("Assignment not implemented");
}

auto StatementVisitor::operator()(const seds::model::Calibration &calibration) -> void
{
    Q_UNUSED(calibration);
    throw TranslationException("Calibration activity not implemented");
}

auto StatementVisitor::operator()(const seds::model::Conditional &conditional) -> void
{
    Q_UNUSED(conditional);
    throw TranslationException("Conditional activity not implemented");
}

auto StatementVisitor::operator()(const seds::model::Iteration &iteration) -> void
{
    Q_UNUSED(iteration);
    throw TranslationException("Iteration activity not implemented");
}

auto StatementVisitor::operator()(const seds::model::MathOperation &operation) -> void
{
    Q_UNUSED(operation);
    throw TranslationException("Operation activity not implemented");
}

auto StatementVisitor::operator()(const seds::model::SendCommandPrimitive &sendCommand) -> void
{
    const auto commandName = sendCommand.command().value();
    const auto interfaceName = sendCommand.interface().value();

    const auto callName = InterfaceCommandTranslator::getCommandName(
            interfaceName, ivm::IVInterface::InterfaceType::Required, commandName);
    // TODO check if this is sync or async, and generate OUTPUT instead if applicable

    auto call = std::make_unique<::sdl::ProcedureCall>();

    const auto &procedure = std::find_if(m_sdlProcess->procedures().begin(), m_sdlProcess->procedures().end(),
            [callName](const auto &p) { return p->name() == callName; });

    if (procedure == m_sdlProcess->procedures().end()) {
        throw TranslationException(QString("Procedure %1 not found").arg(callName));
    }
    call->setProcedure(procedure->get());

    for (const auto &argument : sendCommand.argumentValues()) {
        call->addArgument(std::move(translateArgument(m_sdlProcess, m_sdlProcedure, argument)));
    }
    m_sdlTransition->addAction(std::move(call));
}

auto StatementVisitor::operator()(const seds::model::SendParameterPrimitive &sendPrimitive) -> void
{
    Q_UNUSED(sendPrimitive);
    throw TranslationException("SendPrimitive activity not implemented");
}

auto StatementVisitor::translateActivityCall(::sdl::Process *process, const seds::model::ActivityInvocation &invocation)
        -> std::unique_ptr<::sdl::ProcedureCall>
{
    auto call = std::make_unique<::sdl::ProcedureCall>();
    const auto procedureName = Escaper::escapeSdlName(invocation.activity().value());
    const auto &procedure = std::find_if(process->procedures().begin(), process->procedures().end(),
            [procedureName](const auto &p) { return p->name() == procedureName; });

    if (procedure == process->procedures().end()) {
        throw TranslationException(QString("Procedure %1 not found").arg(procedureName));
    }
    call->setProcedure(procedure->get());

    for (const auto &argument : invocation.argumentValues()) {
        call->addArgument(std::move(translateArgument(process, nullptr, argument)));
    }
    return std::move(call);
}

auto StatementVisitor::findVariableDeclaration(::sdl::Process *process, ::sdl::Procedure *sdlProcedure, QString name)
        -> ::sdl::VariableDeclaration *
{
    const auto processVariableDeclaration = std::find_if(process->variables().begin(), process->variables().end(),
            [name](const auto &variable) { return variable->name() == name; });
    if (processVariableDeclaration != process->variables().end()) {
        return (*processVariableDeclaration).get();
    }
    if (sdlProcedure != nullptr) {
        const auto procedureVariableDeclaration = std::find_if(sdlProcedure->parameters().begin(),
                sdlProcedure->parameters().end(), [name](const auto &variable) { return variable->name() == name; });
        if (procedureVariableDeclaration != sdlProcedure->parameters().end()) {
            return (*procedureVariableDeclaration).get();
        }
    }

    throw TranslationException(QString("Variable %1 not found").arg(name));
    return nullptr;
}

auto StatementVisitor::translateArgument(::sdl::Process *process, ::sdl::Procedure *sdlProcedure,
        const seds::model::NamedArgumentValue &argument) -> ::sdl::ProcedureCall::Argument
{
    if (std::holds_alternative<seds::model::ValueOperand>(argument.value())) {
        const auto &value = std::get<seds::model::ValueOperand>(argument.value());
        return std::make_unique<::sdl::VariableLiteral>(value.value().value());
    }
    if (std::holds_alternative<seds::model::VariableRefOperand>(argument.value())) {
        const auto &value = std::get<seds::model::VariableRefOperand>(argument.value());
        const auto variableName = Escaper::escapeAsn1FieldName(value.variableRef().value().value());
        const auto variableDeclaration = findVariableDeclaration(process, sdlProcedure, variableName);
        return std::make_unique<::sdl::VariableReference>(variableDeclaration);
    }
    return std::unique_ptr<::sdl::VariableReference>();
}

}
