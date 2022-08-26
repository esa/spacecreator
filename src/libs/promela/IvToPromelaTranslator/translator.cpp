/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "translator.h"

#include <QDebug>
#include <conversion/common/escaper/escaper.h>
#include <ivcore/ivconnection.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <promela/PromelaOptions/options.h>
#include <shared/exportableproperty.h>

using Asn1Acn::Asn1Model;
using conversion::Escaper;
using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::promela::PromelaOptions;
using conversion::translator::TranslationException;
using ivm::IVConnection;
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVInterfaceRequired;
using ivm::IVModel;
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
using promela::model::Proctype;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::Sequence;
using promela::model::Skip;
using promela::model::Utype;
using promela::model::UtypeRef;
using promela::model::VariableRef;
using shared::ExportableProperty;
using shared::InterfaceParameter;
using shared::VEObject;

namespace promela::translator {

IvToPromelaTranslator::ObserverAttachment::Kind IvToPromelaTranslator::ObserverAttachment::stringToKind(
        const QString &kind)
{
    const auto kindIn = QString("ObservedSignalKind.INPUT");
    const auto kindOut = QString("ObservedSignalKind.OUTPUT");
    if (kind == kindIn) {
        return ObserverAttachment::Kind::Kind_Input;
    } else if (kind == kindOut) {
        return ObserverAttachment::Kind::Kind_Output;
    } else {
        const auto message = QString("Observer kind %1 is unknown").arg(kind);
        throw TranslationException(message);
    }
}

IvToPromelaTranslator::ObserverAttachment::ObserverAttachment(const QString &specification)
{
    const auto separator = QString(":");
    const auto recipientPrefix = QString(">");
    const auto senderPrefix = QString("<");
    const auto priorityPrefix = QString("p");
    const auto elements = specification.split(separator, QString::KeepEmptyParts);
    if (elements.size() < 4) {
        const auto message =
                QString("Observer attachment specification <%1> contains too few elements").arg(specification);
        throw TranslationException(message);
    }
    m_observerName = elements[0];
    m_kind = stringToKind(elements[1]);
    m_observerInterfaceName = elements[2];
    m_interfaceName = elements[3];
    m_priority = 1;

    for (auto i = 4; i < elements.size(); i++) {
        if (elements[i].startsWith(recipientPrefix)) {
            m_toFunctionName = elements[i].right(elements[i].length() - 1);
        } else if (elements[i].startsWith(senderPrefix)) {
            m_fromFunctionName = elements[i].right(elements[i].length() - 1);
        } else if (elements[i].startsWith(priorityPrefix)) {
            bool ok = false;
            const auto priorityString = elements[i].right(elements[i].length() - 1);
            m_priority = priorityString.toUInt(&ok);
            if (!ok) {
                const auto message = QString("Priority %1 could not be parsed as an integer").arg(priorityString);
                throw TranslationException(message);
            }
        }
    }

    if (m_interfaceName.isEmpty()) {
        const auto message = QString("Observed interface name is empty in observer %1 ").arg(m_observerName);
        throw TranslationException(message);
    }
}

std::optional<QString> IvToPromelaTranslator::ObserverAttachment::toFunction() const
{
    return m_toFunctionName;
}

std::optional<QString> IvToPromelaTranslator::ObserverAttachment::fromFunction() const
{
    return m_fromFunctionName;
}

const QString &IvToPromelaTranslator::ObserverAttachment::observerInterface() const
{
    return m_observerInterfaceName;
}

const QString &IvToPromelaTranslator::ObserverAttachment::interface() const
{
    return m_interfaceName;
}

const QString &IvToPromelaTranslator::ObserverAttachment::observer() const
{
    return m_observerName;
}

IvToPromelaTranslator::ObserverAttachment::Kind IvToPromelaTranslator::ObserverAttachment::kind() const
{
    return m_kind;
}

IvToPromelaTranslator::ObserverAttachment::Priority IvToPromelaTranslator::ObserverAttachment::priority() const
{
    return m_priority;
}

IvToPromelaTranslator::Context::Context(model::PromelaModel *promelaModel, const IVModel *ivModel,
        const conversion::Options &options, const std::vector<const Asn1Acn::Definitions *> &asn1SubtypesDefinitons,
        const std::vector<QString> &modelFunctions, const std::vector<QString> &observerNames)
    : m_promelaModel(promelaModel)
    , m_ivModel(ivModel)
    , m_options(options)
    , m_asn1SubtypesDefinitons(asn1SubtypesDefinitons)
    , m_modelFunctions(modelFunctions)
    , m_observerNames(observerNames)
    , m_baseProctypePriority(0)
{
}

void IvToPromelaTranslator::Context::addObserverAttachment(const IvToPromelaTranslator::ObserverAttachment &attachment)
{
    if (attachment.fromFunction().has_value()) {
        m_fromObserverAttachments[*attachment.fromFunction()][attachment.interface()].push_back(attachment);
    }
    if (attachment.toFunction().has_value()) {
        m_toObserverAttachments[*attachment.toFunction()][attachment.interface()].push_back(attachment);
    }
}

auto IvToPromelaTranslator::Context::getObserverAttachments(const QString &function, const QString &interface,
        const ObserverAttachment::Kind kind) -> const IvToPromelaTranslator::ObserverAttachments
{
    ObserverAttachments result;
    if (m_toObserverAttachments.find(function) == m_toObserverAttachments.end()) {
        return result;
    }
    const auto &attachments = m_toObserverAttachments.at(function);
    if (attachments.find(interface) == attachments.end()) {
        return result;
    }
    for (const auto &attachment : attachments.at(interface)) {
        if (attachment.kind() == kind) {
            result.push_back(attachment);
        }
    }
    std::sort(result.begin(), result.end(),
            [](const auto &a, const auto &b) -> bool { return a.priority() > b.priority(); });
    return result;
}

auto IvToPromelaTranslator::Context::hasObserverAttachments(
        const QString &function, const QString &interface, const ObserverAttachment::Kind kind) -> bool
{
    return getObserverAttachments(function, interface, kind).size() > 0;
}

auto IvToPromelaTranslator::Context::getObserverAttachments(const ObserverAttachment::Kind kind)
        -> const ObserverAttachments
{
    ObserverAttachments result;
    if (kind == ObserverAttachment::Kind::Kind_Output) {
        for (auto functionIter = m_fromObserverAttachments.begin(); functionIter != m_fromObserverAttachments.end();
                ++functionIter) {
            for (auto interfaceIter = functionIter->second.begin(); interfaceIter != functionIter->second.end();
                    ++interfaceIter) {
                std::copy_if(interfaceIter->second.begin(), interfaceIter->second.end(), std::back_inserter(result),
                        [kind](const ObserverAttachment &attachment) { return attachment.kind() == kind; });
            }
        }
    } else {
        for (auto functionIter = m_toObserverAttachments.begin(); functionIter != m_toObserverAttachments.end();
                ++functionIter) {
            for (auto interfaceIter = functionIter->second.begin(); interfaceIter != functionIter->second.end();
                    ++interfaceIter) {
                std::copy_if(interfaceIter->second.begin(), interfaceIter->second.end(), std::back_inserter(result),
                        [kind](const ObserverAttachment &attachment) { return attachment.kind() == kind; });
            }
        }
    }
    return result;
}

auto IvToPromelaTranslator::Context::model() const -> model::PromelaModel *
{
    return m_promelaModel;
}

const IVModel *IvToPromelaTranslator::Context::ivModel() const
{
    return m_ivModel;
}

const conversion::Options &IvToPromelaTranslator::Context::options() const
{
    return m_options;
}

const std::vector<const Asn1Acn::Definitions *> &IvToPromelaTranslator::Context::subtypesDefinitions() const
{
    return m_asn1SubtypesDefinitons;
}

const std::vector<QString> &IvToPromelaTranslator::Context::modelFunctions() const
{
    return m_modelFunctions;
}

const std::vector<QString> &IvToPromelaTranslator::Context::observerNames() const
{
    return m_observerNames;
}

void IvToPromelaTranslator::Context::setBaseProctypePriority(uint32_t priority)
{
    m_baseProctypePriority = priority;
}

uint32_t IvToPromelaTranslator::Context::getBaseProctypePriority() const
{
    return m_baseProctypePriority;
}

void IvToPromelaTranslator::addChannelAndLock(
        IvToPromelaTranslator::Context &context, const QString &functionName) const
{
    QList<ChannelInit::Type> channelType;
    channelType.append(BasicType::INT);
    ChannelInit channelInit(1, std::move(channelType));
    Declaration channelDeclaration(DataType(BasicType::CHAN), getFunctionLockChannelName(functionName));
    channelDeclaration.setInit(channelInit);
    context.model()->addDeclaration(channelDeclaration);
}

std::vector<std::unique_ptr<Model>> IvToPromelaTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    const auto &additionalIncludes = options.values(PromelaOptions::additionalIncludes);
    const auto &modelFunctions = options.values(PromelaOptions::modelFunctionName);
    const auto &environmentFunctions = options.values(PromelaOptions::environmentFunctionName);
    const auto &observerAttachmentInfos = options.values(PromelaOptions::observerAttachment);
    const auto &observerNames = options.values(PromelaOptions::observerFunctionName);
    auto promelaModel = std::make_unique<PromelaModel>();
    const auto *ivModel = getModel<IVModel>(sourceModels);
    const auto *asn1Model = getModel<Asn1Model>(sourceModels);
    const auto asn1SubtypesDefinitions = getSubtypesDefinitions(asn1Model, options);

