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

#include "baseitems/common/objectanchor.h"
#include "instanceitem.h"
#include "mscinstance.h"

#include <QGraphicsScene>
#include <QtTest>

using namespace msc;

class tst_ObjectAnchor : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void cleanup();

    void testCtorDefault();
    void testCtorParam();
    void testCtorCopy();

    void testObject();
    void testPoint();
    void testReplaceNoSnap();
    void testReplaceSnap();

private:
    QGraphicsScene *m_scene = nullptr;
    InstanceItem *m_instanceItemA = nullptr;
    InstanceItem *m_instanceItemB = nullptr;
};

void tst_ObjectAnchor::init()
{
    m_scene = new QGraphicsScene();
    m_instanceItemA = new InstanceItem(new MscInstance("InstA", this));
    m_instanceItemB = new InstanceItem(new MscInstance("InstB", this));
    m_scene->addItem(m_instanceItemA);
    m_scene->addItem(m_instanceItemB);
}

void tst_ObjectAnchor::cleanup()
{
    delete m_instanceItemB;
    delete m_instanceItemA;
    delete m_scene;
}

void tst_ObjectAnchor::testCtorDefault()
{
    const ObjectAnchor anchor;
    QCOMPARE(anchor, ObjectAnchor());
}

void tst_ObjectAnchor::testCtorParam()
{
    const ObjectAnchor anchorOrphan;
    QCOMPARE(anchorOrphan, ObjectAnchor());

    const ObjectAnchor anchorParented(this);
    QCOMPARE(anchorParented, ObjectAnchor(this));

    const QPointF pnt10(10., 10.);
    const ObjectAnchor anchorParam(m_instanceItemA, pnt10, this);
    QCOMPARE(anchorParam.object(), m_instanceItemA);
    QCOMPARE(anchorParam.point(), pnt10);
}

void tst_ObjectAnchor::testCtorCopy()
{
    const QPointF pnt10(10., 10.);
    const ObjectAnchor anchorSource(m_instanceItemA, pnt10);
    const ObjectAnchor anchorCopy(anchorSource);
    QCOMPARE(anchorCopy, anchorSource);
}

void tst_ObjectAnchor::testObject()
{
    const QGraphicsItem::GraphicsItemFlags originalFlags(m_instanceItemA->flags());

    {
        const ObjectAnchor anchorA(m_instanceItemA, QPointF());

        QCOMPARE(anchorA.object(), m_instanceItemA);
        QVERIFY(originalFlags == anchorA.originalFlags());
        QVERIFY(m_instanceItemA->flags() == anchorA.object()->flags());
    }

    // ensure flags are restored
    QVERIFY(m_instanceItemA->flags() == originalFlags);
}

void tst_ObjectAnchor::testPoint()
{
    const QPointF pnt10(10., 10.);
    const ObjectAnchor anchorA(nullptr, pnt10);
    QCOMPARE(anchorA.point(), pnt10);
}

void tst_ObjectAnchor::testReplaceNoSnap()
{
    ObjectAnchor anchor;

    QCOMPARE(anchor.object(), static_cast<InstanceItem *>(nullptr));
    QCOMPARE(anchor.point(), QPointF());

    const QPointF pnt10(10., 10.);
    bool notificationCatched(false);
    connect(&anchor, &ObjectAnchor::anchorChanged, this,
            [&notificationCatched](InteractiveObject *, const QPointF &) { notificationCatched = true; },
            Qt::DirectConnection);

    anchor.replace(m_instanceItemA, pnt10, ObjectAnchor::Snap::NoSnap);

    QCOMPARE(anchor.object(), m_instanceItemA);
    QCOMPARE(anchor.point(), pnt10);
    QCOMPARE(notificationCatched, true);
}

void tst_ObjectAnchor::testReplaceSnap()
{
    const QPointF shift1(1., 0.);
    m_instanceItemA->setPos(300., 300.);
    m_instanceItemA->setBoundingRect(QRectF(0, 0, 50, 50));
    const QPointF itemCenterA = m_instanceItemA->mapToScene(m_instanceItemA->boundingRect().center());
    const QPointF itemCenterB = m_instanceItemB->mapToScene(m_instanceItemB->boundingRect().center());
    const QPointF anchorPointA(itemCenterA + shift1);
    const QPointF anchorPointB(itemCenterB + shift1);
    ObjectAnchor anchor(m_instanceItemB, anchorPointB);

    QCOMPARE(anchor.object(), m_instanceItemB);
    QCOMPARE(anchor.point(), anchorPointB);

    bool notificationCatched(false);
    connect(&anchor, &ObjectAnchor::anchorChanged, this,
            [&notificationCatched](InteractiveObject *, const QPointF &) { notificationCatched = true; },
            Qt::DirectConnection);

    anchor.replace(m_instanceItemA, anchorPointA + shift1, ObjectAnchor::Snap::SnapTo);

    QCOMPARE(anchor.object(), m_instanceItemA);
    QCOMPARE(anchor.point(), itemCenterA);
    QCOMPARE(notificationCatched, true);
}

QTEST_MAIN(tst_ObjectAnchor)

#include "tst_objectanchor.moc"
