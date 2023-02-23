/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2023 N7 Space Sp. z o.o.
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

#include "ivtopromelagenerator.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivconnection.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <promela/PromelaOptions/options.h>

using conversion::Escaper;
using conversion::promela::PromelaOptions;
using conversion::translator::TranslationException;
using promela::model::ArrayType;
using promela::model::Assignment;
using promela::model::BasicType;
using promela::model::BinaryExpression;
using promela::model::BooleanConstant;
using promela::model::ChannelInit;
using promela::model::ChannelRecv;
using promela::model::ChannelSend;
using promela::model::Conditional;
using promela::model::Constant;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::DoLoop;
using promela::model::ElseStatement;
using promela::model::Expression;
using promela::model::ForLoop;
using promela::model::GoTo;
using promela::model::InitProctype;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::Label;
using promela::model::PrintfStatement;
using promela::model::Proctype;
using promela::model::ProctypeElement;
using promela::model::PromelaSystemModel;
using promela::model::Sequence;
using promela::model::VariableRef;

using promela::model::Skip;
using promela::model::StringConstant;
using promela::model::Utype;
using promela::model::UtypeRef;
using promela::model::VariableRef;

namespace promela::translator {
IvToPromelaGenerator::IvToPromelaGenerator(IvToPromelaTranslatorContext &context, SystemInfo &systemInfo)
    : m_context(context)
    , m_systemInfo(systemInfo)
{
}

void IvToPromelaGenerator::generate()
{
    m_context.model()->addInclude("dataview.pml");

    m_context.model()->addDeclaration(Declaration(DataType(BasicType::INT), m_systemInitedVariableName));

    createPromelaObjectsForTimers();

    createPromelaObjectsForObservers();

    for (auto iter = m_systemInfo.m_functions.begin(); iter != m_systemInfo.m_functions.end(); ++iter) {
        const QString functionName = iter->first;
        if (iter->second->m_isEnvironment) {
            createPromelaObjectsForEnvironment(functionName, *iter->second);
        } else {
            m_context.model()->addInclude(QString("%1.pml").arg(functionName.toLower()));
            createPromelaObjectsForFunction(functionName, *iter->second);
        }
    }

    for (const QString &observerName : m_systemInfo.m_observers) {
        m_context.model()->addInclude(QString("%1.pml").arg(observerName));
    }

    m_context.model()->addInclude("env_inlines.pml");

    createSystemState();

    for (auto iter = m_systemInfo.m_functions.begin(); iter != m_systemInfo.m_functions.end(); ++iter) {
        if (!iter->second->m_isEnvironment) {
            addChannelAndLock(iter->first);
        }
    }

    for (const QString &observer : m_systemInfo.m_observers) {
        addChannelAndLock(observer);
    }

    generateInitProctype();
    const auto &additionalIncludes = m_context.options().values(PromelaOptions::additionalIncludes);

    for (const QString &additionalInclude : additionalIncludes) {
        m_context.model()->addEpilogueInclude(additionalInclude);
    }
}

void IvToPromelaGenerator::initializeFunction(Sequence &sequence, const QString &functionName) const
{
    QString initFn = QString("%1_0_init").arg(Escaper::escapePromelaIV(functionName));
    sequence.appendElement(InlineCall(initFn, {}));

    const VariableRef lockChannelName = VariableRef(getFunctionLockChannelName(functionName));
    QList<Expression> lockChannelArguments;
    lockChannelArguments.append(Expression(Constant(1)));

    sequence.appendElement(ChannelSend(VariableRef(lockChannelName), lockChannelArguments));
}

void IvToPromelaGenerator::generateInitProctype() const
{
    Sequence sequence(Sequence::Type::ATOMIC);

    sequence.appendElement(InlineCall("global_dataview_init", {}));

    std::vector<QString> modelFunctions = m_context.modelFunctions();
    std::sort(modelFunctions.begin(), modelFunctions.end());
    for (const QString &functionName : modelFunctions) {
        initializeFunction(sequence, functionName);
    }

    std::vector<QString> observerNames = m_context.observerNames();
    std::sort(observerNames.begin(), observerNames.end());
    for (const QString &observer : observerNames) {
        initializeFunction(sequence, observer);
    }

    sequence.appendElement(Assignment(VariableRef(m_systemInitedVariableName), Expression(Constant(1))));

    m_context.model()->setInit(InitProctype(std::move(sequence)));
}

QString IvToPromelaGenerator::observerInputSignalName(const ObserverAttachment &attachment) const
{
    return QString("%1_0_PI_0_%2")
            .arg(Escaper::escapePromelaIV(attachment.observer()))
            .arg(Escaper::escapePromelaName(attachment.observerInterface()));
}

QString IvToPromelaGenerator::observerInputSignalName(const ObserverInfo &observerInfo) const
{
    return QString("%1_0_PI_0_%2")
            .arg(Escaper::escapePromelaIV(observerInfo.m_observerName))
            .arg(Escaper::escapePromelaName(observerInfo.m_observerInterface));
}

std::list<std::unique_ptr<promela::model::ProctypeElement>> IvToPromelaGenerator::attachInputObservers(
        const QString &functionName, const QString &interfaceName, const QString &parameterName,
        const QString &parameterType) const
{
    std::list<std::unique_ptr<promela::model::ProctypeElement>> result;
    auto attachments =
            m_context.getObserverAttachments(functionName, interfaceName, ObserverAttachment::Kind::Kind_Input);
    for (const auto &attachment : attachments) {
        result.push_back(createLockAcquireStatement(attachment.observer()));

        QList<InlineCall::Argument> arguments;
        if (!parameterType.isEmpty()) {
            arguments.append(VariableRef(parameterName));
        }
        result.push_back(std::make_unique<ProctypeElement>(InlineCall(observerInputSignalName(attachment), arguments)));

        result.push_back(createLockReleaseStatement(attachment.observer()));
    }

    return result;
}

void IvToPromelaGenerator::generateProctype(
        const QString &functionName, bool environment, const ProctypeInfo &proctypeInfo) const
{
    QString channelName = proctypeInfo.m_queueName;

    QList<QString> channelNames;
    channelNames.append(proctypeInfo.m_queueName);
    createChannel(proctypeInfo.m_queueName, proctypeInfo.m_parameterTypeName, proctypeInfo.m_queueSize);

    for (auto iter = proctypeInfo.m_observers.begin(); iter != proctypeInfo.m_observers.end(); ++iter) {
        channelNames.append((*iter)->m_observerQueue);
        createChannel((*iter)->m_observerQueue, proctypeInfo.m_parameterTypeName, proctypeInfo.m_queueSize);
    }

    Sequence sequence(Sequence::Type::NORMAL);
    sequence.appendElement(createWaitForInitStatement());

    const QString &signalParameterName = QString("%1_%2_signal_parameter")
                                                 .arg(Escaper::escapePromelaIV(functionName))
                                                 .arg(proctypeInfo.m_interfaceName.toLower());
    const QString channelUsedName = QString("%1_%2_channel_used")
                                            .arg(Escaper::escapePromelaIV(functionName))
                                            .arg(proctypeInfo.m_interfaceName.toLower());

    if (!proctypeInfo.m_parameterTypeName.isEmpty()) {
        // parameter variable declaration
        // channel used declaration
        m_context.model()->addDeclaration(Declaration(
                DataType(UtypeRef(Escaper::escapePromelaName(proctypeInfo.m_parameterTypeName))), signalParameterName));
        Declaration channelUsedDeclaration = Declaration(DataType(BasicType::BOOLEAN), channelUsedName);
        channelUsedDeclaration.setInit(Expression(Constant(0)));
        m_context.model()->addDeclaration(channelUsedDeclaration);
    }

    // do loop body
    std::unique_ptr<Sequence> loopSequence = std::make_unique<Sequence>(Sequence::Type::ATOMIC);

    // check if at least one channel is not empty
    {
        std::unique_ptr<Expression> checkQueuesExpression = createCheckQueuesExpression(channelNames, false);
        loopSequence->appendElement(*checkQueuesExpression);
    }

    // acquire function mutex
    if (!environment) {
        loopSequence->appendElement(createLockAcquireStatement(functionName));
    }

    const QString mainLoopLabel = QString("%1_%2_loop")
                                          .arg(Escaper::escapePromelaIV(functionName))
                                          .arg(proctypeInfo.m_interfaceName.toLower());

    loopSequence->appendElement(Label(mainLoopLabel));

    // the incoming message is processed by output observers first
    // and then by sdl process
    // however, code of message processing is generated in reverse order
    // first the process, and then the observers in reverse order, i.e. lower priority first

    // process sdl process
    {
        const QString currentChannelName = proctypeInfo.m_queueName;

        const QString piName = environment
                ? QString()
                : QString("%1_0_PI_0_%2").arg(Escaper::escapePromelaIV(functionName)).arg(proctypeInfo.m_interfaceName);

        std::list<std::unique_ptr<promela::model::ProctypeElement>> preProcessingElements;
        if (!proctypeInfo.m_parameterTypeName.isEmpty()) {
            preProcessingElements.push_back(std::make_unique<ProctypeElement>(
                    Assignment(VariableRef(channelUsedName), Expression(Constant(1)))));
        }

        // Observers can be also attached to environment
        std::list<std::unique_ptr<promela::model::ProctypeElement>> observerStatements = attachInputObservers(
                functionName, proctypeInfo.m_interfaceName, signalParameterName, proctypeInfo.m_parameterTypeName);

        // Add processing of continuous signals in observers
        for (const QString &observer : m_context.getObserversWithContinuousSignals()) {
            const QString inlineName = QString("%1_0_check_continuous_signals").arg(Escaper::escapePromelaIV(observer));
            observerStatements.push_back(createLockAcquireStatement(observer));
            observerStatements.push_back(std::make_unique<ProctypeElement>(InlineCall(inlineName, {})));
            observerStatements.push_back(createLockReleaseStatement(observer));
        }

        loopSequence->appendElement(generateProcessMessageBlock(functionName, currentChannelName, piName,
                proctypeInfo.m_parameterTypeName, signalParameterName, mainLoopLabel, false,
                std::move(preProcessingElements), std::move(observerStatements)));
    }

    // process all observers
    for (auto iter = proctypeInfo.m_observers.rbegin(); iter != proctypeInfo.m_observers.rend(); ++iter) {
        const QString currentChannelName = (*iter)->m_observerQueue;

        loopSequence->appendElement(generateProcessMessageBlock((*iter)->m_observerName, currentChannelName,
                observerInputSignalName(**iter), proctypeInfo.m_parameterTypeName, signalParameterName, mainLoopLabel,
                true, {}, {}));
    }

    // release function mutex
    if (!environment) {
        loopSequence->appendElement(createLockReleaseStatement(functionName));
    }

    DoLoop loop;

    loop.appendSequence(std::move(loopSequence));

    sequence.appendElement(std::move(loop));

    const QString proctypeName =
            QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(proctypeInfo.m_interfaceName);
    std::unique_ptr<Proctype> proctype = std::make_unique<Proctype>(proctypeName, std::move(sequence));

    proctype->setActive(true);
    proctype->setInstancesCount(1);
    proctype->setPriority(proctypeInfo.m_priority);

    m_context.model()->addProctype(std::move(proctype));
}

std::unique_ptr<ProctypeElement> IvToPromelaGenerator::generateProcessMessageBlock(const QString &functionName,
        const QString &channelName, const QString &inlineName, const QString &parameterType,
        const QString &parameterName, const QString &exitLabel, bool lock,
        std::list<std::unique_ptr<promela::model::ProctypeElement>> preProcessingElements,
        std::list<std::unique_ptr<promela::model::ProctypeElement>> postProcessingElements) const
{
    QList<InlineCall::Argument> checkQueueArguments;
    checkQueueArguments.append(VariableRef(channelName));

    std::unique_ptr<Sequence> processMessageSeq = std::make_unique<Sequence>(Sequence::Type::NORMAL);
    processMessageSeq->appendElement(InlineCall("nempty", checkQueueArguments));

    // channel receive
    processMessageSeq->appendElement(createReceiveStatement(channelName, parameterType, parameterName));

    while (!preProcessingElements.empty()) {
        processMessageSeq->appendElement(std::move(preProcessingElements.front()));
        preProcessingElements.pop_front();
    }

    // acquire lock (observer or sdl process)
    if (lock) {
        processMessageSeq->appendElement(createLockAcquireStatement(functionName));
    }

    // call process transition (observer or sdl process)
    if (!inlineName.isEmpty()) {
        processMessageSeq->appendElement(createProcessInlineCall(inlineName, parameterType, parameterName));
    }

    // release lock (observer or sdl process)
    if (lock) {
        processMessageSeq->appendElement(createLockReleaseStatement(functionName));
    }

    while (!postProcessingElements.empty()) {
        processMessageSeq->appendElement(std::move(postProcessingElements.front()));
        postProcessingElements.pop_front();
    }

    processMessageSeq->appendElement(GoTo(exitLabel));

    std::unique_ptr<Sequence> emptySeq = std::make_unique<Sequence>(Sequence::Type::NORMAL);
    emptySeq->appendElement(InlineCall("empty", checkQueueArguments));
    emptySeq->appendElement(Skip());

    Conditional cond;
    cond.appendAlternative(std::move(processMessageSeq));
    cond.appendAlternative(std::move(emptySeq));

    return std::make_unique<ProctypeElement>(std::move(cond));
}

void IvToPromelaGenerator::generateEnvironmentProctype(const QString &functionName, const QString &interfaceName,
        const std::pair<QString, QString> &interfaceParameter, const QString &sendInline) const
{
    Sequence sequence(Sequence::Type::NORMAL);
    sequence.appendElement(createWaitForInitStatement());

    const auto &parameterName = interfaceParameter.first;
    const auto &parameterType = interfaceParameter.second;

    if (!parameterType.isEmpty()) {
        sequence.appendElement(Declaration(DataType(UtypeRef(Escaper::escapePromelaName(parameterType))), "value"));
    }

    const auto &globalInputVectorLengthLimit = m_context.options().value(PromelaOptions::globalInputVectorLengthLimit);
    const auto &interfaceInputVectorLenghtLimit =
            m_context.options().value(PromelaOptions::interfaceInputVectorLengthLimit.arg(interfaceName.toLower()));

    std::unique_ptr<Sequence> loopSequence = std::make_unique<Sequence>(Sequence::Type::ATOMIC);

    QList<InlineCall::Argument> sendInlineArguments;

    if (!parameterType.isEmpty()) {
        sendInlineArguments.append(VariableRef("value"));

        const auto parameterSubtype = handleParameterSubtype(
                Escaper::escapePromelaName(parameterType), parameterName, interfaceName, functionName);
        const QString generateValueInlineName = QString("%1_generate_value").arg(parameterSubtype);
        loopSequence->appendElement(InlineCall(generateValueInlineName, sendInlineArguments));
    }

    loopSequence->appendElement(InlineCall(sendInline, sendInlineArguments));

    // clang-format off
    auto limit = [&]() -> std::optional<int> {
        if (interfaceInputVectorLenghtLimit.has_value()) {
            return interfaceInputVectorLenghtLimit->toInt();
        } else if (globalInputVectorLengthLimit.has_value()) {
            return globalInputVectorLengthLimit->toInt();
        } else {
            return std::nullopt;
        }
    }();
    // clang-format on

    if (limit.has_value()) {
        // If limit was set to 0, then we don't generate the loop at all
        if (*limit != 0) {
            Declaration iteratorVariable(DataType(UtypeRef("int")), "inputVectorCounter");
            sequence.appendElement(std::move(iteratorVariable));

            VariableRef iteratorVariableRef("inputVectorCounter");
            Expression firstExpression(0);
            Expression lastExpression(*limit - 1);

            ForLoop loop(std::move(iteratorVariableRef), firstExpression, lastExpression, std::move(loopSequence));

            sequence.appendElement(std::move(loop));
        }
    } else {
        DoLoop loop;
        loop.appendSequence(std::move(loopSequence));

        sequence.appendElement(std::move(loop));
    }

    const QString proctypeName = QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

    std::unique_ptr<Proctype> proctype = std::make_unique<Proctype>(proctypeName, std::move(sequence));

    proctype->setActive(true);
    proctype->setInstancesCount(1);
    proctype->setPriority(1);

    m_context.model()->addProctype(std::move(proctype));
}

std::unique_ptr<model::ProctypeElement> IvToPromelaGenerator::createWaitForInitStatement() const
{
    return std::make_unique<ProctypeElement>(Expression(VariableRef(m_systemInitedVariableName)));
}

void IvToPromelaGenerator::generateSendInline(const QString &functionName, const QString &interfaceName,
        const QString &sourceFunctionName, const QString &sourceInterfaceName, const QString &channelName,
        const QString &parameterType, const QString &parameterName) const
{
    QString inlineName =
            QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(sourceFunctionName)).arg(sourceInterfaceName);