    Context context(promelaModel.get(), ivModel, options, asn1SubtypesDefinitions, modelFunctions, observerNames);

    for (const auto &info : observerAttachmentInfos) {
        context.addObserverAttachment(ObserverAttachment(info));
    }

    if (options.isSet(PromelaOptions::processesBasePriority)) {
        context.setBaseProctypePriority(options.value(PromelaOptions::processesBasePriority)->toUInt());
    }

    const auto ivFunctionList = ivModel->allObjectsByType<IVFunction>();

    promelaModel->addInclude("dataview.pml");

    promelaModel->addDeclaration(Declaration(DataType(BasicType::INT), m_systemInitedVariableName));

    createPromelaObjectsForTimers(context);

    createPromelaObjectsForObservers(context);

    for (const IVFunction *ivFunction : ivFunctionList) {
        const QString functionName = ivFunction->property("name").toString();

        if (std::find(modelFunctions.begin(), modelFunctions.end(), functionName) != modelFunctions.end()) {
            promelaModel->addInclude(QString("%1.pml").arg(functionName.toLower()));
            createPromelaObjectsForFunction(context, ivFunction, functionName);
        } else if (std::find(environmentFunctions.begin(), environmentFunctions.end(), functionName)
                != environmentFunctions.end()) {
            createPromelaObjectsForEnvironment(context, ivFunction, functionName);
        }
    }

    for (const auto &observerName : observerNames) {
        promelaModel->addInclude(QString("%1.pml").arg(observerName));
    }

    promelaModel->addInclude("env_inlines.pml");

    createSystemState(context);

    for (const QString &function : modelFunctions) {
        addChannelAndLock(context, function);
    }

    for (const auto &observer : observerNames) {
        addChannelAndLock(context, observer);
    }

    (generateInitProctype(context));

    for (const QString &additionalInclude : additionalIncludes) {
        promelaModel->addEpilogueInclude(additionalInclude);
    }

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(promelaModel));
    return result;
}

ModelType IvToPromelaTranslator::getSourceModelType() const
{
    return ModelType::InterfaceView;
}

ModelType IvToPromelaTranslator::getTargetModelType() const
{
    return ModelType::Promela;
}

std::set<ModelType> IvToPromelaTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::InterfaceView, ModelType::Asn1 };
}

void IvToPromelaTranslator::initializeFunction(Sequence &sequence, const QString &functionName) const
{
    QString initFn = QString("%1_0_init").arg(Escaper::escapePromelaIV(functionName));
    sequence.appendElement(InlineCall(initFn, {}));

    const VariableRef lockChannelName = VariableRef(getFunctionLockChannelName(functionName));
    QList<Expression> lockChannelArguments;
    lockChannelArguments.append(Expression(Constant(1)));

    sequence.appendElement(ChannelSend(VariableRef(lockChannelName), lockChannelArguments));
}

