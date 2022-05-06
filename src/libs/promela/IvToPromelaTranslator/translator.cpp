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
using promela::model::Assignment;
using promela::model::BasicType;
using promela::model::BinaryExpression;
using promela::model::ChannelInit;
using promela::model::ChannelRecv;
using promela::model::ChannelSend;
using promela::model::Constant;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::DoLoop;
using promela::model::Expression;
using promela::model::InitProctype;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::Proctype;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::Sequence;
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
    const auto priorityPrefix = QString("");
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

IvToPromelaTranslator::Context::Context(::promela::model::PromelaModel *promelaModel)
{
    m_promelaModel = promelaModel;
}

void IvToPromelaTranslator::Context::addObserverAttachment(const IvToPromelaTranslator::ObserverAttachment &attachment)
{
    if (attachment.fromFunction().has_value()) {
        m_observerAttachments[*attachment.fromFunction()][attachment.interface()].push_back(attachment);
    }
    if (attachment.toFunction().has_value()) {
        m_observerAttachments[*attachment.toFunction()][attachment.interface()].push_back(attachment);
    }
}

auto IvToPromelaTranslator::Context::getObserverAttachments(const QString &function, const QString &interface,
        const ObserverAttachment::Kind kind) -> const IvToPromelaTranslator::ObserverAttachments
{
    ObserverAttachments result;
    // [] may create new keys, but performance penalty is negligible and code is more terse
    // The best way to implement this would be to use LINQ-like C++20 ranges
    for (const auto attachment : m_observerAttachments.at(function).at(interface)) {
        if (attachment.kind() == kind) {
            result.push_back(attachment);
        }
    }
    return result;
}

auto IvToPromelaTranslator::Context::hasObserverAttachments(
        const QString &function, const QString &interface, const ObserverAttachment::Kind kind) -> bool
{
    return getObserverAttachments(function, interface, kind).size() > 0;
}

auto IvToPromelaTranslator::Context::model() -> ::promela::model::PromelaModel *
{
    return m_promelaModel;
}

static auto addChannelAndLock(IvToPromelaTranslator::Context &context, const QString &functionName)
{
    QList<ChannelInit::Type> channelType;
    channelType.append(BasicType::INT);
    ChannelInit channelInit(1, std::move(channelType));
    Declaration channelDeclaration(
            DataType(BasicType::CHAN), QString("%1_lock").arg(Escaper::escapePromelaIV(functionName)));
    channelDeclaration.setInit(channelInit);
    context.model()->addDeclaration(channelDeclaration);
}

std::vector<std::unique_ptr<Model>> IvToPromelaTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    const std::vector<QString> additionalIncludes = options.values(PromelaOptions::additionalIncludes);
    const std::vector<QString> modelFunctions = options.values(PromelaOptions::modelFunctionName);
    const std::vector<QString> environmentFunctions = options.values(PromelaOptions::environmentFunctionName);
    const std::vector<QString> observerAttachmentInfos = options.values(PromelaOptions::observerAttachment);
    const std::vector<QString> observerNames = options.values(PromelaOptions::observerFunctionName);
    std::unique_ptr<PromelaModel> promelaModel = std::make_unique<PromelaModel>();

    Context context(promelaModel.get());

    for (const auto &info : observerAttachmentInfos) {
        qDebug() << "Attaching  " << info;
        context.addObserverAttachment(ObserverAttachment(info));
    }

    const auto *ivModel = getModel<IVModel>(sourceModels);

    QVector<IVFunction *> ivFunctionList = ivModel->allObjectsByType<IVFunction>();

    promelaModel->addInclude("dataview.pml");

    promelaModel->addDeclaration(Declaration(DataType(BasicType::INT), "inited"));

    for (IVFunction *ivFunction : ivFunctionList) {
        const QString functionName = ivFunction->property("name").toString();

        if (std::find(modelFunctions.begin(), modelFunctions.end(), functionName) != modelFunctions.end()) {
            promelaModel->addInclude(QString("%1.pml").arg(functionName.toLower()));
            createPromelaObjectsForFunction(context, ivModel, ivFunction, functionName);
        } else if (std::find(environmentFunctions.begin(), environmentFunctions.end(), functionName)
                != environmentFunctions.end()) {
            createPromelaObjectsForEnvironment(context, ivModel, ivFunction, functionName);
        }
    }

    for (const auto &observerName : observerNames) {
        promelaModel->addInclude(QString("%1.pml").arg(observerName));
    }

    promelaModel->addInclude("env_inlines.pml");

    createSystemState(promelaModel.get(), ivModel, modelFunctions, observerNames);

    for (const QString &function : modelFunctions) {
        addChannelAndLock(context, function);
    }

    for (const auto &observer : observerNames) {
        addChannelAndLock(context, observer);
    }

    promelaModel->setInit(generateInitProctype(modelFunctions, observerNames, ivModel));

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
    return std::set<ModelType> { ModelType::InterfaceView };
}

