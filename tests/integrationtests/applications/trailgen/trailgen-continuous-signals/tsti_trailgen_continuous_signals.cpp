#include <QObject>
#include <QtTest>
#include <stdlib.h>

namespace tmc::test {
class tsti_TrailgenContinuousSignals : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test_trailgenContinuousSignals();
};

void tsti_TrailgenContinuousSignals::test_trailgenContinuousSignals()
{
    const int result = system("./test_trailgen_continuous_signals.sh");
    QCOMPARE(result, 0);
}
}

QTEST_MAIN(tmc::test::tsti_TrailgenContinuousSignals)

#include "tsti_trailgen_continuous_signals.moc"
