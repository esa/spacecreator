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

#include "spintrailtosimulatortrailtranslator.h"

#include "spintrailvalueparser.h"

#include <QDebug>
#include <asn1library/asn1/choicevalue.h>
#include <asn1library/asn1/namedvalue.h>
#include <asn1library/asn1/singlevalue.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/sequence.h>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivconnection.h>
#include <ivcore/ivfunction.h>
#include <simulatortrail/SimulatorTrailOptions/options.h>

using namespace simulatortrail::model;
using Asn1Acn::Asn1Model;
using Asn1Acn::ChoiceValue;
using Asn1Acn::NamedValue;
using Asn1Acn::SingleValue;
using Asn1Acn::ValuePtr;
using Asn1Acn::Types::Type;
using conversion::Escaper;
using conversion::Model;
using conversion::ModelType;
using conversion::simulatortrail::SimulatorTrailOptions;
using conversion::translator::TranslationException;
using ivm::IVConnection;
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVModel;
using promela::translator::IvToPromelaTranslator;
using shared::InterfaceParameter;
using spintrail::model::ChannelEvent;
using spintrail::model::ContinuousSignal;
using spintrail::model::ResetTimerEvent;
using spintrail::model::SetTimerEvent;
using spintrail::model::SpinTrailModel;
using spintrail::model::TrailEvent;
using spintrail::model::UnhandledInputEvent;

namespace simulatortrail::translator {

namespace {
void showPossibleSenders(const QMap<QString, QString> &data)
{
    qDebug() << "        possibleSenders ";
    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        qDebug() << "            " << iter.key() << " " << iter.value();
    }
}

void showObservers(const std::list<std::unique_ptr<IvToPromelaTranslator::ObserverInfo>> &data)
{
    qDebug() << "        observers ";
    for (auto iter = data.begin(); iter != data.end(); ++iter) {
        qDebug() << "            observerName " << (*iter)->m_observerName;
        qDebug() << "            observerInterface " << (*iter)->m_observerInterface;
        qDebug() << "            observerQueue " << (*iter)->m_observerQueue;
    }
}

void showProctype(const QString &name, const IvToPromelaTranslator::ProctypeInfo &info, bool isEnv)
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

void showEnvironmentProctype(const QString &name, const IvToPromelaTranslator::EnvProctypeInfo &info)
{
    qDebug() << "    Value generation proctype " << name;
    qDebug() << "        interfaceName " << info.m_interfaceName;
    qDebug() << "        priority " << info.m_priority;
}
}

SpinTrailToSimulatorTrailTranslator::ChannelInfo::ChannelInfo()
    : m_parameterType(nullptr)
    , m_channelSize(0)
    , m_isTimer(false)
{
}

SpinTrailToSimulatorTrailTranslator::ChannelInfo::ChannelInfo(const ChannelInfo &other)
    : m_functionName(other.m_functionName)
    , m_interfaceName(other.m_interfaceName)
    , m_parameterName(other.m_parameterName)
    , m_parameterTypeName(other.m_parameterTypeName)
    , m_parameterType(other.m_parameterType)
    , m_possibleSenders(other.m_possibleSenders)
    , m_channelSize(other.m_channelSize)
    , m_isTimer(other.m_isTimer)
    , m_senders(other.m_senders)
    , m_lastReceivedValue(other.m_lastReceivedValue != nullptr ? other.m_lastReceivedValue->clone() : nullptr)
{
}

SpinTrailToSimulatorTrailTranslator::ChannelInfo::ChannelInfo(ChannelInfo &&other)
    : m_functionName(std::move(other.m_functionName))
    , m_interfaceName(std::move(other.m_interfaceName))
    , m_parameterName(std::move(other.m_parameterName))
    , m_parameterTypeName(std::move(other.m_parameterTypeName))
    , m_parameterType(other.m_parameterType)
    , m_possibleSenders(std::move(other.m_possibleSenders))
    , m_channelSize(other.m_channelSize)
    , m_isTimer(other.m_isTimer)
    , m_senders(std::move(other.m_senders))
    , m_lastReceivedValue(std::move(other.m_lastReceivedValue))
{
}

