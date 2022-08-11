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

void NeverSequenceTranslator::createObserver(const MscChart *mscChart) const
{
    auto context = createSdlSkeleton(mscChart);

    const auto &mscEvents = mscChart->instanceEvents();
    for (auto it = std::next(mscEvents.begin()); it != mscEvents.end(); ++it) {
        const auto mscEvent = *it;
        handleEvent(context, mscEvent);
    }

    context.process.addErrorState(context.lastState->name());

    auto system = createSdlSystem(context);

    m_sdlModel->addSystem(std::move(system));
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
                                    .arg(context.process.name());
        throw TranslationException(std::move(errorMessage));
    }
    }
}

void NeverSequenceTranslator::handleMessageEvent(
        NeverSequenceTranslator::Context &context, const MscMessage *mscMessage) const
{
    auto state = std::make_unique<State>();
    state->setName(m_stateNameTemplate.arg(++context.stateCounter));

    auto transition = std::make_unique<Transition>();
    transition->addAction(std::make_unique<NextState>(state->name(), state.get()));

    auto signalRename = std::make_unique<Rename>();
    signalRename->setName(m_signalRenameNameTemplate.arg(context.stateCounter));
    signalRename->setDirection(Rename::Direction::Input);
    signalRename->setOriginalName(Escaper::escapeSdlName(mscMessage->name()));
    signalRename->setOriginalFunctionName(Escaper::escapeSdlName(mscMessage->targetInstance()->name()));

    auto input = std::make_unique<Input>();
    input->setName(signalRename->name());
    input->setTransition(transition.get());

    context.lastState->addInput(std::move(input));

    auto returnTransition = std::make_unique<Transition>();
    returnTransition->addAction(std::make_unique<NextState>("", context.startState));

    auto returnInput = std::make_unique<Input>();
    returnInput->setName("*");
    returnInput->setTransition(returnTransition.get());

    context.lastState->addInput(std::move(returnInput));

    context.lastState = state.get();

    context.stateMachine->addState(std::move(state));
    context.stateMachine->addTransition(std::move(transition));
    context.stateMachine->addTransition(std::move(returnTransition));

    context.signalRenames.push_back(std::move(signalRename));
}

NeverSequenceTranslator::Context NeverSequenceTranslator::createSdlSkeleton(const MscChart *mscChart) const
{
    Process process;
    process.setName(Escaper::escapeSdlName(mscChart->name()));

    auto stateMachine = std::make_unique<StateMachine>();
    auto stateMachinePtr = stateMachine.get();

    auto startState = std::make_unique<State>();
    auto startStatePtr = startState.get();
    startState->setName(m_stateNameTemplate.arg(0));
    stateMachine->addState(std::move(startState));

    auto startTransition = std::make_unique<Transition>();
    startTransition->addAction(std::make_unique<NextState>("", startStatePtr));
    process.setStartTransition(std::move(startTransition));

    process.setStateMachine(std::move(stateMachine));

    auto eventMonitorVariable = std::make_unique<VariableDeclaration>("event", "Observable_Event", true);
    process.addVariable(std::move(eventMonitorVariable));

    Context context;
    context.process = std::move(process);
    context.stateMachine = stateMachinePtr;
    context.startState = startStatePtr;
    context.lastState = startStatePtr;
    context.stateCounter = 0;

    return context;
}

System NeverSequenceTranslator::createSdlSystem(NeverSequenceTranslator::Context &context) const
{
    const auto processName = context.process.name();

    Block block(processName);
    block.setProcess(std::move(context.process));

    System system(processName);
    system.setBlock(std::move(block));

    if (m_options.isSet(MscOptions::simuDataViewFilepath)) {
        const auto simuDataViewFilepath = *m_options.value(MscOptions::simuDataViewFilepath);
        auto useDatamodel = QString("use datamodel comment '%1'").arg(simuDataViewFilepath);

        system.addFreeformText(std::move(useDatamodel));
    } else {
        system.addFreeformText("use datamodel comment 'observer.asn'");
    }

    for (auto &signalRename : context.signalRenames) {
        system.addSignal(std::move(signalRename));
    }

    system.createRoutes(m_defaultChannelName, m_defaultRouteName);

    return system;
}

} // namespace conversion::sdl::translator
