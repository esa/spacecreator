#include "spinerrorparser.h"

#include <QDebug>
#include <QRegularExpression>

reporting::SpinErrorReport reporting::SpinErrorParser::parse(const QString &spinMessage) const
{
    QRegularExpressionMatchIterator matches = matchSpinErrors(spinMessage);
    reporting::SpinErrorReport report;
    while (matches.hasNext()) {
        // build report
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
        // build violation report
        violationReport.variableName =
                matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationVariableName);
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
    // build report
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