void IvToPromelaTranslator::generateInitProctype(Context &context) const
{
    Sequence sequence(Sequence::Type::ATOMIC);

    sequence.appendElement(InlineCall("global_dataview_init", {}));

    for (const QString &functionName : context.modelFunctions()) {
        initializeFunction(sequence, functionName);
    }

    for (const QString &observer : context.observerNames()) {
        initializeFunction(sequence, observer);
    }

    sequence.appendElement(Assignment(VariableRef(m_systemInitedVariableName), Expression(Constant(1))));

    context.model()->setInit(InitProctype(std::move(sequence)));
}

QString IvToPromelaTranslator::observerInputSignalName(
        const IvToPromelaTranslator::ObserverAttachment &attachment) const
{
    return QString("%1_0_PI_0_%2")
            .arg(Escaper::escapePromelaIV(attachment.observer()))
            .arg(Escaper::escapePromelaName(attachment.observerInterface()));
}

std::list<std::unique_ptr<promela::model::ProctypeElement>> IvToPromelaTranslator::attachInputObservers(
        IvToPromelaTranslator::Context &context, const QString &functionName, const QString &interfaceName,
        const QString &parameterName, const QString &parameterType) const
{
    std::list<std::unique_ptr<promela::model::ProctypeElement>> result;
    auto attachments = context.getObserverAttachments(
            functionName, interfaceName, IvToPromelaTranslator::ObserverAttachment::Kind::Kind_Input);
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

void IvToPromelaTranslator::generateProctype(Context &context, const QString &functionName,
        const QString &interfaceName, const QString &parameterType, size_t queueSize, size_t priority,
        bool environment) const
{
    QString channelName = constructChannelName(functionName, interfaceName);

    ObserverAttachments outputObservers =
            getObserverAttachments(context, functionName, interfaceName, ObserverAttachment::Kind::Kind_Output);
    QList<QString> channelNames;
    channelNames.append(channelName);
    for (const ObserverAttachment &attachment : outputObservers) {
        const QString toFunction = getAttachmentToFunction(context.ivModel(), attachment);
        channelNames.append(observerChannelName(attachment, toFunction));
    }

    QList<ChannelInit::Type> channelType;
    channelType.append(parameterType.isEmpty()
                    ? ChannelInit::Type(BasicType::INT)
                    : ChannelInit::Type(UtypeRef(Escaper::escapePromelaName(parameterType))));
    ChannelInit channelInit(queueSize, std::move(channelType));
    Declaration declaration(DataType(BasicType::CHAN), channelName);
    declaration.setInit(channelInit);
    context.model()->addDeclaration(declaration);

    Sequence sequence(Sequence::Type::NORMAL);
    sequence.appendElement(createWaitForInitStatement());

    const QString &signalParameterName =
            QString("%1_%2_signal_parameter").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName.toLower());
    const QString channelUsedName =
            QString("%1_%2_channel_used").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName.toLower());

    if (!parameterType.isEmpty()) {
        // parameter variable declaration
        // channel used declaration
        context.model()->addDeclaration(
                Declaration(DataType(UtypeRef(Escaper::escapePromelaName(parameterType))), signalParameterName));
        Declaration channelUsedDeclaration = Declaration(DataType(BasicType::BOOLEAN), channelUsedName);
        channelUsedDeclaration.setInit(Expression(Constant(0)));
        context.model()->addDeclaration(channelUsedDeclaration);
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

    const QString mainLoopLabel =
            QString("%1_%2_loop").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName.toLower());

    loopSequence->appendElement(Label(mainLoopLabel));

    // the incoming message is processed by output observers first
    // and then by sdl process
    // however, code of message processing is generated in reverse order
    // first the process, and then the observers

    // process sdl process
    {
        const QString currentChannelName = channelNames.back();
        channelNames.pop_back();

        const QString piName = environment
                ? QString()
                : QString("%1_0_PI_0_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

        // Observers can be also attached to environment
        auto observerStatements =
                attachInputObservers(context, functionName, interfaceName, signalParameterName, parameterType);

        loopSequence->appendElement(generateProcessMessageBlock(functionName, currentChannelName, piName, parameterType,
                signalParameterName, mainLoopLabel, false, std::move(observerStatements)));
    }

    // process all observers
    for (auto iter = outputObservers.rbegin(); iter != outputObservers.rend(); ++iter) {
        const ObserverAttachment &attachment = *iter;
        const QString currentChannelName = channelNames.front();
        channelNames.pop_front();

        loopSequence->appendElement(generateProcessMessageBlock(attachment.observer(), currentChannelName,
                observerInputSignalName(attachment), parameterType, signalParameterName, mainLoopLabel, true, {}));
    }

    // release function mutex
    if (!environment) {
        loopSequence->appendElement(createLockReleaseStatement(functionName));
    }

    DoLoop loop;

    loop.appendSequence(std::move(loopSequence));

    sequence.appendElement(std::move(loop));

    const QString proctypeName = QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);
    std::unique_ptr<Proctype> proctype = std::make_unique<Proctype>(proctypeName, std::move(sequence));

    proctype->setActive(true);
    proctype->setInstancesCount(1);
    proctype->setPriority(priority);

    context.model()->addProctype(std::move(proctype));
}

