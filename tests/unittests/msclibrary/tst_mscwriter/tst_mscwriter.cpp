/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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
#include "msccondition.h"
#include "msccoregion.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "msctimer.h"
#include "mscwriter.h"

#include <QtTest>

using namespace msc;

class tst_MscWriter : public MscWriter
{
    Q_OBJECT

private Q_SLOTS:
    void cleanup();

    void testSelthTabbing();
    void testSerializeMscMessage();
    void testSerializeMscMessageParameters();
    void testSerializeMscTimer();
    void testSerializeMscCoregion();
    void testSerializeMscInstance();
    void testSerializeMscInstanceStop();
    void testSerializeMscInstanceKind();
    void testSerializeMscInstanceEvents();
    void testSerializeMscConditions();
    void testSerializeMscActionsInformal();
    void testSerializeMscActionsFormal();
    void testSerializeMscChart();
    void testSerializeMscChartInstance();
    void testSerializeMscDocument();
    void testSerializeMscDocumentChart();
    void testSerializeDataDefinition();
    void testSerializeCreate();
    void testSerializeComments();

private:
    static const QString TabSpaces;
    static QString prependTabs(const QString &str, int count);
    static QString tab1(const QString &str);
    static QString tab2(const QString &str);
    static QString tab3(const QString &str);
};

const QString tst_MscWriter::TabSpaces = QString("    "); // 4 space chars

QString tst_MscWriter::prependTabs(const QString &str, int count)
{
    QString res;
    for (int i = 0; i < count; ++i)
        res += TabSpaces;
    return res + str;
}

QString tst_MscWriter::tab1(const QString &str)
{
    return prependTabs(str, 1);
}

QString tst_MscWriter::tab2(const QString &str)
{
    return prependTabs(str, 2);
}

QString tst_MscWriter::tab3(const QString &str)
{
    return prependTabs(str, 3);
}

void tst_MscWriter::cleanup()
{
    setModel(nullptr);
}

void tst_MscWriter::testSelthTabbing()
{
    static const QString tabTest("tabTest");
    QCOMPARE(tab1(tabTest), TabSpaces + tabTest);
    QCOMPARE(tab2(tabTest), TabSpaces + TabSpaces + tabTest);
    QCOMPARE(tab3(tabTest), TabSpaces + TabSpaces + TabSpaces + tabTest);
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
    message.setParameters({ "a", "longitude:-174.0", "" });

    MscInstance source("Inst_1");
    MscInstance target("Inst_2");

    message.setSourceInstance(&source);
    message.setTargetInstance(&target);

    QCOMPARE(this->serialize(&message, &source), QString("out Msg_1,a(longitude:-174.0) to Inst_2;\n"));
}

void tst_MscWriter::testSerializeMscTimer()
{
    MscTimer timer1("T1", MscTimer::TimerType::Start);
    QCOMPARE(this->serialize(&timer1), QString("starttimer T1;\n"));
    QCOMPARE(this->serialize(&timer1, 1), tab1("starttimer T1;\n"));
    QCOMPARE(this->serialize(&timer1, 2), tab2("starttimer T1;\n"));

    MscTimer timer2("T2", MscTimer::TimerType::Stop);
    QCOMPARE(this->serialize(&timer2), QString("stoptimer T2;\n"));

    MscTimer timer3("T3", MscTimer::TimerType::Timeout);
    QCOMPARE(this->serialize(&timer3), QString("timeout T3;\n"));
}

void tst_MscWriter::testSerializeMscCoregion()
{
    MscCoregion region1(MscCoregion::Type::Begin);
    QCOMPARE(this->serialize(&region1), QString("concurrent;\n"));
    QCOMPARE(this->serialize(&region1, 1), tab1("concurrent;\n"));
    QCOMPARE(this->serialize(&region1, 2), tab2("concurrent;\n"));

    MscCoregion region2(MscCoregion::Type::End);
    QCOMPARE(this->serialize(&region2), QString("endconcurrent;\n"));
}

