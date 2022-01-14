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

#include "csv/CsvModel/csvmodel.h"
#include "csv/CsvModel/row.h"
#include "modeltype.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/options.h>
#include <csv/CsvImporter/csvimporter.h>
#include <csv/CsvOptions/options.h>
#include <memory>
#include <qtestcase.h>

using conversion::ModelType;
using conversion::Options;
// using conversion::importer::ImportException;
using csv::CsvModel;
using csv::CsvOptions;
using csv::Field;
using csv::Row;
using csv::importer::CsvImporter;
// using tests::common::CsvFieldBuilder;

namespace tests::csv {

class tst_csvimporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testEmpty();
};

void tst_csvimporter::testEmpty()
{
    Options options;
    options.add(CsvOptions::inputFilepath, "resources/empty.csv");

    const CsvImporter importer;
    const std::unique_ptr<conversion::Model> model = importer.importModel(options);
    QVERIFY(model != nullptr);
    auto *const csvModel = dynamic_cast<CsvModel *>(model.get());
    QVERIFY(csvModel != nullptr);

    QCOMPARE(csvModel->modelType(), ModelType::Csv);
    QCOMPARE(csvModel->separator(), ",");

    const Row importedHeader = csvModel->header();
    QCOMPARE(importedHeader.fields().size(), 0);

    const std::vector<std::unique_ptr<Row>> &importedRecords = csvModel->records();
    QCOMPARE(importedRecords.size(), 0);
}

} // namespace tests::csv

QTEST_MAIN(tests::csv::tst_csvimporter)

#include "tst_csvimporter.moc"
