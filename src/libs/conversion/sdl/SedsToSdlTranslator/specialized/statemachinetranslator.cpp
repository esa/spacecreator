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

#include "statemachinetranslator.h"

#include "constants.h"
#include "specialized/descriptiontranslator.h"
#include "specialized/statementtranslatorvisitor.h"

#include <algorithm>
#include <conversion/asn1/SedsToAsn1Translator/constants.h>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/overloaded.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/SedsToIvTranslator/interfacetranslatorhelper.h>
#include <conversion/iv/SedsToIvTranslator/translator.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/task.h>
#include <seds/SedsOptions/options.h>

using conversion::Escaper;
using conversion::asn1::translator::seds::SedsToAsn1Translator;
using conversion::iv::translator::seds::InterfaceTranslatorHelper;
using conversion::seds::SedsOptions;
using conversion::translator::MissingAsn1TypeDefinitionException;
using conversion::translator::MissingInterfaceViewFunctionException;
using conversion::translator::TranslationException;

using SedsCommandPrimitiveFilterFunc = std::function<bool(const ::seds::model::OnCommandPrimitive *)>;
using SedsParameterPrimitiveFilterFunc = std::function<bool(const ::seds::model::OnParameterPrimitive *)>;

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
static inline auto getElementOfName(const ::seds::model::StateMachine &sedsStateMachine, const QString &name)
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

static inline auto getOnExit(const ::seds::model::StateMachine &sedsStateMachine, const QString &name)
        -> std::optional<const ::seds::model::ActivityInvocation *>
{
    // OnExit applies only to states, not entry or exit states
    const auto &state = getElementOfName<::seds::model::State>(sedsStateMachine, name);
    if (!state.has_value()) {
        return std::nullopt;
    }
    if (!(*state)->onExit().has_value()) {
        return std::nullopt;
    }

    return &(*((*state)->onExit()));
}

static inline auto getOnEntry(const ::seds::model::StateMachine &sedsStateMachine, const QString &name)
        -> std::optional<const ::seds::model::ActivityInvocation *>
{
    // OnEntry applies only to states, not entry or exit states
    const auto &state = getElementOfName<::seds::model::State>(sedsStateMachine, name);
    if (!state.has_value()) {
        return std::nullopt;
    }
    if (!(*state)->onEntry().has_value()) {
        return std::nullopt;
    }

    return &(*((*state)->onEntry()));
}

static inline auto getInterfaceByName(ivm::IVFunction *function, const QString &name) -> ivm::IVInterface *
{
    for (auto interface : function->allInterfaces()) {
        if (interface->title() == name) {
            return interface;
        }
    }
    return nullptr;
}

static inline auto getTransitionsForCommand(const ::seds::model::StateMachine &sedsStateMachine,
        const QString &interface, const QString &command) -> std::vector<const ::seds::model::Transition *>
{
    std::vector<const ::seds::model::Transition *> result;
    for (const auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<::seds::model::Transition>(element)) {
            const auto &transition = std::get<::seds::model::Transition>(element);
            if (std::holds_alternative<::seds::model::OnCommandPrimitive>(transition.primitive())) {
                const auto &onCommandPrimitive = std::get<::seds::model::OnCommandPrimitive>(transition.primitive());
                const auto &interfaceName = onCommandPrimitive.interface().value();
                const auto &commandName = onCommandPrimitive.command().value();
                if (interfaceName == interface && commandName == command) {
                    result.push_back(&transition);
                }
            }
        }
    }
    return result;
}

static inline auto getTransitionsForParameter(const ::seds::model::StateMachine &sedsStateMachine,
        const QString &interface, const QString &parameter, const ::seds::model::ParameterOperation operation)
        -> std::vector<const ::seds::model::Transition *>
{
    std::vector<const ::seds::model::Transition *> result;

    for (const auto &element : sedsStateMachine.elements()) {
        const auto transition = std::get_if<::seds::model::Transition>(&element);
        if (transition != nullptr) {
            const auto onParameterPrimitive =
                    std::get_if<::seds::model::OnParameterPrimitive>(&transition->primitive());
            if (onParameterPrimitive != nullptr) {
                const auto &interfaceName = onParameterPrimitive->interface().value();
                const auto &parameterName = onParameterPrimitive->parameter().value();
                const auto &parameterOperation = onParameterPrimitive->operation();
                if (interfaceName == interface && parameterName == parameter && parameterOperation == operation) {
                    result.push_back(transition);
                }
            }
        }
    }

    return result;
}

static inline auto areCommandTransactionsRequired(const std::vector<const ::seds::model::Transition *> &transitions)
        -> bool
{
    if (transitions.empty()) {
        throw TranslationException("Sync commands with no associated transitions are not supported");
    }

    const auto firstTransition = transitions.front();

    // primitive is now guaranteed to be OnCommandPrimitive
    const auto primitive = std::get_if<::seds::model::OnCommandPrimitive>(&firstTransition->primitive());
    if (primitive == nullptr) {
        throw TranslationException(
                "Unknown translator bug: set of Transitions filtered for OnCommandPrimitive contains "
                "a transition which is not OnCommandPrimitive");
    }

    for (const auto &transition : transitions) {
        if (!transition->doActivity().has_value()) {
            throw TranslationException(
                    "Sync commands with transitions without associated activities are not supported");
        }
    }

    if (primitive->transaction().has_value()) {
        return true;
    }

    // Only one transition, but it doesn't have transaction
    if (transitions.size() == 1) {
        return false;
    }

    for (const auto &otherTransition : transitions) {
        const auto otherPrimitive = std::get_if<::seds::model::OnCommandPrimitive>(&otherTransition->primitive());
        if (otherPrimitive == nullptr) {
            throw TranslationException(
                    "Unknown translator bug: set of Transitions filtered for OnCommandPrimitive contains "
                    "a transition which is not OnCommandPrimitive");
        }

        if (primitive->argumentValues().size() != otherPrimitive->argumentValues().size()) {
            return true;
        }

        for (size_t i = 0; i < primitive->argumentValues().size(); i++) {
            const auto &argumentName = primitive->argumentValues()[i].name().value();
            const auto &otherArgumentName = otherPrimitive->argumentValues()[i].name().value();

            if (argumentName != otherArgumentName) {
                return true;
            }

            const auto &outputVariable = primitive->argumentValues()[i].outputVariableRef().value().value();
            const auto &otherOutputVariable = otherPrimitive->argumentValues()[i].outputVariableRef().value().value();

            if (outputVariable != otherOutputVariable) {
                return true;
            }
        }
    }

    // doActivity optional is now guaranteed to have a value
    const auto invocation = &(*(firstTransition->doActivity()));

    for (const auto &otherTransition : transitions) {
        const auto otherInvocation = &(*(otherTransition->doActivity()));

        if (invocation->activity().value() != otherInvocation->activity().value()) {
            return true;
        }

        if (invocation->argumentValues().size() != otherInvocation->argumentValues().size()) {
            return true;
        }

        for (size_t i = 0; i < invocation->argumentValues().size(); i++) {
            const auto &argument = invocation->argumentValues()[i];
            const auto &otherArgument = otherInvocation->argumentValues()[i];

            if (argument.name().value() != otherArgument.name().value()) {
                return true;
            }
        }
    }

    return false;
}