static void initializeFunction(Sequence &sequence, const QString &functionName)
{
    QString initFn = QString("%1_0_init").arg(Escaper::escapePromelaIV(functionName));
    std::unique_ptr<ProctypeElement> initCall = std::make_unique<ProctypeElement>(InlineCall(initFn, {}));
    sequence.appendElement(std::move(initCall));

    const VariableRef lockChannelName = VariableRef(QString("%1_lock").arg(Escaper::escapePromelaIV(functionName)));
    QList<VariableRef> lockChannelArguments;
    lockChannelArguments.append(VariableRef("init_token"));

    sequence.appendElement(
            std::make_unique<ProctypeElement>(ChannelSend(VariableRef(lockChannelName), lockChannelArguments)));
}

InitProctype IvToPromelaTranslator::generateInitProctype(
        const std::vector<QString> &modelFunctions, const std::vector<QString> &observers, const IVModel *ivModel) const
{
    QSet<QString> functionsWithContextVariables;

    QVector<IVFunction *> ivFunctionList = ivModel->allObjectsByType<IVFunction>();

    for (IVFunction *ivFunction : ivFunctionList) {
        if (containsContextVariables(ivFunction->contextParams())) {
            const QString functionName = ivFunction->property("name").toString();
            functionsWithContextVariables.insert(functionName);
        }
    }

    Sequence sequence(Sequence::Type::ATOMIC);

    {
        Declaration initTokenDeclaration = Declaration(DataType(BasicType::INT), "init_token");
        initTokenDeclaration.setInit(Expression(Constant(1)));
        std::unique_ptr<ProctypeElement> element = std::make_unique<ProctypeElement>(std::move(initTokenDeclaration));

        sequence.appendElement(std::move(element));
    }

    for (const QString &functionName : modelFunctions) {
        if (functionsWithContextVariables.contains(functionName)) {
            const QString ctxtInitInline =
                    QString("%1_ctxt_init").arg(Escaper::escapePromelaIV(functionName).toLower());
            sequence.appendElement(std::make_unique<ProctypeElement>(InlineCall(ctxtInitInline, {})));
        }
    }

    for (const QString &functionName : modelFunctions) {
        initializeFunction(sequence, functionName);
    }

    for (const QString &observer : observers) {
        initializeFunction(sequence, observer);
    }

    std::unique_ptr<ProctypeElement> setInited =
            std::make_unique<ProctypeElement>(Assignment(VariableRef("inited"), Expression(Constant(1))));
    sequence.appendElement(std::move(setInited));
    return InitProctype(std::move(sequence));
}

static QString observerInputSignalName(const IvToPromelaTranslator::ObserverAttachment &attachment)
{
    return QString("%1_0_PI_0_%2")
            .arg(Escaper::escapePromelaName(attachment.observer()))
            .arg(Escaper::escapePromelaName(attachment.observerInterface()));
}

