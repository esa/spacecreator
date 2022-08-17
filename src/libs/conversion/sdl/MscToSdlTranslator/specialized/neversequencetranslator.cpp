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

#include "specialized/neversequencetranslator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/msc/MscOptions/options.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/variabledeclaration.h>

using conversion::msc::MscOptions;
using conversion::translator::TranslationException;
using msc::MscChart;
using msc::MscEntity;
using msc::MscInstanceEvent;
using msc::MscMessage;
using sdl::Block;
using sdl::Input;
using sdl::NextState;
using sdl::Process;
using sdl::Rename;
using sdl::SdlModel;
using sdl::State;
using sdl::StateMachine;
using sdl::System;
using sdl::Transition;
using sdl::VariableDeclaration;

namespace conversion::sdl::translator {

NeverSequenceTranslator::NeverSequenceTranslator(SdlModel *sdlModel, const Options &options)
    : m_sdlModel(sdlModel)
    , m_options(options)
{
}

void NeverSequenceTranslator::createObserver(const MscChart *mscChart)
{
    auto context = collectData(mscChart);
    createSdlSystem(context);
}

NeverSequenceTranslator::Context NeverSequenceTranslator::collectData(const MscChart *mscChart) const
{
    Context context;
    context.chartName = Escaper::escapeSdlName(mscChart->name());
    context.signalCounter = 0;

    const auto &mscEvents = mscChart->instanceEvents();
    for (auto it = std::next(mscEvents.begin()); it != mscEvents.end(); ++it) {
        handleEvent(context, *it);
    }

    return context;
}

void NeverSequenceTranslator::handleEvent(
        NeverSequenceTranslator::Context &context, const MscInstanceEvent *mscEvent) const
{
    switch (mscEvent->entityType()) {
    case MscEntity::EntityType::Message: {
        const auto mscMessageEvent = dynamic_cast<const MscMessage *>(mscEvent);
        handleMessageEvent(context, mscMessageEvent);
    } break;
    case MscEntity::EntityType::Coregion: {
        throw TranslationException("Coregion for never observer is not yet implemented");
    } break;
    case MscEntity::EntityType::Comment:
        break;
    default: {
        auto errorMessage = QString("Encountered unsupported %1 event in never observer %2")
                                    .arg(mscEvent->entityTypeName())
                                    .arg(context.chartName);
        throw TranslationException(std::move(errorMessage));
    }
    }
}

void NeverSequenceTranslator::handleMessageEvent(
        NeverSequenceTranslator::Context &context, const MscMessage *mscMessage) const
{
    const auto signalRenamed = std::find_if(context.signals.begin(), context.signals.end(),
            [&](auto &&sig) { return sig.second->referencedName() == mscMessage->name(); });

    if (signalRenamed == context.signals.end()) {
        auto signalRename = std::make_unique<Rename>();
        signalRename->setName(m_signalRenameNameTemplate.arg(context.signalCounter));
        signalRename->setDirection(Rename::Direction::Input);
        signalRename->setReferencedName(Escaper::escapeSdlName(mscMessage->name()));
        signalRename->setReferencedFunctionName(Escaper::escapeSdlName(mscMessage->targetInstance()->name()));

        context.signals.insert({ context.signalCounter, std::move(signalRename) });

        context.sequence.push_back(context.signalCounter++);
    } else {
        context.sequence.push_back(signalRenamed->first);
    }
}

void NeverSequenceTranslator::createSdlSystem(NeverSequenceTranslator::Context &context)
{
    auto process = createSdlProcess(context);

    Block block(context.chartName);
    block.setProcess(std::move(process));

    System system(context.chartName);
    system.setBlock(std::move(block));

    if (m_options.isSet(MscOptions::simuDataViewFilepath)) {
        const auto simuDataViewFilepath = *m_options.value(MscOptions::simuDataViewFilepath);
        auto useDatamodel = QString("use datamodel comment '%1'").arg(simuDataViewFilepath);

        system.addFreeformText(std::move(useDatamodel));
    } else {
        system.addFreeformText("use datamodel comment 'observer.asn'");
    }

    for (auto &[id, signalRename] : context.signals) {
        system.addSignal(std::move(signalRename));
    }

    system.createRoutes(m_defaultChannelName, m_defaultRouteName);

    m_sdlModel->addSystem(std::move(system));
}

Process NeverSequenceTranslator::createSdlProcess(const NeverSequenceTranslator::Context &context)
{
    Process process;
    process.setName(context.chartName);

    auto stateMachine = createStateMachine(context);

    const auto startState = stateMachine->states().front().get();
    const auto lastState = stateMachine->states().back().get();

    auto startTransition = std::make_unique<Transition>();
    startTransition->addAction(std::make_unique<NextState>("", startState));
    process.setStartTransition(std::move(startTransition));

    process.addErrorState(lastState->name());

    auto eventMonitorVariable = std::make_unique<VariableDeclaration>("event", "Observable_Event", true);
    process.addVariable(std::move(eventMonitorVariable));

    process.setStateMachine(std::move(stateMachine));

    return process;
}

std::unique_ptr<StateMachine> NeverSequenceTranslator::createStateMachine(
        const NeverSequenceTranslator::Context &context) const
{
    TFTable table(context.sequence, context.signals.size());

    auto states = createStates(table.stateCount());
    auto transitions = createTransitions(table, states);

    auto stateMachine = std::make_unique<StateMachine>();

    for (auto &state : states) {
        stateMachine->addState(std::move(state));
    }

    for (auto &transition : transitions) {
        stateMachine->addTransition(std::move(transition));
    }

    return stateMachine;
}

NeverSequenceTranslator::StateList NeverSequenceTranslator::createStates(const uint32_t stateCount) const
{
    std::vector<std::unique_ptr<State>> states;

    for (uint32_t stateId = 0; stateId <= stateCount; ++stateId) {
        auto state = std::make_unique<State>();
        state->setName(m_stateNameTemplate.arg(stateId));

        states.push_back(std::move(state));
    }

    return states;
}

NeverSequenceTranslator::TransitionList NeverSequenceTranslator::createTransitions(
        const TFTable &table, StateList &states) const
{
    std::vector<std::unique_ptr<Transition>> transitions;

    for (uint32_t stateId = 0; stateId < table.stateCount(); ++stateId) {
        const auto &transitionsForState = table.transitionsForState(stateId);

        for (uint32_t signalId = 0; signalId < transitionsForState.size(); ++signalId) {
            const auto targetStateId = transitionsForState.at(signalId);

            const auto signalName = m_signalRenameNameTemplate.arg(signalId);
            auto sourceState = states.at(stateId).get();
            const auto targetState = states.at(targetStateId).get();

            auto transition = std::make_unique<Transition>();
            transition->addAction(std::make_unique<NextState>(targetState->name(), targetState));

            auto input = std::make_unique<Input>();
            input->setName(signalName);
            input->setTransition(transition.get());

            sourceState->addInput(std::move(input));

            transitions.push_back(std::move(transition));
        }
    }

    return transitions;
}

} // namespace conversion::sdl::translator
