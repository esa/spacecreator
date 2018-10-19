#include <exceptions.h>
#include <mscchart.h>
#include <mscinstance.h>
#include <mscmessage.h>

#include <QtTest>

using namespace msc;

class tst_MscChart : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testName();
    void testAddInstance();
    void testNoDuplicateInstance();
    void testNoNullPtrInstance();
    void testInstanceByName();
    void testAddMessage();
    void testNoDuplicateMessage();
    void testNoNullPtrMessage();
    void testMessageByName();

private:
    MscChart *m_chart = nullptr;
};

void tst_MscChart::init()
{
    m_chart = new MscChart;
}

void tst_MscChart::cleanup()
{
    delete m_chart;
    m_chart = nullptr;
}

void tst_MscChart::testName()
{
    QCOMPARE(m_chart->name(), QString());

    const QString chartName("Rosetta");
    m_chart->setName(chartName);
    QCOMPARE(m_chart->name(), chartName);
}

void tst_MscChart::testAddInstance()
{
    QCOMPARE(m_chart->instances().size(), 0);

    auto instance1 = new MscInstance("IN", m_chart);
    m_chart->addInstance(instance1);
    auto instance2 = new MscInstance("OUT", m_chart);
    m_chart->addInstance(instance2);
    QCOMPARE(m_chart->instances().size(), 2);
}

void tst_MscChart::testNoDuplicateInstance()
{
    auto instance = new MscInstance("IN", m_chart);
    m_chart->addInstance(instance);
    m_chart->addInstance(instance);
    QCOMPARE(m_chart->instances().size(), 1);
}

void tst_MscChart::testNoNullPtrInstance()
{
    m_chart->addInstance(nullptr);
    QCOMPARE(m_chart->instances().size(), 0);
}

void tst_MscChart::testInstanceByName()
{
    auto instance = new MscInstance("IN", m_chart);
    m_chart->addInstance(instance);
    QCOMPARE(m_chart->instanceByName("IN"), instance);
    QCOMPARE(m_chart->instanceByName("OUT"), nullptr);
}

void tst_MscChart::testAddMessage()
{
    QCOMPARE(m_chart->messages().size(), 0);

    auto message1 = new MscMessage("IN", m_chart);
    m_chart->addMessage(message1);
    auto message2 = new MscMessage("OUT", m_chart);
    m_chart->addMessage(message2);
    QCOMPARE(m_chart->messages().size(), 2);
}

void tst_MscChart::testNoDuplicateMessage()
{
    auto message = new MscMessage("IN", m_chart);
    m_chart->addMessage(message);
    m_chart->addMessage(message);
    QCOMPARE(m_chart->messages().size(), 1);
}

void tst_MscChart::testNoNullPtrMessage()
{
    m_chart->addMessage(nullptr);
    QCOMPARE(m_chart->messages().size(), 0);
}

void tst_MscChart::testMessageByName()
{
    auto message = new MscMessage("IN", m_chart);
    m_chart->addMessage(message);
    QCOMPARE(m_chart->messageByName("IN"), message);
    QCOMPARE(m_chart->messageByName("OUT"), nullptr);
}

QTEST_APPLESS_MAIN(tst_MscChart)

#include "tst_mscchart.moc"
