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
#include <conversion/iv/SedsToIvTranslator/specialized/interfaceparametertranslator.h>
#include <conversion/iv/SedsToIvTranslator/translator.h>
#include <iostream>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/task.h>

using conversion::Escaper;
using conversion::asn1::translator::SedsToAsn1Translator;
using conversion::iv::translator::InterfaceCommandTranslator;
using conversion::iv::translator::InterfaceParameterTranslator;
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

StateMachineTranslator::Context::Context(const seds::model::Package &sedsPackage,
        const seds::model::Component &sedsComponent, Asn1Acn::Asn1Model *asn1Model, ivm::IVFunction *ivFunction,
        ::sdl::Process *sdlProcess, ::sdl::StateMachine *sdlStateMachine)
    : m_sedsPackage(sedsPackage)
    , m_sedsComponent(sedsComponent)
    , m_asn1Model(asn1Model)
    , m_ivFunction(ivFunction)
    , m_sdlProcess(sdlProcess)
    , m_sdlStateMachine(sdlStateMachine)
{
}

auto StateMachineTranslator::Context::sedsPackage() -> const seds::model::Package &
{
    return m_sedsPackage;
}

auto StateMachineTranslator::Context::sedsComponent() -> const seds::model::Component &
{
    return m_sedsComponent;
}

auto StateMachineTranslator::Context::asn1Model() -> Asn1Acn::Asn1Model *
{
    return m_asn1Model;
}

auto StateMachineTranslator::Context::ivFunction() -> ivm::IVFunction *
{
    return m_ivFunction;
}

auto StateMachineTranslator::Context::sdlProcess() -> ::sdl::Process *
{
    return m_sdlProcess;
}

auto StateMachineTranslator::Context::sdlStateMachine() -> ::sdl::StateMachine *
{
    return m_sdlStateMachine;
}

template<typename ElementType>
static inline auto getElementOfName(const seds::model::StateMachine &sedsStateMachine, const QString &name)
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

static inline auto getOnExit(const seds::model::StateMachine &sedsStateMachine, const QString &name)
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

static inline auto getOnEntry(const seds::model::StateMachine &sedsStateMachine, const QString &name)
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

auto StateMachineTranslator::translateStateMachine(Context &context, const seds::model::StateMachine &sedsStateMachine)
        -> void
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
    createStartTransition(sedsStateMachine, context.sdlProcess(), stateMap);

    // Second pass through transitions
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<seds::model::Transition>(element)) {
            translateTransition(context, sedsStateMachine, std::get<seds::model::Transition>(element), stateMap);
        }
    }
    for (auto &entry : stateMap) {
        context.sdlStateMachine()->addState(std::move(entry.second));
    }
}

auto StateMachineTranslator::translateVariables(
        Context &context, const seds::model::ComponentImplementation::VariableSet &variables) -> void
{
    for (const auto &variable : variables) {
        const auto variableName = Escaper::escapeAsn1FieldName(variable.nameStr());
        const auto variableTypeName = Escaper::escapeAsn1TypeName(variable.type().nameStr());
        // TODO implement check for types imported from other packages
        auto asn1Definitions = SedsToAsn1Translator::getAsn1Definitions(context.sedsPackage(), context.asn1Model());
        const auto *referencedType = asn1Definitions->type(variableTypeName);
        if (referencedType == nullptr) {
            throw MissingAsn1TypeDefinitionException(variableTypeName);
        }
        context.sdlProcess()->addVariable(std::make_unique<::sdl::VariableDeclaration>(variableName, variableTypeName));
    }
}

auto StateMachineTranslator::createTimerVariables(Context &context, const seds::model::StateMachine &sedsStateMachine)
        -> void
{
    std::set<QString> statesWithTimers;
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<seds::model::Transition>(element)) {
            const auto &transition = std::get<seds::model::Transition>(element);
            if (std::holds_alternative<seds::model::TimerSink>(transition.primitive())) {
                const auto stateName = transition.fromState().nameStr();
                if (statesWithTimers.find(stateName) == statesWithTimers.end()) {
                    statesWithTimers.insert(stateName);
                    context.sdlProcess()->addTimer(timerName(stateName));
                }
            }
        }
    }
}

