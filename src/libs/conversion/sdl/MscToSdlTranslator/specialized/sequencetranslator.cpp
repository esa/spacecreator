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

#include "specialized/sequencetranslator.h"

#include <conversion/common/translation/exceptions.h>
#include <conversion/msc/MscOptions/options.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/variabledeclaration.h>

using conversion::msc::MscOptions;
using conversion::translator::TranslationException;
using sdl::Block;
using sdl::Input;
using sdl::NextState;
using sdl::Process;
using sdl::SdlModel;
using sdl::State;
using sdl::StateMachine;
using sdl::System;
using sdl::Transition;
using sdl::VariableDeclaration;

namespace conversion::sdl::translator {

SequenceTranslator::SequenceTranslator(
        SdlModel *sdlModel, const Asn1Acn::File *observerAsn1File, const Options &options)
    : m_sdlModel(sdlModel)
    , m_observerAsn1File(observerAsn1File)
    , m_options(options)
{
}

Process SequenceTranslator::createSdlProcess(const QString &chartName, std::unique_ptr<StateMachine> stateMachine)
{
    Process process;
    process.setName(chartName);

    const auto startState = stateMachine->states().front().get();

    auto startTransition = std::make_unique<Transition>();
    startTransition->addAction(std::make_unique<NextState>("", startState));
    process.setStartTransition(std::move(startTransition));

    auto eventMonitorVariable = std::make_unique<VariableDeclaration>("event", "Observable_Event", true);
    process.addVariable(std::move(eventMonitorVariable));

    process.setStateMachine(std::move(stateMachine));

    return process;
}

System SequenceTranslator::createSdlSystem(const QString &chartName, Process process)
{
    Block block(chartName);
    block.setProcess(std::move(process));

    System system(chartName);
    system.setBlock(std::move(block));

    if (m_options.isSet(MscOptions::simuDataViewFilepath)) {
        const auto simuDataViewFilepath = *m_options.value(MscOptions::simuDataViewFilepath);
        auto useDatamodel = QString("use datamodel comment '%1'").arg(simuDataViewFilepath);

        system.addFreeformText(std::move(useDatamodel));
    } else {
        system.addFreeformText("use datamodel comment 'observer.asn'");
    }

    return system;
}

SequenceTranslator::StateList SequenceTranslator::createStates(const uint32_t stateCount) const
{
    std::vector<std::unique_ptr<State>> states;

    for (uint32_t stateId = 0; stateId <= stateCount; ++stateId) {
        auto state = std::make_unique<State>();
        state->setName(m_stateNameTemplate.arg(stateId));

        states.push_back(std::move(state));
    }

    return states;
}

SequenceTranslator::TransitionList SequenceTranslator::createTransitions(
        const TFTable &table, StateList &states, const uint32_t startStateId) const
{
    std::vector<std::unique_ptr<Transition>> transitions;

    const auto startState = states.at(startStateId).get();

    for (uint32_t stateId = 0; stateId < table.stateCount(); ++stateId) {
        const auto &transitionsForState = table.transitionsForState(stateId);

        auto sourceState = states.at(stateId).get();

        for (uint32_t signalId = 0; signalId < transitionsForState.size(); ++signalId) {
            const auto targetStateId = transitionsForState.at(signalId);

            if (targetStateId == startStateId) {
                continue;
            }

            const auto signalName = m_signalRenameNameTemplate.arg(signalId);
            const auto targetState = states.at(targetStateId).get();

            auto transition = createTransitionOnInput(signalName, sourceState, targetState);
            transitions.push_back(std::move(transition));
        }

        auto returnTransition = createTransitionOnInput(m_anySignalName, sourceState, startState);
        transitions.push_back(std::move(returnTransition));
    }

    return transitions;
}

std::unique_ptr<Transition> SequenceTranslator::createTransitionOnInput(
        const QString &signalName, State *sourceState, const State *targetState) const
{
    auto transition = std::make_unique<Transition>();
    transition->addAction(std::make_unique<NextState>(targetState->name(), targetState));

    auto input = std::make_unique<Input>();
    input->setName(signalName);
    input->setTransition(transition.get());

    sourceState->addInput(std::move(input));

    return transition;
}

} // namespace conversion::sdl::translator
