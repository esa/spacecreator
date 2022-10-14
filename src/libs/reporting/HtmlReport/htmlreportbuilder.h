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
#include <reporting/Report/spinerrorreportitem.h>

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
     * @brief   Parse spin error report
     *
     * @param   spinErrorReport      Parsed error report from spin
     *
     * @return  Error report in an HTML document format
     */
    QString parse(const SpinErrorReport &spinErrorReport, const QString &templateFile) const;

private:
    Grantlee::Engine *m_engine = nullptr;
    QSharedPointer<Grantlee::FileSystemTemplateLoader> m_fileLoader;

    static const QHash<reporting::SpinErrorReportItem::ErrorType, QString> m_errorTypeNames;

    static QVariantList buildReportVariant(const SpinErrorReport &spinErrorReport);
    static QVariantHash buildReportItemVariant(const SpinErrorReportItem &spinErrorReportItem);

    static QVariantHash buildDataConstraintViolationVariant(const QVariant &errorDetails);
};

}
