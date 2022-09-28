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

    findChannelNames(*ivModel, channels);

    std::unique_ptr<SimulatorTrailModel> simulatorTrail = std::make_unique<SimulatorTrailModel>();

    translate(*simulatorTrail, *spinTrailModel, channels, *asn1Model);

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
        const ivm::IVModel &ivModel, QMap<QString, ChannelInfo> &channels) const
{
    const QVector<IVFunction *> ivFunctionList = ivModel.allObjectsByType<IVFunction>();
    for (const IVFunction *ivFunction : ivFunctionList) {
        const QString targetFunctionName = ivFunction->property("name").toString();
        QList<QString> channelNames;
        for (const IVInterface *providedInterface : ivFunction->pis()) {
            if (providedInterface->kind() != IVInterface::OperationKind::Sporadic) {
                continue;
            }
            const QString interfaceName = providedInterface->property("name").toString();
            IVConnection *connection = ivModel.getConnectionForIface(providedInterface->id());
            IVInterface *requiredInterface = connection->sourceInterface();
            const QString sourceFunctionName = requiredInterface->function()->property("name").toString();
            const QVector<InterfaceParameter> parameterList = providedInterface->params();
            const QString channelName =
                    QString("%1_%2_channel").arg(Escaper::escapePromelaIV(targetFunctionName)).arg(interfaceName);
            if (parameterList.length() > 1) {
                throw TranslationException("Sporadic interface with more than one parameters is not allowed");
            }
            // in the simplest case this should be enaugh
            // let's add support for observers later
            ChannelInfo info;
            info.m_functionName = targetFunctionName;
            info.m_interfaceName = interfaceName;
            if (parameterList.length() == 1) {
                info.m_parameterTypeName = parameterList.front().paramTypeName();
            }
            channels.insert(channelName, info);
        }
    }
}

void SpinTrailToSimulatorTrailTranslator::translate(simulatortrail::model::SimulatorTrailModel &result,
        const spintrail::model::SpinTrailModel &spinTrailModel, const QMap<QString, ChannelInfo> &channels,
        const Asn1Acn::Asn1Model &asn1Model) const
{
    const std::list<std::unique_ptr<ChannelEvent>> &events = spinTrailModel.getEvents();

    for (const std::unique_ptr<ChannelEvent> &event : events) {
        // check if event is not a lock implementation
        // obtain type of the message
        // transform message to asn1 value notation
        if (isFunctionLockChannel(event->getChannelName())) {
            continue;
        }
        if (event->getType() == ChannelEvent::Type::Recv) {
            bool handled = false;

            if (channels.contains(event->getChannelName())) {
                const ChannelInfo &info = channels.value(event->getChannelName());
                if (info.m_parameterTypeName.isEmpty()) {
                    qDebug() << "Function " << info.m_functionName << " receives signal " << info.m_interfaceName;
                } else {
                    qDebug() << "Function " << info.m_functionName << " receives signal " << info.m_interfaceName
                             << " with parameters " << event->getParameters() << " of type "
                             << info.m_parameterTypeName;
                }
                result.appendValue(getValue(event->getParameters(), info, asn1Model));
                handled = true;
            }
            if (!handled) {
                qWarning() << "Not handled message on channel " << event->getChannelName();
            }
        }
    }
}

Asn1Acn::ValuePtr SpinTrailToSimulatorTrailTranslator::getValue(
        const QStringList &parameters, const ChannelInfo &info, const Asn1Acn::Asn1Model &asn1Model) const
{
    // workaround
    if (info.m_parameterTypeName.isEmpty()) {
        return std::make_unique<SingleValue>("0");
    }

    const Asn1Acn::File &file = *asn1Model.data().front();

    const Type *observableEventType = file.typeFromName("Observable-Event");

    if (observableEventType == nullptr) {
        throw TranslationException("Unable to find ASN.1 type Observable-Event");
    }

    const Type *parameterType = file.typeFromName(info.m_parameterTypeName);
    if (parameterType == nullptr) {
        QString message = QString("Unable to find ASN.1 type %1").arg(info.m_parameterTypeName);
        throw TranslationException(std::move(message));
    }

    SpinTrailValueParser valueParser;

    ValuePtr parameterValue = valueParser.parseValue(parameters, parameterType);

    // TODO source, dest and other parameters shall be present
    return std::make_unique<ChoiceValue>("input-event", std::move(parameterValue));
}

bool SpinTrailToSimulatorTrailTranslator::isFunctionLockChannel(const QString &channelName) const
{
    return !channelName.endsWith("_channel");
}
}
