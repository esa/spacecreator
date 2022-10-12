#include "htmlreportbuilder.h"

QString reporting::HtmlReportBuilder::parse(const SpinErrorReport &spinErrorReport) const
{
    QString out;
    for (auto item : spinErrorReport) {
        out = item.rawErrorDetails;
    }
    return out;
}
