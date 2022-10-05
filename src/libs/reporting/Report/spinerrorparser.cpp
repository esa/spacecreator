#include "spinerrorparser.h"

#include <QDebug>
#include <QRegularExpression>

reporting::SpinErrorReport reporting::SpinErrorParser::parse(const QString &spinMessage) const
{
    QRegularExpressionMatchIterator matches = findSpinErrors(spinMessage);
    reporting::SpinErrorReport reports;
    while (matches.hasNext()) {
        // build report
        auto report = buildReport(matches.next());
        reports.append(report);
    }
    return reports;
}

QRegularExpressionMatchIterator reporting::SpinErrorParser::findSpinErrors(const QString &spinMessage) const
{
    const QRegularExpression regex = buildSpinErrorRegex();
    return regex.globalMatch(spinMessage);
}

QList<QVariant> reporting::SpinErrorParser::findVariableViolations(const QString &str) const
{
    qDebug() << "vv:" << str;

    const QRegularExpression regex = buildDataConstraintViolationRegex();
    QList<QVariant> violations;
    QRegularExpressionMatchIterator matches = regex.globalMatch(str);
    while (matches.hasNext()) {
        const QRegularExpressionMatch matchedError = matches.next();
        // build violation report
        DataConstraintViolationReport report;
        report.variableName = matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationVariableName);
        report.constraint = matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationType);
        // put either int or float value into the report
        const QString boundingValueToken =
                matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationBoundingValue);
        if (boundingValueToken.contains(QChar('.'))) {
            report.boundingValue.setValue(boundingValueToken.toFloat());
        } else {
            report.boundingValue.setValue(boundingValueToken.toInt());
        }
        // add report to violations
        QVariant violation;
        violation.setValue(report);
        violations.append(violation);
    }
    return violations;
}

reporting::SpinErrorReportItem reporting::SpinErrorParser::buildReport(
        const QRegularExpressionMatch &matchedError) const
{
    // build report
    SpinErrorReportItem report;
    report.errorNumber = matchedError.captured(ReportParseTokens::ErrorNumber).toUInt();
    report.errorDepth = matchedError.captured(ReportParseTokens::ErrorDepth).toUInt();
    report.errorType = SpinErrorReportItem::DataConstraintViolation;
    report.rawErrorDetails = matchedError.captured(ReportParseTokens::ErrorDetails);
    // parse data constraint violation
    // for now, only data constraint violation
    switch (report.errorType) {
    case SpinErrorReportItem::DataConstraintViolation:
        report.parsedErrorDetails = findVariableViolations(report.rawErrorDetails);
        break;
    default:
        break;
    }
    return report;
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
