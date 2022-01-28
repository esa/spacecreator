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
using promela::model::Assignment;
using promela::model::BasicType;
using promela::model::ChannelInit;
using promela::model::ChannelRecv;
using promela::model::ChannelSend;
using promela::model::Constant;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::DoLoop;
using promela::model::Expression;
using promela::model::InitProctype;
using promela::model::InlineCall;
using promela::model::MtypeRef;
using promela::model::NamedMtype;
using promela::model::Proctype;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::Sequence;
using promela::model::TypeAlias;
using promela::model::UnsignedDataType;
using promela::model::Utype;
using promela::model::UtypeRef;
using promela::model::ValueDefinition;
using promela::model::VariableRef;

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
    void testIncludes();
    void testChannelInitialization();
    void testBasicProctypes();
    void testInitProctype();
    void testProctypeElements();
    void testDoLoop();
    void testEqilpgueInclude();

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
    QString out2 = getFileContents("expect_promela_mtype.pml");
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
    QString out2 = getFileContents("expect_promela_named_mtype.pml");
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
    QString out2 = getFileContents("expect_promela_utype.pml");
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
    QString out2 = getFileContents("expect_promela_utype_visibility.pml");
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
    QString out2 = getFileContents("expect_promela_utype_union.pml");
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
    QString out2 = getFileContents("expect_promela_type_alias.pml");
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
    QString out2 = getFileContents("expect_promela_value_definition.pml");
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
    QString out2 = getFileContents("expect_promela_declaration.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testIncludes()
{
    PromelaModel model;

    model.addInclude("first.pml");
    model.addInclude("second.pml");

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_includes.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testChannelInitialization()
{
    PromelaModel model;

    model.addTypeAlias(TypeAlias("named_parameter", BasicType::INT));

    Declaration channel1 = Declaration(DataType(BasicType::CHAN), "channel1");
    QList<ChannelInit::Type> channel1Type;
    channel1Type.append(ChannelInit::Type(BasicType::INT));
    channel1.setInit(ChannelInit(1, std::move(channel1Type)));
    Declaration channel2 = Declaration(DataType(BasicType::CHAN), "channel2");
    QList<ChannelInit::Type> channel2Type;
    channel2Type.append(ChannelInit::Type(UtypeRef("named_parameter")));
    channel2.setInit(ChannelInit(2, std::move(channel2Type)));

    model.addDeclaration(channel1);
    model.addDeclaration(channel2);

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_channel_initialization.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testBasicProctypes()
{
    PromelaModel model;

    model.addDeclaration(Declaration(DataType(BasicType::INT), "inited"));

    Sequence basicSequence;
    basicSequence.appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("inited"))));
    std::unique_ptr<Proctype> basicProctype = std::make_unique<Proctype>("basic", std::move(basicSequence));
    model.addProctype(std::move(basicProctype));

    Sequence activeSequence;
    activeSequence.appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("inited"))));
    std::unique_ptr<Proctype> activeProctype = std::make_unique<Proctype>("activeProctype", std::move(activeSequence));
    activeProctype->setActive(true);
    activeProctype->setInstancesCount(1);
    model.addProctype(std::move(activeProctype));

    Sequence activeManyInstancesSequence;
    activeManyInstancesSequence.appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("inited"))));
    std::unique_ptr<Proctype> activeProctypeManyInstances =
            std::make_unique<Proctype>("activeProctypeMany", std::move(activeManyInstancesSequence));
    activeProctypeManyInstances->setActive(true);
    activeProctypeManyInstances->setInstancesCount(2);
    model.addProctype(std::move(activeProctypeManyInstances));

    Sequence sequenceWithPriority;
    sequenceWithPriority.appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("inited"))));
    std::unique_ptr<Proctype> proctypeWithPriority =
            std::make_unique<Proctype>("withPriority", std::move(sequenceWithPriority));
    proctypeWithPriority->setPriority(10);
    model.addProctype(std::move(proctypeWithPriority));

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_basic_proctypes.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testInitProctype()
{
    PromelaModel model;

    model.addDeclaration(Declaration(DataType(BasicType::INT), "inited"));

    Sequence initSequence;
    initSequence.appendElement(std::make_unique<ProctypeElement>(Expression(VariableRef("inited"))));
    InitProctype init(std::move(initSequence));

    model.setInit(std::move(init));

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_init_proctype.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testProctypeElements()
{
    PromelaModel model;

    model.addDeclaration(Declaration(DataType(BasicType::INT), "inited"));
    Declaration channel1 = Declaration(DataType(BasicType::CHAN), "channel1");
    QList<ChannelInit::Type> channel1Type;
    channel1Type.append(ChannelInit::Type(BasicType::INT));
    channel1.setInit(ChannelInit(2, std::move(channel1Type)));
    model.addDeclaration(channel1);

    std::unique_ptr<ProctypeElement> assignment =
            std::make_unique<ProctypeElement>(Assignment(VariableRef("inited"), Expression(Constant(1))));
    std::unique_ptr<ProctypeElement> expressionVariableRef =
            std::make_unique<ProctypeElement>(Expression(VariableRef("inited")));
    std::unique_ptr<ProctypeElement> expressionConstant = std::make_unique<ProctypeElement>(Expression(Constant(2)));
    Declaration paramDecl(DataType(BasicType::INT), "param");
    paramDecl.setInit(Expression(Constant(4)));
    std::unique_ptr<ProctypeElement> declaration = std::make_unique<ProctypeElement>(paramDecl);
    QList<VariableRef> params;
    params.append(VariableRef("param"));
    std::unique_ptr<ProctypeElement> channelSend =
            std::make_unique<ProctypeElement>(ChannelSend(VariableRef("channel1"), params));
    std::unique_ptr<ProctypeElement> channelRecv =
            std::make_unique<ProctypeElement>(ChannelRecv(VariableRef("channel1"), params));
    std::unique_ptr<ProctypeElement> inlineCall = std::make_unique<ProctypeElement>(InlineCall("fn", {}));
    std::unique_ptr<ProctypeElement> inlineCallWithParams =
            std::make_unique<ProctypeElement>(InlineCall("fnParams", params));

    Sequence initSequence;
    initSequence.appendElement(std::move(assignment));
    initSequence.appendElement(std::move(expressionVariableRef));
    initSequence.appendElement(std::move(expressionConstant));
    initSequence.appendElement(std::move(declaration));
    initSequence.appendElement(std::move(channelSend));
    initSequence.appendElement(std::move(channelRecv));
    initSequence.appendElement(std::move(inlineCall));
    initSequence.appendElement(std::move(inlineCallWithParams));

    InitProctype init(std::move(initSequence));

    model.setInit(std::move(init));

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_proctype_elements.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testDoLoop()
{
    PromelaModel model;

    model.addDeclaration(Declaration(DataType(BasicType::INT), "reset"));
    model.addDeclaration(Declaration(DataType(BasicType::INT), "set"));

    std::unique_ptr<Sequence> doLoopFirstSequence = std::make_unique<Sequence>();
    std::unique_ptr<ProctypeElement> resetRef = std::make_unique<ProctypeElement>(Expression(VariableRef("reset")));
    std::unique_ptr<ProctypeElement> resetAssignment0 =
            std::make_unique<ProctypeElement>(Assignment(VariableRef("reset"), Expression(Constant(0))));
    std::unique_ptr<ProctypeElement> setAssignment1 =
            std::make_unique<ProctypeElement>(Assignment(VariableRef("set"), Expression(Constant(1))));

    doLoopFirstSequence->appendElement(std::move(resetRef));
    doLoopFirstSequence->appendElement(std::move(resetAssignment0));
    doLoopFirstSequence->appendElement(std::move(setAssignment1));

    std::unique_ptr<Sequence> doLoopSecondSequence = std::make_unique<Sequence>();
    std::unique_ptr<ProctypeElement> setRef = std::make_unique<ProctypeElement>(Expression(VariableRef("set")));
    std::unique_ptr<ProctypeElement> resetAssignment1 =
            std::make_unique<ProctypeElement>(Assignment(VariableRef("reset"), Expression(Constant(1))));
    std::unique_ptr<ProctypeElement> setAssignment0 =
            std::make_unique<ProctypeElement>(Assignment(VariableRef("set"), Expression(Constant(0))));

    doLoopSecondSequence->appendElement(std::move(setRef));
    doLoopSecondSequence->appendElement(std::move(resetAssignment1));
    doLoopSecondSequence->appendElement(std::move(setAssignment0));

    DoLoop doLoop;
    doLoop.appendSequence(std::move(doLoopFirstSequence));
    doLoop.appendSequence(std::move(doLoopSecondSequence));

    std::unique_ptr<ProctypeElement> doLoopElement = std::make_unique<ProctypeElement>(std::move(doLoop));

    Sequence initSequence;
    std::unique_ptr<ProctypeElement> assignment =
            std::make_unique<ProctypeElement>(Assignment(VariableRef("reset"), Expression(Constant(1))));

    initSequence.appendElement(std::move(assignment));
    initSequence.appendElement(std::move(doLoopElement));

    InitProctype init(std::move(initSequence));

    model.setInit(std::move(init));

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_do_loop.pml");
    showInfo(out, out2);
    QCOMPARE(out, out2);
}

void tst_PromelaExporter::testEqilpgueInclude()
{
    PromelaModel model;

    model.addDeclaration(Declaration(DataType(BasicType::INT), "inited"));

    model.addEpilogueInclude("first.pml");
    model.addEpilogueInclude("second.pml");

    QString out;
    try {
        out = generatePromelaFromModel(model);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
    QString out2 = getFileContents("expect_promela_epilogue_includes.pml");
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
    options.add(PromelaOptions::outputFilepath, "generated_promela.pml");

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