static inline auto areParameterTransactionsRequired(const std::vector<const ::seds::model::Transition *> &transitions)
        -> bool
{
    if (transitions.empty()) {
        return false;
    }

    const auto firstTransition = transitions.front();

    // primitive is now guaranteed to be OnParameterPrimitive
    const auto primitive = std::get_if<::seds::model::OnParameterPrimitive>(&firstTransition->primitive());
    if (primitive == nullptr) {
        throw TranslationException(
                "Unknown translator bug: set of Transitions filtered for OnParameterPrimitive contains "
                "a transition which is not OnParameterPrimitive");
    }

    if (primitive->transaction().has_value()) {
        return true;
    }

    // Only one transition, but it doesn't have transaction
    if (transitions.size() == 1) {
        return false;
    }

    for (const auto &otherTransition : transitions) {
        const auto otherPrimitive = std::get_if<::seds::model::OnParameterPrimitive>(&otherTransition->primitive());
        if (otherPrimitive == nullptr) {
            throw TranslationException(
                    "Unknown translator bug: set of Transitions filtered for OnParameterPrimitive contains "
                    "a transition which is not OnParameterPrimitive");
        }

        if (primitive->variableRef() != otherPrimitive->variableRef()) {
            return true;
        }
    }

    const auto hasActivity = firstTransition->doActivity().has_value();

    for (const auto &otherTransition : transitions) {
        const auto otherHasActivity = otherTransition->doActivity().has_value();

        if (hasActivity != otherHasActivity) {
            return true;
        }

        if (hasActivity) {
            const auto &invocation = firstTransition->doActivity().value();
            const auto &otherInvocation = otherTransition->doActivity().value();

            if (invocation.activity().value() != otherInvocation.activity().value()) {
                return true;
            }

            if (invocation.argumentValues().size() != otherInvocation.argumentValues().size()) {
                return true;
            }

            for (size_t i = 0; i < invocation.argumentValues().size(); i++) {
                const auto &argument = invocation.argumentValues()[i];
                const auto &otherArgument = otherInvocation.argumentValues()[i];

                if (argument.name().value() != otherArgument.name().value()) {
                    return true;
                }
            }
        }
    }

    return false;
}

static inline auto createTransactionsElements(Context &context, ::sdl::Procedure *procedure,
        ::ivm::IVInterface *ivInterface, const QString &transactionVariableName, const Options &options)
{
    if (!options.isSet(SedsOptions::transactionNameType)) {
        throw TranslationException(
                "SEDS transaction feature was used but no ASN.1 type for transaction name was specified");
    }

    const auto transactionParamTypeName = options.value(SedsOptions::transactionNameType).value();

    // Add transaction name parameter to the IV interface
    auto ivParameter = shared::InterfaceParameter(
            Constants::transactionParamName, shared::BasicParameter::Type::Other, transactionParamTypeName);
    ivInterface->addParam(ivParameter);

    // Add transaction name parameter to the interface procedure
    auto transactionParameter = std::make_unique<::sdl::ProcedureParameter>(
            Constants::transactionParamName, transactionParamTypeName, "in");
    procedure->addParameter(std::move(transactionParameter));

    // Add variable to pass transaction name outside of the procedure
    auto transactionVariable =
            std::make_unique<::sdl::VariableDeclaration>(transactionVariableName, transactionParamTypeName);
    auto sdlProcess = context.sdlProcess();
    sdlProcess->addVariable(std::move(transactionVariable));
}

static inline auto createCommandTransition(Context &context, const ::seds::model::Transition *sedsTransition)
        -> std::unique_ptr<::sdl::Transition>
{
    if (sedsTransition == nullptr) {
        throw TranslationException("Sync commands with no associated transitions are not supported");
    }

    if (!sedsTransition->doActivity().has_value()) {
        throw TranslationException("Sync commands with transition without associated activity are not supported");
    }

    const auto primitive = std::get_if<::seds::model::OnCommandPrimitive>(&sedsTransition->primitive());
    if (primitive == nullptr) {
        throw TranslationException(
                "Unknown translator bug: set of Transitions filtered for OnCommandPrimitive contains "
                "a transition which is not OnCommandPrimitive");
    }

    auto sdlTransition = std::make_unique<::sdl::Transition>();

    for (const auto &argument : primitive->argumentValues()) {
        const auto targetVariableName = Escaper::escapeSdlVariableName(argument.outputVariableRef().value().value());
        const auto fieldName = Escaper::escapeSdlVariableName(argument.name().value());

        sdlTransition->addAction(
                std::make_unique<::sdl::Task>("", QString("%1 := %2").arg(targetVariableName, fieldName)));
    }

    const auto &activityInvocation = sedsTransition->doActivity().value();

    auto call = StatementTranslatorVisitor::translateActivityCall(context.sdlProcess(), activityInvocation);
    sdlTransition->addAction(std::move(call));

    const auto &activityName = activityInvocation.activity().value();
    const auto activityInfo = context.getActivityInfo(activityName);

    if (activityInfo != nullptr) {
        for (const auto &assignment : activityInfo->returnAssignments()) {
            sdlTransition->addAction(std::make_unique<::sdl::Task>(
                    "", QString("%1 := %2").arg(assignment.left()).arg(assignment.right())));
        }
    }

    return sdlTransition;
}

static inline auto createCommandTransactionDecision(Context &context,
        const std::vector<const ::seds::model::Transition *> &sedsTransitions,
        SedsCommandPrimitiveFilterFunc filterFunc) -> std::unique_ptr<::sdl::Decision>
{
    auto sdlDecisionExpression = std::make_unique<::sdl::Expression>(Constants::transactionParamName);
    auto sdlDecision = std::make_unique<::sdl::Decision>();
    sdlDecision->setExpression(std::move(sdlDecisionExpression));

    for (const auto sedsTransition : sedsTransitions) {
        const auto primitive = std::get_if<::seds::model::OnCommandPrimitive>(&sedsTransition->primitive());
        if (primitive == nullptr) {
            throw TranslationException(
                    "Unknown translator bug: set of Transitions filtered for OnCommandPrimitive contains "
                    "a transition which is not OnCommandPrimitive");
        }

        if (!filterFunc(primitive)) {
            continue;
        }

        const auto &transaction = primitive->transaction();
        if (!transaction.has_value()) {
            throw TranslationException("Missing transaction name in one of the command transitions");
        }
        const auto &transactionName = QString("\"%1\"").arg(transaction->value());

        auto sdlAnswerTransition = createCommandTransition(context, sedsTransition);

        auto sdlAnswer = std::make_unique<::sdl::Answer>();
        sdlAnswer->setLiteral(transactionName);
        sdlAnswer->setTransition(std::move(sdlAnswerTransition));

        sdlDecision->addAnswer(std::move(sdlAnswer));
    }

    auto sdlElseAnswer = std::make_unique<::sdl::Answer>();
    sdlElseAnswer->setLiteral(::sdl::Answer::ElseLiteral);
    sdlElseAnswer->setTransition(std::make_unique<::sdl::Transition>());
    sdlDecision->addAnswer(std::move(sdlElseAnswer));

    return sdlDecision;
}

static inline auto createCommandTransitionWithTransactions(Context &context,
        const std::vector<const ::seds::model::Transition *> &sedsTransitions, const QString &transactionVariableName)
        -> std::unique_ptr<::sdl::Transition>
{
    // Create decision based on the transaction
    auto sdlTransactionDecision = createCommandTransactionDecision(context, sedsTransitions, [](auto) { return true; });

    // Create result transition
    auto sdlTransition = std::make_unique<::sdl::Transition>();

    const auto transactionVariableAssignment =
            QString("%1 := %2").arg(transactionVariableName).arg(Constants::transactionParamName);
    auto transactionVariableAssignmentTask = std::make_unique<::sdl::Task>("", transactionVariableAssignment);
    sdlTransition->addAction(std::move(transactionVariableAssignmentTask));

    sdlTransition->addAction(std::move(sdlTransactionDecision));

    return sdlTransition;
}