    Sequence sequence(Sequence::Type::NORMAL);

    const auto argumentName = handleSendInlineArgument(parameterType, functionName, interfaceName, parameterName);

    QList<QString> arguments;
    QList<Expression> params;

    if (argumentName.isEmpty()) {
        params.append(Expression(Constant(0)));
    } else {
        params.append(Expression(VariableRef(argumentName)));
        arguments.push_back(argumentName);
    }

    sequence.appendElement(ChannelSend(VariableRef(channelName), params));

    m_context.model()->addInlineDef(std::make_unique<InlineDef>(inlineName, arguments, std::move(sequence)));
}

void IvToPromelaGenerator::generateUnhandledInputInline(
        const QString &functionName, const ProctypeInfo &proctypeInfo) const
{
    QString inlineName = QString("%1_0_PI_0_%2_unhandled_input")
                                 .arg(Escaper::escapePromelaIV(functionName))
                                 .arg(proctypeInfo.m_interfaceName);

    Sequence sequence(Sequence::Type::NORMAL);

    QString message = QString("unhandled_input %1 %2\\n").arg(functionName).arg(proctypeInfo.m_interfaceName);
    QList<Expression> messageArgs;
    messageArgs.append(Expression(StringConstant(message)));
    sequence.appendElement(PrintfStatement(messageArgs));

    QList<QString> arguments;

    if (!proctypeInfo.m_parameterTypeName.isEmpty()) {
        arguments.push_back(proctypeInfo.m_parameterName);
    }

    ObserverAttachments attachments =
            m_context.getUnhandledInputObserversForFunction(functionName, proctypeInfo.m_interfaceName);

    if (attachments.empty()) {
        sequence.appendElement(Skip());
    } else {
        for (const ObserverAttachment &attachment : attachments) {
            sequence.appendElement(createLockAcquireStatement(attachment.observer()));

            QList<InlineCall::Argument> callArguments;
            if (!attachment.interface().isEmpty()) {
                callArguments.append(VariableRef(proctypeInfo.m_parameterName));
            }
            sequence.appendElement(InlineCall(observerInputSignalName(attachment), callArguments));

            sequence.appendElement(createLockReleaseStatement(attachment.observer()));
        }
    }

    m_context.model()->addInlineDef(std::make_unique<InlineDef>(inlineName, arguments, std::move(sequence)));
}

