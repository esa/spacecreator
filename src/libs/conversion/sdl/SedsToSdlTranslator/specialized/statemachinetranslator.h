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

#pragma once

#include "common.h"

#include <asn1library/asn1/asn1model.h>
#include <ivcore/ivmodel.h>
#include <map>
#include <sdl/SdlModel/newtype.h>
#include <sdl/SdlModel/procedurecall.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlModel/syntype.h>
#include <seds/SedsModel/components/states/entrystate.h>
#include <seds/SedsModel/components/states/exitstate.h>
#include <seds/SedsModel/sedsmodel.h>
#include <unordered_map>

namespace conversion {
class Options;
} // namespace conversion

namespace conversion::sdl::translator {

/**
 *  @brief Translator of SEDS state machines into SDL state machines
 */
class StateMachineTranslator final
{
public:
    /**
     * @brief   Translate the given SEDS state machine into SDL state machine
     *
     * @param context           Translation context
     * @param sedsStateMachine  State machine to be translated
     */
    static auto translateStateMachine(
            Context &context, const seds::model::StateMachine &sedsStateMachine, const Options &options) -> void;

    /**
     * @brief   Extract SEDS component implementation's variables into SDL variable declarations
     *
     * @param context           Translation context
     * @param variables         Variables to be translated
     * @param options           Conversion options
     */
    static auto translateVariables(Context &context, const seds::model::ComponentImplementation::VariableSet &variables,
            const Options &options) -> void;

    /**
     * @brief   Create timer variables
     *
     * @param context           Translation context
     * @param sedsStateMachine  State machine to derive the timers from
     */
    static auto createTimerVariables(Context &context, const seds::model::StateMachine &sedsStateMachine) -> void;

    /**
     * @brief   Create variables for storing input and output signal parameters
     *
     * @param context           Translation context
     */
    static auto createIoVariables(Context &context) -> void;

    /**
     * @brief   Create declarations of external procedures
     *
     * @param context           Translation context
     */
    static auto createExternalProcedures(Context &context) -> void;

    /**
     * @brief   Get name of the variable used for packing parameters of the given interface
     *
     * @param interfaceName     Interface name
     *
     * @return Variable name
     */
    static auto ioVariableName(const QString &interfaceName) -> QString;

    /**
     * @brief   Get name of the timer for a state
     *
     * @param stateName         State name
     *
     * @return Timer name
     */
    static auto timerName(const QString &stateName) -> QString;

    /**
     * @brief   Ensure that the state machine has at least the start state.
     *
     * @param context           Translation context
     */
    static auto ensureMinimalStateMachineExists(Context &context) -> void;

    /**
     * @brief  Translate parameter maps into getter/setter procedures or input/output signals
     *
     * @param context           Translation context
     * @param parameterMaps     Parameter maps
     * @param stateMachine      SEDS state machine
     * @param options           Conversion options
     */
    static auto translateParameterMaps(Context &context,
            const seds::model::ComponentImplementation::ParameterMapSet &parameterMaps,
            const ::seds::model::StateMachine &stateMachine, const Options &options) -> void;

    /**
     * @brief   Build mapping of names to command declarations
     *
     * @param context   Translation context which contains both the source component and the target map
     */
    static auto buildCommandMap(Context &context) -> void;

private:
    enum class ParameterType
    {
        Getter,
        Setter
    };

    enum class ParameterMode
    {
        Sync,
        Async
    };

    struct InputHandler {
        std::unique_ptr<::sdl::Input> input;
        std::vector<std::unique_ptr<::sdl::Action>> actions;
        bool isSporadic;
        bool isReturnCall;
    };

    struct TransitionInfo {
        ::seds::model::Transition const *sedsTransition;
        std::unique_ptr<::sdl::Transition> sdlTransition;
        std::unique_ptr<::sdl::Input> input;
        std::optional<QString> transactionName;
        bool isSporadic;
        bool isReturnCall;
        bool isFailed;
    };

    using InputsForStatesMap = std::unordered_map<QString, std::vector<StateMachineTranslator::TransitionInfo>>;

    static auto setInitialVariableValues(
            const seds::model::ComponentImplementation::VariableSet &variables, ::sdl::Transition *transition) -> void;

    static auto getAnyState(::sdl::StateMachine *stateMachine) -> ::sdl::State *;

    static auto getParameterInterface(ivm::IVFunction *function, const ParameterType type, const ParameterMode mode,
            const QString &interfaceName, const QString &parameterName) -> ivm::IVInterface *;

