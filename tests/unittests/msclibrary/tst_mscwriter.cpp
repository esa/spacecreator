/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "cif/cifblockfactory.h"
#include "cif/ciflines.h"
#include "datastatement.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccomment.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "msclibrary.h"
#include "mscmessage.h"
#include "mscmessagedeclaration.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "msctimer.h"
#include "mscwriter.h"

#include <QtTest>

using namespace msc;

class tst_MscWriter : public MscWriter
{
    Q_OBJECT
public:
    tst_MscWriter();

private Q_SLOTS:
    void cleanup();

    void testSaveDocumentModel_data();
    void testSaveDocumentModel();
    void testSerializeMscChart_data();
    void testSerializeMscChart();
    void testSerializeMscInstance_data();
    void testSerializeMscInstance();
    void testSerializeMscActions_data();
    void testSerializeMscActions();
    void testSerializeMscConditions_data();
    void testSerializeMscConditions();
    void testSerializeMscCoregion_data();
    void testSerializeMscCoregion();
    void testSerializeCreate_data();
    void testSerializeCreate();
    void testSerializeMscMessage_data();
    void testSerializeMscMessage();
    void testSerializeMscTimer_data();
    void testSerializeMscTimer();

private:
    QString removeIndention(const QString &text) const;
};

tst_MscWriter::tst_MscWriter()
{
    initMscLibrary();
}

QString tst_MscWriter::removeIndention(const QString &text) const
{
    QStringList lines = text.split("\n");
    QString result;
    for (const QString &line : lines) {
        if (!line.isEmpty()) {
            if (!result.isEmpty()) {
                result += "\n";
            }
            result += line.trimmed();
        }
    }

    return result;
}

void tst_MscWriter::cleanup()
{
    setModel(nullptr);
}

void tst_MscWriter::testSaveDocumentModel_data()
{
    QTest::addColumn<MscModel *>("model");
    QTest::addColumn<QString>("result");
    QTest::addColumn<QString>("resultGrantLee");

    auto model = new MscModel(this);
    model->addDocument(new MscDocument("EmptyDoc", model));
    QString result = { "mscdocument EmptyDoc /* MSC AND */;\nendmscdocument;" };
    QTest::addRow("Simple doc") << model << result << result;

    model = new MscModel(this);
    model->addDocument(new MscDocument("Doc1", model));
    model->addDocument(new MscDocument("Doc2", model));
    result = QString("mscdocument Doc1 /* MSC AND */;\nendmscdocument;\n"
                     "mscdocument Doc2 /* MSC AND */;\nendmscdocument;");
    QTest::addRow("Two docs") << model << result << result;

    model = new MscModel(this);
    model->addDocument(new MscDocument("DataDoc", model));
    model->setDataLanguage("ASN.1");
    model->setDataDefinitionString("RPos.asn");
    result = "mscdocument DataDoc /* MSC AND */;\n"
             "    language ASN.1;\n"
             "    data RPos.asn;\n"
             "endmscdocument;";
    QTest::addRow("DataDef") << model << result << result;

    model = new MscModel(this);
    auto doc1 = new MscDocument("ParentDoc", model);
    model->addDocument(doc1);
    doc1->addDocument(new MscDocument("ChildDoc", model));
    result = "mscdocument ParentDoc /* MSC AND */;\n"
             "    mscdocument ChildDoc /* MSC AND */;\n"
             "    endmscdocument;\n"
             "endmscdocument;";
    QTest::addRow("Nested documents") << model << result << removeIndention(result);

    model = new MscModel(this);
    auto doc2 = new MscDocument("CommentedDoc", model);
    doc2->setCommentString("Ignore this");
    model->addDocument(doc2);
    result = QString("mscdocument CommentedDoc comment 'Ignore this' /* MSC AND */;\nendmscdocument;");
    QTest::addRow("Commented doc") << model << result << result;

    model = new MscModel(this);
    auto doc3 = new MscDocument("DeclDoc", model);
    auto declaration = new msc::MscMessageDeclaration(doc3);
    declaration->setNames({ "init", "x" });
    declaration->setTypeRefList({ "int", "width" });
    doc3->messageDeclarations()->append(declaration);
    auto declaration1 = new msc::MscMessageDeclaration(doc3);
    declaration1->setNames({ "gui_send_tm", "pepe" });
    declaration1->setTypeRefList({ "str", "T-POS" });
    doc3->messageDeclarations()->append(declaration1);
    model->addDocument(doc3);
    result = QString("mscdocument DeclDoc /* MSC AND */;\n"
                     "    msg init, x : (int, width);\n"
                     "    msg gui_send_tm, pepe : (str, T-POS);\n"
                     "endmscdocument;");
    QTest::addRow("Doc with declaration") << model << result << result;

    model = new MscModel(this);
    auto doc4 = new MscDocument("ChartDoc", model);
    doc4->addChart(new MscChart("UltimateChart"));
    model->addDocument(doc4);
    result = "mscdocument ChartDoc /* MSC AND */;\n"
             "    msc UltimateChart;\n"
             "    endmsc;\n"
             "endmscdocument;";
    QTest::addRow("ChartIncluded") << model << result << removeIndention(result);

    model = new MscModel(this);
    auto doc5 = new MscDocument("BaseDoc", model);
    auto doc6 = new MscDocument("CifDoc", model);
    doc5->addDocument(doc6);
    cif::CifBlockShared mscDocCif = cif::CifBlockFactory::createBlockMscDocument();
    mscDocCif->addLine(cif::CifLineShared(new cif::CifLineMscDocument()));
    const QVector<QPoint> points { QPoint(20, 30), QPoint(500, 400) };
    mscDocCif->setPayload(QVariant::fromValue(points), cif::CifLine::CifType::MscDocument);
    doc6->addCif(mscDocCif);
    model->addDocument(doc5);
    result = "mscdocument BaseDoc /* MSC AND */;\n"
             "/* CIF MSCDOCUMENT (20, 30) (500, 400) */\n"
             "    mscdocument CifDoc /* MSC AND */;\n"
             "    endmscdocument;\n"
             "endmscdocument;";
    QTest::addRow("ChartIncluded") << model << result << removeIndention(result);
}

