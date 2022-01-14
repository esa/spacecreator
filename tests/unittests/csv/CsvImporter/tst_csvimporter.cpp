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
#include <conversion/common/export/exceptions.h>
#include <conversion/common/options.h>
#include <csv/CsvImporter/csvimporter.h>
#include <memory>
#include <qtestcase.h>

using conversion::ModelType;
using conversion::Options;
// using conversion::importer::ImportException;
// using csv::importer::CsvImporter;
// using tests::common::CsvFieldBuilder;

namespace tests::csv {

class tst_csvimporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void illFail();
};

void tst_csvimporter::illFail()
{
    QFAIL("this shall happen");
}

} // namespace tests::csv

QTEST_MAIN(tests::csv::tst_csvimporter)

#include "tst_csvimporter.moc"
