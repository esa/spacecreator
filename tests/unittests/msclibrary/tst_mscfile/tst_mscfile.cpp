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
#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "mscdocument.h"
#include "mscfile.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "msctimer.h"

#include <QtTest>

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
    void testGateMessage();
    void testCondition();
    void testCoregion();
    void testTimer();
    void testTimerRelation();
    void testAction();
    void testInstanceStop_data();
    void testInstanceStop();
    void testHierarchy();
    void testDataDefinitionLanguage();
    void testDataDefinitionData();
    void testKeywordAsName();
    void testNonStandardVia();
    void testNonStandardInstance();
    void testDocumentsType_data();
    void testDocumentsType();
    void testDefaultDocumentTypeFromLoad();
    void testMessageDeclaration();
    void testNameFiltering();

    // for message-related tests see the tst_MscEventsParsing

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

    // out of spec instance kind
    model.reset(file->parseText(" \
        mscdocument automade; \
            msc recorded; \
                instance mygui_GUI : process foo control unit; \
                endinstance; \
            endmsc; \
        endmscdocument; \
    "));
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->charts().size(), 1);
    chart = model->documents().at(0)->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    instance = chart->instances().at(0);
    QCOMPARE(instance->name(), QString("mygui_GUI"));
    QCOMPARE(instance->kind(), QString("foo control unit"));
    QCOMPARE(instance->denominator(), QString("process"));
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
    QCOMPARE(condition->shared(), true);

    condition = static_cast<MscCondition *>(chart->instanceEvents().at(1));
    QCOMPARE(condition->name(), QString("Con_2"));
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
                         starttimer T1, 1; \
                       ENDINSTANCE; \
                   ENDMSC;";

    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 4);

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

    event = chart->instanceEvents().at(3);
    QVERIFY(event->entityType() == MscEntity::EntityType::Timer);
    timer = static_cast<MscTimer *>(event);
    QCOMPARE(timer->name(), QString("T1"));
    QCOMPARE(timer->timerType(), MscTimer::TimerType::Start);
    QCOMPARE(timer->timerInstanceName(), QString("1"));
    QCOMPARE(timer->fullName(), QString("T1,1"));
}

void tst_MscFile::testTimerRelation()
{
    QString msc = "MSC msc1; \
                      INSTANCE Inst_1; \
                         starttimer T1; \
                         stoptimer T1; \
                         timeout T3; \
                       ENDINSTANCE; \
                       INSTANCE Inst_2; \
                          timeout T1; \
                      ENDINSTANCE; \
                   ENDMSC;";

    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 4);
    auto event = chart->instanceEvents().at(0);
    QVERIFY(event->entityType() == MscEntity::EntityType::Timer);
    auto timer1 = static_cast<MscTimer *>(event);
    event = chart->instanceEvents().at(1);
    QVERIFY(event->entityType() == MscEntity::EntityType::Timer);
    auto timer2 = static_cast<MscTimer *>(event);
    event = chart->instanceEvents().at(2);
    QVERIFY(event->entityType() == MscEntity::EntityType::Timer);
    auto timer3 = static_cast<MscTimer *>(event);

    QVERIFY(timer1->precedingTimer() == nullptr);
    QCOMPARE(timer1->followingTimer(), timer2);

    QCOMPARE(timer2->precedingTimer(), timer1);
    QVERIFY(timer2->followingTimer() == nullptr);

    QVERIFY(timer3->precedingTimer() == nullptr);
    QVERIFY(timer3->followingTimer() == nullptr);

    event = chart->instanceEvents().at(3);
    QVERIFY(event->entityType() == MscEntity::EntityType::Timer);
    auto timer4 = static_cast<MscTimer *>(event);
    QVERIFY(timer4->precedingTimer() == nullptr);
    QVERIFY(timer4->followingTimer() == nullptr);

    msc = "MSC msc1; \
            INSTANCE Inst_1; \
                SET nouseraction(30000000.0); \
                RESET nouseraction; \
                SET nox25reply(3000000.0); \
                RESET nox25reply; \
                SET nouseraction(30000000.0); \
                RESET nouseraction; \
                SET nox25reply(3000000.0); \
                SET nouseraction(30000000.0); \
                RESET nouseraction; \
                SET nouseraction(30000000.0); \
                RESET nouseraction; \
                SET nox25reply(3000000.0); \
                RESET nox25reply; \
                SET nouseraction(30000000.0); \
                RESET nouseraction; \
                SET nox25reply(3000000.0); \
                SET nouseraction(30000000.0); \
                RESET nouseraction; \
            ENDINSTANCE; \
           ENDMSC;";

    model.reset(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    chart = model->charts().at(0);
    QCOMPARE(chart->instanceEvents().size(), 18);

    QVector<msc::MscTimer *> timers;
    for (int i = 0; i < chart->instanceEvents().size(); ++i)
        timers.append(qobject_cast<MscTimer *>(chart->instanceEvents().at(i)));

    QVERIFY(timers[0]->precedingTimer() == nullptr);
    QVERIFY(timers[0]->followingTimer() == timers[1]);

    QVERIFY(timers[1]->precedingTimer() == timers[0]);
    QVERIFY(timers[1]->followingTimer() == nullptr);

    QVERIFY(timers[2]->precedingTimer() == nullptr);
    QVERIFY(timers[2]->followingTimer() == timers[3]);

    QVERIFY(timers[3]->precedingTimer() == timers[2]);
    QVERIFY(timers[3]->followingTimer() == nullptr);

    QVERIFY(timers[4]->precedingTimer() == nullptr);
    QVERIFY(timers[4]->followingTimer() == timers[5]);

    QVERIFY(timers[5]->precedingTimer() == timers[4]);
    QVERIFY(timers[5]->followingTimer() == nullptr);
}

