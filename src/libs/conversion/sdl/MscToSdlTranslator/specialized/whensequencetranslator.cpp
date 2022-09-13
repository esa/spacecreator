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

#include "specialized/whensequencetranslator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <msccore/msccondition.h>
#include <sdl/SdlModel/nextstate.h>

using conversion::translator::TranslationException;
using ivm::IVModel;
using msc::MscChart;
using msc::MscCondition;
using msc::MscEntity;
using msc::MscInstanceEvent;
using msc::MscMessage;
using sdl::Input;
using sdl::NextState;
using sdl::Rename;
using sdl::SdlModel;
using sdl::State;
using sdl::StateMachine;
using sdl::Transition;

namespace conversion::sdl::translator {

WhenSequenceTranslator::WhenSequenceTranslator(
        SdlModel *sdlModel, const Asn1Acn::File *observerAsn1File, const IVModel *ivModel, const Options &options)
    : SequenceTranslator(sdlModel, observerAsn1File, ivModel, options)
{
}

void WhenSequenceTranslator::createObserver(const MscChart *mscChart)
{
    auto context = collectData(mscChart);

    if (context.mode == Mode::When) {
        auto errorMessage = QString("When sequence without 'then' or 'then-not' in chart %1").arg(context.chartName);
        throw TranslationException(std::move(errorMessage));
    }

    auto stateMachine = createStateMachine(context);

    auto process = createSdlProcess(context.chartName, std::move(stateMachine));
    if (context.mode == Mode::Then) {
        process.addErrorState(m_errorStateName);
        process.addSuccessState(context.endState->name());
    } else if (context.mode == Mode::ThenNot) {
        process.addErrorState(context.endState->name());
    }

    auto system = createSdlSystem(context.chartName, std::move(process));
    for (auto &signalRename : context.signalRenames) {
        system.addSignal(std::move(signalRename));
    }
    system.createRoutes(m_defaultChannelName, m_defaultRouteName);

    m_sdlModel->addSystem(std::move(system));
}

WhenSequenceTranslator::Context WhenSequenceTranslator::collectData(const MscChart *mscChart) const
{
    Context context;
    context.chartName = Escaper::escapeSdlName(mscChart->name());
    context.signalCounter = 0;
    context.mode = Mode::When;

    const auto &mscEvents = mscChart->instanceEvents();
    for (auto it = std::next(mscEvents.begin()); it != mscEvents.end(); ++it) {
        handleEvent(context, *it);
    }

    return context;
}

void WhenSequenceTranslator::handleEvent(
        WhenSequenceTranslator::Context &context, const MscInstanceEvent *mscEvent) const
{
    switch (mscEvent->entityType()) {
    case MscEntity::EntityType::Message: {
        const auto mscMessageEvent = dynamic_cast<const MscMessage *>(mscEvent);
        handleMessageEvent(context, mscMessageEvent);
    } break;
    case MscEntity::EntityType::Condition: {
        const auto mscConditionEvent = dynamic_cast<const MscCondition *>(mscEvent);
        handleConditionEvent(context, mscConditionEvent);
    } break;
    case MscEntity::EntityType::Coregion: {
        throw TranslationException("Coregion for when observer is not yet implemented");
    } break;
    case MscEntity::EntityType::Comment:
        break;
    default: {
        auto errorMessage = QString("Encountered unsupported %1 event in when observer %2")
                                    .arg(mscEvent->entityTypeName())
                                    .arg(context.chartName);
        throw TranslationException(std::move(errorMessage));
    }
    }
}

void WhenSequenceTranslator::handleMessageEvent(
        WhenSequenceTranslator::Context &context, const MscMessage *mscMessage) const
{
    auto signalRenamed = std::find_if(context.signalRenames.begin(), context.signalRenames.end(),
            [&](const auto &signalRename) { return signalRename->referencedName() == mscMessage->name(); });

    if (signalRenamed == context.signalRenames.end()) {
        const auto name = m_signalRenameNameTemplate.arg(context.signalCounter);
        context.signalRenames.push_back(renameSignal(name, mscMessage));
        signalRenamed = std::prev(context.signalRenames.end());
    }

    const auto renamedSignal = (*signalRenamed).get();
    const auto &parameterList = mscMessage->parameters();

    uint32_t sequenceValue = 0;

    auto signalCaptured = std::find_if(context.signals.begin(), context.signals.end(), [&](const auto &sig) {
        const auto &signalInfo = sig.second;
        return signalInfo.signal == renamedSignal && signalInfo.parameterList == parameterList;
    });

    if (signalCaptured == context.signals.end()) {
        SignalInfo signalInfo;
        signalInfo.signal = (*signalRenamed).get();
        signalInfo.parameterList = parameterList;

        context.signals.insert({ context.signalCounter, std::move(signalInfo) });
        sequenceValue = context.signalCounter++;
    } else {
        sequenceValue = signalCaptured->first;
    }

    if (context.mode == Mode::When) {
        context.whenSequence.push_back(sequenceValue);
    } else {
        context.thenSequence.push_back(sequenceValue);
    }
}

void WhenSequenceTranslator::handleConditionEvent(
        WhenSequenceTranslator::Context &context, const MscCondition *mscCondition) const
{
    if (!mscCondition->shared()) {
        auto errorMessage =
                QString("Encountered a condition that is not shared in when observer %1").arg(context.chartName);
        throw TranslationException(std::move(errorMessage));
    }

    if (context.mode == Mode::Then) {
        auto errorMessage = QString("Encountered a condition while already handling 'then' in when observer %1")
                                    .arg(context.chartName);
        throw TranslationException(std::move(errorMessage));
    } else if (context.mode == Mode::ThenNot) {
        auto errorMessage = QString("Encountered a condition while already handling 'then not' in when observer %1")
                                    .arg(context.chartName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &mscConditionName = mscCondition->name();

    if (mscConditionName.toLower() == m_observerNameThen) {
        context.mode = Mode::Then;
    } else if (mscConditionName.toLower() == m_observerNameThenNot) {
        context.mode = Mode::ThenNot;
    } else {
        auto errorMessage = QString("Encountered a condition with unknown name '%1' in when observer %2")
                                    .arg(mscConditionName)
                                    .arg(context.chartName);
        throw TranslationException(std::move(errorMessage));
    }
}

std::unique_ptr<StateMachine> WhenSequenceTranslator::createStateMachine(WhenSequenceTranslator::Context &context) const
{
    Q_UNUSED(context);

    auto stateMachine = std::make_unique<StateMachine>();

    auto states = createStates(context.whenSequence.size() + context.thenSequence.size());
    context.endState = states.back().get();

    MscParameterValueParser messageParser(context.chartName, m_observerAsn1File);
    const auto &signalRequirements = messageParser.parseSignals(context.signals);

    TFTable table(context.whenSequence, context.signals.size());
    auto whenTransitions = createTransitions(table, states, context.signals, signalRequirements, 0);

    for (auto &transition : whenTransitions) {
        stateMachine->addTransition(std::move(transition));
    }

    const auto requirementStartStateId = context.whenSequence.size();

    if (context.mode == Mode::Then) {
        auto thenTransitions =
                createThenTransitions(context, states, context.signals, signalRequirements, requirementStartStateId);
        for (auto &transition : thenTransitions) {
            stateMachine->addTransition(std::move(transition));
        }
    } else if (context.mode == Mode::ThenNot) {
        auto thenNotTransitions =
                createThenNotTransitions(context, states, context.signals, signalRequirements, requirementStartStateId);
        for (auto &transition : thenNotTransitions) {
            stateMachine->addTransition(std::move(transition));
        }
    }

    for (auto &state : states) {
        stateMachine->addState(std::move(state));
    }

    return stateMachine;
}

WhenSequenceTranslator::TransitionList WhenSequenceTranslator::createThenTransitions(
        WhenSequenceTranslator::Context &context, StateList &states, const SignalsMap &signals,
        const MscParameterValueParser::SignalRequirementsMap &signalRequirements, const uint32_t startStateId) const
{
    auto errorState = std::make_unique<State>();
    errorState->setName(m_errorStateName);

    auto errorTransition = std::make_unique<Transition>();
    errorTransition->addAction(std::make_unique<NextState>(errorState->name(), errorState.get()));

    std::vector<std::unique_ptr<Transition>> transitions;

    uint32_t stateId = startStateId;

    for (const auto &signalId : context.thenSequence) {
        const auto &signalInfo = signals.at(signalId);
        const auto signal = signalInfo.signal;
        const auto &signalName = signal->name();

        auto sourceState = states.at(stateId).get();
        const auto targetState = states.at(stateId + 1).get();

        const auto parameterCount = signal->parametersTypes().size();
        auto transition = createTransitionOnSignal(signalName, parameterCount, sourceState);

        const auto signalHasRequirements = (signalRequirements.count(signalId) != 0);
        if (signalHasRequirements) {
            const auto &parametersRequirements = signalRequirements.at(signalId);
            auto signalRequirementAction = createSignalRequirements(parametersRequirements, targetState);

            transition->addAction(std::move(signalRequirementAction));

            auto errorStateAction = std::make_unique<NextState>(errorState->name(), errorState.get());
            transition->addAction(std::move(errorStateAction));
        } else {
            transition->addAction(std::make_unique<NextState>(targetState->name(), targetState));
        }

        transitions.push_back(std::move(transition));

        auto errorInput = std::make_unique<Input>();
        errorInput->setName(m_anySignalName);
        errorInput->setTransition(errorTransition.get());
        sourceState->addInput(std::move(errorInput));

        ++stateId;
    }

    states.push_back(std::move(errorState));
    transitions.push_back(std::move(errorTransition));

    return transitions;
}

WhenSequenceTranslator::TransitionList WhenSequenceTranslator::createThenNotTransitions(
        WhenSequenceTranslator::Context &context, StateList &states, const SignalsMap &signals,
        const MscParameterValueParser::SignalRequirementsMap &signalRequirements, const uint32_t startStateId) const
{
    auto okState = std::make_unique<State>();
    okState->setName(m_okStateName);

    auto okTransition = std::make_unique<Transition>();
    okTransition->addAction(std::make_unique<NextState>(okState->name(), okState.get()));

    std::vector<std::unique_ptr<Transition>> transitions;

    uint32_t stateId = startStateId;

    for (const auto &signalId : context.thenSequence) {
        const auto &signalInfo = signals.at(signalId);
        const auto signal = signalInfo.signal;
        const auto &signalName = signal->name();

        auto sourceState = states.at(stateId).get();
        const auto targetState = states.at(stateId + 1).get();

        const auto parameterCount = signal->parametersTypes().size();
        auto transition = createTransitionOnSignal(signalName, parameterCount, sourceState);

        const auto signalHasRequirements = (signalRequirements.count(signalId) != 0);
        if (signalHasRequirements) {
            const auto &parametersRequirements = signalRequirements.at(signalId);
            auto signalRequirementAction = createSignalRequirements(parametersRequirements, targetState);

            transition->addAction(std::move(signalRequirementAction));

            auto okStateAction = std::make_unique<NextState>(okState->name(), okState.get());
            transition->addAction(std::move(okStateAction));
        } else {
            transition->addAction(std::make_unique<NextState>(targetState->name(), targetState));
        }

        transitions.push_back(std::move(transition));

        auto okInput = std::make_unique<Input>();
        okInput->setName(m_anySignalName);
        okInput->setTransition(okTransition.get());
        sourceState->addInput(std::move(okInput));

        ++stateId;
    }

    states.push_back(std::move(okState));
    transitions.push_back(std::move(okTransition));

    return transitions;
}

} // namespace conversion::sdl::translator
