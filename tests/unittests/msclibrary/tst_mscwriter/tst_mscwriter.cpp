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

#include <QtTest>

#include "mscchart.h"
#include "msccondition.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscwriter.h"

using namespace msc;

class tst_MscWriter : public MscWriter
{
    Q_OBJECT

private Q_SLOTS:
    void testSerializeMscMessage();
    void testSerializeMscMessageParameters();
    void testSerializeMscInstance();
    void testSerializeMscInstanceKind();
    void testSerializeMscInstanceEvents();
    void testSerializeMscConditions();
    void testSerializeMscChart();
    void testSerializeMscChartInstance();
    void testSerializeMscDocument();
    void testSerializeMscDocumentChart();
};

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

void tst_MscWriter::testSerializeMscInstance()
{
    MscInstance instance("Inst_1");

    QCOMPARE(this->serialize(&instance, QVector<MscMessage *>(), QVector<MscCondition *>()),
             QString("instance Inst_1;\nendinstance;\n"));
}

void tst_MscWriter::testSerializeMscInstanceKind()
{
    MscInstance instance("Inst_1");
    instance.setKind("process");
    instance.setInheritance("P1");

    QCOMPARE(this->serialize(&instance, QVector<MscMessage *>(), QVector<MscCondition *>()),
             QString("instance Inst_1: process P1;\nendinstance;\n"));
}

void tst_MscWriter::testSerializeMscInstanceEvents()
{
    MscInstance instance("Inst_1");

    MscMessage *message = new MscMessage("Msg_1");
    MscInstance instance2("Inst_2");

    message->setSourceInstance(&instance2);
    message->setTargetInstance(&instance);

    MscMessage *message2 = new MscMessage("Msg_2");
    message2->setSourceInstance(&instance);
    message2->setTargetInstance(&instance2);

    QVector<MscMessage *> messages;
    messages.append(message);
    messages.append(message2);

    QStringList serializeList = this->serialize(&instance, messages, QVector<MscCondition *>()).split("\n");

    QVERIFY(serializeList.size() >= 4);

    QCOMPARE(serializeList.at(0), QString("instance Inst_1;"));
    QCOMPARE(serializeList.at(1), QString("   in Msg_1 from Inst_2;"));
    QCOMPARE(serializeList.at(2), QString("   out Msg_2 to Inst_2;"));
    QCOMPARE(serializeList.at(3), QString("endinstance;"));
}

void tst_MscWriter::testSerializeMscConditions()
{
    MscInstance instance("Inst_1");

    MscMessage *message = new MscMessage("Msg_1");
    message->setTargetInstance(&instance);

    QVector<MscMessage *> messages;
    messages.append(message);

    MscCondition *condition = new MscCondition("Con_1");
    condition->setInstance(&instance);

    QVector<MscCondition *> conditions;
    conditions.append(condition);

    QStringList serializeList = this->serialize(&instance, messages, conditions).split("\n");

    QVERIFY(serializeList.size() >= 4);

    QCOMPARE(serializeList.at(0), QString("instance Inst_1;"));
    QCOMPARE(serializeList.at(1), QString("   condition Con_1;"));
    QCOMPARE(serializeList.at(2), QString("   in Msg_1 from Inst_2;"));
    QCOMPARE(serializeList.at(3), QString("endinstance;"));
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
    condition2->setMessageName("Msg_1");

    chart.addMessage(message);
    chart.addMessage(message2);

    chart.addInstance(instance);
    chart.addInstance(instance2);

    chart.addCondition(condition);
    chart.addCondition(condition2);

    QStringList serializeList = this->serialize(&chart).split("\n");

    QVERIFY(serializeList.size() >= 12);

    QCOMPARE(serializeList.at(0), QString("msc Chart_1;"));

    QCOMPARE(serializeList.at(1), QString("   instance Inst_1;"));
    QCOMPARE(serializeList.at(2), QString("      condition Con_1 shared all;"));
    QCOMPARE(serializeList.at(3), QString("      in Msg_1 from Inst_2;"));
    QCOMPARE(serializeList.at(4), QString("      out Msg_2 to Inst_2;"));
    QCOMPARE(serializeList.at(5), QString("   endinstance;"));

    QCOMPARE(serializeList.at(6), QString("   instance Inst_2;"));
    QCOMPARE(serializeList.at(7), QString("      out Msg_1 to Inst_1;"));
    QCOMPARE(serializeList.at(8), QString("      condition Con_2;"));
    QCOMPARE(serializeList.at(9), QString("      in Msg_2 from Inst_1;"));
    QCOMPARE(serializeList.at(10), QString("   endinstance;"));

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

    QCOMPARE(serializeList.at(1), QString("   msc Chart_1;"));
    QCOMPARE(serializeList.at(2), QString("      instance Inst_1;"));
    QCOMPARE(serializeList.at(3), QString("      endinstance;"));
    QCOMPARE(serializeList.at(4), QString("      instance Inst_2;"));
    QCOMPARE(serializeList.at(5), QString("      endinstance;"));
    QCOMPARE(serializeList.at(6), QString("      instance Inst_3;"));
    QCOMPARE(serializeList.at(7), QString("      endinstance;"));
    QCOMPARE(serializeList.at(8), QString("   endmsc;"));

    QCOMPARE(serializeList.at(9), QString("   msc Chart_2;"));
    QCOMPARE(serializeList.at(10), QString("   endmsc;"));

    QCOMPARE(serializeList.at(11), QString("endmscdocument;"));
}

QTEST_APPLESS_MAIN(tst_MscWriter)

#include "tst_mscwriter.moc"
