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

#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <csv/CsvImporter/csvimporter.h>
#include <csv/CsvModel/csvmodel.h>
#include <csv/CsvOptions/csvimporteroptions.h>
#include <csv/CsvOptions/options.h>
#include <memory>
#include <qtestcase.h>

using csv::CsvModel;
using csv::Field;
using csv::Row;
using csv::importer::CsvImporter;
using csv::importer::CsvOptions;
using csv::importer::Options;

namespace tests::csv {

class tst_csvimporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testEmpty();
    void testHeaderOnly();
    void testOneRowWithHeader();
    void testTwoRowsWithHeader();
    void testTwoRowsNoHeader();
    void testManyRowsWithHeader();
    void testManyRowsNoHeader();
};

static void checkExpectedFields(const std::vector<Field> &expected, const Row &row)
{
    QCOMPARE(row.fields().size(), expected.size());
    for (std::vector<Field>::size_type i = 0; i < expected.size(); i++) {
        QCOMPARE(expected[i], row.fields()[i]);
    }
}

static void checkExpectedRecords(const std::vector<std::vector<Field>> &expectedRecords, //
        const std::vector<std::unique_ptr<Row>> &actualRecords)
{
    QCOMPARE(actualRecords.size(), expectedRecords.size());
    for (std::vector<std::unique_ptr<Row>>::size_type i = 0; i < actualRecords.size(); i++) {
        for (std::vector<Field>::size_type j = 0; j < actualRecords[i]->fields().size(); j++) {
            QCOMPARE(actualRecords.at(i)->fields()[j], expectedRecords[i][j]);
        }
    }
}

void tst_csvimporter::testEmpty()
{
    Options options;
    options.add(CsvOptions::inputFilepath, "resources/empty.csv");

    const CsvImporter importer;
    const std::unique_ptr<CsvModel> csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);

    checkExpectedFields({}, csvModel->header());
    checkExpectedRecords({}, csvModel->records());
}

void tst_csvimporter::testHeaderOnly()
{
    Options options;
    options.add(CsvOptions::inputFilepath, "resources/header_only.csv");

    const CsvImporter importer;
    const std::unique_ptr<CsvModel> csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);

    checkExpectedFields({ "name", "2nd name", "date of birth", "address" }, csvModel->header());
    checkExpectedRecords({}, csvModel->records());
}

void tst_csvimporter::testOneRowWithHeader()
{
    Options options;
    options.add(CsvOptions::inputFilepath, "resources/one_row_with_header.csv");

    const CsvImporter importer;
    const std::unique_ptr<CsvModel> csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);

    checkExpectedFields({ "posX", "posY", "temp" }, csvModel->header());
    checkExpectedRecords({ { "1", "3.2", "-10.0" } }, csvModel->records());
}

void tst_csvimporter::testTwoRowsWithHeader()
{
    Options options;
    options.add(CsvOptions::inputFilepath, "resources/two_rows_with_header.csv");

    const CsvImporter importer;
    const std::unique_ptr<CsvModel> csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);

    checkExpectedFields({ "posX", "posY", "temp" }, csvModel->header());
    checkExpectedRecords(
            {
                    { "1", "3.2", "-10.0" },
                    { "10", "33", "3000" },
            },
            csvModel->records());
}

void tst_csvimporter::testTwoRowsNoHeader()
{
    Options options;
    options.add(CsvOptions::inputFilepath, "resources/two_rows_no_header.csv");

    const CsvImporter importer;
    const std::unique_ptr<CsvModel> csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);

    checkExpectedFields({}, csvModel->header());
    checkExpectedRecords(
            {
                    { "1", "3.2", "-10.0" },
                    { "10", "33", "3000" },
            },
            csvModel->records());
}

void tst_csvimporter::testManyRowsWithHeader()
{
    Options options;
    options.add(CsvOptions::inputFilepath, "resources/many_rows_with_header.csv");

    const CsvImporter importer;
    const std::unique_ptr<CsvModel> csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);

    checkExpectedFields({ "posX", "posY", "temp" }, csvModel->header());
    checkExpectedRecords(
            {
                    { "1", "3.2", "-10.0" },
                    { "10", "33", "3000" },
                    { "3425", "2543", "33" },
                    { "345", "23", "0" },
                    { "0", "0", "0" },
                    { "-1", "-1", "-1" },
            },
            csvModel->records());
}

void tst_csvimporter::testManyRowsNoHeader()
{
    Options options;
    options.add(CsvOptions::inputFilepath, "resources/many_rows_no_header.csv");

    const CsvImporter importer;
    const std::unique_ptr<CsvModel> csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);

    checkExpectedFields({}, csvModel->header());
    checkExpectedRecords(
            {
                    { "1", "3.2", "-10.0" },
                    { "10", "33", "3000" },
                    { "3425", "2543", "33" },
                    { "345", "23", "0" },
                    { "0", "0", "0" },
                    { "-1", "-1", "-1" },
            },
            csvModel->records());
}

} // namespace tests::csv

QTEST_MAIN(tests::csv::tst_csvimporter)

#include "tst_csvimporter.moc"
