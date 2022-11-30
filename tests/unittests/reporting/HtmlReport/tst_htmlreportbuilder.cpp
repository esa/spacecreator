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
#include <reporting/Report/rawerroritem.h>
#include <reporting/Report/spinerrorparser.h>

using namespace reporting;

namespace Report::test {

void tst_HtmlReportBuilder::testNoError()
{
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/empty_result.html");

    const HtmlReportBuilder htmlBuilder;
    auto html = htmlBuilder.parseAndBuildHtmlReport({}, {}, {}, QStringList(), htmlTemplatePath);
    const auto htmlResult = readFile(htmlResultPath);

    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testDataConstraintViolation()
{
    const QString spinMessagePath("resources/data_constraint_msg.txt");
    const QString spinTracesPath("resources/data_constraint_spin.txt");
    const QString sclPath("resources/data_constraint_scl.txt");
    const QString scenarioPath("resources/scenario.sim");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/data_constraint_result.html");

    RawErrorItem rawError;
    rawError.spinTraceFile = spinTracesPath;
    rawError.scenarioFile = scenarioPath;

    const HtmlReportBuilder htmlBuilder;
    auto html = htmlBuilder.parseAndBuildHtmlReport(
            { spinMessagePath }, { sclPath }, { rawError }, QStringList(), htmlTemplatePath);
    const auto htmlResult = readFile(htmlResultPath);

    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testDataConstraintViolationNested()
{
    const QString spinMessagePath("resources/data_constraint_nested_msg.txt");
    const QString spinTracesPath("resources/data_constraint_nested_spin.txt");
    const QString sclPath("resources/data_constraint_nested_scl.txt");
    const QString scenarioPath("resources/scenario.sim");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/data_constraint_nested_result.html");

    RawErrorItem rawError;
    rawError.spinTraceFile = spinTracesPath;
    rawError.scenarioFile = scenarioPath;

    const HtmlReportBuilder htmlBuilder;
    auto html = htmlBuilder.parseAndBuildHtmlReport(
            { spinMessagePath }, { sclPath }, { rawError }, QStringList(), htmlTemplatePath);
    const auto htmlResult = readFile(htmlResultPath);

    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testStopConditionAlwaysEmpty()
{
    const QString spinMessagePath("resources/stop_condition_empty_msg.txt");
    const QString spinTracesPath("resources/stop_condition_empty_spin.txt");
    const QString sclPath("resources/stop_condition_empty_scl.txt");
    const QString scenarioPath("resources/scenario.sim");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/stop_condition_empty_result.html");

    RawErrorItem rawError;
    rawError.spinTraceFile = spinTracesPath;
    rawError.scenarioFile = scenarioPath;

    const HtmlReportBuilder htmlBuilder;
    auto html = htmlBuilder.parseAndBuildHtmlReport(
            { spinMessagePath }, { sclPath }, { rawError }, QStringList(), htmlTemplatePath);
    const auto htmlResult = readFile(htmlResultPath);

    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testStopConditionEventually()
{
    const QString spinMessagePath("resources/stop_condition_eventually_msg.txt");
    const QString spinTracesPath("resources/stop_condition_eventually_spin.txt");
    const QString sclPath("resources/stop_condition_eventually_scl.txt");
    const QString scenarioPath("resources/scenario.sim");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/stop_condition_eventually_result.html");

    RawErrorItem rawError;
    rawError.spinTraceFile = spinTracesPath;
    rawError.scenarioFile = scenarioPath;

    const HtmlReportBuilder htmlBuilder;
    auto html = htmlBuilder.parseAndBuildHtmlReport(
            { spinMessagePath }, { sclPath }, { rawError }, QStringList(), htmlTemplatePath);
    const auto htmlResult = readFile(htmlResultPath);

    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testStopConditionNever()
{
    const QString spinMessagePath("resources/stop_condition_never_msg.txt");
    const QString spinTracesPath("resources/stop_condition_never_spin.txt");
    const QString sclPath("resources/stop_condition_never_scl.txt");
    const QString scenarioPath("resources/scenario.sim");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/stop_condition_never_result.html");

    RawErrorItem rawError;
    rawError.spinTraceFile = spinTracesPath;
    rawError.scenarioFile = scenarioPath;

    const HtmlReportBuilder htmlBuilder;
    auto html = htmlBuilder.parseAndBuildHtmlReport(
            { spinMessagePath }, { sclPath }, { rawError }, QStringList(), htmlTemplatePath);
    const auto htmlResult = readFile(htmlResultPath);

    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testObserverFailureErrorState()
{
    const QString spinMessagePath("resources/obs_failure_error_state_msg.txt");
    const QString spinTracesPath("resources/obs_failure_error_state_spin.txt");
    const QString sclPath("resources/obs_failure_error_state_scl.txt");
    const QString scenarioPath("resources/scenario.sim");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/obs_failure_error_state_result.html");

    RawErrorItem rawError;
    rawError.spinTraceFile = spinTracesPath;
    rawError.scenarioFile = scenarioPath;

    const HtmlReportBuilder htmlBuilder;
    auto html = htmlBuilder.parseAndBuildHtmlReport(
            { spinMessagePath }, { sclPath }, { rawError }, QStringList(), htmlTemplatePath);
    const auto htmlResult = readFile(htmlResultPath);

    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testObserverFailureSuccessState()
{
    const QString spinMessagePath("resources/obs_failure_success_state_msg.txt");
    const QString spinTracesPath("resources/obs_failure_success_state_spin.txt");
    const QString sclPath("resources/obs_failure_success_state_scl.txt");
    const QString scenarioPath("resources/scenario.sim");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/obs_failure_success_state_result.html");

    RawErrorItem rawError;
    rawError.spinTraceFile = spinTracesPath;
    rawError.scenarioFile = scenarioPath;

    const HtmlReportBuilder htmlBuilder;
    auto html = htmlBuilder.parseAndBuildHtmlReport(
            { spinMessagePath }, { sclPath }, { rawError }, QStringList(), htmlTemplatePath);
    const auto htmlResult = readFile(htmlResultPath);

    QVERIFY(html == htmlResult);
}

void tst_HtmlReportBuilder::testMscFailure()
{
    const QString spinMessagePath("resources/msc_failure_msg.txt");
    const QString spinTracesPath("resources/msc_failure_spin.txt");
    const QString sclPath("resources/msc_failure_scl.txt");
    const QString scenarioPath("resources/scenario.sim");
    const QString mscPath("resources/msc_failure_mscdemo.msc");
    const QString htmlTemplatePath("resources/template.html");
    const QString htmlResultPath("resources/msc_failure_result.html");

    RawErrorItem rawError;
    rawError.spinTraceFile = spinTracesPath;
    rawError.scenarioFile = scenarioPath;

    const HtmlReportBuilder htmlBuilder;
    auto html = htmlBuilder.parseAndBuildHtmlReport(
            { spinMessagePath }, { sclPath }, { rawError }, { mscPath }, htmlTemplatePath);
    const auto htmlResult = readFile(htmlResultPath);

    QFile f("/home/taste/msc_failure_result.html");
    f.open(QFile::WriteOnly);
    f.write(html.toUtf8());
    f.close();

    QVERIFY(html == htmlResult);
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
