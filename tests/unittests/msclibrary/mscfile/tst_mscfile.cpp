#include <mscfile.h>

#include <QtTest>

class tst_MscFile : public QObject
{
    Q_OBJECT

public:
    tst_MscFile();
    ~tst_MscFile();

private slots:
    void test_case1();
};

tst_MscFile::tst_MscFile()
{
}

tst_MscFile::~tst_MscFile()
{
}

void tst_MscFile::test_case1()
{
    MscFile file;
    QCOMPARE(1, 1);
}

QTEST_APPLESS_MAIN(tst_MscFile)

#include "tst_mscfile.moc"
