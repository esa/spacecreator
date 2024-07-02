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

#include "helper.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivmodel.h>
#include <promela/PromelaCommon/constants.h>
#include <promela/PromelaOptions/options.h>

using conversion::Escaper;
using conversion::promela::PromelaOptions;
using conversion::translator::TranslationException;
using promela::common::PromelaConstants;
using promela::model::ArrayType;
using promela::model::Assignment;
using promela::model::BasicType;
using promela::model::BinaryExpression;
using promela::model::BooleanConstant;
using promela::model::CCode;
using promela::model::CDecl;
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
using promela::model::Skip;
using promela::model::StringConstant;
using promela::model::Utype;
using promela::model::UtypeRef;
using promela::model::VariableRef;

namespace promela::translator {
IvToPromelaGenerator::IvToPromelaGenerator(IvToPromelaTranslatorContext &context, SystemInfo &systemInfo)
    : m_context(context)
    , m_systemInfo(systemInfo)
    , m_ccodeGenerator(context.asn1Model(), m_systemInfo.m_messageTypes)
{
}

void IvToPromelaGenerator::generate()
{
    m_context.model()->addInclude("dataview.pml");

    m_context.model()->addDeclaration(Declaration(DataType(BasicType::INT), m_systemInitedVariableName));

    createMessageTypes();

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
    m_context.model()->addInclude("message_sizes.pml");

    m_context.model()->addCDecl(CDecl("\\#include \"dataview-uniq.h\""));

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

void IvToPromelaGenerator::createMessageTypes()
{
    for (const QString &messageType : m_systemInfo.m_messageTypes) {
        QString bufferTypeName = messageTypeName(messageType);
        QString sizeConstantName =
                QString("asn1Scc%1_REQUIRED_BYTES_FOR_ENCODING").arg(Escaper::escapeCName(messageType));

        Utype bufferType(std::move(bufferTypeName));
        DataType arrayType = DataType(ArrayType(std::move(sizeConstantName), BasicType::BYTE));
        Declaration arrayDeclaration = Declaration(std::move(arrayType), "data");
        bufferType.addField(std::move(arrayDeclaration));

        m_context.model()->addUtype(bufferType);
    }
}

void IvToPromelaGenerator::initializeFunction(Sequence &sequence, const QString &functionName)
{
    QString initFn = QString("%1_0_init").arg(Escaper::escapePromelaIV(functionName));
    sequence.appendElement(InlineCall(initFn, {}));

    const VariableRef lockChannelName = VariableRef(getFunctionLockChannelName(functionName));
    QList<Expression> lockChannelArguments;
    lockChannelArguments.append(Expression(Constant(1)));

    sequence.appendElement(ChannelSend(VariableRef(lockChannelName), lockChannelArguments));
}

void IvToPromelaGenerator::generateInitProctype()
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

QString IvToPromelaGenerator::observerInputSignalName(const ObserverAttachment &attachment)
{
    return QString("%1_0_PI_0_%2")
            .arg(Escaper::escapePromelaIV(attachment.observer()))
            .arg(Escaper::escapePromelaName(attachment.observerInterface()));
}

QString IvToPromelaGenerator::observerInputSignalName(const ObserverInfo &observerInfo)
{
    return QString("%1_0_PI_0_%2")
            .arg(Escaper::escapePromelaIV(observerInfo.m_observerName))
            .arg(Escaper::escapePromelaName(observerInfo.m_observerInterface));
}

std::list<std::unique_ptr<promela::model::ProctypeElement>> IvToPromelaGenerator::attachInputObservers(
        const QString &functionName, const QString &interfaceName, const QString &parameterName,
        const QString &parameterType)
{
    std::list<std::unique_ptr<promela::model::ProctypeElement>> result;
    auto attachments =
            m_context.getObserverAttachments(functionName, interfaceName, ObserverAttachment::Kind::Kind_Input);
    for (const auto &attachment : attachments) {
        // if multicast is and observer has explicit 'from' function, then wrap call using alternative
        if (m_context.isMulticastSupported() && attachment.fromFunction().has_value()) {
            const QString senderVariableName = QString("%1_sender").arg(Escaper::escapePromelaField(functionName));
            const QString pidName =
                    QString("PID_%1").arg(Escaper::escapePromelaField(attachment.fromFunction().value()));
            Conditional cond;

            std::unique_ptr<Sequence> processSeq = std::make_unique<Sequence>(Sequence::Type::NORMAL);
            std::unique_ptr<Expression> left = std::make_unique<Expression>(VariableRef(senderVariableName));
            std::unique_ptr<Expression> right = std::make_unique<Expression>(VariableRef(pidName));
            processSeq->appendElement(std::make_unique<ProctypeElement>(Expression(
                    BinaryExpression(BinaryExpression::Operator::EQUAL, std::move(left), std::move(right)))));

            processSeq->appendElement(createLockAcquireStatement(attachment.observer()));

            QList<InlineCall::Argument> arguments;
            if (!parameterType.isEmpty()) {
                arguments.append(VariableRef(parameterName));
            }
            processSeq->appendElement(
                    std::make_unique<ProctypeElement>(InlineCall(observerInputSignalName(attachment), arguments)));

            processSeq->appendElement(createLockReleaseStatement(attachment.observer()));

            cond.appendAlternative(std::move(processSeq));

            std::unique_ptr<Sequence> emptySeq = std::make_unique<Sequence>(Sequence::Type::NORMAL);
            emptySeq->appendElement(ElseStatement());
            emptySeq->appendElement(Skip());
            cond.appendAlternative(std::move(emptySeq));

            result.push_back(std::make_unique<ProctypeElement>(std::move(cond)));
        } else {
            result.push_back(createLockAcquireStatement(attachment.observer()));

            QList<InlineCall::Argument> arguments;
            if (!parameterType.isEmpty()) {
                arguments.append(VariableRef(parameterName));
            }
            result.push_back(
                    std::make_unique<ProctypeElement>(InlineCall(observerInputSignalName(attachment), arguments)));

            result.push_back(createLockReleaseStatement(attachment.observer()));
        }
    }

    return result;
}

void IvToPromelaGenerator::generateProctype(
        const QString &functionName, bool environment, const ProctypeInfo &proctypeInfo)
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

    const QString signalParameterName = QString("%1_%2_signal_parameter")
                                                .arg(Escaper::escapePromelaIV(functionName))
                                                .arg(proctypeInfo.m_interfaceName.toLower());

    const QString signalMessageName = QString("%1_message").arg(signalParameterName);

    const QString channelUsedName = QString("%1_%2_channel_used")
                                            .arg(Escaper::escapePromelaIV(functionName))
                                            .arg(proctypeInfo.m_interfaceName.toLower());

    if (!proctypeInfo.m_parameterTypeName.isEmpty()) {
        // parameter variable declaration
        // channel used declaration
        const QString parameterTypeName = Escaper::escapePromelaName(proctypeInfo.m_parameterTypeName);
        m_context.model()->addDeclaration(Declaration(DataType(UtypeRef(parameterTypeName)), signalParameterName));
        m_context.model()->addDeclaration(
                Declaration(DataType(UtypeRef(messageTypeName(parameterTypeName))), signalMessageName));
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

        loopSequence->appendElement(generateProcessMessageBlock(functionName, functionName, currentChannelName, piName,
                proctypeInfo.m_parameterTypeName, signalParameterName, signalMessageName, mainLoopLabel, false,
                std::move(preProcessingElements), std::move(observerStatements)));
    }

    // process all observers
    for (auto iter = proctypeInfo.m_observers.rbegin(); iter != proctypeInfo.m_observers.rend(); ++iter) {
        const QString currentChannelName = (*iter)->m_observerQueue;

        loopSequence->appendElement(generateProcessMessageBlock((*iter)->m_observerName, functionName,
                currentChannelName, observerInputSignalName(**iter), proctypeInfo.m_parameterTypeName,
                signalParameterName, signalMessageName, mainLoopLabel, true, {}, {}));
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
        const QString &modelFunctionName, const QString &channelName, const QString &inlineName,
        const QString &parameterType, const QString &parameterName, const QString &messageName,
        const QString &exitLabel, bool lock,
        std::list<std::unique_ptr<promela::model::ProctypeElement>> preProcessingElements,
        std::list<std::unique_ptr<promela::model::ProctypeElement>> postProcessingElements)
{
    // parameterName - > name of the variable
    // messageName - > name of the message
    QList<InlineCall::Argument> checkQueueArguments;
    checkQueueArguments.append(VariableRef(channelName));

    std::unique_ptr<Sequence> processMessageSeq = std::make_unique<Sequence>(Sequence::Type::NORMAL);
    processMessageSeq->appendElement(InlineCall("nempty", checkQueueArguments));

    // channel receive
    processMessageSeq->appendElement(
            createReceiveStatement(modelFunctionName, channelName, parameterType, messageName));

    if (!parameterType.isEmpty()) {
        m_ccodeGenerator.generateConversionFromBufferToParameter(*processMessageSeq, parameterType, parameterName);

        QList<PrintfStatement> statements = m_ccodeGenerator.printfStatements(parameterType, parameterName,
                QString("%1 %2: ").arg(PromelaConstants::channelRecvMessage, channelName));

        while (!statements.empty()) {
            processMessageSeq->appendElement(statements.takeFirst());
        }

        while (!preProcessingElements.empty()) {
            processMessageSeq->appendElement(std::move(preProcessingElements.front()));
            preProcessingElements.pop_front();
        }
    } else {
        QList<Expression> arguments;
        arguments.append(
                Expression(StringConstant(QString("%1 %2: 0").arg(PromelaConstants::channelRecvMessage, channelName))));
        processMessageSeq->appendElement(PrintfStatement(std::move(arguments)));
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
        const std::pair<QString, QString> &interfaceParameter, const QString &sendInline)
{
    Sequence sequence(Sequence::Type::NORMAL);
    sequence.appendElement(createWaitForInitStatement());

    const auto &parameterName = interfaceParameter.first;
    const auto &parameterType = interfaceParameter.second;

    if (!parameterType.isEmpty()) {
        sequence.appendElement(Declaration(DataType(UtypeRef(Escaper::escapePromelaName(parameterType))), "value"));
    }

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

    addEnvironmentProctype(functionName, interfaceName, std::move(sequence), std::move(loopSequence));
}

void IvToPromelaGenerator::generateEnvironmentProctypeForSynchronousInterface(
        const QString &functionName, const RequiredCallInfo &info)
{
    const QString interfaceName = info.m_interfaceName;

    Sequence sequence(Sequence::Type::NORMAL);
    sequence.appendElement(createWaitForInitStatement());

    // declare variables for parameters
    size_t index = 1;
    for (const RequiredCallInfo::ParameterInfo &param : info.m_parameters) {
        const QString variableName = QString("value_%1").arg(index);
        sequence.appendElement(
                Declaration(DataType(UtypeRef(Escaper::escapePromelaName(param.m_parameterType))), variableName));
        ++index;
    }

    std::unique_ptr<Sequence> loopSequence = std::make_unique<Sequence>(Sequence::Type::ATOMIC);

    QList<InlineCall::Argument> callArguments;
    index = 1;
    for (const RequiredCallInfo::ParameterInfo &param : info.m_parameters) {
        const QString variableName = QString("value_%1").arg(index);
        callArguments.append(VariableRef(variableName));

        if (!param.m_isOutput) {
            // generate values only for input parameters
            const auto parameterSubtype = handleParameterSubtype(Escaper::escapePromelaName(param.m_parameterType),
                    param.m_parameterName, interfaceName, functionName);
            const QString generateValueInlineName = QString("%1_generate_value").arg(parameterSubtype);
            QList<InlineCall::Argument> generateValueArguments;
            generateValueArguments.append(VariableRef(variableName));
            loopSequence->appendElement(InlineCall(generateValueInlineName, generateValueArguments));
        }
        ++index;
    }

    for (auto targetIter = info.m_targets.begin(); targetIter != info.m_targets.end(); ++targetIter) {
        const RequiredCallInfo::TargetInfo &targetInfo = targetIter->second;
        if (info.m_isProtected) {
            loopSequence->appendElement(createLockAcquireStatement(targetInfo.m_targetFunctionName));
        }
        loopSequence->appendElement(InlineCall(targetInfo.m_providedInlineName, callArguments));
        if (info.m_isProtected) {
            loopSequence->appendElement(createLockReleaseStatement(targetInfo.m_targetFunctionName));
        }
    }

    addEnvironmentProctype(functionName, interfaceName, std::move(sequence), std::move(loopSequence));
}

void IvToPromelaGenerator::addEnvironmentProctype(const QString &functionName, const QString &interfaceName,
        Sequence sequence, std::unique_ptr<Sequence> loopSequence)
{
    const auto &globalInputVectorLengthLimit = m_context.options().value(PromelaOptions::globalInputVectorLengthLimit);
    const auto &interfaceInputVectorLenghtLimit =
            m_context.options().value(PromelaOptions::interfaceInputVectorLengthLimit.arg(interfaceName.toLower()));

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
            Declaration iteratorVariable(DataType(BasicType::INT), "inputVectorCounter");
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

std::unique_ptr<model::ProctypeElement> IvToPromelaGenerator::createWaitForInitStatement()
{
    return std::make_unique<ProctypeElement>(Expression(VariableRef(m_systemInitedVariableName)));
}

void IvToPromelaGenerator::generateUnhandledInputInline(const QString &functionName, const ProctypeInfo &proctypeInfo)
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
        const QString &functionName, const FunctionInfo &functionInfo)
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

    for (auto iter = functionInfo.m_sporadicCalls.begin(); iter != functionInfo.m_sporadicCalls.end(); ++iter) {
        createPromelaObjectsForSporadicRis(functionName, *iter->second);
    }

    if (!channelNames.empty()) {
        // if the function hasn't sporadic interfaces, only synchronous,
        // then the list of channelNames is empty
        // the inline shall be generated when the list is not empty
        createCheckQueueInline(m_context.model(), functionName, channelNames);
    }
    createGetSenderInline(m_context.model(), functionName);
    if (m_context.isMulticastSupported()) {
        createSenderPidVariable(m_context.model(), functionName);
    }
}

void IvToPromelaGenerator::generateProctypeForTimer(const QString &functionName, const ProctypeInfo &proctypeInfo)
{
    generateUnhandledInputInline(functionName, proctypeInfo);

    generateProctype(functionName, false, proctypeInfo);
}

void IvToPromelaGenerator::createPromelaObjectsForAsyncPis(
        const QString &functionName, const ProctypeInfo &proctypeInfo)
{
    generateUnhandledInputInline(functionName, proctypeInfo);

    generateProctype(functionName, false, proctypeInfo);
}

void IvToPromelaGenerator::createPromelaObjectsForSyncRis(const QString &functionName, const RequiredCallInfo &info)
{
    QList<QString> arguments;
    QList<InlineCall::Argument> callArguments;
    for (const RequiredCallInfo::ParameterInfo &parameterInfo : info.m_parameters) {
        arguments.append(parameterInfo.m_parameterName);
        callArguments.append(parameterInfo.m_parameterName);
    }

    Sequence sequence(Sequence::Type::NORMAL);

    bool atLeastOneEnvironmentTarget = false;

    for (auto targetIter = info.m_targets.begin(); targetIter != info.m_targets.end(); ++targetIter) {
        const RequiredCallInfo::TargetInfo &targetInfo = targetIter->second;
        if (targetInfo.m_isEnvironment) {
            for (const RequiredCallInfo::ParameterInfo &parameterInfo : info.m_parameters) {
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
            atLeastOneEnvironmentTarget = true;
        } else {
            if (info.m_isProtected) {
                sequence.appendElement(createLockAcquireStatement(targetInfo.m_targetFunctionName));
            }
            if (m_context.isMulticastSupported()) {
                const QString senderVariableName =
                        QString("%1_sender").arg(Escaper::escapePromelaField(targetInfo.m_targetFunctionName));

                const QString pidName = QString("PID_%1").arg(Escaper::escapePromelaField(functionName));
                sequence.appendElement(Assignment(VariableRef(senderVariableName), Expression(VariableRef(pidName))));
            }
            sequence.appendElement(InlineCall(targetInfo.m_providedInlineName, callArguments));

            if (info.m_isProtected) {
                sequence.appendElement(createLockReleaseStatement(targetInfo.m_targetFunctionName));
            }
        }
    }

    if (sequence.getContent().size() == 0) {
        if (atLeastOneEnvironmentTarget) {
            sequence.appendElement(Skip());
        } else {
            auto message = QString("Empty content of inline %1").arg(info.m_name);
            throw TranslationException(message);
        }
    }

    std::unique_ptr<InlineDef> inlineDef = std::make_unique<InlineDef>(info.m_name, arguments, std::move(sequence));
    m_context.model()->addInlineDef(std::move(inlineDef));
}

void IvToPromelaGenerator::createPromelaObjectsForSporadicRis(const QString &functionName, const RequiredCallInfo &info)
{
    // create a inline to call sporadic REQUIRED interface.
    // such inline will put a message into channel.

    assert(info.m_parameters.size() <= 1); // sporadic interface shall have at most one parameter

    QList<QString> inlineArguments;
    QList<Expression> sendArguments;

    QString temporaryVariableName;
    QString temporaryVariableType;

    if (m_context.isMulticastSupported()) {
        // if multicast is supported - send a pid of the caller
        const QString pidName = QString("PID_%1").arg(Escaper::escapePromelaField(functionName));
        sendArguments.append(Expression(VariableRef(pidName)));
    }

    Sequence sequence(Sequence::Type::NORMAL);
    if (!info.m_parameters.empty()) {
        const RequiredCallInfo::ParameterInfo &parameterInfo = info.m_parameters.front();
        const QString argumentName = handleSendInlineArgument(
                parameterInfo.m_parameterType, functionName, info.m_interfaceName, parameterInfo.m_parameterName);

        const QString messageVariableName = globalMessageName(argumentName);
        temporaryVariableName = globalTemporaryVariableName(argumentName);
        temporaryVariableType = parameterInfo.m_parameterType;

        Declaration messageVariableDecl =
                Declaration(DataType(UtypeRef(messageTypeName(parameterInfo.m_parameterType))), messageVariableName);
        Declaration temporaryVariableDecl = Declaration(
                DataType(UtypeRef(Escaper::escapePromelaName(parameterInfo.m_parameterType))), temporaryVariableName);

        m_context.model()->addDeclaration(std::move(messageVariableDecl));
        m_context.model()->addDeclaration(std::move(temporaryVariableDecl));

        inlineArguments.append(argumentName);
        sendArguments.append(Expression(VariableRef(messageVariableName)));

        m_ccodeGenerator.generateConversionFromParameterToBuffer(
                sequence, argumentName, parameterInfo.m_parameterType, temporaryVariableName, messageVariableName);
    } else {
        if (info.m_parameters.empty() && sendArguments.empty()) {
            // if there's PID nor parameter, put zero in queue.
            sendArguments.append(Expression(Constant(0)));
        }
    }

    for (auto targetIter = info.m_targets.begin(); targetIter != info.m_targets.end(); ++targetIter) {
        const RequiredCallInfo::TargetInfo &targetInfo = targetIter->second;
        const QString channelName = targetInfo.m_providedQueueName;
        if (temporaryVariableName.isEmpty()) {
            QList<Expression> arguments;
            arguments.append(Expression(
                    StringConstant(QString("%1 %2: 0").arg(PromelaConstants::channelSendMessage, channelName))));
            sequence.appendElement(PrintfStatement(std::move(arguments)));
        } else {
            QList<PrintfStatement> statements = m_ccodeGenerator.printfStatements(temporaryVariableType,
                    temporaryVariableName, QString("%1 %2: ").arg(PromelaConstants::channelSendMessage, channelName));
            while (!statements.empty()) {
                sequence.appendElement(statements.takeFirst());
            }
        }
        sequence.appendElement(ChannelSend(VariableRef(channelName), sendArguments));
    }

    if (sequence.getContent().size() == 0) {
        auto message = QString("Empty content of inline %1").arg(info.m_name);
        throw TranslationException(message);
    }

    std::unique_ptr<InlineDef> inlineDef =
            std::make_unique<InlineDef>(info.m_name, inlineArguments, std::move(sequence));
    m_context.model()->addInlineDef(std::move(inlineDef));
}

void IvToPromelaGenerator::createPromelaObjectsForEnvironment(
        const QString &functionName, const FunctionInfo &functionInfo)
{
    for (auto iter = functionInfo.m_environmentSinkProctypes.begin();
            iter != functionInfo.m_environmentSinkProctypes.end(); ++iter) {
        const QString interfaceName = iter->second->m_interfaceName;
        const QString parameterName = iter->second->m_parameterName;
        const QString parameterType = iter->second->m_parameterTypeName;
        const QString sourceFunction = iter->second->m_possibleSenders.firstKey();
        const QString sourceInterface = iter->second->m_possibleSenders.first();
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

    for (auto iter = functionInfo.m_sporadicCalls.begin(); iter != functionInfo.m_sporadicCalls.end(); ++iter) {
        createPromelaObjectsForSporadicRis(functionName, *iter->second);
    }

    if (m_context.isMulticastSupported()) {
        createSenderPidVariable(m_context.model(), functionName);
    }

    for (auto iter = functionInfo.m_synchronousCalls.begin(); iter != functionInfo.m_synchronousCalls.end(); ++iter) {
        generateEnvironmentProctypeForSynchronousInterface(functionName, *iter->second);
    }
}

void IvToPromelaGenerator::createCheckQueueInline(
        PromelaSystemModel *promelaModel, const QString &functionName, const QList<QString> &channelNames)
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
        const QList<QString> &channelNames, bool empty)
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

void IvToPromelaGenerator::createGetSenderInline(model::PromelaSystemModel *promelaModel, const QString &functionName)
{
    // the SDL process requires a procedure '<processname>_get_sender'
    Sequence sequence(Sequence::Type::NORMAL);

    QList<QString> arguments;
    const QString argumentName = QString("%1_sender_arg").arg(Escaper::escapePromelaIV(functionName));
    arguments.append(argumentName);

    const QString senderVariableName = QString("%1_sender").arg(Escaper::escapePromelaField(functionName));

    if (m_context.isMulticastSupported()) {
        sequence.appendElement(Assignment(VariableRef(argumentName), Expression(VariableRef(senderVariableName))));
    } else {
        sequence.appendElement(Skip());
    }

    const QString checkQueueInlineName = QString("%1_0_RI_0_get_sender").arg(Escaper::escapePromelaIV(functionName));
    promelaModel->addInlineDef(std::make_unique<InlineDef>(checkQueueInlineName, arguments, std::move(sequence)));
}

void IvToPromelaGenerator::createSenderPidVariable(model::PromelaSystemModel *promelaModel, const QString &functionName)
{
    const QString senderVariableName = QString("%1_sender").arg(Escaper::escapePromelaField(functionName));

    promelaModel->addDeclaration(Declaration(DataType(UtypeRef("PID")), senderVariableName));
}

void IvToPromelaGenerator::createSystemState()
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

void IvToPromelaGenerator::createPromelaObjectsForTimers()
{
    std::map<QString, VariableRef> timerSignals;

    for (auto iter = m_systemInfo.m_functions.begin(); iter != m_systemInfo.m_functions.end(); ++iter) {
        const QString functionName = iter->first;
        for (auto proctypeIter = iter->second->m_proctypes.begin(); proctypeIter != iter->second->m_proctypes.end();
                ++proctypeIter) {
            if (!proctypeIter->second->m_isTimer) {
                continue;
            }
            const QString channelName = proctypeIter->second->m_observers.empty()
                    ? proctypeIter->second->m_queueName
                    : proctypeIter->second->m_observers.front()->m_observerQueue;
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
        const QString &functionName, const QString &timerName, const VariableRef &timerData)
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

void IvToPromelaGenerator::createGlobalTimerObjects(const std::map<QString, VariableRef> &timerSignals)
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

void IvToPromelaGenerator::createPromelaObjectsForObservers()
{
    // in case of multicast the definitions may be repeated,
    // prevent this using the set
    QSet<std::pair<QString, QString>> visitedObserverInterfaces;

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

                if (visitedObserverInterfaces.contains(std::make_pair(observerName, observerInterface))) {
                    continue;
                }

                visitedObserverInterfaces.insert(std::make_pair(observerName, observerInterface));
                // send inline
                const QString inlineName =
                        QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(observerName)).arg(observerInterface);

                QList<QString> inlineArguments;
                QList<Expression> sendParams;
                Sequence sequence(Sequence::Type::NORMAL);

                QString temporaryVariableName;

                if (m_context.isMulticastSupported()) {
                    const QString senderVariableName =
                            QString("%1_sender").arg(Escaper::escapePromelaField(toFunction));
                    sendParams.append(Expression(VariableRef(senderVariableName)));
                }

                if (!parameterType.isEmpty()) {
                    const QString name = QString("%1_%2_%3").arg(observerName).arg(toFunction).arg(parameterName);
                    inlineArguments.append(name);
                    const QString messageVariableName = globalMessageName(name);
                    temporaryVariableName = globalTemporaryVariableName(name);

                    Declaration messageVariableDecl =
                            Declaration(DataType(UtypeRef(messageTypeName(parameterType))), messageVariableName);
                    Declaration temporaryVariableDecl = Declaration(
                            DataType(UtypeRef(Escaper::escapePromelaName(parameterType))), temporaryVariableName);

                    m_context.model()->addDeclaration(std::move(messageVariableDecl));
                    m_context.model()->addDeclaration(std::move(temporaryVariableDecl));

                    sendParams.append(Expression(VariableRef(messageVariableName)));
                    m_ccodeGenerator.generateConversionFromParameterToBuffer(
                            sequence, name, parameterType, temporaryVariableName, messageVariableName);
                    QList<PrintfStatement> statements =
                            m_ccodeGenerator.printfStatements(parameterType, temporaryVariableName,
                                    QString("%1 %2: ").arg(PromelaConstants::channelSendMessage, channelName));
                    while (!statements.empty()) {
                        sequence.appendElement(statements.takeFirst());
                    }
                } else {
                    sendParams.append(Expression(Constant(1)));

                    QList<Expression> printfArguments;
                    printfArguments.append(Expression(StringConstant(
                            QString("%1 %2: 0").arg(PromelaConstants::channelSendMessage, channelName))));
                    sequence.appendElement(PrintfStatement(std::move(printfArguments)));
                }

                sequence.appendElement(ChannelSend(VariableRef(channelName), sendParams));

                m_context.model()->addInlineDef(
                        std::make_unique<InlineDef>(inlineName, inlineArguments, std::move(sequence)));
            }
        }
    }
}

std::unique_ptr<ProctypeElement> IvToPromelaGenerator::createLockAcquireStatement(const QString &functionName)
{
    const VariableRef lockChannelName = VariableRef(getFunctionLockChannelName(functionName));
    QList<VariableRef> lockChannelArguments;
    lockChannelArguments.append(VariableRef("_"));

    return std::make_unique<ProctypeElement>(ChannelRecv(lockChannelName, lockChannelArguments));
}

std::unique_ptr<ProctypeElement> IvToPromelaGenerator::createLockReleaseStatement(const QString &functionName)
{
    const VariableRef lockChannelName = VariableRef(getFunctionLockChannelName(functionName));

    QList<Expression> unlockChannelArguments;
    unlockChannelArguments.append(Expression(Constant(1)));
    return std::make_unique<ProctypeElement>(ChannelSend(VariableRef(lockChannelName), unlockChannelArguments));
}

std::unique_ptr<ProctypeElement> IvToPromelaGenerator::createProcessInlineCall(
        const QString &inlineName, const QString &parameterType, const QString &parameterName)
{
    if (parameterType.isEmpty()) {
        return std::make_unique<ProctypeElement>((InlineCall(inlineName, {})));
    } else {
        QList<InlineCall::Argument> arguments;
        arguments.append(VariableRef(parameterName));
        return std::make_unique<ProctypeElement>(InlineCall(inlineName, arguments));
    }
}

std::unique_ptr<model::ProctypeElement> IvToPromelaGenerator::createReceiveStatement(const QString &functionName,
        const QString &channelName, const QString &parameterType, const QString &parameterName)
{
    // channel receive
    QList<VariableRef> receiveParams;
    if (m_context.isMulticastSupported()) {
        const QString senderVariableName = QString("%1_sender").arg(Escaper::escapePromelaField(functionName));
        receiveParams.append(VariableRef(senderVariableName));
        if (!parameterType.isEmpty()) {
            receiveParams.append(VariableRef(parameterName));
        }
    } else {
        receiveParams.append(VariableRef(parameterType.isEmpty() ? QString("_") : parameterName));
    }

    return std::make_unique<ProctypeElement>(ChannelRecv(VariableRef(channelName), receiveParams));
}

QString IvToPromelaGenerator::getFunctionLockChannelName(const QString &functionName)
{
    return QString("%1_lock").arg(Escaper::escapePromelaIV(functionName));
}

void IvToPromelaGenerator::addChannelAndLock(const QString &functionName)
{
    QList<ChannelInit::Type> channelType;
    channelType.append(BasicType::INT);
    ChannelInit channelInit(1, std::move(channelType));
    Declaration channelDeclaration(DataType(BasicType::CHAN), getFunctionLockChannelName(functionName));
    channelDeclaration.setInit(channelInit);
    m_context.model()->addDeclaration(channelDeclaration);
}

QString IvToPromelaGenerator::handleParameterSubtype(const QString &parameterTypeName, const QString &parameterName,
        const QString &interfaceName, const QString &functionName)
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
        const QString &interfaceName, const QString parameterName)
{
    if (parameterType.isEmpty()) {
        return "";
    } else {
        return QString("%1_%2_%3").arg(functionName).arg(interfaceName).arg(parameterName);
    }
}

