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
#include <QFile>
#include <QRegularExpression>
#include <QString>
#include <grantlee/template.h>
#include <grantlee/texthtmlbuilder.h>
#include <iostream>
#include <optional>
#include <reporting/HtmlReport/htmlreportbuilder.h>
#include <reporting/HtmlReport/tracebuilder.h>
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
    std::optional<QString> spinTraces;
    std::optional<QString> sclCondition;
    QStringList trailFiles;
    std::optional<QString> templateFile;
    std::optional<QString> targetFile;

    const QStringList args = app.arguments();
    for (int i = 1; i < args.size(); ++i) {
        const QString &arg = args[i];
        if (arg == "-im") {
            if (i + 1 == args.size()) {
                qCritical("Missing error message after -im");
                exit(EXIT_FAILURE);
            }
            if (spinMessage.has_value()) {
                qCritical("Duplicated -im argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            spinMessage = args[i];
        } else if (arg == "-is") {
            if (i + 1 == args.size()) {
                qCritical("Missing spin traces after -is");
                exit(EXIT_FAILURE);
            }
            if (spinTraces.has_value()) {
                qCritical("Duplicated -is argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            spinTraces = args[i];
        } else if (arg == "-ic") {
            if (i + 1 == args.size()) {
                qCritical("Missing scl condition after -ic");
                exit(EXIT_FAILURE);
            }
            if (sclCondition.has_value()) {
                qCritical("Duplicated -ic argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            sclCondition = args[i];
        } else if (arg == "-ir") {
            if (i + 1 == args.size()) {
                qCritical("Missing trail file after -ir");
                exit(EXIT_FAILURE);
            }
            ++i;
            trailFiles.append(args[i]);
        } else if (arg == "-it") {
            if (i + 1 == args.size()) {
                qCritical("Missing template file after -it");
                exit(EXIT_FAILURE);
            }
            if (templateFile.has_value()) {
                qCritical("Duplicated -it argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            templateFile = args[i];
        } else if (arg == "-of") {
            if (i + 1 == args.size()) {
                qCritical("Missing target file after -of");
                exit(EXIT_FAILURE);
            }
            if (targetFile.has_value()) {
                qCritical("Duplicated -of argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            targetFile = args[i];
        }

        else if (arg == "-h" || arg == "--help") {
            qInfo("spinerrorparser: Spin Error Parser");
            qInfo("Usage: spinerrorparser [OPTIONS]");
            qInfo("  -im <message>          Message from spin");
            qInfo("  -is <message>          Spin traces");
            qInfo("  -ic <condition>        Condition from scl.txt");
            qInfo("  -ir <filename>         Trail files (can be repeated)");
            qInfo("  -it <filename>         HTML template file name");
            qInfo("  -of <filename>         Target file name");
            qInfo("  -h, --help             Print this message and exit");
            exit(EXIT_SUCCESS);
        } else {
            qCritical("Unrecognized command line option");
            exit(EXIT_FAILURE);
        }
    }

    if (!spinMessage.has_value()) {
        qCritical("Missing mandatory argument: -im spinMessage");
        exit(EXIT_FAILURE);
    }

    if (!spinTraces.has_value()) {
        qCritical("Missing mandatory argument: -is spinTraces");
        exit(EXIT_FAILURE);
    }

    if (!sclCondition.has_value()) {
        qCritical("Missing mandatory argument: -ic sclCondition");
        exit(EXIT_FAILURE);
    }

    if (!templateFile.has_value()) {
        qCritical("Missing mandatory argument: -it templateFile");
        exit(EXIT_FAILURE);
    }

    if (!targetFile.has_value()) {
        qCritical("Missing mandatory argument: -of targetFile");
        exit(EXIT_FAILURE);
    }

    // parse trails
    const TraceBuilder traceBuilder;
    QStringList trailsHtml;
    for (auto trailFile : trailFiles) {
        QFile file(trailFile);
        if (file.open(QFile::ReadOnly)) {
            const QString fileContents(file.readAll());
            trailsHtml.append(traceBuilder.buildTraceReport(fileContents));
            file.close();
        } else {
            qCritical("Unable to open trail file");
            exit(EXIT_FAILURE);
        }
    }

    // parse message
    SpinErrorParser parser;
    auto reports = parser.parse(spinMessage.value(), spinTraces.value(), sclCondition.value());
    for (auto report : reports) {
        qDebug() << "----- Report -----";
        qDebug() << "Error number:" << report.errorNumber;
        qDebug() << "Error type:" << report.errorType;
        qDebug() << "Error depth:" << report.errorDepth;
        qDebug() << "Error details (raw):" << report.rawErrorDetails;

        switch (report.errorType) {
        case SpinErrorReportItem::DataConstraintViolation: {
            const DataConstraintViolationReport dataConstraintViolationReport =
                    qvariant_cast<DataConstraintViolationReport>(report.parsedErrorDetails);
            qDebug() << "Function name:" << dataConstraintViolationReport.functionName;
            qDebug() << "Variable name:" << dataConstraintViolationReport.variableName;
            qDebug() << "Nested state:" << dataConstraintViolationReport.nestedStateName;
            for (auto constraint : dataConstraintViolationReport.constraints) {
                qDebug() << "    Constraint:" << constraint;
            }
            for (auto boundingValue : dataConstraintViolationReport.boundingValues) {
                qDebug() << "    Bounding value:" << boundingValue;
            }
        } break;
        case SpinErrorReportItem::StopConditionViolation: {
            const StopConditionViolationReport stopConditionViolationReport =
                    qvariant_cast<StopConditionViolationReport>(report.parsedErrorDetails);
            qDebug() << "Violation type:" << stopConditionViolationReport.violationType;
        } break;
        case SpinErrorReportItem::ObserverFailure:
            break;
        default:
            qDebug() << "unknown error";
            break;
        }
    }

    const HtmlReportBuilder htmlReportBuilder;
    const auto htmlReport = htmlReportBuilder.buildHtmlReport(reports, templateFile.value());

    QFile file(targetFile.value());
    if (file.open(QFile::WriteOnly)) {
        const qint64 bytesWritten = file.write(htmlReport.toUtf8());
        if (bytesWritten == -1) {
            qCritical("Unable to write to target filename");
            exit(EXIT_FAILURE);
        }
    } else {
        qCritical("Unable to open target filename");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