void tst_MscFile::testAction()
{
    QString msc = { "MSC msc1; \
            INSTANCE Inst_1; \
               action 'Stop'; \
               ACTION def \"digit1\", def \"digit2\"; \
               ACTION DSPinit(1,4)=10; \
               ACTION filter(mkpixrow_t((. 0,0,0 .), (. 128,128,128 .)))=mkpixrow_t((. 4,0,0 .),(. 132,128,128 .)); \
               ACTION convert(400)= (. 0,0,0 .); \
            ENDINSTANCE; \
         ENDMSC;" };

    QScopedPointer<MscModel> model(file->parseText(msc));
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instanceEvents().size(), 5);

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

    action = static_cast<MscAction *>(chart->instanceEvents().at(3));
    QVERIFY(action != nullptr);
    QCOMPARE(action->actionType(), MscAction::ActionType::Informal);
    QCOMPARE(action->informalAction(),
             QString("filter(mkpixrow_t((. 0,0,0 .), (. 128,128,128 .)))=mkpixrow_t((. 4,0,0 .),(. 132,128,128 .))"));
    QCOMPARE(action->instance(), chart->instances().at(0));
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
                    MSCDOCUMENT test_leaf /* MSC LEAF */; \
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

void tst_MscFile::testDocumentsType_data()
{
    QTest::addColumn<QString>("mscContent");

    QString msc = "MSCDOCUMENT root_doc /* MSC LEAF */; \
                  MSCDOCUMENT doc1; \
                  ENDMSCDOCUMENT; \
              ENDMSCDOCUMENT;";
    QTest::newRow("Leaf type docs have no child doc") << msc;
    msc = "MSCDOCUMENT root_doc /* MSC REPEAT */; \
                  MSCDOCUMENT doc1; \
                  ENDMSCDOCUMENT; \
                  MSCDOCUMENT doc1; \
                  ENDMSCDOCUMENT; \
              ENDMSCDOCUMENT;";
    QTest::newRow("Repeat type docs have only one child doc") << msc;
    msc = "MSCDOCUMENT root_doc /* MSC PARALLEL */; \
                    MSCDOCUMENT doc1; \
                    ENDMSCDOCUMENT; \
                    MSCDOCUMENT doc1; \
                    ENDMSCDOCUMENT; \
                ENDMSCDOCUMENT;";
    QTest::newRow("Parallel type docs have only one child doc") << msc;
    msc = "MSCDOCUMENT root_doc /* MSC EXCEPTION */; \
                    MSCDOCUMENT doc1; \
                    ENDMSCDOCUMENT; \
                    MSCDOCUMENT doc1; \
                    ENDMSCDOCUMENT; \
                ENDMSCDOCUMENT;";
    QTest::newRow("Exception type docs have only one child doc") << msc;
    msc = "MSCDOCUMENT root_doc /* MSC IS */; \
                    MSCDOCUMENT doc1; \
                    ENDMSCDOCUMENT; \
                    MSCDOCUMENT doc1; \
                    ENDMSCDOCUMENT; \
                ENDMSCDOCUMENT;";
    QTest::newRow("Is type docs have only one child doc") << msc;
}

