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

#include "statementtranslatorvisitor.h"

#include <algorithm>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/SedsToIvTranslator/specialized/interfacecommandtranslator.h>
#include <conversion/iv/SedsToIvTranslator/translator.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/task.h>

using conversion::Escaper;
using conversion::asn1::translator::SedsToAsn1Translator;
using conversion::iv::translator::InterfaceCommandTranslator;
using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::MissingInterfaceViewFunctionException;
using conversion::translator::TranslationException;

namespace conversion::sdl::translator {

static const QString IO_VARIABLE_PATTERN = "io_%1";
static const QString FALSE_LITERAL = "False";
static const QString TRUE_LITERAL = "True";
static const QString TIMER_NAME_PATTERN = "timer_%1";
// This is a built-in procedure, so it is not declared as an external one
// And it is not defined internally
// But a pointer is still needed to the SDL model for call invocation
static ::sdl::Procedure BUILT_IN_SET_TIMER_PROCEDURE("set_timer");

template<typename ElementType>
static inline auto getElementOfName(const seds::model::StateMachine &sedsStateMachine, const QString name)
        -> std::optional<const ElementType *>
{
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<ElementType>(element)) {
            const auto &item = std::get<ElementType>(element);
            if (item.nameStr() == name) {
                return &item;
            }
        }
    }
    return std::nullopt;
}

static inline auto getOnExit(const seds::model::StateMachine &sedsStateMachine, const QString name)
        -> std::optional<const seds::model::ActivityInvocation *>
{
    // OnExit applies only to states, not entry or exit states
    const auto &state = getElementOfName<seds::model::State>(sedsStateMachine, name);
    if (!state.has_value()) {
        return std::nullopt;
    }
    if (!(*state)->onExit().has_value()) {
        return std::nullopt;
    }

    return &(*((*state)->onExit()));
}

static inline auto getOnEntry(const seds::model::StateMachine &sedsStateMachine, const QString name)
        -> std::optional<const seds::model::ActivityInvocation *>
{
    // OnEntry applies only to states, not entry or exit states
    const auto &state = getElementOfName<seds::model::State>(sedsStateMachine, name);
    if (!state.has_value()) {
        return std::nullopt;
    }
    if (!(*state)->onEntry().has_value()) {
        return std::nullopt;
    }

    return &(*((*state)->onEntry()));
}

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
            translateTransition(
                    sedsStateMachine, std::get<seds::model::Transition>(element), sdlProcess, stateMachine, stateMap);
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
        auto asn1Definitions = SedsToAsn1Translator::getAsn1Definitions(sedsPackage, asn1Model);
        const auto *referencedType = asn1Definitions->type(variableTypeName);
        if (referencedType == nullptr) {
            throw MissingAsn1TypeDefinitionException(variableTypeName);
        }
        sdlProcess->addVariable(std::make_unique<::sdl::VariableDeclaration>(variableName, variableTypeName));
    }
}

auto StateMachineTranslator::createTimerVariables(
        const seds::model::StateMachine &sedsStateMachine, ::sdl::Process *sdlProcess) -> void
{
    std::set<QString> statesWithTimers;
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<seds::model::Transition>(element)) {
            const auto &transition = std::get<seds::model::Transition>(element);
            if (std::holds_alternative<seds::model::TimerSink>(transition.primitive())) {
                const auto stateName = transition.fromState().nameStr();
                if (statesWithTimers.find(stateName) == statesWithTimers.end()) {
                    statesWithTimers.insert(stateName);
                    sdlProcess->addTimer(timerName(stateName));
                }
            }
        }
    }
}

auto StateMachineTranslator::createIoVariables(
        const seds::model::Component &sedsComponent, ivm::IVModel *ivModel, ::sdl::Process *sdlProcess) -> void
{
    const auto functionName = Escaper::escapeIvName(sedsComponent.nameStr());
    const auto function = ivModel->getFunction(functionName, Qt::CaseSensitive);
    if (function == nullptr) {
        throw MissingInterfaceViewFunctionException(functionName);
    }
    for (const auto &interface : function->interfaces()) {
        if (interface->kind() == ivm::IVInterface::OperationKind::Sporadic) {
            createIoVariable(interface, sdlProcess);
        }
    }
}

