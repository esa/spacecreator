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

#include "spinerrorparser.h"

#include <QDebug>
#include <QRegularExpression>

reporting::SpinErrorReport reporting::SpinErrorParser::parse(
        const QString &spinMessage, const QString &spinTraces, const QString &sclConditions) const
{
    reporting::SpinErrorReport report;
    // parse variable violations
    QRegularExpressionMatchIterator matches = matchSpinErrors(spinMessage);
    while (matches.hasNext()) {
        auto reportItem = buildReportItem(matches.next());
        report.append(reportItem);
    }
    // parse stop condition violation
    if (!spinTraces.isEmpty() && spinTraces.trimmed() != m_spinNoTrailFileMessage) {
        auto reportItem = buildStopConditionViolationReportItem(sclConditions);
        report.append(reportItem);
    }
    return report;
}

reporting::SpinErrorReportItem reporting::SpinErrorParser::buildReportItem(
        const QRegularExpressionMatch &matchedError) const
{
    SpinErrorReportItem reportItem;
    reportItem.errorNumber = matchedError.captured(ReportItemParseTokens::ErrorNumber).toUInt();
    reportItem.errorDepth = matchedError.captured(ReportItemParseTokens::ErrorDepth).toUInt();
    reportItem.errorType = SpinErrorReportItem::DataConstraintViolation;
    reportItem.rawErrorDetails = matchedError.captured(ReportItemParseTokens::ErrorDetails);
    // for now, only parse data constraint violation
    switch (reportItem.errorType) {
    case SpinErrorReportItem::DataConstraintViolation:
        reportItem.parsedErrorDetails = parseVariableViolation(reportItem.rawErrorDetails);
        break;
    case SpinErrorReportItem::StopConditionViolation:
        reportItem.parsedErrorDetails = parseStopConditionViolation(reportItem.rawErrorDetails);
        break;
    default:
        break;
    }
    return reportItem;
}

reporting::SpinErrorReportItem reporting::SpinErrorParser::buildStopConditionViolationReportItem(const QString &) const
{
    SpinErrorReportItem reportItem;
    reportItem.errorNumber = 0;
    reportItem.errorType = SpinErrorReportItem::OtherError;
    return reportItem;
}

QRegularExpressionMatchIterator reporting::SpinErrorParser::matchSpinErrors(const QString &spinMessage) const
{
    const QRegularExpression regex = buildSpinErrorRegex();
    return regex.globalMatch(spinMessage);
}

QVariant reporting::SpinErrorParser::parseVariableViolation(const QString &rawError) const
{
    DataConstraintViolationReport violationReport;
    const QRegularExpression regex = buildDataConstraintViolationRegex();
    QRegularExpressionMatchIterator matches = regex.globalMatch(rawError);
    while (matches.hasNext()) {
        const QRegularExpressionMatch matchedError = matches.next();

        parseVariableName(matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationVariableName),
                violationReport);
        violationReport.constraints.append(
                matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationType));
        QVariant boundingValue;
        // put either int or float value into the report,
        // depending on whether the "." character exists in the matched token
        const QString boundingValueToken =
                matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationBoundingValue);
        if (boundingValueToken.contains(QChar('.'))) {
            boundingValue.setValue(boundingValueToken.toFloat());
        } else {
            boundingValue.setValue(boundingValueToken.toInt());
        }
        violationReport.boundingValues.append(boundingValue);
    }
    QVariant variableViolation;
    variableViolation.setValue(violationReport);
    return variableViolation;
}

QVariant reporting::SpinErrorParser::parseStopConditionViolation(const QString &) const
{
    StopConditionViolationReport violationReport;
    violationReport.functionName = "Function Name2";
    violationReport.violationType = StopConditionViolationReport::NeverFilterOut;

    QVariant stopConditionViolation;
    stopConditionViolation.setValue(violationReport);
    return stopConditionViolation;
}

QRegularExpression reporting::SpinErrorParser::buildSpinErrorRegex()
{
    // error number
    QString pattern = QStringLiteral("pan:(\\d+):\\s+");
    // error type
    pattern += QStringLiteral("(.+?)\\s+");
    // error details
    pattern += QStringLiteral("\\((.+?)\\)\\s+");
    // error depth
    pattern += QStringLiteral("\\(at depth (\\d+)\\)");
    return QRegularExpression(pattern);
}

QRegularExpression reporting::SpinErrorParser::buildDataConstraintViolationRegex()
{
    // opening parenthesis
    QString pattern = QStringLiteral("\\(");
    // variable name
    pattern += QStringLiteral("([a-z_][\\w\\.]+)");
    // operator
    pattern += QStringLiteral("(.+?)");
    // bounding value
    pattern += QStringLiteral("([\\d\\.]+)");
    // closing parenthesis
    pattern += QStringLiteral("\\)");
    return QRegularExpression(pattern);
}

QRegularExpression reporting::SpinErrorParser::buildStopConditionViolationRegex()
{
    QString pattern;
    return QRegularExpression(pattern);
}

void reporting::SpinErrorParser::parseVariableName(
        const QString &variable, reporting::DataConstraintViolationReport &violationReport)
{
    static const QString nestedStateSeparator = QStringLiteral("_0_");
    const auto tokens = variable.split(QChar('.'));
    violationReport.functionName = tokens.at(VariableParseTokens::VariableFunction);
    // try to split last token into nested state and proper variable name
    if (tokens.at(VariableParseTokens::VariableName).contains(nestedStateSeparator)) {
        const auto nestedTokens = tokens.at(VariableParseTokens::VariableName).split(nestedStateSeparator);
        violationReport.variableName = nestedTokens.last();
        violationReport.nestedStateName = nestedTokens.first();
    } else {
        // name doesn't contain a nested state separator, indicating a nested state doesn't exist
        violationReport.variableName = tokens.at(VariableParseTokens::VariableName);
        violationReport.nestedStateName = QString();
    }
}

const QString reporting::SpinErrorParser::m_spinNoTrailFileMessage = QStringLiteral("spin: cannot find trail file");
