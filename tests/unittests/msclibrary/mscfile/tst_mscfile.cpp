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

public:
private slots:
    void init();
    void testFileOpenError();
    void testSyntaxError();
    void testEmptyDocument();
    void testComments();
    void testNestedDocuments();
    void testMscInDocument();
    void testInstance();
    void testMessage();
    void testSameMessageInTwoInstances();
    void testGateMessage();

private:
    MscFile *file = nullptr;
};

void tst_MscFile::init()
{
    file = new MscFile;
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

    model = file->parseText("mscdocument inst_1_cu_nominal.cu_controller;\nendmscdocument;");
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->name(), QString("inst_1_cu_nominal.cu_controller"));

    model = file->parseText("  \n mscdocument \n  ABC  ; \n  \n  endmscdocument  ;\n  ");
    QCOMPARE(model->documents().size(), 1);
    QCOMPARE(model->documents().at(0)->name(), QString("ABC"));
    // no exception thrown
}

void tst_MscFile::testComments()
{
    file->parseText("/* This is a comment*/MSCDOCUMENT CU_level;\nENDMSCDOCUMENT;");
    file->parseText("MSCDOCUMENT CU_level /*This is a comment */ ;\nENDMSCDOCUMENT;");
    file->parseText("MSCDOCUMENT CU_level;\n/* This is a comment*/\nENDMSCDOCUMENT;");
    // no exception thrown
}

void tst_MscFile::testNestedDocuments()
{
    MscModel *model = file->parseText("MSCDOCUMENT l1; MSCDOCUMENT l2;ENDMSCDOCUMENT; \nENDMSCDOCUMENT;");
    auto mainDocuments = model->documents();
    QCOMPARE(mainDocuments.size(), 1);
    QCOMPARE(mainDocuments.at(0)->documents().size(), 1);

    // documents in parallel
    model = file->parseText("MSCDOCUMENT l1; MSCDOCUMENT l2;ENDMSCDOCUMENT; MSCDOCUMENT l3;ENDMSCDOCUMENT; \nENDMSCDOCUMENT;");
    mainDocuments = model->documents();
    QCOMPARE(mainDocuments.size(), 1);
    QCOMPARE(mainDocuments.at(0)->documents().size(), 2);
}

void tst_MscFile::testMscInDocument()
{
    MscModel *model = file->parseText("MSC msc1;ENDMSC;");
    QCOMPARE(model->charts().size(), 1);
    QCOMPARE(model->charts().at(0)->name(), QString("msc1"));

    model = file->parseText("MSCDOCUMENT doc1; MSC msc1;ENDMSC; \nENDMSCDOCUMENT;");
    QCOMPARE(model->documents().size(), 1);
    auto mainDocument = model->documents().at(0);
    QCOMPARE(mainDocument->charts().size(), 1);
    QCOMPARE(mainDocument->charts().at(0)->name(), QString("msc1"));
}

void tst_MscFile::testInstance()
{
    MscModel *model = file->parseText("MSC msc1;\nINSTANCE inst1;ENDINSTANCE;ENDMSC;");
    QCOMPARE(model->charts().size(), 1);
    MscChart *chart = model->charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instances().at(0)->name(), QString("inst1"));

    // with decomposition
    //    model = file->parseText("MSC msc1;INSTANCE foo : PROCESS satellite com1;ENDINSTANCE;ENDMSC;");
    //    QCOMPARE(model->charts().size(), 1);
    //    chart = model->charts().at(0);
    //    QCOMPARE(chart->instances().size(), 1);
    //    MscInstance *instance = chart->instances().at(0);
    //    QCOMPARE(instance->name(), QString("foo"));
    //    QCOMPARE(instance->kind(), QString("PROCESS"));
    //    QStringList decomp = { "satellite", "com1" };
    //    QCOMPARE(instance->decomposition(), decomp);
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
    QCOMPARE(message1->sourceInstance(), nullptr);
    QCOMPARE(message1->targetInstance(), instance);

    MscMessage *message2 = chart->messages().at(1);
    QCOMPARE(message2->name(), QString("ICON"));
    QCOMPARE(message2->sourceInstance(), instance);
    QCOMPARE(message2->targetInstance(), nullptr);
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
}

QTEST_APPLESS_MAIN(tst_MscFile)

#include "tst_mscfile.moc"
