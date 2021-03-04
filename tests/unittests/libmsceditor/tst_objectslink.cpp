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

#include "baseitems/common/mscutils.h"
#include "baseitems/common/objectanchor.h"
#include "baseitems/common/objectslink.h"
#include "instanceitem.h"
#include "mscinstance.h"
#include "sharedlibrary.h"

#include <QGraphicsScene>
#include <QtTest>

using namespace msc;

class tst_ObjectsLink : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    void testMakeLink();
    void testLinkCenter();
    void testReplaceSourceNoSnap();
    void testReplaceSourceSnap();
    void testReplaceTargetNoSnap();
    void testReplaceTargetSnap();

private:
    QGraphicsScene *m_scene = nullptr;
    InstanceItem *m_itemA = nullptr;
    InstanceItem *m_itemB = nullptr;
    InstanceItem *m_itemC = nullptr;
    QPointF centerA, centerB, centerC, centerCShifted;
};

void tst_ObjectsLink::init()
{
    shared::initSharedLibrary();
    m_scene = new QGraphicsScene();

    m_itemA = new InstanceItem(new MscInstance("A", this), nullptr);
    m_itemB = new InstanceItem(new MscInstance("B", this), nullptr);
    m_itemC = new InstanceItem(new MscInstance("C", this), nullptr);

    for (InstanceItem *item : { m_itemA, m_itemB, m_itemC }) {
        m_scene->addItem(item);
        item->setGeometry(QRectF(0, 0, 50, 50));
    }

    m_itemA->setPos(100., 100.);
    m_itemB->setPos(200., 100.);
    m_itemC->setPos(100., 200.);

    centerA = QPointF(m_itemA->mapToScene(m_itemA->boundingRect().center()));
    centerB = QPointF(m_itemB->mapToScene(m_itemB->boundingRect().center()));
    centerC = QPointF(m_itemC->mapToScene(m_itemC->boundingRect().center()));
    centerCShifted = QPointF(centerC - QPointF(1., 0.));
}

void tst_ObjectsLink::cleanup()
{
    delete m_itemC;
    delete m_itemB;
    delete m_itemA;
    delete m_scene;
}

void tst_ObjectsLink::testMakeLink()
{
    centerA = m_itemA->mapToScene(m_itemA->boundingRect().center());
    centerB = m_itemB->mapToScene(m_itemB->boundingRect().center());
    const QLineF ab(centerA, centerB);

    ObjectsLink link;
    const QPointF linkCenter = link.makeLink(m_itemA, centerA, m_itemB, centerB);

    QCOMPARE(linkCenter, ab.center());

    QCOMPARE(link.source()->object(), m_itemA);
    QCOMPARE(link.source()->point(), centerA);

    QCOMPARE(link.target()->object(), m_itemB);
    QCOMPARE(link.target()->point(), centerB);
}

void tst_ObjectsLink::testLinkCenter()
{
    centerA = m_itemA->mapToScene(m_itemA->boundingRect().center());
    centerB = m_itemB->mapToScene(m_itemB->boundingRect().center());
    const QLineF ab(centerA, centerB);

    ObjectsLink link;
    link.makeLink(m_itemA, centerA, m_itemB, centerB);

    QCOMPARE(link.linkCenter(), ab.center());
}

void tst_ObjectsLink::testReplaceSourceNoSnap()
{
    ObjectsLink link;
    link.makeLink(m_itemA, centerA, m_itemB, centerB);

    bool notifiyed(false);
    connect(
            &link, &ObjectsLink::sourceAnchorChanged, this, [&notifiyed](const QPointF &) { notifiyed = true; },
            Qt::DirectConnection);

    link.replaceSource(m_itemC, centerCShifted, ObjectAnchor::Snap::NoSnap);
    QCOMPARE(link.source()->object(), m_itemC);
    QCOMPARE(link.source()->point(), centerCShifted);
}

void tst_ObjectsLink::testReplaceSourceSnap()
{
    ObjectsLink link;
    link.makeLink(m_itemA, centerA, m_itemB, centerB);

    bool notifiyed(false);
    connect(
            &link, &ObjectsLink::sourceAnchorChanged, this, [&notifiyed](const QPointF &) { notifiyed = true; },
            Qt::DirectConnection);

    link.replaceSource(m_itemC, centerCShifted, ObjectAnchor::Snap::SnapTo);
    QCOMPARE(link.source()->object(), m_itemC);
    QCOMPARE(link.source()->point(), centerC);
}

void tst_ObjectsLink::testReplaceTargetNoSnap()
{
    ObjectsLink link;
    link.makeLink(m_itemA, centerA, m_itemB, centerB);

    bool notifiyed(false);
    connect(
            &link, &ObjectsLink::sourceAnchorChanged, this, [&notifiyed](const QPointF &) { notifiyed = true; },
            Qt::DirectConnection);

    link.replaceTarget(m_itemC, centerCShifted, ObjectAnchor::Snap::NoSnap);
    QCOMPARE(link.target()->object(), m_itemC);
    QCOMPARE(link.target()->point(), centerCShifted);
}

void tst_ObjectsLink::testReplaceTargetSnap()
{
    ObjectsLink link;
    link.makeLink(m_itemA, centerA, m_itemB, centerB);

    bool notifiyed(false);
    connect(
            &link, &ObjectsLink::sourceAnchorChanged, this, [&notifiyed](const QPointF &) { notifiyed = true; },
            Qt::DirectConnection);

    link.replaceTarget(m_itemC, centerCShifted, ObjectAnchor::Snap::SnapTo);
    QCOMPARE(link.target()->object(), m_itemC);
    QCOMPARE(link.target()->point(), centerC);
}

QTEST_MAIN(tst_ObjectsLink)

#include "tst_objectslink.moc"