static inline auto generateProcedureForSyncCommand(Context &context,
        const ::seds::model::StateMachine &sedsStateMachine, const QString &interfaceName,
        const ::seds::model::InterfaceCommand &command, const Options &options) -> void
{
    const auto sedsTransitions = getTransitionsForCommand(sedsStateMachine, interfaceName, command.nameStr());

    if (sedsTransitions.empty()) {
        return;
    }

    const auto &ivInterfaceName = InterfaceTranslatorHelper::buildCommandInterfaceName(
            interfaceName, command.nameStr(), ivm::IVInterface::InterfaceType::Provided, options);
    const auto &ivInterface = getInterfaceByName(context.ivFunction(), ivInterfaceName);

    auto procedure = std::make_unique<::sdl::Procedure>(ivInterfaceName);

    for (const auto &ivParameter : ivInterface->params()) {
        // TASTE skeleton generator generates upper-case names
        const auto parameterName = Escaper::escapeSdlVariableName(ivParameter.name());
        const auto &parameterType = Escaper::escapeSdlName(ivParameter.paramTypeName());
        const auto &parameterDirection =
                ivParameter.direction() == shared::InterfaceParameter::Direction::IN ? "in" : "in/out";
        auto sdlParameter =
                std::make_unique<::sdl::ProcedureParameter>(parameterName, parameterType, parameterDirection);
        procedure->addParameter(std::move(sdlParameter));
    }

    // If there are many transitions for the same command then they have to have a transaction name
    const auto transactionsRequired = areCommandTransactionsRequired(sedsTransitions);

    if (transactionsRequired) {
        const auto transactionVariableName = Constants::transactionVariableNameTemplate.arg(ivInterface->title());
        createTransactionsElements(context, procedure.get(), ivInterface, transactionVariableName, options);

        auto sdlTransition = createCommandTransitionWithTransactions(context, sedsTransitions, transactionVariableName);
        procedure->setTransition(std::move(sdlTransition));
    } else {
        auto sdlTransition = createCommandTransition(context, sedsTransitions.front());
        procedure->setTransition(std::move(sdlTransition));
    }

    context.sdlProcess()->addProcedure(std::move(procedure));
}

static inline auto buildCommandMapInternal(Context &context, const bool isProvided, const QString &interfaceName,
        const ::seds::model::InterfaceDeclaration &intefaceDeclaration) -> void
{
    for (const auto &command : intefaceDeclaration.commands()) {
        if (isProvided) {
            context.addProvidedCommand(interfaceName, command.nameStr(), &command);
        } else {
            context.addRequiredCommand(interfaceName, command.nameStr(), &command);
        }
    }

    for (const auto &baseInterface : intefaceDeclaration.baseInterfaces()) {
        const auto &baseIntefaceDeclaration = InterfaceTranslatorHelper::findInterfaceDeclaration(
                baseInterface.type(), context.sedsComponent(), &context.sedsPackage(), context.sedsPackages());
        buildCommandMapInternal(context, isProvided, interfaceName, baseIntefaceDeclaration);
    }
}

auto StateMachineTranslator::setInitialVariableValues(
        const ::seds::model::ComponentImplementation::VariableSet &variables, ::sdl::Transition *transition) -> void
{
    for (const auto &variable : variables) {
        if (variable.initialValue().has_value()) {
            const auto name = Escaper::escapeSdlVariableName(variable.nameStr());
            const auto value = variable.initialValue()->value();
            auto assignment = std::make_unique<::sdl::Task>("", QString("%1 := %2").arg(name, value));
            transition->addAction(std::move(assignment));
        }
    }
}

auto StateMachineTranslator::buildCommandMap(Context &context) -> void
{
    for (const auto &interface : context.sedsComponent().providedInterfaces()) {
        const auto &intefaceDeclaration = InterfaceTranslatorHelper::findInterfaceDeclaration(
                interface.type(), context.sedsComponent(), &context.sedsPackage(), context.sedsPackages());
        buildCommandMapInternal(context, true, interface.nameStr(), intefaceDeclaration);
    }
    for (const auto &interface : context.sedsComponent().requiredInterfaces()) {
        const auto &intefaceDeclaration = InterfaceTranslatorHelper::findInterfaceDeclaration(
                interface.type(), context.sedsComponent(), &context.sedsPackage(), context.sedsPackages());
        buildCommandMapInternal(context, false, interface.nameStr(), intefaceDeclaration);
    }
}

static inline auto generateProceduresForSyncCommands(
        Context &context, const ::seds::model::StateMachine &sedsStateMachine, const Options &options) -> void
{
    for (const auto &command : context.commands()) {
        if (command.second->interfaceType() == CommandInfo::HostInterfaceType::Provided
                && command.second->definition()->mode() == ::seds::model::InterfaceCommandMode::Sync) {
            // Procedures are generated only for sync commands in provided interfaces
            generateProcedureForSyncCommand(
                    context, sedsStateMachine, command.first, *command.second->definition(), options);
        }
    }
}

auto StateMachineTranslator::translateStateMachine(
        Context &context, const ::seds::model::StateMachine &sedsStateMachine, const Options &options) -> void
{
    // Consider rewriting this to filters when C++20 is supported
    std::map<QString, std::unique_ptr<::sdl::State>> stateMap;
    // First pass through states
    for (auto &sedsElement : sedsStateMachine.elements()) {
        // clang-format off
        std::visit(
            overloaded {
                [&](auto &&element) {
                    auto state = translateState(element);
                    DescriptionTranslator::translate(element, state.get());
                    stateMap.insert({ element.nameStr(), std::move(state) });
                },
                [&](const ::seds::model::Transition &transition) {
                    Q_UNUSED(transition);
                }
            }, sedsElement);
        // clang-format on
    }

    generateProceduresForSyncCommands(context, sedsStateMachine, options);

    // Setup the start transition
    createStartTransition(context, sedsStateMachine, stateMap);

    // Second pass through transitions
    std::vector<const ::seds::model::Transition *> sedsTransitions;

    for (const auto &element : sedsStateMachine.elements()) {
        const auto sedsTransition = std::get_if<::seds::model::Transition>(&element);

        if (sedsTransition != nullptr) {
            sedsTransitions.push_back(sedsTransition);
        }
    }

    auto sdlInputsForStates = translateTransitions(context, sedsTransitions, stateMap, sedsStateMachine, options);

    for (auto &[state, sdlTransitionsForInputs] : sdlInputsForStates) {
        createInputs(context, state, std::move(sdlTransitionsForInputs), options);
    }

    for (auto &entry : stateMap) {
        context.sdlStateMachine()->addState(std::move(entry.second));
    }
}

auto StateMachineTranslator::translateVariables(Context &context,
        const ::seds::model::ComponentImplementation::VariableSet &variables, const Options &options) -> void
{
    for (const auto &variable : variables) {
        const auto variableName = Escaper::escapeSdlVariableName(variable.nameStr());
        const auto variableType = variable.type();
        auto variableTypeName = Escaper::escapeAsn1TypeName(variableType.nameStr());

        const auto &arrayDimensions = variable.arrayDimensions();
        if (!arrayDimensions.empty()) {
            variableTypeName =
                    handleVariableArrayDimensions(context, variableName, variableTypeName, arrayDimensions, options);
        }

        auto sdlVariable =
                std::make_unique<::sdl::VariableDeclaration>(variableName, Escaper::escapeSdlName(variableTypeName));
        DescriptionTranslator::translate(variable, sdlVariable.get());

        context.sdlProcess()->addVariable(std::move(sdlVariable));
    }

    if (options.isSet(SedsOptions::enableFailureReporting)) {
        if (!options.isSet(SedsOptions::failureReportingType)) {
            throw TranslationException(
                    "SEDS failure reporting feature was used but no ASN.1 type for failure parameter was specified");
        }

        const auto failureVariableTypeName = options.value(SedsOptions::failureReportingType).value();

        const auto &failureVariableName = asn1::translator::seds::Constants::failureVariableName;
        auto failureVariable = std::make_unique<::sdl::VariableDeclaration>(
                failureVariableName, Escaper::escapeSdlName(failureVariableTypeName));
        context.sdlProcess()->addVariable(std::move(failureVariable));

        const auto &failureReturnVariableName = asn1::translator::seds::Constants::failureReturnVariableName;
        auto failureReturnVariable = std::make_unique<::sdl::VariableDeclaration>(
                failureReturnVariableName, Escaper::escapeSdlName(failureVariableTypeName));
        context.sdlProcess()->addVariable(std::move(failureReturnVariable));
    }
}

