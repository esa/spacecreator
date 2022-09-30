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

QRegularExpressionMatchIterator reporting::SpinErrorParser::findVariableViolations(const QString &) const
{
    return QRegularExpressionMatchIterator();
}

reporting::SpinErrorReport reporting::SpinErrorParser::buildReport(const QRegularExpressionMatch &matchedError) const
{
    SpinErrorReport report;
    report.errorNumber = matchedError.captured(1).toUInt();
    report.errorDepth = matchedError.captured(4).toUInt();
    report.errorType = SpinErrorReport::DataConstraintViolation;
    report.rawErrorDetails = matchedError.captured(3);
    return report;
}
