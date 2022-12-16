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

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/msc/MscOptions/options.h>
#include <iostream>
#include <ivcore/ivfunction.h>
#include <msccore/mscinstance.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/variabledeclaration.h>

using conversion::msc::MscOptions;
using conversion::translator::TranslationException;
using ivm::IVInterface;
using ivm::IVModel;
using msc::MscMessage;
using sdl::Action;
using sdl::Answer;
using sdl::Block;
using sdl::Decision;
using sdl::Expression;
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
using sdl::VariableLiteral;
using sdl::VariableReference;

namespace conversion::sdl::translator {

SequenceTranslator::SequenceTranslator(
        SdlModel *sdlModel, const Asn1Acn::File *asn1File, const IVModel *ivModel, const Options &options)
    : m_sdlModel(sdlModel)
    , m_asn1File(asn1File)
    , m_ivModel(ivModel)
    , m_options(options)
{
}

Rename *SequenceTranslator::getRenamedSignal(Context &context, const MscMessage *mscMessage) const
{
    auto signalRenamed = std::find_if(context.signalRenames.begin(), context.signalRenames.end(),
            [&](const auto &signalRename) { return signalRename->referencedName() == mscMessage->name(); });

    if (signalRenamed == context.signalRenames.end()) {
        const auto name = m_signalRenameNameTemplate.arg(context.signalCounter);
        context.signalRenames.push_back(renameSignal(name, mscMessage));
        signalRenamed = std::prev(context.signalRenames.end());
    }

    return (*signalRenamed).get();
}

std::unique_ptr<Rename> SequenceTranslator::renameSignal(const QString &name, const MscMessage *mscMessage) const
{
    const auto &referencedFunctionName = Escaper::escapeSdlName(mscMessage->targetInstance()->name());
    const auto &referencedName = Escaper::escapeSdlName(mscMessage->name());

    auto parametersTypes = getArgumentsTypes(referencedFunctionName, referencedName);

    auto signalRename = std::make_unique<Rename>();
    signalRename->setName(name);
    signalRename->setDirection(Rename::Direction::Input);
    signalRename->setReferencedName(referencedName);
    signalRename->setReferencedFunctionName(referencedFunctionName);
    signalRename->setParametersTypes(std::move(parametersTypes));

    return signalRename;
}

uint32_t SequenceTranslator::getSequenceValue(
        Context &context, const Rename *renamedSignal, const MscMessage *mscMessage) const
{
    const auto &parameterList = mscMessage->parameters();

    auto signalHandled = std::find_if(context.signals.begin(), context.signals.end(), [&](const auto &sig) {
        const auto &signalInfo = sig.second;
        return signalInfo.signal == renamedSignal && signalInfo.parameterList == parameterList;
    });

    if (signalHandled == context.signals.end()) {
        SignalInfo signalInfo;
        signalInfo.signal = renamedSignal;
        signalInfo.parameterList = parameterList;

        context.signals.insert({ context.signalCounter, std::move(signalInfo) });
        return context.signalCounter++;
    } else {
        return signalHandled->first;
    }
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
    StateList states;

    for (uint32_t stateId = 0; stateId <= stateCount; ++stateId) {
        auto state = std::make_unique<State>();
        state->setName(m_stateNameTemplate.arg(stateId));

        states.push_back(std::move(state));
    }

    return states;
}

SequenceTranslator::TransitionList SequenceTranslator::createTransitions(const TFTable &table, StateList &states,
        const SignalsMap &signals, const MscParameterValueParser::SignalRequirementsMap &signalRequirements,
        const uint32_t startStateId) const
{
    TransitionList transitions;

    const auto startState = states.at(startStateId).get();

    for (uint32_t stateId = 0; stateId < table.stateCount(); ++stateId) {
        auto sourceState = states.at(stateId).get();

        const auto &transitionsForState = table.transitionsForState(stateId);
        auto signalActions =
                createSignalActions(startStateId, transitionsForState, states, signals, signalRequirements);

        for (auto &[signal, actions] : signalActions) {
            const auto &signalName = signal->name();

            const auto parameterCount = signal->parametersTypes().size();
            auto transition = createTransitionOnSignal(signalName, parameterCount, sourceState);
            for (auto &action : actions) {
                transition->addAction(std::move(action));
            }

            const auto lastAction = transition->actions().back().get();
            const auto endsWithNextState = (dynamic_cast<NextState *>(lastAction) != nullptr);
            if (!endsWithNextState) {
                auto startStateAction = std::make_unique<NextState>(startState->name(), startState);
                transition->addAction(std::move(startStateAction));
            }

            transitions.push_back(std::move(transition));
        }

        auto returnTransition = createTransitionOnSignal(m_anySignalName, 0, sourceState);
        returnTransition->addAction(std::make_unique<NextState>(startState->name(), startState));
        transitions.push_back(std::move(returnTransition));
    }

    return transitions;
}

std::unique_ptr<Action> SequenceTranslator::createSignalRequirements(
        const MscParameterValueParser::ParametersRequirements &parametersRequirements, const State *targetState) const
{
    std::unique_ptr<Action> lastAction = std::make_unique<NextState>(targetState->name(), targetState);

    for (auto it = parametersRequirements.rbegin(); it != parametersRequirements.rend(); ++it) {
        const auto &[name, value] = *it;
        lastAction = createParameterRequirements(name, value, std::move(lastAction));
    }

    return lastAction;
}

std::unique_ptr<Decision> SequenceTranslator::createParameterRequirements(
        const QString &name, const std::optional<QString> &value, std::unique_ptr<Action> trueAction) const
{
    auto decision = std::make_unique<Decision>();

    if (value.has_value()) {
        auto trueAnswer = createTrueAnswer(std::move(trueAction), *value);

        auto decisionExpression = std::make_unique<Expression>(name);
        decision->setExpression(std::move(decisionExpression));
        decision->addAnswer(std::move(trueAnswer));
    } else {
        const auto [choiceName, choiceFieldName] = splitChoiceName(name);

        auto trueAnswer = createTrueAnswer(std::move(trueAction), choiceFieldName);

        auto decisionExpression = std::make_unique<Expression>(m_isPresentTemplate.arg(choiceName));
        decision->setExpression(std::move(decisionExpression));
        decision->addAnswer(std::move(trueAnswer));
    }

    auto elseAnswer = createElseAnswer();
    decision->addAnswer(std::move(elseAnswer));

    return decision;
}

std::unique_ptr<Answer> SequenceTranslator::createTrueAnswer(
        std::unique_ptr<Action> action, const QString &literal) const
{
    auto transition = std::make_unique<Transition>();
    transition->addAction(std::move(action));

    auto answer = std::make_unique<Answer>();
    answer->setLiteral(VariableLiteral(literal));
    answer->setTransition(std::move(transition));

    return answer;
}

std::unique_ptr<Answer> SequenceTranslator::createElseAnswer() const
{
    auto transition = std::make_unique<Transition>();
    auto answer = std::make_unique<Answer>();
    answer->setLiteral(::sdl::Answer::ElseLiteral);
    answer->setTransition(std::move(transition));

    return answer;
}

std::unique_ptr<Transition> SequenceTranslator::createTransitionOnSignal(
        const QString &signalName, const int parameterCount, State *sourceState) const
{
    auto transition = std::make_unique<Transition>();

    auto input = std::make_unique<Input>();
    input->setName(signalName);
    input->setTransition(transition.get());

    for (int i = 0; i < parameterCount; ++i) {
        auto paramName = m_signalParameterNameTemplate.arg(signalName).arg(i);
        auto param = std::make_unique<VariableReference>(std::move(paramName));
        input->addParameter(std::move(param));
    }

    sourceState->addInput(std::move(input));

    return transition;
}

QStringList SequenceTranslator::getArgumentsTypes(const QString &ivFunctionName, const QString &ivInterfaceName) const
{
    QStringList types;

    const auto ivInterface = findIvInterface(ivFunctionName, ivInterfaceName);
    for (const auto &param : ivInterface->params()) {
        types << Escaper::escapeSdlName(param.paramTypeName());
    }

    return types;
}

IVInterface *SequenceTranslator::findIvInterface(const QString &ivFunctionName, const QString &ivInterfaceName) const
{
    const auto ivFunction = m_ivModel->getFunction(ivFunctionName, Qt::CaseInsensitive);

    if (ivFunction == nullptr) {
        auto errorMessage = QString("Unable to find function named %1 in given InterfaceView").arg(ivFunctionName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto ivInterfaces = ivFunction->interfaces();
    const auto ivInterfaceFound = std::find_if(ivInterfaces.begin(), ivInterfaces.end(),
            [&](const auto interface) { return interface->title() == ivInterfaceName; });

    if (ivInterfaceFound == ivInterfaces.end()) {
        auto errorMessage = QString("Unable to find interface named %1 in function %2 in given InterfaceView")
                                    .arg(ivInterfaceName)
                                    .arg(ivFunctionName);
        throw TranslationException(std::move(errorMessage));
    }

    return *ivInterfaceFound;
}

SequenceTranslator::ActionsMap SequenceTranslator::createSignalActions(const uint32_t startStateId,
        const std::vector<uint32_t> &transitions, StateList &states, const SignalsMap &signals,
        const MscParameterValueParser::SignalRequirementsMap &signalRequirements) const
{
    ActionsMap signalActions;

    for (uint32_t signalId = 0; signalId < transitions.size(); ++signalId) {
        const auto targetStateId = transitions.at(signalId);

        if (targetStateId == startStateId) {
            continue;
        }

        const auto &signalInfo = signals.at(signalId);
        const auto targetState = states.at(targetStateId).get();

        auto &actions = signalActions[signalInfo.signal];

        const auto signalHasRequirements = (signalRequirements.count(signalId) != 0);
        if (signalHasRequirements) {
            const auto &parametersRequirements = signalRequirements.at(signalId);

            auto decisions = createSignalRequirements(parametersRequirements, targetState);
            actions.push_back(std::move(decisions));
        } else {
            auto nextStateAction = std::make_unique<NextState>(targetState->name(), targetState);
            actions.push_back(std::move(nextStateAction));
        }
    }

    return signalActions;
}

std::pair<QString, QString> SequenceTranslator::splitChoiceName(const QString &name) const
{
    const auto choiceName = name.section('.', 0, -2);
    const auto choiceFieldName = name.section('.', -1);

    return { choiceName, choiceFieldName };
}

} // namespace conversion::sdl::translator
