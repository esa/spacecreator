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
#include "msccondition.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscfile.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "msctimer.h"

#include <QCoreApplication>
#include <QScopedPointer>
#include <QtTest>

using namespace msc;

class tst_MscEventsParsing : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    // Currently here is only the message-related stuff,
    // see the tst_MscFile for other

    void testMessage();
    void testSameMessageInTwoInstances();
    void testMessageWithParameters();
    void testMessageParameterWildcard();
    void testMessageParameterExpression();
    void testMultiParameters();

    void testInstanceCreate();
    void testInstanceCreateNoParameter();
    void testInstanceCreateMultiParameter();
    void testInstanceCreateEmptyParameter();
    void testInstanceCreateNoInstance();
    void testInstanceCreateDublicate();

    void testSortedMessage();
    void testSortedMessageTwoCharts();
    void testSortedInstanceEvents();

    void testMessageCreateInstance();

    void testIncompleteMessageIn();
    void testIncompleteMessageOut();

    void testConditionDublicate();

    void testTestMessageInstanceName();
    void testSameNameDifferentInstances();
    void testDifferentParameter();
    void testMultiMessageOccurrence();

private:
    MscFile *file = nullptr;
};

void tst_MscEventsParsing::init()
{
    file = new MscFile;
}

void tst_MscEventsParsing::cleanup()
{
    delete file;
    file = nullptr;
}

void tst_MscEventsParsing::testMessage()
{
    QString msc = "MSC msc1; \
        INSTANCE inst1; \
            in ICONreq from env; \
            in ICONreq2 from ; \
            out ICON to Responder; \
        ENDINSTANCE; \
    ENDMSC;";
    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    MscInstance *instance = chart->instances().at(0);

    QCOMPARE(chart->instanceEvents().size(), 3);
    MscMessage *message1 = dynamic_cast<MscMessage *>(chart->instanceEvents().at(0));
    QVERIFY(message1 != nullptr);
    QCOMPARE(message1->name(), QString("ICONreq"));
    QCOMPARE(message1->sourceInstance(), static_cast<MscInstance *>(nullptr));
    QCOMPARE(message1->targetInstance(), instance);

    MscMessage *message2 = dynamic_cast<MscMessage *>(chart->instanceEvents().at(2));
    QVERIFY(message2 != nullptr);
    QCOMPARE(message2->name(), QString("ICON"));
    QCOMPARE(message2->sourceInstance(), instance);
    QCOMPARE(message2->targetInstance(), static_cast<MscInstance *>(nullptr));
}

void tst_MscEventsParsing::testSameMessageInTwoInstances()
{
    QString msc = "MSC msc1; \
                  INSTANCE Initiator;in ICON from Responder;ENDINSTANCE; \
                  INSTANCE Responder;out ICON to Initiator;ENDINSTANCE; \
                  ENDMSC;";
    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 1);
    delete model;
}

void tst_MscEventsParsing::testMessageWithParameters()
{
    QString msc = "MSC msc1; \
                   INSTANCE Initiator; \
                      in gui_send_tm,a(longitude:-174.0) from env; \
                      out gui_int_send_tm(12) to env; \
                      OUT req(data:'Hello' ) TO A2B; \
                      IN userpidgot(pid(connhandler(1))) FROM inst_1_switchctrl; \
                      OUT msg('Sorry, wrong id, try again') TO ENV; \
                      IN pixelrow ( mkpixrow_t( (. 0,0,0 .),(. 128,128,128 .) ) ) FROM inst_1_adconverter; \
                      IN card ( (. 1002,9999,'1701' .) ) FROM inst_1_ui.ui; \
                      OUT q_accept( { cardData (. 1002,9999,'1701' .),amount '400' } ) TO ENV VIA co_tr; \
                   ENDINSTANCE; \
                   ENDMSC;";
    QScopedPointer<MscModel> model(file->parseText(msc));

    QVERIFY(model->charts().size() == 1);
    MscChart *chart = model->charts().at(0);

    QVERIFY(chart->instanceEvents().size() == 8);

    auto *message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(0));
    QVERIFY(message != nullptr);
    QCOMPARE(message->messageInstanceName(), QString("a"));
    MscParameterList parameters = message->parameters();
    QCOMPARE(parameters.size(), 1);
    QCOMPARE(parameters.at(0).expression(), QString("longitude:-174.0"));