SpinTrailToSimulatorTrailTranslator::ChannelInfo &SpinTrailToSimulatorTrailTranslator::ChannelInfo::operator=(
        const ChannelInfo &rhs)
{
    m_functionName = rhs.m_functionName;
    m_interfaceName = rhs.m_interfaceName;
    m_parameterName = rhs.m_parameterName;
    m_parameterTypeName = rhs.m_parameterTypeName;
    m_parameterType = rhs.m_parameterType;
    m_possibleSenders = rhs.m_possibleSenders;
    m_channelSize = rhs.m_channelSize;
    m_isTimer = rhs.m_isTimer;
    m_senders = rhs.m_senders;
    m_lastReceivedValue = rhs.m_lastReceivedValue != nullptr ? rhs.m_lastReceivedValue->clone() : nullptr;

    return *this;
}

SpinTrailToSimulatorTrailTranslator::ChannelInfo &SpinTrailToSimulatorTrailTranslator::ChannelInfo::operator=(
        ChannelInfo &&rhs)
{
    m_functionName = std::move(rhs.m_functionName);
    m_interfaceName = std::move(rhs.m_interfaceName);
    m_parameterName = std::move(rhs.m_parameterName);
    m_parameterTypeName = std::move(rhs.m_parameterTypeName);
    m_parameterType = rhs.m_parameterType;
    m_possibleSenders = std::move(rhs.m_possibleSenders);
    m_channelSize = rhs.m_channelSize;
    m_isTimer = rhs.m_isTimer;
    m_senders = std::move(rhs.m_senders);
    m_lastReceivedValue = std::move(rhs.m_lastReceivedValue);

    return *this;
}

std::vector<std::unique_ptr<conversion::Model>> SpinTrailToSimulatorTrailTranslator::translateModels(
        std::vector<conversion::Model *> sourceModels, const conversion::Options &options) const
{
    const IVModel *ivModel = getModel<IVModel>(sourceModels);
    const Asn1Model *asn1Model = getModel<Asn1Model>(sourceModels);
    const SpinTrailModel *spinTrailModel = getModel<SpinTrailModel>(sourceModels);

    if (ivModel == nullptr) {
        throw TranslationException("Missing InterfaceView model.");
    }
    if (asn1Model == nullptr) {
        throw TranslationException("Missing ASN.1 model.");
    }
    if (spinTrailModel == nullptr) {
        throw TranslationException("Missing SpinTrail model.");
    }

    IvToPromelaTranslator translator;

    std::unique_ptr<IvToPromelaTranslator::SystemInfo> systemInfo = translator.prepareSystemInfo(ivModel, options);

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
    }

    QMap<QString, ChannelInfo> channels;
    QMap<QString, std::pair<ChannelInfo, bool>> observerChannels;
    findChannelNames(*systemInfo, *asn1Model, channels, observerChannels);

    QMap<QString, QString> proctypes;
    findProctypes(*systemInfo, proctypes);

    const Type *observableEventType = findType(*asn1Model, "Observable-Event");

    if (observableEventType == nullptr) {
        throw TranslationException("Unable to find ASN.1 type Observable-Event");
    }

    std::unique_ptr<SimulatorTrailModel> simulatorTrail = std::make_unique<SimulatorTrailModel>();

    translate(*simulatorTrail, *spinTrailModel, channels, observerChannels, proctypes, observableEventType);

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(simulatorTrail));
    return result;
}

conversion::ModelType SpinTrailToSimulatorTrailTranslator::getSourceModelType() const
{
    return ModelType::SpinTrail;
}

conversion::ModelType SpinTrailToSimulatorTrailTranslator::getTargetModelType() const
{
    return ModelType::SimulatorTrail;
}

