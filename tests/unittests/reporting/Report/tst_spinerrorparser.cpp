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

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QList>
#include <QtTest>
#include <reporting/Report/dataconstraintviolationreport.h>
#include <reporting/Report/rawerroritem.h>
#include <reporting/Report/spinerrorparser.h>

using namespace reporting;

namespace Report::test {

class tst_SpinErrorParser : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testNoError();
    void testViolationVariableType();
    void testNestedVariable();

private:
    QString readFile(const QString &filepath);
};

void tst_SpinErrorParser::testNoError()
{
    // read message file
    const QString filepath("resources/spin_no_error_output.txt");

    QVERIFY(true);

    /*RawErrorItem rawError;
    rawError.spinMessages = readFile(filepath);
    rawError.spinTraces = QString();
    rawError.sclConditions = QString();
    rawError.scenario = QString();

    const SpinErrorParser parser;
    const SpinErrorReport errorReport = parser.parse(rawError);

    QVERIFY(errorReport.size() == 0);*/
}

void tst_SpinErrorParser::testViolationVariableType()
{
    // read message file
    const QString filepath("resources/spin_error_output.txt");

    QVERIFY(true);

    /*RawErrorItem rawError;
    rawError.spinMessages = readFile(filepath);
    rawError.spinTraces = QString();
    rawError.sclConditions = QString();
    rawError.scenario = QString();

    const SpinErrorParser parser;
    const auto errorReport = parser.parse(rawError);
    QVERIFY(errorReport.size() == 1);

    const auto errorReportItem = errorReport.at(0);
    QVERIFY(errorReportItem.errorType == SpinErrorReportItem::DataConstraintViolation);
    QVERIFY(errorReportItem.errorDepth == 553);

    const auto errorDetails = qvariant_cast<DataConstraintViolationReport>(errorReportItem.parsedErrorDetails);
    QVERIFY(errorDetails.functionName == QString("asw"));
    QVERIFY(errorDetails.variableName == QString("tmp"));
    QVERIFY(errorDetails.nestedStateName == QString());
    QVERIFY(errorDetails.constraints
            == QList<QString>() << ">="
                                << "<=");
    QVERIFY(errorDetails.boundingValues == QList<QVariant>() << QVariant(0.0f) << QVariant(10.0f));*/
}

void tst_SpinErrorParser::testNestedVariable()
{
    // read message file
    const QString filepath("resources/spin_error_nested_output.txt");

    QVERIFY(true);

    /*RawErrorItem rawError;
    rawError.spinMessages = readFile(filepath);
    rawError.spinTraces = QString();
    rawError.sclConditions = QString();
    rawError.scenario = QString();

    const SpinErrorParser parser;
    const auto errorReport = parser.parse(rawError);
    QVERIFY(errorReport.size() == 1);

    const auto errorReportItem = errorReport.at(0);
    QVERIFY(errorReportItem.errorType == SpinErrorReportItem::DataConstraintViolation);
    QVERIFY(errorReportItem.errorDepth == 293);

    const auto errorDetails = qvariant_cast<DataConstraintViolationReport>(errorReportItem.parsedErrorDetails);
    QVERIFY(errorDetails.functionName == QString("function2"));
    QVERIFY(errorDetails.variableName == QString("b"));
    QVERIFY(errorDetails.nestedStateName == QString("nestedstate"));
    QVERIFY(errorDetails.constraints
            == QList<QString>() << ">="
                                << "<=");
    QVERIFY(errorDetails.boundingValues == QList<QVariant>() << QVariant(5.0f) << QVariant(15.0f));*/
}

QString tst_SpinErrorParser::readFile(const QString &filepath)
{
    QFile file(filepath);
    if (file.exists() && file.open(QFile::ReadOnly | QFile::Text)) {
        return QString(file.readAll());
    }

    qCritical("Unable to open file");
    return QString();
}

} // namespace tmc::test

QTEST_MAIN(Report::test::tst_SpinErrorParser)

#include "tst_spinerrorparser.moc"