#ifndef __clang_analyzer__
    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(1));
    QVERIFY(message != nullptr);
    QVERIFY(message->messageInstanceName().isEmpty());
    parameters = message->parameters();
    QCOMPARE(parameters.at(0).pattern(), QString("12"));

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(6));
    QVERIFY(message != nullptr);
    parameters = message->parameters();
    QCOMPARE(parameters.size(), 1);
    QCOMPARE(parameters.at(0).pattern(), QString("(. 1002,9999,'1701' .)"));
#endif
}

void tst_MscEventsParsing::testMessageParameterWildcard()
{
    QString msc = "MSC msc1; \
                      INSTANCE inst1; \
                         in HeartbeatIn,113({header {functionCode 0, nodeID 0, rtr 0, dlc 0}}) from inst2; \
                         stoptimer ToggleTimer; \
                         out ODWriteObject,118(bDefault:bus-a) to inst2; \
                     ENDINSTANCE; \
                     INSTANCE inst2; \
                         out HeartbeatIn,113({header {functionCode 0, nodeID 0, rtr 0, dlc 0}}) to inst1; \
                         in ODWriteObject,118(bDefault:bus-a) from inst1; \
                     ENDINSTANCE; \
                  ENDMSC;";

    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 3);
}

void tst_MscEventsParsing::testMessageParameterExpression()
{
    QString msc = "MSC msc1; \
                      INSTANCE inst1; \
                        in CanFrameIn,111(heartbeat:{header {functionCode 0, nodeID 0, rtr 0, dlc 0}}) from inst2; \
                     ENDINSTANCE; \
                     INSTANCE inst2; \
                        out CanFrameIn,111(heartbeat:{header {functionCode 0, nodeID 0, rtr 0, dlc 0}}) to inst1; \
                     ENDINSTANCE; \
                  ENDMSC;";

    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 1);
}

void tst_MscEventsParsing::testMultiParameters()
{
    QString msc = "msc Untitled_1; \
       instance Inst_1; \
          out hello(b:   FALSE, 11) to Inst_2; \
       endinstance; \
       instance Inst_2; \
          in hello(b:   FALSE, 11) from Inst_1; \
       endinstance; \
    endmsc;";

    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 1);
    auto *message = static_cast<MscCreate *>(chart->instanceEvents().at(0));
    QCOMPARE(message->name(), QString("hello"));
    MscParameterList parameters = message->parameters();
    QCOMPARE(parameters.size(), 2);
    QCOMPARE(parameters.at(0).expression(), QString("b:   FALSE"));
    QCOMPARE(parameters.at(1).pattern(), QString("11"));
}

void tst_MscEventsParsing::testInstanceCreate()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         in ICONreq from env; \
                         create subscriber(data); \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;";

    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 2);

    auto *create = static_cast<MscCreate *>(chart->instanceEvents().at(1));
    QCOMPARE(create->name(), QString());
    QCOMPARE(create->sourceInstance()->name(), QString("Inst_1"));
    QCOMPARE(create->targetInstance()->name(), QString("subscriber"));

    MscParameterList parameters = create->parameters();
    QCOMPARE(parameters.size(), 1);
    QCOMPARE(parameters.at(0).pattern(), QString("data"));
    QCOMPARE(parameters.at(0).expression(), QString());
}

void tst_MscEventsParsing::testInstanceCreateNoParameter()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber; \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;";

    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 1);

    auto *create = static_cast<MscCreate *>(chart->instanceEvents().at(0));
    QCOMPARE(create->name(), QString());
    QCOMPARE(create->sourceInstance()->name(), QString("Inst_1"));
    QCOMPARE(create->targetInstance()->name(), QString("subscriber"));
    QVERIFY(create->parameters().isEmpty());
}

void tst_MscEventsParsing::testInstanceCreateMultiParameter()
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

    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 1);

    auto *create = static_cast<MscCreate *>(chart->instanceEvents().at(0));
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

void tst_MscEventsParsing::testInstanceCreateEmptyParameter()
{
    static const QString msc = QString("MSC msc1; \
                      INSTANCE Inst_1; \
                         out Heartbeat,120() to subscriber; \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                         in Heartbeat,120() from Inst_1; \
                      ENDINSTANCE; \
                   ENDMSC;");

    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 1);

    auto *msg = static_cast<MscCreate *>(chart->instanceEvents().at(0));
    QCOMPARE(msg->fullName(), QString("Heartbeat,120"));
    QVERIFY(msg->parameters().isEmpty());
}