std::set<conversion::ModelType> SpinTrailToSimulatorTrailTranslator::getDependencies() const
{
    std::set<ModelType> dependencies;
    dependencies.insert(ModelType::SpinTrail);
    dependencies.insert(ModelType::InterfaceView);
    dependencies.insert(ModelType::Asn1);
    return dependencies;
}

void SpinTrailToSimulatorTrailTranslator::findChannelNames(const IvToPromelaTranslator::SystemInfo &systemInfo,
        const Asn1Acn::Asn1Model &asn1Model, QMap<QString, ChannelInfo> &channels,
        QMap<QString, std::pair<ChannelInfo, bool>> &observerChannels) const
{
    for (auto iter = systemInfo.m_functions.begin(); iter != systemInfo.m_functions.end(); ++iter) {
        for (auto proctypeIter = iter->second->m_proctypes.begin(); proctypeIter != iter->second->m_proctypes.end();
                ++proctypeIter) {
            ChannelInfo info;
            info.m_functionName = iter->first;
            info.m_interfaceName = proctypeIter->second->m_interfaceName;
            info.m_parameterName = proctypeIter->second->m_parameterName;
            info.m_parameterTypeName = proctypeIter->second->m_parameterTypeName;
            info.m_possibleSenders = proctypeIter->second->m_possibleSenders;
            info.m_channelSize = proctypeIter->second->m_queueSize;
            info.m_isTimer = proctypeIter->second->m_isTimer;

            if (!info.m_parameterTypeName.isEmpty()) {
                info.m_parameterType = findType(asn1Model, info.m_parameterTypeName);
                if (info.m_parameterType == nullptr) {
                    QString message = QString("Cannot find type of queue %1:%2, name %3")
                                              .arg(info.m_functionName)
                                              .arg(info.m_interfaceName)
                                              .arg(info.m_parameterTypeName);
                    throw TranslationException(std::move(message));
                }
            } else {
                info.m_parameterType = nullptr;
            }

            channels.insert(proctypeIter->second->m_queueName, info);

            bool first = true;
            for (const std::unique_ptr<IvToPromelaTranslator::ObserverInfo> &observerInfo :
                    proctypeIter->second->m_observers) {
                observerChannels.insert(observerInfo->m_observerQueue, std::make_pair(info, first));
                first = false;
            }
        }
        for (auto proctypeIter = iter->second->m_environmentSinkProctypes.begin();
                proctypeIter != iter->second->m_environmentSinkProctypes.end(); ++proctypeIter) {
            ChannelInfo info;
            info.m_functionName = iter->first;
            info.m_interfaceName = proctypeIter->second->m_interfaceName;
            info.m_parameterName = proctypeIter->second->m_parameterName;
            info.m_parameterTypeName = proctypeIter->second->m_parameterTypeName;
            info.m_possibleSenders = proctypeIter->second->m_possibleSenders;
            info.m_channelSize = proctypeIter->second->m_queueSize;
            info.m_isTimer = false;

            if (!info.m_parameterTypeName.isEmpty()) {
                info.m_parameterType = findType(asn1Model, info.m_parameterTypeName);
                if (info.m_parameterType == nullptr) {
                    QString message = QString("Cannot find type of queue %1:%2, name %3")
                                              .arg(info.m_functionName)
                                              .arg(info.m_interfaceName)
                                              .arg(info.m_parameterTypeName);
                    throw TranslationException(std::move(message));
                }
            } else {
                info.m_parameterType = nullptr;
            }

            channels.insert(proctypeIter->second->m_queueName, info);
            bool first = true;
            for (const std::unique_ptr<IvToPromelaTranslator::ObserverInfo> &observerInfo :
                    proctypeIter->second->m_observers) {
                observerChannels.insert(observerInfo->m_observerQueue, std::make_pair(info, first));
                first = false;
            }
        }
    }
}