auto StateMachineTranslator::createIoVariables(Context &context) -> void
{
    for (const auto &interface : context.ivFunction()->interfaces()) {
        if (interface->kind() == ivm::IVInterface::OperationKind::Sporadic) {
            createIoVariable(interface, context.sdlProcess());
        }
    }
}

auto StateMachineTranslator::createExternalProcedures(Context &context) -> void
{
    for (const auto &interface : context.ivFunction()->interfaces()) {
        if (interface->isRequired() && interface->kind() == ivm::IVInterface::OperationKind::Protected) {
            createExternalProcedure(interface, context.sdlProcess());
        }
    }
}

auto StateMachineTranslator::ioVariableName(const QString &interfaceName) -> QString
{
    return IO_VARIABLE_PATTERN.arg(interfaceName);
}

auto StateMachineTranslator::timerName(const QString &stateName) -> QString
{
    return Escaper::escapeSdlName(TIMER_NAME_PATTERN.arg(stateName));
}

auto StateMachineTranslator::ensureMinimalStateMachineExists(Context &context) -> void
{
    if (context.sdlStateMachine()->states().size() > 0) {
        // State machine has at least one state, minimum is ensured.
        return;
    }
    auto state = std::make_unique<::sdl::State>();
    state->setName("Idle");
    auto transition = std::make_unique<::sdl::Transition>();
    transition->addAction(std::make_unique<::sdl::NextState>("", state.get()));
    context.sdlStateMachine()->addState(std::move(state));
    context.sdlProcess()->setStartTransition(std::move(transition));
}

static inline auto getInterfaceByName(ivm::IVFunction *function, QString name) -> ivm::IVInterface *
{
    for (auto interface : function->allInterfaces()) {
        if (interface->title() == name) {
            return interface;
        }
    }
    return nullptr;
}

auto StateMachineTranslator::getAnyState(::sdl::StateMachine *stateMachine) -> ::sdl::State *
{
    const auto &anyState = std::find_if(stateMachine->states().begin(), stateMachine->states().end(),
            [&](const std::unique_ptr<::sdl::State> &state) { return state->name() == "*"; });
    if (anyState != stateMachine->states().end()) {
        return anyState->get();
    }
    return nullptr;
}

auto StateMachineTranslator::getParameterInterface(ivm::IVFunction *function, const ParameterType type,
        const ParameterMode mode, const QString interfaceName, const QString parameterName) -> ivm::IVInterface *
{
    const auto typeName = type == ParameterType::Getter ? "Get" : "Set";
    const auto name = Escaper::escapeIvName(QString("%1_%2_%3_Pi").arg(typeName).arg(interfaceName).arg(parameterName));
    auto interface = getInterfaceByName(function, name);
    if (interface == nullptr) {
        return nullptr;
    }
    switch (mode) {
    case ParameterMode::Async:
        // We are interested in an async (sporadic) interface
        return interface->kind() == ivm::IVInterface::OperationKind::Sporadic ? interface : nullptr;
    case ParameterMode::Sync:
        // We are interested in a sync (protected/unprotected) interface
        return interface->kind() == ivm::IVInterface::OperationKind::Sporadic ? nullptr : interface;
    }
    return nullptr;
}

auto StateMachineTranslator::createParameterSyncGetter(
        ivm::IVInterface *interface, const seds::model::ParameterMap &map, ::sdl::Process *sdlProcess) -> void
{
    const auto paramName = Escaper::escapeAsn1FieldName(interface->params()[0].name());
    auto procedure = std::make_unique<::sdl::Procedure>(interface->title());
    auto parameter =
            std::make_unique<::sdl::ProcedureParameter>(paramName, interface->params()[0].paramTypeName(), "in/out");
    procedure->addParameter(std::move(parameter));
    auto transition = std::make_unique<::sdl::Transition>();
    const auto action =
            QString("%1 := %2").arg(paramName, Escaper::escapeAsn1FieldName(map.variableRef().value().value()));
    transition->addAction(std::make_unique<::sdl::Task>("", action));
    procedure->setTransition(std::move(transition));
    sdlProcess->addProcedure(std::move(procedure));
}

