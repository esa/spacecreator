#include <QObject>
#include <QtTest>
#include <stdlib.h>

namespace tmc::test {
class tsti_TrailgenTimers : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test_trailgenTimers();
};

void tsti_TrailgenTimers::test_trailgenTimers()
{
    const int result = system("./test_trailgen_timers.sh");
    QCOMPARE(result, 0);
}
}

QTEST_MAIN(tmc::test::tsti_TrailgenTimers)

#include "tsti_trailgen_timers.moc"
