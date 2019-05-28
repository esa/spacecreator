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

#include "baseitems/common/coordinatesconverter.h"
#include "chartitem.h"
#include "chartviewmodel.h"
#include "commands/common/commandsstack.h"
#include "exceptions.h"
#include "messageitem.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscfile.h"
#include "mscmessage.h"
#include "mscmodel.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScopedPointer>
#include <QUndoStack>
#include <QVector>
#include <QtTest>
#include <cmath>

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
    void testNameWithParameterWithExtraBraces();
    void testNameAndParameterEntering();
    void testNameAndParametersEntering();
    void testPositionUpdateOnInstanceChange();
    void testFirstMessagePosition();

private:
    void enterText(const QString &text);
    void parseMsc(const QString &mscText);

    MscMessage *m_message = nullptr;
    QPointer<MessageItem> m_messageItem = nullptr;

    QGraphicsView *m_view = nullptr;
    QUndoStack *m_undoStack = nullptr;

    ChartViewModel *m_chartModel = nullptr;
    MscModel *m_mscModel = nullptr;
    QPointer<MscChart> m_chart;
    QVector<MscInstance *> m_instances;
    QVector<InstanceItem *> m_instanceItems;

    qreal m_maxOffset = 1.5;
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

void tst_MessageItem::parseMsc(const QString &mscText)
{
    MscFile mscFile;
    m_mscModel = mscFile.parseText(mscText);

    QVERIFY(!m_mscModel->documents().isEmpty());
    QVERIFY(m_mscModel->documents().first());
    QVERIFY(!m_mscModel->documents().first()->charts().isEmpty());
    m_chart = m_mscModel->documents().first()->charts().first();
    QVERIFY(m_chart);

    m_chartModel->fillView(m_chart);
    QApplication::processEvents();

    for (MscInstance *instance : m_chart->instances()) {
        InstanceItem *instanceItem = m_chartModel->itemForInstance(instance);
        QVERIFY(instanceItem != nullptr);

        m_instances.append(instance);
        m_instanceItems.append(instanceItem);
    }
}

void tst_MessageItem::init()
{
    m_chartModel = new ChartViewModel;
    m_undoStack = new QUndoStack();
    cmd::CommandsStack::setCurrent(m_undoStack);

    m_view = new QGraphicsView();
    m_view->setScene(m_chartModel->graphicsScene());

    m_message = new MscMessage("Event1");
    m_messageItem = new MessageItem(m_message, nullptr);

    m_view->scene()->addItem(m_messageItem);
}

void tst_MessageItem::cleanup()
{
    delete m_messageItem;
    m_messageItem = nullptr;
    delete m_message;
    m_message = nullptr;
    delete m_view;
    m_view = nullptr;
    delete m_undoStack;
    m_undoStack = nullptr;
    delete m_chartModel;
    m_chartModel = nullptr;

    m_instances.clear();
    m_instanceItems.clear();
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

    m_message->setParameters({ "", "", { { "", "pattern" } } });
    m_messageItem = new MessageItem(m_message, nullptr);

    QCOMPARE(m_messageItem->displayedText(), QString("Event1(pattern)"));

    m_message->setParameters({ "", "", { { "", "pattern" }, { "ex: pression", "" } } });
    QCOMPARE(m_messageItem->displayedText(), QString("Event1(pattern, ex: pression)"));
}

void tst_MessageItem::testNameWithParameterWithExtraBraces()
{
    delete m_messageItem;

    m_message->setParameters({ "{", "}", { { "", "pattern" } } });
    m_messageItem = new MessageItem(m_message, nullptr);

    QCOMPARE(m_messageItem->displayedText(), QString("Event1({pattern})"));

    m_message->setParameters({ "{", "}", { { "", "pattern" }, { "ex: pression", "" } } });
    QCOMPARE(m_messageItem->displayedText(), QString("Event1({pattern, ex: pression})"));
}

void tst_MessageItem::testNameAndParameterEntering()
{
    enterText("call(47)");
    QCOMPARE(m_message->name(), QString("call"));
    QCOMPARE(m_message->parameters().data().size(), 1);
    QCOMPARE(m_message->parameters().data().at(0).pattern(), QString("47"));
}

