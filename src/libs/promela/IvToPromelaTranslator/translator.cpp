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

#include "ivtopromelagenerator.h"

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
using promela::model::PromelaSystemModel;
using shared::ExportableProperty;
using shared::InterfaceParameter;
using shared::VEObject;

namespace promela::translator {

namespace {
void showPossibleSenders(const QMap<QString, QString> &data)
{
    qDebug() << "        possibleSenders ";
    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        qDebug() << "            " << iter.key() << " " << iter.value();
    }
}

void showObservers(const std::list<std::unique_ptr<ObserverInfo>> &data)
{
    qDebug() << "        observers ";
    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        qDebug() << "            observerName " << (*iter)->m_observerName;
        qDebug() << "            observerInterface " << (*iter)->m_observerInterface;
        qDebug() << "            observerQueue " << (*iter)->m_observerQueue;
    }
}

void showProctype(const QString &name, const ProctypeInfo &info, bool isEnv)
{
    if (isEnv) {
        qDebug() << "    Environment proctype " << name;
    } else {
        qDebug() << "    Proctype " << name;
    }
    qDebug() << "        interfaceName " << info.m_interfaceName;
    qDebug() << "        queueName " << info.m_queueName;
    qDebug() << "        queueSize " << info.m_queueSize;
    qDebug() << "        priority " << info.m_priority;
    if (!info.m_parameterTypeName.isEmpty()) {
        qDebug() << "        parameterName " << info.m_parameterName;
        qDebug() << "        parameterType " << info.m_parameterTypeName;
    }
    qDebug() << "        isTimer " << info.m_isTimer;
    showPossibleSenders(info.m_possibleSenders);
    showObservers(info.m_observers);
}

void showEnvironmentProctype(const QString &name, const EnvProctypeInfo &info)
{
    qDebug() << "    Value generation proctype " << name;
    qDebug() << "        interfaceName " << info.m_interfaceName;
    qDebug() << "        priority " << info.m_priority;
}

void showSynchronousCallInfo(const QString &name, const SynchronousCallInfo &info)
{
    qDebug() << "    Synchronous call " << name;
    qDebug() << "        target functions";
    for (const auto &target : info.m_targets) {
        qDebug() << "            " << target.m_targetFunctionName << " " << target.m_providedInlineName << " (env "
                 << target.m_isEnvironment << ")";
    }
    qDebug() << "        protected " << info.m_isProtected;
    for (const auto &param : info.m_parameters) {
        qDebug() << "            " << param.m_parameterName << " " << param.m_parameterType << "  (output "
                 << param.m_isOutput << ")";
    }
}
}

std::vector<std::unique_ptr<Model>> IvToPromelaTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    const auto &modelFunctions = options.values(PromelaOptions::modelFunctionName);
    const auto &observerAttachmentInfos = options.values(PromelaOptions::observerAttachment);
    const auto &observerNames = options.values(PromelaOptions::observerFunctionName);
    auto promelaModel = std::make_unique<PromelaSystemModel>();
    const auto *ivModel = getModel<IVModel>(sourceModels);
    const auto *asn1Model = getModel<Asn1Model>(sourceModels);
    const auto asn1SubtypesDefinitions = getSubtypesDefinitions(asn1Model, options);

    IvToPromelaTranslatorContext context(
            promelaModel.get(), ivModel, options, asn1SubtypesDefinitions, modelFunctions, observerNames);

    for (const auto &info : observerAttachmentInfos) {
        context.addObserverAttachment(ObserverAttachment(info));
    }

    if (options.isSet(PromelaOptions::processesBasePriority)) {
        context.setBaseProctypePriority(options.value(PromelaOptions::processesBasePriority)->toUInt());
    }

    std::unique_ptr<SystemInfo> systemInfo = prepareSystemInfo(ivModel, options);

    qDebug() << "SystemInfo";
    for (auto iter = systemInfo->m_functions.begin(); iter != systemInfo->m_functions.end(); ++iter) {
        qDebug() << "Function " << iter->first << " env " << iter->second->m_isEnvironment;
        for (auto proctypeIter = iter->second->m_proctypes.begin(); proctypeIter != iter->second->m_proctypes.end();
                ++proctypeIter) {
            showProctype(proctypeIter->first, *proctypeIter->second, false);
        }
        for (auto proctypeIter = iter->second->m_environmentSourceProctypes.begin();
                proctypeIter != iter->second->m_environmentSourceProctypes.end(); ++proctypeIter) {
            showEnvironmentProctype(proctypeIter->first, *proctypeIter->second);
        }
        for (auto proctypeIter = iter->second->m_environmentSinkProctypes.begin();
                proctypeIter != iter->second->m_environmentSinkProctypes.end(); ++proctypeIter) {
            showProctype(proctypeIter->first, *proctypeIter->second, true);
        }
        for (auto synchronousCallIter = iter->second->m_synchronousCalls.begin();
                synchronousCallIter != iter->second->m_synchronousCalls.end(); ++synchronousCallIter) {
            showSynchronousCallInfo(synchronousCallIter->first, *synchronousCallIter->second);
        }
    }

    IvToPromelaGenerator generator(context, *systemInfo);
    generator.generate();

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
    return ModelType::PromelaSystem;
}