void IvToPromelaGenerator::createPromelaObjectsForFunction(
        const QString &functionName, const FunctionInfo &functionInfo) const
{
    QList<QString> channelNames;

    for (auto iter = functionInfo.m_proctypes.begin(); iter != functionInfo.m_proctypes.end(); ++iter) {
        const QString interfaceName = iter->second->m_interfaceName;
        if (iter->second->m_isTimer) {
            generateProctypeForTimer(functionName, *iter->second);
            continue;
        }

        createPromelaObjectsForAsyncPis(functionName, *iter->second);

        channelNames.append(iter->second->m_queueName);
        for (const std::unique_ptr<ObserverInfo> &observerInfo : iter->second->m_observers) {
            channelNames.append(observerInfo->m_observerQueue);
        }
    }

    for (auto iter = functionInfo.m_synchronousCalls.begin(); iter != functionInfo.m_synchronousCalls.end(); ++iter) {
        createPromelaObjectsForSyncRis(functionName, *iter->second);
    }

    createCheckQueueInline(m_context.model(), functionName, channelNames);
    createGetSenderInline(m_context.model(), functionName);
}

void IvToPromelaGenerator::generateProctypeForTimer(const QString &functionName, const ProctypeInfo &proctypeInfo) const
{
    generateUnhandledInputInline(functionName, proctypeInfo);

    generateProctype(functionName, false, proctypeInfo);
}

