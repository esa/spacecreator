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
#include <reporting/Report/dataconstraintviolationreport.h>
#include <reporting/Report/rawerroritem.h>
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

    QStringList spinMessages;
    QStringList spinTraceFiles;
    QStringList sclConditionFiles;
    QStringList scenarioFiles;
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
            ++i;
            spinMessages.append(args[i]);
        } else if (arg == "-is") {
            if (i + 1 == args.size()) {
                qCritical("Missing spin traces after -is");
                exit(EXIT_FAILURE);
            }
            ++i;
            spinTraceFiles.append(args[i]);
        } else if (arg == "-scl") {
            if (i + 1 == args.size()) {
                qCritical("Missing scl condition after -scl");
                exit(EXIT_FAILURE);
            }
            ++i;
            sclConditionFiles.append(args[i]);
        } else if (arg == "-ir") {
            if (i + 1 == args.size()) {
                qCritical("Missing trail file after -ir");
                exit(EXIT_FAILURE);
            }
            ++i;
            scenarioFiles.append(args[i]);
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
            qInfo("  -im <message>          Message from spin (can be repeated)");
            qInfo("  -is <message>          Spin traces (can be repeated)");
            qInfo("  -scl <condition>       Condition from scl.txt (can be repeated)");
            qInfo("  -ir <filename>         Trail files (can be repeated)");
            qInfo("  -it <filename>         HTML template file name (optional)");
            qInfo("  -of <filename>         Target file name");
            qInfo("  -h, --help             Print this message and exit");
            exit(EXIT_SUCCESS);
        } else {
            qCritical("Unrecognized command line option");
            exit(EXIT_FAILURE);
        }
    }

    if (spinMessages.isEmpty()) {
        qCritical("Missing mandatory argument: -im spinMessage");
        exit(EXIT_FAILURE);
    }

    if (spinTraceFiles.isEmpty()) {
        qCritical("Missing mandatory argument: -is spinTraces");
        exit(EXIT_FAILURE);
    }

    if (sclConditionFiles.isEmpty()) {
        qCritical("Missing mandatory argument: -ic sclCondition");
        exit(EXIT_FAILURE);
    }

    if (!targetFile.has_value()) {
        qCritical("Missing mandatory argument: -of targetFile");
        exit(EXIT_FAILURE);
    }

    // build error structure
    QList<reporting::RawErrorItem> rawErrorItems;
    const int traceCount = spinTraceFiles.size();
    for (int i = 0; i < traceCount; ++i) {
        reporting::RawErrorItem rawErrorItem;
        rawErrorItem.spinTraceFile = spinTraceFiles[i];
        rawErrorItem.scenarioFile = scenarioFiles[i];
        rawErrorItem.possibleCycleSource = std::make_pair(QString(), 0);
        rawErrorItems.append(rawErrorItem);
    };

    // parse message
    SpinErrorParser parser;
    auto reports = parser.parse(spinMessages, sclConditionFiles, rawErrorItems, QStringList());
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

    QString htmlReport;
    const HtmlReportBuilder htmlReportBuilder;
    if (templateFile.has_value()) {
        htmlReport = htmlReportBuilder.buildHtmlReport(reports, templateFile.value());
    } else {
        htmlReport = htmlReportBuilder.buildHtmlReport(reports);
    }

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