void tst_MscWriter::testSerializeMscInstance()
{
    MscInstance instance("Inst_1");

    QCOMPARE(this->serialize(&instance, QVector<MscInstanceEvent *>()), QString("instance Inst_1;\nendinstance;\n"));
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
    instance.setKind("process");
    instance.setInheritance("P1");

    QCOMPARE(this->serialize(&instance, QVector<MscInstanceEvent *>()),
             QString("instance Inst_1: process P1;\nendinstance;\n"));
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
    messages.insert(1, timer1.data());
    QScopedPointer<MscTimer> timer2(new MscTimer("T_fire", MscTimer::TimerType::Timeout));
    messages.append(timer2.data());
    QScopedPointer<MscTimer> timer3(new MscTimer("T_stop", MscTimer::TimerType::Stop));
    messages.append(timer3.data());

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

void tst_MscWriter::testSerializeMscConditions()
{
    MscInstance instance("Inst_1");

    QScopedPointer<MscMessage> message(new MscMessage("Msg_1"));
    message->setTargetInstance(&instance);

    QScopedPointer<MscCondition> condition(new MscCondition("Con_1"));
    condition->setInstance(&instance);

    QVector<MscInstanceEvent *> messages;
    messages.append(condition.data());
    messages.append(message.data());

    QStringList serializeList = this->serialize(&instance, messages).split("\n", QString::SkipEmptyParts);

    QVERIFY(serializeList.size() >= 4);

    QCOMPARE(serializeList.at(0), QString("instance Inst_1;"));
    QCOMPARE(serializeList.at(1), tab1("condition Con_1;"));
    QCOMPARE(serializeList.at(2), tab1("in Msg_1 from env;"));
    QCOMPARE(serializeList.at(3), QString("endinstance;"));
}

void tst_MscWriter::testSerializeMscActionsInformal()
{
    MscInstance instance("Inst_1");

    QScopedPointer<MscAction> action(new MscAction());
    action->setActionType(MscAction::ActionType::Informal);
    action->setInformalAction("informal_stop");
    action->setInstance(&instance);

    QVector<MscInstanceEvent *> events;
    events.append(action.data());

    QStringList serializeList = this->serialize(&instance, events).split("\n", QString::SkipEmptyParts);
    QCOMPARE(serializeList.size(), 3);
    QCOMPARE(serializeList.at(0), QString("instance Inst_1;"));
    QCOMPARE(serializeList.at(1), tab1("action 'informal_stop';"));
    QCOMPARE(serializeList.at(2), QString("endinstance;"));

    MscInstance instance2("Inst_2");
    serializeList = this->serialize(&instance2, events).split("\n", QString::SkipEmptyParts);
    QCOMPARE(serializeList.size(), 2);
    QCOMPARE(serializeList.at(0), QString("instance Inst_2;"));
    QCOMPARE(serializeList.at(1), QString("endinstance;"));
}

void tst_MscWriter::testSerializeMscActionsFormal()
{
    MscInstance instance("Inst_1");

    QScopedPointer<MscAction> action(new MscAction());
    action->setActionType(MscAction::ActionType::Formal);
    MscAction::DataStatement statement;
    statement.m_type = MscAction::DataStatement::StatementType::Define;
    statement.m_variableString = "digit1";
    action->addDataStatement(statement);

    statement.m_type = MscAction::DataStatement::StatementType::UnDefine;
    statement.m_variableString = "digit2";
    action->addDataStatement(statement);

    action->setInstance(&instance);

    QVector<MscInstanceEvent *> events;
    events.append(action.data());

    QStringList serializeList = this->serialize(&instance, events).split("\n", QString::SkipEmptyParts);
    QCOMPARE(serializeList.size(), 3);
    QCOMPARE(serializeList.at(0), QString("instance Inst_1;"));
    QCOMPARE(serializeList.at(1), tab1("action def digit1, undef digit2;"));
    QCOMPARE(serializeList.at(2), QString("endinstance;"));
}

void tst_MscWriter::testSerializeMscChart()
{
    MscChart chart("Chart_1");
    QCOMPARE(this->serialize(&chart), QString("msc Chart_1;\nendmsc;\n"));
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

    chart.addInstanceEvent(condition);
    chart.addInstanceEvent(message);
    chart.addInstanceEvent(condition2);
    chart.addInstanceEvent(message2);

    chart.addInstance(instance);
    chart.addInstance(instance2);

    QStringList serializeList = this->serialize(&chart).split("\n");

    QVERIFY(serializeList.size() >= 12);

    QCOMPARE(serializeList.at(0), QString("msc Chart_1;"));

    QCOMPARE(serializeList.at(1), tab1("instance Inst_1;"));
    QCOMPARE(serializeList.at(2), tab2("condition Con_1 shared all;"));
    QCOMPARE(serializeList.at(3), tab2("in Msg_1 from Inst_2;"));
    QCOMPARE(serializeList.at(4), tab2("out Msg_2 to Inst_2;"));
    QCOMPARE(serializeList.at(5), tab1("endinstance;"));

    QCOMPARE(serializeList.at(6), tab1("instance Inst_2;"));
    QCOMPARE(serializeList.at(7), tab2("out Msg_1 to Inst_1;"));
    QCOMPARE(serializeList.at(8), tab2("condition Con_2;"));
    QCOMPARE(serializeList.at(9), tab2("in Msg_2 from Inst_1;"));
    QCOMPARE(serializeList.at(10), tab1("endinstance;"));

    QCOMPARE(serializeList.at(11), QString("endmsc;"));
}

void tst_MscWriter::testSerializeMscDocument()
{
    MscDocument document("Doc_1");
    QCOMPARE(this->serialize(&document), QString("mscdocument Doc_1;\nendmscdocument;\n"));
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

    QCOMPARE(serializeList.at(0), QString("mscdocument Doc_1;"));

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
    QCOMPARE(serializeList.at(0), QString("mscdocument automade;"));
    QCOMPARE(serializeList.at(1), tab1("language ASN.1;"));
    QCOMPARE(serializeList.at(2), tab1("data TPos.asn;"));
    QCOMPARE(serializeList.at(3), QString("endmscdocument;"));
}

void tst_MscWriter::testSerializeCreate()
{
    MscInstance instance("Inst_1");

    QScopedPointer<MscCreate> create(new MscCreate("subscriber"));
    create->addParameter("data1");
    create->addParameter("data2");
    create->setInstance(&instance);

    QVector<MscInstanceEvent *> messages;
    messages.append(create.data());

    QScopedPointer<MscMessage> message(new MscMessage("Msg_1"));
    message->setTargetInstance(&instance);
    messages.append(message.data());

    QScopedPointer<MscCreate> create2(new MscCreate("subscriber2"));
    create2->setInstance(&instance);
    messages.append(create2.data());

    QStringList serializeList = this->serialize(&instance, messages).split("\n", QString::SkipEmptyParts);

    QVERIFY(serializeList.size() >= 5);

    QCOMPARE(serializeList.at(0), QString("instance Inst_1;"));
    QCOMPARE(serializeList.at(1), tab1("create subscriber(data1, data2);"));
    QCOMPARE(serializeList.at(2), tab1("in Msg_1 from env;"));
    QCOMPARE(serializeList.at(3), tab1("create subscriber2;"));
    QCOMPARE(serializeList.at(4), QString("endinstance;"));
}

void tst_MscWriter::testSerializeComments()
{
    MscDocument document("Doc_1");
    document.setComment("Doc1 comment");

    MscChart *chart = new MscChart("Chart_1");
    chart->setComment("Chart1 comment");
    document.addChart(chart);

    MscInstance *instance1 = new MscInstance("Inst_1");
    instance1->setComment("Inst1 comment");
    chart->addInstance(instance1);

    MscMessage *message = new MscMessage("Msg_1");
    message->setComment("Msg1 comment");
    message->setTargetInstance(instance1);
    chart->addInstanceEvent(message);

    MscAction *action = new MscAction();
    action->setInformalAction("Stop");
    action->setComment("Action1 comment");
    action->setInstance(instance1);
    chart->addInstanceEvent(action);

    QStringList serializeList = this->serialize(&document).split("\n");

    QVERIFY(serializeList.size() >= 8);
    QCOMPARE(serializeList.at(0), QString("mscdocument Doc_1 comment 'Doc1 comment';"));
    QCOMPARE(serializeList.at(1), tab1("msc Chart_1 comment 'Chart1 comment';"));
    QCOMPARE(serializeList.at(2), tab2("instance Inst_1 comment 'Inst1 comment';"));
    QCOMPARE(serializeList.at(3), tab3("in Msg_1 from env comment 'Msg1 comment';"));
    QCOMPARE(serializeList.at(4), tab3("action 'Stop' comment 'Action1 comment';"));
    QCOMPARE(serializeList.at(5), tab2("endinstance;"));
    QCOMPARE(serializeList.at(6), tab1("endmsc;"));
    QCOMPARE(serializeList.at(7), QString("endmscdocument;"));
}

QTEST_APPLESS_MAIN(tst_MscWriter)

#include "tst_mscwriter.moc"
