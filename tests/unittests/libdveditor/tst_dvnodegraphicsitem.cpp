/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dveditor.h"
#include "dvnode.h"
#include "dvpartition.h"
#include "graphicsviewutils.h"
#include "itemeditor/dvnodegraphicsitem.h"
#include "itemeditor/dvpartitiongraphicsitem.h"
#include "standardpaths.h"

#include <QGraphicsScene>
#include <QtTest>

using namespace shared::graphicsviewutils;

class tst_DVNodeGraphicsItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

    void testResizeOnChildchangeToLeft();
    void testResizeOnChildchangeToTop();
    void testResizeOnChildchangeToWidth();
    void testResizeOnChildchangeToHeight();

private:
    std::unique_ptr<QGraphicsScene> m_scene;
    std::unique_ptr<dvm::DVNode> m_node;
    dve::DVNodeGraphicsItem *m_nodeItem = nullptr;
    std::unique_ptr<dvm::DVPartition> m_partition;
    dve::DVPartitionGraphicsItem *m_partitionItem = nullptr;
};

void tst_DVNodeGraphicsItem::initTestCase()
{
    shared::StandardPaths::setTestModeEnabled(true);
    dve::initDvEditor();
}

void tst_DVNodeGraphicsItem::init()
{
    m_scene = std::make_unique<QGraphicsScene>();
    m_node = std::make_unique<dvm::DVNode>();

    m_nodeItem = new dve::DVNodeGraphicsItem(m_node.get());
    m_scene->addItem(m_nodeItem);
    m_nodeItem->setRect(QRectF(300, 200, 160, 160));

    m_partition = std::make_unique<dvm::DVPartition>();
    m_partitionItem = new dve::DVPartitionGraphicsItem(m_partition.get(), m_nodeItem);
}

void tst_DVNodeGraphicsItem::cleanup()
{
    m_partitionItem = nullptr;
}

void tst_DVNodeGraphicsItem::testResizeOnChildchangeToLeft()
{
    m_partitionItem->setRect(QRectF(280, 250, 50, 50)); // content is on the left
    m_nodeItem->childBoundingBoxChanged();
    const QRectF expectedRect(280 - kContentMargins.left(), 200, 160 + 20 + kContentMargins.left(), 160);
    QCOMPARE(m_nodeItem->sceneBoundingRect(), expectedRect);
}

void tst_DVNodeGraphicsItem::testResizeOnChildchangeToTop()
{
    m_partitionItem->setRect(QRectF(350, 180, 50, 50)); // content is on the top
    m_nodeItem->childBoundingBoxChanged();
    const QRectF expectedRect(300, 180 - kContentMargins.top(), 160, 160 + 20 + kContentMargins.top());
    QCOMPARE(m_nodeItem->sceneBoundingRect(), expectedRect);
}

void tst_DVNodeGraphicsItem::testResizeOnChildchangeToWidth()
{
    m_partitionItem->setRect(QRectF(350, 250, 500, 50)); // content is wider
    m_nodeItem->childBoundingBoxChanged();
    const QRectF expectedRect(300, 200, 850 + kContentMargins.right() - 300, 160);
    QCOMPARE(m_nodeItem->sceneBoundingRect(), expectedRect);
}

void tst_DVNodeGraphicsItem::testResizeOnChildchangeToHeight()
{
    m_partitionItem->setRect(QRectF(350, 250, 50, 500)); // content is hight
    m_nodeItem->childBoundingBoxChanged();
    const QRectF expectedRect(300, 200, 160, 750 + kContentMargins.bottom() - 200);
    QCOMPARE(m_nodeItem->sceneBoundingRect(), expectedRect);
}

QTEST_MAIN(tst_DVNodeGraphicsItem)

#include "tst_dvnodegraphicsitem.moc"
