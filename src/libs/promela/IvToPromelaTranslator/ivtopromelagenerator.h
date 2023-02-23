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

#include "ivtopromelatranslatorcontext.h"
#include "systeminfo.h"

namespace promela::translator {
class IvToPromelaGenerator final
{
public:
    IvToPromelaGenerator(IvToPromelaTranslatorContext &context, SystemInfo &systemInfo);

    auto generate() -> void;

private:
    auto initializeFunction(model::Sequence &sequence, const QString &functionName) const -> void;
    auto addChannelAndLock(const QString &functionName) const -> void;
    auto observerInputSignalName(const ObserverAttachment &attachment) const -> QString;
    auto observerInputSignalName(const ObserverInfo &observerInfo) const -> QString;
    auto attachInputObservers(const QString &functionName, const QString &interfaceName, const QString &parameterName,
            const QString &parameterType) const -> std::list<std::unique_ptr<promela::model::ProctypeElement>>;
    auto generateInitProctype() const -> void;
    auto generateProctype(const QString &functionName, bool environment, const ProctypeInfo &proctypeInfo) const
            -> void;
    auto generateProcessMessageBlock(const QString &functionName, const QString &channelName, const QString &inlineName,
            const QString &parameterType, const QString &parameterName, const QString &exitLabel, bool lock,
            std::list<std::unique_ptr<promela::model::ProctypeElement>> preProcessingElements,
            std::list<std::unique_ptr<promela::model::ProctypeElement>> postProcessingElements) const
            -> std::unique_ptr<model::ProctypeElement>;
    auto generateEnvironmentProctype(const QString &functionName, const QString &interfaceName,
            const std::pair<QString, QString> &interfaceParameter, const QString &sendInline) const -> void;
    auto generateSendInline(const QString &functionName, const QString &interfaceName,
            const QString &sourceFunctionName, const QString &sourceInterfaceName, const QString &channelName,
            const QString &parameterType, const QString &parameterName) const -> void;
    auto generateUnhandledInputInline(const QString &functionName, const ProctypeInfo &proctypeInfo) const -> void;
    auto createPromelaObjectsForFunction(const QString &functionName, const FunctionInfo &functionInfo) const -> void;
    auto generateProctypeForTimer(const QString &functionName, const ProctypeInfo &proctypeInfo) const -> void;
    auto createPromelaObjectsForAsyncPis(const QString &functionName, const ProctypeInfo &proctypeInfo) const -> void;
    auto createPromelaObjectsForSyncRis(const QString &functionInfo, const SynchronousCallInfo &info) const -> void;
    auto createPromelaObjectsForEnvironment(const QString &functionName, const FunctionInfo &functionInfo) const
            -> void;
    auto createCheckQueueInline(model::PromelaSystemModel *promelaModel, const QString &functionName,
            const QList<QString> &channelNames) const -> void;
    auto createCheckQueuesExpression(const QList<QString> &channelNames, bool empty) const
            -> std::unique_ptr<::promela::model::Expression>;
    auto createGetSenderInline(model::PromelaSystemModel *promelaModel, const QString &functionName) const -> void;
    auto createSystemState() const -> void;
    auto createPromelaObjectsForTimers() const -> void;
    auto createTimerInlinesForFunction(const QString &functionName, const QString &timerName,
            const promela::model::VariableRef &timerData) const -> void;
    auto createGlobalTimerObjects(const std::map<QString, promela::model::VariableRef> &timerSignals) const -> void;
    auto createWaitForInitStatement() const -> std::unique_ptr<model::ProctypeElement>;
    auto createPromelaObjectsForObservers() const -> void;

    auto createLockAcquireStatement(const QString &functionName) const -> std::unique_ptr<model::ProctypeElement>;
    auto createLockReleaseStatement(const QString &functionName) const -> std::unique_ptr<model::ProctypeElement>;
    auto createProcessInlineCall(const QString &inlineName, const QString &parameterType,
            const QString &parameterName) const -> std::unique_ptr<model::ProctypeElement>;
    auto createReceiveStatement(const QString &channelName, const QString &parameterType,
            const QString &parameterName) const -> std::unique_ptr<model::ProctypeElement>;

    auto getFunctionLockChannelName(const QString &functionName) const -> QString;

    auto handleParameterSubtype(const QString &parameterTypeName, const QString &parameterName,
            const QString &interfaceName, const QString &functionName) const -> QString;
    auto handleSendInlineArgument(const QString &parameterType, const QString &functionName,
            const QString &interfaceName, const QString parameterName) const -> QString;
    auto buildParameterSubtypeName(
            const QString &functionName, const QString &interfaceName, const QString &parameterName) const -> QString;
    auto createChannel(const QString &channelName, const QString &messageType, size_t channelSize) const -> void;

private:
    inline static const QString m_systemInitedVariableName = "inited";
    inline static const QString m_timerManagerProctypeName = "timer_manager_proc";

private:
    IvToPromelaTranslatorContext &m_context;
    SystemInfo &m_systemInfo;
};
}
