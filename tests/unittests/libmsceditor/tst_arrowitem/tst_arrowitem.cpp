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

#include "baseitems/arrowitem.h"
#include "baseitems/common/mscutils.h"
#include "baseitems/common/objectslink.h"
#include "chartlayoutmanager.h"
#include "instanceitem.h"
#include "mscchart.h"
#include "mscinstance.h"

#include <QGraphicsScene>
#include <QtTest>
#include <chrono>
#include <random>

using namespace msc;

class tst_ArrowItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testMakeArrow();
    void testBoundingRect();
    void testUpdateStart();
    void testUpdateEnd();
    void testSignLocals();
    void testObjectsLinkItem();

private:
    QScopedPointer<ChartLayoutManager> m_model;
    QScopedPointer<QGraphicsScene> m_scene;
    QScopedPointer<msc::MscChart> m_chart;
    QPointer<InstanceItem> m_item1;
    QPointer<InstanceItem> m_item2;

    QRectF instanceRect(const InstanceItem *const instance) const;
    int scenePopulation() const;
};

void tst_ArrowItem::initTestCase()
{
    m_model.reset(new ChartLayoutManager);
    m_chart.reset(new msc::MscChart);
    m_model->setCurrentChart(m_chart.data());
    m_scene.reset(new QGraphicsScene);
    m_item1 = m_model->createDefaultInstanceItem();
    m_item2 = m_model->createDefaultInstanceItem();

    m_scene->addItem(m_item1);
    m_scene->addItem(m_item2);

    m_item1->setPos(QPointF(100., 100.));
    m_item2->setPos(QPointF(600., 100.));
}

void tst_ArrowItem::cleanupTestCase()
{
    if (removeSceneItem(m_item2))
        delete m_item2;
    if (removeSceneItem(m_item1))
        delete m_item1;
}

int tst_ArrowItem::scenePopulation() const
{
    int res(0);
    for (const QGraphicsItem *item : m_scene->items())
        if (!item->parentItem())
            ++res;
    return res;
}

QRectF tst_ArrowItem::instanceRect(const InstanceItem *const instance) const
{
    return instance->boundingRect().translated(instance->pos());
};

void tst_ArrowItem::testMakeArrow()
{
    QCOMPARE(scenePopulation(), 2);
    ArrowItem ai;

    QCOMPARE(ai.pos(), QPointF());
    QCOMPARE(ai.boundingRect(), QRectF());
    QCOMPARE(ai.anchorPointSource(), QPointF());
    QCOMPARE(ai.anchorPointTarget(), QPointF());

    const QRectF &rect1(instanceRect(m_item1));
    const QRectF &rect2(instanceRect(m_item2));

    ai.makeArrow(m_item1, rect1.center(), m_item2, rect2.center());

    QCOMPARE(ai.pos(), QPointF());
    QVERIFY(ai.boundingRect() != QRectF());
    QCOMPARE(ai.anchorPointSource(), rect1.center());
    QCOMPARE(ai.anchorPointTarget(), rect2.center());
}

void tst_ArrowItem::testBoundingRect()
{
    ArrowItem ai;
    QCOMPARE(ai.boundingRect(), QRectF());

    const QRectF &rect1(instanceRect(m_item1));
    const QRectF &rect2(instanceRect(m_item2));
    const QLineF itemCenters(rect1.center(), rect2.center());

    ai.makeArrow(m_item1, rect1.center(), m_item2, rect2.center());
    ai.setPos(itemCenters.center());

    const QRectF &aiBounds(ai.sceneBoundingRect());
    QCOMPARE(aiBounds.width(), itemCenters.length());
    QVERIFY(!qFuzzyIsNull(aiBounds.height()));
    QCOMPARE(QLineF(aiBounds.topLeft(), aiBounds.bottomLeft()).center(), itemCenters.p1());
    QCOMPARE(QLineF(aiBounds.topRight(), aiBounds.bottomRight()).center(), itemCenters.p2());
}

