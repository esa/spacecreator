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
#include <QFile>
#include <QRegularExpression>

reporting::SpinErrorReport reporting::SpinErrorParser::parse(
        const QStringList &, const QStringList &sclFiles, const QList<RawErrorItem> &errors, const QStringList &) const
{
    reporting::SpinErrorReport report;

    // read scl files
    QStringList sclConditions;
    for (auto sclFile : sclFiles) {
        sclConditions.append(readFile(sclFile));
    }

    // number of errors is equal to the number of separate spin traces
    uint32_t errorIndex = 0;
    for (auto error : errors) {
        // read traces and scenario
        const auto spinTrace = readFile(error.spinTraceFile);
        const auto scenario = readFile(error.scenarioFile);

        SpinErrorReportItem newReportItem = parseTrace(spinTrace, sclFiles);
        newReportItem.errorNumber = errorIndex++;
        newReportItem.scenario = scenario;
        report.append(newReportItem);
    }

    return report;
}

reporting::SpinErrorReportItem reporting::SpinErrorParser::parseTrace(
        const QString &spinTraces, const QStringList &) const
{
    SpinErrorReportItem reportItem;
    auto stopConditionMatch = matchStopCondition(spinTraces);
    if (stopConditionMatch.hasMatch()) {
        auto violationString = stopConditionMatch.captured(ErrorDetailsMatch);
        auto report = parseStopConditionViolation(violationString);
        reportItem.errorDepth = 0;
        reportItem.errorType = SpinErrorReportItem::StopConditionViolation;
        reportItem.rawErrorDetails = stopConditionMatch.captured(RawErrorMatch).trimmed();
        reportItem.parsedErrorDetails = report;
        // found stop condition violation
        return reportItem;
    }
    auto observerFailureErrorStateMatch = matchObserverFailureErrorState(spinTraces);
    if (observerFailureErrorStateMatch.hasMatch()) {
        // found observer failure (error state)
        auto observerName = stopConditionMatch.captured(ErrorDetailsMatch);
        auto report = parseObserverFailureErrorState(observerName);
        reportItem.errorDepth = 0;
        reportItem.errorType = SpinErrorReportItem::ObserverFailure;
        reportItem.rawErrorDetails = observerFailureErrorStateMatch.captured(RawErrorMatch).trimmed();
        reportItem.parsedErrorDetails = report;
        return reportItem;
    }
    auto observerFailureSuccessStateMatch = matchObserverFailureSuccessState(spinTraces);
    if (observerFailureSuccessStateMatch.hasMatch()) {
        // found observer failure (success state)
        auto report = parseObserverFailureSuccessState(QString());
        reportItem.errorDepth = 0;
        reportItem.errorType = SpinErrorReportItem::ObserverFailure;
        reportItem.rawErrorDetails = observerFailureSuccessStateMatch.captured(RawErrorMatch).trimmed();
        reportItem.parsedErrorDetails = report;
        return reportItem;
    }
    auto variableViolationMatch = matchVariableViolation(spinTraces);
    if (variableViolationMatch.hasMatch()) {
        // found variable violation
        auto violationString = variableViolationMatch.captured(ErrorDetailsMatch);
        auto report = parseVariableViolation(violationString);
        reportItem.errorDepth = 0;
        reportItem.errorType = SpinErrorReportItem::DataConstraintViolation;
        reportItem.rawErrorDetails = variableViolationMatch.captured(RawErrorMatch).trimmed();
        reportItem.parsedErrorDetails = report;
        return reportItem;
    }
    // no error found - return unknown error
    return reportItem;
}

QRegularExpressionMatch reporting::SpinErrorParser::matchStopCondition(const QString &spinTraces) const
{
    // identification prefix
    QString pattern("SCL violation:\\s*");
    // stop condition match
    pattern += QString("(.+)\\n");

    const QRegularExpression regex(pattern);
    return regex.match(spinTraces);
}

QRegularExpressionMatch reporting::SpinErrorParser::matchObserverFailureErrorState(const QString &spinTraces) const
{
    // identification prefix
    QString pattern("Observer entered errorstate:\\s*");
    // observer failure match
    pattern += QString("(.+)\\n");

    const QRegularExpression regex(pattern);
    return regex.match(spinTraces);
}

QRegularExpressionMatch reporting::SpinErrorParser::matchObserverFailureSuccessState(const QString &spinTraces) const
{
    // identification string
    const QRegularExpression regex(QStringLiteral("<<<<<START OF CYCLE>>>>>"));
    return regex.match(spinTraces);
}

QRegularExpressionMatch reporting::SpinErrorParser::matchVariableViolation(const QString &spinTraces) const
{
    // identification prefix
    QString pattern("spin: text of failed assertion: ");
    // variable violation match
    pattern += QString("assert\\((.+)\\)\\n");

    const QRegularExpression regex(pattern);
    return regex.match(spinTraces);
}

reporting::SpinErrorReportItem reporting::SpinErrorParser::buildDataConstraintViolationReportItem(
        const QRegularExpressionMatch &matchedError) const
{
    SpinErrorReportItem reportItem;
    reportItem.errorNumber = matchedError.captured(ReportItemParseTokens::ErrorNumber).toUInt();
    reportItem.errorDepth = matchedError.captured(ReportItemParseTokens::ErrorDepth).toUInt();
    reportItem.errorType = SpinErrorReportItem::DataConstraintViolation;
    reportItem.rawErrorDetails = matchedError.captured(ReportItemParseTokens::ErrorDetails);
    reportItem.parsedErrorDetails = parseVariableViolation(reportItem.rawErrorDetails);
    return reportItem;
}