std::unique_ptr<ProctypeElement> IvToPromelaTranslator::generateProcessMessageBlock(const QString &functionName,
        const QString &channelName, const QString &inlineName, const QString &parameterType,
        const QString &parameterName, const QString &exitLabel, bool lock,
        std::list<std::unique_ptr<promela::model::ProctypeElement>> additionalElements) const
{
    QList<InlineCall::Argument> checkQueueArguments;
    checkQueueArguments.append(VariableRef(channelName));

    std::unique_ptr<Sequence> processMessageSeq = std::make_unique<Sequence>(Sequence::Type::NORMAL);
    processMessageSeq->appendElement(InlineCall("nempty", checkQueueArguments));

    // channel receive
    processMessageSeq->appendElement(createReceiveStatement(channelName, parameterType, parameterName));

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

    while (!additionalElements.empty()) {
        processMessageSeq->appendElement(std::move(additionalElements.front()));
        additionalElements.pop_front();
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

void IvToPromelaTranslator::generateEnvironmentProctype(Context &context, const QString &functionName,
        const QString &interfaceName, const QString &parameterType, const QString &sendInline) const
{
    Sequence sequence(Sequence::Type::NORMAL);
    sequence.appendElement(createWaitForInitStatement());

    if (!parameterType.isEmpty()) {
        sequence.appendElement(Declaration(DataType(UtypeRef(Escaper::escapePromelaName(parameterType))), "value"));
    }

    const auto &globalInputVectorLengthLimit = context.options().value(PromelaOptions::globalInputVectorLengthLimit);
    const auto &interfaceInputVectorLenghtLimit =
            context.options().value(PromelaOptions::interfaceInputVectorLengthLimit.arg(interfaceName.toLower()));

    std::unique_ptr<Sequence> loopSequence = std::make_unique<Sequence>(Sequence::Type::ATOMIC);

    if (parameterType.isEmpty()) {
        loopSequence->appendElement(InlineCall(sendInline, {}));
    } else {
        const QString generateValueInlineName =
                QString("%1_generate_value").arg(Escaper::escapePromelaName(parameterType));
        QList<InlineCall::Argument> arguments;
        arguments.append(VariableRef("value"));

        loopSequence->appendElement(InlineCall(generateValueInlineName, arguments));
        loopSequence->appendElement(InlineCall(sendInline, arguments));
    }

    int limit = 0;
    if (interfaceInputVectorLenghtLimit.has_value()) {
        limit = interfaceInputVectorLenghtLimit->toInt();
    } else if (globalInputVectorLengthLimit.has_value()) {
        limit = globalInputVectorLengthLimit->toInt();
    }

    if (limit == 0) {
        DoLoop loop;
        loop.appendSequence(std::move(loopSequence));

        sequence.appendElement(std::move(loop));
    } else {
        Declaration iteratorVariable(DataType(UtypeRef("int")), "inputVectorCounter");
        sequence.appendElement(std::move(iteratorVariable));

        VariableRef iteratorVariableRef("inputVectorCounter");
        Expression firstExpression(0);
        Expression lastExpression(limit - 1);

        ForLoop loop(std::move(iteratorVariableRef), firstExpression, lastExpression, std::move(loopSequence));

        sequence.appendElement(std::move(loop));
    }

    const QString proctypeName = QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

    std::unique_ptr<Proctype> proctype = std::make_unique<Proctype>(proctypeName, std::move(sequence));

    proctype->setActive(true);
    proctype->setInstancesCount(1);
    proctype->setPriority(1);

    context.model()->addProctype(std::move(proctype));
}

std::unique_ptr<model::ProctypeElement> IvToPromelaTranslator::createWaitForInitStatement() const
{
    return std::make_unique<ProctypeElement>(Expression(VariableRef(m_systemInitedVariableName)));
}

void IvToPromelaTranslator::generateSendInline(Context &context, const QString &functionName,
        const QString &interfaceName, const QString &parameterName, const QString &parameterType,
        const QString &sourceFunctionName, const QString &sourceInterfaceName, const bool parameterSubtyped) const
{
    QString inlineName =
            QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(sourceFunctionName)).arg(sourceInterfaceName);
    QString channelName = constructChannelName(functionName, interfaceName);

    Sequence sequence(Sequence::Type::NORMAL);

    const auto argumentName =
            handleSendInlineArgument(parameterType, functionName, interfaceName, parameterName, sequence);

    QList<QString> arguments;
    QList<Expression> params;

    if (argumentName.isEmpty()) {
        params.append(Expression(VariableRef(m_dummyParamName)));
    } else {
        if (parameterSubtyped) {
            params.append(handleSendInlineParameter(argumentName, parameterType, sequence));
        } else {
            params.append(Expression(VariableRef(argumentName)));
        }

        arguments.push_back(argumentName);
    }

    sequence.appendElement(ChannelSend(VariableRef(channelName), params));

    context.model()->addInlineDef(std::make_unique<InlineDef>(inlineName, arguments, std::move(sequence)));
}

void IvToPromelaTranslator::createPromelaObjectsForFunction(
        IvToPromelaTranslator::Context &context, const IVFunction *ivFunction, const QString &functionName) const
{
    QList<QString> channelNames;

    for (const auto providedInterface : ivFunction->pis()) {
        if (providedInterface->kind() == IVInterface::OperationKind::Cyclic
                || providedInterface->kind() == IVInterface::OperationKind::Sporadic) {
            const auto interfaceName = getInterfaceName(providedInterface);
            const auto priority = getInterfacePriority(providedInterface) + context.getBaseProctypePriority();
            createPromelaObjectsForAsyncPis(context, providedInterface, functionName, interfaceName, priority);
            ObserverAttachments outputObservers =
                    context.getObserverAttachments(functionName, interfaceName, ObserverAttachment::Kind::Kind_Output);
            for (const ObserverAttachment &attachment : outputObservers) {
                const QString toFunction = getAttachmentToFunction(context.ivModel(), attachment);
                channelNames.append(observerChannelName(attachment, toFunction));
            }
            channelNames.append(constructChannelName(functionName, interfaceName));
        } else {
            auto message =
                    QString("Unallowed interface kind in function %1, only sporadic and cyclic interfaces are allowed")
                            .arg(functionName);
            throw TranslationException(message);
        }
    }

    const QVector<shared::ContextParameter> parameters = ivFunction->contextParams();

    for (const shared::ContextParameter &parameter : parameters) {
        if (parameter.paramType() == shared::BasicParameter::Type::Timer) {
            generateProctype(context, functionName, parameter.name().toLower(), QString(), 1, 1, false);
        }
    }

    createCheckQueueInline(context.model(), functionName, channelNames);

    for (const auto requiredInterface : ivFunction->ris()) {
        if (requiredInterface->kind() == IVInterface::OperationKind::Protected
                || requiredInterface->kind() == IVInterface::OperationKind::Unprotected) {
            createPromelaObjectsForSyncRis(context, requiredInterface, functionName);
        }
    }
}

void IvToPromelaTranslator::createPromelaObjectsForAsyncPis(IvToPromelaTranslator::Context &context,
        const IVInterface *providedInterface, const QString &functionName, const QString &interfaceName,
        const std::size_t priority) const
{
    const auto connection = context.ivModel()->getConnectionForIface(providedInterface->id());

    const auto requiredInterface = connection->sourceInterface();
    const auto sourceInterfaceName = getInterfaceName(requiredInterface);
    const auto sourceFunctionName = getInterfaceFunctionName(requiredInterface);

    const auto queueSize = getInterfaceQueueSize(providedInterface);
    const auto &[parameterName, parameterType] = getInterfaceParameter(providedInterface);

    const auto parameterSubtyped =
            isParameterSubtyped(context, parameterType, parameterName, sourceInterfaceName, sourceFunctionName);

    generateSendInline(context, functionName, interfaceName, parameterName, parameterType, sourceFunctionName,
            sourceInterfaceName, parameterSubtyped);

    generateProctype(context, functionName, interfaceName, parameterType, queueSize, priority, false);
}

void IvToPromelaTranslator::createPromelaObjectsForSyncRis(IvToPromelaTranslator::Context &context,
        const IVInterface *requiredInterface, const QString &functionName) const
{
    Sequence sequence(Sequence::Type::NORMAL);

    QList<QString> arguments;
    for (const auto &interfaceParam : requiredInterface->params()) {
        const auto &paramName = interfaceParam.name();
        const auto &paramTypeName = interfaceParam.paramTypeName();

        arguments.append(paramName);

        if (interfaceParam.isInDirection()) {
            continue;
        }

        const auto generateValueInlineName = QString("%1_generate_value").arg(paramTypeName);
        const QList<InlineCall::Argument> generateValueInlineArgs({ paramName });
        InlineCall generateValueInlineCall(generateValueInlineName, generateValueInlineArgs);
        sequence.appendElement(std::move(generateValueInlineCall));
    }

    const auto inlineName = QString("%1_0_%2").arg(functionName).arg(requiredInterface->title());
    auto inlineDef = std::make_unique<InlineDef>(inlineName, arguments, std::move(sequence));
    context.model()->addInlineDef(std::move(inlineDef));
}

void IvToPromelaTranslator::createPromelaObjectsForEnvironment(
        IvToPromelaTranslator::Context &context, const IVFunction *ivFunction, const QString &functionName) const
{
    QVector<IVInterface *> providedInterfaceList = ivFunction->pis();
    for (IVInterface *providedInterface : providedInterfaceList) {
        if (providedInterface->kind() != IVInterface::OperationKind::Sporadic) {
            continue;
        }

        IVConnection *connection = context.ivModel()->getConnectionForIface(providedInterface->id());

        IVInterface *requiredInterface = connection->sourceInterface();
        const QString sourceInterfaceName = getInterfaceName(requiredInterface);
        const QString sourceFunctionName = getInterfaceFunctionName(requiredInterface);

        const QString interfaceName = getInterfaceName(providedInterface);
        const auto &[parameterName, parameterType] = getInterfaceParameter(providedInterface);

        const auto parameterSubtyped =
                isParameterSubtyped(context, parameterType, parameterName, interfaceName, functionName);

        generateSendInline(context, functionName, interfaceName, parameterName, parameterType, sourceFunctionName,
                sourceInterfaceName, parameterSubtyped);

        const size_t queueSize = getInterfaceQueueSize(providedInterface);
        const size_t priority = getInterfacePriority(providedInterface) + context.getBaseProctypePriority();

        generateProctype(context, functionName, interfaceName, parameterType, queueSize, priority, true);
    }

    QVector<IVInterface *> requiredInterfaceList = ivFunction->ris();
    for (IVInterface *requiredInterface : requiredInterfaceList) {
        if (requiredInterface->kind() != IVInterface::OperationKind::Sporadic) {
            auto message =
                    QString("Unallowed interface kind in function %1, only sporadic required interfaces are allowed")
                            .arg(functionName);
            throw TranslationException(message);
        }

        const QString interfaceName = getInterfaceName(requiredInterface);

        const std::pair<QString, QString> parameter = getInterfaceParameter(requiredInterface);

        const QString parameterName = parameter.first;
        const QString parameterType =
                handleParameterSubtype(context, parameter.second, parameterName, interfaceName, functionName);

        const QString sendInline =
                QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

        generateEnvironmentProctype(context, functionName, interfaceName, parameterType, sendInline);
    }
}

void IvToPromelaTranslator::createCheckQueueInline(
        PromelaModel *promelaModel, const QString &functionName, const QList<QString> &channelNames) const
{
    if (channelNames.empty()) {
        auto message = QString("No sporadic nor cyclic interfaces in function %1").arg(functionName);
        throw TranslationException(message);
    }

    Sequence sequence(Sequence::Type::ATOMIC);

    std::unique_ptr<Expression> expr = createCheckQueuesExpression(channelNames, true);

    sequence.appendElement(*expr);

    const QString checkQueueInlineName = QString("%1_check_queue").arg(Escaper::escapePromelaIV(functionName));
    promelaModel->addInlineDef(
            std::make_unique<InlineDef>(checkQueueInlineName, QList<QString>(), std::move(sequence)));
}

std::unique_ptr<Expression> IvToPromelaTranslator::createCheckQueuesExpression(
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

void IvToPromelaTranslator::createSystemState(Context &context) const
{
    QVector<IVFunction *> ivFunctionList = context.ivModel()->allObjectsByType<IVFunction>();

    Utype systemState("system_state");

    for (const auto &observer : context.observerNames()) {
        QString dataType = QString("%1_Context").arg(Escaper::escapePromelaIV(observer));
        QString fieldName = Escaper::escapePromelaField(observer);
        systemState.addField(Declaration(DataType(UtypeRef(dataType)), fieldName));
    }

    const auto &modelFunctions = context.modelFunctions();
    for (IVFunction *ivFunction : ivFunctionList) {
        const QString functionName = ivFunction->property("name").toString();
        if (std::find(modelFunctions.begin(), modelFunctions.end(), functionName) == modelFunctions.end()) {
            continue;
        }
        if (ivFunction->instanceOf() != nullptr) {
            const QString functionTypeName = ivFunction->instanceOf()->property("name").toString();
            QString dataType = QString("%1_Context").arg(Escaper::escapePromelaIV(functionTypeName));
            QString fieldName = Escaper::escapePromelaField(functionName);
            systemState.addField(Declaration(DataType(UtypeRef(dataType)), fieldName));
        } else {
            QString dataType = QString("%1_Context").arg(Escaper::escapePromelaIV(functionName));
            QString fieldName = Escaper::escapePromelaField(functionName);
            systemState.addField(Declaration(DataType(UtypeRef(dataType)), fieldName));
        }
    }

    systemState.addField(Declaration(DataType(UtypeRef("AggregateTimerData")), "timers"));

    context.model()->addUtype(systemState);

    context.model()->addDeclaration(Declaration(DataType(UtypeRef("system_state")), "global_state"));
}

void IvToPromelaTranslator::createPromelaObjectsForTimers(Context &context) const
{
    std::map<QString, VariableRef> timerSignals;

    QVector<IVFunction *> ivFunctionList = context.ivModel()->allObjectsByType<IVFunction>();

    for (IVFunction *ivFunction : ivFunctionList) {
        const QString functionName = ivFunction->property("name").toString();
        const auto &modelFunctions = context.modelFunctions();
        if (std::find(modelFunctions.begin(), modelFunctions.end(), functionName) != modelFunctions.end()) {
            const QVector<shared::ContextParameter> parameters = ivFunction->contextParams();
            for (const shared::ContextParameter &parameter : parameters) {
                if (parameter.paramType() == shared::BasicParameter::Type::Timer) {
                    const QString timerName = parameter.name().toLower();
                    const QString channelName = constructChannelName(functionName, timerName);
                    VariableRef timerData("global_state");
                    timerData.appendElement("timers");
                    timerData.appendElement(functionName.toLower());
                    timerData.appendElement(timerName.toLower());
                    createTimerInlinesForFunction(context, functionName, timerName, timerData);
                    timerSignals.emplace(channelName, timerData);
                }
            }
        }
    }

    if (!timerSignals.empty()) {
        createGlobalTimerObjects(context, timerSignals);
    }
}

void IvToPromelaTranslator::createTimerInlinesForFunction(
        Context &context, const QString &functionName, const QString &timerName, const VariableRef &timerData) const
{
    const QList<InlineCall::Argument> arguments;

    const QString setTimerName = QString("%1_0_%2_set").arg(Escaper::escapePromelaIV(functionName)).arg(timerName);
    const QString resetTimerName = QString("%1_0_%2_reset").arg(Escaper::escapePromelaIV(functionName)).arg(timerName);

    Sequence setTimerSequence(Sequence::Type::NORMAL);

    QString setTimerParameterName = QString("%1_%2_interval")
                                            .arg(Escaper::escapePromelaName(functionName))
                                            .arg(Escaper::escapePromelaName(timerName));

    VariableRef timerIntervalVar(timerData);
    timerIntervalVar.appendElement("interval");
    setTimerSequence.appendElement(Assignment(timerIntervalVar, Expression(VariableRef(setTimerParameterName))));

    VariableRef timerEnabledVar(timerData);
    timerEnabledVar.appendElement("timer_enabled");
    setTimerSequence.appendElement(Assignment(timerEnabledVar, Expression(BooleanConstant(true))));

    Sequence resetTimerSequence(Sequence::Type::NORMAL);

    resetTimerSequence.appendElement(Assignment(timerEnabledVar, Expression(BooleanConstant(false))));

    QList<QString> params;
    params.append(setTimerParameterName);

    context.model()->addInlineDef(
            std::make_unique<InlineDef>(setTimerName, std::move(params), std::move(setTimerSequence)));
    context.model()->addInlineDef(
            std::make_unique<InlineDef>(resetTimerName, QList<QString>(), std::move(resetTimerSequence)));
}

void IvToPromelaTranslator::createGlobalTimerObjects(
        Context &context, const std::map<QString, VariableRef> &timerSignals) const
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
    context.model()->addProctype(std::move(timerManagerProctype));
}

void IvToPromelaTranslator::createPromelaObjectsForObservers(Context &context) const
{
    const auto attachments =
            context.getObserverAttachments(IvToPromelaTranslator::ObserverAttachment::Kind::Kind_Output);

    for (const auto &attachment : attachments) {
        const QString toFunction = getAttachmentToFunction(context.ivModel(), attachment);
        const QString fromFunction = getAttachmentFromFunction(context.ivModel(), attachment);
        const QString channelName = observerChannelName(attachment, toFunction);

        const IVInterface *interface = findProvidedInterface(context.ivModel(), fromFunction, attachment.interface());
        size_t queueSize;
        QString parameterName;
        QString parameterType;
        if (interface != nullptr) {
            queueSize = getInterfaceQueueSize(interface);
            std::tie(parameterName, parameterType) = getInterfaceParameter(interface);
        } else {
            const QString timer = findTimerSignal(context.ivModel(), toFunction, attachment.interface());
            if (timer.isEmpty()) {
                const auto message = QString("Cannot find interface '%1::%2' while attaching observer '%3'")
                                             .arg(toFunction)
                                             .arg(attachment.interface())
                                             .arg(attachment.observer());
                throw TranslationException(message);
            }

            queueSize = 1;
        }

        QList<ChannelInit::Type> channelType;
        channelType.append(parameterType.isEmpty()
                        ? ChannelInit::Type(BasicType::INT)
                        : ChannelInit::Type(UtypeRef(Escaper::escapePromelaName(parameterType))));
        ChannelInit channelInit(queueSize, std::move(channelType));
        Declaration channelDeclaration = Declaration(DataType(BasicType::CHAN), channelName);
        channelDeclaration.setInit(std::move(channelInit));
        context.model()->addDeclaration(channelDeclaration);
        const QString inlineName = QString("%1_0_RI_0_%2")
                                           .arg(Escaper::escapePromelaIV(attachment.observer()))
                                           .arg(attachment.observerInterface());

        QList<QString> arguments;
        QList<Expression> params;
        if (!parameterType.isEmpty()) {
            const QString name = QString("%1_%2_%3").arg(attachment.observer()).arg(toFunction).arg(parameterName);
            arguments.append(name);
            params.append(Expression(VariableRef(name)));
        } else {
            params.append(Expression(Constant(1)));
        }
        Sequence sequence(Sequence::Type::NORMAL);

        sequence.appendElement(ChannelSend(VariableRef(channelName), params));

        context.model()->addInlineDef(std::make_unique<InlineDef>(inlineName, arguments, std::move(sequence)));
    }
}

QString IvToPromelaTranslator::constructChannelName(const QString &functionName, const QString &interfaceName) const
{
    return QString("%1_%2_channel").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);
}