void tst_ArrowItem::testUpdateStart()
{
    const QPointF shift(10, 10);
    const QRectF &rect1(instanceRect(m_item1));
    const QRectF &rect2(instanceRect(m_item2));

    ArrowItem ai;
    ai.makeArrow(m_item1, rect1.center(), m_item2, rect2.center());
    ai.updateStart(m_item1, ai.anchorPointSource() + shift, ObjectAnchor::Snap::NoSnap);
    QCOMPARE(ai.link()->source()->object(), static_cast<InteractiveObject *>(m_item1.data()));
    QCOMPARE(ai.anchorPointSource(), rect1.center() + shift);

    ai.updateStart(m_item2, ai.anchorPointTarget(), ObjectAnchor::Snap::NoSnap);
    QCOMPARE(ai.link()->source()->object(), static_cast<InteractiveObject *>(m_item2.data()));
    QCOMPARE(ai.anchorPointSource(), rect2.center());
}

void tst_ArrowItem::testUpdateEnd()
{
    const QPointF shift(10, 10);
    const QRectF &rect1(instanceRect(m_item1));
    const QRectF &rect2(instanceRect(m_item2));

    ArrowItem ai;
    ai.makeArrow(m_item1, rect1.center(), m_item2, rect2.center());
    ai.updateEnd(m_item2, ai.anchorPointTarget() + shift, ObjectAnchor::Snap::NoSnap);
    QCOMPARE(ai.link()->target()->object(), static_cast<InteractiveObject *>(m_item2.data()));
    QCOMPARE(ai.anchorPointTarget(), rect2.center() + shift);

    ai.updateEnd(m_item1, ai.anchorPointSource(), ObjectAnchor::Snap::NoSnap);
    QCOMPARE(ai.link()->target()->object(), static_cast<InteractiveObject *>(m_item1.data()));
    QCOMPARE(ai.anchorPointTarget(), rect1.center());
}

void tst_ArrowItem::testSignLocals()
{
    const QRectF &rect1(instanceRect(m_item1));
    const QRectF &rect2(instanceRect(m_item2));
    const QLineF itemCenters(rect1.center(), rect2.center());

    ArrowItem ai;
    ai.makeArrow(m_item1, rect1.center(), m_item2, rect2.center());

    QCOMPARE(ai.startSignLocal().x(), -itemCenters.length() / 2.);
    QCOMPARE(ai.endSignLocal().x(), itemCenters.length() / 2.);
}

void tst_ArrowItem::testObjectsLinkItem()
{
    // TODO: Use the Qt::QRandomGenerator after switching to the Qt 5.10:
    std::default_random_engine generator(
            static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<double> doubler(0., 1.);
    std::uniform_int_distribution<int> penStyler(Qt::SolidLine, Qt::DashDotDotLine);
    std::uniform_int_distribution<int> brushStyler(Qt::SolidPattern, Qt::ConicalGradientPattern);

    auto generatePen = [&doubler, &penStyler, &generator]() {
        QPen pen;
        pen.setColor(QColor::fromRgbF(doubler(generator), doubler(generator), doubler(generator), doubler(generator)));
        pen.setStyle(Qt::PenStyle(penStyler(generator)));
        pen.setWidthF(doubler(generator));

        return pen;
    };

    auto generateBrush = [&doubler, &brushStyler, &generator]() {
        QBrush brush;
        brush.setColor(
                QColor::fromRgbF(doubler(generator), doubler(generator), doubler(generator), doubler(generator)));
        brush.setStyle(Qt::BrushStyle(brushStyler(generator)));

        return brush;
    };

    ArrowItem ai;

    QBrush brush = generateBrush();
    ai.setBodyBrush(brush);
    QCOMPARE(ai.bodyBrush(), brush);

    brush = generateBrush();
    ai.setSourceBrush(brush);
    QCOMPARE(ai.sourceBrush(), brush);

    brush = generateBrush();
    ai.setTargetBrush(brush);
    QCOMPARE(ai.targetBrush(), brush);

    QPen pen = generatePen();
    ai.setBodyPen(pen);
    QCOMPARE(ai.bodyPen(), pen);

    pen = generatePen();
    ai.setSourcePen(pen);
    QCOMPARE(ai.sourcePen(), pen);

    pen = generatePen();
    ai.setTargetPen(pen);
    QCOMPARE(ai.targetPen(), pen);

    ai.setStartSignShown(false);
    QCOMPARE(ai.startSignShown(), false);
    ai.setStartSignShown(true);
    QCOMPARE(ai.startSignShown(), true);

    ai.setEndSignShown(false);
    QCOMPARE(ai.endSignShown(), false);
    ai.setEndSignShown(true);
    QCOMPARE(ai.endSignShown(), true);
}

QTEST_MAIN(tst_ArrowItem)

#include "tst_arrowitem.moc"
