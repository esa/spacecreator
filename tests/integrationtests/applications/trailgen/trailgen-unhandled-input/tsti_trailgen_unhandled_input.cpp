#include <QObject>
#include <QtTest>
#include <stdlib.h>

namespace tmc::test {
class tsti_TrailgenUnhandledInput : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test_trailgenUnhandledInput();
};

void tsti_TrailgenUnhandledInput::test_trailgenUnhandledInput()
{
    const int result = system("./test_trailgen_unhandled_input.sh");
    QCOMPARE(result, 0);
}
}
QTEST_MAIN(tmc::test::tsti_TrailgenUnhandledInput)

#include "tsti_trailgen_unhandled_input.moc"
