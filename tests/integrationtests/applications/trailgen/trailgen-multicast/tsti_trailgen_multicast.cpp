#include <QObject>
#include <QtTest>
#include <stdlib.h>

namespace tmc::test {
class tsti_TrailgenMulticast : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test_trailgenMulticast();
};

void tsti_TrailgenMulticast::test_trailgenMulticast()
{
    const int result = system("./test_trailgen_multicast.sh");
    QCOMPARE(result, 0);
}
}

QTEST_MAIN(tmc::test::tsti_TrailgenMulticast)

#include "tsti_trailgen_multicast.moc"
