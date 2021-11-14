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

#include "statemachinetranslator.h"

#include "statementvisitor.h"

#include <algorithm>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/SedsToIvTranslator/specialized/interfacecommandtranslator.h>
#include <conversion/iv/SedsToIvTranslator/translator.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/task.h>

using conversion::Escaper;
using conversion::iv::translator::InterfaceCommandTranslator;
using conversion::translator::TranslationException;

namespace conversion::sdl::translator {

static constexpr auto RECEPTION_VARIABLE_PATTERN = "input_%1";

auto StateMachineTranslator::translateStateMachine(const seds::model::StateMachine &sedsStateMachine,
        ::sdl::Process *sdlProcess, ::sdl::StateMachine *stateMachine) -> void
{
    // Consider rewriting this to filters when C++20 is supported
    std::map<QString, std::unique_ptr<::sdl::State>> stateMap;
    // First pass through states
    for (auto &element : sedsStateMachine.elements()) {
        // Tried to rewrite it with std::visit and overload - this is the simpler way
        if (std::holds_alternative<seds::model::State>(element)) {
            const auto &state = std::get<seds::model::State>(element);
            stateMap[state.nameStr()] = translateState(state);
        } else if (std::holds_alternative<seds::model::EntryState>(element)) {
            const auto &state = std::get<seds::model::EntryState>(element);
            stateMap[state.name().value()] = translateState(state);
        } else if (std::holds_alternative<seds::model::ExitState>(element)) {
            const auto &state = std::get<seds::model::ExitState>(element);
            stateMap[state.name().value()] = translateState(state);
        }
    }

    // Setup the start transition
    createStartTransition(sedsStateMachine, sdlProcess, stateMap);

    // Second pass through transitions
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<seds::model::Transition>(element)) {
            translateTransition(std::get<seds::model::Transition>(element), sdlProcess, stateMachine, stateMap);
        }
    }
    for (auto &entry : stateMap) {
        stateMachine->addState(std::move(entry.second));
    }
}

auto StateMachineTranslator::translateVariables(const seds::model::Package &sedsPackage, Asn1Acn::Asn1Model *asn1Model,
        const seds::model::ComponentImplementation::VariableSet &variables, ::sdl::Process *sdlProcess) -> void
{
    for (const auto &variable : variables) {
        const auto variableName = Escaper::escapeAsn1FieldName(variable.nameStr());
        const auto variableTypeName = Escaper::escapeAsn1TypeName(variable.type().nameStr());
        // TODO implement check for types imported from other packages
        auto asn1Definitions = iv::translator::SedsToIvTranslator::getAsn1Definitions(sedsPackage, asn1Model);
        const auto *referencedType = asn1Definitions->type(variableTypeName);
        if (referencedType == nullptr) {
            throw TranslationException(QString("Type %1 not found").arg(variableTypeName));
        }
        sdlProcess->addVariable(std::make_unique<::sdl::VariableDeclaration>(variableName, variableTypeName));
    }
}

auto StateMachineTranslator::createVariablesForInputReception(
        const seds::model::Component &sedsComponent, ivm::IVModel *ivModel, ::sdl::Process *sdlProcess) -> void
{
    const auto functionName = Escaper::escapeIvName(sedsComponent.nameStr());
    const auto function = ivModel->getFunction(functionName, Qt::CaseSensitive);
    if (function == nullptr) {
        throw TranslationException(QString("Function %1 not found in the InterfaceView").arg(functionName));
    }
    for (const auto &interface : function->interfaces()) {
        if (interface->isProvided() && interface->kind() == ivm::IVInterface::OperationKind::Sporadic) {
            createVariableForInput(interface, sdlProcess);
        }
    }
}

auto StateMachineTranslator::createExternalProcedures(
        const seds::model::Component &sedsComponent, ivm::IVModel *ivModel, ::sdl::Process *sdlProcess) -> void
{
    const auto functionName = Escaper::escapeIvName(sedsComponent.nameStr());
    const auto function = ivModel->getFunction(functionName, Qt::CaseSensitive);
    if (function == nullptr) {
        throw TranslationException(QString("Function %1 not found in the InterfaceView").arg(functionName));
    }
    for (const auto &interface : function->interfaces()) {
        if (interface->isRequired() && interface->kind() == ivm::IVInterface::OperationKind::Protected) {
            createExternalProcedure(interface, sdlProcess);
        }
    }
}

auto StateMachineTranslator::receptionVariableName(const QString interfaceName) -> QString
{
    return QString(RECEPTION_VARIABLE_PATTERN).arg(interfaceName);
}

auto StateMachineTranslator::createStartTransition(const seds::model::StateMachine &sedsStateMachine,
        ::sdl::Process *sdlProcess, std::map<QString, std::unique_ptr<::sdl::State>> &stateMap) -> void
{
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<seds::model::EntryState>(element)) {
            if (sdlProcess->startTransition() != nullptr) {
                throw TranslationException("Multiple entry states are not supported");
            }
            auto transition = std::make_unique<::sdl::Transition>();
            const auto &state = std::get<seds::model::EntryState>(element);
            transition->addAction(std::make_unique<::sdl::NextState>("", stateMap[state.nameStr()].get()));
            sdlProcess->setStartTransition(std::move(transition));
        }
    }
}

auto StateMachineTranslator::translateState(const seds::model::State &sedsState) -> std::unique_ptr<::sdl::State>
{
    auto state = std::make_unique<::sdl::State>();
    state->setName(Escaper::escapeSdlName(sedsState.nameStr()));
    // Entry and exit procedures shall be translated for transitions
    return state;
}

