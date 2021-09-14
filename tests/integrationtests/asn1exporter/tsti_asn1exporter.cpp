/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "definitions.h"
#include "typeassignment.h"
#include "types/integer.h"
#include "types/typereadingvisitor.h"
#include "value.h"
#include "valueassignment.h"

#include <QFileInfo>
#include <QObject>
#include <QTextStream>
#include <QUuid>
#include <QtTest>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/singlevalue.h>
#include <conversion/asn1/Asn1Exporter/Asn1Options/options.h>
#include <conversion/asn1/Asn1Exporter/exporter.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/options.h>
#include <memory>
#include <qtestcase.h>

using Asn1Acn::Asn1Model;
using conversion::Options;
using conversion::asn1::Asn1Options;
using conversion::asn1::exporter::Asn1Exporter;

namespace asn1::test {

class tsti_Asn1Exporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void cleanupTestCase();
    void testValid();

private:
    static const std::array<const QString, 4u> m_filenames;
};
const std::array<const QString, 4u> tsti_Asn1Exporter::m_filenames { "Acn_file1.acn", "Acn_file2.acn", "Asn1_file1.asn",
    "Asn1_file2.asn" };

void tsti_Asn1Exporter::cleanupTestCase()
{
    for (auto filename : m_filenames) {
        QFile(filename).remove();
    }
}

static Asn1Acn::Definitions CreateExampleDefs()
{
    Asn1Acn::Definitions defs("example1Definition", {});

    Asn1Acn::Types::Integer i("integer_id");
    i.setSize(4);
    Asn1Acn::TypeAssignment ta(
            "name_of_type_assignment", "cName_of_assignment", {}, std::make_unique<Asn1Acn::Types::Integer>(i));

    Asn1Acn::SingleValue sv("example integer's value");
    Asn1Acn::ValueAssignment va("name_of_value_assignment", {}, std::make_unique<Asn1Acn::Types::Integer>(i),
            std::make_unique<Asn1Acn::SingleValue>(sv));

    defs.addType(std::make_unique<Asn1Acn::TypeAssignment>(ta));
    defs.addValue(std::make_unique<Asn1Acn::ValueAssignment>(va));

    return defs;
}

static Asn1Acn::Definitions CreateAnotherExampleDefs()
{
    Asn1Acn::Definitions defs("anotherExampleDefinition", {});

    Asn1Acn::Types::Integer i("another_integer_id");
    Asn1Acn::TypeAssignment ta("name_of_another_type_assignment", "cName_of_another_assignment", {},
            std::make_unique<Asn1Acn::Types::Integer>(i));

    Asn1Acn::SingleValue sv("another_single_value");
    Asn1Acn::ValueAssignment va("name_of_another_value_assignment", {}, std::make_unique<Asn1Acn::Types::Integer>(i),
            std::make_unique<Asn1Acn::SingleValue>(sv));

    defs.addType(std::make_unique<Asn1Acn::TypeAssignment>(ta));
    defs.addValue(std::make_unique<Asn1Acn::ValueAssignment>(va));

    return defs;
}

QString getFileContents(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}

void tsti_Asn1Exporter::testValid()
{
    Asn1Acn::File file1("file1");
    auto defs1 = CreateExampleDefs();
    file1.add(std::make_unique<Asn1Acn::Definitions>(defs1));

    Asn1Acn::File file2("file2");
    auto defs2 = CreateAnotherExampleDefs();
    file2.add(std::make_unique<Asn1Acn::Definitions>(defs2));

    Asn1Model model(std::vector<Asn1Acn::File>({ file1, file2 }));

    Options options;
    options.add(Asn1Options::asn1FilepathPrefix, "Asn1_");
    options.add(Asn1Options::acnFilepathPrefix, "Acn_");

    Asn1Exporter asn1Exporter;
    try {
        asn1Exporter.exportModel(&model, options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    for (auto filename : m_filenames) {
        const QString expectedFilename = "expect_" + filename;
        QVERIFY(getFileContents(filename) == getFileContents(expectedFilename));
    }
}

} // namespace asn1::test

QTEST_MAIN(asn1::test::tsti_Asn1Exporter)

#include "tsti_asn1exporter.moc"