QString IvToPromelaGenerator::globalTemporaryVariableName(const QString &parameterName)
{
    return QString("%1_var").arg(Escaper::escapePromelaName(parameterName));
}

QString IvToPromelaGenerator::globalMessageName(const QString &parameterName)
{
    return QString("%1_message").arg(Escaper::escapePromelaName(parameterName));
}

QString IvToPromelaGenerator::buildParameterSubtypeName(
        const QString &functionName, const QString &interfaceName, const QString &parameterName)
{
    auto subtypeName = QString("%1-%2-%3").arg(functionName).arg(interfaceName).arg(parameterName);
    subtypeName.replace('_', '-');
    subtypeName = subtypeName.toLower();
    subtypeName[0] = subtypeName[0].toUpper();

    return subtypeName;
}

void IvToPromelaGenerator::createChannel(const QString &channelName, const QString &messageType, size_t channelSize)
{
    if (m_createdChannels.contains(channelName)) {
        return;
    }
    m_createdChannels.insert(channelName);
    QList<ChannelInit::Type> channelType;
    if (m_context.isMulticastSupported()) {
        channelType.append(ChannelInit::Type(UtypeRef("PID")));
        if (!messageType.isEmpty()) {
            QString modifiedMessageType = messageTypeName(messageType);
            channelType.append(ChannelInit::Type(UtypeRef(modifiedMessageType)));
        }
    } else {
        channelType.append(messageType.isEmpty() ? ChannelInit::Type(BasicType::INT)
                                                 : ChannelInit::Type(UtypeRef(messageTypeName(messageType))));
    }
    ChannelInit channelInit(channelSize, std::move(channelType));
    Declaration declaration(DataType(BasicType::CHAN), channelName);
    declaration.setInit(channelInit);
    m_context.model()->addDeclaration(std::move(declaration));
}

QString IvToPromelaGenerator::messageTypeName(const QString &typeName)
{
    return QString("%1Message").arg(Escaper::escapePromelaName(typeName));
}
}
