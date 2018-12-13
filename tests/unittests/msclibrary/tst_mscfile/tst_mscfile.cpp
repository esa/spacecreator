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
#include <mscdocument.h>
#include <mscfile.h>
#include <mscinstance.h>
#include <mscmessage.h>
#include <mscmodel.h>

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
    void testEmptyDocument();
    void testComments();
    void testNestedDocuments();
    void testMscInDocument();
    void testInstance();
    void testInstanceWithKind();
    void testInstanceWithInheritance();
    void testMessage();
    void testSameMessageInTwoInstances();
    void testMessageWithParameters();
    void testGateMessage();
    void testSortedMessage();
    void testSortedMessageTwoCharts();

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

void tst_MscFile::testEmptyDocument()
{
    MscModel *model = file->parseText("MSCDOCUMENT CU_level;\nENDMSCDOCUMENT;");
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->name(), QString("CU_level"));
    delete model;

    model = file->parseText("mscdocument inst_1_cu_nominal.cu_controller;\nendmscdocument;");
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->name(), QString("inst_1_cu_nominal.cu_controller"));
    delete model;

    model = file->parseText("  \n mscdocument \n  ABC  ; \n  \n  endmscdocument  ;\n  ");
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->name(), QString("ABC"));
    // no exception thrown
    delete model;
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

void tst_MscFile::testNestedDocuments()
{
    MscModel *model = file->parseText("MSCDOCUMENT l1; MSCDOCUMENT l2;ENDMSCDOCUMENT; \nENDMSCDOCUMENT;");
    auto mainDocuments = model->documents();
    QCOMPARE(mainDocuments.size(), 1);
    QCOMPARE(mainDocuments.at(0)->documents().size(), 1);
    delete model;

    // documents in parallel
    model = file->parseText("MSCDOCUMENT l1; MSCDOCUMENT l2;ENDMSCDOCUMENT; MSCDOCUMENT l3;ENDMSCDOCUMENT; \nENDMSCDOCUMENT;");
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

    model = file->parseText("MSCDOCUMENT doc1; MSC msc1;ENDMSC; \nENDMSCDOCUMENT;");
    QCOMPARE(model->documents().size(), 1);
    auto mainDocument = model->documents().at(0);
    QCOMPARE(mainDocument->charts().size(), 1);
    QCOMPARE(mainDocument->charts().at(0)->name(), QString("msc1"));
    delete model;
}

void tst_MscFile::testInstance()
{
    MscModel *model = file->parseText("MSC msc1;\nINSTANCE inst1;ENDINSTANCE;ENDMSC;");
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instances().at(0)->name(), QString("inst1"));
    delete model;
}

void tst_MscFile::testInstanceWithKind()
{
    MscModel *model = file->parseText("MSC msc1;INSTANCE foo : Process;ENDINSTANCE;ENDMSC;");
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(instance->name(), QString("foo"));
    QCOMPARE(instance->kind(), QString("Process"));
    delete model;
}

void tst_MscFile::testInstanceWithInheritance()
{
    MscModel *model = file->parseText("MSC msc1;INSTANCE foo : PROCESS satellite;ENDINSTANCE;ENDMSC;");
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    MscInstance *instance = chart->instances().at(0);
    QCOMPARE(instance->name(), QString("foo"));
    QCOMPARE(instance->kind(), QString("PROCESS"));
    QCOMPARE(instance->inheritance(), QString("satellite"));
    delete model;
}

void tst_MscFile::testMessage()
{
    MscModel *model = file->parseText("MSC msc1;\nINSTANCE inst1;in ICONreq from env;out ICON to Responder;ENDINSTANCE;ENDMSC;");
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    MscInstance *instance = chart->instances().at(0);

    QCOMPARE(chart->messages().size(), 2);
    MscMessage *message1 = chart->messages().at(0);
    QCOMPARE(message1->name(), QString("ICONreq"));
    QCOMPARE(message1->sourceInstance(), static_cast<MscInstance *>(nullptr));
    QCOMPARE(message1->targetInstance(), instance);

    MscMessage *message2 = chart->messages().at(1);
    QCOMPARE(message2->name(), QString("ICON"));
    QCOMPARE(message2->sourceInstance(), instance);
    QCOMPARE(message2->targetInstance(), static_cast<MscInstance *>(nullptr));
    delete model;
}

void tst_MscFile::testSameMessageInTwoInstances()
{
    QString msc = { "MSC msc1; \
                    INSTANCE Initiator;in ICON from Responder;ENDINSTANCE; \
                    INSTANCE Responder;out ICON to Initiator;ENDINSTANCE; \
                    ENDMSC;" };
    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->messages().size(), 1);
    delete model;
}

