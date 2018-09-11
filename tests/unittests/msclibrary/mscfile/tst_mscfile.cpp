#include <exceptions.h>
#include <mscchart.h>
#include <mscdocument.h>
#include <mscfile.h>
#include <mscinstance.h>
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
    //    QVERIFY_EXCEPTION_THROWN(file->parseFile(fileName), ParserException);
}

void tst_MscFile::testEmptyDocument()
{
    file->parseText("MSCDOCUMENT CU_level;\nENDMSCDOCUMENT;");
    QCOMPARE(file->model().documents().size(), 1);
    QCOMPARE(file->model().documents().at(0)->name(), QString("CU_level"));

    file->parseText("mscdocument CI_level;\nendmscdocument;");
    QCOMPARE(file->model().documents().size(), 1);
    QCOMPARE(file->model().documents().at(0)->name(), QString("CI_level"));

    file->parseText("  \n mscdocument \n  ABC  ; \n  \n  endmscdocument  ;\n  ");
    QCOMPARE(file->model().documents().size(), 1);
    QCOMPARE(file->model().documents().at(0)->name(), QString("ABC"));
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
    file->parseText("MSCDOCUMENT l1; MSCDOCUMENT l2;ENDMSCDOCUMENT; \nENDMSCDOCUMENT;");
    auto mainDocuments = file->model().documents();
    QCOMPARE(mainDocuments.size(), 1);
    QCOMPARE(mainDocuments.at(0)->documents().size(), 1);

    // documents in parallel
    file->parseText("MSCDOCUMENT l1; MSCDOCUMENT l2;ENDMSCDOCUMENT; MSCDOCUMENT l3;ENDMSCDOCUMENT; \nENDMSCDOCUMENT;");
    mainDocuments = file->model().documents();
    QCOMPARE(mainDocuments.size(), 1);
    QCOMPARE(mainDocuments.at(0)->documents().size(), 2);
}

void tst_MscFile::testMscInDocument()
{
    file->parseText("MSC msc1;ENDMSC;");
    QCOMPARE(file->model().charts().size(), 1);
    QCOMPARE(file->model().charts().at(0)->name(), QString("msc1"));

    file->parseText("MSCDOCUMENT doc1; MSC msc1;ENDMSC; \nENDMSCDOCUMENT;");
    QCOMPARE(file->model().documents().size(), 1);
    auto mainDocument = file->model().documents().at(0);
    QCOMPARE(mainDocument->charts().size(), 1);
    QCOMPARE(mainDocument->charts().at(0)->name(), QString("msc1"));
}

void tst_MscFile::testInstance()
{
    file->parseText("MSC msc1;\nINSTANCE inst1;ENDINSTANCE;ENDMSC;");
    QCOMPARE(file->model().charts().size(), 1);
    MscChart *chart = file->model().charts().at(0);
    QCOMPARE(chart->instances().size(), 1);
    QCOMPARE(chart->instances().at(0)->name(), QString("inst1"));
}

QTEST_APPLESS_MAIN(tst_MscFile)

#include "tst_mscfile.moc"
