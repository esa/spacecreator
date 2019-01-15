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

#include <exceptions.h>
#include <mscchart.h>
#include <msccondition.h>
#include <msccreate.h>
#include <mscdocument.h>
#include <mscfile.h>
#include <mscinstance.h>
#include <mscmessage.h>
#include <mscmodel.h>
#include <msctimer.h>
#include <msccoregion.h>

#include <QtTest>
#include <mscaction.h>

using namespace msc;

class tst_MscFile : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testFileOpenError();
    void testSyntaxError();
    void testExampleFilesParsing_data();
    void testExampleFilesParsing();
    void testEmptyDocument();
    void testComments();
    void testEntityComments();
    void testNestedDocuments();
    void testMscInDocument();
    void testInstance();
    void testInstanceWithKind();
    void testMessage();
    void testSameMessageInTwoInstances();
    void testMessageWithParameters();
    void testGateMessage();
    void testCondition();
    void testCoregion();
    void testTimer();
    void testAction();
    void testInstanceStop_data();
    void testInstanceStop();
    void testSortedMessage();
    void testSortedMessageTwoCharts();
    void testSortedInstanceEvents();
    void testHierarchy();
    void testDataDefinitionLanguage();
    void testDataDefinitionData();
    void testInstanceCreate();
    void testInstanceCreateNoParameter();
    void testInstanceCreateMultiParameter();
    void testInstanceCreateNoInstance();
    void testInstanceCreateDublicate();
    void testKeywordAsName();
    void testNonStandardVia();
    void testNonStandardInstance();

private:
    MscFile *file = nullptr;
};

void tst_MscFile::init()
{
    file = new MscFile;
}

void tst_MscFile::cleanup()
{
    delete file;
    file = nullptr;
}

void tst_MscFile::testFileOpenError()
{
    QVERIFY_EXCEPTION_THROWN(file->parseFile("some_dummy_file_name"), FileNotFoundException);
}

void tst_MscFile::testSyntaxError()
{
    QString fileName = QFINDTESTDATA("syntax_error.msc");
    QVERIFY_EXCEPTION_THROWN(file->parseFile(fileName), ParserException);
}

void tst_MscFile::testExampleFilesParsing_data()
{
    QTest::addColumn<QString>("filename");

    QString fileName = QFINDTESTDATA(EXAMPLES_DIR "test1.msc");
    QFileInfo fileInfo(fileName);
    QDir exampleDir = fileInfo.dir();
    QStringList fileList = exampleDir.entryList(QDir::Files);
    for (const QString &file : fileList) {
        if (file.endsWith(".msc", Qt::CaseInsensitive) && file != "example02.msc") {
            QTest::newRow(file.toStdString().c_str()) << exampleDir.absolutePath() + "/" + file;
        }
    }
}

// test if the files from the example dir can be opened without an exception
void tst_MscFile::testExampleFilesParsing()
{
    QFETCH(QString, filename);
    try {
        QScopedPointer<MscModel> model(file->parseFile(filename));
    } catch (...) {
        filename = "Failed to open file " + filename;
        QFAIL(filename.toStdString().c_str());
    }
}

void tst_MscFile::testEmptyDocument()
{
    QScopedPointer<MscModel> model(file->parseText("MSCDOCUMENT CU_level;\nENDMSCDOCUMENT;"));
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->name(), QString("CU_level"));

    model.reset(file->parseText("mscdocument inst_1_cu_nominal.cu_controller;\nendmscdocument;"));
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->name(), QString("inst_1_cu_nominal.cu_controller"));

    model.reset(file->parseText("  \n mscdocument \n  ABC  ; \n  \n  endmscdocument  ;\n  "));
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->name(), QString("ABC"));

    model.reset(file->parseText("MSCDOCUMENT error1; MSCDOCUMENT seq; ENDMSCDOCUMENT; ENDMSCDOCUMENT;"));
    QCOMPARE(model->documents().size(), 1);
    //    QCOMPARE(model->documents().at(0)->documents().size(), 1);
    // no exception thrown
}

