/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "commands/common/commandsstack.h"
#include "exceptions.h"
#include "messageitem.h"
#include "mscmessage.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QUndoStack>
#include <QtTest>

using namespace msc;

class tst_MessageItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();
    void testNameUpdate();
    void testNameEntering();
    void testNameWithParameter();
    void testNameAndParameterEntering();
    void testNameAndParametersEntering();

private:
    void enterText(const QString &text);

    MscMessage *m_message = nullptr;
    MessageItem *m_messageItem = nullptr;

    QGraphicsView *m_view = nullptr;
    QUndoStack *m_undoStack = nullptr;
};

void tst_MessageItem::enterText(const QString &text)
{
    if (!m_view)
        return;

    QTest::mouseMove(m_view->viewport());
    QTest::mouseDClick(m_view->viewport(), Qt::LeftButton);
    QTest::keyClicks(m_view->viewport(), text);
    QTest::keyClick(m_view->viewport(), Qt::Key_Return);
}

void tst_MessageItem::init()
{
    m_undoStack = new QUndoStack();
    cmd::CommandsStack::setCurrent(m_undoStack);

    m_view = new QGraphicsView();
    m_view->setScene(new QGraphicsScene(m_view));

    m_message = new MscMessage("Event1");
    m_messageItem = new MessageItem(m_message, nullptr, nullptr, 0.);

    m_view->scene()->addItem(m_messageItem);
}

void tst_MessageItem::cleanup()
{
    delete m_messageItem;
    m_messageItem = nullptr;
    delete m_message;
    m_message = nullptr;

    delete m_undoStack;
    m_undoStack = nullptr;
}

void tst_MessageItem::testNameUpdate()
{
    QCOMPARE(m_messageItem->displayedText(), QString("Event1"));

    m_message->setName("OutCall");
    QCOMPARE(m_messageItem->displayedText(), QString("OutCall"));
}

void tst_MessageItem::testNameEntering()
{
    enterText("Ping");
    QCOMPARE(m_message->name(), QString("Ping"));
}

void tst_MessageItem::testNameWithParameter()
{
    delete m_messageItem;

    m_message->setParameters({ { "", "pattern" } });
    m_messageItem = new MessageItem(m_message, nullptr, nullptr, 0.);

    QCOMPARE(m_messageItem->displayedText(), QString("Event1(pattern)"));

    m_message->setParameters({ { "", "pattern" }, { "ex: pression", "" } });
    QCOMPARE(m_messageItem->displayedText(), QString("Event1(pattern, ex: pression)"));
}

void tst_MessageItem::testNameAndParameterEntering()
{
    enterText("call(47)");
    QCOMPARE(m_message->name(), QString("call"));
    QCOMPARE(m_message->parameters().size(), 1);
    QCOMPARE(m_message->parameters().at(0).pattern(), QString("47"));
}

void tst_MessageItem::testNameAndParametersEntering()
{
    enterText("call(47, ex: pression)");
    QCOMPARE(m_message->name(), QString("call"));
    QCOMPARE(m_message->parameters().size(), 2);
    QCOMPARE(m_message->parameters().at(0).pattern(), QString("47"));
    QCOMPARE(m_message->parameters().at(1).expression(), QString("ex: pression"));
}

QTEST_MAIN(tst_MessageItem)

#include "tst_messageitem.moc"