void IvToPromelaTranslator::attachInputObservers(IvToPromelaTranslator::Context &context, const QString &functionName,
        const QString &interfaceName, const QString &parameterName, const QString &parameterType,
        Sequence *sequence) const
{
    auto attachments = context.getObserverAttachments(
            functionName, interfaceName, IvToPromelaTranslator::ObserverAttachment::Kind::Kind_Input);
    std::sort(attachments.begin(), attachments.end(),
            [](const auto &a, const auto &b) -> bool { return a.priority() > b.priority(); });
    for (const auto &attachment : attachments) {
        const VariableRef lockChannelName =
                VariableRef(QString("%1_lock").arg(Escaper::escapePromelaIV(attachment.observer())));
        QList<VariableRef> lockChannelArguments;
        lockChannelArguments.append(VariableRef("token"));
        sequence->appendElement(std::make_unique<ProctypeElement>(ChannelRecv(lockChannelName, lockChannelArguments)));

        QList<InlineCall::Argument> arguments;
        if (!parameterType.isEmpty()) {
            arguments.append(VariableRef(parameterName));
        }
        sequence->appendElement(
                std::make_unique<ProctypeElement>(InlineCall(observerInputSignalName(attachment), arguments)));

        sequence->appendElement(
                std::make_unique<ProctypeElement>(ChannelSend(VariableRef(lockChannelName), lockChannelArguments)));
    }
}

std::unique_ptr<Proctype> IvToPromelaTranslator::generateProctype(Context &context, const QString &functionName,
        const QString &interfaceName, const QString &parameterType, size_t queueSize, size_t priority,
        bool environment) const
{
    QString channelName = constructChannelName(functionName, interfaceName);
    QList<ChannelInit::Type> channelType;
    channelType.append(parameterType.isEmpty()
                    ? ChannelInit::Type(BasicType::INT)
                    : ChannelInit::Type(UtypeRef(Escaper::escapePromelaName(parameterType))));
    ChannelInit channelInit(queueSize, std::move(channelType));
    Declaration declaration(DataType(BasicType::CHAN), channelName);
    declaration.setInit(channelInit);
    context.model()->addDeclaration(declaration);

    Sequence sequence(Sequence::Type::NORMAL);
    std::unique_ptr<ProctypeElement> waitForInit = std::make_unique<ProctypeElement>(Expression(VariableRef("inited")));
    sequence.appendElement(std::move(waitForInit));

    const auto hasInputObservers = context.hasObserverAttachments(
            functionName, interfaceName, IvToPromelaTranslator::ObserverAttachment::Kind::Kind_Input);

    if (hasInputObservers || !environment) {
        std::unique_ptr<ProctypeElement> tokenDeclaration =
                std::make_unique<ProctypeElement>(Declaration(DataType(BasicType::INT), "token"));
        sequence.appendElement(std::move(tokenDeclaration));
    }

    const QString &signalParameterName = "signal_parameter";

    if (!parameterType.isEmpty()) {
        std::unique_ptr<ProctypeElement> variableDecl = std::make_unique<ProctypeElement>(
                Declaration(DataType(UtypeRef(Escaper::escapePromelaName(parameterType))), signalParameterName));
        sequence.appendElement(std::move(variableDecl));
    }

    std::unique_ptr<Sequence> loopSequence = std::make_unique<Sequence>(Sequence::Type::ATOMIC);

    QList<VariableRef> params;
    params.append(VariableRef(parameterType.isEmpty() ? QString("_") : signalParameterName));
    std::unique_ptr<ProctypeElement> channelReceive =
            std::make_unique<ProctypeElement>(ChannelRecv(VariableRef(channelName), params));

    loopSequence->appendElement(std::move(channelReceive));

    if (!environment) {
        const QString piName = QString("%1_0_PI_0_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);
        const VariableRef lockChannelName = VariableRef(QString("%1_lock").arg(Escaper::escapePromelaIV(functionName)));
        QList<VariableRef> lockChannelArguments;
        lockChannelArguments.append(VariableRef("token"));

        loopSequence->appendElement(
                std::make_unique<ProctypeElement>(ChannelRecv(lockChannelName, lockChannelArguments)));

        if (parameterType.isEmpty()) {
            loopSequence->appendElement(std::make_unique<ProctypeElement>(InlineCall(piName, {})));
        } else {
            QList<InlineCall::Argument> arguments;
            arguments.append(VariableRef(signalParameterName));
            loopSequence->appendElement(std::make_unique<ProctypeElement>(InlineCall(piName, arguments)));
        }

        loopSequence->appendElement(
                std::make_unique<ProctypeElement>(ChannelSend(VariableRef(lockChannelName), lockChannelArguments)));
    }

    // Observers can be also attached to environment
    attachInputObservers(context, functionName, interfaceName, signalParameterName, parameterType, loopSequence.get());

    DoLoop loop;

    loop.appendSequence(std::move(loopSequence));

    std::unique_ptr<ProctypeElement> loopElement = std::make_unique<ProctypeElement>(std::move(loop));

    sequence.appendElement(std::move(loopElement));

    const QString proctypeName = QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);
    std::unique_ptr<Proctype> proctype = std::make_unique<Proctype>(proctypeName, std::move(sequence));

    proctype->setActive(true);
    proctype->setInstancesCount(1);
    proctype->setPriority(priority);

    return proctype;
}