void IvToPromelaGenerator::createPromelaObjectsForAsyncPis(
        const QString &functionName, const ProctypeInfo &proctypeInfo) const
{
    const QString interfaceName = proctypeInfo.m_interfaceName;

    const QString sourceFunctionName = proctypeInfo.m_possibleSenders.firstKey();
    const QString sourceInterfaceName = proctypeInfo.m_possibleSenders.first();

    const QString channelName = proctypeInfo.m_observers.empty() ? proctypeInfo.m_queueName
                                                                 : proctypeInfo.m_observers.front()->m_observerQueue;

    generateSendInline(functionName, interfaceName, sourceFunctionName, sourceInterfaceName, channelName,
            proctypeInfo.m_parameterTypeName, proctypeInfo.m_parameterName);

    generateUnhandledInputInline(functionName, proctypeInfo);

    generateProctype(functionName, false, proctypeInfo);
}

void IvToPromelaGenerator::createPromelaObjectsForSyncRis(
        const QString &functionName, const SynchronousCallInfo &info) const
{
    QList<QString> arguments;
    QList<InlineCall::Argument> callArguments;
    for (const SynchronousCallInfo::ParameterInfo &parameterInfo : info.m_parameters) {
        arguments.append(parameterInfo.m_parameterName);
        callArguments.append(parameterInfo.m_parameterName);
    }

    Sequence sequence(Sequence::Type::NORMAL);

    for (const SynchronousCallInfo::TargetInfo &targetInfo : info.m_targets) {
        if (targetInfo.m_isEnvironment) {
            for (const SynchronousCallInfo::ParameterInfo &parameterInfo : info.m_parameters) {
                if (!parameterInfo.m_isOutput) {
                    continue;
                }
                const auto parameterSubtype =
                        handleParameterSubtype(Escaper::escapePromelaName(parameterInfo.m_parameterType),
                                parameterInfo.m_parameterName, info.m_interfaceName, functionName);

                const auto generateValueInlineName = QString("%1_generate_value").arg(parameterSubtype);
                const QList<InlineCall::Argument> generateValueInlineArgs({ parameterInfo.m_parameterName });
                InlineCall generateValueInlineCall(generateValueInlineName, generateValueInlineArgs);
                sequence.appendElement(std::move(generateValueInlineCall));
            }
        } else {
            if (info.m_isProtected) {
                sequence.appendElement(createLockAcquireStatement(targetInfo.m_targetFunctionName));
            }
            sequence.appendElement(InlineCall(targetInfo.m_providedInlineName, callArguments));

            if (info.m_isProtected) {
                sequence.appendElement(createLockReleaseStatement(targetInfo.m_targetFunctionName));
            }
        }
    }

    if (sequence.getContent().size() == 0) {
        auto message = QString("Empty content of inline %1").arg(info.m_name);
        throw TranslationException(message);
    }

    std::unique_ptr<InlineDef> inlineDef = std::make_unique<InlineDef>(info.m_name, arguments, std::move(sequence));
    m_context.model()->addInlineDef(std::move(inlineDef));
}

