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

reporting::SpinErrorReport reporting::SpinErrorParser::parse(const QStringList &, const QStringList &sclFiles,
        const QList<RawErrorItem> &errors, const QStringList &mscObserverFiles) const
{
    reporting::SpinErrorReport report;

    // read msc files
    QHash<QString, QString> mscObservers;
    for (auto mscObserverFile : mscObserverFiles) {
        const auto mscFileText = readFile(mscObserverFile);
        for (auto mscObserverName : parseMscObserver(mscFileText)) {
            mscObservers.insert(mscObserverName, mscObserverFile);
        }
    }

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
        // check for spin trace file indication (no error if not present)
        if (spinTrace.isEmpty() || spinTrace.trimmed() == m_spinNoTrailFileMessage) {
            continue;
        }

        SpinErrorReportItem newReportItem = parseTrace(spinTrace, sclConditions, mscObservers);
        newReportItem.errorNumber = errorIndex++;
        newReportItem.scenario = scenario;
        report.append(newReportItem);
    }

    return report;
}

reporting::SpinErrorReportItem reporting::SpinErrorParser::parseTrace(
        const QString &spinTraces, const QStringList &sclConditions, const QHash<QString, QString> &mscObservers) const
{
    SpinErrorReportItem reportItem;
    // an observer failure (entering success state) can be detected
    // by finding an acceptance cycle in the report, but it can also
    // indicate another stop condition violation with an "eventually" clause
    auto observerFailureSuccessStateMatch = matchObserverFailureSuccessState(spinTraces);
    if (observerFailureSuccessStateMatch.hasMatch()) {
        // try matching for "eventually" clause in scl
        const QString sclSanitized = cleanUpSclComments(sclConditions.join(QChar('\n')));
        const QRegularExpression sclRegex = buildStopConditionViolationRegex();
        auto sclMatches = sclRegex.globalMatch(sclSanitized);
        while (sclMatches.hasNext()) {
            auto sclMatch = sclMatches.next();
            auto clause = sclMatch.captured(StopConditionParseTokens::StopConditionClause);
            if (m_stopConditionViolationClauses.value(clause, reporting::StopConditionViolationReport::UnknownClause)
                    == reporting::StopConditionViolationReport::Eventually) {
                // eventually clause found in scl
                auto violationString = sclMatch.captured();
                auto report = parseStopConditionViolation(violationString);
                reportItem.errorDepth = 0;
                reportItem.errorType = SpinErrorReportItem::StopConditionViolation;
                reportItem.rawErrorDetails = violationString;
                reportItem.parsedErrorDetails = report;
                return reportItem;
            }
        }
        // no "eventually" clause found observer failure (interpreting as observer success state violation)
        auto report = parseObserverFailureSuccessState(QString());
        reportItem.errorDepth = 0;
        reportItem.errorType = SpinErrorReportItem::ObserverFailure;
        reportItem.rawErrorDetails = observerFailureSuccessStateMatch.captured(RawErrorMatch).trimmed();
        reportItem.parsedErrorDetails = report;
        return reportItem;
    }
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
        auto observerName = observerFailureErrorStateMatch.captured(ErrorDetailsMatch);
        // check for MSC observer
        if (mscObservers.contains(observerName)) {
            auto report = parseMscFailure(observerName, mscObservers.value(observerName));
            reportItem.errorDepth = 0;
            reportItem.errorType = SpinErrorReportItem::MscFailure;
            reportItem.rawErrorDetails = observerFailureErrorStateMatch.captured(RawErrorMatch).trimmed();
            reportItem.parsedErrorDetails = report;
            return reportItem;
        }
        auto report = parseObserverFailureErrorState(observerName);
        reportItem.errorDepth = 0;
        reportItem.errorType = SpinErrorReportItem::ObserverFailure;
        reportItem.rawErrorDetails = observerFailureErrorStateMatch.captured(RawErrorMatch).trimmed();
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

QVariant reporting::SpinErrorParser::parseVariableViolation(const QString &parsedErrorToken) const
{
    DataConstraintViolationReport violationReport;
    const QRegularExpression regex = buildDataConstraintViolationRegex();
    QRegularExpressionMatchIterator matches = regex.globalMatch(parsedErrorToken);
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
        const QString boundingValueTokenClean = removeParentheses(boundingValueToken);
        if (boundingValueToken.contains(QChar('.'))) {
            boundingValue.setValue(boundingValueTokenClean.toFloat());
        } else {
            boundingValue.setValue(boundingValueTokenClean.toInt());
        }
        violationReport.boundingValues.append(boundingValue);
    }

    QVariant variableViolation;
    variableViolation.setValue(violationReport);
    return variableViolation;
}

