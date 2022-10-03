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
using conversion::translator::TranslationException;
using ivm::IVConnection;
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVModel;
using shared::InterfaceParameter;
using spintrail::model::ChannelEvent;
using spintrail::model::SpinTrailModel;

namespace simulatortrail::translator {
std::vector<std::unique_ptr<conversion::Model>> SpinTrailToSimulatorTrailTranslator::translateModels(
        std::vector<conversion::Model *> sourceModels, const conversion::Options &options) const
{
    Q_UNUSED(options);

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

    QMap<QString, ChannelInfo> channels;
    findChannelNames(*ivModel, *asn1Model, channels);

    QMap<QString, QString> proctypes;
    findProctypes(*ivModel, proctypes);

    const Asn1Acn::File &asn1File = *asn1Model->data().front();

    const Type *observableEventType = asn1File.typeFromName("Observable-Event");

    if (observableEventType == nullptr) {
        throw TranslationException("Unable to find ASN.1 type Observable-Event");
    }

    std::unique_ptr<SimulatorTrailModel> simulatorTrail = std::make_unique<SimulatorTrailModel>();

    translate(*simulatorTrail, *spinTrailModel, channels, proctypes, observableEventType);

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

void SpinTrailToSimulatorTrailTranslator::findChannelNames(
        const ivm::IVModel &ivModel, const Asn1Acn::Asn1Model &asn1Model, QMap<QString, ChannelInfo> &channels) const
{
    const Asn1Acn::File &ans1file = *asn1Model.data().front();

    const QVector<IVFunction *> ivFunctionList = ivModel.allObjectsByType<IVFunction>();
    for (const IVFunction *ivFunction : ivFunctionList) {
        const QString targetFunctionName = ivFunction->property("name").toString();
        QList<QString> channelNames;
        for (const IVInterface *providedInterface : ivFunction->pis()) {
            if (providedInterface->kind() != IVInterface::OperationKind::Sporadic) {
                continue;
            }
            const QString interfaceName = providedInterface->property("name").toString();
            const QVariant queueSizeProperty = providedInterface->property("queue_size");
            const size_t channelSize = queueSizeProperty.isValid() ? queueSizeProperty.toULongLong() : 1;
            // this does not support multiple connections
            IVConnection *connection = ivModel.getConnectionForIface(providedInterface->id());
            IVInterface *requiredInterface = connection->sourceInterface();
            const QString sourceFunctionName = requiredInterface->function()->property("name").toString();
            const QVector<InterfaceParameter> parameterList = providedInterface->params();
            const QString channelName =
                    QString("%1_%2_channel").arg(Escaper::escapePromelaIV(targetFunctionName)).arg(interfaceName);
            if (parameterList.length() > 1) {
                throw TranslationException("Sporadic interface with more than one parameters is not allowed");
            }
            ChannelInfo info;
            info.m_functionName = targetFunctionName;
            info.m_interfaceName = interfaceName;
            info.m_channelSize = channelSize;
            info.m_possibleSenders.append(sourceFunctionName);
            if (parameterList.length() == 1) {
                info.m_parameterTypeName = parameterList.front().paramTypeName();
                info.m_parameterName = parameterList.front().name();
                info.m_parameterType = ans1file.typeFromName(info.m_parameterTypeName);
                if (info.m_parameterType == nullptr) {
                    QString message = QString("Cannot find type of queue %1:%2, name %3")
                                              .arg(targetFunctionName)
                                              .arg(interfaceName)
                                              .arg(info.m_parameterTypeName);
                    throw TranslationException(std::move(message));
                }
            } else {
                info.m_parameterType = nullptr;
            }
            channels.insert(channelName, info);
        }
    }
}

void SpinTrailToSimulatorTrailTranslator::findProctypes(
        const ivm::IVModel &ivModel, QMap<QString, QString> &proctypes) const
{
    const QVector<IVFunction *> ivFunctionList = ivModel.allObjectsByType<IVFunction>();
    for (const IVFunction *ivFunction : ivFunctionList) {
        const QString functionName = ivFunction->property("name").toString();
        QList<QString> channelNames;
        for (const IVInterface *providedInterface : ivFunction->pis()) {
            if (providedInterface->kind() != IVInterface::OperationKind::Sporadic) {
                continue;
            }
            const QString interfaceName = providedInterface->property("name").toString();

            const QString proctypeName =
                    QString("%1_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

            proctypes.insert(proctypeName, functionName);
        }
    }
}

void SpinTrailToSimulatorTrailTranslator::translate(simulatortrail::model::SimulatorTrailModel &result,
        const spintrail::model::SpinTrailModel &spinTrailModel, QMap<QString, ChannelInfo> &channels,
        const QMap<QString, QString> &proctypes, const Asn1Acn::Types::Type *observableEvent) const
{
    const std::list<std::unique_ptr<ChannelEvent>> &events = spinTrailModel.getEvents();

    for (const std::unique_ptr<ChannelEvent> &event : events) {
        if (isFunctionLockChannel(event->getChannelName())) {
            continue;
        }
        if (event->getProctypeName().isEmpty()) {
            // TODO this is init proctype
        }
        if (event->getType() == ChannelEvent::Type::Send) {
            if (channels.contains(event->getChannelName())) {
                ChannelInfo &channelInfo = channels[event->getChannelName()];
                const QString source = event->getProctypeName().isEmpty() ? channelInfo.m_possibleSenders.front()
                                                                          : proctypes.value(event->getProctypeName());
                const QString destination = channelInfo.m_functionName;
                ValuePtr message =
                        getValue(source, destination, channelInfo, observableEvent, event->getParameters(), false);
                qDebug() << "output from " << source << " to " << destination << " value " << message->asString();
                result.appendValue(std::move(message));
                if (channelInfo.m_senders.length() < static_cast<int>(channelInfo.m_channelSize)) {
                    channelInfo.m_senders.push_back(source);
                } else {
                    qDebug() << "message loss";
                }
            } else {
                qCritical() << "Cannot process trail event";
            }
        }
        if (event->getType() == ChannelEvent::Type::Recv) {
            if (channels.contains(event->getChannelName()) && proctypes.contains(event->getProctypeName())) {
                ChannelInfo &channelInfo = channels[event->getChannelName()];
                const QString destination = channelInfo.m_functionName;
                if (channelInfo.m_senders.isEmpty()) {
                    throw TranslationException("Cannot find sender of message");
                }
                const QString source = channelInfo.m_senders.front();
                channelInfo.m_senders.pop_front();
                ValuePtr message =
                        getValue(source, destination, channelInfo, observableEvent, event->getParameters(), true);
                qDebug() << "input from " << source << " to " << destination << " value " << message->asString();
                result.appendValue(std::move(message));
            } else {
                qCritical() << "Cannot process trail event";
            }
        }
    }
}

Asn1Acn::ValuePtr SpinTrailToSimulatorTrailTranslator::getValue(const QString &source, const QString &target,
        const ChannelInfo &info, const Asn1Acn::Types::Type *observableEvent, const QStringList &parameters,
        bool isInput) const
{
    if (observableEvent->typeEnum() != Asn1Acn::Types::Type::CHOICE) {
        throw TranslationException("Invalid type Observable-Event");
    }

    const Asn1Acn::Types::Choice *choice = dynamic_cast<const Asn1Acn::Types::Choice *>(observableEvent);

    QString messageType = QString(isInput ? "input-event" : "output-event");

    const Asn1Acn::Types::ChoiceAlternative *alt = choice->component(messageType);
    if (alt == nullptr) {
        QString message = QString("Cannot find alternative %1 in Observable-Event").arg(messageType);
        throw TranslationException(std::move(message));
    }

    SpinTrailValueParser valueParser;

    ValuePtr parameterValue = info.m_parameterTypeName.isEmpty()
            ? std::make_unique<NamedValue>()
            : valueParser.parseValue(parameters, info.m_parameterType);

    std::unique_ptr<NamedValue> interfaceParamValue = std::make_unique<NamedValue>();
    interfaceParamValue->addValue(info.m_parameterName, std::move(parameterValue));

    std::unique_ptr<ChoiceValue> interfaceValue =
            std::make_unique<ChoiceValue>(info.m_interfaceName, std::move(interfaceParamValue));

    std::unique_ptr<ChoiceValue> event =
            std::make_unique<ChoiceValue>(QString(isInput ? "msg-in" : "msg-out"), std::move(interfaceValue));

    std::unique_ptr<ChoiceValue> functionEvent =
            std::make_unique<ChoiceValue>(isInput ? target : source, std::move(event));

    std::unique_ptr<NamedValue> interfaceEventValue = std::make_unique<NamedValue>();
    interfaceEventValue->addValue("source", std::make_unique<SingleValue>(source));
    interfaceEventValue->addValue("dest", std::make_unique<SingleValue>(target));
    interfaceEventValue->addValue("event", std::move(functionEvent));

    return std::make_unique<ChoiceValue>(messageType, std::move(interfaceEventValue));
}

bool SpinTrailToSimulatorTrailTranslator::isFunctionLockChannel(const QString &channelName) const
{
    return !channelName.endsWith("_channel");
}
}
