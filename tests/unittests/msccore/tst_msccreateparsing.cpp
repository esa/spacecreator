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

#include "exceptions.h"
#include "mscchart.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "tst_mscreader.h"

#include <QScopedPointer>

using namespace msc;

void tst_MscReader::testInstanceCreate()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         in ICONreq from env; \
                         create subscriber(data); \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;";

    QScopedPointer<MscModel> model(m_reader->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(chart->totalEventNumber(), 2);

    auto *create = static_cast<MscCreate *>(chart->eventsForInstance(instance).at(1));
    QCOMPARE(create->name(), QString());
    QCOMPARE(create->sourceInstance()->name(), QString("Inst_1"));
    QCOMPARE(create->targetInstance()->name(), QString("subscriber"));

    MscParameterList parameters = create->parameters();
    QCOMPARE(parameters.size(), 1);
    QCOMPARE(parameters.at(0).pattern(), QString("data"));
    QCOMPARE(parameters.at(0).expression(), QString());
}

void tst_MscReader::testInstanceCreateNoParameter()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber; \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;";

    QScopedPointer<MscModel> model(m_reader->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(chart->totalEventNumber(), 1);

    auto *create = static_cast<MscCreate *>(chart->eventsForInstance(instance).at(0));
    QCOMPARE(create->name(), QString());
    QCOMPARE(create->sourceInstance()->name(), QString("Inst_1"));
    QCOMPARE(create->targetInstance()->name(), QString("subscriber"));
    QVERIFY(create->parameters().isEmpty());
}

void tst_MscReader::testInstanceCreateMultiParameter()
{
    static const QStringList paramsIn = { "data1", "data2", "data3" };
    static const QString msc = QString("MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber(%1); \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;")
                                       .arg(paramsIn.join(", "));

    QScopedPointer<MscModel> model(m_reader->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(chart->totalEventNumber(), 1);

    auto *create = static_cast<MscCreate *>(chart->eventsForInstance(instance).at(0));
    QCOMPARE(create->name(), QString());
    QCOMPARE(create->sourceInstance()->name(), QString("Inst_1"));
    QCOMPARE(create->targetInstance()->name(), QString("subscriber"));

    QStringList paramsOut;
    for (const MscParameter &param : create->parameters()) {
        paramsOut << param.pattern();
    }

    QCOMPARE(paramsOut.size(), paramsIn.size());

    for (int i = 0; i < paramsOut.size(); ++i) {
        QCOMPARE(paramsOut.at(i), paramsIn.at(i));
    }
}

void tst_MscReader::testInstanceCreateEmptyParameter()
{
    static const QString msc = QString("MSC msc1; \
                      INSTANCE Inst_1; \
                         out Heartbeat,120() to subscriber; \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                         in Heartbeat,120() from Inst_1; \
                      ENDINSTANCE; \
                   ENDMSC;");

    QScopedPointer<MscModel> model(m_reader->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(chart->totalEventNumber(), 1);

    auto *msg = static_cast<MscCreate *>(chart->eventsForInstance(instance).at(0));
    QCOMPARE(msg->fullName(), QString("Heartbeat,120"));
    QVERIFY(msg->parameters().isEmpty());
}

void tst_MscReader::testInstanceCreateNoInstance()
{
    static const QLatin1String msc("MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber2(data1); \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;");

    QVERIFY_EXCEPTION_THROWN(m_reader->parseText(msc), ParserException);
}

void tst_MscReader::testInstanceCreateDublicate()
{
    static const QLatin1String msc("MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber(data1); \
                         create subscriber(data2); \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;");
    QVERIFY_EXCEPTION_THROWN(m_reader->parseText(msc), ParserException);
}

void tst_MscReader::testMessageCreateInstance()
{
    static const QVector<QString> msgNames = { "Msg01", "", "Msg02", "Msg03", "Msg04", "Msg05" };
    static const QString msc = QString("mscdocument Doc2; \
                  msc msc3; \
                      instance Instance_A; \
                          out %1 to Instance_B; \
                          create New_Instance1; \
                          in %2 from Instance_B; \
                          out %3 to New_Instance1; \
                          out %5 to New_Instance1; \
                      endinstance; \
                      instance New_Instance1; \
                          in %3 from Instance_A; \
                          out %4 to Instance_B; \
                          in %5 from Instance_A; \
                      endinstance; \
                      instance Instance_B; \
                          in %1 from Instance_A; \
                          out %2 to Instance_A; \
                          in %4 from New_Instance1; \
                      stop; \
                  endmsc; \
              endmscdocument;")
                                       .arg(msgNames[0], msgNames[2], msgNames[3], msgNames[4], msgNames[5]);

    QScopedPointer<MscModel> model(m_reader->parseText(msc));
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().first()->charts().size(), 1);

    MscChart *chart = model->documents().first()->charts().first();
    QCOMPARE(chart->instances().size(), 3);
    MscInstance *instance1 = chart->instances().at(1);

    QCOMPARE(chart->totalEventNumber(), 6);
    QVector<MscInstanceEvent *> events = chart->eventsForInstance(instance1);
    QCOMPARE(events.size(), 4);
    MscMessage *message = dynamic_cast<MscMessage *>(events.at(0));
    QVERIFY(message != nullptr);
    QCOMPARE(message->messageType(), MscMessage::MessageType::Create);
}
