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
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "msctimer.h"
#include "tst_mscreader.h"

#include <QScopedPointer>

using namespace msc;

void tst_MscReader::testMessage()
{
    QString msc = "MSC msc1; \
        INSTANCE inst1; \
            in ICONreq from env; \
            in ICONreq2 from ; \
            out ICON to Responder; \
        ENDINSTANCE; \
    ENDMSC;";
    QScopedPointer<MscModel> model(m_reader->parseText(msc));
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

void tst_MscReader::testSameMessageInTwoInstances()
{
    QString msc = "MSC msc1; \
                  INSTANCE Initiator;in ICON from Responder;ENDINSTANCE; \
                  INSTANCE Responder;out ICON to Initiator;ENDINSTANCE; \
                  ENDMSC;";
    MscModel *model = m_reader->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 1);
    delete model;
}

void tst_MscReader::testMessageWithParameters()
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
    QScopedPointer<MscModel> model(m_reader->parseText(msc));

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

void tst_MscReader::testMessageParameterWildcard()
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

    QScopedPointer<MscModel> model(m_reader->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 3);
}

void tst_MscReader::testMessageParameterExpression()
{
    QString msc = "MSC msc1; \
                      INSTANCE inst1; \
                        in CanFrameIn,111(heartbeat:{header {functionCode 0, nodeID 0, rtr 0, dlc 0}}) from inst2; \
                        out activate_ieu,1({selected-controller controller:nominal, propulsion-board propulsion:nominal}) to inst2;\
                     ENDINSTANCE; \
                     INSTANCE inst2; \
                        out CanFrameIn,111(heartbeat:{header {functionCode 0, nodeID 0, rtr 0, dlc 0}}) to inst1; \
                        in activate_ieu,1({selected-controller controller:nominal, propulsion-board propulsion:nominal}) from inst1;\
                     ENDINSTANCE; \
                  ENDMSC;";

    QScopedPointer<MscModel> model(m_reader->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 2);

    auto message = qobject_cast<MscMessage *>(chart->instanceEvents().at(1));
    QVERIFY(message != nullptr);
    QCOMPARE(message->parameters().size(), 1);
    QCOMPARE(message->parameters().at(0).parameter(),
            QString("{selected-controller controller:nominal, propulsion-board propulsion:nominal}"));
}

void tst_MscReader::testMultiParameters()
{
    QString msc = "msc Untitled_1; \
       instance Inst_1; \
          out hello(b:   FALSE, 11) to Inst_2; \
       endinstance; \
       instance Inst_2; \
          in hello(b:   FALSE, 11) from Inst_1; \
       endinstance; \
    endmsc;";

    QScopedPointer<MscModel> model(m_reader->parseText(msc));
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

void tst_MscReader::testMessageParametersCurlyBraces()
{
    static const QStringList params { "{}", "{dpu_undervoltage:TRUE}",
        "heartbeat:{header {functionCode 0, nodeID 0, rtr 0, dlc 0}}", "{VBS_Valid: FALSE, CHU_Valid: FALSE}" };
    static const QString mscTemplate = "MSC msc1;"
                                       "INSTANCE A;"
                                       "%1"
                                       "ENDINSTANCE;"
                                       "INSTANCE B;"
                                       "%2"
                                       "ENDINSTANCE;"
                                       "ENDMSC;";

    auto prepareMsc = []() {
        QString msgA, msgB;
        for (int i = 0; i < params.size(); ++i) {
            const QString &param = params.at(i);
            const QString msgParams = QString("Msg%1(%2)").arg(i).arg(param);
            msgA += QString("in %1 from B;").arg(msgParams);
            msgB += QString("out %1 to A;").arg(msgParams);
        }

        return mscTemplate.arg(msgA, msgB);
    };

    QScopedPointer<MscModel> model(m_reader->parseText(prepareMsc()));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), params.size());

    for (int i = 0; i < chart->instanceEvents().size(); ++i)
        if (MscMessage *message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(i)))
            QCOMPARE(message->paramString(), params.at(i));
}

void tst_MscReader::testMessageChoiceParameter()
{
    QString msc = "MSC msc1; \
                   INSTANCE Inst_1;in Msg_1 (act:heater: nominal) from env;ENDINSTANCE; \
                   ENDMSC;";
    QScopedPointer<MscModel> model(m_reader->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 1);
    auto message = qobject_cast<MscMessage *>(chart->instanceEvents().at(0));
    QVERIFY(message != nullptr);
    QCOMPARE(message->parameters().size(), 1);
    QCOMPARE(message->parameters().at(0).parameter(), QString("act:heater:nominal"));
}

void tst_MscReader::testMessageComplexParameter()
{
    QString msc = "mscdocument automade;\
                     inst obsw;\
                     inst ieu_manager;\
                     msg acceptance : (Status);\
                     msc recorded; \
                       obsw: instance; \
                         in acceptance,2({tc-id '4143545f494555'H, report success}) from ieu_manager; \
                         out Completion({tc-id \"ACT-IEU\", report success}) to ieu_manager;\
                       endinstance; \
                       ieu_manager: instance; \
                         out acceptance,2({tc-id '4143545f494555'H, report success}) to obsw; \
                         in Completion({tc-id \"ACT-IEU\", report success}) from obsw;\
                       endinstance; \
                     endmsc;\
                   endmscdocument;";

    QScopedPointer<MscModel> model(m_reader->parseText(msc));

    QCOMPARE(model->documents().size(), 1);
    MscDocument *doc = model->documents().at(0);
    QCOMPARE(doc->charts().size(), 1);
    MscChart *chart = doc->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 2);
    auto message = qobject_cast<MscMessage *>(chart->instanceEvents().at(0));
    QVERIFY(message != nullptr);
    QCOMPARE(message->parameters().size(), 1);
    QCOMPARE(message->parameters().at(0).parameter(), QString("{tc-id '4143545f494555'H, report success}"));
    QCOMPARE(message->messageInstanceName(), QString("2"));
}

