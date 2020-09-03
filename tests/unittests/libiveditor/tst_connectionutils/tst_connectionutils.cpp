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

#include "aadlobjectfunction.h"
#include "baseitems/common/aadlutils.h"
#include "graphicsviewutils.h"
#include "interface/aadlfunctiongraphicsitem.h"

#include <QObject>
#include <QtTest>

class tst_ConnectionUtils : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void tst_segmentGenerationByPoints();
    void tst_segmentGeneration();
    void tst_ifaceSegment();
    void tst_createConnectionPath();
    void tst_findPath();
    void tst_findSubPath();
    void tst_pathByPoints();
    void tst_path();

private:
    QGraphicsScene m_scene;
    aadlinterface::AADLFunctionGraphicsItem *f1 { nullptr };
    aadlinterface::AADLFunctionGraphicsItem *f2 { nullptr };
};

void tst_ConnectionUtils::initTestCase()
{
    auto entity1 = new aadl::AADLObjectFunction("F1");
    f1 = new aadlinterface::AADLFunctionGraphicsItem(entity1);
    m_scene.addItem(f1);
    f1->setRect(QRectF(100, 100, 300, 300));

    auto entity2 = new aadl::AADLObjectFunction("F2");
    f2 = new aadlinterface::AADLFunctionGraphicsItem(entity2);
    m_scene.addItem(f2);
    f2->setRect(QRectF(600, 100, 300, 300));
}

void tst_ConnectionUtils::cleanupTestCase()
{
    m_scene.clear();
    f1 = nullptr;
    f2 = nullptr;
}

void tst_ConnectionUtils::tst_segmentGenerationByPoints()
{
    QPointF startPoint { 100, 100 };
    QPointF endPoint { startPoint };
    auto segments = aadlinterface::generateSegments(startPoint, endPoint);

    endPoint += QPointF(200, 200);
    segments = aadlinterface::generateSegments(startPoint, endPoint);
    QVERIFY(!segments.isEmpty());
    for (auto segment : segments) {
        QVERIFY(segment.startsWith(startPoint));
        QVERIFY(segment.endsWith(endPoint));
    }
}

void tst_ConnectionUtils::tst_segmentGeneration()
{
    auto segments = aadlinterface::generateSegments(QLineF(), QLineF());
    QVERIFY(segments.isEmpty());
    QLineF startSegment { QPointF(100, 100), QPointF(100, 200) };
    segments = aadlinterface::generateSegments(startSegment, QLineF());
    QVERIFY(segments.isEmpty());
    QLineF endSegment { QPointF(300, 200), QPointF(300, 100) };
    segments = aadlinterface::generateSegments(QLineF(), endSegment);
    QVERIFY(segments.isEmpty());

    // ||
    segments = aadlinterface::generateSegments(startSegment, endSegment);
    QVERIFY(segments.size() >= 3);
    QVERIFY(segments.first() == startSegment.p2());
    QVERIFY(segments.last() == endSegment.p2());

    // |_
    endSegment.setP2(QPointF(200, 200));
    segments = aadlinterface::generateSegments(startSegment, endSegment);
    QVERIFY(segments.size() >= 3);
    QVERIFY(segments.first() == startSegment.p2());
    QVERIFY(segments.last() == endSegment.p2());

    // \_
    startSegment.setP1(QPointF(0, 0));
    segments = aadlinterface::generateSegments(startSegment, endSegment);
    QVERIFY(segments.size() >= 3);
    QVERIFY(segments.first() == startSegment.p2());
    QVERIFY(segments.last() == endSegment.p2());

    // \|
    endSegment.setP1(QPointF(200, 100));
    segments = aadlinterface::generateSegments(startSegment, endSegment);
    QVERIFY(segments.size() >= 3);
    QVERIFY(segments.first() == startSegment.p2());
    QVERIFY(segments.last() == endSegment.p2());

    // /|
    startSegment.setP1(QPointF(100, 100));
    segments = aadlinterface::generateSegments(startSegment, endSegment);
    QVERIFY(segments.size() >= 3);
    QVERIFY(segments.first() == startSegment.p2());
    QVERIFY(segments.last() == endSegment.p2());

    // /_
    endSegment.setP1(QPointF(300, 200));
    segments = aadlinterface::generateSegments(startSegment, endSegment);
    QVERIFY(segments.size() >= 3);
    QVERIFY(segments.first() == startSegment.p2());
    QVERIFY(segments.last() == endSegment.p2());
}

