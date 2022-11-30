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

#include "htmlreportbuilder.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <grantlee/outputstream.h>
#include <grantlee_templates.h>
#include <reporting/HtmlReport/tracebuilder.h>
#include <reporting/Report/dataconstraintviolationreport.h>
#include <reporting/Report/spinerrorparser.h>

reporting::HtmlReportBuilder::HtmlReportBuilder()
{
    // initialize file loader relative to the root path
    m_fileLoader = QSharedPointer<Grantlee::FileSystemTemplateLoader>::create();
    m_fileLoader->setTemplateDirs({ QDir::rootPath() });
    m_engine = new Grantlee::Engine(this);
    m_engine->addTemplateLoader(m_fileLoader);
}

QString reporting::HtmlReportBuilder::parseAndBuildHtmlReport(const QStringList &spinMessages,
        const QStringList &sclFiles, const QList<RawErrorItem> &errors, const QStringList &mscObserverFiles,
        const QString &templateFile) const
{
    SpinErrorParser parser;
    auto reports = parser.parse(spinMessages, sclFiles, errors, mscObserverFiles);
    if (templateFile.isEmpty()) {
        initResource();
        return buildHtmlReport(reports, m_defaultTemplateFile);
    } else {
        return buildHtmlReport(reports, templateFile);
    }
}

QString reporting::HtmlReportBuilder::buildHtmlReport(const reporting::SpinErrorReport &spinErrorReport) const
{
    initResource();
    return buildHtmlReport(spinErrorReport, m_defaultTemplateFile);
}

QString reporting::HtmlReportBuilder::buildHtmlReport(
        const SpinErrorReport &spinErrorReport, const QString &templateFile) const
{
    // get absolute path for template file
    const QFileInfo templateFileInfo(templateFile);
    const auto templateFileAbsolutePath = templateFileInfo.absoluteFilePath();
    // load template to string
    QString templateContent = loadTemplateFile(templateFile);

    const Grantlee::Template stringTemplate = m_engine->newTemplate(templateContent, "template");
    const auto reportVariantList = buildReportVariant(spinErrorReport);

    QVariantHash mapping;
    mapping.insert("report", reportVariantList);

    Grantlee::Context context(mapping);
    const auto html = stringTemplate->render(&context);
    return html;
}

QString reporting::HtmlReportBuilder::loadTemplateFile(const QString &path) const
{
    QFile templateFile(path);
    if (templateFile.open(QFile::ReadOnly)) {
        QString templateContent = templateFile.readAll();
        templateFile.close();
        return templateContent;
    } else {
        return QString();
    }
}

QVariantList reporting::HtmlReportBuilder::buildReportVariant(const reporting::SpinErrorReport &spinErrorReport)
{
    QVariantList variantList;
    for (const auto &reportItem : spinErrorReport) {
        const auto variantHash = buildReportItemVariant(reportItem);
        variantList.append(variantHash);
    }
    return variantList;
}

QVariantHash reporting::HtmlReportBuilder::buildReportItemVariant(
        const reporting::SpinErrorReportItem &spinErrorReportItem)
{
    // add html formatting to spin trails
    const TraceBuilder traceBuilder;
    const auto scenarioHtml = traceBuilder.buildTraceReport(spinErrorReportItem.scenario);

    QVariantHash variantHash;
    variantHash.insert("errorNumber", spinErrorReportItem.errorNumber);
    variantHash.insert("errorDepth", spinErrorReportItem.errorDepth);
    variantHash.insert("errorCode", spinErrorReportItem.errorType);
    variantHash.insert("rawErrorDetails", spinErrorReportItem.rawErrorDetails);
    variantHash.insert("scenario", scenarioHtml);

    // resolve error type as string
    variantHash.insert("errorType",
            m_errorTypeNames.value(
                    spinErrorReportItem.errorType, m_errorTypeNames[reporting::SpinErrorReportItem::OtherError]));

    // resolve parsed error details depending on error type
    QVariant variantErrorDetails;
    switch (spinErrorReportItem.errorType) {
    case reporting::SpinErrorReportItem::DataConstraintViolation:
        variantErrorDetails = buildDataConstraintViolationVariant(spinErrorReportItem.parsedErrorDetails);
        break;
    case reporting::SpinErrorReportItem::StopConditionViolation:
        variantErrorDetails = buildStopConditionViolationVariant(spinErrorReportItem.parsedErrorDetails);
        break;
    case reporting::SpinErrorReportItem::ObserverFailure:
        variantErrorDetails = buildObserverFailureVariant(spinErrorReportItem.parsedErrorDetails);
        break;
    case reporting::SpinErrorReportItem::MscFailure:
        variantErrorDetails = buildMscFailureVariant(spinErrorReportItem.parsedErrorDetails);
        break;
    default:
        break;
    }

    variantHash.insert("errorDetails", variantErrorDetails);
    return variantHash;
}

