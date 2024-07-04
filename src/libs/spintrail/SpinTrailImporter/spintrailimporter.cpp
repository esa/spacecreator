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

#include "spintrailimporter.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QTextStream>
#include <algorithm>
#include <conversion/common/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <iterator>
#include <promela/PromelaCommon/constants.h>
#include <spintrail/SpinTrailModel/channelevent.h>
#include <spintrail/SpinTrailModel/continuoussignal.h>
#include <spintrail/SpinTrailModel/resettimerevent.h>
#include <spintrail/SpinTrailModel/settimerevent.h>
#include <spintrail/SpinTrailModel/spintrailmodel.h>
#include <spintrail/SpinTrailModel/unhandledinputevent.h>
#include <spintrail/SpinTrailOptions/options.h>

using namespace conversion::spintrail;

using conversion::FileNotFoundException;
using conversion::importer::ImportException;
using promela::common::PromelaConstants;
using spintrail::model::ChannelEvent;
using spintrail::model::ContinuousSignal;
using spintrail::model::ResetTimerEvent;
using spintrail::model::SetTimerEvent;
using spintrail::model::SpinTrailModel;
using spintrail::model::UnhandledInputEvent;

namespace spintrail::importer {
std::unique_ptr<conversion::Model> SpinTrailImporter::importModel(const conversion::Options &options) const
{
    std::unique_ptr<SpinTrailModel> model = std::make_unique<SpinTrailModel>();

    std::optional<QString> inputFilepath = options.value(SpinTrailOptions::inputFilepath);

    if (inputFilepath) {
        const QFileInfo inputFileinfo(inputFilepath.value());

        if (!inputFileinfo.exists()) {
            throw FileNotFoundException(inputFilepath.value(), "while importing");
        }

        QFile inputFile(inputFileinfo.absoluteFilePath());
        if (!inputFile.open(QFileDevice::ReadOnly | QFileDevice::Text)) {
            throw ImportException(QString("Cannot open file: %1").arg(inputFileinfo.absoluteFilePath()));
        }

        QTextStream input(&inputFile);

        while (!input.atEnd()) {
            QString line = input.readLine();
            processLine(*model, line);
        }
    } else {
        throw ImportException("Spin Trail file to import wasn't specified");
    }

    return model;
}

void SpinTrailImporter::processLine(spintrail::model::SpinTrailModel &model, const QString &line) const
{
    QRegularExpression continuousSignalValidation(R"(^ *process continuous signals in (\w+)$)");
    QRegularExpressionMatch match = continuousSignalValidation.match(line);
    if (match.hasMatch()) {
        const QString functionName = match.captured(CONTINUOUS_SIGNAL_EVENT_FUNCTION_NAME);
        model.appendEvent(std::make_unique<ContinuousSignal>(functionName));
        return;
    }
    QRegularExpression setTimerEventValidation(R"(^ *set_timer (\w+) (\w+) (\d+)$)");
    match = setTimerEventValidation.match(line);
    if (match.hasMatch()) {
        const QString functionName = match.captured(SET_TIMER_EVENT_FUNCTION_NAME);
        const QString interfaceName = match.captured(SET_TIMER_EVENT_INTERFACE_NAME);
        size_t interval = match.captured(SET_TIMER_EVENT_INTERVAL).toULong();
        model.appendEvent(std::make_unique<SetTimerEvent>(functionName, interfaceName, interval));
        return;
    }
    QRegularExpression resetTimerEventValidation(R"( *reset_timer (\w+) (\w+))");
    match = resetTimerEventValidation.match(line);
    if (match.hasMatch()) {
        const QString functionName = match.captured(RESET_TIMER_EVENT_FUNCTION_NAME);
        const QString interfaceName = match.captured(RESET_TIMER_EVENT_INTERFACE_NAME);
        model.appendEvent(std::make_unique<ResetTimerEvent>(functionName, interfaceName));
        return;
    }
    QRegularExpression unhandledInputEventValidation(R"( *unhandled_input (\w+) (\w+))");
    match = unhandledInputEventValidation.match(line);
    if (match.hasMatch()) {
        const QString functionName = match.captured(UNHANDLED_INPUT_EVENT_FUNCTION_NAME);
        const QString interfaceName = match.captured(UNHANDLED_INPUT_EVENT_INTERFACE_NAME);
        model.appendEvent(std::make_unique<UnhandledInputEvent>(functionName, interfaceName));
        return;
    }

    QRegularExpression channelSendEventValidation(
            QString(R"( *%1 (\w+): (.*)$)").arg(PromelaConstants::channelSendMessage));
    match = channelSendEventValidation.match(line);
    if (match.hasMatch()) {
        QString channelName = match.captured(CHANNEL_EVENT_CHANNEL_NAME);
        QStringList rawParameters = match.captured(CHANNEL_EVENT_PARAMETER_LIST).split(',');
        QStringList parameters;
        std::transform(rawParameters.begin(), rawParameters.end(), std::back_inserter(parameters),
                [](const QString &value) { return value.trimmed(); });
        model.appendEvent(std::make_unique<ChannelEvent>(
                ChannelEvent::Type::Send, std::move(channelName), std::move(parameters)));
    }

    QRegularExpression channelRecvEventValidation(
            QString(R"( *%1 (\w+): (.*)$)").arg(PromelaConstants::channelRecvMessage));
    match = channelRecvEventValidation.match(line);
    if (match.hasMatch()) {
        QString channelName = match.captured(CHANNEL_EVENT_CHANNEL_NAME);
        QStringList rawParameters = match.captured(CHANNEL_EVENT_PARAMETER_LIST).split(',');
        QStringList parameters;
        std::transform(rawParameters.begin(), rawParameters.end(), std::back_inserter(parameters),
                [](const QString &value) { return value.trimmed(); });
        model.appendEvent(std::make_unique<ChannelEvent>(
                ChannelEvent::Type::Recv, std::move(channelName), std::move(parameters)));
    }
}
}