void SpinTrailToSimulatorTrailTranslator::findProctypes(
        const IvToPromelaTranslator::SystemInfo &systemInfo, QMap<QString, QString> &proctypes) const
{
    for (auto iter = systemInfo.m_functions.begin(); iter != systemInfo.m_functions.end(); ++iter) {
        for (auto proctypeIter = iter->second->m_proctypes.begin(); proctypeIter != iter->second->m_proctypes.end();
                ++proctypeIter) {
            proctypes.insert(proctypeIter->first, iter->first);
        }
        for (auto proctypeIter = iter->second->m_environmentSourceProctypes.begin();
                proctypeIter != iter->second->m_environmentSourceProctypes.end(); ++proctypeIter) {
            proctypes.insert(proctypeIter->first, iter->first);
        }
        for (auto proctypeIter = iter->second->m_environmentSinkProctypes.begin();
                proctypeIter != iter->second->m_environmentSinkProctypes.end(); ++proctypeIter) {
            proctypes.insert(proctypeIter->first, iter->first);
        }
    }
}

void SpinTrailToSimulatorTrailTranslator::translate(SimulatorTrailModel &result,
        const spintrail::model::SpinTrailModel &spinTrailModel, QMap<QString, ChannelInfo> &channels,
        QMap<QString, std::pair<ChannelInfo, bool>> &observerChannels, const QMap<QString, QString> &proctypes,
        const Asn1Acn::Types::Type *observableEvent) const
{
    const std::list<std::unique_ptr<TrailEvent>> &events = spinTrailModel.getEvents();

    for (const std::unique_ptr<TrailEvent> &trailEvent : events) {
        switch (trailEvent->getEventType()) {
        case TrailEvent::EventType::CHANNEL_EVENT: {
            const ChannelEvent *event = dynamic_cast<const ChannelEvent *>(trailEvent.get());
            processSpinTrailEvent(result, event, channels, observerChannels, proctypes, observableEvent);
        } break;
        case TrailEvent::EventType::CONTINUOUS_SIGNAL: {
            const ContinuousSignal *event = dynamic_cast<const ContinuousSignal *>(trailEvent.get());
            processSpinTrailEvent(result, event);
        } break;
        case TrailEvent::EventType::SET_TIMER_EVENT: {
            const SetTimerEvent *event = dynamic_cast<const SetTimerEvent *>(trailEvent.get());
            processSpinTrailEvent(event);
        } break;
        case TrailEvent::EventType::RESET_TIMER_EVENT: {
            const ResetTimerEvent *event = dynamic_cast<const ResetTimerEvent *>(trailEvent.get());
            processSpinTrailEvent(event);
        } break;
        case TrailEvent::EventType::UNHANDLED_INPUT_EVENT: {
            const UnhandledInputEvent *event = dynamic_cast<const UnhandledInputEvent *>(trailEvent.get());
            processSpinTrailEvent(result, event, channels);
        } break;
        }
    }
}

void SpinTrailToSimulatorTrailTranslator::processSpinTrailEvent(SimulatorTrailModel &result,
        const spintrail::model::ChannelEvent *event, QMap<QString, ChannelInfo> &channels,
        QMap<QString, std::pair<ChannelInfo, bool>> &observerChannels, const QMap<QString, QString> &proctypes,
        const Asn1Acn::Types::Type *observableEvent) const
{
    if (isFunctionLockChannel(event->getChannelName())) {
        return;
    }
    if (event->getType() == ChannelEvent::Type::Send) {
        processSpinTrailSendEvent(result, event, channels, observerChannels, observableEvent);
    }
    if (event->getType() == ChannelEvent::Type::Recv) {
        processSpinTrailRecvEvent(result, event, channels, observerChannels, proctypes, observableEvent);
    }
}