std::set<ModelType> IvToPromelaTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::InterfaceView, ModelType::Asn1 };
}

std::unique_ptr<SystemInfo> IvToPromelaTranslator::prepareSystemInfo(
        const ivm::IVModel *model, const conversion::Options &options) const
{
    std::vector<const Asn1Acn::Definitions *> asn1Definitions;

    const auto &modelFunctions = options.values(PromelaOptions::modelFunctionName);
    const auto &environmentFunctions = options.values(PromelaOptions::environmentFunctionName);
    const auto &observerAttachmentInfos = options.values(PromelaOptions::observerAttachment);
    const auto &observerNames = options.values(PromelaOptions::observerFunctionName);

    IvToPromelaTranslatorContext context(nullptr, model, options, asn1Definitions, modelFunctions, observerNames);

    if (options.isSet(PromelaOptions::processesBasePriority)) {
        context.setBaseProctypePriority(options.value(PromelaOptions::processesBasePriority)->toUInt());
    }
    for (const QString &info : observerAttachmentInfos) {
        context.addObserverAttachment(ObserverAttachment(info));
    }

    std::unique_ptr<SystemInfo> result = std::make_unique<SystemInfo>();

    const QVector<IVFunction *> ivFunctionList = model->allObjectsByType<IVFunction>();

    for (const IVFunction *ivFunction : ivFunctionList) {
        const QString functionName = ivFunction->property("name").toString();
        if (std::find(modelFunctions.begin(), modelFunctions.end(), functionName) != modelFunctions.end()) {
            std::unique_ptr<FunctionInfo> functionInfo = std::make_unique<FunctionInfo>();
            functionInfo->m_isEnvironment = false;
            prepareFunctionInfo(context, ivFunction, functionName, *functionInfo);
            result->m_functions.emplace(functionName, std::move(functionInfo));
        } else if (std::find(environmentFunctions.begin(), environmentFunctions.end(), functionName)
                != environmentFunctions.end()) {
            std::unique_ptr<FunctionInfo> functionInfo = std::make_unique<FunctionInfo>();
            functionInfo->m_isEnvironment = true;
            prepareEnvironmentFunctionInfo(context, ivFunction, functionName, *functionInfo);
            result->m_functions.emplace(functionName, std::move(functionInfo));
        }
    }

    result->m_observers = std::set<QString>(observerNames.begin(), observerNames.end());

    return result;
}

QString IvToPromelaTranslator::constructChannelName(const QString &functionName, const QString &interfaceName) const
{
    return QString("%1_%2_channel").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName.toLower());
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

