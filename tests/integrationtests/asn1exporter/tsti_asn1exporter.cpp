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

using Asn1Acn::Asn1Model;
using conversion::Options;
using conversion::asn1::Asn1Options;
using conversion::asn1::exporter::Asn1Exporter;

namespace asn1::test {

class tsti_Asn1Exporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanup();
    void testValid();
};

void tsti_Asn1Exporter::initTestCase() {}

void tsti_Asn1Exporter::cleanup()
{
    QFile file("output.asn");
    file.remove();
}

void tsti_Asn1Exporter::testValid()
{
    // first asn.1/acn file
    QFileInfo outputFileInfo("output.asn");
    QVERIFY(!outputFileInfo.exists());

    Asn1Acn::Types::Integer integer("integer");
    Asn1Acn::Definitions defs(QString("example1Definition1"), {});
    Asn1Acn::TypeAssignment assign1(
            QString("assignment1"), QString("cName_of_assign"), {}, std::make_unique<Asn1Acn::Types::Integer>(integer));
    Asn1Acn::SingleValue sv(QString("example integer's single value"));
    Asn1Acn::ValueAssignment va(QString("val_assignment1"), {}, std::make_unique<Asn1Acn::Types::Integer>(integer),
            std::make_unique<Asn1Acn::SingleValue>(sv));

    integer.setSize(4);
    defs.addType(std::make_unique<Asn1Acn::TypeAssignment>(assign1));
    defs.addValue(std::make_unique<Asn1Acn::ValueAssignment>(va));

    Asn1Acn::File file(outputFileInfo.fileName());
    file.add(std::make_unique<Asn1Acn::Definitions>(defs));

    // second asn.1/acn file
    QFileInfo outputFile2Info("output2.asn");
    QVERIFY(!outputFile2Info.exists());

    Asn1Acn::Types::Integer integer2("integer2");
    Asn1Acn::Definitions defs2(QString("exampleName"), {});
    Asn1Acn::TypeAssignment assign2(QString("name_of_assign2"), QString("cName_of_assign"), {},
            std::make_unique<Asn1Acn::Types::Integer>(integer2));

    Asn1Acn::SingleValue sv2(QString("1"));
    Asn1Acn::ValueAssignment va2(QString("val2_assignment"), {}, std::make_unique<Asn1Acn::Types::Integer>(integer2),
            std::make_unique<Asn1Acn::SingleValue>(sv2));

    defs2.addType(std::make_unique<Asn1Acn::TypeAssignment>(assign2));
    defs.addValue(std::make_unique<Asn1Acn::ValueAssignment>(va2));

    Asn1Acn::File file2(outputFile2Info.fileName());
    file2.add(std::make_unique<Asn1Acn::Definitions>(defs2));

    // asn1acn vector of files
    std::vector<Asn1Acn::File> files;
    files.push_back(file);
    files.push_back(file2);
    Asn1Model model(files);

    Options options;
    options.add(Asn1Options::asn1FilenamePrefix, "Asn1Pref_");
    options.add(Asn1Options::acnFilenamePrefix, "AcnPref_");

    Asn1Exporter asn1Exporter;
    try {
        asn1Exporter.exportModel(&model, options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    // TODO: check created files

    // TODO: remove created files
}

} // namespace asn1::test

QTEST_MAIN(asn1::test::tsti_Asn1Exporter)

#include "tsti_asn1exporter.moc"