void SpinTrailToSimulatorTrailTranslator::processSpinTrailSendEvent(SimulatorTrailModel &result,
        const spintrail::model::ChannelEvent *event, QMap<QString, ChannelInfo> &channels,
        QMap<QString, std::pair<ChannelInfo, bool>> &observerChannels,
        const Asn1Acn::Types::Type *observableEvent) const
{
    if (channels.contains(event->getChannelName())) {
        ChannelInfo &channelInfo = channels[event->getChannelName()];
        if (channelInfo.m_isTimer) {
            // this is a send to channel related to timer
            // ignore - the receive event shall produce value for simulator
            return;
        }
        const QString source = channelInfo.m_possibleSenders.firstKey();
        const QString destination = channelInfo.m_functionName;
        ValuePtr message =
                getMessageValue(source, destination, channelInfo, observableEvent, event->getParameters(), false);
        result.appendValue(std::move(message));
        if (channelInfo.m_senders.length() < static_cast<int>(channelInfo.m_channelSize)) {
            channelInfo.m_senders.push_back(source);
        } else {
            qDebug() << "message loss";
        }
    } else if (observerChannels.contains(event->getChannelName())) {
        // If message is processed by observers, the simulator trail shall contain
        // the output-signal to first observer (but the destination is model function)
        // and input-signal to the model function (if any)
        //
        // if this is first output observer
        // then register output value for simulator,
        // else do not produce simulator value, but keep info about sender
        std::pair<ChannelInfo, bool> &channelInfo = observerChannels[event->getChannelName()];
        if (channelInfo.first.m_isTimer) {
            // ignore
            return;
        }
        const QString source = channelInfo.first.m_possibleSenders.firstKey();
        const QString destination = channelInfo.first.m_functionName;

        if (channelInfo.first.m_senders.length() < static_cast<int>(channelInfo.first.m_channelSize)) {
            channelInfo.first.m_senders.push_back(source);
        } else {
            qDebug() << "message loss";
        }

        if (channelInfo.second) {
            ValuePtr message = getMessageValue(
                    source, destination, channelInfo.first, observableEvent, event->getParameters(), false);
            result.appendValue(std::move(message));
        }

    } else {
        qCritical() << "Cannot process trail event, unknown channel " << event->getChannelName();
        throw TranslationException("Cannot process trail event");
    }
}

void SpinTrailToSimulatorTrailTranslator::processSpinTrailRecvEvent(SimulatorTrailModel &result,
        const spintrail::model::ChannelEvent *event, QMap<QString, ChannelInfo> &channels,
        QMap<QString, std::pair<ChannelInfo, bool>> &observerChannels, const QMap<QString, QString> &proctypes,
        const Asn1Acn::Types::Type *observableEvent) const
{
    if (channels.contains(event->getChannelName()) && proctypes.contains(event->getProctypeName())) {
        ChannelInfo &channelInfo = channels[event->getChannelName()];
        if (channelInfo.m_isTimer) {
            std::unique_ptr<ChoiceValue> inputNone = std::make_unique<ChoiceValue>(
                    Escaper::escapeAsn1FieldName(channelInfo.m_interfaceName), std::make_unique<NamedValue>());
            std::unique_ptr<ChoiceValue> eventValue = std::make_unique<ChoiceValue>("msg-in", std::move(inputNone));
            std::unique_ptr<ChoiceValue> functionEvent = std::make_unique<ChoiceValue>(
                    Escaper::escapeAsn1FieldName(channelInfo.m_functionName), std::move(eventValue));
            std::unique_ptr<NamedValue> inputEvent = std::make_unique<NamedValue>();
            inputEvent->addValue("source", std::make_unique<SingleValue>(m_environmentName));
            inputEvent->addValue(
                    "dest", std::make_unique<SingleValue>(Escaper::escapeAsn1FieldName(channelInfo.m_functionName)));
            inputEvent->addValue("event", std::move(functionEvent));
            result.appendValue(std::make_unique<ChoiceValue>("input-event", std::move(inputEvent)));
        } else {
            const QString destination = channelInfo.m_functionName;
            if (channelInfo.m_senders.isEmpty()) {
                throw TranslationException("Cannot find sender of message");
            }
            const QString source = channelInfo.m_senders.front();
            channelInfo.m_senders.pop_front();
            ValuePtr message =
                    getMessageValue(source, destination, channelInfo, observableEvent, event->getParameters(), true);
            result.appendValue(std::move(message));
        }
    } else if (observerChannels.contains(event->getChannelName()) && proctypes.contains(event->getProctypeName())) {
        // do not produce input event when observer receives message
        std::pair<ChannelInfo, bool> &channelInfo = observerChannels[event->getChannelName()];
        channelInfo.first.m_senders.pop_front();
    } else {
        qCritical() << "Cannot process trail event, unknown channel " << event->getChannelName();
        throw TranslationException("Cannot process trail event");
    }
}