void IvToPromelaGenerator::createPromelaObjectsForEnvironment(
        const QString &functionName, const FunctionInfo &functionInfo) const
{
    for (auto iter = functionInfo.m_environmentSinkProctypes.begin();
            iter != functionInfo.m_environmentSinkProctypes.end(); ++iter) {
        const QString interfaceName = iter->second->m_interfaceName;
        const QString parameterName = iter->second->m_parameterName;
        const QString parameterType = iter->second->m_parameterTypeName;
        const QString sourceFunction = iter->second->m_possibleSenders.firstKey();
        const QString sourceInterface = iter->second->m_possibleSenders.first();
        generateSendInline(functionName, interfaceName, sourceFunction, sourceInterface, iter->second->m_queueName,
                parameterType, parameterName);

        generateProctype(functionName, true, *iter->second);
    }

    for (auto iter = functionInfo.m_environmentSourceProctypes.begin();
            iter != functionInfo.m_environmentSourceProctypes.end(); ++iter) {
        const QString interfaceName = iter->second->m_interfaceName;

        std::pair<QString, QString> parameter =
                std::make_pair(iter->second->m_parameterType, iter->second->m_parameterName);
        const QString sendInlineName = iter->second->m_sendInlineName;
        generateEnvironmentProctype(functionName, interfaceName, parameter, sendInlineName);
    }
}

void IvToPromelaGenerator::createCheckQueueInline(
        PromelaSystemModel *promelaModel, const QString &functionName, const QList<QString> &channelNames) const
{
    if (channelNames.empty()) {
        auto message = QString("No sporadic interfaces in function %1").arg(functionName);
        throw TranslationException(message);
    }

    Sequence sequence(Sequence::Type::ATOMIC);

    std::unique_ptr<Expression> expr = createCheckQueuesExpression(channelNames, true);

    sequence.appendElement(*expr);

    const QString checkQueueInlineName = QString("%1_check_queue").arg(Escaper::escapePromelaIV(functionName));
    promelaModel->addInlineDef(
            std::make_unique<InlineDef>(checkQueueInlineName, QList<QString>(), std::move(sequence)));
}