void tst_MscFile::testMessageWithParameters()
{
    QString msc = { "MSC msc1; \
                     INSTANCE Initiator; \
                        in gui_send_tm,a(longitude:-174.0) from env; \
                        out gui_int_send_tm(12) to env; \
                     ENDINSTANCE; \
                     ENDMSC;" };

    MscModel *model = file->parseText(msc);

    QVERIFY(model->charts().size() == 1);
    MscChart *chart = model->charts().at(0);

    QVERIFY(chart->messages().size() == 2);

    auto *message = chart->messages().at(0);
    QCOMPARE(message->parameters().name, QString("a"));
    QCOMPARE(message->parameters().expression, QString("longitude:-174.0"));

    message = chart->messages().at(1);
    QVERIFY(message->parameters().name.isEmpty());
    QCOMPARE(message->parameters().pattern, QString("12"));

    delete model;
}

void tst_MscFile::testGateMessage()
{
    QString msc = { "MSC msc1; \
                     gate out Msg_4 to Inst_1; \
                     INSTANCE Inst_1;in Msg_4 from env;out Msg_3 to Inst_2;ENDINSTANCE; \
                     INSTANCE Inst_2;in Msg_3 from Inst_1;ENDINSTANCE; \
                     ENDMSC;" };
    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 2);
    QCOMPARE(chart->messages().size(), 2);
    delete model;
}

void tst_MscFile::testSortedMessage()
{
    QString msc = { "msc connection; \
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
                    endmsc;" };

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->charts().size(), 1);

    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 2);
    MscInstance *initiator = chart->instances().at(0);
    MscInstance *responder = chart->instances().at(1);

    QCOMPARE(chart->messages().size(), 6);

    MscMessage *message = chart->messages().at(0);
    QCOMPARE(message->name(), QString("ICONreq"));
    QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
    QCOMPARE(message->targetInstance(), initiator);

    message = chart->messages().at(1);
    QCOMPARE(message->name(), QString("ICON"));
    QCOMPARE(message->sourceInstance(), initiator);
    QCOMPARE(message->targetInstance(), responder);

    message = chart->messages().at(2);
    QCOMPARE(message->name(), QString("ICONind"));
    QCOMPARE(message->sourceInstance(), responder);
    QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));

    message = chart->messages().at(3);
    QCOMPARE(message->name(), QString("ICONresp"));
    QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
    QCOMPARE(message->targetInstance(), responder);

    message = chart->messages().at(4);
    QCOMPARE(message->name(), QString("ICONF"));
    QCOMPARE(message->sourceInstance(), responder);
    QCOMPARE(message->targetInstance(), initiator);

    message = chart->messages().at(5);
    QCOMPARE(message->name(), QString("ICONconf"));
    QCOMPARE(message->sourceInstance(), initiator);
    QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));

    delete model;
}

void tst_MscFile::testSortedMessageTwoCharts()
{
    QString msc = { "mscdocument doc1; \
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
                    endmscdocument;" };

    MscModel *model = file->parseText(msc);
    QCOMPARE(model->documents().size(), 1);

    MscDocument *document = model->documents().at(0);
    QCOMPARE(document->charts().size(), 2);

    for (int x = 0; x < document->charts().size(); ++x) {
        MscChart *chart = document->charts().at(x);
        QCOMPARE(chart->instances().size(), 2);

        MscInstance *initiator = chart->instances().at(0);
        MscInstance *responder = chart->instances().at(1);

        QCOMPARE(chart->messages().size(), 6);

        MscMessage *message = chart->messages().at(0);
        QCOMPARE(message->name(), QString("ICONreq%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
        QCOMPARE(message->targetInstance(), initiator);

        message = chart->messages().at(1);
        QCOMPARE(message->name(), QString("ICON%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), initiator);
        QCOMPARE(message->targetInstance(), responder);

        message = chart->messages().at(2);
        QCOMPARE(message->name(), QString("ICONind%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), responder);
        QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));

        message = chart->messages().at(3);
        QCOMPARE(message->name(), QString("ICONresp%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), static_cast<MscInstance *>(nullptr));
        QCOMPARE(message->targetInstance(), responder);

        message = chart->messages().at(4);
        QCOMPARE(message->name(), QString("ICONF%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), responder);
        QCOMPARE(message->targetInstance(), initiator);

        message = chart->messages().at(5);
        QCOMPARE(message->name(), QString("ICONconf%1").arg(x + 1));
        QCOMPARE(message->sourceInstance(), initiator);
        QCOMPARE(message->targetInstance(), static_cast<MscInstance *>(nullptr));
    }

    delete model;
}

QTEST_APPLESS_MAIN(tst_MscFile)

#include "tst_mscfile.moc"
