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
#include <conversion/common/exceptions.h>
#include <conversion/common/import/exceptions.h>
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
    // example input line:
    // '9387:	proc  3 (Actuator_step:1) system.pml:53 Send 84	-> queue 5 (Controller_result_channel)'
    // The line shall contains three substrings separated by tab character '\t'
    // first step is to split the line and check if the result has 3 substrings
    const QStringList elements = line.split('\t');
    if (elements.length() != 3) {
        return;
    }

    // use this to additionaly validate if the parsed line is an trail event
    QRegularExpression eventValidation(R"(^ *\d+:)");

    QRegularExpression commandValidation = buildChannelCommandRegexp();

    // channel name is enclosed by '(' and ')'
    QRegularExpression channelValidation(R"(\((\w+)\))");

    QRegularExpressionMatch eventMatch = eventValidation.match(elements[CHANNEL_EVENT_VALIDATION_PART]);

    if (eventMatch.hasMatch()) {
        QRegularExpressionMatch commandMatch = commandValidation.match(elements[CHANNEL_EVENT_COMMAND_PART]);
        QRegularExpressionMatch channelMatch = channelValidation.match(elements[CHANNEL_EVENT_CHANNEL_PART]);
        if (commandMatch.hasMatch() && channelMatch.hasMatch()) {
            const QString proctypeString = commandMatch.captured(CHANNEL_EVENT_PROCTYPE_NAME);
            QString proctypeName = proctypeString.split(':').front();
            const QString commandString = commandMatch.captured(CHANNEL_EVENT_COMMAND_STRING);
            const QString command = commandString.split(' ').front();
            QStringList parameters = commandString.split(' ').back().split(',');
            QString channelName = channelMatch.captured(CHANNEL_EVENT_CHANNEL_NAME);
            const ChannelEvent::Type eventType = command.compare("recv", Qt::CaseInsensitive) == 0
                    ? ChannelEvent::Type::Recv
                    : ChannelEvent::Type::Send;
            model.appendEvent(std::make_unique<ChannelEvent>(
                    eventType, std::move(proctypeName), std::move(channelName), std::move(parameters)));
        }
    }
}

QRegularExpression SpinTrailImporter::buildChannelCommandRegexp() const
{
    // use this to get proctype name, command (send or recv) and parameters
    // the first parsed group contains proctype name
    // the second parsed group contains command type and parameters
    // for the example input is 'proc  3 (Actuator_step:1) system.pml:53 Send 84'
    // the first group shall be 'Actuator_step:1' and the second 'Send 84'

    // begin
    QString pattern = QStringLiteral("^");

    // match "proc" and spin proctype number
    pattern += QStringLiteral(R"(proc\s+\d+\s+)");

    // match proctype name enclosed within parentheses
    pattern += QStringLiteral(R"(\(([\w:]+)\)\s+)");

    // match file and line number
    pattern += QStringLiteral(R"([\w\.]+:\d+\s)");

    // match operation (Send/Recv) and list of arguments
    pattern += QStringLiteral(R"(([\w\s,\.]+))");

    // end
    pattern += QStringLiteral("$");

    QRegularExpression commandValidation(pattern);
    return commandValidation;
}
}