void tst_MscFile::testComments()
{
    MscModel *model;
    model = file->parseText("/* This is a comment*/MSCDOCUMENT CU_level;\nENDMSCDOCUMENT;");
    delete model;
    model = file->parseText("MSCDOCUMENT CU_level /*This is a comment */ ;\nENDMSCDOCUMENT;");
    delete model;
    model = file->parseText("MSCDOCUMENT CU_level;\n/* This is a comment*/\nENDMSCDOCUMENT;");
    // no exception thrown
    delete model;
}

void tst_MscFile::testEntityComments()
{
    QScopedPointer<MscModel> model(file->parseText(" \
        mscdocument automade COMMENT 'doc comment'; \
            inst mygui_GUI : process; \
            msc recorded comment 'chart comment'; \
                mygui_GUI : instance process foo COMMENT 'instance comment'; \
                    in request from env comment 'msg comment'; \
                endinstance; \
            endmsc; \
        endmscdocument; \
    "));
    QCOMPARE(model->documents().size(), 1);
    MscDocument *doc = model->documents().at(0);
    QCOMPARE(doc->comment(), QString("doc comment"));

    QCOMPARE(doc->charts().size(), 1);
    MscChart *chart = model->documents().at(0)->charts().at(0);
    QCOMPARE(chart->comment(), QString("chart comment"));

    QCOMPARE(chart->instances().size(), 1);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(instance->comment(), QString("instance comment"));

    QCOMPARE(chart->instanceEvents().size(), 1);
    MscInstanceEvent *event = chart->instanceEvents().at(0);
    QCOMPARE(event->comment(), QString("msg comment"));
}

void tst_MscFile::testNestedDocuments()
{
    MscModel *model = file->parseText("MSCDOCUMENT l1; MSCDOCUMENT l2;ENDMSCDOCUMENT; \nENDMSCDOCUMENT;");
    auto mainDocuments = model->documents();
    QCOMPARE(mainDocuments.size(), 1);
    QCOMPARE(mainDocuments.at(0)->documents().size(), 1);
    delete model;

    // documents in parallel
    model = file->parseText("MSCDOCUMENT l1; MSCDOCUMENT l2;ENDMSCDOCUMENT; MSCDOCUMENT "
                            "l3;ENDMSCDOCUMENT; \nENDMSCDOCUMENT;");
    mainDocuments = model->documents();
    QCOMPARE(mainDocuments.size(), 1);
    QCOMPARE(mainDocuments.at(0)->documents().size(), 2);
    delete model;
}

void tst_MscFile::testMscInDocument()
{
    MscModel *model = file->parseText("MSC msc1;ENDMSC;");
    QCOMPARE(model->charts().size(), 1);
    QCOMPARE(model->charts().at(0)->name(), QString("msc1"));
    delete model;

    model = file->parseText("MSCDOCUMENT doc1; MSC msc1;ENDMSC; ENDMSCDOCUMENT;");
    QCOMPARE(model->documents().size(), 1);
    auto mainDocument = model->documents().at(0);
    QCOMPARE(mainDocument->charts().size(), 1);
    QCOMPARE(mainDocument->charts().at(0)->name(), QString("msc1"));
    delete model;
}

void tst_MscFile::testInstance()
{
    MscModel *model = file->parseText("MSC msc1; INSTANCE inst1; ENDINSTANCE; ENDMSC;");
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instances().at(0)->name(), QString("inst1"));
    delete model;
}

void tst_MscFile::testInstanceWithKind()
{
    QScopedPointer<MscModel> model(file->parseText(" \
        mscdocument automade; \
            inst mygui_GUI : process; \
            msc recorded; \
                mygui_GUI : instance process foo; \
                endinstance; \
            endmsc; \
        endmscdocument; \
    "));
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->charts().size(), 1);
    MscChart *chart = model->documents().at(0)->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(instance->name(), QString("mygui_GUI"));
    QCOMPARE(instance->kind(), QString("foo"));
    QCOMPARE(instance->denominator(), QString("process"));
}