void tst_MscWriter::testSaveDocumentModel()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(SaveMode::CUSTOM);
    QString text = modelText(model);
    text = text.trimmed();

    QCOMPARE(text, result);

    setSaveMode(SaveMode::GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscChart_data()
{
    QTest::addColumn<MscModel *>("model");
    QTest::addColumn<QString>("result");
    QTest::addColumn<QString>("resultGrantLee");

    auto model = new MscModel(this);
    model->addChart(new MscChart("Chart_1"));
    QString result = "msc Chart_1;\nendmsc;\n";
    QTest::addRow("Simple chart") << model << result << removeIndention(result);

    model = new MscModel(this);
    auto chart1 = new MscChart("Chart_1");
    chart1->setCommentString("Importante");
    model->addChart(chart1);
    result = "msc Chart_1;\nendmsc;\n";
    QTest::addRow("Plain chart comments ignored") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart1 = new MscChart("Chart_1");
    chart1->setCommentString("Global information");
    chart1->comment()->setRect(QRect(140, 55, 40, 30));
    // add global comment
    model->addChart(chart1);
    result = "msc Chart_1;\n"
             "/* CIF TEXT (140, 55) (40, 30) */\n"
             "/* Global information */\n"
             "/* CIF END TEXT */\n\n\n"
             "endmsc;\n";
    QTest::addRow("Global comment") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeMscChart()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(SaveMode::CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(SaveMode::GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscInstance_data()
{
    QTest::addColumn<MscModel *>("model");
    QTest::addColumn<QString>("result");
    QTest::addColumn<QString>("resultGrantLee");

    auto model = new MscModel(this);
    auto chart1 = new MscChart("Chart_1");
    auto instance = new MscInstance("instA");
    chart1->addInstance(instance);
    model->addChart(chart1);
    auto result = QString("msc Chart_1;\n"
                          "    instance instA;\n"
                          "    endinstance;\n"
                          "endmsc;\n");
    QTest::addRow("Empty instance") << model << result << removeIndention(result);

    model = new MscModel(this);
    auto chart2 = new MscChart("Chart_2");
    auto instance2 = new MscInstance("instB");
    instance2->setCommentString("Do not touch");
    chart2->addInstance(instance2);
    model->addChart(chart2);
    result = QString("msc Chart_2;\n"
                     "    instance instB comment 'Do not touch';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented instance") << model << result << removeIndention(result);

    model = new MscModel(this);
    auto chart = new MscChart("Chart");
    instance = new MscInstance("instB");
    instance->setCommentString("Do not touch");
    instance->comment()->setRect(QRect(10, 20, 30, 40));
    chart->addInstance(instance);
    model->addChart(chart);
    result = QString("msc Chart;\n"
                     "    instance instB\n"
                     "    /* CIF COMMENT (10, 20) (30, 40) */\n\n"
                     " comment 'Do not touch';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented instance geometry") << model << result << removeIndention(result);

    model = new MscModel(this);
    auto chart3 = new MscChart("Chart_3");
    auto instance3 = new MscInstance("instC");
    instance3->setDenominator("PROCESS");
    instance3->setKind("cu_unit");
    instance3->setInheritance("P1");
    chart3->addInstance(instance3);
    model->addChart(chart3);
    result = QString("msc Chart_3;\n"
                     "    instance instC: PROCESS cu_unit P1;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Instance denominator") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_4");
    instance = new MscInstance("instS");
    instance->setExplicitStop(true);
    chart->addInstance(instance);
    model->addChart(chart);
    result = QString("msc Chart_4;\n"
                     "    instance instS;\n"
                     "    stop;\n"
                     "endmsc;\n");
    QTest::addRow("Instance stop") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_4");
    instance = new MscInstance("instS");
    instance->setCifGeometry({ QPoint(50, 10), QPoint(50, 140), QPoint(800, 1000) });
    chart->addInstance(instance);
    model->addChart(chart);
    result = QString("msc Chart_4;\n"
                     "    /* CIF INSTANCE (50, 10) (50, 140) (800, 1000) */\n"
                     "    instance instS;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Instance geometry") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeMscInstance()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(SaveMode::CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(SaveMode::GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscActions_data()
{
    QTest::addColumn<MscModel *>("model");
    QTest::addColumn<QString>("result");
    QTest::addColumn<QString>("resultGrantLee");

    auto model = new MscModel(this);
    auto chart = new MscChart("Chart_1");
    auto instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    auto instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    auto action = new MscAction();
    action->setActionType(MscAction::ActionType::Informal);
    action->setInformalAction("informal_stop");
    action->setInstance(instance);
    chart->addInstanceEvent(action);
    model->addChart(chart);
    auto result = QString("msc Chart_1;\n"
                          "    instance Inst_1;\n"
                          "        action 'informal_stop';\n"
                          "    endinstance;\n"
                          "    instance Inst_2;\n"
                          "    endinstance;\n"
                          "endmsc;\n");
    QTest::addRow("Informal action") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_2");
    instance = new MscInstance("Inst_2");
    chart->addInstance(instance);
    action = new MscAction();
    action->setActionType(MscAction::ActionType::Formal);
    auto statement = new msc::DataStatement(action);
    statement->setType(msc::DataStatement::StatementType::Define);
    statement->setVariableString("digit1");
    action->addDataStatement(statement);
    auto statement2 = new msc::DataStatement(action);
    statement2->setType(msc::DataStatement::StatementType::UnDefine);
    statement2->setVariableString("digit2");
    action->addDataStatement(statement2);
    action->setInstance(instance);
    chart->addInstanceEvent(action);
    model->addChart(chart);
    result = QString("msc Chart_2;\n"
                     "    instance Inst_2;\n"
                     "        action def digit1, undef digit2;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Formal action") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_3");
    instance = new MscInstance("Inst_3");
    chart->addInstance(instance);
    action = new MscAction();
    action->setActionType(MscAction::ActionType::Informal);
    action->setInformalAction("doX");
    action->setCommentString("Very important");
    action->setInstance(instance);
    chart->addInstanceEvent(action);
    model->addChart(chart);
    result = QString("msc Chart_3;\n"
                     "    instance Inst_3;\n"
                     "        action 'doX' comment 'Very important';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented action") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_3");
    instance = new MscInstance("Inst_3");
    chart->addInstance(instance);
    action = new MscAction();
    action->setActionType(MscAction::ActionType::Informal);
    action->setInformalAction("doX");
    action->setCommentString("Very important");
    action->comment()->setRect(QRect(50, 30, 20, 15));
    action->setInstance(instance);
    chart->addInstanceEvent(action);
    model->addChart(chart);
    result = QString("msc Chart_3;\n"
                     "    instance Inst_3;\n"
                     "        action 'doX'\n"
                     "    /* CIF COMMENT (50, 30) (20, 15) */\n\n"
                     " comment 'Very important';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented action geometry") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeMscActions()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(SaveMode::CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(SaveMode::GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscConditions_data()
{
    QTest::addColumn<MscModel *>("model");
    QTest::addColumn<QString>("result");
    QTest::addColumn<QString>("resultGrantLee");

    auto model = new MscModel(this);
    auto chart = new MscChart("Chart_1");
    auto instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    auto instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    auto condition = new MscCondition("Con_1");
    condition->setInstance(instance);
    chart->addInstanceEvent(condition);
    model->addChart(chart);
    auto result = QString("msc Chart_1;\n"
                          "    instance Inst_1;\n"
                          "        condition Con_1;\n"
                          "    endinstance;\n"
                          "    instance Inst_2;\n"
                          "    endinstance;\n"
                          "endmsc;\n");
    QTest::addRow("Simple condition") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    condition = new MscCondition("Con_1");
    condition->setShared(true);
    condition->setInstance(instance);
    chart->addInstanceEvent(condition);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        condition Con_1 shared all;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Shared condition") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    condition = new MscCondition("Con_1");
    condition->setInstance(instance);
    condition->setCommentString("Hey ho");
    chart->addInstanceEvent(condition);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        condition Con_1 comment 'Hey ho';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented condition") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    condition = new MscCondition("Con_1");
    condition->setInstance(instance);
    condition->setCommentString("Hey ho");
    condition->comment()->setRect(QRect(50, 30, 20, 15));
    chart->addInstanceEvent(condition);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        condition Con_1\n"
                     "    /* CIF COMMENT (50, 30) (20, 15) */\n\n"
                     " comment 'Hey ho';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented condition geometry") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeMscConditions()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(SaveMode::CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(SaveMode::GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscCoregion_data()
{
    QTest::addColumn<MscModel *>("model");
    QTest::addColumn<QString>("result");
    QTest::addColumn<QString>("resultGrantLee");

    auto model = new MscModel(this);
    auto chart = new MscChart("Chart_1");
    auto instance = new MscInstance("Inst_1");
    auto instance2 = new MscInstance("Inst_2");
    auto region1 = new MscCoregion(MscCoregion::Type::Begin);
    region1->setInstance(instance);
    chart->addInstanceEvent(region1);
    auto region2 = new MscCoregion(MscCoregion::Type::End);
    region2->setInstance(instance);
    chart->addInstanceEvent(region2);
    chart->addInstance(instance);
    chart->addInstance(instance2);
    model->addChart(chart);
    auto result = QString("msc Chart_1;\n"
                          "    instance Inst_1;\n"
                          "        concurrent;\n"
                          "        endconcurrent;\n"
                          "    endinstance;\n"
                          "    instance Inst_2;\n"
                          "    endinstance;\n"
                          "endmsc;\n");
    QTest::addRow("Simple coregion") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    region1 = new MscCoregion(MscCoregion::Type::Begin);
    region1->setCommentString("parallel begin");
    region1->setInstance(instance);
    chart->addInstanceEvent(region1);
    region2 = new MscCoregion(MscCoregion::Type::End);
    region2->setCommentString("parallel end");
    region2->setInstance(instance);
    chart->addInstanceEvent(region2);
    chart->addInstance(instance);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        concurrent comment 'parallel begin';\n"
                     "        endconcurrent comment 'parallel end';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented coregion") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    region1 = new MscCoregion(MscCoregion::Type::Begin);
    region1->setCommentString("parallel begin");
    region1->comment()->setRect(QRect(50, 30, 20, 15));
    region1->setInstance(instance);
    chart->addInstanceEvent(region1);
    region2 = new MscCoregion(MscCoregion::Type::End);
    region2->setCommentString("parallel end");
    region2->comment()->setRect(QRect(60, 90, 30, 15));
    region2->setInstance(instance);
    chart->addInstanceEvent(region2);
    chart->addInstance(instance);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        concurrent\n"
                     "    /* CIF COMMENT (50, 30) (20, 15) */\n\n"
                     " comment 'parallel begin';\n"
                     "        endconcurrent\n"
                     "    /* CIF COMMENT (60, 90) (30, 15) */\n\n"
                     " comment 'parallel end';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented coregion geometry") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeMscCoregion()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(SaveMode::CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(SaveMode::GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, removeIndention(resultGrantLee));
}

void tst_MscWriter::testSerializeCreate_data()
{
    QTest::addColumn<MscModel *>("model");
    QTest::addColumn<QString>("result");
    QTest::addColumn<QString>("resultGrantLee");

    auto model = new MscModel(this);
    auto chart = new MscChart("Chart_1");
    auto instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    auto instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    auto instance3 = new MscInstance("Inst_3");
    chart->addInstance(instance3);
    auto create = new MscCreate("");
    create->setSourceInstance(instance);
    create->setTargetInstance(instance2);
    chart->addInstanceEvent(create);
    model->addChart(chart);
    auto result = QString("msc Chart_1;\n"
                          "    instance Inst_1;\n"
                          "        create Inst_2;\n"
                          "    endinstance;\n"
                          "    instance Inst_2;\n"
                          "    endinstance;\n"
                          "    instance Inst_3;\n"
                          "    endinstance;\n"
                          "endmsc;\n");
    QTest::addRow("Simple create") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    create = new MscCreate("");
    create->setSourceInstance(instance);
    create->setTargetInstance(instance2);
    MscParameterList parameters;
    MscParameter parameter;
    parameter.setPattern("data1");
    parameters << parameter;
    parameter.setPattern("data2");
    parameters << parameter;
    create->setParameters(parameters);
    chart->addInstanceEvent(create);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        create Inst_2(data1, data2);\n"
                     "    endinstance;\n"
                     "    instance Inst_2;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Create with parameters") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    create = new MscCreate("");
    create->setSourceInstance(instance);
    create->setTargetInstance(instance2);
    create->setCommentString("Let there be an instance");
    chart->addInstanceEvent(create);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        create Inst_2 comment 'Let there be an instance';\n"
                     "    endinstance;\n"
                     "    instance Inst_2;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented create") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    create = new MscCreate("");
    create->setSourceInstance(instance);
    create->setTargetInstance(instance2);
    create->setCommentString("Let there be an instance");
    create->comment()->setRect(QRect(50, 30, 20, 15));
    chart->addInstanceEvent(create);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        create Inst_2\n"
                     "    /* CIF COMMENT (50, 30) (20, 15) */\n\n"
                     " comment 'Let there be an instance';\n"
                     "    endinstance;\n"
                     "    instance Inst_2;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented create geometry") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeCreate()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(SaveMode::CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(SaveMode::GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscMessage_data()
{
    QTest::addColumn<MscModel *>("model");
    QTest::addColumn<QString>("result");
    QTest::addColumn<QString>("resultGrantLee");

    auto model = new MscModel(this);
    auto chart = new MscChart("Chart_1");
    auto instance1 = new MscInstance("Inst_1");
    chart->addInstance(instance1);
    auto instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    auto message = new MscMessage("Msg_1");
    message->setSourceInstance(instance1);
    message->setTargetInstance(instance2);
    chart->addInstanceEvent(message);
    model->addChart(chart);
    auto result = QString("msc Chart_1;\n"
                          "    instance Inst_1;\n"
                          "        out Msg_1 to Inst_2;\n"
                          "    endinstance;\n"
                          "    instance Inst_2;\n"
                          "        in Msg_1 from Inst_1;\n"
                          "    endinstance;\n"
                          "endmsc;\n");
    QTest::addRow("Simple message") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance1 = new MscInstance("Inst_1");
    chart->addInstance(instance1);
    instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    message = new MscMessage("Msg_1");
    message->setSourceInstance(instance1);
    chart->addInstanceEvent(message);
    auto message2 = new MscMessage("Msg_2");
    message2->setTargetInstance(instance2);
    chart->addInstanceEvent(message2);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        out Msg_1 to env;\n"
                     "    endinstance;\n"
                     "    instance Inst_2;\n"
                     "        in Msg_2 from env;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Env.  message") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance1 = new MscInstance("Inst_1");
    chart->addInstance(instance1);
    instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    message = new MscMessage("Msg_1");
    message->setSourceInstance(instance1);
    message->setTargetInstance(instance2);
    message->setMessageInstanceName("a");
    chart->addInstanceEvent(message);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        out Msg_1,a to Inst_2;\n"
                     "    endinstance;\n"
                     "    instance Inst_2;\n"
                     "        in Msg_1,a from Inst_1;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Instance name") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance1 = new MscInstance("Inst_1");
    chart->addInstance(instance1);
    instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    message = new MscMessage("Msg_1");
    message->setSourceInstance(instance1);
    message->setTargetInstance(instance2);
    message->setMessageInstanceName("a");
    message->setParameters({ { "longitude:-174.0", "" } });
    chart->addInstanceEvent(message);
    message2 = new MscMessage("Msg_2");
    message2->setSourceInstance(instance2);
    message2->setTargetInstance(instance1);
    message2->setParameters({ { "{tc-id '4143545f494555'H, report success}", "" } });
    chart->addInstanceEvent(message2);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        out Msg_1,a(longitude:-174.0) to Inst_2;\n"
                     "        in Msg_2({tc-id '4143545f494555'H, report success}) from Inst_2;\n"
                     "    endinstance;\n"
                     "    instance Inst_2;\n"
                     "        in Msg_1,a(longitude:-174.0) from Inst_1;\n"
                     "        out Msg_2({tc-id '4143545f494555'H, report success}) to Inst_1;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Parameter") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance1 = new MscInstance("Inst_1");
    chart->addInstance(instance1);
    instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    message = new MscMessage("Msg_1");
    message->setSourceInstance(instance1);
    message->setTargetInstance(instance2);
    message->setMessageInstanceName("a");
    message->setParameters({ { "longitude:-174.0", "" }, { "", "init" } });
    chart->addInstanceEvent(message);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        out Msg_1,a(longitude:-174.0, init) to Inst_2;\n"
                     "    endinstance;\n"
                     "    instance Inst_2;\n"
                     "        in Msg_1,a(longitude:-174.0, init) from Inst_1;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Multi-parameter") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance1 = new MscInstance("Inst_1");
    chart->addInstance(instance1);
    message = new MscMessage("Msg_1");
    message->setSourceInstance(instance1);
    message->setCommentString("Shout out");
    chart->addInstanceEvent(message);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        out Msg_1 to env comment 'Shout out';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented message") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance1 = new MscInstance("Inst_1");
    chart->addInstance(instance1);
    message = new MscMessage("Msg_1");
    message->setSourceInstance(instance1);
    message->setCifPoints({ QPoint(125, 50), QPoint(90, 150) });
    chart->addInstanceEvent(message);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "    /* CIF MESSAGE (125, 50) (90, 150) */\n"
                     "        out Msg_1 to env;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Message geometry") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance1 = new MscInstance("Inst_1");
    chart->addInstance(instance1);
    message = new MscMessage("Msg_1");
    message->setSourceInstance(instance1);
    message->setCommentString("Shout out");
    message->setCifPoints({ QPoint(125, 50), QPoint(90, 150) });
    message->comment()->setRect(QRect(50, 30, 20, 15));
    chart->addInstanceEvent(message);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "    /* CIF MESSAGE (125, 50) (90, 150) */\n"
                     "        out Msg_1 to env\n"
                     "    /* CIF COMMENT (50, 30) (20, 15) */\n\n"
                     " comment 'Shout out';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented message geometry") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeMscMessage()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(SaveMode::CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(SaveMode::GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscTimer_data()
{
    QTest::addColumn<MscModel *>("model");
    QTest::addColumn<QString>("result");
    QTest::addColumn<QString>("resultGrantLee");

    auto model = new MscModel(this);
    auto chart = new MscChart("Chart_1");
    auto instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    auto instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    auto timer = new MscTimer("T1", MscTimer::TimerType::Start);
    timer->setInstance(instance);
    chart->addInstanceEvent(timer);
    model->addChart(chart);
    auto result = QString("msc Chart_1;\n"
                          "    instance Inst_1;\n"
                          "        starttimer T1;\n"
                          "    endinstance;\n"
                          "    instance Inst_2;\n"
                          "    endinstance;\n"
                          "endmsc;\n");
    QTest::addRow("Start") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    timer = new MscTimer("T1", MscTimer::TimerType::Stop);
    timer->setInstance(instance);
    chart->addInstanceEvent(timer);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        stoptimer T1;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Stop") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    timer = new MscTimer("T1", MscTimer::TimerType::Timeout);
    timer->setInstance(instance);
    chart->addInstanceEvent(timer);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        timeout T1;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Timeout") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    timer = new MscTimer("T1", MscTimer::TimerType::Start);
    timer->setTimerInstanceName("a");
    timer->setInstance(instance);
    chart->addInstanceEvent(timer);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        starttimer T1,a;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Instance name") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    timer = new MscTimer("T1", MscTimer::TimerType::Start);
    timer->setCommentString("Go go go");
    timer->setInstance(instance);
    chart->addInstanceEvent(timer);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        starttimer T1 comment 'Go go go';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented timer") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    timer = new MscTimer("T1", MscTimer::TimerType::Start);
    timer->setCommentString("Go go go");
    timer->comment()->setRect(QRect(50, 30, 20, 15));
    timer->setInstance(instance);
    chart->addInstanceEvent(timer);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        starttimer T1\n"
                     "    /* CIF COMMENT (50, 30) (20, 15) */\n\n"
                     " comment 'Go go go';\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Commented timer geometry") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeMscTimer()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(SaveMode::CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(SaveMode::GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

QTEST_MAIN(tst_MscWriter)

#include "tst_mscwriter.moc"