auto StateMachineTranslator::createParameterSyncSetter(
        ivm::IVInterface *interface, const seds::model::ParameterMap &map, ::sdl::Process *sdlProcess) -> void
{
    const auto paramName = Escaper::escapeAsn1FieldName(interface->params()[0].name());
    auto procedure = std::make_unique<::sdl::Procedure>(interface->title());
    auto parameter =
            std::make_unique<::sdl::ProcedureParameter>(paramName, interface->params()[0].paramTypeName(), "in");
    procedure->addParameter(std::move(parameter));
    auto transition = std::make_unique<::sdl::Transition>();
    const auto action =
            QString("%1 := %2").arg(Escaper::escapeAsn1FieldName(map.variableRef().value().value()), paramName);
    transition->addAction(std::make_unique<::sdl::Task>("", action));
    procedure->setTransition(std::move(transition));
    sdlProcess->addProcedure(std::move(procedure));
}

static inline auto getInputOfName(::sdl::State *state, const QString name) -> ::sdl::Input *
{
    const auto &input = std::find_if(
            state->inputs().begin(), state->inputs().end(), [&](const auto &i) { return i->name() == name; });
    if (input == state->inputs().end()) {
        return nullptr;
    }
    return input->get();
}

auto StateMachineTranslator::createParameterAsyncPi(ivm::IVInterface *interface, const seds::model::ParameterMap &map,
        ::sdl::Process *sdlProcess, ::sdl::StateMachine *stateMachine) -> void
{
    for (const auto &state : stateMachine->states()) {
        const auto &existingInput = getInputOfName(state.get(), interface->title());
        if (existingInput != nullptr) {
            // Setter was already generated for the given state
            continue;
        }
        auto input = std::make_unique<::sdl::Input>();
        input->setName(interface->title());
        const auto variableName = ioVariableName(input->name());
        const auto &variableIterator = std::find_if(sdlProcess->variables().begin(), sdlProcess->variables().end(),
                [variableName](const auto &variable) { return variable->name() == variableName; });
        if (variableIterator == sdlProcess->variables().end()) {
            throw TranslationException(QString("Reception variable %1 not found").arg(variableName));
        }
        input->addParameter(std::make_unique<::sdl::VariableReference>((*variableIterator).get()));
        const auto targetVariableName = Escaper::escapeAsn1FieldName(map.variableRef().value().value());

        auto transition = std::make_unique<::sdl::Transition>();
        auto transitionPtr = transition.get();
        transitionPtr->addAction(
                std::make_unique<::sdl::Task>("", QString("%1 := %2").arg(targetVariableName, variableName)));
        transitionPtr->addAction(std::make_unique<::sdl::NextState>("", state.get()));
        input->setTransition(transitionPtr);
        stateMachine->addTransition(std::move(transition));
        state->addInput(std::move(input));
    }
}

auto StateMachineTranslator::translateParameter(Context &context, const seds::model::ParameterMap &map) -> void
{
    // Sync Setters/Getters require a procedure
    // Associated transition (defined as an Input) is optionally translated
    // while handling OnParameterPrimitive
    // We depend on the SEDS -> IV translation
    const auto syncGetter = getParameterInterface(context.ivFunction(), ParameterType::Getter, ParameterMode::Sync,
            map.interface().value(), map.parameter().value());
    if (syncGetter != nullptr) {
        createParameterSyncGetter(syncGetter, map, context.sdlProcess());
    }
    const auto syncSetter = getParameterInterface(context.ivFunction(), ParameterType::Setter, ParameterMode::Sync,
            map.interface().value(), map.parameter().value());
    if (syncSetter != nullptr) {
        createParameterSyncSetter(syncSetter, map, context.sdlProcess());
    }
    // Handle all async Setters/Getters not handled during onParameterPrimitiveTranslation
    const auto asyncSetter = getParameterInterface(context.ivFunction(), ParameterType::Setter, ParameterMode::Async,
            map.interface().value(), map.parameter().value());
    if (asyncSetter != nullptr) {
        createParameterAsyncPi(asyncSetter, map, context.sdlProcess(), context.sdlStateMachine());
    }
    const auto asyncGetter = getParameterInterface(context.ivFunction(), ParameterType::Getter, ParameterMode::Async,
            map.interface().value(), map.parameter().value());
    if (asyncGetter != nullptr) {
        createParameterAsyncPi(asyncGetter, map, context.sdlProcess(), context.sdlStateMachine());
    }
}

