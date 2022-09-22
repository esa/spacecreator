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

#include "mscparametervalueparser.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <iostream>

using conversion::translator::TranslationException;
using ivm::IVModel;
using msc::MscChart;
using msc::MscEntity;
using msc::MscInstanceEvent;
using msc::MscMessage;
using sdl::Rename;
using sdl::SdlModel;
using sdl::StateMachine;

namespace conversion::sdl::translator {

NeverSequenceTranslator::NeverSequenceTranslator(
        SdlModel *sdlModel, const Asn1Acn::File *asn1File, const IVModel *ivModel, const Options &options)
    : SequenceTranslator(sdlModel, asn1File, ivModel, options)
{
}

void NeverSequenceTranslator::createObserver(const MscChart *mscChart)
{
    auto context = collectData(mscChart);
    auto stateMachine = createStateMachine(context);

    auto process = createSdlProcess(context.chartName, std::move(stateMachine));
    process.addErrorState(context.errorState->name());

    auto system = createSdlSystem(context.chartName, std::move(process));
    for (auto &signalRename : context.signalRenames) {
        system.addSignal(std::move(signalRename));
    }
    system.createRoutes(m_defaultChannelName, m_defaultRouteName);

    m_sdlModel->addSystem(std::move(system));
}

NeverSequenceTranslator::NeverContext NeverSequenceTranslator::collectData(const MscChart *mscChart) const
{
    NeverContext context;
    context.chartName = Escaper::escapeSdlName(mscChart->name());
    context.signalCounter = 0;

    const auto &mscEvents = mscChart->instanceEvents();
    for (auto it = std::next(mscEvents.begin()); it != mscEvents.end(); ++it) {
        handleEvent(context, *it);
    }

    return context;
}

void NeverSequenceTranslator::handleEvent(
        NeverSequenceTranslator::NeverContext &context, const MscInstanceEvent *mscEvent) const
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
        NeverSequenceTranslator::NeverContext &context, const MscMessage *mscMessage) const
{
    const auto renamedSignal = getRenamedSignal(context, mscMessage);
    const auto sequenceValue = getSequenceValue(context, renamedSignal, mscMessage);

    context.sequence.push_back(sequenceValue);
}

std::unique_ptr<StateMachine> NeverSequenceTranslator::createStateMachine(
        NeverSequenceTranslator::NeverContext &context) const
{
    auto stateMachine = std::make_unique<StateMachine>();

    auto states = createStates(context.sequence.size());
    context.errorState = states.back().get();

    MscParameterValueParser messageParser(context.chartName, m_asn1File);
    const auto &signalRequirements = messageParser.parseSignals(context.signals);

    TFTable table(context.sequence, context.signals.size());
    auto transitions = createTransitions(table, states, context.signals, signalRequirements, 0);

    for (auto &transition : transitions) {
        stateMachine->addTransition(std::move(transition));
    }
    for (auto &state : states) {
        stateMachine->addState(std::move(state));
    }

    return stateMachine;
}

} // namespace conversion::sdl::translator
