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

#include "specialized/neverobservertranslator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <sdl/SdlModel/nextstate.h>

using conversion::translator::TranslationException;
using msc::MscChart;
using msc::MscEntity;
using msc::MscInstanceEvent;
using msc::MscMessage;
using sdl::Input;
using sdl::NextState;
using sdl::Process;
using sdl::SdlModel;
using sdl::State;
using sdl::StateMachine;
using sdl::Transition;

namespace conversion::sdl::translator {

NeverObserverTranslator::NeverObserverTranslator(SdlModel *sdlModel)
    : m_sdlModel(sdlModel)
{
}

void NeverObserverTranslator::createObserver(const MscChart *mscChart) const
{
    auto context = createSdlSkeleton(mscChart);

    const auto &mscEvents = mscChart->instanceEvents();
    for (auto it = std::next(mscEvents.begin()); it != mscEvents.end(); ++it) {
        const auto mscEvent = *it;
        handleEvent(context, mscEvent);
    }
}

NeverObserverTranslator::Context NeverObserverTranslator::createSdlSkeleton(const MscChart *mscChart) const
{
    Process process;
    process.setName(Escaper::escapeSdlName(mscChart->name()));

    auto stateMachine = std::make_unique<StateMachine>();

    auto startState = std::make_unique<State>();
    startState->setName(m_stateNameTemplate.arg(0));

    auto startTransition = std::make_unique<Transition>();
    startTransition->addAction(std::make_unique<NextState>("", startState.get()));
    process.setStartTransition(std::move(startTransition));

    // clang-format off
    Context context {
        .processName = process.name(),
        .stateMachine = stateMachine.get(),
        .startState = startState.get(),
        .lastState = startState.get(),
        .stateCounter = 0
    };
    // clang-format on

    stateMachine->addState(std::move(startState));

    process.setStateMachine(std::move(stateMachine));
    m_sdlModel->addProcess(std::move(process));

    return context;
}

void NeverObserverTranslator::handleEvent(
        NeverObserverTranslator::Context &context, const MscInstanceEvent *mscEvent) const
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
                                    .arg(context.processName);
        throw TranslationException(std::move(errorMessage));
    }
    }
}

void NeverObserverTranslator::handleMessageEvent(
        NeverObserverTranslator::Context &context, const MscMessage *mscEvent) const
{
    auto state = std::make_unique<State>();
    state->setName(m_stateNameTemplate.arg(++context.stateCounter));

    auto transition = std::make_unique<Transition>();
    transition->addAction(std::make_unique<NextState>("", state.get()));

    auto input = std::make_unique<Input>();
    input->setName(Escaper::escapeSdlName(mscEvent->name()));
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
}

} // namespace conversion::sdl::translator