auto StateMachineTranslator::translateParameterMaps(
        Context &context, const seds::model::ComponentImplementation::ParameterMapSet &parameterMaps) -> void
{
    for (const auto &map : parameterMaps) {
        translateParameter(context, map);
    }
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

auto StateMachineTranslator::translatePrimitive(Context &context, const seds::model::OnCommandPrimitive &command)
        -> InputHandler
{
    auto sdlProcess = context.sdlProcess();
    auto input = std::make_unique<::sdl::Input>();
    std::vector<std::unique_ptr<::sdl::Action>> unpackingActions;

    // Input signal can be received only via a provided interface
    input->setName(InterfaceCommandTranslator::getCommandName(
            command.interface().value(), ivm::IVInterface::InterfaceType::Provided, command.command().value()));
    if (command.argumentValues().empty()) {
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

auto StateMachineTranslator::translatePrimitive(Context &context, const seds::model::OnParameterPrimitive &parameter)
        -> InputHandler
{
    auto sdlProcess = context.sdlProcess();
    const auto mode = parameter.operation() == seds::model::ParameterOperation::Set
            ? InterfaceParameterTranslator::InterfaceMode::Setter
            : InterfaceParameterTranslator::InterfaceMode::Getter;
    auto input = std::make_unique<::sdl::Input>();
    std::vector<std::unique_ptr<::sdl::Action>> unpackingActions;

    // Input signal can be received only via a provided interface
    const auto name = InterfaceParameterTranslator::getParameterName(mode, parameter.interface().value(),
            ivm::IVInterface::InterfaceType::Provided, parameter.parameter().value());
    input->setName(name);
    const auto interface = getInterfaceByName(context.ivFunction(), name);
    if (interface == nullptr) {
        throw TranslationException(QString("Interface %1 not found").arg(name));
    }
    const bool isSporadic = interface->kind() == ivm::IVInterface::OperationKind::Sporadic;

    // TODO support variable ref in OnParameterPrimitive
    // TODO support ParameterActivityMap
    if (isSporadic) {
        // This is a sporadic interface, so we must unpack the value.
        // For protected/unprotected interfaces, the value assignment is done in the associated procedure
        // and there are no parameters to unpack.
        const auto variableName = ioVariableName(name);
        const auto &variableIterator = std::find_if(sdlProcess->variables().begin(), sdlProcess->variables().end(),
                [variableName](const auto &variable) { return variable->name() == variableName; });
        if (variableIterator == sdlProcess->variables().end()) {
            throw TranslationException(QString("Reception variable %1 not found").arg(variableName));
        }
        input->addParameter(std::make_unique<::sdl::VariableReference>((*variableIterator).get()));
        const auto &parameterMaps = context.sedsComponent().implementation().parameterMaps();
        const auto &map = std::find_if(parameterMaps.begin(), parameterMaps.end(), [&parameter](const auto &m) {
            return m.interface().value() == parameter.interface().value()
                    && m.parameter().value() == parameter.parameter().value();
        });
        if (map == parameterMaps.end()) {
            throw TranslationException(QString("Parameter map for input %1 not found").arg(name));
        }
        const auto targetVariableName = Escaper::escapeAsn1FieldName(map->variableRef().value().value());
        unpackingActions.push_back(
                std::make_unique<::sdl::Task>("", QString("%1 := %2").arg(targetVariableName, variableName)));
    }
    return std::make_pair(std::move(input), std::move(unpackingActions));
}

auto StateMachineTranslator::translatePrimitive(::sdl::State *sdlFromState) -> InputHandler
{
    auto input = std::make_unique<::sdl::Input>();
    input->setName(TIMER_NAME_PATTERN.arg(sdlFromState->name()));
    return std::make_pair(std::move(input), std::vector<std::unique_ptr<::sdl::Action>>());
}

auto StateMachineTranslator::translatePrimitive(Context &context, ::sdl::State *sdlFromState,
        const seds::model::Transition::Primitive &primitive) -> InputHandler
{
    // clang-format off
    return std::visit(
            overloaded {
                [&context](const seds::model::OnCommandPrimitive &command) {
                        return translatePrimitive(context, command);
                    },
                [&context](const seds::model::OnParameterPrimitive &parameter) {
                        return translatePrimitive(context, parameter);
                    },
                [&sdlFromState](const seds::model::TimerSink &) {
                        return translatePrimitive(sdlFromState);
                    }

            },
            primitive);
    // clang-format on
}

auto StateMachineTranslator::translateTransition(Context &context, const seds::model::StateMachine &sedsStateMachine,
        const seds::model::Transition &sedsTransition, std::map<QString, std::unique_ptr<::sdl::State>> &stateMap)
        -> void
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

    auto inputHandler = translatePrimitive(context, sdlFromState, sedsTransition.primitive());

    auto mainTransition = std::make_unique<::sdl::Transition>();
    auto currentTransitionPtr = mainTransition.get();
    inputHandler.first->setTransition(mainTransition.get());
    sdlFromState->addInput(std::move(inputHandler.first));
    // Argument unpacking
    for (auto &action : inputHandler.second) {
        currentTransitionPtr->addAction(std::move(action));
    }

    if (sedsTransition.guard().has_value()) {
        currentTransitionPtr =
                translateGuard(context.sdlProcess(), sdlFromState, currentTransitionPtr, *sedsTransition.guard());
    }

    if (stateChange) {
        const auto onExit = getOnExit(sedsStateMachine, sedsTransition.fromState().nameStr());
        if (onExit.has_value()) {
            currentTransitionPtr->addAction(
                    StatementTranslatorVisitor::translateActivityCall(context.sdlProcess(), **onExit));
        }
    }

    if (sedsTransition.doActivity().has_value()) {
        currentTransitionPtr->addAction(
                StatementTranslatorVisitor::translateActivityCall(context.sdlProcess(), *sedsTransition.doActivity()));
    }
    if (stateChange) {
        const auto onEntry = getOnEntry(sedsStateMachine, sedsTransition.toState().nameStr());
        if (onEntry.has_value()) {
            currentTransitionPtr->addAction(
                    StatementTranslatorVisitor::translateActivityCall(context.sdlProcess(), **onEntry));
        }
        const auto timerTime = getTimerInvocationTime(sedsStateMachine, sedsTransition.toState().nameStr());
        if (timerTime.has_value()) {
            currentTransitionPtr->addAction(
                    createTimerSetCall(timerName(sedsTransition.toState().nameStr()), *timerTime));
        }
    }
    // State switch
    currentTransitionPtr->addAction(std::make_unique<::sdl::NextState>("", sdlToState));

    context.sdlStateMachine()->addTransition(std::move(mainTransition));
}

auto StateMachineTranslator::createIoVariable(ivm::IVInterface const *interface, ::sdl::Process *sdlProcess) -> void
{
    if (interface->params().empty()) {
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
        const seds::model::StateMachine &sedsStateMachine, const QString &stateName) -> std::optional<uint64_t>
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
    return nanoseconds / 1000U; // NOLINT
}

auto StateMachineTranslator::createTimerSetCall(QString timerName, const uint64_t callTimeInNanoseconds)
        -> std::unique_ptr<::sdl::ProcedureCall>
{
    auto call = std::make_unique<::sdl::ProcedureCall>();
    call->setProcedure(&BUILT_IN_SET_TIMER_PROCEDURE);
    call->addArgument(
            std::make_unique<::sdl::VariableLiteral>(QString::number(nanosecondsToMiliseconds(callTimeInNanoseconds))));
    call->addArgument(std::make_unique<::sdl::VariableLiteral>(std::move(timerName)));
    return call;
}

} // namespace conversion::sdl::translator