std::unique_ptr<Proctype> IvToPromelaTranslator::generateEnvironmentProctype(const QString &functionName,
        const QString &interfaceName, const QString &parameterType, const QString &sendInline) const
{
    Sequence sequence(Sequence::Type::NORMAL);
    std::unique_ptr<ProctypeElement> waitForInit = std::make_unique<ProctypeElement>(Expression(VariableRef("inited")));
    sequence.appendElement(std::move(waitForInit));

    if (!parameterType.isEmpty()) {
        sequence.appendElement(std::make_unique<ProctypeElement>(
                Declaration(DataType(UtypeRef(Escaper::escapePromelaName(parameterType))), "value")));
    }

    DoLoop loop;

    std::unique_ptr<Sequence> loopSequence = std::make_unique<Sequence>(Sequence::Type::ATOMIC);

    if (parameterType.isEmpty()) {
        std::unique_ptr<ProctypeElement> inlineCall = std::make_unique<ProctypeElement>(InlineCall(sendInline, {}));

        loopSequence->appendElement(std::move(inlineCall));
    } else {
        const QString generateValueInlineName =
                QString("%1_generate_value").arg(Escaper::escapePromelaName(parameterType));
        QList<InlineCall::Argument> arguments;
        arguments.append(VariableRef("value"));
        std::unique_ptr<ProctypeElement> valueGeneration =
                std::make_unique<ProctypeElement>(InlineCall(generateValueInlineName, arguments));
        std::unique_ptr<ProctypeElement> inlineCall =
                std::make_unique<ProctypeElement>(InlineCall(sendInline, arguments));

        loopSequence->appendElement(std::move(valueGeneration));
        loopSequence->appendElement(std::move(inlineCall));
    }

    loop.appendSequence(std::move(loopSequence));

    std::unique_ptr<ProctypeElement> loopElement = std::make_unique<ProctypeElement>(std::move(loop));

    sequence.appendElement(std::move(loopElement));

    const QString proctypeName = QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

    std::unique_ptr<Proctype> proctype = std::make_unique<Proctype>(proctypeName, std::move(sequence));

    proctype->setActive(true);
    proctype->setInstancesCount(1);

    return proctype;
}

std::unique_ptr<::promela::model::InlineDef> IvToPromelaTranslator::generateSendInline(const QString &functionName,
        const QString &interfaceName, const QString &parameterName, const QString &parameterType,
        const QString &sourceFunctionName, const QString &sourceInterfaceName) const
{
    QString inlineName =
            QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(sourceFunctionName)).arg(sourceInterfaceName);
    QString channelName = constructChannelName(functionName, interfaceName);

    QList<QString> arguments;
    QString argumentName;
    Sequence sequence(Sequence::Type::NORMAL);

    if (parameterType.isEmpty()) {
        argumentName = "dummy";
        std::unique_ptr<ProctypeElement> dummyVariableDef =
                std::make_unique<ProctypeElement>(Declaration(DataType(BasicType::INT), argumentName));
        sequence.appendElement(std::move(dummyVariableDef));
    } else {
        argumentName = QString("%1_%2_%3").arg(functionName).arg(interfaceName).arg(parameterName);
        arguments.append(argumentName);
    }

    QList<VariableRef> params;
    params.append(VariableRef(argumentName));
    std::unique_ptr<ProctypeElement> channelSend =
            std::make_unique<ProctypeElement>(ChannelSend(VariableRef(channelName), params));

    sequence.appendElement(std::move(channelSend));

    return std::make_unique<InlineDef>(inlineName, arguments, std::move(sequence));
}