void tst_MscFile::testMessage()
{
    QString msc = "MSC msc1; \
        INSTANCE inst1; \
            in ICONreq from env; \
            in ICONreq2 from ; \
            out ICON to Responder; \
            OUT req(data:'Hello' ) TO A2B; \
            IN userpidgot(pid(connhandler(1))) FROM inst_1_switchctrl; \
            OUT msg('Sorry, wrong id, try again') TO ENV; \
        ENDINSTANCE; \
    ENDMSC;";
    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    MscInstance *instance = chart->instances().at(0);

    QCOMPARE(chart->instanceEvents().size(), 6);
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

void tst_MscFile::testSameMessageInTwoInstances()
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

void tst_MscFile::testMessageWithParameters()
{
    QString msc = "MSC msc1; \
                   INSTANCE Initiator; \
                      in gui_send_tm,a(longitude:-174.0) from env; \
                      out gui_int_send_tm(12) to env; \
                   ENDINSTANCE; \
                   ENDMSC;";

    MscModel *model = file->parseText(msc);

    QVERIFY(model->charts().size() == 1);
    MscChart *chart = model->charts().at(0);

    QVERIFY(chart->instanceEvents().size() == 2);

    auto *message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(0));
    QVERIFY(message != nullptr);
    QCOMPARE(message->parameters().name, QString("a"));
    QCOMPARE(message->parameters().expression, QString("longitude:-174.0"));

#ifndef __clang_analyzer__
    message = dynamic_cast<MscMessage *>(chart->instanceEvents().at(1));
    QVERIFY(message != nullptr);
    QVERIFY(message->parameters().name.isEmpty());
    QCOMPARE(message->parameters().pattern, QString("12"));

    delete model;
#endif
}

void tst_MscFile::testGateMessage()
{
    QString msc = "MSC msc1; \
                   gate out Msg_4 to Inst_1; \
                   INSTANCE Inst_1;in Msg_4 from env;out Msg_3 to Inst_2;ENDINSTANCE; \
                   INSTANCE Inst_2;in Msg_3 from Inst_1;ENDINSTANCE; \
                   ENDMSC;";
    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 2);
    delete model;
}

void tst_MscFile::testCondition()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         condition Con_1 shared all; \
                         condition Con_2; \
                       ENDINSTANCE; \
                   ENDMSC;";

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 2);

    auto *condition = static_cast<MscCondition *>(chart->instanceEvents().at(0));
    QCOMPARE(condition->name(), QString("Con_1"));
    QVERIFY(condition->messageName().isEmpty());
    QCOMPARE(condition->shared(), true);

    condition = static_cast<MscCondition *>(chart->instanceEvents().at(1));
    QCOMPARE(condition->name(), QString("Con_2"));
    QVERIFY(condition->messageName().isEmpty());
    QCOMPARE(condition->shared(), false);

    delete model;
}

void tst_MscFile::testCoregion()
{
    QString msc = "msc connection; \
                      instance Initiator; \
                          concurrent; \
                          starttimer T1; \
                          in ICONreq from env; \
                          endconcurrent; \
                      endinstance; \
                  endmsc;";

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 4);

    auto event = chart->instanceEvents().at(0);
    QCOMPARE(event->entityType(), MscEntity::EntityType::Coregion);
    auto coregion = static_cast<MscCoregion *>(event);
    QCOMPARE(coregion->type(), MscCoregion::Type::Begin);

    event = chart->instanceEvents().at(1);
    QCOMPARE(event->entityType(), MscEntity::EntityType::Timer);
    event = chart->instanceEvents().at(2);
    QCOMPARE(event->entityType(), MscEntity::EntityType::Message);

    event = chart->instanceEvents().at(3);
    QCOMPARE(event->entityType(), MscEntity::EntityType::Coregion);
    coregion = static_cast<MscCoregion *>(event);
    QCOMPARE(coregion->type(), MscCoregion::Type::End);

    delete model;
}

void tst_MscFile::testTimer()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         starttimer T1; \
                         stoptimer T2; \
                         timeout T3; \
                       ENDINSTANCE; \
                   ENDMSC;";

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 3);

    auto event = chart->instanceEvents().at(0);
    QVERIFY(event->entityType() == MscEntity::EntityType::Timer);
    auto timer = static_cast<MscTimer *>(event);
    QCOMPARE(timer->name(), QString("T1"));
    QCOMPARE(timer->timerType(), MscTimer::TimerType::Start);

    event = chart->instanceEvents().at(1);
    QVERIFY(event->entityType() == MscEntity::EntityType::Timer);
    timer = static_cast<MscTimer *>(event);
    QCOMPARE(timer->name(), QString("T2"));
    QCOMPARE(timer->timerType(), MscTimer::TimerType::Stop);

    event = chart->instanceEvents().at(2);
    QVERIFY(event->entityType() == MscEntity::EntityType::Timer);
    timer = static_cast<MscTimer *>(event);
    QCOMPARE(timer->name(), QString("T3"));
    QCOMPARE(timer->timerType(), MscTimer::TimerType::Timeout);

    delete model;
}