QVariantHash reporting::HtmlReportBuilder::buildDataConstraintViolationVariant(const QVariant &errorDetails)
{
    reporting::DataConstraintViolationReport report =
            qvariant_cast<reporting::DataConstraintViolationReport>(errorDetails);
    QVariantHash variantHash;
    variantHash.insert("functionName", report.functionName);
    variantHash.insert("variableName", report.variableName);
    variantHash.insert("nestedStateName", report.nestedStateName);

    QVariantList variantConstraints;
    for (const auto &constraint : report.constraints) {
        variantConstraints.append(constraint);
    }

    QVariantList variantBoundingValues;
    for (const auto &boundingValue : report.boundingValues) {
        variantBoundingValues.append(boundingValue);
    }

    // count number of constraints for template iterator
    variantHash.insert("constraintCount", qMin(variantConstraints.size(), variantBoundingValues.size()));

    variantHash.insert("constraints", variantConstraints);
    variantHash.insert("boundingValues", variantBoundingValues);
    return variantHash;
}

QVariantHash reporting::HtmlReportBuilder::buildStopConditionViolationVariant(const QVariant &errorDetails)
{
    reporting::StopConditionViolationReport report =
            qvariant_cast<reporting::StopConditionViolationReport>(errorDetails);
    QVariantHash variantHash;
    // resolve violation clause as string
    variantHash.insert("violationClause",
            m_stopConditionViolationClauseNames.value(report.violationClause,
                    m_stopConditionViolationClauseNames[reporting::StopConditionViolationReport::UnknownClause]));
    // resolve violation type as string
    variantHash.insert("violationType",
            m_stopConditionViolationTypeNames.value(report.violationType,
                    m_stopConditionViolationTypeNames[reporting::StopConditionViolationReport::UndefinedType]));

    return variantHash;
}

QVariantHash reporting::HtmlReportBuilder::buildObserverFailureVariant(const QVariant &errorDetails)
{
    reporting::ObserverFailureReport report = qvariant_cast<reporting::ObserverFailureReport>(errorDetails);
    QVariantHash variantHash;
    variantHash.insert("observerName", report.observerName);
    // resolve observer state as string
    variantHash.insert("observerState",
            m_observerFailureObserverStateNames.value(report.observerState,
                    m_observerFailureObserverStateNames[reporting::ObserverFailureReport::UnknownState]));

    return variantHash;
}

QVariantHash reporting::HtmlReportBuilder::buildMscFailureVariant(const QVariant &errorDetails)
{
    reporting::MscFailureReport report = qvariant_cast<reporting::MscFailureReport>(errorDetails);
    QVariantHash variantHash;
    variantHash.insert("observerName", report.observerName);
    return variantHash;
}

const QString reporting::HtmlReportBuilder::m_defaultTemplateFile = QStringLiteral(":/template.html");

const QHash<reporting::SpinErrorReportItem::ErrorType, QString> reporting::HtmlReportBuilder::m_errorTypeNames = {
    { reporting::SpinErrorReportItem::DataConstraintViolation, "Data Constraint Violation" },
    { reporting::SpinErrorReportItem::StopConditionViolation, "Stop Condition Violation" },
    { reporting::SpinErrorReportItem::ObserverFailure, "Observer Failure" },
    { reporting::SpinErrorReportItem::MscFailure, "MSC Failure" },
    { reporting::SpinErrorReportItem::OtherError, "Unknown Error" }
};

const QHash<reporting::StopConditionViolationReport::ViolationClause, QString>
        reporting::HtmlReportBuilder::m_stopConditionViolationClauseNames = {
            { reporting::StopConditionViolationReport::Never, "Never" },
            { reporting::StopConditionViolationReport::Always, "Always" },
            { reporting::StopConditionViolationReport::Eventually, "Eventually" },
            { reporting::StopConditionViolationReport::FilterOut, "Filter Out" },
            { reporting::StopConditionViolationReport::UnknownClause, "Unknown Clause" },
        };

const QHash<reporting::StopConditionViolationReport::ViolationType, QString>
        reporting::HtmlReportBuilder::m_stopConditionViolationTypeNames = {
            { reporting::StopConditionViolationReport::Empty, "Empty" },
            { reporting::StopConditionViolationReport::Exist, "Exist" },
            { reporting::StopConditionViolationReport::GetState, "Get State" },
            { reporting::StopConditionViolationReport::Length, "Length" },
            { reporting::StopConditionViolationReport::QueueLast, "Queue Last" },
            { reporting::StopConditionViolationReport::QueueLength, "Queue Length" },
            { reporting::StopConditionViolationReport::Present, "Present" },
            { reporting::StopConditionViolationReport::UndefinedType, "Undefined Type" },
        };

const QHash<reporting::ObserverFailureReport::ObserverState, QString>
        reporting::HtmlReportBuilder::m_observerFailureObserverStateNames = {
            { reporting::ObserverFailureReport::ErrorState, "Error State" },
            { reporting::ObserverFailureReport::SuccessState, "Success State" },
            { reporting::ObserverFailureReport::UnknownState, "Unknown State" },
        };