void IvToPromelaTranslator::createPromelaObjectsForFunction(IvToPromelaTranslator::Context &context,
        const IVModel *ivModel, IVFunction *ivFunction, const QString &functionName) const
{
    QVector<IVInterface *> providedInterfaceList = ivFunction->pis();

    QList<QString> channelNames;

    for (IVInterface *providedInterface : providedInterfaceList) {

        if (providedInterface->kind() != IVInterface::OperationKind::Cyclic
                && providedInterface->kind() != IVInterface::OperationKind::Sporadic) {
            auto message =
                    QString("Unallowed interface kind in function %1, only sporatic and cyclic interfaces are allowed")
                            .arg(functionName);
            throw TranslationException(message);
        }

        IVConnection *connection = ivModel->getConnectionForIface(providedInterface->id());

        IVInterface *requiredInterface = connection->sourceInterface();
        const QString sourceInterfaceName = getInterfaceName(requiredInterface);
        const QString sourceFunctionName = getInterfaceFunctionName(requiredInterface);

        const QString interfaceName = getInterfaceName(providedInterface);
        const size_t queueSize = getInterfaceQueueSize(providedInterface);
        const size_t priority = getInterfacePriority(providedInterface);
        const std::pair<QString, QString> parameter = getInterfaceParameter(providedInterface);

        const QString parameterName = parameter.first;
        const QString parameterType = parameter.second;

        context.model()->addInlineDef(generateSendInline(
                functionName, interfaceName, parameterName, parameterType, sourceFunctionName, sourceInterfaceName));

        context.model()->addProctype(
                generateProctype(context, functionName, interfaceName, parameterType, queueSize, priority, false));

        channelNames.append(constructChannelName(functionName, interfaceName));
    }

    createCheckQueueInline(context.model(), functionName, channelNames);
}

void IvToPromelaTranslator::createPromelaObjectsForEnvironment(IvToPromelaTranslator::Context &context,
        const IVModel *ivModel, IVFunction *ivFunction, const QString &functionName) const
{
    QVector<IVInterface *> providedInterfaceList = ivFunction->pis();
    for (IVInterface *providedInterface : providedInterfaceList) {

        if (providedInterface->kind() != IVInterface::OperationKind::Sporadic) {
            auto message = QString("Unallowed interface kind in function %1, only sporatic interfaces are allowed")
                                   .arg(functionName);
            throw TranslationException(message);
        }

        IVConnection *connection = ivModel->getConnectionForIface(providedInterface->id());

        IVInterface *requiredInterface = connection->sourceInterface();
        const QString sourceInterfaceName = getInterfaceName(requiredInterface);
        const QString sourceFunctionName = getInterfaceFunctionName(requiredInterface);

        const QString interfaceName = getInterfaceName(providedInterface);
        const std::pair<QString, QString> parameter = getInterfaceParameter(providedInterface);

        const QString parameterName = parameter.first;
        const QString parameterType = parameter.second;

        context.model()->addInlineDef(generateSendInline(
                functionName, interfaceName, parameterName, parameterType, sourceFunctionName, sourceInterfaceName));

        const size_t queueSize = getInterfaceQueueSize(providedInterface);
        const size_t priority = getInterfacePriority(providedInterface);

        context.model()->addProctype(
                generateProctype(context, functionName, interfaceName, parameterType, queueSize, priority, true));
    }

    QVector<IVInterface *> requiredInterfaceList = ivFunction->ris();
    for (IVInterface *requiredInterface : requiredInterfaceList) {
        if (requiredInterface->kind() != IVInterface::OperationKind::Sporadic) {
            auto message = QString("Unallowed interface kind in function %1, only sporatic interfaces are allowed")
                                   .arg(functionName);
            throw TranslationException(message);
        }

        const QString interfaceName = getInterfaceName(requiredInterface);

        const std::pair<QString, QString> parameter = getInterfaceParameter(requiredInterface);

        const QString parameterName = parameter.first;
        const QString parameterType = parameter.second;

        const QString sendInline =
                QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

        context.model()->addProctype(
                generateEnvironmentProctype(functionName, interfaceName, parameterType, sendInline));
    }
}

