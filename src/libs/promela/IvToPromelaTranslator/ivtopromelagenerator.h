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

#pragma once

#include "ccodegenerator.h"
#include "ivtopromelatranslatorcontext.h"
#include "systeminfo.h"

#include <QHash>

namespace promela::translator {
class IvToPromelaGenerator final
{
public:
    IvToPromelaGenerator(IvToPromelaTranslatorContext &context, SystemInfo &systemInfo);

    auto generate() -> void;

private:
    auto initializeFunction(model::Sequence &sequence, const QString &functionName) -> void;
    auto createMessageTypes() -> void;
    auto addChannelAndLock(const QString &functionName) -> void;
    auto observerInputSignalName(const ObserverAttachment &attachment) -> QString;
    auto observerInputSignalName(const ObserverInfo &observerInfo) -> QString;
    auto attachInputObservers(const QString &functionName, const QString &interfaceName, const QString &parameterName,
            const QString &parameterType) -> std::list<std::unique_ptr<promela::model::ProctypeElement>>;
    auto generateInitProctype() -> void;
    auto generateProctype(const QString &functionName, bool environment, const ProctypeInfo &proctypeInfo) -> void;
    auto generateProcessMessageBlock(const QString &functionName, const QString &modelFunctionName,
            const QString &channelName, const QString &inlineName, const QString &parameterType,
            const QString &parameterName, const QString &messageName, const QString &exitLabel, bool lock,
            std::list<std::unique_ptr<promela::model::ProctypeElement>> preProcessingElements,
            std::list<std::unique_ptr<promela::model::ProctypeElement>> postProcessingElements)
            -> std::unique_ptr<model::ProctypeElement>;
    auto generateEnvironmentProctype(const QString &functionName, const QString &interfaceName,
            const std::pair<QString, QString> &interfaceParameter, const QString &sendInline) -> void;
    auto generateEnvironmentProctypeForSynchronousInterface(const QString &functionName, const RequiredCallInfo &info)
            -> void;
    auto addEnvironmentProctype(const QString &functionName, const QString &interfaceName,
            promela::model::Sequence sequence, std::unique_ptr<promela::model::Sequence> loopSequence) -> void;
    auto generateSendInline(const QString &functionName, const QString &interfaceName,
            const QString &sourceFunctionName, const QString &sourceInterfaceName, const QString &channelName,
            const QString &parameterType, const QString &parameterName) -> void;
    auto generateUnhandledInputInline(const QString &functionName, const ProctypeInfo &proctypeInfo) -> void;
    auto createPromelaObjectsForFunction(const QString &functionName, const FunctionInfo &functionInfo) -> void;
    auto generateProctypeForTimer(const QString &functionName, const ProctypeInfo &proctypeInfo) -> void;
    auto createPromelaObjectsForAsyncPis(const QString &functionName, const ProctypeInfo &proctypeInfo) -> void;
    auto createPromelaObjectsForSporadicRis(const QString &functionInfo, const RequiredCallInfo &info) -> void;
    auto createPromelaObjectsForSyncRis(const QString &functionInfo, const RequiredCallInfo &info) -> void;
    auto createPromelaObjectsForEnvironment(const QString &functionName, const FunctionInfo &functionInfo) -> void;
    auto createCheckQueueInline(model::PromelaSystemModel *promelaModel, const QString &functionName,
            const QList<QString> &channelNames) -> void;
    auto createCheckQueuesExpression(const QList<QString> &channelNames, bool empty)
            -> std::unique_ptr<::promela::model::Expression>;
    auto createGetSenderInline(model::PromelaSystemModel *promelaModel, const QString &functionName) -> void;
    auto createSenderPidVariable(model::PromelaSystemModel *promelaModel, const QString &functionName) -> void;
    auto createSystemState() -> void;
    auto createPromelaObjectsForTimers() -> void;
    auto createTimerInlinesForFunction(const QString &functionName, const QString &timerName,
            const promela::model::VariableRef &timerData) -> void;
    auto createGlobalTimerObjects(const std::map<QString, promela::model::VariableRef> &timerSignals) -> void;
    auto createWaitForInitStatement() -> std::unique_ptr<model::ProctypeElement>;
    auto createPromelaObjectsForObservers() -> void;

    auto createLockAcquireStatement(const QString &functionName) -> std::unique_ptr<model::ProctypeElement>;
    auto createLockReleaseStatement(const QString &functionName) -> std::unique_ptr<model::ProctypeElement>;
    auto createProcessInlineCall(const QString &inlineName, const QString &parameterType, const QString &parameterName)
            -> std::unique_ptr<model::ProctypeElement>;
    auto createReceiveStatement(const QString &functionName, const QString &channelName, const QString &parameterType,
            const QString &parameterName) -> std::unique_ptr<model::ProctypeElement>;

    auto getFunctionLockChannelName(const QString &functionName) -> QString;

    auto handleParameterSubtype(const QString &parameterTypeName, const QString &parameterName,
            const QString &interfaceName, const QString &functionName) -> QString;
    auto handleSendInlineArgument(const QString &parameterType, const QString &functionName,
            const QString &interfaceName, const QString parameterName) -> QString;
    auto globalTemporaryVariableName(const QString &parameterName) -> QString;
    auto globalMessageName(const QString &parameterName) -> QString;
    auto buildParameterSubtypeName(
            const QString &functionName, const QString &interfaceName, const QString &parameterName) -> QString;
    auto createChannel(const QString &channelName, const QString &messageType, size_t channelSize) -> void;

    auto messageTypeName(const QString &typeName) -> QString;

private:
    inline static const QString m_systemInitedVariableName = "inited";
    inline static const QString m_timerManagerProctypeName = "timer_manager_proc";

private:
    IvToPromelaTranslatorContext &m_context;
    SystemInfo &m_systemInfo;
    QSet<QString> m_createdChannels;
    CCodeGenerator m_ccodeGenerator;
};
}