void tst_MscEventsParsing::testInstanceCreateNoInstance()
{
    static const QLatin1String msc("MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber2(data1); \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;");

    QVERIFY_EXCEPTION_THROWN(file->parseText(msc), ParserException);
}

void tst_MscEventsParsing::testInstanceCreateDublicate()
{
    static const QLatin1String msc("MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber(data1); \
                         create subscriber(data2); \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;");
    QVERIFY_EXCEPTION_THROWN(file->parseText(msc), ParserException);
}

void tst_MscEventsParsing::testSortedMessage()
{
    static const QLatin1String msc("msc connection; \
                      instance Initiator; \
                          in ICONreq from env; \
                          out ICON to Responder; \
                          in ICONF from Responder; \
                          out ICONconf to env; \
                      endinstance; \
                      instance Responder; \
                          in ICON from Initiator; \
                          out ICONind to env; \
                          in ICONresp from env; \
                          out ICONF to Initiator; \
                      endinstance; \
                  endmsc;");

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);

    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 2);
    MscInstance *initiator = chart->instances().at(0);
    MscInstance *responder = chart->instances().at(1);

    QCOMPARE(chart->instanceEvents().size(), 6);

    MscMessage *message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(0));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONreq"));
    QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
    QCOMPARE(message->targetInstance(), initiator);

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(1));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICON"));
    QCOMPARE(message->sourceInstance(), initiator);
    QCOMPARE(message->targetInstance(), responder);

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(2));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONind"));
    QCOMPARE(message->sourceInstance(), responder);
    QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(3));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONresp"));
    QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
    QCOMPARE(message->targetInstance(), responder);

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(4));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONF"));
    QCOMPARE(message->sourceInstance(), responder);
    QCOMPARE(message->targetInstance(), initiator);

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(5));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONconf"));
    QCOMPARE(message->sourceInstance(), initiator);
    QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));

    delete model;
}

