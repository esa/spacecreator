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
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <csv/CsvImporter/csvimporter.h>
#include <csv/CsvOptions/options.h>
#include <qtestcase.h>
#include <testgenerator/TestGenerator.h>

using testgenerator::TestGenerator;

namespace tests::testgenerator {

class tst_testgenerator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testEmpty();
    void testNominal();
};

void tst_testgenerator::testEmpty()
{
    csv::importer::CsvImporter importer;
    csv::importer::Options options;
    options.add(csv::importer::CsvOptions::separator, ",");
    options.add(csv::importer::CsvOptions::inputFilepath, "resources/empty.csv");

    auto csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);
    QVERIFY(csvModel->header().fields().empty());

    TestGenerator generator;
    (void)generator;
    // TODO: verify that generator with empty test data throws an exception
}

void tst_testgenerator::testNominal()
{
    csv::importer::CsvImporter importer;
    csv::importer::Options options;
    options.add(csv::importer::CsvOptions::separator, ",");
    options.add(csv::importer::CsvOptions::inputFilepath, "resources/test_data.csv");

    auto csvModel = importer.importModel(options);
    QVERIFY(csvModel != nullptr);
    QVERIFY(!csvModel->header().fields().empty());

    QString headerText = "";
    const auto &headerFields = csvModel->header().fields();
    std::for_each(headerFields.begin(), headerFields.end(),
            [&](const auto &field) { //
                headerText = QString("%1, %2").arg(headerText, field);
            });
    qDebug() << "header" << headerText;

    // TODO: verify that generator with correct inputs creates a correct source file
    // - csv data read
    // - function's interface drom interfaceview (protected or unprotected)
}

// TODO: test case to verify that the generator throws an exception on cyclic interface

// TODO: test case to verify that the generator throws an exception on sporadic interface

// TODO: test case to verify that the generator throws an exception on interface with implementation
//       NOT in C

// TODO:

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_testgenerator)

#include "tst_testgenerator.moc"