void tst_MscFile::testAction()
{
    QString msc = { "MSC msc1; \
                      INSTANCE Inst_1; \
                         action 'Stop'; \
                         ACTION def \"digit1\", def \"digit2\"; \
                         ACTION DSPinit(1,4)=10; \
                       ENDINSTANCE; \
                   ENDMSC;" };

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 3);

    auto action = static_cast<MscAction *>(chart->instanceEvents().at(0));
    QVERIFY(action != nullptr);
    QCOMPARE(action->actionType(), MscAction::ActionType::Informal);
    QCOMPARE(action->informalAction(), QString("Stop"));
    QCOMPARE(action->instance(), chart->instances().at(0));

    action = static_cast<MscAction *>(chart->instanceEvents().at(1));
    QVERIFY(action != nullptr);
    QCOMPARE(action->actionType(), MscAction::ActionType::Formal);
    QCOMPARE(action->dataStatements().size(), 2);
    QCOMPARE(action->instance(), chart->instances().at(0));

    delete model;
}

void tst_MscFile::testInstanceStop_data()
{
    QTest::addColumn<QString>("mscContent");

    QString msc = "msc process_stop; inst subscriber; \
            instance subscriber; \
                in terminate from env; \
                stop; \
        endmsc;";
    QTest::newRow("Spec conform (no endinstance;)") << msc;

    msc = "msc process_stop; inst subscriber; \
            instance subscriber; \
                in terminate from env; \
                stop; \
            endinstance; \
        endmsc;";
    QTest::newRow("Example conform (with endinstance;)") << msc;
}

void tst_MscFile::testInstanceStop()
{
    QFETCH(QString, mscContent);

    QScopedPointer<MscModel> model(file->parseText(mscContent));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 1);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(instance->explicitStop(), true);
}

void tst_MscFile::testSortedMessage()
{
    QString msc = "msc connection; \
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
                  endmsc;";

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

void tst_MscFile::testSortedMessageTwoCharts()
{
    QString msc = "mscdocument doc1; \
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
                  endmscdocument;";

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

void tst_MscFile::testSortedInstanceEvents()
{
    QString msc = "msc connection; \
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
                  endmsc;";

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

void tst_MscFile::testHierarchy()
{
    QString msc = "\
        mscdocument test_is /* MSC IS */; \
            /* MSC AND */ \
            mscdocument test_and; \
                /* MSC OR */ \
                MSCDOCUMENT test_or; \
                    MSC connection; \
                    ENDMSC; \
                endmscdocument; \
                /* MSC PARALLEL */ \
                MSCDOCUMENT test_parallel; \
                    MSC connection; \
                    ENDMSC; \
                endmscdocument; \
                /* MSC REPEAT */ \
                MSCDOCUMENT test_repeat; \
                    MSC connection; \
                    ENDMSC; \
                endmscdocument; \
                MSCDOCUMENT test_exception /* MSC EXCEPTION */; \
                    MSCDOCUMENT test_leaf; \
                        MSC connection; \
                        ENDMSC; \
                    endmscdocument; \
                endmscdocument; \
            endmscdocument; \
        endmscdocument;";

    auto model = file->parseText(msc);
    QCOMPARE(model->documents().size(), 1);

    auto documentIs = model->documents().at(0);
    QCOMPARE(documentIs->hierarchyType(), MscDocument::HierarchyIs);
    QCOMPARE(documentIs->documents().size(), 1);

    auto documentAnd = documentIs->documents().at(0);
    QCOMPARE(documentAnd->hierarchyType(), MscDocument::HierarchyAnd);
    QCOMPARE(documentAnd->documents().size(), 4);

    auto documentOr = documentAnd->documents().at(0);
    QCOMPARE(documentOr->hierarchyType(), MscDocument::HierarchyOr);
    QCOMPARE(documentOr->documents().size(), 0);

    auto documentParallel = documentAnd->documents().at(1);
    QCOMPARE(documentParallel->hierarchyType(), MscDocument::HierarchyParallel);
    QCOMPARE(documentParallel->documents().size(), 0);

    auto documentRepeat = documentAnd->documents().at(2);
    QCOMPARE(documentRepeat->hierarchyType(), MscDocument::HierarchyRepeat);
    QCOMPARE(documentRepeat->documents().size(), 0);

    auto documentException = documentAnd->documents().at(3);
    QCOMPARE(documentException->hierarchyType(), MscDocument::HierarchyException);
    QCOMPARE(documentException->documents().size(), 1);

    auto documentLeaf = documentException->documents().at(0);
    QCOMPARE(documentLeaf->hierarchyType(), MscDocument::HierarchyLeaf);

    delete model;
}

void tst_MscFile::testDataDefinitionLanguage()
{
    QString msc = "mscdocument automade; language ASN.1; endmscdocument;";
    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->dataLanguage(), QString("ASN.1"));
}

void tst_MscFile::testDataDefinitionData()
{
    QString msc = "mscdocument automade; data TPos.asn; endmscdocument;";
    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->dataDefinitionString(), QString("TPos.asn"));
}

void tst_MscFile::testInstanceCreate()
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

    auto *create = static_cast<MscCreate *>(chart->instanceEvents().at(0));
    QCOMPARE(create->name(), QString("subscriber"));

    QCOMPARE(create->parameters().size(), 1);
    QCOMPARE(create->parameters()[0], QString("data"));
}

