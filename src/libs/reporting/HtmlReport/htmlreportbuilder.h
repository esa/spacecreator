/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include <QSharedPointer>
#include <QString>
#include <reporting/Report/rawerroritem.h>
#include <reporting/Report/spinerrorreportitem.h>
#include <reporting/Report/stopconditionviolationreport.h>

namespace Grantlee {
class Engine;
class FileSystemTemplateLoader;
}

namespace reporting {

/**
 * @brief   Builds HTML document from SpinErrorReport.
 */
class HtmlReportBuilder : public QObject
{
public:
    /**
     * @brief   Constructor
     */
    HtmlReportBuilder();

    /**
     * @brief   Parse and build report using a default template file embedded into the library
     *
     * @param   spinMessages     Spin command outputs
     * @param   spinTraces       Spin traces
     * @param   sclConditions    SCL file conditions
     * @param   trails           Parsed trail file contents
     */
    QString parseAndBuildHtmlReport(const QList<RawErrorItem> &rawErrors) const;

    /**
     * @brief   Parse and build report using a default template file embedded into the library
     *
     * @param   spinMessages     Spin command outputs
     * @param   spinTraces       Spin traces
     * @param   sclConditions    SCL file conditions
     * @param   trails           Parsed trail file contents
     * @param   templateFile     Path to the HTML template file
     */
    QString parseAndBuildHtmlReport(const QList<RawErrorItem> &rawErrors, const QString &templateFile) const;

    /**
     * @brief   Build spin error report using a default template file embedded into the library
     *
     * @param   spinErrorReport      Parsed error report from spin
     * @param   trails               Parsed trail file contents
     *
     * @return  Error report in an HTML document format
     */
    QString buildHtmlReport(const SpinErrorReport &spinErrorReport) const;

    /**
     * @brief   Build spin error report using a custom template file
     *
     * @param   spinErrorReport      Parsed error report from spin
     * @param   trails               Parsed trail file contents
     * @param   templateFile         Path to the HTML template file
     *
     * @return  Error report in an HTML document format
     */
    QString buildHtmlReport(const SpinErrorReport &spinErrorReport, const QString &templateFile) const;

private:
    Grantlee::Engine *m_engine = nullptr;
    QSharedPointer<Grantlee::FileSystemTemplateLoader> m_fileLoader;

    QString loadTemplateFile(const QString &path) const;

    static const QString m_defaultTemplateFile;
    static const QHash<reporting::SpinErrorReportItem::ErrorType, QString> m_errorTypeNames;
    static const QHash<reporting::StopConditionViolationReport::ViolationType, QString>
            m_stopConditionViolationTypeNames;

    static QVariantList buildReportVariant(const SpinErrorReport &spinErrorReport);
    static QVariantHash buildReportItemVariant(const SpinErrorReportItem &spinErrorReportItem);

    static QVariantHash buildDataConstraintViolationVariant(const QVariant &errorDetails);
    static QVariantHash buildStopConditionViolationVariant(const QVariant &errorDetails);
};

}

// This function is required by Qt outside the reporting namespace,
// in order to initialize resource files associated with the library
inline void initResource()
{
    Q_INIT_RESOURCE(htmlreportbuilder);
}
