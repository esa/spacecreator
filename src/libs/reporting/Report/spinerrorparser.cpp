#include "spinerrorparser.h"

#include <QDebug>
#include <QRegularExpression>

reporting::SpinErrorReport reporting::SpinErrorParser::parse(const QString &spinMessage) const
{
    QRegularExpressionMatchIterator matches = matchSpinErrors(spinMessage);
    reporting::SpinErrorReport report;
    while (matches.hasNext()) {
        auto reportItem = buildReportItem(matches.next());
        report.append(reportItem);
    }
    return report;
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
    default:
        break;
    }
    return reportItem;
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
