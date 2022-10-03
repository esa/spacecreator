#include "spinerrorparser.h"

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
    const QString pattern = QStringLiteral("pan:(\\d+):\\s+(.+?)\\s+\\((.+?)\\)\\s+\\(at depth (\\d+)\\)");
    const QRegularExpression regex(pattern);
    return regex.globalMatch(spinMessage);
}

QList<QVariant> reporting::SpinErrorParser::findVariableViolations(const QString &str) const
{
    QList<QVariant> violations;
    const QString pattern = QStringLiteral("\\(([a-z_][\\w\\.]+)(.+?)(\\d+)\\)");
    const QRegularExpression regex(pattern);
    QRegularExpressionMatchIterator matches = regex.globalMatch(str);
    while (matches.hasNext()) {
        const QRegularExpressionMatch matchedError = matches.next();
        // build violation report
        DataConstraintViolationReport report;
        report.variableName = matchedError.captured(1);
        report.constraint = matchedError.captured(2);
        report.boundingValue = matchedError.captured(3).toInt();
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
    report.errorNumber = matchedError.captured(1).toUInt();
    report.errorDepth = matchedError.captured(4).toUInt();
    report.errorType = SpinErrorReport::DataConstraintViolation;
    report.rawErrorDetails = matchedError.captured(3);
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