void tst_ConnectionUtils::tst_ifaceSegment()
{
    auto line = aadlinterface::ifaceSegment(QRectF(), QPointF(), QPointF());
    QVERIFY(line.isNull());

    QRectF rect { 100, 100, 300, 300 };
    QPointF first, last;
    line = aadlinterface::ifaceSegment(rect, first, last);

    first = QPointF(200, 100);
    Q_ASSERT(rect.contains(first));

    last = QPointF(200, 0);
    line = aadlinterface::ifaceSegment(rect, first, last);
    QVERIFY(!line.isNull());
    QVERIFY(line.p1() != line.p2());
    QVERIFY(rect.contains(last) == rect.contains(line.p2()));

    last = QPointF(200, 200);
    line = aadlinterface::ifaceSegment(rect, first, last);
    QVERIFY(!line.isNull());
    QVERIFY(line.p1() != line.p2());
    QVERIFY(rect.contains(last) == rect.contains(line.p2()));

    first = QPointF(400, 200);
    Q_ASSERT(rect.contains(first));

    last = QPointF(200, 200);
    line = aadlinterface::ifaceSegment(rect, first, last);
    QVERIFY(!line.isNull());
    QVERIFY(line.p1() != line.p2());
    QVERIFY(rect.contains(last) == rect.contains(line.p2()));

    last = QPointF(600, 200);
    line = aadlinterface::ifaceSegment(rect, first, last);
    QVERIFY(!line.isNull());
    QVERIFY(line.p1() != line.p2());
    QVERIFY(rect.contains(last) == rect.contains(line.p2()));

    first = QPointF(200, 400);
    Q_ASSERT(rect.contains(first));

    last = QPointF(200, 600);
    line = aadlinterface::ifaceSegment(rect, first, last);
    QVERIFY(!line.isNull());
    QVERIFY(line.p1() != line.p2());
    QVERIFY(rect.contains(last) == rect.contains(line.p2()));

    last = QPointF(200, 200);
    line = aadlinterface::ifaceSegment(rect, first, last);
    QVERIFY(!line.isNull());
    QVERIFY(line.p1() != line.p2());
    QVERIFY(rect.contains(last) == rect.contains(line.p2()));

    first = QPointF(100, 300);
    Q_ASSERT(rect.contains(first));

    last = QPointF(0, 300);
    line = aadlinterface::ifaceSegment(rect, first, last);
    QVERIFY(!line.isNull());
    QVERIFY(line.p1() != line.p2());
    QVERIFY(rect.contains(last) == rect.contains(line.p2()));

    last = QPointF(200, 200);
    line = aadlinterface::ifaceSegment(rect, first, last);
    QVERIFY(!line.isNull());
    QVERIFY(line.p1() != line.p2());
    QVERIFY(rect.contains(last) == rect.contains(line.p2()));
}

void tst_ConnectionUtils::tst_createConnectionPath()
{
    const QRectF r1 = f1->sceneBoundingRect();
    const QRectF r2 = f2->sceneBoundingRect();
    QVector<QPointF> path = aadlinterface::createConnectionPath(
            &m_scene, QPointF(r1.x(), r1.y() + 100), r1, QPointF(r2.right(), r2.y() + 100), r2);
    QVERIFY(!path.isEmpty());
    path.append(path.last());
    path.append(path.last());
    path.prepend(path.first());
    path.prepend(path.first());
    auto simplifiedPath = aadlinterface::simplifyPoints(path);
    QCOMPARE(simplifiedPath.size() + 4, path.size());
}

void tst_ConnectionUtils::tst_findPath()
{
    const QRectF r1 = f1->sceneBoundingRect();
    const QRectF r2 = f2->sceneBoundingRect();
    const QPointF startPoint(r1.x(), r1.y() + 100);
    const QPointF endPoint(r2.right(), r2.y() + 100);

    const QLineF startSegment = aadlinterface::ifaceSegment(r1, startPoint, endPoint);
    const QLineF endSegment = aadlinterface::ifaceSegment(r2, endPoint, startPoint);

    QRectF intersectedRect;
    auto path = aadlinterface::findPath(&m_scene, startSegment, endSegment, &intersectedRect);
    QVERIFY(!intersectedRect.isNull());
    QVERIFY(path.isEmpty());

    intersectedRect = {};
    path = aadlinterface::findPath(
            &m_scene, QLineF(r1.topLeft(), r1.topRight()), QLineF(r2.bottomRight(), r2.bottomLeft()), &intersectedRect);
    QVERIFY(intersectedRect.isNull());
    QVERIFY(!path.isEmpty());
}

