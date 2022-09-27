#include "spinerrorparser.h"

#include <QDebug>
#include <QRegularExpression>

QList<reporting::SpinErrorReport> reporting::SpinErrorParser::parse(const QString &spinMessage)
{
    // match message report
    QRegularExpressionMatchIterator matches = findErrors(spinMessage);
    while (matches.hasNext()) {
        // build report
    }

    QList<SpinErrorReport> reports;
    return reports;
}

QRegularExpressionMatchIterator reporting::SpinErrorParser::findErrors(const QString &spinMessage)
{
    const QString pattern = QStringLiteral("pan:(\\d+):\\s+(.+?)\\s+\\((.+?)\\)\\s+\\(at depth (\\d+)\\)\\n");
    const QRegularExpression regex(pattern);
    return regex.globalMatch(spinMessage);
}

reporting::SpinErrorReport reporting::SpinErrorParser::buildReport(const QRegularExpressionMatch &matchedError)
{
    SpinErrorReport report;
    report.errorNumber = matchedError.captured(1).toInt();
    return report;
}