void tst_MscEventsParsing::testSortedMessageTwoCharts()
{
    static const QLatin1String msc("mscdocument doc1; \
                      msc connection1; \
                          instance Initiator1; \
                              in ICONreq1 from env; \
                              out ICON1 to Responder1; \
                              in ICONF1 from Responder1; \
                              out ICONconf1 to env; \
                          endinstance; \
                          instance Responder1; \
                              in ICON1 from Initiator1; \
                              out ICONind1 to env; \
                              in ICONresp1 from env; \
                              out ICONF1 to Initiator1; \
                          endinstance; \
                      endmsc; \
                      msc connection2; \
                          instance Initiator2; \
                              in ICONreq2 from env; \
                              out ICON2 to Responder2; \
                              in ICONF2 from Responder2; \
                              out ICONconf2 to env; \
                          endinstance; \
                          instance Responder2; \
                              in ICON2 from Initiator2; \
                              out ICONind2 to env; \
                              in ICONresp2 from env; \
                              out ICONF2 to Initiator2; \
                          endinstance; \
                      endmsc; \
                  endmscdocument;");

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->documents().size(), 1);

    MscDocument *document = model->documents().at(0);
    QCOMPARE(document->charts().size(), 2);

    for (int x = 0; x < document->charts().size(); ++x) {
        MscChart *chart = document->charts().at(x);
        QCOMPARE(chart->instances().size(), 2);

        MscInstance *initiator = chart->instances().at(0);
        MscInstance *responder = chart->instances().at(1);

        QCOMPARE(chart->instanceEvents().size(), 6);

        MscMessage *message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(0));
        QVERIFY(message != nullptr);
        QCOMPARE(message->name(), QString("ICONreq%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
        QCOMPARE(message->targetInstance(), initiator);

        message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(1));
        QVERIFY(message != nullptr);
        QCOMPARE(message->name(), QString("ICON%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), initiator);
        QCOMPARE(message->targetInstance(), responder);

        message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(2));
        QVERIFY(message != nullptr);
        QCOMPARE(message->name(), QString("ICONind%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), responder);
        QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));

        message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(3));
        QVERIFY(message != nullptr);
        QCOMPARE(message->name(), QString("ICONresp%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
        QCOMPARE(message->targetInstance(), responder);

        message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(4));
        QVERIFY(message != nullptr);
        QCOMPARE(message->name(), QString("ICONF%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), responder);
        QCOMPARE(message->targetInstance(), initiator);

        message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(5));
        QVERIFY(message != nullptr);
        QCOMPARE(message->name(), QString("ICONconf%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), initiator);
        QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));
    }

    delete model;
}

void tst_MscEventsParsing::testSortedInstanceEvents()
{
    static const QLatin1String msc("msc connection; \
                      instance Initiator; \
                          starttimer T1; \
                          in ICONreq from env; \
                          out ICON to Responder; \
                          stoptimer T1; \
                          in ICONF from Responder; \
                          out ICONconf to env; \
                          timeout T1; \
                      endinstance; \
                      instance Responder; \
                          in ICON from Initiator; \
                          out ICONind to env; \
                          starttimer T2; \
                          in ICONresp from env; \
                          out ICONF to Initiator; \
                      endinstance; \
                  endmsc;");

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);

    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 2);
    MscInstance *initiator = chart->instances().at(0);
    MscInstance *responder = chart->instances().at(1);

    QCOMPARE(chart->instanceEvents().size(), 10);

    int eventNr = -1;

    MscTimer *timer = dynamic_cast<MscTimer *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(timer != nullptr);
    QCOMPARE(timer->timerType(), MscTimer::TimerType::Start);
    QCOMPARE(timer->name(), QString("T1"));

    MscMessage *message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONreq"));
    QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
    QCOMPARE(message->targetInstance(), initiator);

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICON"));
    QCOMPARE(message->sourceInstance(), initiator);
    QCOMPARE(message->targetInstance(), responder);

    timer = dynamic_cast<MscTimer *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(timer != nullptr);
    QCOMPARE(timer->timerType(), MscTimer::TimerType::Stop);
    QCOMPARE(timer->name(), QString("T1"));

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONind"));
    QCOMPARE(message->sourceInstance(), responder);
    QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));

    timer = dynamic_cast<MscTimer *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(timer != nullptr);
    QCOMPARE(timer->timerType(), MscTimer::TimerType::Start);
    QCOMPARE(timer->name(), QString("T2"));

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONresp"));
    QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
    QCOMPARE(message->targetInstance(), responder);

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONF"));
    QCOMPARE(message->sourceInstance(), responder);
    QCOMPARE(message->targetInstance(), initiator);

    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONconf"));
    QCOMPARE(message->sourceInstance(), initiator);
    QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));

    timer = dynamic_cast<MscTimer *>(chart->instanceEvents().at(++eventNr));
    QVERIFY(timer != nullptr);
    QCOMPARE(timer->timerType(), MscTimer::TimerType::Timeout);
    QCOMPARE(timer->name(), QString("T1"));

    delete model;
}

void tst_MscEventsParsing::testMessageCreateInstance()
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

    // expected messages order is:
    // 0 / 6 "Msg01"
    // 1 / 6 "" // create New_Instance1
    // 2 / 6 "Msg02"
    // 3 / 6 "Msg03"
    // 4 / 6 "Msg04"
    // 5 / 6 "Msg05"

    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().first()->charts().size(), 1);

    MscChart *chart = model->documents().first()->charts().first();
    QCOMPARE(chart->instances().size(), 3);

    QCOMPARE(chart->instanceEvents().size(), 6);

    for (int i = 0; i < chart->instanceEvents().size(); ++i) {
        if (MscMessage *message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(i))) {
            const MscMessage::MessageType expectedType =
                    i == 1 ? MscMessage::MessageType::Create : MscMessage::MessageType::Message;

            QCOMPARE(message->name(), msgNames.at(i));
            QCOMPARE(message->messageType(), expectedType);
        }
    }
}

void tst_MscEventsParsing::testIncompleteMessageIn()
{
    static const QLatin1String msc("mscdocument TestDoc; \
                                     msc msc3; \
                                         instance A; \
                                             out A_to_Env to Env; \
                                             in Env_to_A from Env; \
                                             out A_to_B to B; \
                                         endinstance; \
                                         instance B; \
                                             in A_to_B from A; \
                                             in NO_A_to_B from A; \
                                         endinstance; \
                                     endmsc; \
                                 endmscdocument;");

    QVERIFY_EXCEPTION_THROWN(file->parseText(msc), ParserException);
}

