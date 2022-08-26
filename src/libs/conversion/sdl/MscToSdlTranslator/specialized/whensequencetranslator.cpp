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
    for (auto &[id, signalInfo] : context.signals) {
        system.addSignal(std::move(signalInfo.signal));
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
    const auto signalRenamed = std::find_if(context.signals.begin(), context.signals.end(), [&](auto &&sig) {
        return sig.second.signal->referencedName() == mscMessage->name()
                && sig.second.parameterList == mscMessage->parameters();
    });

    uint32_t sequenceValue = 0;

    if (signalRenamed == context.signals.end()) {
        auto signalRename = std::make_unique<Rename>();
        signalRename->setName(m_signalRenameNameTemplate.arg(context.signalCounter));
        signalRename->setDirection(Rename::Direction::Input);
        signalRename->setReferencedName(Escaper::escapeSdlName(mscMessage->name()));
        signalRename->setReferencedFunctionName(Escaper::escapeSdlName(mscMessage->targetInstance()->name()));

        SignalInfo signalInfo;
        signalInfo.signal = std::move(signalRename);
        signalInfo.parameterList = mscMessage->parameters();

        context.signals.insert({ context.signalCounter, std::move(signalInfo) });

        sequenceValue = context.signalCounter++;
    } else {
        sequenceValue = signalRenamed->first;
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
    auto stateMachine = std::make_unique<StateMachine>();

    auto states = createStates(context.whenSequence.size() + context.thenSequence.size());
    context.endState = states.back().get();

    TFTable table(context.whenSequence, context.signals.size());
    auto whenTransitions = createTransitions(table, states, 0);
    for (auto &transition : whenTransitions) {
        stateMachine->addTransition(std::move(transition));
    }

    const auto requirementStartStateId = context.whenSequence.size();

    if (context.mode == Mode::Then) {
        auto thenTransitions = createThenTransitions(context, states, requirementStartStateId);
        for (auto &transition : thenTransitions) {
            stateMachine->addTransition(std::move(transition));
        }
    } else if (context.mode == Mode::ThenNot) {
        auto thenNotTransitions = createThenNotTransitions(context, states, requirementStartStateId);
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
        WhenSequenceTranslator::Context &context, StateList &states, const uint32_t startStateId) const
{
    auto errorState = std::make_unique<State>();
    errorState->setName(m_errorStateName);

    auto errorTransition = std::make_unique<Transition>();
    errorTransition->addAction(std::make_unique<NextState>(errorState->name(), errorState.get()));

    std::vector<std::unique_ptr<Transition>> transitions;

    uint32_t stateId = startStateId;

    for (const auto &signalId : context.thenSequence) {
        const auto signalName = m_signalRenameNameTemplate.arg(signalId);
        auto sourceState = states.at(stateId).get();
        const auto targetState = states.at(stateId + 1).get();

        auto transition = std::make_unique<Transition>();
        transition->addAction(std::make_unique<NextState>(targetState->name(), targetState));

        auto input = std::make_unique<Input>();
        input->setName(signalName);
        input->setTransition(transition.get());

        sourceState->addInput(std::move(input));

        auto errorInput = std::make_unique<Input>();
        errorInput->setName(m_anySignalName);
        errorInput->setTransition(errorTransition.get());

        sourceState->addInput(std::move(errorInput));

        transitions.push_back(std::move(transition));

        ++stateId;
    }

    states.push_back(std::move(errorState));
    transitions.push_back(std::move(errorTransition));

    return transitions;
}

WhenSequenceTranslator::TransitionList WhenSequenceTranslator::createThenNotTransitions(
        WhenSequenceTranslator::Context &context, StateList &states, const uint32_t startStateId) const
{
    auto okState = std::make_unique<State>();
    okState->setName(m_okStateName);

    auto okTransition = std::make_unique<Transition>();
    okTransition->addAction(std::make_unique<NextState>(okState->name(), okState.get()));

    std::vector<std::unique_ptr<Transition>> transitions;

    uint32_t stateId = startStateId;

    for (const auto &signalId : context.thenSequence) {
        const auto signalName = m_signalRenameNameTemplate.arg(signalId);
        auto sourceState = states.at(stateId).get();
        const auto targetState = states.at(stateId + 1).get();

        auto transition = std::make_unique<Transition>();
        transition->addAction(std::make_unique<NextState>(targetState->name(), targetState));

        auto input = std::make_unique<Input>();
        input->setName(signalName);
        input->setTransition(transition.get());

        sourceState->addInput(std::move(input));

        auto okInput = std::make_unique<Input>();
        okInput->setName(m_anySignalName);
        okInput->setTransition(okTransition.get());

        sourceState->addInput(std::move(okInput));

        transitions.push_back(std::move(transition));

        ++stateId;
    }

    states.push_back(std::move(okState));
    transitions.push_back(std::move(okTransition));

    return transitions;
}

} // namespace conversion::sdl::translator