void SpinTrailToSimulatorTrailTranslator::processSpinTrailEvent(
        SimulatorTrailModel &result, const spintrail::model::ContinuousSignal *event) const
{
    std::unique_ptr<ChoiceValue> inputNone =
            std::make_unique<ChoiceValue>("input-none", std::make_unique<NamedValue>());
    std::unique_ptr<ChoiceValue> eventValue = std::make_unique<ChoiceValue>("msg-in", std::move(inputNone));
    std::unique_ptr<ChoiceValue> functionEvent = std::make_unique<ChoiceValue>(
            Escaper::escapeAsn1FieldName(event->getFunctionName()), std::move(eventValue));
    std::unique_ptr<NamedValue> inputEvent = std::make_unique<NamedValue>();
    inputEvent->addValue("source", std::make_unique<SingleValue>(m_environmentName));
    inputEvent->addValue("dest", std::make_unique<SingleValue>(Escaper::escapeAsn1FieldName(event->getFunctionName())));
    inputEvent->addValue("event", std::move(functionEvent));
    result.appendValue(std::make_unique<ChoiceValue>("input-event", std::move(inputEvent)));
}

void SpinTrailToSimulatorTrailTranslator::processSpinTrailEvent(const SetTimerEvent *event) const
{
    qDebug() << event->getFunctionName() << " calls set_timer(" << event->getInterval() << ", " << event->getTimerName()
             << ")";
}

void SpinTrailToSimulatorTrailTranslator::processSpinTrailEvent(const ResetTimerEvent *event) const
{
    qDebug() << event->getFunctionName() << " calls reset_timer(" << event->getTimerName() << ")";
}

void SpinTrailToSimulatorTrailTranslator::processSpinTrailEvent(
        SimulatorTrailModel &result, const UnhandledInputEvent *event, const QMap<QString, ChannelInfo> &channels) const
{
    QString source;
    ValuePtr interfaceParamValue;

    for (auto iter = channels.begin(); iter != channels.end(); ++iter) {
        if (iter.value().m_functionName.compare(event->getFunctionName(), Qt::CaseInsensitive) == 0
                && iter.value().m_interfaceName.compare(event->getInterfaceName(), Qt::CaseInsensitive) == 0) {
            source = iter->m_possibleSenders.begin().key();
            if (iter->m_lastReceivedValue) {
                interfaceParamValue = iter->m_lastReceivedValue->clone();
            } else {
                interfaceParamValue = std::make_unique<NamedValue>();
            }
            break;
        }
    }

    if (source.isEmpty()) {
        qCritical() << "Cannot find interface, function: " << event->getFunctionName()
                    << " interface: " << event->getInterfaceName();
        throw TranslationException("Cannot process trail event");
    }

    const QString interfaceName = event->getInterfaceName();
    std::unique_ptr<ChoiceValue> interfaceValue =
            std::make_unique<ChoiceValue>(Escaper::escapeAsn1FieldName(interfaceName), std::move(interfaceParamValue));

    std::unique_ptr<ChoiceValue> msgValue = std::make_unique<ChoiceValue>(QString("msg-in"), std::move(interfaceValue));

    std::unique_ptr<ChoiceValue> functionEvent =
            std::make_unique<ChoiceValue>(Escaper::escapeAsn1FieldName(event->getFunctionName()), std::move(msgValue));

    std::unique_ptr<NamedValue> interfaceEventValue = std::make_unique<NamedValue>();
    interfaceEventValue->addValue("source", std::make_unique<SingleValue>(Escaper::escapeAsn1FieldName(source)));
    interfaceEventValue->addValue(
            "dest", std::make_unique<SingleValue>(Escaper::escapeAsn1FieldName(event->getFunctionName())));
    interfaceEventValue->addValue("event", std::move(functionEvent));

    result.appendValue(std::make_unique<ChoiceValue>("unhandled-input", std::move(interfaceEventValue)));
}

