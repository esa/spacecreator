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

#include "testdriver.h"

#include <QDebug>
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <qtestcase.h>

namespace tests::testdriver {

class tst_testdriver : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testTestDriver();
};

void tst_testdriver::testTestDriver()
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

QTEST_MAIN(tests::testdriver::tst_testdriver)

#include "tst_testdriver.moc"
