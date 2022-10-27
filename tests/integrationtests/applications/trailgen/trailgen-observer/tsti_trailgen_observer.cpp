#include <QObject>
#include <QtTest>
#include <stdlib.h>

namespace tmc::test {
class tsti_TrailgenObserver : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test_trailgenObserver();
};

void tsti_TrailgenObserver::test_trailgenObserver()
{
    const int result = system("./test_trailgen_observer.sh");
    QCOMPARE(result, 0);
}
}
QTEST_MAIN(tmc::test::tsti_TrailgenObserver)

#include "tsti_trailgen_observer.moc"