void tst_MscFile::testInstanceCreateNoParameter()
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
    QCOMPARE(create->name(), QString("subscriber"));
    QVERIFY(create->parameters().isEmpty());
}

void tst_MscFile::testInstanceCreateMultiParameter()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber(data1, data2, data3); \
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
    QCOMPARE(create->name(), QString("subscriber"));

    QCOMPARE(create->parameters().size(), 3);
    QCOMPARE(create->parameters()[0], QString("data1"));
    QCOMPARE(create->parameters()[1], QString("data2"));
    QCOMPARE(create->parameters()[2], QString("data3"));
}

void tst_MscFile::testInstanceCreateNoInstance()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber2(data1); \
                      ENDINSTANCE; \
                      INSTANCE subscriber; \
                      ENDINSTANCE; \
                   ENDMSC;";

    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QVERIFY(chart->instanceEvents().isEmpty());
}

void tst_MscFile::testInstanceCreateDublicate()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         create subscriber(data1); \
                         create subscriber(data2); \
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
    QCOMPARE(create->name(), QString("subscriber"));

    QCOMPARE(create->parameters().size(), 1);
    QCOMPARE(create->parameters()[0], QString("data1"));
}

void tst_MscFile::testKeywordAsName()
{
    QString msc = "MSCDOCUMENT timer;\
                MSC action;\
                ENDMSC; \
            ENDMSCDOCUMENT;";
    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->documents().size(), 1);
    MscDocument *doc = model->documents().at(0);
    QCOMPARE(doc->name(), QString("timer"));

    QCOMPARE(doc->charts().size(), 1);
    MscChart *chart = doc->charts().at(0);
    QCOMPARE(chart->name(), QString("action"));
}

void tst_MscFile::testNonStandardVia()
{
    // Using via that way is not really in line with the standard
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         OUT check1 ( pin ) VIA gtX; \
                         OUT check2(1) TO  VIA gtY; \
                      ENDINSTANCE; \
                   ENDMSC;";
    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 2);
}

void tst_MscFile::testNonStandardInstance()
{
    // Using INSTANCE that way is not really in line with the standard
    QString msc = "MSC msc1; \
                      INSTANCE : FAB 1; \
                      ENDINSTANCE; \
                   ENDMSC;";
    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 1);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(instance->name(), QString("FAB 1"));
}

QTEST_APPLESS_MAIN(tst_MscFile)

#include "tst_mscfile.moc"