reporting::SpinErrorReportItem reporting::SpinErrorParser::buildStopConditionViolationReportItem(
        const QString &conditions) const
{
    SpinErrorReportItem reportItem;
    reportItem.errorNumber = 0;
    reportItem.errorType = SpinErrorReportItem::StopConditionViolation;
    reportItem.errorDepth = 0;
    reportItem.rawErrorDetails = conditions;
    reportItem.parsedErrorDetails = parseStopConditionViolation(conditions);
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

QVariant reporting::SpinErrorParser::parseStopConditionViolation(const QString &conditions) const
{
    StopConditionViolationReport violationReport;
    violationReport.violationType = StopConditionViolationReport::UnknownType;

    const QString sanitized = cleanUpSclComments(conditions);
    const QRegularExpression regex = buildStopConditionViolationRegex();
    auto matches = regex.globalMatch(sanitized);
    while (matches.hasNext()) {
        // parse clause and expressions
        const auto match = matches.next();
        const auto clause = match.captured(StopConditionParseTokens::StopConditionClause);
        const auto expressions = splitExpression(match.captured(StopConditionParseTokens::StopConditionExpression));
        // resolve violation clause
        violationReport.violationClause =
                m_stopConditionViolationClauses.value(clause, StopConditionViolationReport::UnknownClause);
        // check violation type can be matched in current expression
        const auto currentViolationType = resolveViolationType(expressions);
        if (currentViolationType != reporting::StopConditionViolationReport::UnknownType) {
            violationReport.violationType = currentViolationType;
            break;
        }
    }

    QVariant stopConditionViolation;
    stopConditionViolation.setValue(violationReport);
    return stopConditionViolation;
}

QVariant reporting::SpinErrorParser::parseObserverFailureErrorState(const QString &rawError) const
{
    ObserverFailureReport violationReport;
    violationReport.observerState = ObserverFailureReport::ErrorState;
    violationReport.observerName = rawError;

    QVariant observerFailure;
    observerFailure.setValue(violationReport);
    return observerFailure;
}

QVariant reporting::SpinErrorParser::parseObserverFailureSuccessState(const QString &) const
{
    ObserverFailureReport violationReport;
    violationReport.observerState = ObserverFailureReport::SuccessState;
    // observer name is currently not parsed in success states
    violationReport.observerName = QString();

    QVariant observerFailure;
    observerFailure.setValue(violationReport);
    return observerFailure;
}

QString reporting::SpinErrorParser::readFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.exists() && file.open(QFile::ReadOnly | QFile::Text)) {
        return QString(file.readAll());
    }
    return QString();
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
    QString pattern("(never|always|eventually|filter_out)\\s+(.+?);");
    return QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
}

QString reporting::SpinErrorParser::cleanUpSclComments(const QString &scl)
{
    QString sanitized;
    const auto lines = scl.split(QChar('\n'));
    for (auto line : lines) {
        sanitized.append(line.mid(0, line.indexOf(QStringLiteral("--"))) + " ");
    }
    return sanitized;
}

reporting::StopConditionViolationReport::ViolationType reporting::SpinErrorParser::resolveViolationType(
        const QStringList &expressions)
{
    QStringList identifiers;
    QRegularExpression regex("([a-z0-9_]+)\\s*\\(", QRegularExpression::CaseInsensitiveOption);
    for (const auto &expression : expressions) {
        auto matches = regex.globalMatch(expression);
        while (matches.hasNext()) {
            const auto match = matches.next();
            const auto identifier = (match.captured(IdentifierParseTokens::IdentifierName));
            if (m_stopConditionViolationTypes.contains(identifier)) {
                return m_stopConditionViolationTypes.value(identifier);
            }
        }
    }
    return reporting::StopConditionViolationReport::UnknownType;
}

QStringList reporting::SpinErrorParser::splitExpression(const QString &expression)
{
    return expression.split(QRegularExpression("\\s+(?:or|xor|and)\\s+", QRegularExpression::CaseInsensitiveOption));
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

const QHash<QString, reporting::StopConditionViolationReport::ViolationClause>
        reporting::SpinErrorParser::m_stopConditionViolationClauses = {
            { "never", reporting::StopConditionViolationReport::Never },
            { "always", reporting::StopConditionViolationReport::Always },
            { "eventually", reporting::StopConditionViolationReport::Eventually },
            { "filter_out", reporting::StopConditionViolationReport::FilterOut },
        };

const QHash<QString, reporting::StopConditionViolationReport::ViolationType>
        reporting::SpinErrorParser::m_stopConditionViolationTypes = {
            { "empty", reporting::StopConditionViolationReport::Empty },
            { "exist", reporting::StopConditionViolationReport::Exist },
            { "get_state", reporting::StopConditionViolationReport::GetState },
            { "length", reporting::StopConditionViolationReport::Length },
            { "queue_last", reporting::StopConditionViolationReport::QueueLast },
            { "queue_length", reporting::StopConditionViolationReport::QueueLength },
            { "present", reporting::StopConditionViolationReport::Present },
        };