    static auto createParameterSyncPi(Context &context, ivm::IVInterface *ivInterface,
            const seds::model::ParameterMap &map, const std::vector<const ::seds::model::Transition *> &sedsTransitions,
            ::sdl::Process *sdlProcess, const ParameterType type, const Options &options) -> void;
    static auto createParameterAsyncPi(ivm::IVInterface *interface, const seds::model::ParameterMap &map,
            ::sdl::StateMachine *stateMachine) -> void;

    static auto translateParameter(Context &context, const seds::model::ParameterMap &map,
            const ::seds::model::StateMachine &stateMachine, const Options &options) -> void;

    static auto createStartTransition(Context &context, const seds::model::StateMachine &sedsStateMachine,
            std::map<QString, std::unique_ptr<::sdl::State>> &stateMap) -> void;

    static auto translateState(const seds::model::State &sedsState) -> std::unique_ptr<::sdl::State>;

    static auto translateState(const seds::model::ExitState &sedsState) -> std::unique_ptr<::sdl::State>;

    static auto translateState(const seds::model::EntryState &sedsState) -> std::unique_ptr<::sdl::State>;

    static auto translatePrimitive(
            Context &context, const seds::model::OnCommandPrimitive &command, const Options &options) -> InputHandler;

    static auto translatePrimitive(Context &context, const seds::model::OnParameterPrimitive &parameter,
            const Options &options) -> InputHandler;

    static auto translatePrimitive(Context &context, ::sdl::State *sdlFromState,
            const seds::model::Transition::Primitive &primitive, const Options &options) -> InputHandler;

    static auto translatePrimitive(::sdl::State *sdlFromState) -> InputHandler;

    static auto translateTransitions(Context &context,
            const std::vector<const ::seds::model::Transition *> sedsTransitions,
            std::map<QString, std::unique_ptr<::sdl::State>> &stateMap,
            const ::seds::model::StateMachine &sedsStateMachine, const Options &options)
            -> std::unordered_map<::sdl::State *, InputsForStatesMap>;
    static auto translateTransition(Context &context, const ::seds::model::Transition *sedsTransition,
            const ::sdl::State *sdlFromState, const ::sdl::State *sdlToState,
            std::vector<std::unique_ptr<::sdl::Action>> actions, const ::seds::model::StateMachine &sedsStateMachine)
            -> TransitionInfo;

    static auto createInputs(Context &context, ::sdl::State *fromState,
            std::unordered_map<QString, std::vector<StateMachineTranslator::TransitionInfo>> sdlTransitions,
            const Options &options) -> void;
    static auto createInput(
            Context &context, ::sdl::State *fromState, StateMachineTranslator::TransitionInfo transitionInfo) -> void;
    static auto createInputWithTransactions(Context &context, ::sdl::State *fromState,
            std::vector<StateMachineTranslator::TransitionInfo> transitionInfos) -> void;
    static auto createInputWithFailureReporting(Context &context, ::sdl::State *fromState,
            std::vector<StateMachineTranslator::TransitionInfo> transitionInfos) -> void;

    static auto createIoVariable(ivm::IVInterface const *interface, ::sdl::Process *sdlProcess) -> void;

    static auto createExternalProcedure(ivm::IVInterface const *interface, ::sdl::Process *sdlProcess) -> void;

    static auto translateGuard(::sdl::Process *sdlProcess, const ::sdl::State *fromState,
            ::sdl::Transition *currentTransitionPtr, const seds::model::BooleanExpression &guard)
            -> ::sdl::Transition *;

    static auto getTimerInvocationTime(const seds::model::StateMachine &sedsStateMachine, const QString &stateName)
            -> std::optional<uint64_t>;

    static auto createTimerSetCall(QString timerName, const uint64_t callTimeInNanoseconds)
            -> std::unique_ptr<::sdl::ProcedureCall>;

    static auto handleVariableArrayDimensions(Context &context, const QString &variableName,
            const QString &variableTypeName, const std::vector<::seds::model::DimensionSize> &arrayDimensions)
            -> QString;
    static auto createVariableSizeDimensionIndexingType(
            const uint64_t size, const QString &variableName, const QString &baseTypeName) -> ::sdl::Syntype;
    static auto createVariableSizeDimensionType(const uint64_t size, const QString &variableName,
            const QString &indexingTypeName, const QString &elementTypeName) -> ::sdl::Newtype;
    static auto createVariableTypeDimensionType(const QString &typeName, const QString &variableName,
            const QString &indexingTypeName, const QString &elementTypeName) -> ::sdl::Newtype;

    static auto getSdlState(const ::seds::model::StateRef &sedsState,
            std::map<QString, std::unique_ptr<::sdl::State>> &stateMap) -> ::sdl::State *;
    static auto getStateInput(const ::sdl::State *state, const QString &inputName) -> ::sdl::Input *;
};

} // namespace conversion::sdl::translator
