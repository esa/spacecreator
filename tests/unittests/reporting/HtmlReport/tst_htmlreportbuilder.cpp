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

#include "tst_htmlreportbuilder.h"

#include <reporting/HtmlReport/htmlreportbuilder.h>
#include <reporting/Report/spinerrorparser.h>

using namespace reporting;

namespace Report::test {

void tst_HtmlReportBuilder::testNoError()
{
    const QString spinMessagePath("resources/spin_no_error_output.txt");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/result_empty.html");

    const QString spinMessage = readFile(spinMessagePath);
    const QString spinTraces = QString();
    const QString sclConditions = QString();
    const QString htmlResult = readFile(htmlResultPath);

    const SpinErrorParser parser;
    auto reports = parser.parse(spinMessage, spinTraces, sclConditions);

    const HtmlReportBuilder htmlReportBuilder;
    const auto html = htmlReportBuilder.buildHtmlReport(reports, htmlTemplatePath);
    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testDataConstraintViolation()
{
    const QString spinMessagePath("resources/spin_error_output.txt");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/result_error.html");

    const QString spinMessage = readFile(spinMessagePath);
    const QString spinTraces = QString();
    const QString sclConditions = QString();
    const QString htmlResult = readFile(htmlResultPath);

    const SpinErrorParser parser;
    auto reports = parser.parse(spinMessage, spinTraces, sclConditions);

    const HtmlReportBuilder htmlReportBuilder;
    const auto html = htmlReportBuilder.buildHtmlReport(reports, htmlTemplatePath);

    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testDataConstraintViolationNested()
{
    const QString spinMessagePath("resources/spin_nested_error_output.txt");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/result_nested.html");

    const QString spinMessage = readFile(spinMessagePath);
    const QString spinTraces = QString();
    const QString sclConditions = QString();
    const QString htmlResult = readFile(htmlResultPath);

    const SpinErrorParser parser;
    auto reports = parser.parse(spinMessage, spinTraces, sclConditions);

    const HtmlReportBuilder htmlReportBuilder;
    const auto html = htmlReportBuilder.buildHtmlReport(reports, htmlTemplatePath);
    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testStopConditionViolationEmpty()
{
    const QString spinMessagePath("resources/spin_scv_empty_output.txt");
    const QString spinTracesPath("resources/spin_scv_empty_trails.txt");
    const QString spinSclConditionsPath("resources/spin_scv_empty_scl.txt");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/result_scv_empty.html");

    const QString spinMessage = readFile(spinMessagePath);
    const QString spinTraces = readFile(spinTracesPath);
    const QString sclConditions = readFile(spinSclConditionsPath);
    const QString htmlResult = readFile(htmlResultPath);

    const SpinErrorParser parser;
    auto reports = parser.parse(spinMessage, spinTraces, sclConditions);

    const HtmlReportBuilder htmlReportBuilder;
    const auto html = htmlReportBuilder.buildHtmlReport(reports, htmlTemplatePath);
    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testStopConditionViolationGetState()
{
    const QString spinMessagePath("resources/spin_scv_get_state_output.txt");
    const QString spinTracesPath("resources/spin_scv_get_state_trails.txt");
    const QString spinSclConditionsPath("resources/spin_scv_get_state_scl.txt");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/result_scv_get_state.html");

    const QString spinMessage = readFile(spinMessagePath);
    const QString spinTraces = readFile(spinTracesPath);
    const QString sclConditions = readFile(spinSclConditionsPath);
    const QString htmlResult = readFile(htmlResultPath);

    const SpinErrorParser parser;
    auto reports = parser.parse(spinMessage, spinTraces, sclConditions);

    const HtmlReportBuilder htmlReportBuilder;
    const auto html = htmlReportBuilder.buildHtmlReport(reports, htmlTemplatePath);

    QFile file("/home/taste/result_scv_get_state.html");
    file.open(QFile::WriteOnly);
    file.write(html.toUtf8());
    file.close();

    QVERIFY(true);
}

QString tst_HtmlReportBuilder::readFile(const QString &filepath)
{
    QFile file(filepath);
    if (file.exists() && file.open(QFile::ReadOnly | QFile::Text)) {
        return QString(file.readAll());
    }

    qCritical("Unable to open file");
    return QString();
}

} // namespace Report::test