std::unique_ptr<Expression> IvToPromelaGenerator::createCheckQueuesExpression(
        const QList<QString> &channelNames, bool empty) const
{
    const BinaryExpression::Operator oper = empty ? BinaryExpression::Operator::AND : BinaryExpression::Operator::OR;
    auto iter = channelNames.rbegin();
    const QString inlineName = empty ? QString("empty") : QString("nempty");
    std::unique_ptr<Expression> expr;
    {
        QList<InlineCall::Argument> arguments;
        arguments.append(VariableRef((*iter)));

        expr = std::make_unique<Expression>(InlineCall(inlineName, arguments));

        ++iter;
    }

    while (iter != channelNames.rend()) {
        QList<InlineCall::Argument> arguments;
        arguments.append(VariableRef(*iter));

        std::unique_ptr<Expression> left = std::make_unique<Expression>(InlineCall(inlineName, arguments));

        expr = std::make_unique<Expression>(BinaryExpression(oper, std::move(left), std::move(expr)));

        ++iter;
    }

    return expr;
}

void IvToPromelaGenerator::createGetSenderInline(
        model::PromelaSystemModel *promelaModel, const QString &functionName) const
{
    // the SDL process required a procedure '_get_sender'
    // currently this feature is not fully supported in model checking
    // to ensure that the generated model will be valid
    // the empty inline with required name is created here.
    Sequence sequence(Sequence::Type::NORMAL);

    sequence.appendElement(Skip());

    QList<QString> arguments;
    arguments.append(QString("%1_sender_arg").arg(Escaper::escapePromelaIV(functionName)));

    const QString checkQueueInlineName = QString("%1_0_RI_0_get_sender").arg(Escaper::escapePromelaIV(functionName));
    promelaModel->addInlineDef(std::make_unique<InlineDef>(checkQueueInlineName, arguments, std::move(sequence)));
}

void IvToPromelaGenerator::createSystemState() const
{
    Utype systemState("system_state");

    for (auto iter = m_systemInfo.m_functions.begin(); iter != m_systemInfo.m_functions.end(); ++iter) {
        if (iter->second->m_isEnvironment) {
            continue;
        }
        if (!iter->second->m_functionType.isEmpty()) {
            QString dataType = QString("%1_Context").arg(Escaper::escapePromelaIV(iter->second->m_functionType));
            QString fieldName = Escaper::escapePromelaField(iter->first);
            systemState.addField(Declaration(DataType(UtypeRef(dataType)), fieldName));
        } else {
            QString dataType = QString("%1_Context").arg(Escaper::escapePromelaIV(iter->first));
            QString fieldName = Escaper::escapePromelaField(iter->first);
            systemState.addField(Declaration(DataType(UtypeRef(dataType)), fieldName));
        }
    }

    for (auto iter = m_systemInfo.m_observers.begin(); iter != m_systemInfo.m_observers.end(); ++iter) {
        QString dataType = QString("%1_Context").arg(Escaper::escapePromelaIV(*iter));
        QString fieldName = Escaper::escapePromelaField(*iter);
        systemState.addField(Declaration(DataType(UtypeRef(dataType)), fieldName));
    }

    systemState.addField(Declaration(DataType(UtypeRef("AggregateTimerData")), "timers"));

    m_context.model()->addUtype(systemState);

    m_context.model()->addDeclaration(Declaration(DataType(UtypeRef("system_state")), "global_state"));
}

void IvToPromelaGenerator::createPromelaObjectsForTimers() const
{
    std::map<QString, VariableRef> timerSignals;

    for (auto iter = m_systemInfo.m_functions.begin(); iter != m_systemInfo.m_functions.end(); ++iter) {
        const QString functionName = iter->first;
        for (auto proctypeIter = iter->second->m_proctypes.begin(); proctypeIter != iter->second->m_proctypes.end();
                ++proctypeIter) {
            if (!proctypeIter->second->m_isTimer) {
                continue;
            }
            const QString channelName = proctypeIter->second->m_queueName;
            const QString timerName = proctypeIter->second->m_interfaceName;
            VariableRef timerData("global_state");
            timerData.appendElement("timers");
            timerData.appendElement(functionName.toLower());
            timerData.appendElement(timerName.toLower());
            createTimerInlinesForFunction(functionName, timerName, timerData);
            timerSignals.emplace(channelName, timerData);
        }
    }

    if (!timerSignals.empty()) {
        createGlobalTimerObjects(timerSignals);
    }
}