void tst_MscFile::testDocumentsType()
{
    QFETCH(QString, mscContent);

    QVERIFY_EXCEPTION_THROWN(file->parseText(mscContent), ParserException);
}

void tst_MscFile::testDefaultDocumentTypeFromLoad()
{
    QString msc = "MSCDOCUMENT root_doc /* MSC OR */; \
                        MSCDOCUMENT doc1; \
                          MSCDOCUMENT doc1_1; \
                          ENDMSCDOCUMENT; \
                        ENDMSCDOCUMENT; \
                        MSCDOCUMENT doc2 /* MSC IS */; \
                          MSCDOCUMENT doc2_2 /* MSC EXCEPTION */; \
                          ENDMSCDOCUMENT; \
                        ENDMSCDOCUMENT; \
                  ENDMSCDOCUMENT;";

    QScopedPointer<MscModel> model(file->parseText(msc));
    MscDocument *doc = model->documents().at(0);
    QCOMPARE(doc->hierarchyType(), MscDocument::HierarchyOr);

    MscDocument *childDoc = doc->documents().at(0);
    QCOMPARE(childDoc->hierarchyType(), MscDocument::HierarchyAnd);
    childDoc = childDoc->documents().at(0);
    QCOMPARE(childDoc->hierarchyType(), MscDocument::HierarchyLeaf);

    childDoc = doc->documents().at(1);
    QCOMPARE(childDoc->hierarchyType(), MscDocument::HierarchyIs);
    childDoc = childDoc->documents().at(0);
    QCOMPARE(childDoc->hierarchyType(), MscDocument::HierarchyException);
}

void tst_MscFile::testMessageDeclaration()
{
    QString msc = "mscdocument automade; \
                inst mygui_GUI : process; \
                msg gui_send_tm, pepe : (str, T-POS); \
                msc recorded; \
                    gate out gui_send_tm,a(hello, b) to mygui_GUI; \
                    mygui_GUI : instance  process; \
                        in gui_send_tm,a(hello, b)  from env; \
                    endinstance; \
                endmsc; \
            endmscdocument;";
    QScopedPointer<MscModel> model(file->parseText(msc));

    QCOMPARE(model->documents().size(), 1);
    MscDocument *doc = model->documents().at(0);
    MscMessageDeclarationList *mdl = doc->messageDeclarations();
    QCOMPARE(mdl->size(), 1);

    MscMessageDeclaration *md = mdl->at(0);
    QStringList nameList = md->names();
    QCOMPARE(nameList.size(), 2);
    QCOMPARE(nameList.at(0), QString("gui_send_tm"));
    QCOMPARE(nameList.at(1), QString("pepe"));
    QStringList typeRefs = md->typeRefList();
    QCOMPARE(typeRefs.size(), 2);
    QCOMPARE(typeRefs.at(0), QString("str"));
    QCOMPARE(typeRefs.at(1), QString("T-POS"));
}

void tst_MscFile::testNameFiltering()
{
    QString msc = "MSC msc1; \
                        INSTANCE `oid`1`351`Inst_1`; \
                            condition `oid`1`351`Con_2`; \
                        ENDINSTANCE; \
                        INSTANCE `FLX`Inst_2`; \
                        ENDINSTANCE; \
                   ENDMSC;";

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);

    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->instanceEvents().size(), 1);

    auto *instance1 = static_cast<MscInstance *>(chart->instances().at(0));
    QCOMPARE(instance1->name(), QString("Inst_1"));

    auto *instance2 = static_cast<MscInstance *>(chart->instances().at(1));
    QCOMPARE(instance2->name(), QString("`FLX`Inst_2`"));

    auto *condition = static_cast<MscCondition *>(chart->instanceEvents().at(0));
    QCOMPARE(condition->name(), QString("Con_2"));
}

QTEST_APPLESS_MAIN(tst_MscFile)

#include "tst_mscfile.moc"
