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

#include "scversion.h"

#include <QCoreApplication>
#include <QRegularExpression>
#include <QString>
#include <iostream>
#include <optional>
#include <reporting/Report/dataconstraintviolationreport.h>
#include <reporting/Report/spinerrorreport.h>
#include <string.h>

using namespace reporting;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName(SC_ORGANISATION);
    app.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    app.setApplicationVersion(spaceCreatorVersion);
    app.setApplicationName(QObject::tr("Spin Error Parser"));

    std::optional<QString> spinMessage;

    const QStringList args = app.arguments();
    for (int i = 1; i < args.size(); ++i) {
        const QString &arg = args[i];
        if (arg == "-im") {
            if (i + 1 == args.size()) {
                qCritical("Missing error message after -im");
                exit(EXIT_FAILURE);
            }
            if (spinMessage.has_value()) {
                qCritical("Duplicated -m argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            spinMessage = args[i];
        } else if (arg == "-h" || arg == "--help") {
            qInfo("spinerrorparser: Spin Error Parser");
            qInfo("Usage: spinerrorparser [OPTIONS]");
            qInfo("  -im <message>          Message from spin");
            qInfo("  -h, --help             Print this message and exit");
            exit(EXIT_SUCCESS);
        } else {
            qCritical("Unrecognized command line option");
            exit(EXIT_FAILURE);
        }
    }

    if (!spinMessage.has_value()) {
        qCritical("Missing mandatory argument: input spinMessage");
    }

    // build pattern for error matching
    // error handling to be moved to the reporting lib
    const QRegularExpression regex("pan:(\\d+):\\s+(.+?)\\s+\\((.+?)\\)\\s+\\(at depth (\\d+)\\)\\n");
    // convert all matches to spin errors
    QList<SpinErrorReport> reports;

    auto globalMatch = regex.globalMatch(spinMessage.value());
    while (globalMatch.hasNext()) {
        const auto match = globalMatch.next();
        // build error from match tokens
        SpinErrorReport report;
        report.errorNumber = match.captured(1).toUInt();
        report.errorDepth = match.captured(4).toUInt();
        report.errorType = SpinErrorReport::DataConstraintViolation;
        report.rawErrorDetails = match.captured(3);
        reports.append(report);
    }
    return EXIT_SUCCESS;
}