std::vector<const Asn1Acn::Definitions *> IvToPromelaTranslator::getSubtypesDefinitions(
        const Asn1Model *asn1Model, const Options &options) const
{
    std::vector<const Asn1Acn::Definitions *> result;

    const auto subtypesFilepaths = options.values(PromelaOptions::subtypesFilepath);

    for (const auto &subtypesFilepath : subtypesFilepaths) {
        const auto &asn1ModelData = asn1Model->data();

        const auto foundAsn1File = std::find_if(asn1ModelData.begin(), asn1ModelData.end(),
                [&](const auto &file) { return file->name() == subtypesFilepath; });

        if (foundAsn1File == asn1ModelData.end()) {
            auto errorMessage = QString("Unable to find subtypes file '%1' in the ASN.1 model").arg(subtypesFilepath);
            throw TranslationException(std::move(errorMessage));
        }

        const auto &definitionsList = (*foundAsn1File)->definitionsList();
        std::for_each(definitionsList.begin(), definitionsList.end(),
                [&](const auto &definitions) { result.push_back(definitions.get()); });
    }

    return result;
}

QString IvToPromelaTranslator::getInterfaceName(const IVInterface *interface) const
{
    return interface->property("name").toString();
}

QString IvToPromelaTranslator::getInterfaceFunctionName(const IVInterface *interface) const
{
    return interface->function()->property("name").toString();
}

