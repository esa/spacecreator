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
        SdlModel *sdlModel, const Asn1Acn::File *observerAsn1File, const IVModel *ivModel, const Options &options)
    : SequenceTranslator(sdlModel, observerAsn1File, ivModel, options)
{
}

void NeverSequenceTranslator::createObserver(const MscChart *mscChart)
{
    auto context = collectData(mscChart);
    auto stateMachine = createStateMachine(context);

    auto process = createSdlProcess(context.chartName, std::move(stateMachine));
    process.addErrorState(context.errorState->name());

    auto system = createSdlSystem(context.chartName, std::move(process));
    for (auto &[id, signalInfo] : context.signals) {
        system.addSignal(std::move(signalInfo.signal));
    }
    system.createRoutes(m_defaultChannelName, m_defaultRouteName);

    m_sdlModel->addSystem(std::move(system));
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
    const auto signalRenamed = std::find_if(context.signals.begin(), context.signals.end(), [&](auto &&sig) {
        return sig.second.signal->referencedName() == mscMessage->name()
                && sig.second.parameterList == mscMessage->parameters();
    });

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

        context.sequence.push_back(context.signalCounter++);
    } else {
        context.sequence.push_back(signalRenamed->first);
    }
}

std::unique_ptr<StateMachine> NeverSequenceTranslator::createStateMachine(
        NeverSequenceTranslator::Context &context) const
{
    auto stateMachine = std::make_unique<StateMachine>();

    auto states = createStates(context.sequence.size());
    context.errorState = states.back().get();

    parseMessageParameters(context.signals);

    TFTable table(context.sequence, context.signals.size());
    auto transitions = createTransitions(table, states, 0);
    for (auto &transition : transitions) {
        stateMachine->addTransition(std::move(transition));
    }

    for (auto &state : states) {
        stateMachine->addState(std::move(state));
    }

    return stateMachine;
}

} // namespace conversion::sdl::translator
