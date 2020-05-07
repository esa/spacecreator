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

    void testSelthTabbing();
    void testSaveDocumentModel_data();
    void testSaveDocumentModel();
    void testSerializeMscMessage();
    void testSerializeMscMessageParameters();
    void testSerializeMscMessageMultiParameters();
    void testSerializeMscTimer();
    void testSerializeMscInstance_data();
    void testSerializeMscInstance();
    void testSerializeMscInstanceStop();
    void testSerializeMscInstanceKind();
    void testSerializeMscInstanceEvents();
    void testSerializeMscActions_data();
    void testSerializeMscActions();
    void testSerializeMscConditions_data();
    void testSerializeMscConditions();
    void testSerializeMscCoregion();
    void testSerializeMscChart_data();
    void testSerializeMscChart();
    void testSerializeMscChartInstance();
    void testSerializeMscDocument();
    void testSerializeMscDocumentChart();
    void testSerializeDataDefinition();
    void testSerializeCreate();
    void testSerializeChartWithCreate();
    void testSerializeComments();
    void testSerializeMessageDeclarations();

private:
    QString prependTabs(const QString &str, int count) const;
    QString tab1(const QString &str) const;
    QString tab2(const QString &str) const;
    QString tab3(const QString &str) const;
    QString removeIndention(const QString &text) const;
};

tst_MscWriter::tst_MscWriter()
{
    initMscLibrary();
}

QString tst_MscWriter::prependTabs(const QString &str, int count) const
{
    const QString tabSpaces = QString("    "); // 4 space chars
    QString res;
    for (int i = 0; i < count; ++i)
        res += tabSpaces;
    return res + str;
}

QString tst_MscWriter::tab1(const QString &str) const
{
    return prependTabs(str, 1);
}

QString tst_MscWriter::tab2(const QString &str) const
{
    return prependTabs(str, 2);
}