void tst_MscEventsParsing::testIncompleteMessageOut()
{
    static const QLatin1String msc("mscdocument TestDoc; \
                                     msc msc3; \
                                         instance A; \
                                             out A_to_Env to Env; \
                                             in Env_to_A from Env; \
                                             out A_to_B to B; \
                                         endinstance; \
                                         instance B; \
                                             in A_to_B from A; \
                                             out NO_B_to_A to A; \
                                         endinstance; \
                                     endmsc; \
                                 endmscdocument;");

    QVERIFY_EXCEPTION_THROWN(file->parseText(msc), ParserException);
}

void tst_MscEventsParsing::testConditionDublicate()
{
    static const QLatin1String msc("msc connection; \
                    instance Initiator; \
                        condition Disconnected shared all; \
                        in ICONreq from env; \
                        condition Wait_forResp shared all; \
                    endinstance; \
                    instance Responder; \
                        condition Disconnected shared all; \
                        condition Wait_forResp; \
                    endinstance;\
                endmsc;");

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);

    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 2);
    MscInstance *initiator = chart->instances().at(0);
    MscInstance *responder = chart->instances().at(1);

    QCOMPARE(chart->instanceEvents().size(), 4);

    auto event = dynamic_cast<MscCondition *>(chart->instanceEvents().at(0));
    QVERIFY(event != nullptr);
    QCOMPARE(event->name(), QString("Disconnected"));
    QVERIFY(event->shared());
    QCOMPARE(event->instance(), initiator);

    auto message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(1));
    QVERIFY(message != nullptr);
    QCOMPARE(message->name(), QString("ICONreq"));
    QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
    QCOMPARE(message->targetInstance(), initiator);

    event = dynamic_cast<MscCondition *>(chart->instanceEvents().at(2));
    QVERIFY(event != nullptr);
    QCOMPARE(event->name(), QString("Wait_forResp"));
    QVERIFY(event->shared());
    QCOMPARE(event->instance(), initiator);

    event = dynamic_cast<MscCondition *>(chart->instanceEvents().at(3));
    QVERIFY(event != nullptr);
    QCOMPARE(event->name(), QString("Wait_forResp"));
    QVERIFY(event->shared() == false);
    QCOMPARE(event->instance(), responder);

    delete model;
}

void tst_MscEventsParsing::testTestMessageInstanceName()
{
    static const QLatin1String msc("mscdocument automade; \
                    inst mux; \
                    inst mastercanfsm; \
                    msc recorded; \
                        mux: instance; \
                            in HeartbeatOut,51 from mastercanfsm; \
                            in HeartbeatOut,52 from mastercanfsm; \
                        endinstance; \
                        mastercanfsm: instance; \
                            out HeartbeatOut,51 to mux; \
                            out HeartbeatOut,52 to mux; \
                        endinstance; \
                endmsc; endmscdocument;");

    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->documents().size(), 1);

    QCOMPARE(model->documents().at(0)->charts().size(), 1);
    MscChart *chart = model->documents().at(0)->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 2);
}

void tst_MscEventsParsing::testSameNameDifferentInstances()
{
    static const QLatin1String msc("MSC msc1;\
      INSTANCE instA;\
        IN getbts ( duke ) FROM instB;\
      ENDINSTANCE;\
      INSTANCE instB;\
        OUT getbts ( ann ) TO instC;\
        OUT getbts ( duke ) TO instA; /* Check if it still loads if this is the second message */\
      ENDINSTANCE;\
      INSTANCE instC;\
        IN getbts ( ann ) FROM instB;\
      ENDINSTANCE;\
      ENDMSC;");
    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 3);
    QCOMPARE(chart->instanceEvents().size(), 2);
}

void tst_MscEventsParsing::testDifferentParameter()
{
    static const QLatin1String msc("msc recorded; \
                        instance mux; \
                            in HeartbeatOut(3) from responder; \
                            in HeartbeatOut(11) from responder; \
                        endinstance; \
                        instance responder; \
                            out HeartbeatOut(3) to mux; \
                            out HeartbeatOut(11) to mux; \
                        endinstance; \
                endmsc; ");

    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 2);
}

void tst_MscEventsParsing::testMultiMessageOccurrence()
{
    static const QLatin1String msc("msc recorded; \
                        instance sender; \
                            out ping to responder; \
                            out ping to responder; \
                            out ping to responder; \
                        endinstance; \
                        instance responder; \
                            in ping from sender; \
                            in ping from sender; \
                            in ping from sender; \
                        endinstance; \
                endmsc; ");

    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 3);
}

QTEST_APPLESS_MAIN(tst_MscEventsParsing)

#include "tst_msceventsparsing.moc"