QVariant IvToPromelaTranslator::getInterfaceProperty(const IVInterface *interface, const QString &name) const
{
    const QVariantList attrs = interface->attributes();
    {
        for (const QVariant &attr : qAsConst(attrs)) {
            if (attr.canConvert<ExportableProperty>()) {
                ExportableProperty ep = attr.value<ExportableProperty>();
                if (ep.name() == name) {
                    return ep.value();
                }
            }
        }
    }

    return QVariant();
}

std::pair<QString, QString> IvToPromelaTranslator::getInterfaceParameter(const IVInterface *interface) const
{
    QVector<InterfaceParameter> parameterList = interface->params();
    if (parameterList.length() > 1) {
        // at most one parameter is allowed for sporadic interfaces
        throw TranslationException("Sporadic interface with more than one parameters is not allowed");
    }
    QString parameterName;
    QString parameterType;
    if (parameterList.length() == 1) {
        parameterName = parameterList.first().name();
        parameterType = parameterList.first().paramTypeName();
    }
    return std::make_pair(parameterName, parameterType);
}

size_t IvToPromelaTranslator::getInterfaceQueueSize(const IVInterface *interface) const
{
    QVariant property = getInterfaceProperty(interface, "queue_size");
    if (property.isValid()) {
        return property.toULongLong();
    }
    return 1;
}