auto StateMachineTranslator::createExternalProcedures(
        const seds::model::Component &sedsComponent, ivm::IVModel *ivModel, ::sdl::Process *sdlProcess) -> void
{
    const auto functionName = Escaper::escapeIvName(sedsComponent.nameStr());
    const auto function = ivModel->getFunction(functionName, Qt::CaseSensitive);
    if (function == nullptr) {
        throw MissingInterfaceViewFunctionException(functionName);
    }
    for (const auto &interface : function->interfaces()) {
        if (interface->isRequired() && interface->kind() == ivm::IVInterface::OperationKind::Protected) {
            createExternalProcedure(interface, sdlProcess);
        }
    }
}

auto StateMachineTranslator::ioVariableName(const QString interfaceName) -> QString
{
    return IO_VARIABLE_PATTERN.arg(interfaceName);
}

auto StateMachineTranslator::timerName(const QString stateName) -> QString
{
    return Escaper::escapeSdlName(TIMER_NAME_PATTERN.arg(stateName));
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
            const auto timerTime = getTimerInvocationTime(sedsStateMachine, state.nameStr());
            if (timerTime.has_value()) {
                transition->addAction(createTimerSetCall(timerName(state.nameStr()), *timerTime));
            }
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
    auto input = std::make_unique<::sdl::Input>();
    std::vector<std::unique_ptr<::sdl::Action>> unpackingActions;

    // Input signal can be received only via a provided interface
    input->setName(InterfaceCommandTranslator::getCommandName(
            command.interface().value(), ivm::IVInterface::InterfaceType::Provided, command.command().value()));
    if (command.argumentValues().size() == 0) {
        return std::make_pair(std::move(input), std::move(unpackingActions));
    }

    const auto variableName = ioVariableName(input->name());
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

auto StateMachineTranslator::translatePrimitive(::sdl::State *sdlFromState) -> InputHandler
{
    auto input = std::make_unique<::sdl::Input>();
    input->setName(TIMER_NAME_PATTERN.arg(sdlFromState->name()));
    return std::make_pair(std::move(input), std::vector<std::unique_ptr<::sdl::Action>>());
}

auto StateMachineTranslator::translatePrimitive(::sdl::Process *sdlProcess, ::sdl::State *sdlFromState,
        const seds::model::Transition::Primitive &primitive) -> InputHandler
{
    return std::visit(
            overloaded { [&sdlProcess](const seds::model::OnCommandPrimitive &command) {
                            return translatePrimitive(sdlProcess, command);
                        },
                    [](const seds::model::OnParameterPrimitive &parameter) {
                        Q_UNUSED(parameter);
                        throw TranslationException("Encountered unsupported primitive");
                        return InputHandler();
                    },
                    [&sdlFromState](const seds::model::TimerSink &) { return translatePrimitive(sdlFromState); }

            },
            primitive);
}

auto StateMachineTranslator::translateTransition(const seds::model::StateMachine &sedsStateMachine,
        const seds::model::Transition &sedsTransition, ::sdl::Process *sdlProcess, ::sdl::StateMachine *stateMachine,
        std::map<QString, std::unique_ptr<::sdl::State>> &stateMap) -> void
{
    const auto fromStateName = Escaper::escapeSdlName(sedsTransition.fromState().nameStr());
    const auto toStateName = Escaper::escapeSdlName(sedsTransition.toState().nameStr());
    const auto stateChange = fromStateName != toStateName;
    const auto &fromStateIterator = stateMap.find(fromStateName);
    const auto &toStateIterator = stateMap.find(toStateName);

    if (fromStateIterator == stateMap.end()) {
        throw TranslationException(QString("Unknown state %1 when translating transition").arg(fromStateName));
    }
    if (toStateIterator == stateMap.end()) {
        throw TranslationException(QString("Unknown state %1 when translating transition").arg(toStateName));
    }
    auto sdlFromState = (*fromStateIterator).second.get();
    auto sdlToState = (*toStateIterator).second.get();

    auto inputHandler = translatePrimitive(sdlProcess, sdlFromState, sedsTransition.primitive());

    auto mainTransition = std::make_unique<::sdl::Transition>();
    auto currentTransitionPtr = mainTransition.get();
    inputHandler.first->setTransition(mainTransition.get());
    sdlFromState->addInput(std::move(inputHandler.first));
    // Argument unpacking
    for (auto &action : inputHandler.second) {
        currentTransitionPtr->addAction(std::move(action));
    }

    if (sedsTransition.guard().has_value()) {
        currentTransitionPtr = translateGuard(sdlProcess, sdlFromState, currentTransitionPtr, *sedsTransition.guard());
    }

    if (stateChange) {
        const auto onExit = getOnExit(sedsStateMachine, sedsTransition.fromState().nameStr());
        if (onExit.has_value()) {
            currentTransitionPtr->addAction(StatementTranslatorVisitor::translateActivityCall(sdlProcess, **onExit));
        }
    }

    if (sedsTransition.doActivity().has_value()) {
        currentTransitionPtr->addAction(
                StatementTranslatorVisitor::translateActivityCall(sdlProcess, *sedsTransition.doActivity()));
    }
    if (stateChange) {
        const auto onEntry = getOnEntry(sedsStateMachine, sedsTransition.toState().nameStr());
        if (onEntry.has_value()) {
            currentTransitionPtr->addAction(StatementTranslatorVisitor::translateActivityCall(sdlProcess, **onEntry));
        }
        const auto timerTime = getTimerInvocationTime(sedsStateMachine, sedsTransition.toState().nameStr());
        if (timerTime.has_value()) {
            currentTransitionPtr->addAction(
                    createTimerSetCall(timerName(sedsTransition.toState().nameStr()), *timerTime));
        }
    }
    // State switch
    currentTransitionPtr->addAction(std::make_unique<::sdl::NextState>("", sdlToState));

    stateMachine->addTransition(std::move(mainTransition));
}

auto StateMachineTranslator::createIoVariable(ivm::IVInterface const *interface, ::sdl::Process *sdlProcess) -> void
{
    if (interface->params().size() == 0) {
        return; // NOP
    }
    const auto interfaceName = interface->title();
    if (interface->params().size() > 1) {
        throw TranslationException(QString("Sporadic interface %1 has more than one parameter").arg(interfaceName));
        return;
    }
    const auto variableName = ioVariableName(interfaceName);
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

auto StateMachineTranslator::translateGuard(::sdl::Process *sdlProcess, ::sdl::State *fromState,
        ::sdl::Transition *currentTransitionPtr, const seds::model::BooleanExpression &guard) -> ::sdl::Transition *
{
    auto decision = StatementTranslatorVisitor::translateBooleanExpression(sdlProcess, nullptr, guard);

    auto falseTransition = std::make_unique<::sdl::Transition>();
    // Abort the transition
    falseTransition->addAction(std::make_unique<::sdl::NextState>("", fromState));
    auto falseAnswer = std::make_unique<::sdl::Answer>();
    falseAnswer->setLiteral(::sdl::VariableLiteral(FALSE_LITERAL));
    falseAnswer->setTransition(std::move(falseTransition));
    auto trueTransition = std::make_unique<::sdl::Transition>();
    // Switch the current transition for exit/action/entry to the true answer
    auto newTransitionPtr = trueTransition.get();
    auto trueAnswer = std::make_unique<::sdl::Answer>();
    trueAnswer->setLiteral(::sdl::VariableLiteral(TRUE_LITERAL));
    trueAnswer->setTransition(std::move(trueTransition));

    decision->addAnswer(std::move(trueAnswer));
    decision->addAnswer(std::move(falseAnswer));

    currentTransitionPtr->addAction(std::move(decision));

    return newTransitionPtr;
}
auto StateMachineTranslator::getTimerInvocationTime(
        const seds::model::StateMachine &sedsStateMachine, const QString stateName) -> std::optional<uint64_t>
{
    // TODO - consider supporting multiple different OnTimer events for a state timed separately
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<seds::model::Transition>(element)) {
            const auto &transition = std::get<seds::model::Transition>(element);
            if (transition.fromState().nameStr() != stateName) {
                continue;
            }
            if (std::holds_alternative<seds::model::TimerSink>(transition.primitive())) {
                return std::get<seds::model::TimerSink>(transition.primitive()).nanosecondsAfterEntry();
            }
        }
    }
    return std::nullopt;
}

static inline auto nanosecondsToMiliseconds(const uint64_t nanoseconds) -> uint64_t
{
    return nanoseconds / 1000u;
}

auto StateMachineTranslator::createTimerSetCall(const QString timerName, const uint64_t callTimeInNanoseconds)
        -> std::unique_ptr<::sdl::ProcedureCall>
{
    auto call = std::make_unique<::sdl::ProcedureCall>();
    call->setProcedure(&BUILT_IN_SET_TIMER_PROCEDURE);
    call->addArgument(
            std::make_unique<::sdl::VariableLiteral>(QString::number(nanosecondsToMiliseconds(callTimeInNanoseconds))));
    call->addArgument(std::make_unique<::sdl::VariableLiteral>(timerName));
    return std::move(call);
}

} // namespace conversion::sdl::translator
