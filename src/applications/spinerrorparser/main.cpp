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
#include <QDebug>
#include <QRegularExpression>
#include <QString>
#include <iostream>
#include <optional>
#include <reporting/Report/dataconstraintviolationreport.h>
#include <reporting/Report/spinerrorparser.h>
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
        exit(EXIT_FAILURE);
    }

    // parse message
    SpinErrorParser parser;
    auto reports = parser.parse(spinMessage.value());

    for (auto report : reports) {
        qDebug() << "----- Report -----";
        qDebug() << "Error number:" << report.errorNumber;
        qDebug() << "Error type:" << report.errorType;
        qDebug() << "Error depth:" << report.errorDepth;
        qDebug() << "Error details (raw):" << report.rawErrorDetails;
        const DataConstraintViolationReport dataConstraintViolationReport =
                qvariant_cast<DataConstraintViolationReport>(report.parsedErrorDetails);
        qDebug() << "Variable name:" << dataConstraintViolationReport.variableName;
        for (auto constraint : dataConstraintViolationReport.constraints) {
            qDebug() << "    Constraint:" << constraint;
        }
        for (auto boundingValue : dataConstraintViolationReport.boundingValues) {
            qDebug() << "    Bounding value:" << boundingValue;
        }
    }

    return EXIT_SUCCESS;
}