const ::ivm::IVInterface *IvToPromelaTranslator::findProvidedInterface(
        const ::ivm::IVModel *model, const QString &fromFunction, const QString &interfaceName) const
{
    const IVInterface *ri = findRequiredInterface(model, fromFunction, interfaceName);
    if (ri == nullptr) {
        return nullptr;
    }
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

ObserverAttachments IvToPromelaTranslator::getObserverAttachments(IvToPromelaTranslatorContext &context,
        const QString &function, const QString &interface, const ObserverAttachment::Kind kind) const
{
    ObserverAttachments allAttachments = context.getObserverAttachments(kind);
    ObserverAttachments result;

    if (kind == ObserverAttachment::Kind::Kind_Output) {
        for (const ObserverAttachment &attachment : allAttachments) {
            const QString fromFunction = getAttachmentFromFunction(context.ivModel(), attachment);
            const QString toFunction = getAttachmentToFunction(context.ivModel(), attachment);
            const IVInterface *i = findProvidedInterface(context.ivModel(), fromFunction, attachment.interface());
            const IVConnection *connection = i == nullptr ? nullptr : context.ivModel()->getConnectionForIface(i->id());

            // check possible connection
            if (connection != nullptr && function.compare(connection->targetName(), Qt::CaseInsensitive) == 0
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
    } else if (kind == ObserverAttachment::Kind::Kind_Input) {
        for (const ObserverAttachment &attachment : allAttachments) {
            const QString fromFunction = getAttachmentFromFunction(context.ivModel(), attachment);
            if (function.compare(fromFunction, Qt::CaseInsensitive) == 0
                    && interface.compare(attachment.interface(), Qt::CaseInsensitive) == 0) {
                result.push_back(attachment);
            }
        }
    }

    std::sort(result.begin(), result.end(),
            [](const auto &a, const auto &b) -> bool { return a.priority() > b.priority(); });

    return result;
}

void IvToPromelaTranslator::prepareFunctionInfo(IvToPromelaTranslatorContext &context,
        const ::ivm::IVFunction *ivFunction, const QString &functionName, FunctionInfo &functionInfo) const
{
    if (ivFunction->instanceOf() != nullptr) {
        const QString functionTypeName = ivFunction->instanceOf()->property("name").toString();
        functionInfo.m_functionType = functionTypeName;
    }
    for (const IVInterface *providedInterface : ivFunction->pis()) {
        if (providedInterface->kind() == IVInterface::OperationKind::Sporadic) {
            std::unique_ptr<ProctypeInfo> proctypeInfo = prepareProctypeInfo(context, providedInterface, functionName);
            const QString proctypeName = proctypeInfo->m_proctypeName;
            functionInfo.m_proctypes.emplace(proctypeName, std::move(proctypeInfo));
        } else if (providedInterface->kind() == IVInterface::OperationKind::Protected
                || providedInterface->kind() == IVInterface::OperationKind::Unprotected) {
            // skip
        } else {
            auto message = QString("Unallowed interface kind in function %1, only sporadic, protected and unprotected "
                                   "interfaces are allowed")
                                   .arg(functionName);
            throw TranslationException(message);
        }
    }
    for (const IVInterface *requiredInterface : ivFunction->ris()) {
        if (requiredInterface->kind() == IVInterface::OperationKind::Sporadic) {
            // skip
        } else if (requiredInterface->kind() == IVInterface::OperationKind::Protected
                || requiredInterface->kind() == IVInterface::OperationKind::Unprotected) {
            prepareSynchronousCallInfo(context, functionName, requiredInterface, functionInfo);
        } else {
            auto message = QString("Unallowed interface kind in function %1, only sporadic, protected and unprotected "
                                   "interfaces are allowed")
                                   .arg(functionName);
            throw TranslationException(message);
        }
    }
    const QVector<shared::ContextParameter> parameters = ivFunction->contextParams();

    for (const shared::ContextParameter &parameter : parameters) {
        if (parameter.paramType() == shared::BasicParameter::Type::Timer) {
            const QString interfaceName = parameter.name().toLower();
            const QString proctypeName =
                    QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);
            const size_t priority = 1;
            const size_t queueSize = 1;
            const QString parameterType = QString();
            const QString parameterName = QString();
            const ObserverAttachments outputObservers =
                    getObserverAttachments(context, functionName, interfaceName, ObserverAttachment::Kind::Kind_Output);
            std::unique_ptr<ProctypeInfo> proctypeInfo = std::make_unique<ProctypeInfo>();

            proctypeInfo->m_proctypeName = proctypeName;
            proctypeInfo->m_interfaceName = interfaceName;
            proctypeInfo->m_queueSize = queueSize;
            proctypeInfo->m_priority = priority;
            proctypeInfo->m_parameterTypeName = parameterType;
            proctypeInfo->m_parameterName = parameterName;
            proctypeInfo->m_isTimer = true;

            QString currentQueueName = constructChannelName(functionName, interfaceName);

            for (const ObserverAttachment &attachment : outputObservers) {
                const QString toFunction = getAttachmentToFunction(context.ivModel(), attachment);
                const QString channelName = observerChannelName(attachment, toFunction);

                std::unique_ptr<ObserverInfo> observerInfo = std::make_unique<ObserverInfo>();

                observerInfo->m_observerName = attachment.observer();
                observerInfo->m_observerInterface = attachment.observerInterface();
                observerInfo->m_observerQueue = currentQueueName;
                observerInfo->m_nextQueue = channelName;
                observerInfo->m_toFunction = toFunction;

                proctypeInfo->m_observers.push_back(std::move(observerInfo));

                currentQueueName = channelName;
            }

            proctypeInfo->m_queueName = currentQueueName;
            functionInfo.m_proctypes.emplace(proctypeName, std::move(proctypeInfo));
        }
    }
}

void IvToPromelaTranslator::prepareEnvironmentFunctionInfo(IvToPromelaTranslatorContext &context,
        const ::ivm::IVFunction *ivFunction, const QString &functionName, FunctionInfo &functionInfo) const
{
    for (const IVInterface *providedInterface : ivFunction->pis()) {
        if (providedInterface->kind() == IVInterface::OperationKind::Sporadic) {
            std::unique_ptr<ProctypeInfo> proctypeInfo = prepareProctypeInfo(context, providedInterface, functionName);
            const QString proctypeName = proctypeInfo->m_proctypeName;
            functionInfo.m_environmentSinkProctypes.emplace(proctypeName, std::move(proctypeInfo));
        }
    }
    for (const IVInterface *requiredInterface : ivFunction->ris()) {
        if (requiredInterface->kind() == IVInterface::OperationKind::Sporadic) {
            std::unique_ptr<EnvProctypeInfo> proctypeInfo =
                    prepareEnvProctypeInfo(context, requiredInterface, functionName);
            const QString proctypeName = proctypeInfo->m_proctypeName;
            functionInfo.m_environmentSourceProctypes.emplace(proctypeName, std::move(proctypeInfo));
        } else if (requiredInterface->kind() == IVInterface::OperationKind::Protected
                || requiredInterface->kind() == IVInterface::OperationKind::Unprotected) {
            // skip
        } else {
            auto message = QString("Unallowed interface kind in function %1, only sporadic, protected and unprotected "
                                   "required interfaces are allowed")
                                   .arg(functionName);
            throw TranslationException(message);
        }
    }
}

std::unique_ptr<ProctypeInfo> IvToPromelaTranslator::prepareProctypeInfo(IvToPromelaTranslatorContext &context,
        const ivm::IVInterface *providedInterface, const QString &functionName) const
{
    const QString interfaceName = getInterfaceName(providedInterface);
    const QString proctypeName = QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

    const size_t priority = getInterfacePriority(providedInterface) + context.getBaseProctypePriority();
    const size_t queueSize = getInterfaceQueueSize(providedInterface);
    const auto [parameterName, parameterType] = getInterfaceParameter(providedInterface);

    const ObserverAttachments outputObservers =
            getObserverAttachments(context, functionName, interfaceName, ObserverAttachment::Kind::Kind_Output);

    std::unique_ptr<ProctypeInfo> proctypeInfo = std::make_unique<ProctypeInfo>();

    const QVector<IVConnection *> connections = context.ivModel()->getConnectionsForIface(providedInterface->id());

    if (connections.empty()) {
        auto message = QString("No connections to interface %1 in function %2")
                               .arg(getInterfaceName(providedInterface))
                               .arg(functionName);
        throw TranslationException(message);
    }

    if (!context.isMulticastSupported() && connections.size() > 1) {
        auto message = QString("Support for multicast is disabled.");
        throw TranslationException(message);
    }

    for (const IVConnection *connection : connections) {
        const IVInterface *requiredInterface = connection->sourceInterface();
        const QString sourceFunctionName = requiredInterface->function()->property("name").toString();
        const QString sourceInterfaceName = getInterfaceName(requiredInterface);
        proctypeInfo->m_possibleSenders.insert(sourceFunctionName, sourceInterfaceName);
    }

    proctypeInfo->m_proctypeName = proctypeName;
    proctypeInfo->m_interfaceName = interfaceName;
    proctypeInfo->m_queueSize = queueSize;
    proctypeInfo->m_priority = priority;
    proctypeInfo->m_parameterTypeName = parameterType;
    proctypeInfo->m_parameterName = parameterName;
    proctypeInfo->m_isTimer = false;

    QString currentQueueName = constructChannelName(functionName, interfaceName);
    for (auto iter = outputObservers.begin(); iter != outputObservers.end(); ++iter) {
        const ObserverAttachment &attachment = *iter;
        const QString toFunction = getAttachmentToFunction(context.ivModel(), attachment);
        const QString fromFunction = getAttachmentFromFunction(context.ivModel(), attachment);
        const QString &channelName = observerChannelName(attachment, toFunction);

        std::unique_ptr<ObserverInfo> observerInfo = std::make_unique<ObserverInfo>();

        observerInfo->m_observerName = attachment.observer();
        observerInfo->m_observerInterface = attachment.observerInterface();
        observerInfo->m_observerQueue = currentQueueName;
        observerInfo->m_fromFunction = fromFunction;
        observerInfo->m_toFunction = toFunction;
        observerInfo->m_nextQueue = channelName;
        proctypeInfo->m_observers.push_back(std::move(observerInfo));

        currentQueueName = channelName;
    }

    proctypeInfo->m_queueName = currentQueueName;

    return proctypeInfo;
}

std::unique_ptr<EnvProctypeInfo> IvToPromelaTranslator::prepareEnvProctypeInfo(IvToPromelaTranslatorContext &context,
        const ivm::IVInterface *requiredInterface, const QString &functionName) const
{
    Q_UNUSED(context);
    const QString interfaceName = getInterfaceName(requiredInterface);

    const QString proctypeName = QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

    std::unique_ptr<EnvProctypeInfo> proctypeInfo = std::make_unique<EnvProctypeInfo>();
    proctypeInfo->m_proctypeName = proctypeName;
    proctypeInfo->m_interfaceName = interfaceName;
    const auto [parameterType, parameterName] = getInterfaceParameter(requiredInterface);
    proctypeInfo->m_parameterType = parameterType;
    proctypeInfo->m_parameterName = parameterName;
    proctypeInfo->m_sendInlineName =
            QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

    return proctypeInfo;
}

void IvToPromelaTranslator::prepareSynchronousCallInfo(IvToPromelaTranslatorContext &context,
        const QString &functionName, const ivm::IVInterface *requiredInterface, FunctionInfo &functionInfo) const
{
    std::unique_ptr<SynchronousCallInfo> info = std::make_unique<SynchronousCallInfo>();
    QString inlineName =
            QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(functionName)).arg(requiredInterface->title());
    info->m_name = inlineName;
    info->m_interfaceName = getInterfaceName(requiredInterface);

    const QString parameterNamePrefix =
            QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(requiredInterface->title());
    for (const InterfaceParameter &interfaceParam : requiredInterface->params()) {
        SynchronousCallInfo::ParameterInfo parameterInfo;
        parameterInfo.m_parameterType = interfaceParam.paramTypeName();
        parameterInfo.m_parameterName = QString("%1_%2").arg(parameterNamePrefix).arg(interfaceParam.name());

        parameterInfo.m_isOutput = interfaceParam.isOutDirection();

        info->m_parameters.append(parameterInfo);
    }

    info->m_isProtected = requiredInterface->kind() == IVInterface::OperationKind::Protected;

    const QVector<IVConnection *> connections = context.ivModel()->getConnectionsForIface(requiredInterface->id());

    if (connections.empty()) {
        auto message = QString("No connections from interface %1 in function %2")
                               .arg(getInterfaceName(requiredInterface))
                               .arg(functionName);
        throw TranslationException(message);
    }

    if (!context.isMulticastSupported() && connections.size() > 1) {
        auto message = QString("Support for multicast is disabled.");
        throw TranslationException(message);
    }

    for (const IVConnection *connection : connections) {
        const IVInterface *sourceInterface = connection->targetInterface();
        const QString targetFunctionName = getInterfaceFunctionName(sourceInterface);

        SynchronousCallInfo::TargetInfo targetInfo;
        targetInfo.m_targetFunctionName = targetFunctionName;

        if (std::find_if(context.modelFunctions().begin(), context.modelFunctions().end(),
                    [targetFunctionName](const QString &function) {
                        return function.compare(targetFunctionName, Qt::CaseInsensitive) == 0;
                    })
                == context.modelFunctions().end()) {
            targetInfo.m_isEnvironment = true;
            targetInfo.m_providedInlineName = QString();
        } else {
            const QString targetInterfaceName = getInterfaceName(sourceInterface);
            targetInfo.m_providedInlineName =
                    QString("%1_0_PI_0_%2").arg(Escaper::escapePromelaIV(targetFunctionName)).arg(targetInterfaceName);
        }
        info->m_targets.append(targetInfo);
    }

    functionInfo.m_synchronousCalls.emplace(inlineName, std::move(info));
}
}