QString tst_MscWriter::tab3(const QString &str) const
{
    return prependTabs(str, 3);
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

void tst_MscWriter::testSelthTabbing()
{
    const QString tabSpaces = QString("    "); // 4 space chars
    const QString tabTest("tabTest");
    QCOMPARE(tab1(tabTest), tabSpaces + tabTest);
    QCOMPARE(tab2(tabTest), tabSpaces + tabSpaces + tabTest);
    QCOMPARE(tab3(tabTest), tabSpaces + tabSpaces + tabSpaces + tabTest);
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
}

void tst_MscWriter::testSaveDocumentModel()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(CUSTOM);
    QString text = modelText(model);
    text = text.trimmed();

    QCOMPARE(text, result);

    setSaveMode(GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscMessage()
{
    MscMessage message("Msg_1");
    MscInstance source("Inst_1");
    MscInstance target("Inst_2");

    message.setSourceInstance(&source);
    message.setTargetInstance(&target);

    QCOMPARE(this->serialize(&message, &source), QString("out Msg_1 to Inst_2;\n"));
    QCOMPARE(this->serialize(&message, &target), QString("in Msg_1 from Inst_1;\n"));
}

void tst_MscWriter::testSerializeMscMessageParameters()
{
    MscMessage message("Msg_1");
    message.setMessageInstanceName("a");
    message.setParameters({ { "longitude:-174.0", "" } });

    MscInstance source("Inst_1");
    MscInstance target("Inst_2");

    message.setSourceInstance(&source);
    message.setTargetInstance(&target);

    QCOMPARE(this->serialize(&message, &source), QString("out Msg_1,a(longitude:-174.0) to Inst_2;\n"));
}

void tst_MscWriter::testSerializeMscMessageMultiParameters()
{
    MscMessage message("Msg_1");
    message.setMessageInstanceName("a");
    message.setParameters({ { "longitude:-174.0", "" }, { "", "init" } });

    MscInstance source("Inst_1");
    MscInstance target("Inst_2");

    message.setSourceInstance(&source);
    message.setTargetInstance(&target);

    QCOMPARE(this->serialize(&message, &source), QString("out Msg_1,a(longitude:-174.0, init) to Inst_2;\n"));
}

void tst_MscWriter::testSerializeMscTimer()
{
    MscTimer timer1("T1", MscTimer::TimerType::Start);
    timer1.setTimerInstanceName("a");
    QCOMPARE(this->serialize(&timer1, nullptr), QString("starttimer T1,a;\n"));
    QCOMPARE(this->serialize(&timer1, nullptr, 1), tab1("starttimer T1,a;\n"));
    QCOMPARE(this->serialize(&timer1, nullptr, 2), tab2("starttimer T1,a;\n"));

    MscTimer timer2("T2", MscTimer::TimerType::Stop);
    QCOMPARE(this->serialize(&timer2, nullptr), QString("stoptimer T2;\n"));

    MscTimer timer3("T3", MscTimer::TimerType::Timeout);
    QCOMPARE(this->serialize(&timer3, nullptr), QString("timeout T3;\n"));
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
    auto chart3 = new MscChart("Chart_3");
    auto instance3 = new MscInstance("instC");
    instance3->setDenominator("foo");
    instance3->setKind("bar");
    instance3->setInheritance("master");
    chart3->addInstance(instance3);
    model->addChart(chart3);
    result = QString("msc Chart_3;\n"
                     "    instance instC: foo bar master;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Instance denominator") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeMscInstance()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscInstanceStop()
{
    MscInstance instance("Inst_1");
    instance.setExplicitStop(true);

    QCOMPARE(this->serialize(&instance, QVector<MscInstanceEvent *>()), QString("instance Inst_1;\nstop;\n"));
}

void tst_MscWriter::testSerializeMscInstanceKind()
{
    MscInstance instance("Inst_1");
    instance.setDenominator("PROCESS");
    instance.setKind("cu_unit");
    instance.setInheritance("P1");

    QCOMPARE(this->serialize(&instance, QVector<MscInstanceEvent *>()),
             QString("instance Inst_1: PROCESS cu_unit P1;\nendinstance;\n"));
}

void tst_MscWriter::testSerializeMscInstanceEvents()
{
    MscInstance instance("Inst_1");

    QScopedPointer<MscMessage> message(new MscMessage("Msg_1"));
    MscInstance instance2("Inst_2");

    message->setSourceInstance(&instance2);
    message->setTargetInstance(&instance);

    QScopedPointer<MscMessage> message2(new MscMessage("Msg_2"));
    message2->setSourceInstance(&instance);
    message2->setTargetInstance(&instance2);

    QVector<MscInstanceEvent *> messages;
    messages.append(message.data());
    messages.append(message2.data());

    QStringList serializeList = this->serialize(&instance, messages).split("\n", QString::SkipEmptyParts);
    QCOMPARE(serializeList.size(), 4);
    QCOMPARE(serializeList.at(0), QString("instance Inst_1;"));
    QCOMPARE(serializeList.at(1), tab1("in Msg_1 from Inst_2;"));
    QCOMPARE(serializeList.at(2), tab1("out Msg_2 to Inst_2;"));
    QCOMPARE(serializeList.at(3), QString("endinstance;"));

    // Add some timers and test again
    QScopedPointer<MscTimer> timer1(new MscTimer("T_start", MscTimer::TimerType::Start));
    timer1->setInstance(&instance);
    messages.insert(1, timer1.data());
    QScopedPointer<MscTimer> timer2(new MscTimer("T_fire", MscTimer::TimerType::Timeout));
    timer2->setInstance(&instance);
    messages.append(timer2.data());
    QScopedPointer<MscTimer> timer3(new MscTimer("T_stop", MscTimer::TimerType::Stop));
    messages.append(timer3.data());
    timer3->setInstance(&instance);

    serializeList = this->serialize(&instance, messages).split("\n", QString::SkipEmptyParts);
    QCOMPARE(serializeList.size(), 7);
    QCOMPARE(serializeList.at(0), QString("instance Inst_1;"));
    QCOMPARE(serializeList.at(1), tab1("in Msg_1 from Inst_2;"));
    QCOMPARE(serializeList.at(2), tab1("starttimer T_start;"));
    QCOMPARE(serializeList.at(3), tab1("out Msg_2 to Inst_2;"));
    QCOMPARE(serializeList.at(4), tab1("timeout T_fire;"));
    QCOMPARE(serializeList.at(5), tab1("stoptimer T_stop;"));
    QCOMPARE(serializeList.at(6), QString("endinstance;"));
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
}

void tst_MscWriter::testSerializeMscActions()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(GRANTLEE);
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
    auto condition = new MscCondition("Con_1");
    condition->setInstance(instance);
    chart->addInstanceEvent(condition);
    model->addChart(chart);
    auto result = QString("msc Chart_1;\n"
                          "    instance Inst_1;\n"
                          "        condition Con_1;\n"
                          "    endinstance;\n"
                          "endmsc;\n");
    QTest::addRow("Simple condition") << model << result << removeIndention(result);

    model = new MscModel(this);
    chart = new MscChart("Chart_1");
    instance = new MscInstance("Inst_1");
    chart->addInstance(instance);
    auto instance2 = new MscInstance("Inst_2");
    chart->addInstance(instance2);
    condition = new MscCondition("Con_1");
    condition->setShared(true);
    condition->setInstance(instance);
    chart->addInstanceEvent(condition);
    model->addChart(chart);
    result = QString("msc Chart_1;\n"
                     "    instance Inst_1;\n"
                     "        condition Con_1 shared all;\n"
                     "    endinstance;\n"
                     "    instance Inst_2;\n"
                     "    endinstance;\n"
                     "endmsc;\n");
    QTest::addRow("Shared condition") << model << result << removeIndention(result);
}

void tst_MscWriter::testSerializeMscConditions()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscCoregion()
{
    auto model = new MscModel(this);
    auto chart = new MscChart("Chart_1");
    auto instance = new MscInstance("Inst_1");

    auto region1 = new MscCoregion(MscCoregion::Type::Begin);
    region1->setInstance(instance);
    chart->addInstanceEvent(region1);
    auto region2 = new MscCoregion(MscCoregion::Type::End);
    region2->setInstance(instance);
    chart->addInstanceEvent(region2);

    chart->addInstance(instance);
    model->addChart(chart);

    auto result = QString("msc Chart_1;\n"
                          "    instance Inst_1;\n"
                          "        concurrent;\n"
                          "        endconcurrent;\n"
                          "    endinstance;\n"
                          "endmsc;\n");

    setSaveMode(CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, removeIndention(result));
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
}

void tst_MscWriter::testSerializeMscChart()
{
    QFETCH(MscModel *, model);
    QFETCH(QString, result);
    QFETCH(QString, resultGrantLee);

    setSaveMode(CUSTOM);
    QString text = modelText(model);
    QCOMPARE(text, result);

    setSaveMode(GRANTLEE);
    text = modelText(model);
    QCOMPARE(text, resultGrantLee);
}

void tst_MscWriter::testSerializeMscChartInstance()
{
    MscChart chart("Chart_1");

    MscInstance *instance = new MscInstance("Inst_1");

    MscMessage *message = new MscMessage("Msg_1");
    MscInstance *instance2 = new MscInstance("Inst_2");

    message->setSourceInstance(instance2);
    message->setTargetInstance(instance);

    MscMessage *message2 = new MscMessage("Msg_2");
    message2->setSourceInstance(instance);
    message2->setTargetInstance(instance2);

    MscCondition *condition = new MscCondition("Con_1");
    condition->setShared(true);
    condition->setInstance(instance);

    MscCondition *condition2 = new MscCondition("Con_2");
    condition2->setInstance(instance2);

    MscTimer *timer1 = new MscTimer("T_1", MscTimer::TimerType::Start);
    timer1->setInstance(instance);

    MscTimer *timer2 = new MscTimer("T_2", MscTimer::TimerType::Stop);
    timer2->setInstance(instance2);

    chart.addInstanceEvent(condition);
    chart.addInstanceEvent(timer1);
    chart.addInstanceEvent(message);
    chart.addInstanceEvent(condition2);
    chart.addInstanceEvent(message2);
    chart.addInstanceEvent(timer2);

    chart.addInstance(instance);
    chart.addInstance(instance2);

    QStringList serializeList = this->serialize(&chart).split("\n");

    QVERIFY(serializeList.size() >= 14);

    QCOMPARE(serializeList.at(0), QString("msc Chart_1;"));

    QCOMPARE(serializeList.at(1), tab1("instance Inst_1;"));
    QCOMPARE(serializeList.at(2), tab2("condition Con_1 shared all;"));
    QCOMPARE(serializeList.at(3), tab2("starttimer T_1;"));
    QCOMPARE(serializeList.at(4), tab2("in Msg_1 from Inst_2;"));
    QCOMPARE(serializeList.at(5), tab2("out Msg_2 to Inst_2;"));
    QCOMPARE(serializeList.at(6), tab1("endinstance;"));

    QCOMPARE(serializeList.at(7), tab1("instance Inst_2;"));
    QCOMPARE(serializeList.at(8), tab2("out Msg_1 to Inst_1;"));
    QCOMPARE(serializeList.at(9), tab2("condition Con_2;"));
    QCOMPARE(serializeList.at(10), tab2("in Msg_2 from Inst_1;"));
    QCOMPARE(serializeList.at(11), tab2("stoptimer T_2;"));
    QCOMPARE(serializeList.at(12), tab1("endinstance;"));

    QCOMPARE(serializeList.at(13), QString("endmsc;"));
}

void tst_MscWriter::testSerializeMscDocument()
{
    MscDocument document("Doc_1");
    QCOMPARE(this->serialize(&document), QString("mscdocument Doc_1 /* MSC AND */;\nendmscdocument;\n"));

    // check hierarchy
    document.addDocument(new MscDocument("Doc_2"));
    document.addDocument(new MscDocument("Doc_3"));
    QStringList serializeList = this->serialize(&document).split("\n");
    QVERIFY(serializeList.size() >= 6);
    QCOMPARE(serializeList.at(0), QString("mscdocument Doc_1 /* MSC AND */;"));
    QCOMPARE(serializeList.at(1), tab1("mscdocument Doc_2 /* MSC AND */;"));
    QCOMPARE(serializeList.at(2), tab1("endmscdocument;"));
    QCOMPARE(serializeList.at(3), tab1("mscdocument Doc_3 /* MSC AND */;"));
    QCOMPARE(serializeList.at(4), tab1("endmscdocument;"));
    QCOMPARE(serializeList.at(5), QString("endmscdocument;"));
}

void tst_MscWriter::testSerializeMscDocumentChart()
{
    MscDocument document("Doc_1");

    MscChart *chart = new MscChart("Chart_1");
    MscInstance *instance1 = new MscInstance("Inst_1");
    MscInstance *instance2 = new MscInstance("Inst_2");
    MscInstance *instance3 = new MscInstance("Inst_3");

    chart->addInstance(instance1);
    chart->addInstance(instance2);
    chart->addInstance(instance3);

    document.addChart(chart);
    document.addChart(new MscChart("Chart_2"));

    QStringList serializeList = this->serialize(&document).split("\n");

    QVERIFY(serializeList.size() >= 12);

    QCOMPARE(serializeList.at(0), QString("mscdocument Doc_1 /* MSC AND */;"));

    QCOMPARE(serializeList.at(1), tab1("msc Chart_1;"));
    QCOMPARE(serializeList.at(2), tab2("instance Inst_1;"));
    QCOMPARE(serializeList.at(3), tab2("endinstance;"));
    QCOMPARE(serializeList.at(4), tab2("instance Inst_2;"));
    QCOMPARE(serializeList.at(5), tab2("endinstance;"));
    QCOMPARE(serializeList.at(6), tab2("instance Inst_3;"));
    QCOMPARE(serializeList.at(7), tab2("endinstance;"));
    QCOMPARE(serializeList.at(8), tab1("endmsc;"));

    QCOMPARE(serializeList.at(9), tab1("msc Chart_2;"));
    QCOMPARE(serializeList.at(10), tab1("endmsc;"));

    QCOMPARE(serializeList.at(11), QString("endmscdocument;"));
}

void tst_MscWriter::testSerializeDataDefinition()
{
    MscModel model;
    model.addDocument(new MscDocument("automade", &model));
    model.setDataLanguage("ASN.1");
    model.setDataDefinitionString("TPos.asn");

    setModel(&model);

    QStringList serializeList = serialize(model.documents().at(0)).split("\n");
    QCOMPARE(serializeList.size(), 5);
    QCOMPARE(serializeList.at(0), QString("mscdocument automade /* MSC AND */;"));
    QCOMPARE(serializeList.at(1), tab1("language ASN.1;"));
    QCOMPARE(serializeList.at(2), tab1("data TPos.asn;"));
    QCOMPARE(serializeList.at(3), QString("endmscdocument;"));
}

void tst_MscWriter::testSerializeCreate()
{
    MscInstance instanceSource("Inst_1");
    MscInstance instanceSubscriber("subscriber");
    MscInstance instanceSubscriber2("subscriber2");
    QVector<MscInstanceEvent *> messages;

    QScopedPointer<MscCreate> createSubscriber(new MscCreate());
    createSubscriber->setSourceInstance(&instanceSource);
    createSubscriber->setTargetInstance(&instanceSubscriber);
    MscParameterList parameters;
    MscParameter parameter;
    parameter.setPattern("data1");
    parameters << parameter;
    parameter.setPattern("data2");
    parameters << parameter;
    createSubscriber->setParameters(parameters);

    messages.append(createSubscriber.data());

    QScopedPointer<MscMessage> message(new MscMessage("Msg_1"));
    message->setTargetInstance(&instanceSource);
    messages.append(message.data());

    QScopedPointer<MscCreate> createSubscriber2(new MscCreate());
    createSubscriber2->setSourceInstance(&instanceSource);
    createSubscriber2->setTargetInstance(&instanceSubscriber2);
    messages.append(createSubscriber2.data());

    const QStringList &serializeList = this->serialize(&instanceSource, messages).split("\n", QString::SkipEmptyParts);

    QVERIFY(serializeList.size() >= 5);

    QCOMPARE(serializeList.at(0), QString("instance Inst_1;"));
    QCOMPARE(serializeList.at(1), tab1("create subscriber(data1, data2);"));
    QCOMPARE(serializeList.at(2), tab1("in Msg_1 from env;"));
    QCOMPARE(serializeList.at(3), tab1("create subscriber2;"));
    QCOMPARE(serializeList.at(4), QString("endinstance;"));
}

void tst_MscWriter::testSerializeChartWithCreate()
{
    MscChart chart("Chart01");
    auto instance1 = new MscInstance("Instance01");
    chart.addInstance(instance1);
    auto instance2 = new MscInstance("Instance02");
    chart.addInstance(instance2);
    chart.addInstance(new MscInstance("Instance03"));

    auto create = new MscCreate();
    create->setSourceInstance(instance1);
    create->setTargetInstance(instance2);
    chart.addInstanceEvent(create);

    QStringList serializeList = this->serialize(&chart).split("\n", QString::SkipEmptyParts);
    QVERIFY(serializeList.size() >= 9);
    QCOMPARE(serializeList.at(0), QString("msc Chart01;"));
    QCOMPARE(serializeList.at(1), tab1("instance Instance01;"));
    QCOMPARE(serializeList.at(2), tab2("create Instance02;"));
    QCOMPARE(serializeList.at(3), tab1("endinstance;"));
    QCOMPARE(serializeList.at(4), tab1("instance Instance02;"));
    QCOMPARE(serializeList.at(5), tab1("endinstance;"));
    QCOMPARE(serializeList.at(6), tab1("instance Instance03;"));
    QCOMPARE(serializeList.at(7), tab1("endinstance;"));
    QCOMPARE(serializeList.at(8), QString("endmsc;"));
}

void tst_MscWriter::testSerializeComments()
{
    MscDocument document("Doc_1");
    document.setCommentString("Doc1 comment");

    MscChart *chart = new MscChart("Chart_1");
    document.addChart(chart);

    MscInstance *instance1 = new MscInstance("Inst_1");
    chart->addInstanceEvent(instance1->setCommentString("Inst1 comment"));
    chart->addInstance(instance1);

    MscMessage *message = new MscMessage("Msg_1");
    chart->addInstanceEvent(message->setCommentString("Msg1 comment"));
    message->setTargetInstance(instance1);
    chart->addInstanceEvent(message);

    MscAction *action = new MscAction();
    action->setInformalAction("Stop");
    chart->addInstanceEvent(action->setCommentString("Action1 comment"));
    action->setInstance(instance1);
    chart->addInstanceEvent(action);

    QStringList serializeList = this->serialize(&document).split("\n");

    QVERIFY(serializeList.size() >= 8);
    QCOMPARE(serializeList.at(0), QString("mscdocument Doc_1 comment 'Doc1 comment' /* MSC AND */;"));
    QCOMPARE(serializeList.at(1), tab1("msc Chart_1;"));
    QCOMPARE(serializeList.at(2), tab2("instance Inst_1 comment 'Inst1 comment';"));
    QCOMPARE(serializeList.at(3), tab3("in Msg_1 from env comment 'Msg1 comment';"));
    QCOMPARE(serializeList.at(4), tab3("action 'Stop' comment 'Action1 comment';"));
    QCOMPARE(serializeList.at(5), tab2("endinstance;"));
    QCOMPARE(serializeList.at(6), tab1("endmsc;"));
    QCOMPARE(serializeList.at(7), QString("endmscdocument;"));
}

void tst_MscWriter::testSerializeMessageDeclarations()
{
    MscDocument document("Doc_1");
    auto md = new MscMessageDeclaration(&document);
    md->setNames({ "gui_send_tm", "pepe" });
    md->setTypeRefList({ { "str" }, { "T-POS" } });
    document.messageDeclarations()->append(md);

    QStringList serializeList = this->serialize(&document).split("\n");
    QVERIFY(serializeList.size() >= 3);
    QCOMPARE(serializeList.at(0), QString("mscdocument Doc_1 /* MSC AND */;"));
    QCOMPARE(serializeList.at(1), tab1("msg gui_send_tm, pepe : (str, T-POS);"));
    QCOMPARE(serializeList.at(2), QString("endmscdocument;"));
}

QTEST_MAIN(tst_MscWriter)

#include "tst_mscwriter.moc"
