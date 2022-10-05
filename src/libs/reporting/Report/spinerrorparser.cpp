#include "spinerrorparser.h"

#include <QDebug>
#include <QRegularExpression>

QList<reporting::SpinErrorReport> reporting::SpinErrorParser::parse(const QString &spinMessage) const
{
    // match message report
    QRegularExpressionMatchIterator matches = findSpinErrors(spinMessage);
    // iterate over all reports
    QList<SpinErrorReport> reports;
    while (matches.hasNext()) {
        // build report
        auto report = buildReport(matches.next());
        reports.append(report);
    }
    return reports;
}

QRegularExpressionMatchIterator reporting::SpinErrorParser::findSpinErrors(const QString &spinMessage) const
{
    // error number
    QString pattern = QStringLiteral("pan:(\\d+):\\s+");
    // error type
    pattern += QStringLiteral("(.+?)\\s+");
    // erro details
    pattern += QStringLiteral("\\((.+?)\\)\\s+");
    // error depth
    pattern += QStringLiteral("\\(at depth (\\d+)\\)");

    const QRegularExpression regex(pattern);
    return regex.globalMatch(spinMessage);
}

QList<QVariant> reporting::SpinErrorParser::findVariableViolations(const QString &str) const
{
    QString pattern;
    // opening parenthesis
    pattern += QStringLiteral("\\(");
    // variable name
    pattern += QStringLiteral("([a-z_][\\w\\.]+)");
    // operator
    pattern += QStringLiteral("(.+?)");
    // bounding value
    pattern += QStringLiteral("([\\d\\.]+)");
    // closing parenthesis
    pattern += QStringLiteral("\\)");
    const QRegularExpression regex(pattern);

    QList<QVariant> violations;
    QRegularExpressionMatchIterator matches = regex.globalMatch(str);
    while (matches.hasNext()) {
        const QRegularExpressionMatch matchedError = matches.next();
        // build violation report
        DataConstraintViolationReport report;
        report.variableName = matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationVariableName);
        report.constraint = matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationType);
        report.boundingValue.setValue(
                matchedError.captured(ConstraintViolationParseTokens::ConstraintViolationBoundingValue).toInt());
        // add report to violations
        QVariant violation;
        violation.setValue(report);
        violations.append(violation);
    }
    return violations;
}

reporting::SpinErrorReport reporting::SpinErrorParser::buildReport(const QRegularExpressionMatch &matchedError) const
{
    // build report
    SpinErrorReport report;
    report.errorNumber = matchedError.captured(ReportParseTokens::ErrorNumber).toUInt();
    report.errorDepth = matchedError.captured(ReportParseTokens::ErrorDepth).toUInt();
    report.errorType = SpinErrorReport::DataConstraintViolation;
    report.rawErrorDetails = matchedError.captured(ReportParseTokens::ErrorDetails);
    // parse data constraint violation
    // for now, only
    switch (report.errorType) {
    case SpinErrorReport::DataConstraintViolation:
        report.parsedErrorDetails = findVariableViolations(report.rawErrorDetails);
        break;
    default:
        break;
    }
    return report;
}