auto StateMachineTranslator::createTimerVariables(Context &context, const ::seds::model::StateMachine &sedsStateMachine)
        -> void
{
    std::set<QString> statesWithTimers;
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<::seds::model::Transition>(element)) {
            const auto &transition = std::get<::seds::model::Transition>(element);
            if (std::holds_alternative<::seds::model::TimerSink>(transition.primitive())) {
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
    return Escaper::escapeSdlVariableName(IO_VARIABLE_PATTERN.arg(interfaceName));
}

auto StateMachineTranslator::timerName(const QString &stateName) -> QString
{
    return Escaper::escapeSdlName(TIMER_NAME_PATTERN.arg(stateName));
}

auto StateMachineTranslator::ensureMinimalStateMachineExists(Context &context) -> void
{
    if (!context.sdlStateMachine()->states().empty()) {
        // State machine has at least one state, minimum is ensured.
        return;
    }
    auto state = std::make_unique<::sdl::State>();
    state->setName("Idle");
    auto transition = std::make_unique<::sdl::Transition>();
    setInitialVariableValues(context.sedsComponent().implementation().variables(), transition.get());
    transition->addAction(std::make_unique<::sdl::NextState>("", state.get()));
    context.sdlStateMachine()->addState(std::move(state));
    context.sdlProcess()->setStartTransition(std::move(transition));
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
        const ParameterMode mode, const QString &interfaceName, const QString &parameterName) -> ivm::IVInterface *
{
    const auto parameterType = type == ParameterType::Getter
            ? InterfaceTranslatorHelper::InterfaceParameterType::Getter
            : InterfaceTranslatorHelper::InterfaceParameterType::Setter;
    const auto name = InterfaceTranslatorHelper::buildParameterInterfaceName(
            interfaceName, parameterName, parameterType, ivm::IVInterface::InterfaceType::Provided);
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

static inline auto collectParameterTransactionNames(
        const std::vector<const ::seds::model::Transition *> &sedsTransitions,
        SedsParameterPrimitiveFilterFunc filterFunc) -> QStringList
{
    QStringList transactionNames;

    for (const auto sedsTransition : sedsTransitions) {
        const auto primitive = std::get_if<::seds::model::OnParameterPrimitive>(&sedsTransition->primitive());
        if (primitive == nullptr) {
            throw TranslationException(
                    "Unknown translator bug: set of Transitions filtered for OnParameterPrimitive contains "
                    "a transition which is not OnParameterPrimitive");
        }

        if (!filterFunc(primitive)) {
            continue;
        }

        const auto &transaction = primitive->transaction();
        if (!transaction.has_value()) {
            throw TranslationException("Missing transaction name in one of the parameter transitions");
        }
        const auto &transactionName = QString("\"%1\"").arg(transaction->value());
        transactionNames.push_back(transactionName);
    }

    return transactionNames;
}

static inline auto createParameterTransactionDecision(
        const std::vector<const ::seds::model::Transition *> &sedsTransitions, const QString &assignmentAction,
        SedsParameterPrimitiveFilterFunc filterFunc) -> std::unique_ptr<::sdl::Decision>
{
    const auto transactionNames = collectParameterTransactionNames(sedsTransitions, filterFunc);

    auto sdlDecisionExpression = std::make_unique<::sdl::Expression>(Constants::transactionParamName);
    auto sdlDecision = std::make_unique<::sdl::Decision>();
    sdlDecision->setExpression(std::move(sdlDecisionExpression));

    if (!transactionNames.empty()) {
        auto sdlAnswerTransition = std::make_unique<::sdl::Transition>();
        sdlAnswerTransition->addAction(std::make_unique<::sdl::Task>("", assignmentAction));

        auto sdlAnswer = std::make_unique<::sdl::Answer>();
        sdlAnswer->setLiteral(transactionNames.join(", "));
        sdlAnswer->setTransition(std::move(sdlAnswerTransition));

        sdlDecision->addAnswer(std::move(sdlAnswer));
    }

    auto sdlElseTransition = std::make_unique<::sdl::Transition>();
    auto sdlElseAnswer = std::make_unique<::sdl::Answer>();
    sdlElseAnswer->setLiteral(::sdl::Answer::ElseLiteral);
    sdlElseAnswer->setTransition(std::move(sdlElseTransition));

    sdlDecision->addAnswer(std::move(sdlElseAnswer));

    return sdlDecision;
}

static inline auto createParameterTransitionWithTransactions(
        const std::vector<const ::seds::model::Transition *> &sedsTransitions, const QString &assignmentAction,
        const QString &transactionVariableName) -> std::unique_ptr<::sdl::Transition>
{
    // Create a decision based on the transaction names
    auto sdlTransactionDecision =
            createParameterTransactionDecision(sedsTransitions, assignmentAction, [](auto) { return true; });

    // Create result transition
    auto sdlTransition = std::make_unique<::sdl::Transition>();

    auto transactionVariableAssignment = std::make_unique<::sdl::Task>(
            "", QString("%1 := %2").arg(transactionVariableName).arg(Constants::transactionParamName));
    sdlTransition->addAction(std::move(transactionVariableAssignment));

    sdlTransition->addAction(std::move(sdlTransactionDecision));

    return sdlTransition;
}

auto StateMachineTranslator::createParameterSyncPi(Context &context, ivm::IVInterface *ivInterface,
        const ::seds::model::ParameterMap &map, const std::vector<const ::seds::model::Transition *> &sedsTransitions,
        ::sdl::Process *sdlProcess, const ParameterType type, const Options &options) -> void
{

    auto procedure = std::make_unique<::sdl::Procedure>(ivInterface->title());

    for (const auto &ivParameter : ivInterface->params()) {
        const auto parameterName = Escaper::escapeSdlVariableName(ivParameter.name());
        const auto &parameterTypeName = Escaper::escapeSdlName(ivParameter.paramTypeName());
        const auto &parameterDirection =
                ivParameter.direction() == shared::InterfaceParameter::Direction::IN ? "in" : "in/out";

        auto procedureParameter =
                std::make_unique<::sdl::ProcedureParameter>(parameterName, parameterTypeName, parameterDirection);
        procedure->addParameter(std::move(procedureParameter));
    }

    const auto &firstParameter = procedure->parameters().front();
    const auto outputVariableName = Escaper::escapeSdlVariableName(map.variableRef().nameStr());

    const auto assignmentAction = [&]() {
        switch (type) {
        case ParameterType::Getter:
            return QString("%1 := %2").arg(firstParameter->name()).arg(outputVariableName);
        case ParameterType::Setter:
            return QString("%2 := %1").arg(firstParameter->name()).arg(outputVariableName);
        default:
            return QString();
        }
    }();

    const auto transactionsRequired = areParameterTransactionsRequired(sedsTransitions);

    auto sdlTransition = [&]() {
        if (transactionsRequired) {
            const auto transactionVariableName = QString("%1_transactionName").arg(ivInterface->title());
            createTransactionsElements(context, procedure.get(), ivInterface, transactionVariableName, options);

            return createParameterTransitionWithTransactions(
                    sedsTransitions, assignmentAction, transactionVariableName);
        } else {
            auto transition = std::make_unique<::sdl::Transition>();
            transition->addAction(std::make_unique<::sdl::Task>("", assignmentAction));
            return transition;
        }
    }();

    if (options.isSet(SedsOptions::enableFailureReporting)) {
        const auto &failureVariableName = asn1::translator::seds::Constants::failureParamName;

        const auto failureVariableAssignment = QString("%1 := false").arg(failureVariableName);
        auto failureVariableAssignmentTask = std::make_unique<::sdl::Task>("", failureVariableAssignment);
        sdlTransition->addAction(std::move(failureVariableAssignmentTask));
    }

    procedure->setTransition(std::move(sdlTransition));

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

auto StateMachineTranslator::createParameterAsyncPi(
        ivm::IVInterface *interface, const ::seds::model::ParameterMap &map, ::sdl::StateMachine *stateMachine) -> void
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
        input->addParameter(std::make_unique<::sdl::VariableReference>(variableName));
        const auto targetVariableName = Escaper::escapeSdlVariableName(map.variableRef().value().value());

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

auto StateMachineTranslator::translateParameter(Context &context, const ::seds::model::ParameterMap &map,
        const ::seds::model::StateMachine &sedsStateMachine, const Options &options) -> void
{
    // Sync Setters/Getters require a procedure
    // Associated transition (defined as an Input) is optionally translated
    // while handling OnParameterPrimitive
    // We depend on the SEDS -> IV translation

    const auto ivFunction = context.ivFunction();
    const auto sdlProcess = context.sdlProcess();
    const auto sdlStateMachine = context.sdlStateMachine();
    const auto &interfaceName = map.interface().value();
    const auto &parameterName = map.parameter().value();

    const auto syncGetter =
            getParameterInterface(ivFunction, ParameterType::Getter, ParameterMode::Sync, interfaceName, parameterName);
    if (syncGetter != nullptr) {
        const auto sedsTransitions = getTransitionsForParameter(
                sedsStateMachine, interfaceName, parameterName, ::seds::model::ParameterOperation::Get);
        createParameterSyncPi(context, syncGetter, map, sedsTransitions, sdlProcess, ParameterType::Getter, options);
    }
    const auto syncSetter =
            getParameterInterface(ivFunction, ParameterType::Setter, ParameterMode::Sync, interfaceName, parameterName);
    if (syncSetter != nullptr) {
        const auto sedsTransitions = getTransitionsForParameter(
                sedsStateMachine, interfaceName, parameterName, ::seds::model::ParameterOperation::Set);
        createParameterSyncPi(context, syncSetter, map, sedsTransitions, sdlProcess, ParameterType::Setter, options);
    }

    // Handle all async Setters/Getters not handled during onParameterPrimitiveTranslation
    const auto asyncSetter = getParameterInterface(
            ivFunction, ParameterType::Setter, ParameterMode::Async, interfaceName, parameterName);
    if (asyncSetter != nullptr) {
        createParameterAsyncPi(asyncSetter, map, sdlStateMachine);
    }
    const auto asyncGetter = getParameterInterface(
            ivFunction, ParameterType::Getter, ParameterMode::Async, interfaceName, parameterName);
    if (asyncGetter != nullptr) {
        createParameterAsyncPi(asyncGetter, map, sdlStateMachine);
    }
}

auto StateMachineTranslator::translateParameterMaps(Context &context,
        const ::seds::model::ComponentImplementation::ParameterMapSet &parameterMaps,
        const ::seds::model::StateMachine &stateMachine, const Options &options) -> void
{
    for (const auto &map : parameterMaps) {
        translateParameter(context, map, stateMachine, options);
    }
}

auto StateMachineTranslator::createStartTransition(Context &context,
        const ::seds::model::StateMachine &sedsStateMachine, std::map<QString, std::unique_ptr<::sdl::State>> &stateMap)
        -> void
{
    bool entryStateHandled = false;

    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<::seds::model::EntryState>(element)) {
            if (entryStateHandled) {
                throw TranslationException("Multiple entry states are not supported");
            }

            if (context.sdlProcess()->startTransition() == nullptr) {
                auto transition = std::make_unique<::sdl::Transition>();
                context.sdlProcess()->setStartTransition(std::move(transition));
            }

            auto transition = context.sdlProcess()->startTransition();
            DescriptionTranslator::translate(sedsStateMachine, transition);

            setInitialVariableValues(context.sedsComponent().implementation().variables(), transition);
            const auto &state = std::get<::seds::model::EntryState>(element);
            const auto timerTime = getTimerInvocationTime(sedsStateMachine, state.nameStr());
            if (timerTime.has_value()) {
                transition->addAction(createTimerSetCall(timerName(state.nameStr()), *timerTime));
            }
            transition->addAction(std::make_unique<::sdl::NextState>("", stateMap[state.nameStr()].get()));

            entryStateHandled = true;
        }
    }
}

auto StateMachineTranslator::translateState(const ::seds::model::State &sedsState) -> std::unique_ptr<::sdl::State>
{
    auto state = std::make_unique<::sdl::State>();
    state->setName(Escaper::escapeSdlName(sedsState.nameStr()));
    // Entry and exit procedures shall be translated for transitions
    return state;
}

auto StateMachineTranslator::translateState(const ::seds::model::ExitState &sedsState) -> std::unique_ptr<::sdl::State>
{
    auto state = std::make_unique<::sdl::State>();
    state->setName(Escaper::escapeSdlName(sedsState.nameStr()));
    // No Entry and exit procedures
    return state;
}

auto StateMachineTranslator::translateState(const ::seds::model::EntryState &sedsState) -> std::unique_ptr<::sdl::State>
{
    auto state = std::make_unique<::sdl::State>();
    state->setName(Escaper::escapeSdlName(sedsState.nameStr()));
    // No Entry and exit procedures
    return state;
}

auto StateMachineTranslator::translatePrimitive(Context &context,
        const ::seds::model::OnCommandPrimitive &commandPrimitive, const Options &options) -> InputHandler
{
    auto input = std::make_unique<::sdl::Input>();
    std::vector<std::unique_ptr<::sdl::Action>> unpackingActions;

    const auto interfaceName = commandPrimitive.interface().value();
    const auto commandName = commandPrimitive.command().value();

    // Input signal can be received only via a provided interface
    const auto &inputName = InterfaceTranslatorHelper::buildCommandInterfaceName(
            interfaceName, commandName, ivm::IVInterface::InterfaceType::Provided, options);
    input->setName(inputName);

    const auto ivInterface = getInterfaceByName(context.ivFunction(), inputName);
    if (ivInterface == nullptr) {
        throw TranslationException(QString("Interface %1 not found").arg(inputName));
    }

    const auto &command = context.getCommand(interfaceName, commandName);
    if (command == nullptr) {
        auto errorMessage = QString("Command '%1' of interface '%2' not found").arg(commandName, interfaceName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto isSporadic = ivInterface->kind() == ivm::IVInterface::OperationKind::Sporadic;
    const auto isReturnCall = command->interfaceType() == CommandInfo::HostInterfaceType::Required;

    if (commandPrimitive.argumentValues().empty()) {
        return { std::move(input), std::move(unpackingActions), isSporadic, isReturnCall };
    }

    if (isSporadic) {
        //--taste translation
        if (options.isSet(SedsOptions::tasteTranslation)) {
            if (commandPrimitive.argumentValues().size() != 1) {
                throw TranslationException(
                        QString("More than one(%1) command.argumentValue is not allowed for --taste translation option")
                                .arg(commandPrimitive.argumentValues().size()));
            }

            const auto &argument = commandPrimitive.argumentValues().at(0);
            const auto targetVariableName =
                    Escaper::escapeSdlVariableName(argument.outputVariableRef().value().value());
            input->addParameter(std::make_unique<::sdl::VariableReference>(targetVariableName));
            return { std::move(input), std::move(unpackingActions), isSporadic, isReturnCall };
        }

        const auto variableName = ioVariableName(input->name());
        input->addParameter(std::make_unique<::sdl::VariableReference>(variableName));

        for (const auto &argument : commandPrimitive.argumentValues()) {
            const auto targetVariableName =
                    Escaper::escapeSdlVariableName(argument.outputVariableRef().value().value());
            const auto fieldName = Escaper::escapeSdlVariableName(argument.name().value());
            unpackingActions.push_back(std::make_unique<::sdl::Task>(
                    "", QString("%1 := %2.%3").arg(targetVariableName, variableName, fieldName)));
        }

        return { std::move(input), std::move(unpackingActions), isSporadic, isReturnCall };
    } else {
        // Protected/unprotected - assignments done in the reception procedure
        return { std::move(input), std::move(unpackingActions), isSporadic, isReturnCall };
    }
}

auto StateMachineTranslator::translatePrimitive(
        Context &context, const ::seds::model::OnParameterPrimitive &parameter, const Options &options) -> InputHandler
{
    const auto parameterType = parameter.operation() == ::seds::model::ParameterOperation::Set
            ? InterfaceTranslatorHelper::InterfaceParameterType::Setter
            : InterfaceTranslatorHelper::InterfaceParameterType::Getter;
    auto input = std::make_unique<::sdl::Input>();
    std::vector<std::unique_ptr<::sdl::Action>> unpackingActions;

    // Input signal can be received only via a provided interface
    const auto name = InterfaceTranslatorHelper::buildParameterInterfaceName(parameter.interface().value(),
            parameter.parameter().value(), parameterType, ivm::IVInterface::InterfaceType::Provided);
    input->setName(name);

    const auto ivInterface = getInterfaceByName(context.ivFunction(), name);
    if (ivInterface == nullptr) {
        throw TranslationException(QString("Interface %1 not found").arg(name));
    }

    const bool isSporadic = ivInterface->kind() == ivm::IVInterface::OperationKind::Sporadic;

    if (isSporadic) {
        if (options.isSet(SedsOptions::enableFailureReporting)) {
            auto errorMessage = QString("Async parameters are not allowed when using SEDS failure reporting mechanism");
            throw TranslationException(std::move(errorMessage));
        }

        // This is a sporadic interface, so we must unpack the value.
        const auto variableName = ioVariableName(name);
        input->addParameter(std::make_unique<::sdl::VariableReference>(variableName));
        // Handle ParameterMap
        const auto &parameterMaps = context.sedsComponent().implementation().parameterMaps();
        const auto &map = std::find_if(parameterMaps.begin(), parameterMaps.end(), [&parameter](const auto &m) {
            return m.interface().value() == parameter.interface().value()
                    && m.parameter().value() == parameter.parameter().value();
        });
        if (map != parameterMaps.end()) {
            const auto targetVariableName = Escaper::escapeSdlVariableName(map->variableRef().value().value());
            unpackingActions.push_back(
                    std::make_unique<::sdl::Task>("", QString("%1 := %2").arg(targetVariableName, variableName)));
        }
        // Handle variable ref
        if (parameter.variableRef().has_value()) {
            const auto targetVariableName = Escaper::escapeSdlVariableName((*parameter.variableRef()).value().value());
            unpackingActions.push_back(
                    std::make_unique<::sdl::Task>("", QString("%1 := %2").arg(targetVariableName, variableName)));
        }
        // TODO support ParameterActivityMap
    } else {
        // This is a protected/unprotected interface, the value assignment is done in the associated procedure
        // and there are no parameters to unpack.
        if (parameter.variableRef().has_value()) {
            // The assignment must be done in a map.
            throw TranslationException(QString("VariableRef mapping is not supported for sync parameter %1").arg(name));
        }
    }
    return { std::move(input), std::move(unpackingActions), isSporadic, false };
}

auto StateMachineTranslator::translatePrimitive(::sdl::State *sdlFromState) -> InputHandler
{
    auto input = std::make_unique<::sdl::Input>();
    input->setName(TIMER_NAME_PATTERN.arg(sdlFromState->name()));
    return { std::move(input), std::vector<std::unique_ptr<::sdl::Action>>(), false, false };
}

auto StateMachineTranslator::translatePrimitive(Context &context, ::sdl::State *sdlFromState,
        const ::seds::model::Transition::Primitive &primitive, const Options &options) -> InputHandler
{
    // clang-format off
    return std::visit(
            overloaded {
                [&](const ::seds::model::OnCommandPrimitive &command) {
                        return translatePrimitive(context, command, options);
                    },
                [&](const ::seds::model::OnParameterPrimitive &parameter) {
                        return translatePrimitive(context, parameter, options);
                    },
                [&](const ::seds::model::TimerSink &) {
                        return translatePrimitive(sdlFromState);
                    }

            },
            primitive);
    // clang-format on
}

auto StateMachineTranslator::translateTransitions(Context &context,
        const std::vector<const ::seds::model::Transition *> sedsTransitions,
        std::map<QString, std::unique_ptr<::sdl::State>> &stateMap, const ::seds::model::StateMachine &sedsStateMachine,
        const Options &options) -> std::unordered_map<::sdl::State *, StateMachineTranslator::InputsForStatesMap>
{
    std::unordered_map<::sdl::State *, std::unordered_map<QString, std::vector<TransitionInfo>>> sdlInputsForStates;

    for (const auto sedsTransition : sedsTransitions) {
        const auto sdlFromState = getSdlState(sedsTransition->fromState(), stateMap);
        const auto sdlToState = getSdlState(sedsTransition->toState(), stateMap);

        auto [input, actions, isSporadic, isReturnCall] =
                translatePrimitive(context, sdlFromState, sedsTransition->primitive(), options);

        auto transitionInfo = translateTransition(
                context, sedsTransition, sdlFromState, sdlToState, std::move(actions), sedsStateMachine);

        transitionInfo.sedsTransition = sedsTransition;
        transitionInfo.isSporadic = isSporadic;
        transitionInfo.isReturnCall = isReturnCall;

        const auto &inputName = input->name();
        transitionInfo.input = std::move(input);

        sdlInputsForStates[sdlFromState][inputName].push_back(std::move(transitionInfo));
    }

    return sdlInputsForStates;
}

auto StateMachineTranslator::createInputs(Context &context, ::sdl::State *fromState,
        std::unordered_map<QString, std::vector<StateMachineTranslator::TransitionInfo>> sdlTransitionsForInputs,
        const Options &options) -> void
{
    for (auto &[inputName, transitionInfos] : sdlTransitionsForInputs) {
        // Some data are common for all transitions
        auto &firstTransitionInfo = transitionInfos.front();

        const auto isSporadic = firstTransitionInfo.isSporadic;
        const auto isReturnCall = firstTransitionInfo.isReturnCall;

        if (isSporadic) {
            if (isReturnCall && options.isSet(SedsOptions::enableFailureReporting)) {
                createInputWithFailureReporting(context, fromState, std::move(transitionInfos));
            } else {
                createInput(context, fromState, std::move(firstTransitionInfo));
            }
        } else {
            const auto transactionsRequired = firstTransitionInfo.transactionName.has_value();

            if (transactionsRequired) {
                createInputWithTransactions(context, fromState, std::move(transitionInfos));
            } else {
                createInput(context, fromState, std::move(firstTransitionInfo));
            }
        }
    }
}

auto StateMachineTranslator::createInput(
        Context &context, ::sdl::State *fromState, StateMachineTranslator::TransitionInfo transitionInfo) -> void
{
    auto &input = transitionInfo.input;
    input->setTransition(transitionInfo.sdlTransition.get());
    fromState->addInput(std::move(input));
    context.sdlStateMachine()->addTransition(std::move(transitionInfo.sdlTransition));
}

auto StateMachineTranslator::createInputWithTransactions(Context &context, ::sdl::State *fromState,
        std::vector<StateMachineTranslator::TransitionInfo> transitionInfos) -> void
{
    auto &sdlInput = transitionInfos.front().input;

    // Create decision based on the transaction
    auto transactionDecision = std::make_unique<::sdl::Decision>();

    const auto transactionVariableName = Constants::transactionVariableNameTemplate.arg(sdlInput->name());
    auto transactionDecisionExpression = std::make_unique<::sdl::Expression>(transactionVariableName);
    transactionDecision->setExpression(std::move(transactionDecisionExpression));

    // Create answer for each transition
    for (auto &transitionInfo : transitionInfos) {
        if (*sdlInput != *transitionInfo.input) {
            auto errorMessage = QString("Two inputs with name %1 are different").arg(sdlInput->name());
            throw TranslationException(std::move(errorMessage));
        }

        if (!transitionInfo.transactionName) {
            auto errorMessage =
                    QString("Transaction name is missing on one of the transitions of input %1").arg(sdlInput->name());
            throw TranslationException(std::move(errorMessage));
        }

        const auto &transactionName = QString("\"%1\"").arg(*transitionInfo.transactionName);

        auto transactionDecisionAnswer = std::make_unique<::sdl::Answer>();
        transactionDecisionAnswer->setLiteral(transactionName);
        transactionDecisionAnswer->setTransition(std::move(transitionInfo.sdlTransition));
        transactionDecision->addAnswer(std::move(transactionDecisionAnswer));
    }

    // Add else to the transaction decision
    auto transactionDecisionElseAnswerTransition = std::make_unique<::sdl::Transition>();
    transactionDecisionElseAnswerTransition->addAction(std::make_unique<::sdl::NextState>(""));

    auto transactionDecisionElseAnswer = std::make_unique<::sdl::Answer>();
    transactionDecisionElseAnswer->setLiteral(::sdl::Answer::ElseLiteral);
    transactionDecisionElseAnswer->setTransition(std::move(transactionDecisionElseAnswerTransition));
    transactionDecision->addAnswer(std::move(transactionDecisionElseAnswer));

    // Create result transition
    auto sdlTransition = std::make_unique<::sdl::Transition>();
    sdlTransition->addAction(std::move(transactionDecision));

    sdlInput->setTransition(sdlTransition.get());
    fromState->addInput(std::move(sdlInput));
    context.sdlStateMachine()->addTransition(std::move(sdlTransition));
}

auto StateMachineTranslator::createInputWithFailureReporting(Context &context, ::sdl::State *fromState,
        std::vector<StateMachineTranslator::TransitionInfo> transitionInfos) -> void
{
    auto &sdlInput = transitionInfos.front().input;

    // Create decision based on the failure reporting
    auto failureDecision = std::make_unique<::sdl::Decision>();

    const auto variableName = ioVariableName(sdlInput->name());
    const auto &failureFieldName = asn1::translator::seds::Constants::failureParamName;
    const auto failureVariableName = QString("%1.%2").arg(variableName).arg(failureFieldName);

    auto failureDecisionExpression = std::make_unique<::sdl::Expression>(failureVariableName);
    failureDecision->setExpression(std::move(failureDecisionExpression));

    if (transitionInfos.size() > 2) {
        auto errorMessage = QString(
                "Error while creating input, there are more than two transitions that have the same failure value");
        throw TranslationException(std::move(errorMessage));
    }

    // Create answer for each transition
    for (auto &transitionInfo : transitionInfos) {
        if (*sdlInput != *transitionInfo.input) {
            auto errorMessage = QString("Two inputs with name %1 are different").arg(sdlInput->name());
            throw TranslationException(std::move(errorMessage));
        }

        if (!transitionInfo.isSporadic) {
            auto errorMessage = QString("Found transition for input %1 that is not sporadic").arg(sdlInput->name());
            throw TranslationException(std::move(errorMessage));
        }

        const QString &failureValue = transitionInfo.isFailed ? "true" : "false";

        auto failureDecisionAnswer = std::make_unique<::sdl::Answer>();
        failureDecisionAnswer->setLiteral(failureValue);
        failureDecisionAnswer->setTransition(std::move(transitionInfo.sdlTransition));
        failureDecision->addAnswer(std::move(failureDecisionAnswer));
    }

    // It means we need to create an else transition
    if (failureDecision->answers().size() != 2) {
        auto failureDecisionElseAnswer = std::make_unique<::sdl::Answer>();
        failureDecisionElseAnswer->setLiteral(::sdl::Answer::ElseLiteral);

        auto failureDecisionElseAnswerTransition = std::make_unique<::sdl::Transition>();
        failureDecisionElseAnswerTransition->addAction(std::make_unique<::sdl::NextState>(""));
        failureDecisionElseAnswer->setTransition(std::move(failureDecisionElseAnswerTransition));

        failureDecision->addAnswer(std::move(failureDecisionElseAnswer));
    }

    // Create result transition
    auto sdlTransition = std::make_unique<::sdl::Transition>();
    sdlTransition->addAction(std::move(failureDecision));

    sdlInput->setTransition(sdlTransition.get());
    fromState->addInput(std::move(sdlInput));
    context.sdlStateMachine()->addTransition(std::move(sdlTransition));
}

auto StateMachineTranslator::translateTransition(Context &context, const ::seds::model::Transition *sedsTransition,
        const ::sdl::State *sdlFromState, const ::sdl::State *sdlToState,
        std::vector<std::unique_ptr<::sdl::Action>> actions, const ::seds::model::StateMachine &sedsStateMachine)
        -> StateMachineTranslator::TransitionInfo
{
    TransitionInfo transitionInfo;

    transitionInfo.sdlTransition = std::make_unique<::sdl::Transition>();
    auto sdlTransition = transitionInfo.sdlTransition.get();

    DescriptionTranslator::translate(*sedsTransition, sdlTransition);

    bool invokeDoActivity = true;
    if (std::holds_alternative<::seds::model::OnCommandPrimitive>(sedsTransition->primitive())) {
        const auto &onCommandPrimitive = std::get<::seds::model::OnCommandPrimitive>(sedsTransition->primitive());

        const auto command =
                context.getCommand(onCommandPrimitive.interface().value(), onCommandPrimitive.command().value());
        if (command == nullptr) {
            throw TranslationException(
                    QString("Transition on undefined command %1").arg(onCommandPrimitive.command().value()));
        }

        if (command->definition()->mode() == ::seds::model::InterfaceCommandMode::Sync) {
            invokeDoActivity = false;
        }

        const auto &transaction = onCommandPrimitive.transaction();
        if (transaction) {
            transitionInfo.transactionName = transaction->value();
        }

        transitionInfo.isFailed = onCommandPrimitive.isFailed();
    } else if (std::holds_alternative<::seds::model::OnParameterPrimitive>(sedsTransition->primitive())) {
        const auto &onParameterPrimitive = std::get<::seds::model::OnParameterPrimitive>(sedsTransition->primitive());

        const auto &transaction = onParameterPrimitive.transaction();
        if (transaction) {
            transitionInfo.transactionName = transaction->value();
        }

        transitionInfo.isFailed = onParameterPrimitive.isFailed();
    }

    // Argument unpacking
    for (auto &action : actions) {
        sdlTransition->addAction(std::move(action));
    }

    if (sedsTransition->guard().has_value()) {
        sdlTransition =
                translateGuard(context, context.sdlProcess(), sdlFromState, sdlTransition, *sedsTransition->guard());
    }

    const auto stateChange = sdlFromState->name() != sdlToState->name();

    if (stateChange) {
        const auto onExit = getOnExit(sedsStateMachine, sedsTransition->fromState().nameStr());
        if (onExit.has_value()) {
            sdlTransition->addAction(StatementTranslatorVisitor::translateActivityCall(context.sdlProcess(), **onExit));
        }
    }

    if (invokeDoActivity && sedsTransition->doActivity().has_value()) {
        sdlTransition->addAction(
                StatementTranslatorVisitor::translateActivityCall(context.sdlProcess(), *sedsTransition->doActivity()));
    }
    if (stateChange) {
        const auto onEntry = getOnEntry(sedsStateMachine, sedsTransition->toState().nameStr());
        if (onEntry.has_value()) {
            sdlTransition->addAction(
                    StatementTranslatorVisitor::translateActivityCall(context.sdlProcess(), **onEntry));
        }
    }

    const auto timerTime = getTimerInvocationTime(sedsStateMachine, sedsTransition->toState().nameStr());
    if (timerTime.has_value()) {
        sdlTransition->addAction(createTimerSetCall(timerName(sedsTransition->toState().nameStr()), *timerTime));
    }

    // State switch
    sdlTransition->addAction(std::make_unique<::sdl::NextState>("", sdlToState));

    return transitionInfo;
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
    sdlProcess->addVariable(
            std::make_unique<::sdl::VariableDeclaration>(variableName, Escaper::escapeSdlName(param.paramTypeName())));
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

auto StateMachineTranslator::translateGuard(Context &context, ::sdl::Process *sdlProcess, const ::sdl::State *fromState,
        ::sdl::Transition *currentTransitionPtr, const ::seds::model::BooleanExpression &guard) -> ::sdl::Transition *
{
    auto decision = StatementTranslatorVisitor::translateBooleanExpression(context, sdlProcess, nullptr, guard);

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
        const ::seds::model::StateMachine &sedsStateMachine, const QString &stateName) -> std::optional<uint64_t>
{
    // TODO - consider supporting multiple different OnTimer events for a state timed separately
    for (auto &element : sedsStateMachine.elements()) {
        if (std::holds_alternative<::seds::model::Transition>(element)) {
            const auto &transition = std::get<::seds::model::Transition>(element);
            if (transition.fromState().nameStr() != stateName) {
                continue;
            }
            if (std::holds_alternative<::seds::model::TimerSink>(transition.primitive())) {
                return std::get<::seds::model::TimerSink>(transition.primitive()).nanosecondsAfterEntry();
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

auto StateMachineTranslator::handleVariableArrayDimensions(Context &context, const QString &variableName,
        const QString &variableTypeName, const std::vector<::seds::model::DimensionSize> &arrayDimensions,
        const Options &options) -> QString
{
    auto lastName = variableName;
    auto lastIndexingTypeName = QString("%1_Index").arg(variableName);
    auto lastElementTypeName = variableTypeName;

    const auto baseIndexingTypeName = options.value(SedsOptions::arrayDimensionBaseIndexingType);
    if (!baseIndexingTypeName) {
        auto errorMessage = QString("Variable '%1' uses ArrayDimensions but no base indexing type name was provided")
                                    .arg(variableName);
        throw TranslationException(std::move(errorMessage));
    }

    for (auto it = arrayDimensions.rbegin(); it != arrayDimensions.rend(); ++it) {
        const auto &arrayDimension = *it;

        if (arrayDimension.size()) {
            const auto &dimensionSize = arrayDimension.size()->value();

            auto indexingType =
                    createVariableSizeDimensionIndexingType(dimensionSize, lastIndexingTypeName, *baseIndexingTypeName);
            auto arrayType =
                    createVariableSizeDimensionType(dimensionSize, lastName, indexingType.name(), lastElementTypeName);

            lastName = arrayType.name();
            lastIndexingTypeName = indexingType.name();
            lastElementTypeName = arrayType.name();

            context.sdlProcess()->addSyntype(std::move(indexingType));
            context.sdlProcess()->addNewtype(std::move(arrayType));
        } else if (arrayDimension.indexTypeRef()) {
            const auto &dimensionTypeName = arrayDimension.indexTypeRef()->nameStr();

            auto arrayType = createVariableTypeDimensionType(
                    dimensionTypeName, lastName, dimensionTypeName, lastElementTypeName);

            lastName = arrayType.name();
            lastIndexingTypeName = QString("%1_%2").arg(lastIndexingTypeName).arg(dimensionTypeName);
            lastElementTypeName = arrayType.name();

            context.sdlProcess()->addNewtype(std::move(arrayType));
        } else {
            auto errorMessage = QString("Found variable array dimension without size nor indexTypeRef");
            throw TranslationException(std::move(errorMessage));
        }
    }

    return lastElementTypeName;
}

auto StateMachineTranslator::createVariableSizeDimensionIndexingType(
        const uint64_t size, const QString &variableName, const QString &baseTypeName) -> ::sdl::Syntype
{
    auto indexTypeName = QString("%1_%2").arg(variableName).arg(size);

    ::sdl::Syntype indexType(indexTypeName, baseTypeName);
    indexType.addValueConstant(size);

    return indexType;
}

auto StateMachineTranslator::createVariableSizeDimensionType(const uint64_t size, const QString &variableName,
        const QString &indexingTypeName, const QString &elementTypeName) -> ::sdl::Newtype
{
    auto indexTypeName = QString("%1_%2").arg(variableName).arg(size);

    ::sdl::Newtype arrayType(indexTypeName, indexingTypeName, elementTypeName);

    return arrayType;
}

auto StateMachineTranslator::createVariableTypeDimensionType(const QString &typeName, const QString &variableName,
        const QString &indexingTypeName, const QString &elementTypeName) -> ::sdl::Newtype
{
    auto indexTypeName = QString("%1_%2").arg(variableName).arg(typeName);

    ::sdl::Newtype arrayType(indexTypeName, indexingTypeName, elementTypeName);

    return arrayType;
}

auto StateMachineTranslator::getSdlState(const ::seds::model::StateRef &sedsState,
        std::map<QString, std::unique_ptr<::sdl::State>> &stateMap) -> ::sdl::State *
{
    const auto stateName = Escaper::escapeSdlName(sedsState.nameStr());

    const auto stateFound = stateMap.find(stateName);
    if (stateFound == stateMap.end()) {
        auto errorMessage = QString("Unknown state %1 when translating transition").arg(stateName);
        throw TranslationException(std::move(errorMessage));
    }

    return stateFound->second.get();
}

auto StateMachineTranslator::getStateInput(const ::sdl::State *state, const QString &inputName) -> ::sdl::Input *
{
    const auto &stateInputs = state->inputs();

    const auto inputFound = std::find_if(
            stateInputs.begin(), stateInputs.end(), [&](const auto &input) { return input->name() == inputName; });

    if (inputFound == stateInputs.end()) {
        return nullptr;
    } else {
        return (*inputFound).get();
    }
}

} // namespace conversion::sdl::translator