void IvToPromelaTranslator::createCheckQueueInline(
        PromelaModel *promelaModel, const QString &functionName, QList<QString> &channelNames) const
{
    if (channelNames.empty()) {
        auto message = QString("No sporadic nor cyclic interfaces in function %1").arg(functionName);
        throw TranslationException(message);
    }

    Sequence sequence(Sequence::Type::ATOMIC);

    std::unique_ptr<Expression> expr;
    {
        QList<InlineCall::Argument> arguments;
        arguments.append(VariableRef(channelNames.last()));

        expr = std::make_unique<Expression>(InlineCall("empty", arguments));

        channelNames.pop_back();
    }

    while (!channelNames.empty()) {
        QList<InlineCall::Argument> arguments;
        arguments.append(VariableRef(channelNames.last()));

        std::unique_ptr<Expression> left = std::make_unique<Expression>(InlineCall("empty", arguments));

        expr = std::make_unique<Expression>(
                BinaryExpression(BinaryExpression::Operator::AND, std::move(left), std::move(expr)));

        channelNames.pop_back();
    }

    sequence.appendElement(std::make_unique<ProctypeElement>(*expr));

    const QString checkQueueInlineName = QString("%1_check_queue").arg(Escaper::escapePromelaIV(functionName));
    promelaModel->addInlineDef(
            std::make_unique<InlineDef>(checkQueueInlineName, QList<QString>(), std::move(sequence)));
}

void IvToPromelaTranslator::createSystemState(PromelaModel *promelaModel, const IVModel *ivModel,
        const std::vector<QString> &modelFunctions, const std::vector<QString> &observers) const
{
    QVector<IVFunction *> ivFunctionList = ivModel->allObjectsByType<IVFunction>();

    Utype systemState("system_state");

    for (const auto &observer : observers) {
        QString dataType = QString("%1_Context").arg(Escaper::escapePromelaIV(observer));
        QString fieldName = Escaper::escapePromelaField(observer);
        systemState.addField(Declaration(DataType(UtypeRef(dataType)), fieldName));
    }

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

    promelaModel->addUtype(systemState);

    promelaModel->addDeclaration(Declaration(DataType(UtypeRef("system_state")), "global_state"));
}

bool IvToPromelaTranslator::containsContextVariables(const QVector<shared::ContextParameter> &parameters) const
{
    return std::any_of(parameters.begin(), parameters.end(), [](const shared::ContextParameter &param) {
        return param.paramType() == shared::BasicParameter::Type::Other;
    });
}

QString IvToPromelaTranslator::constructChannelName(const QString &functionName, const QString &interfaceName) const
{
    return QString("%1_%2_channel").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);
}

QString IvToPromelaTranslator::getInterfaceName(const ivm::IVInterface *interface) const
{
    return interface->property("name").toString();
}

QString IvToPromelaTranslator::getInterfaceFunctionName(const ivm::IVInterface *interface) const
{
    return interface->function()->property("name").toString();
}

QVariant IvToPromelaTranslator::getInterfaceProperty(ivm::IVInterface *interface, const QString &name) const
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

std::pair<QString, QString> IvToPromelaTranslator::getInterfaceParameter(const ivm::IVInterface *interface) const
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

size_t IvToPromelaTranslator::getInterfaceQueueSize(IVInterface *interface) const
{
    QVariant property = getInterfaceProperty(interface, "queue_size");
    if (property.isValid()) {
        return property.toULongLong();
    }
    return 1;
}

size_t IvToPromelaTranslator::getInterfacePriority(IVInterface *interface) const
{
    QVariant property = getInterfaceProperty(interface, "priority");
    if (property.isValid()) {
        return property.toULongLong();
    }
    return 1;
}
}
