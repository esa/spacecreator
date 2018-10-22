#include <exceptions.h>
#include <messageitem.h>
#include <mscmessage.h>

#include <QtTest>

using namespace msc;

class tst_MessageItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testNameUpdate();

private:
    MscMessage *m_message = nullptr;
    MessageItem *m_messageItem = nullptr;
};

void tst_MessageItem::init()
{
    m_message = new MscMessage("Event1");
    m_messageItem = new MessageItem(m_message);
}

void tst_MessageItem::cleanup()
{
    delete m_messageItem;
    m_messageItem = nullptr;
    delete m_message;
    m_message = nullptr;
}

void tst_MessageItem::testNameUpdate()
{
    QCOMPARE(m_messageItem->name(), QString("Event1"));

    m_message->setName("OutCall");
    QCOMPARE(m_messageItem->name(), QString("OutCall"));
}

QTEST_MAIN(tst_MessageItem)

#include "tst_messageitem.moc"