QVariant reporting::SpinErrorParser::parseStopConditionViolation(const QString &parsedErrorToken) const
{
    StopConditionViolationReport violationReport;
    violationReport.violationType = StopConditionViolationReport::UndefinedType;

    const QString sanitized = cleanUpSclComments(parsedErrorToken);
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
        if (currentViolationType != reporting::StopConditionViolationReport::UndefinedType) {
            violationReport.violationType = currentViolationType;
            break;
        }
    }

    QVariant stopConditionViolation;
    stopConditionViolation.setValue(violationReport);
    return stopConditionViolation;
}

QVariant reporting::SpinErrorParser::parseObserverFailureErrorState(const QString &parsedErrorToken) const
{
    ObserverFailureReport violationReport;
    violationReport.observerState = ObserverFailureReport::ErrorState;
    violationReport.observerName = parsedErrorToken;

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

QVariant reporting::SpinErrorParser::parseMscFailure(const QString &parsedErrorToken, const QString &mscFileName) const
{
    MscFailureReport violationReport;
    violationReport.observerName = parsedErrorToken;
    violationReport.mscFileName = mscFileName;

    QVariant mscFailure;
    mscFailure.setValue(violationReport);
    return mscFailure;
}

QString reporting::SpinErrorParser::readFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.exists() && file.open(QFile::ReadOnly | QFile::Text)) {
        return QString(file.readAll());
    }
    return QString();
}

QRegularExpression reporting::SpinErrorParser::buildDataConstraintViolationRegex()
{
    // opening parenthesis
    QString pattern = QStringLiteral("\\(");
    // variable name
    pattern += QStringLiteral("([a-z_][\\w\\.]+)");
    // operator
    pattern += QStringLiteral("(.+?)");
    // bounding value (number) - accounts for parentheses
    pattern += QStringLiteral("([-\\d\\.\\(\\)]+)");
    // closing parenthesis
    pattern += QStringLiteral("\\)");
    return QRegularExpression(pattern);
}

QRegularExpression reporting::SpinErrorParser::buildStopConditionViolationRegex()
{
    // clause
    QString pattern = QStringLiteral("(never|always|eventually|filter_out)\\s+");
    // conditions
    pattern += QStringLiteral("(.+?);");
    return QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
}

QRegularExpression reporting::SpinErrorParser::buildStripCommentsRegex()
{
    // match multi-line comments (/* ... */): opening token, comment text, closing token
    QString pattern = QStringLiteral("(?:\\/\\*");
    pattern += QStringLiteral("(?:[^\\*]|\\**[^\\*\\/])*");
    pattern += QStringLiteral("\\*+\\/)");
    // match single line comments
    pattern += QStringLiteral("|(?:\\/\\/[\\S ]*)");
    return QRegularExpression(pattern, QRegularExpression::MultilineOption);
}

QString reporting::SpinErrorParser::removeParentheses(const QString &numberToken)
{
    QString numberTokenClean = numberToken;
    numberTokenClean.replace(QString("("), QString());
    numberTokenClean.replace(QString(")"), QString());
    return numberTokenClean;
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
    QList<reporting::StopConditionViolationReport::ViolationType> foundViolationTypes;
    QRegularExpression regex("([a-z0-9_]+)\\s*\\(", QRegularExpression::CaseInsensitiveOption);
    for (const auto &expression : expressions) {
        auto matches = regex.globalMatch(expression);
        while (matches.hasNext()) {
            const auto match = matches.next();
            const auto identifier = (match.captured(IdentifierParseTokens::IdentifierName));
            if (m_stopConditionViolationTypes.contains(identifier)) {
                foundViolationTypes.append(m_stopConditionViolationTypes.value(identifier));
            }
        }
    }
    if (!foundViolationTypes.isEmpty()) {
        // most nested violation type
        return foundViolationTypes.last();
    }
    return reporting::StopConditionViolationReport::UndefinedType;
}

QStringList reporting::SpinErrorParser::splitExpression(const QString &expression)
{
    return expression.split(QRegularExpression("\\s+(?:or|xor|and)\\s+", QRegularExpression::CaseInsensitiveOption));
}

QStringList reporting::SpinErrorParser::parseMscObserver(const QString &mscFileText)
{
    // strip comments from a msc file
    QString mscFileTextStrip = mscFileText;
    const auto stringCommentsRegex = buildStripCommentsRegex();
    mscFileTextStrip.replace(stringCommentsRegex, QString());
    // match msc observer names in a file
    const QRegularExpression mscMatch("msc\\s+([a-zA-Z_][a-zA-Z0-9_]+)\\s*;", QRegularExpression::MultilineOption);
    auto matches = mscMatch.globalMatch(mscFileTextStrip);
    QStringList foundMscObservers;
    while (matches.hasNext()) {
        const auto match = matches.next();
        foundMscObservers.append(match.captured(MscObserverParseTokens::ObserverName));
    }
    return foundMscObservers;
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