void tst_ConnectionUtils::tst_findSubPath()
{
    const QRectF r1 = f1->sceneBoundingRect();
    const QRectF r2 = f2->sceneBoundingRect();
    const QPointF startPoint(r1.x(), r1.y() + 100);
    const QPointF endPoint(r2.right(), r2.y() + 100);

    const QLineF startSegment = aadlinterface::ifaceSegment(r1, startPoint, endPoint);
    const QLineF endSegment = aadlinterface::ifaceSegment(r2, endPoint, startPoint);

    const auto pathsFromStart =
            aadlinterface::findSubPath(r1, QVector<QPointF> { startSegment.p1(), startSegment.p2() },
                    QVector<QPointF> { endSegment.p1(), endSegment.p2() });
    QVERIFY(!pathsFromStart.isEmpty());

    const auto pathsFromEnd = aadlinterface::findSubPath(r2, QVector<QPointF> { endSegment.p1(), endSegment.p2() },
            QVector<QPointF> { startSegment.p1(), startSegment.p2() });
    QVERIFY(!pathsFromEnd.isEmpty());

    static const QList<int> types = { aadlinterface::AADLFunctionGraphicsItem::Type,
        aadlinterface::AADLFunctionTypeGraphicsItem::Type };
    QList<QVector<QPointF>> paths;
    for (int p1idx = 0; p1idx < pathsFromStart.size(); ++p1idx) {
        for (int p2idx = 0; p2idx < pathsFromEnd.size(); ++p2idx) {
            const QVector<QPointF> checkPath = { pathsFromStart.at(p1idx).last(), pathsFromEnd.at(p2idx).last() };
            auto items = m_scene.items(checkPath);
            auto it = std::find_if(items.constBegin(), items.constEnd(),
                    [](QGraphicsItem *item) { return types.contains(item->type()); });

            if (it == items.constEnd()) {
                QVector<QPointF> path { pathsFromStart.at(p1idx) };
                QVector<QPointF> last { pathsFromEnd.at(p2idx) };
                std::copy(std::crbegin(last), std::crend(last), std::back_inserter(path));
                paths.append(path);
            }
        }
    }
    for (auto path : paths) {
        path = aadlinterface::simplifyPoints(path);
        auto items = m_scene.items(path);
        auto it = std::find_if(items.constBegin(), items.constEnd(), [path](QGraphicsItem *item) {
            return types.contains(item->type())
                    && shared::graphicsviewutils::intersectionPoints(item->sceneBoundingRect(), path).size() >= 2;
        });
        QVERIFY(it == items.constEnd());
    }
}

void tst_ConnectionUtils::tst_pathByPoints()
{
    const QRectF r1 = f1->sceneBoundingRect();
    const QRectF r2 = f2->sceneBoundingRect();
    const QPointF startPoint(r1.x(), r1.y() + 100);
    const QPointF endPoint(r2.right(), r2.y() + 100);

    auto path = aadlinterface::path(&m_scene, startPoint, endPoint);
    QVERIFY(!path.isEmpty());
}

void tst_ConnectionUtils::tst_path()
{
    const QRectF r1 = f1->sceneBoundingRect();
    const QRectF r2 = f2->sceneBoundingRect();
    const QPointF startPoint(r1.x(), r1.y() + 100);
    const QPointF endPoint(r2.right(), r2.y() + 100);

    const QLineF startSegment = aadlinterface::ifaceSegment(r1, startPoint, endPoint);
    const QLineF endSegment = aadlinterface::ifaceSegment(r2, endPoint, startPoint);

    auto path = aadlinterface::path(&m_scene, startSegment, endSegment);
    QVERIFY(!path.isEmpty());
}

QTEST_MAIN(tst_ConnectionUtils)

#include "tst_connectionutils.moc"
