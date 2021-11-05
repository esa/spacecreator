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
#include <iostream>
#include <promela/PromelaExporter/promelaexporter.h>
#include <promela/PromelaModel/promelamodel.h>
#include <promela/PromelaOptions/options.h>

namespace tmc::test {
using conversion::Options;
using conversion::promela::PromelaOptions;
using promela::exporter::PromelaExporter;
using promela::model::ArrayType;
using promela::model::BasicType;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::MtypeRef;
using promela::model::NamedMtype;
using promela::model::PromelaModel;
using promela::model::TypeAlias;
using promela::model::UnsignedDataType;
using promela::model::Utype;
using promela::model::UtypeRef;
using promela::model::ValueDefinition;

class tst_PromelaExporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testMtype();
    void testNamedMtype();
    void testUtype();
    void testUtypeVisibility();
    void testUtypeUnion();
    void testTypeAlias();
    void testValueDefinition();
    void testDeclaration();

private:
    QString getFileContents(const QString &filename);
    QString generatePromelaFromModel(const PromelaModel &model);
    void showInfo(const QString &returned, const QString &expected);
};

void tst_PromelaExporter::initTestCase() {}

void tst_PromelaExporter::cleanupTestCase() {}

void tst_PromelaExporter::testMtype()
{
    PromelaModel model;

    model.addMtype("TYPE1");
    model.addMtype("TYPE2");
    model.addMtype("TYPE3");

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_file1.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testNamedMtype()
{
    PromelaModel model;

    NamedMtype mtype1("states");
    mtype1.addValue("STATE_A");
    mtype1.addValue("STATE_B");
    mtype1.addValue("STATE_C");

    NamedMtype mtype2("formats");
    mtype2.addValue("FORMAT_3");
    mtype2.addValue("FORMAT_2");
    mtype2.addValue("FORMAT_1");

    model.addNamedMtype(mtype1);
    model.addNamedMtype(mtype2);

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_file2.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testUtype()
{
    PromelaModel model;

    model.addMtype("UNNAMED_A");
    model.addMtype("UNNAMED_B");

    NamedMtype named_mtype("states");
    named_mtype.addValue("STATE_A");
    named_mtype.addValue("STATE_B");
    named_mtype.addValue("STATE_C");

    model.addNamedMtype(named_mtype);

    Utype basic_types("basic_types");

    basic_types.addField(Declaration(DataType(BasicType::BIT), "field_one"));
    basic_types.addField(Declaration(DataType(BasicType::BOOLEAN), "field_two"));
    basic_types.addField(Declaration(DataType(BasicType::BYTE), "field_three"));
    basic_types.addField(Declaration(DataType(BasicType::PID), "field_four"));
    basic_types.addField(Declaration(DataType(BasicType::SHORT), "field_five"));
    basic_types.addField(Declaration(DataType(BasicType::INT), "field_six"));
    basic_types.addField(Declaration(DataType(BasicType::FLOAT), "field_seven"));
    basic_types.addField(Declaration(DataType(BasicType::CHAN), "field_eight"));

    model.addUtype(basic_types);

    Utype unsigned_types("unsigned_types");

    unsigned_types.addField(Declaration(DataType(UnsignedDataType(4)), "field_one"));
    unsigned_types.addField(Declaration(DataType(UnsignedDataType(8)), "field_two"));
    model.addUtype(unsigned_types);

    Utype array_types("array_types");
    array_types.addField(Declaration(DataType(ArrayType(2, BasicType::BIT)), "field_bit"));
    array_types.addField(Declaration(DataType(ArrayType(3, BasicType::BOOLEAN)), "field_bool"));
    array_types.addField(Declaration(DataType(ArrayType(4, BasicType::BYTE)), "field_byte"));
    array_types.addField(Declaration(DataType(ArrayType(5, BasicType::PID)), "field_pid"));
    array_types.addField(Declaration(DataType(ArrayType(6, BasicType::SHORT)), "field_short"));
    array_types.addField(Declaration(DataType(ArrayType(7, BasicType::INT)), "field_int"));
    array_types.addField(Declaration(DataType(ArrayType(8, BasicType::CHAN)), "field_chan"));

    model.addUtype(array_types);

    Utype utype_refs("utype_refs");
    utype_refs.addField(Declaration(DataType(UtypeRef("array_types")), "field_one"));
    utype_refs.addField(Declaration(DataType(ArrayType(2, UtypeRef("basic_types"))), "field_two"));

    model.addUtype(utype_refs);

    Utype mtype_refs("mtype_refs");
    mtype_refs.addField(Declaration(DataType(MtypeRef()), "field_one"));
    mtype_refs.addField(Declaration(DataType(MtypeRef("states")), "field_two"));
    mtype_refs.addField(Declaration(DataType(ArrayType(2, MtypeRef())), "field_three"));
    mtype_refs.addField(Declaration(DataType(ArrayType(2, MtypeRef("states"))), "field_four"));

    model.addUtype(mtype_refs);

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_file3.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testUtypeVisibility()
{
    PromelaModel model;

    Utype basic_types("test_visibility");

    basic_types.addField(Declaration(DataType(BasicType::INT), "field_one", Declaration::Visibility::HIDDEN));
    basic_types.addField(Declaration(DataType(BasicType::INT), "field_two", Declaration::Visibility::SHOW));
    basic_types.addField(Declaration(DataType(BasicType::INT), "field_three", Declaration::Visibility::NORMAL));

    model.addUtype(basic_types);
    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_file4.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testUtypeUnion()
{
    PromelaModel model;

    Utype test_union("test_union", true);

    test_union.addField(Declaration(DataType(BasicType::INT), "field_one"));
    test_union.addField(Declaration(DataType(MtypeRef()), "field_two"));

    model.addUtype(test_union);

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_file5.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testTypeAlias()
{
    PromelaModel model;

    model.addTypeAlias(TypeAlias("MyInteger", BasicType::INT));
    model.addTypeAlias(TypeAlias("OtherInteger", UtypeRef("MyInteger")));

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_file6.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testValueDefinition()
{
    PromelaModel model;

    model.addValueDefinition(ValueDefinition("one", 1));
    model.addValueDefinition(ValueDefinition("zero", 0));
    model.addValueDefinition(ValueDefinition("minusOne", -1));

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_file7.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testDeclaration()
{
    PromelaModel model;

    model.addDeclaration(Declaration(DataType(BasicType::BIT), "var_one"));
    model.addDeclaration(Declaration(DataType(BasicType::BOOLEAN), "var_two"));
    model.addDeclaration(Declaration(DataType(BasicType::BYTE), "var_three"));
    model.addDeclaration(Declaration(DataType(BasicType::PID), "var_four"));
    model.addDeclaration(Declaration(DataType(BasicType::SHORT), "var_five"));
    model.addDeclaration(Declaration(DataType(BasicType::INT), "var_six"));
    model.addDeclaration(Declaration(DataType(BasicType::FLOAT), "var_seven"));
    model.addDeclaration(Declaration(DataType(BasicType::CHAN), "var_eight"));

    model.addDeclaration(Declaration(DataType(UnsignedDataType(4)), "var_uns1"));
    model.addDeclaration(Declaration(DataType(UnsignedDataType(8)), "var_uns2"));

    model.addDeclaration(Declaration(DataType(ArrayType(2, BasicType::BIT)), "var_array_bit"));
    model.addDeclaration(Declaration(DataType(ArrayType(3, BasicType::BOOLEAN)), "var_array_bool"));
    model.addDeclaration(Declaration(DataType(ArrayType(4, BasicType::BYTE)), "var_array_byte"));
    model.addDeclaration(Declaration(DataType(ArrayType(5, BasicType::PID)), "var_array_pid"));
    model.addDeclaration(Declaration(DataType(ArrayType(6, BasicType::SHORT)), "var_array_short"));
    model.addDeclaration(Declaration(DataType(ArrayType(7, BasicType::INT)), "var_array_int"));
    model.addDeclaration(Declaration(DataType(ArrayType(8, BasicType::CHAN)), "var_array_chan"));

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_file8.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

QString tst_PromelaExporter::getFileContents(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}

QString tst_PromelaExporter::generatePromelaFromModel(const PromelaModel &model)
{
    PromelaExporter exporter;
    Options options;
    options.add(PromelaOptions::promelaOutputFilepath, "generated_promela.pml");

    exporter.exportModel(&model, options);

    return getFileContents("generated_promela.pml");
}

void tst_PromelaExporter::showInfo(const QString &returned, const QString &expected)
{
    std::string s = returned.toStdString();
    std::cerr << "--------[ Returned ] ----------" << std::endl;
    std::cerr << s << std::endl;

    s = expected.toStdString();
    std::cerr << "--------[ Expected ] ----------" << std::endl;
    std::cerr << s << std::endl;
    std::cerr << "-------------------------------" << std::endl;

    auto i = std::mismatch(returned.begin(), returned.end(), expected.begin());
    auto diff = std::distance(returned.begin(), i.first);
    std::cerr << "Diff position " << diff << std::endl;
}
}
QTEST_MAIN(tmc::test::tst_PromelaExporter)

#include "tst_promelaexporter.moc"