size_t IvToPromelaTranslator::getInterfacePriority(const IVInterface *interface) const
{
    QVariant property = getInterfaceProperty(interface, "priority");
    if (property.isValid()) {
        return property.toULongLong();
    } else {
        return 1;
    }
}

QString IvToPromelaTranslator::handleParameterSubtype(Context &context, const QString &parameterTypeName,
        const QString &parameterName, const QString &interfaceName, const QString &functionName) const
{
    if (context.subtypesDefinitions().empty()) {
        return parameterTypeName;
    }

    auto parameterSubtypeName = buildParameterSubtypeName(functionName, interfaceName, parameterName);

    for (const auto definitions : context.subtypesDefinitions()) {
        const auto parameterSubtype = definitions->type(parameterSubtypeName);

        if (parameterSubtype != nullptr) {
            return parameterSubtypeName;
        }
    }

    return parameterTypeName;
}

QString IvToPromelaTranslator::handleSendInlineArgument(const QString &parameterType, const QString &functionName,
        const QString &interfaceName, const QString parameterName, Sequence &sequence) const
{
    if (parameterType.isEmpty()) {
        sequence.appendElement(Declaration(DataType(BasicType::INT), m_dummyParamName));

        return "";
    } else {
        return QString("%1_%2_%3").arg(functionName).arg(interfaceName).arg(parameterName);
    }
}

Expression IvToPromelaTranslator::handleSendInlineParameter(
        const QString &argumentName, const QString &parameterType, Sequence &sequence) const
{
    const auto parameterName = QString("%1_value").arg(argumentName);

    auto parameterDecl = Declaration(DataType(UtypeRef(Escaper::escapePromelaName(parameterType))), parameterName);
    sequence.appendElement(std::move(parameterDecl));

    const auto inlineCallName = QString("%1_assign_value").arg(Escaper::escapePromelaName(parameterType));
    QList<InlineCall::Argument> inlineArguments;
    inlineArguments.append(VariableRef(parameterName));
    inlineArguments.append(VariableRef(argumentName));
    auto inlineCall = InlineCall(inlineCallName, inlineArguments);
    sequence.appendElement(std::move(inlineCall));

    return Expression(VariableRef(parameterName));
}

bool IvToPromelaTranslator::isParameterSubtyped(Context &context, const QString &parameterType,
        const QString &parameterName, const QString &interfaceName, const QString &functionName) const
{
    const auto subtype = handleParameterSubtype(context, parameterType, parameterName, interfaceName, functionName);
    return parameterType != subtype;
}

QString IvToPromelaTranslator::buildParameterSubtypeName(
        const QString &functionName, const QString &interfaceName, const QString &parameterName) const
{
    auto subtypeName = QString("%1-%2-%3").arg(functionName).arg(interfaceName).arg(parameterName);
    subtypeName.replace('_', '-');
    subtypeName = subtypeName.toLower();
    subtypeName[0] = subtypeName[0].toUpper();

    return subtypeName;
}

const ::ivm::IVInterface *IvToPromelaTranslator::findProvidedInterface(
        const ::ivm::IVModel *model, const QString &fromFunction, const QString &interfaceName) const
{
    const IVInterface *ri = findRequiredInterface(model, fromFunction, interfaceName);
    const IVConnection *connection = model->getConnectionForIface(ri->id());
    if (connection != nullptr) {
        return connection->targetInterface();
    } else {
        return nullptr;
    }
}

