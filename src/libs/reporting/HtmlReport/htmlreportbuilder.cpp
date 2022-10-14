#include "htmlreportbuilder.h"

#include <QFileInfo>
#include <grantlee/outputstream.h>
#include <grantlee_templates.h>
#include <reporting/Report/dataconstraintviolationreport.h>

reporting::HtmlReportBuilder::HtmlReportBuilder()
{
    // initialize file loader relative to the root path
    m_fileLoader = QSharedPointer<Grantlee::FileSystemTemplateLoader>::create();
    m_fileLoader->setTemplateDirs({ "/" });

    m_engine = new Grantlee::Engine(this);
    m_engine->addTemplateLoader(m_fileLoader);
}

QString reporting::HtmlReportBuilder::parse(const SpinErrorReport &spinErrorReport, const QString &templateFile) const
{
    // get absolute path for template file
    const QFileInfo templateFileInfo(templateFile);
    const auto templateFileAbs = templateFileInfo.absoluteFilePath();

    const Grantlee::Template stringTemplate = m_engine->loadByName(templateFileAbs);
    const auto reportVariantList = buildReportVariant(spinErrorReport);

    QVariantHash mapping;
    mapping.insert("report", reportVariantList);

    Grantlee::Context context(mapping);
    const auto html = stringTemplate->render(&context);
    return html;
}

QVariantList reporting::HtmlReportBuilder::buildReportVariant(const reporting::SpinErrorReport &spinErrorReport)
{
    QVariantList variantList;
    for (const auto &reportItem : spinErrorReport) {
        const auto variantHash = buildReportItemVariant(reportItem);
        variantList.append(variantHash);
    }
    return variantList;
}

QVariantHash reporting::HtmlReportBuilder::buildReportItemVariant(
        const reporting::SpinErrorReportItem &spinErrorReportItem)
{
    QVariantHash variantHash;
    variantHash.insert("errorNumber", spinErrorReportItem.errorNumber);
    variantHash.insert("errorDepth", spinErrorReportItem.errorDepth);
    variantHash.insert("rawErrorDetails", spinErrorReportItem.rawErrorDetails);

    // resolve error type as string
    variantHash.insert("errorType",
            m_errorTypeNames.value(
                    spinErrorReportItem.errorType, m_errorTypeNames[reporting::SpinErrorReportItem::OtherError]));

    // resolve parsed error details depending on error type
    QVariant variantErrorDetails;
    switch (spinErrorReportItem.errorType) {
    case reporting::SpinErrorReportItem::DataConstraintViolation:
        variantErrorDetails = buildDataConstraintViolationVariant(spinErrorReportItem.parsedErrorDetails);
        break;
    default:
        break;
    }
    variantHash.insert("errorDetails", variantErrorDetails);
    return variantHash;
}

QVariantHash reporting::HtmlReportBuilder::buildDataConstraintViolationVariant(const QVariant &errorDetails)
{
    reporting::DataConstraintViolationReport report =
            qvariant_cast<reporting::DataConstraintViolationReport>(errorDetails);
    QVariantHash variantHash;
    variantHash.insert("variableName", report.variableName);

    QVariantList variantConstraints;
    for (const auto &constraint : report.constraints) {
        variantConstraints.append(constraint);
    }

    QVariantList variantBoundingValues;
    for (const auto &boundingValue : report.boundingValues) {
        variantBoundingValues.append(boundingValue);
    }

    // count number of constraints for template iterator
    variantHash.insert("constraintCount", qMin(variantConstraints.size(), variantBoundingValues.size()));

    variantHash.insert("constraints", variantConstraints);
    variantHash.insert("boundingValues", variantBoundingValues);
    return variantHash;
}

const QHash<reporting::SpinErrorReportItem::ErrorType, QString> reporting::HtmlReportBuilder::m_errorTypeNames = {
    { reporting::SpinErrorReportItem::DataConstraintViolation, "Data Constraint Violation" },
    { reporting::SpinErrorReportItem::StopConditionViolation, "Stop Condition Violation" },
    { reporting::SpinErrorReportItem::ObserverFailure, "Observer Failure" },
    { reporting::SpinErrorReportItem::OtherError, "Unknown Error" }
};
