/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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
    m_messageItem = new MessageItem(m_message, nullptr, nullptr, 0.);
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
