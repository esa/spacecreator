#include "spinerrorparser.h"

#include <QDebug>
#include <QRegularExpression>

QList<reporting::SpinErrorReport> reporting::SpinErrorParser::parse(const QString &spinMessage) const
{
    // match message report
    QRegularExpressionMatchIterator matches = findErrors(spinMessage);
    // iterate over all reports
    QList<SpinErrorReport> reports;
    while (matches.hasNext()) {
        // build report
        auto report = buildReport(matches.next());
    }
    return reports;
}

QRegularExpressionMatchIterator reporting::SpinErrorParser::findErrors(const QString &spinMessage) const
{
    const QString pattern = QStringLiteral("pan:(\\d+):\\s+(.+?)\\s+\\((.+?)\\)\\s+\\(at depth (\\d+)\\)\\n");
    const QRegularExpression regex(pattern);
    return regex.globalMatch(spinMessage);
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