Asn1Acn::ValuePtr SpinTrailToSimulatorTrailTranslator::getMessageValue(const QString &source, const QString &target,
        ChannelInfo &info, const Asn1Acn::Types::Type *observableEvent, const QStringList &parameters,
        bool isInput) const
{
    if (observableEvent->typeEnum() != Asn1Acn::Types::Type::CHOICE) {
        throw TranslationException("Invalid type Observable-Event");
    }

    const Asn1Acn::Types::Choice *choice = dynamic_cast<const Asn1Acn::Types::Choice *>(observableEvent);

    const QString messageType = QString(isInput ? "input-event" : "output-event");

    const Asn1Acn::Types::ChoiceAlternative *alt = choice->component(messageType);
    if (alt == nullptr) {
        QString message = QString("Cannot find alternative %1 in Observable-Event").arg(messageType);
        throw TranslationException(std::move(message));
    }

    SpinTrailValueParser valueParser;

    std::unique_ptr<NamedValue> interfaceParamValue = std::make_unique<NamedValue>();

    if (!info.m_parameterTypeName.isEmpty()) {
        ValuePtr parameterValue = info.m_parameterTypeName.isEmpty()
                ? std::make_unique<NamedValue>()
                : valueParser.parseValue(parameters, info.m_parameterType);
        info.m_lastReceivedValue = parameterValue->clone();
        interfaceParamValue->addValue(Escaper::escapeAsn1FieldName(info.m_parameterName), std::move(parameterValue));
    } else {
        info.m_lastReceivedValue.reset();
    }

    const QString interfaceName = isInput ? info.m_interfaceName : info.m_possibleSenders.value(source);
    std::unique_ptr<ChoiceValue> interfaceValue =
            std::make_unique<ChoiceValue>(Escaper::escapeAsn1FieldName(interfaceName), std::move(interfaceParamValue));

    std::unique_ptr<ChoiceValue> event =
            std::make_unique<ChoiceValue>(QString(isInput ? "msg-in" : "msg-out"), std::move(interfaceValue));

    std::unique_ptr<ChoiceValue> functionEvent =
            std::make_unique<ChoiceValue>(Escaper::escapeAsn1FieldName(isInput ? target : source), std::move(event));

    std::unique_ptr<NamedValue> interfaceEventValue = std::make_unique<NamedValue>();
    interfaceEventValue->addValue("source", std::make_unique<SingleValue>(Escaper::escapeAsn1FieldName(source)));
    interfaceEventValue->addValue("dest", std::make_unique<SingleValue>(Escaper::escapeAsn1FieldName(target)));
    interfaceEventValue->addValue("event", std::move(functionEvent));

    return std::make_unique<ChoiceValue>(messageType, std::move(interfaceEventValue));
}

bool SpinTrailToSimulatorTrailTranslator::isFunctionLockChannel(const QString &channelName) const
{
    return channelName.endsWith("_lock");
}

const Asn1Acn::Types::Type *SpinTrailToSimulatorTrailTranslator::findType(
        const Asn1Acn::Asn1Model &asn1Model, const QString &name) const
{
    for (const std::unique_ptr<Asn1Acn::File> &asn1File : asn1Model.data()) {
        const Asn1Acn::Types::Type *type = asn1File->typeFromName(name);
        if (type != nullptr) {
            return type;
        }
    }

    return nullptr;
}
}