void IvToPromelaGenerator::createTimerInlinesForFunction(
        const QString &functionName, const QString &timerName, const VariableRef &timerData) const
{
    const QList<InlineCall::Argument> arguments;

    const QString setTimerName = QString("%1_0_%2_set").arg(Escaper::escapePromelaIV(functionName)).arg(timerName);
    const QString resetTimerName = QString("%1_0_%2_reset").arg(Escaper::escapePromelaIV(functionName)).arg(timerName);

    Sequence setTimerSequence(Sequence::Type::NORMAL);

    const QString setTimerParameterName = QString("%1_%2_interval")
                                                  .arg(Escaper::escapePromelaName(functionName))
                                                  .arg(Escaper::escapePromelaName(timerName));

    VariableRef timerIntervalVar(timerData);
    timerIntervalVar.appendElement("interval");
    setTimerSequence.appendElement(Assignment(timerIntervalVar, Expression(VariableRef(setTimerParameterName))));

    VariableRef timerEnabledVar(timerData);
    timerEnabledVar.appendElement("timer_enabled");
    setTimerSequence.appendElement(Assignment(timerEnabledVar, Expression(BooleanConstant(true))));
    const QString setTimerMessage = QString("set_timer %1 %2 %d\\n").arg(functionName).arg(timerName);
    QList<Expression> setTimerMessageArgs;
    setTimerMessageArgs.append(Expression(StringConstant(setTimerMessage)));
    setTimerMessageArgs.append(Expression(VariableRef(setTimerParameterName)));
    setTimerSequence.appendElement(PrintfStatement(setTimerMessageArgs));

    Sequence resetTimerSequence(Sequence::Type::NORMAL);

    resetTimerSequence.appendElement(Assignment(timerEnabledVar, Expression(BooleanConstant(false))));
    const QString resetTimerMessage = QString("reset_timer %1 %2\\n").arg(functionName).arg(timerName);
    QList<Expression> resetTimerMessageArgs;
    resetTimerMessageArgs.append(Expression(StringConstant(resetTimerMessage)));
    resetTimerSequence.appendElement(PrintfStatement(resetTimerMessageArgs));

    QList<QString> params;
    params.append(setTimerParameterName);

    m_context.model()->addInlineDef(
            std::make_unique<InlineDef>(setTimerName, std::move(params), std::move(setTimerSequence)));
    m_context.model()->addInlineDef(
            std::make_unique<InlineDef>(resetTimerName, QList<QString>(), std::move(resetTimerSequence)));
}

void IvToPromelaGenerator::createGlobalTimerObjects(const std::map<QString, VariableRef> &timerSignals) const
{
    Sequence timerProctypeBody(Sequence::Type::NORMAL);
    timerProctypeBody.appendElement(createWaitForInitStatement());

    DoLoop mainLoop;

    std::unique_ptr<Sequence> checkTimersSequence = std::make_unique<Sequence>(Sequence::Type::ATOMIC);

    {
        checkTimersSequence->appendElement(Expression(BooleanConstant(true)));
    }
    {
        for (auto iter = timerSignals.begin(); iter != timerSignals.end(); ++iter) {
            Conditional cond;

            VariableRef timerEnabledVar(iter->second);
            timerEnabledVar.appendElement("timer_enabled");

            std::unique_ptr<Sequence> timerCall = std::make_unique<Sequence>(Sequence::Type::NORMAL);
            timerCall->appendElement(Expression(timerEnabledVar));
            const QString channelName = iter->first;
            QList<Expression> sendParams;
            sendParams.append(Expression(Constant(0)));
            timerCall->appendElement(ChannelSend(VariableRef(channelName), std::move(sendParams)));
            timerCall->appendElement(Assignment(timerEnabledVar, Expression(BooleanConstant(false))));
            cond.appendAlternative(std::move(timerCall));

            std::unique_ptr<Sequence> emptySeq = std::make_unique<Sequence>(Sequence::Type::NORMAL);
            emptySeq->appendElement(ElseStatement());
            emptySeq->appendElement(Skip());
            cond.appendAlternative(std::move(emptySeq));

            checkTimersSequence->appendElement(std::move(cond));
        }
    }

    mainLoop.appendSequence(std::move(checkTimersSequence));

    timerProctypeBody.appendElement(std::move(mainLoop));

    std::unique_ptr<Proctype> timerManagerProctype =
            std::make_unique<Proctype>(m_timerManagerProctypeName, std::move(timerProctypeBody));
    timerManagerProctype->setActive(true);
    timerManagerProctype->setPriority(1);
    m_context.model()->addProctype(std::move(timerManagerProctype));
}

void IvToPromelaGenerator::createPromelaObjectsForObservers() const
{
    for (auto iter = m_systemInfo.m_functions.begin(); iter != m_systemInfo.m_functions.end(); ++iter) {
        for (auto proctypeIter = iter->second->m_proctypes.begin(); proctypeIter != iter->second->m_proctypes.end();
                ++proctypeIter) {
            const QString parameterType = proctypeIter->second->m_parameterTypeName;
            const QString parameterName = proctypeIter->second->m_parameterName;
            for (auto observerIter = proctypeIter->second->m_observers.begin();
                    observerIter != proctypeIter->second->m_observers.end(); ++observerIter) {
                const QString channelName = (*observerIter)->m_nextQueue;
                const QString observerName = (*observerIter)->m_observerName;
                const QString observerInterface = (*observerIter)->m_observerInterface;
                const QString toFunction = (*observerIter)->m_toFunction;

                // send inline
                const QString inlineName =
                        QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(observerName)).arg(observerInterface);

                QList<QString> arguments;
                QList<Expression> params;
                if (!parameterType.isEmpty()) {
                    const QString name = QString("%1_%2_%3").arg(observerName).arg(toFunction).arg(parameterName);
                    arguments.append(name);
                    params.append(Expression(VariableRef(name)));
                } else {
                    params.append(Expression(Constant(1)));
                }
                Sequence sequence(Sequence::Type::NORMAL);

                sequence.appendElement(ChannelSend(VariableRef(channelName), params));

                m_context.model()->addInlineDef(
                        std::make_unique<InlineDef>(inlineName, arguments, std::move(sequence)));
            }
        }
    }
}