void tst_MessageItem::testNameAndParametersEntering()
{
    enterText("call(47, ex: pression)");
    QCOMPARE(m_message->name(), QString("call"));
    QCOMPARE(m_message->parameters().data().size(), 2);
    QCOMPARE(m_message->parameters().data().at(0).pattern(), QString("47"));
    QCOMPARE(m_message->parameters().data().at(1).expression(), QString("ex: pression"));
}

void tst_MessageItem::testPositionUpdateOnInstanceChange()
{
    QString mscText = "MSCDOCUMENT mscdoc;\
                        msc Untitled_MSC;\
                            instance Instance_1;\
                                out Message to Instance_2;\
                            endinstance;\
                            instance Instance_2;\
                                in Message from Instance_1;\
                            endinstance;\
                            instance Instance_3;\
                            endinstance;\
                        endmsc;\
                    ENDMSCDOCUMENT;";
    parseMsc(mscText);
    QCOMPARE(m_instanceItems.size(), 3);
    QCOMPARE(m_chart->instanceEvents().size(), 1);

    MscMessage *message = qobject_cast<MscMessage *>(m_chart->instanceEvents().at(0));
    MessageItem *messageItem = m_chartModel->itemForMessage(message);
    QVERIFY(messageItem != nullptr);

    QVector<qreal> instanceAxesX { m_instanceItems[0]->axis().p1().x(), m_instanceItems[1]->axis().p1().x(),
                                   m_instanceItems[2]->axis().p1().x() };

    QVector<QPointF> points = messageItem->messagePoints();
    QVERIFY(qFuzzyCompare(points[0].x(), instanceAxesX[0]));
    QVERIFY(qFuzzyCompare(points[1].x(), instanceAxesX[1]));

    message->setTargetInstance(m_instances[2]);
    QApplication::processEvents();
    points = messageItem->messagePoints();

    QVERIFY(std::abs(points[0].x() - instanceAxesX[0]) < m_maxOffset);
    QVERIFY(std::abs(points[1].x() - instanceAxesX[2]) < m_maxOffset);

    message->setSourceInstance(m_instances[1]);
    QApplication::processEvents();
    points = messageItem->messagePoints();
    QVERIFY(std::abs(points[0].x() - instanceAxesX[1]) < m_maxOffset);
    QVERIFY(std::abs(points[1].x() - instanceAxesX[2]) < m_maxOffset);
}

void tst_MessageItem::testFirstMessagePosition()
{
    QString mscText = "MSCDOCUMENT mscdoc;\
        msc connection;\
        instance A: 111111111111111111111111122222222222222222222222223333333333333333333333333 ;\
            in EA from env;\
            out AE to env;\
            in BA from B;\
            out AB to B;\
        endinstance;\
        instance B: process ISAP_Manager_Resp ;\
            out BA to A;\
            in AB from A;\
            in CBLongLongNamedEvenLongerABit from C;\
        endinstance;\
        instance C: process np ;\
            out CBLongLongNamedEvenLongerABit to B;\
        endinstance;\
        instance LongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLong: process np ;\
        endinstance;\
        instance ShortName: LongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongLongKind ;\
            in E2ShortName from env;\
            out ShortName2E to env;\
        endinstance;\
    endmsc;\
    ENDMSCDOCUMENT;";
    parseMsc(mscText);
    QCOMPARE(m_instanceItems.size(), 5);
    QCOMPARE(m_chart->instanceEvents().size(), 7);

    MscMessage *message = qobject_cast<MscMessage *>(m_chart->instanceEvents().at(0));
    MessageItem *messageItem = m_chartModel->itemForMessage(message);
    QVERIFY(messageItem != nullptr);
    ChartItem *chartItem = m_chartModel->chartItem();
    QVERIFY(chartItem != nullptr);

    // the first message has to start at the left chart
    QVector<QPointF> points = messageItem->messagePoints();
    QVERIFY(std::abs(points[0].x() - chartItem->sceneBoundingRect().x()) < m_maxOffset);
    QVERIFY(std::abs(points[1].x() - m_instanceItems[0]->axis().p1().x()) < m_maxOffset);
}

QTEST_MAIN(tst_MessageItem)

#include "tst_messageitem.moc"