auto StateMachineTranslator::translateState(const seds::model::ExitState &sedsState) -> std::unique_ptr<::sdl::State>
{
    auto state = std::make_unique<::sdl::State>();
    state->setName(Escaper::escapeSdlName(sedsState.nameStr()));
    // No Entry and exit procedures
    return state;
}

auto StateMachineTranslator::translateState(const seds::model::EntryState &sedsState) -> std::unique_ptr<::sdl::State>
{
    auto state = std::make_unique<::sdl::State>();
    state->setName(Escaper::escapeSdlName(sedsState.nameStr()));
    // No Entry and exit procedures
    return state;
}

auto StateMachineTranslator::translatePrimitive(
        ::sdl::Process *sdlProcess, const seds::model::OnCommandPrimitive &command) -> InputHandler
{
    Q_UNUSED(command);

    auto input = std::make_unique<::sdl::Input>();
    std::vector<std::unique_ptr<::sdl::Action>> unpackingActions;

    // Input signal can be received only via a provided interface
    input->setName(InterfaceCommandTranslator::getCommandName(
            command.interface().value(), ivm::IVInterface::InterfaceType::Provided, command.command().value()));
    if (command.argumentValues().size() == 0) {
        return std::make_pair(std::move(input), std::move(unpackingActions));
    }

    const auto variableName = receptionVariableName(input->name());
    const auto &variableIterator = std::find_if(sdlProcess->variables().begin(), sdlProcess->variables().end(),
            [variableName](const auto &variable) { return variable->name() == variableName; });
    if (variableIterator == sdlProcess->variables().end()) {
        throw TranslationException(QString("Reception variable %1 not found").arg(variableName));
    }
    input->addParameter(std::make_unique<::sdl::VariableReference>((*variableIterator).get()));

    for (const auto &argument : command.argumentValues()) {
        const auto targetVariableName = Escaper::escapeAsn1FieldName(argument.outputVariableRef().value().value());
        const auto fieldName = Escaper::escapeAsn1FieldName(argument.name().value());
        unpackingActions.push_back(std::make_unique<::sdl::Task>(
                "", QString("%1 := %2.%3").arg(targetVariableName, variableName, fieldName)));
    }

    return std::make_pair(std::move(input), std::move(unpackingActions));
}

auto StateMachineTranslator::translatePrimitive(
        ::sdl::Process *sdlProcess, const seds::model::Transition::Primitive &primitive) -> InputHandler
{
    if (std::holds_alternative<seds::model::OnCommandPrimitive>(primitive)) {
        return translatePrimitive(sdlProcess, std::get<seds::model::OnCommandPrimitive>(primitive));
    }
    throw TranslationException("Encountered unsupported primitive");
    return InputHandler();
}

auto StateMachineTranslator::translateTransition(const seds::model::Transition &sedsTransition,
        ::sdl::Process *sdlProcess, ::sdl::StateMachine *stateMachine,
        std::map<QString, std::unique_ptr<::sdl::State>> &stateMap) -> void
{
    const auto fromStateName = Escaper::escapeSdlName(sedsTransition.fromState().nameStr());
    const auto toStateName = Escaper::escapeSdlName(sedsTransition.toState().nameStr());
    const auto &fromStateIterator = stateMap.find(fromStateName);
    const auto &toStateIterator = stateMap.find(toStateName);
    if (fromStateIterator == stateMap.end()) {
        throw TranslationException(QString("Unknown state %1 when translating transition").arg(fromStateName));
    }
    if (toStateIterator == stateMap.end()) {
        throw TranslationException(QString("Unknown state %1 when translating transition").arg(toStateName));
    }
    auto fromState = (*fromStateIterator).second.get();
    auto toState = (*toStateIterator).second.get();
    auto inputHandler = translatePrimitive(sdlProcess, sedsTransition.primitive());

    auto transition = std::make_unique<::sdl::Transition>();
    inputHandler.first->setTransition(transition.get());
    fromState->addInput(std::move(inputHandler.first));
    // Argument unpacking
    for (auto &action : inputHandler.second) {
        transition->addAction(std::move(action));
    }
    // TODO Guard
    // TODO From Exit
    if (sedsTransition.doActivity().has_value()) {
        transition->addAction(StatementVisitor::translateActivityCall(sdlProcess, *sedsTransition.doActivity()));
    }
    // TODO To Entry
    // State switch
    transition->addAction(std::make_unique<::sdl::NextState>("", toState));

    stateMachine->addTransition(std::move(transition));
}

auto StateMachineTranslator::createVariableForInput(ivm::IVInterface const *interface, ::sdl::Process *sdlProcess)
        -> void
{
    if (interface->params().size() == 0) {
        return; // NOP
    }
    const auto interfaceName = interface->title();
    if (interface->params().size() > 1) {
        throw TranslationException(QString("Sporadic interface %1 has more than one parameter").arg(interfaceName));
        return;
    }
    const auto variableName = receptionVariableName(interfaceName);
    const auto &param = interface->params()[0];
    sdlProcess->addVariable(std::make_unique<::sdl::VariableDeclaration>(variableName, param.paramTypeName()));
}

auto StateMachineTranslator::createExternalProcedure(ivm::IVInterface const *interface, ::sdl::Process *sdlProcess)
        -> void
{
    const auto interfaceName = interface->title();

    auto procedure = std::make_unique<::sdl::Procedure>();
    procedure->setName(interfaceName);
    // External procedures are required for call names
    // Arguments do not need to be translated

    sdlProcess->addProcedure(std::move(procedure));
}

} // namespace conversion::sdl::translator