const ::ivm::IVInterface *IvToPromelaTranslator::findRequiredInterface(
        const ::ivm::IVModel *model, const QString &functionName, const QString &interfaceName) const
{
    const IVFunction *function = model->getFunction(functionName, Qt::CaseInsensitive);
    if (function == nullptr) {
        return nullptr;
    }
    const QVector<IVInterface *> ris = function->ris();
    auto iter = std::find_if(ris.begin(), ris.end(), [&interfaceName, this](const IVInterface *i) {
        return interfaceName.compare(getInterfaceName(i), Qt::CaseInsensitive) == 0;
    });
    if (iter == ris.end()) {
        return nullptr;
    }
    return *iter;
}

QString IvToPromelaTranslator::findTimerSignal(
        const ::ivm::IVModel *model, const QString &functionName, const QString &signalName) const
{
    const IVFunction *function = model->getFunction(functionName, Qt::CaseInsensitive);
    if (function == nullptr) {
        return nullptr;
    }

    QString timerName = signalName;

    if (timerName.startsWith(QString("%1_").arg(functionName), Qt::CaseInsensitive)) {
        timerName.remove(QString("%1_").arg(functionName), Qt::CaseInsensitive);
    }

    const QVector<shared::ContextParameter> params = function->contextParams();

    for (const shared::ContextParameter &param : params) {
        if (param.paramType() == shared::ContextParameter::Type::Timer
                && param.name().compare(timerName, Qt::CaseInsensitive) == 0) {
            return param.name();
        }
    }

    return QString();
}

QString IvToPromelaTranslator::observerChannelName(
        const ObserverAttachment &attachment, const QString &toFunction) const
{
    return QString("%1_%2_%3_channel")
            .arg(Escaper::escapePromelaIV(toFunction))
            .arg(Escaper::escapePromelaName(attachment.observer()))
            .arg(attachment.interface());
}

QString IvToPromelaTranslator::getAttachmentToFunction(
        const ivm::IVModel *model, const ObserverAttachment &attachment) const
{
    if (attachment.toFunction().has_value()) {
        return attachment.toFunction().value();
    } else {
        const QString fromFunctionName = attachment.fromFunction().value();
        const IVInterface *ri = findRequiredInterface(model, fromFunctionName, attachment.interface());
        const IVConnection *conn = model->getConnectionForIface(ri->id());
        return conn->targetName();
    }
}

QString IvToPromelaTranslator::getAttachmentFromFunction(
        const ivm::IVModel *model, const ObserverAttachment &attachment) const
{
    Q_UNUSED(model);
    if (attachment.fromFunction().has_value()) {
        return attachment.fromFunction().value();
    } else {
        throw TranslationException("getAttachmentFromFunction is not implemented");
    }
}

QString IvToPromelaTranslator::getFunctionLockChannelName(const QString &functionName) const
{
    return QString("%1_lock").arg(Escaper::escapePromelaIV(functionName));
}

std::unique_ptr<ProctypeElement> IvToPromelaTranslator::createLockAcquireStatement(const QString &functionName) const
{
    const VariableRef lockChannelName = VariableRef(getFunctionLockChannelName(functionName));
    QList<VariableRef> lockChannelArguments;
    lockChannelArguments.append(VariableRef("_"));

    return std::make_unique<ProctypeElement>(ChannelRecv(lockChannelName, lockChannelArguments));
}

std::unique_ptr<ProctypeElement> IvToPromelaTranslator::createLockReleaseStatement(const QString &functionName) const
{
    const VariableRef lockChannelName = VariableRef(getFunctionLockChannelName(functionName));

    QList<Expression> unlockChannelArguments;
    unlockChannelArguments.append(Expression(Constant(1)));
    return std::make_unique<ProctypeElement>(ChannelSend(VariableRef(lockChannelName), unlockChannelArguments));
}

std::unique_ptr<ProctypeElement> IvToPromelaTranslator::createProcessInlineCall(
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

std::unique_ptr<model::ProctypeElement> IvToPromelaTranslator::createReceiveStatement(
        const QString &channelName, const QString &parameterType, const QString &parameterName) const
{
    // channel receive
    QList<VariableRef> receiveParams;
    receiveParams.append(VariableRef(parameterType.isEmpty() ? QString("_") : parameterName));
    return std::make_unique<ProctypeElement>(ChannelRecv(VariableRef(channelName), receiveParams));
}

IvToPromelaTranslator::ObserverAttachments IvToPromelaTranslator::getObserverAttachments(
        Context &context, const QString &function, const QString &interface, const ObserverAttachment::Kind kind) const
{
    ObserverAttachments allAttachments = context.getObserverAttachments(kind);
    ObserverAttachments result;

    if (kind == ObserverAttachment::Kind::Kind_Output) {
        for (const ObserverAttachment &attachment : allAttachments) {
            const QString fromFunction = getAttachmentFromFunction(context.ivModel(), attachment);
            const QString toFunction = getAttachmentToFunction(context.ivModel(), attachment);
            const IVInterface *i = findProvidedInterface(context.ivModel(), fromFunction, attachment.interface());
            const IVConnection *connection = context.ivModel()->getConnectionForIface(i->id());

            // check possible connection
            if (function.compare(connection->targetName(), Qt::CaseInsensitive) == 0
                    && interface.compare(connection->targetInterfaceName(), Qt::CaseInsensitive) == 0) {
                result.push_back(attachment);
            }
            // special case for the timers, when the interface name contains both function name and timer name
            else if (function.compare(toFunction, Qt::CaseInsensitive) == 0
                    && attachment.interface().compare(
                               QString("%1_%2").arg(function).arg(interface), Qt::CaseInsensitive)
                            == 0) {
                result.push_back(attachment);
            }
        }
    } else {
        for (const ObserverAttachment &attachment : allAttachments) {
            const QString fromFunction = getAttachmentFromFunction(context.ivModel(), attachment);
            if (function.compare(fromFunction, Qt::CaseInsensitive) == 0
                    && interface.compare(attachment.interface(), Qt::CaseInsensitive) == 0) {
                result.push_back(attachment);
            }
        }
    }

    return result;
}

}
