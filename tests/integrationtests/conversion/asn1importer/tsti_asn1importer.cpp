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

#include <QObject>
#include <QtTest>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/sequence.h>
#include <conversion/asn1/Asn1Importer/importer.h>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/options.h>

using Asn1Acn::Asn1Model;
using conversion::Options;
using conversion::asn1::Asn1Options;
using conversion::asn1::importer::Asn1Importer;
using conversion::importer::ImportException;

namespace conversion::asn1::test {

class tsti_Asn1Importer : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testValid();
    void testInvalid();
};

void tsti_Asn1Importer::testValid()
{
    Options options;
    options.add(Asn1Options::inputFilepath, "Valid.asn");

    Asn1Importer asn1Importer;

    try {
        const auto model = asn1Importer.importModel(options);
        QVERIFY(model);
        const auto *asn1Model = dynamic_cast<Asn1Model *>(model.get());
        QVERIFY(asn1Model);

        const auto &files = asn1Model->data();
        QCOMPARE(files.size(), 1);

        const auto &definitionsList = files.at(0)->definitionsList();
        QCOMPARE(definitionsList.size(), 1);

        const auto &definitions = definitionsList.at(0);
        const auto &types = definitions->types();
        QCOMPARE(types.size(), 2);

        const auto &sequenceTypeAssignment = types.at(0);
        QVERIFY(sequenceTypeAssignment);
        const auto *sequenceType = sequenceTypeAssignment->type();
        QVERIFY(sequenceType);
        QVERIFY(dynamic_cast<const Asn1Acn::Types::Sequence *>(sequenceType));

        const auto &enumTypeAssignment = types.at(1);
        QVERIFY(enumTypeAssignment);
        const auto *enumType = enumTypeAssignment->type();
        QVERIFY(enumType);
        QVERIFY(dynamic_cast<const Asn1Acn::Types::Enumerated *>(enumType));
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tsti_Asn1Importer::testInvalid()
{
    Options options;
    options.add(Asn1Options::inputFilepath, "Invalid.asn");

    Asn1Importer asn1Importer;
    QVERIFY_EXCEPTION_THROWN(asn1Importer.importModel(options), ImportException);
}

} // namespace conversion::asn1::test

QTEST_MAIN(conversion::asn1::test::tsti_Asn1Importer)

#include "tsti_asn1importer.moc"