void tst_MscReader::testSortedMessage()
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

    MscModel *model = m_reader->parseText(msc);
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

void tst_MscReader::testSortedMessageTwoCharts()
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

    MscModel *model = m_reader->parseText(msc);
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

void tst_MscReader::testSortedInstanceEvents()
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

    MscModel *model = m_reader->parseText(msc);
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

void tst_MscReader::testSortedMessageCreate()
{
    QString msc = "msc Untitled;\
    instance Instance_B;\
      out BA_3 to Instance_A;\
      in AB_4 from Instance_A;\
      create NEW_INSTANCE;\
      out BC_6 to Instance_C;\
      out BN_7 to NEW_INSTANCE;\
      in NB_8 from NEW_INSTANCE;\
      create NEW_INSTANCE2;\
      out BN2_9 to NEW_INSTANCE2;\
      in NB2_10 from NEW_INSTANCE2;\
    endinstance;\
    instance NEW_INSTANCE2;\
      in BN2_9 from Instance_B;\
      out NB2_10 to Instance_B;\
    endinstance;\
    instance Instance_C;\
      out CA_1 to Instance_A;\
      in AC_2 from Instance_A;\
      in BC_6 from Instance_B;\
    stop;\
    instance NEW_INSTANCE;\
      out NA_5 to Instance_A;\
      in BN_7 from Instance_B;\
      out NB_8 to Instance_B;\
    stop;\
    instance Instance_A;\
      in CA_1 from Instance_C;\
      out AC_2 to Instance_C;\
      in BA_3 from Instance_B;\
      out AB_4 to Instance_B;\
      in NA_5 from NEW_INSTANCE;\
    endinstance;\
    endmsc;";
    QScopedPointer<MscModel> model(m_reader->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 5);
    QCOMPARE(chart->instanceEvents().size(), 12);
    QCOMPARE(chart->instanceEvents().at(0)->name(), QString("CA_1"));
    QCOMPARE(chart->instanceEvents().at(1)->name(), QString("AC_2"));
    QCOMPARE(chart->instanceEvents().at(2)->name(), QString("BA_3"));
    QCOMPARE(chart->instanceEvents().at(3)->name(), QString("AB_4"));
    QVERIFY(chart->instanceEvents().at(4)->entityType() == msc::MscEntity::EntityType::Create);
    QCOMPARE(chart->instanceEvents().at(5)->name(), QString("BC_6")); // "NA_5" would be valid as well
    QCOMPARE(chart->instanceEvents().at(6)->name(), QString("NA_5"));
    QCOMPARE(chart->instanceEvents().at(7)->name(), QString("BN_7"));
    QCOMPARE(chart->instanceEvents().at(8)->name(), QString("NB_8"));
    QVERIFY(chart->instanceEvents().at(9)->entityType() == msc::MscEntity::EntityType::Create);
    QCOMPARE(chart->instanceEvents().at(10)->name(), QString("BN2_9"));
    QCOMPARE(chart->instanceEvents().at(11)->name(), QString("NB2_10"));
}

void tst_MscReader::testSortingDeadlock()
{
    QString msc = "msc Untitled;\
    instance Instance_A;\
      out AB_1 from Instance_B;\
      in BA_2 to Instance_B;\
    endinstance;\
    instance Instance_B;\
      out BA_2 to Instance_A;\
      in AB_1 from Instance_A;\
    endinstance;\
    endmsc;";
    QVERIFY_EXCEPTION_THROWN(m_reader->parseText(msc), ParserException);

    msc = "msc Untitled;\
    instance Instance_A;\
      create Instance_B;\
    endinstance;\
    instance Instance_B;\
      create Instance_A;\
    endinstance;\
    endmsc;";
    QVERIFY_EXCEPTION_THROWN(m_reader->parseText(msc), ParserException);
}

void tst_MscReader::testIncompleteMessageIn()
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

    QVERIFY_EXCEPTION_THROWN(m_reader->parseText(msc), ParserException);
}

void tst_MscReader::testIncompleteMessageOut()
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

    QVERIFY_EXCEPTION_THROWN(m_reader->parseText(msc), ParserException);
}

void tst_MscReader::testConditionDublicate()
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

    MscModel *model = m_reader->parseText(msc);
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

void tst_MscReader::testTestMessageInstanceName()
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

    QScopedPointer<MscModel> model(m_reader->parseText(msc));
    QCOMPARE(model->documents().size(), 1);

    QCOMPARE(model->documents().at(0)->charts().size(), 1);
    MscChart *chart = model->documents().at(0)->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 2);
}

void tst_MscReader::testSameNameDifferentInstances()
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
    QScopedPointer<MscModel> model(m_reader->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 3);
    QCOMPARE(chart->instanceEvents().size(), 2);
}

void tst_MscReader::testDifferentParameter()
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

    QScopedPointer<MscModel> model(m_reader->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 2);
}

void tst_MscReader::testMultiMessageOccurrence()
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

    QScopedPointer<MscModel> model(m_reader->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 3);
}

void tst_MscReader::testNonStandardVia()
{
    // Using via that way is not really in line with the standard
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         OUT check1 ( pin ) VIA gtX; \
                         OUT check2(1) TO  VIA gtY; \
                      ENDINSTANCE; \
                   ENDMSC;";
    QScopedPointer<MscModel> model(m_reader->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 2);
}