std::unique_ptr<ProctypeElement> IvToPromelaGenerator::createLockAcquireStatement(const QString &functionName) const
{
    const VariableRef lockChannelName = VariableRef(getFunctionLockChannelName(functionName));
    QList<VariableRef> lockChannelArguments;
    lockChannelArguments.append(VariableRef("_"));

    return std::make_unique<ProctypeElement>(ChannelRecv(lockChannelName, lockChannelArguments));
}

std::unique_ptr<ProctypeElement> IvToPromelaGenerator::createLockReleaseStatement(const QString &functionName) const
{
    const VariableRef lockChannelName = VariableRef(getFunctionLockChannelName(functionName));

    QList<Expression> unlockChannelArguments;
    unlockChannelArguments.append(Expression(Constant(1)));
    return std::make_unique<ProctypeElement>(ChannelSend(VariableRef(lockChannelName), unlockChannelArguments));
}

std::unique_ptr<ProctypeElement> IvToPromelaGenerator::createProcessInlineCall(
        const QString &inlineName, const QString &parameterType, const QString &parameterName) const
{
    if (parameterType.isEmpty()) {
        return std::make_unique<ProctypeElement>((InlineCall(inlineName, {})));
    } else {
        QList<InlineCall::Argument> arguments;
        arguments.append(VariableRef(parameterName));
        return std::make_unique<ProctypeElement>(InlineCall(inlineName, arguments));
    }
}

std::unique_ptr<model::ProctypeElement> IvToPromelaGenerator::createReceiveStatement(
        const QString &channelName, const QString &parameterType, const QString &parameterName) const
{
    // channel receive
    QList<VariableRef> receiveParams;
    receiveParams.append(VariableRef(parameterType.isEmpty() ? QString("_") : parameterName));
    return std::make_unique<ProctypeElement>(ChannelRecv(VariableRef(channelName), receiveParams));
}

QString IvToPromelaGenerator::getFunctionLockChannelName(const QString &functionName) const
{
    return QString("%1_lock").arg(Escaper::escapePromelaIV(functionName));
}

void IvToPromelaGenerator::addChannelAndLock(const QString &functionName) const
{
    QList<ChannelInit::Type> channelType;
    channelType.append(BasicType::INT);
    ChannelInit channelInit(1, std::move(channelType));
    Declaration channelDeclaration(DataType(BasicType::CHAN), getFunctionLockChannelName(functionName));
    channelDeclaration.setInit(channelInit);
    m_context.model()->addDeclaration(channelDeclaration);
}

QString IvToPromelaGenerator::handleParameterSubtype(const QString &parameterTypeName, const QString &parameterName,
        const QString &interfaceName, const QString &functionName) const
{
    if (m_context.subtypesDefinitions().empty()) {
        return parameterTypeName;
    }

    auto parameterSubtypeName = buildParameterSubtypeName(functionName, interfaceName, parameterName);

    for (const auto definitions : m_context.subtypesDefinitions()) {
        const auto parameterSubtype = definitions->type(parameterSubtypeName);

        if (parameterSubtype != nullptr) {
            return Escaper::escapePromelaName(parameterSubtypeName);
        }
    }

    return parameterTypeName;
}

QString IvToPromelaGenerator::handleSendInlineArgument(const QString &parameterType, const QString &functionName,
        const QString &interfaceName, const QString parameterName) const
{
    if (parameterType.isEmpty()) {
        return "";
    } else {
        return QString("%1_%2_%3").arg(functionName).arg(interfaceName).arg(parameterName);
    }
}

QString IvToPromelaGenerator::buildParameterSubtypeName(
        const QString &functionName, const QString &interfaceName, const QString &parameterName) const
{
    auto subtypeName = QString("%1-%2-%3").arg(functionName).arg(interfaceName).arg(parameterName);
    subtypeName.replace('_', '-');
    subtypeName = subtypeName.toLower();
    subtypeName[0] = subtypeName[0].toUpper();

    return subtypeName;
}

void IvToPromelaGenerator::createChannel(
        const QString &channelName, const QString &messageType, size_t channelSize) const
{
    QList<ChannelInit::Type> channelType;
    channelType.append(messageType.isEmpty() ? ChannelInit::Type(BasicType::INT)
                                             : ChannelInit::Type(UtypeRef(Escaper::escapePromelaName(messageType))));
    ChannelInit channelInit(channelSize, std::move(channelType));
    Declaration declaration(DataType(BasicType::CHAN), channelName);
    declaration.setInit(channelInit);
    m_context.model()->addDeclaration(declaration);
}
}
