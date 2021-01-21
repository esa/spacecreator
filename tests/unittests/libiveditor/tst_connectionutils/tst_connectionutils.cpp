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
#include "aadlobjectiface.h"
#include "baseitems/common/aadlutils.h"
#include "common.h"
#include "connectioncreationvalidator.h"
#include "graphicsviewutils.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"
#include "interface/graphicsitemhelpers.h"

#include <QObject>
#include <QPainter>
#include <QtTest>
#include <array>

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
    void tst_endPoints();
    void tst_path();

private:
    QList<QRectF> existingRects() const
    {
        QList<QRectF> rects;
        for (auto item : m_scene.items()) {
            if (item->type() == aadlinterface::AADLFunctionTypeGraphicsItem::Type
                    || item->type() == aadlinterface::AADLFunctionGraphicsItem::Type) {
                rects.append(item->sceneBoundingRect());
            }
        }
        return rects;
    }

    QGraphicsScene m_scene;
    aadlinterface::AADLFunctionGraphicsItem *f1 { nullptr };
    aadlinterface::AADLFunctionGraphicsItem *f2 { nullptr };
    aadlinterface::AADLFunctionGraphicsItem *nf1 { nullptr };
    aadlinterface::AADLFunctionGraphicsItem *nf2 { nullptr };

    struct Data {
        enum class EndPoint
        {
            Req,
            Prov,
            Empty
        };

        Data(aadlinterface::AADLFunctionGraphicsItem *function)
            : fn(function)
        {
            const QRectF r = rect();
            points = { QLineF(r.topRight(), r.bottomRight()).center(), QLineF(r.topLeft(), r.bottomLeft()).center(),
                QLineF(r.topLeft(), r.topRight()).center(), QLineF(r.bottomLeft(), r.bottomRight()).center() };
        }

        QRectF rect() const { return fn->sceneBoundingRect(); }
        aadlinterface::AADLFunctionGraphicsItem *function() const { return fn; }
        QPointF requiredIfacePoint() const { return points.at(static_cast<int>(EndPoint::Req)); }
        QPointF providedIfacePoint() const { return points.at(static_cast<int>(EndPoint::Prov)); }
        QPointF emptyPoint() const { return points.at(static_cast<int>(EndPoint::Empty)); }
        QPointF point(const Data::EndPoint ep) const { return points.at(static_cast<int>(ep)); }
        QPointF point(const aadl::AADLObjectIface::IfaceType type) const
        {
            switch (type) {
            case aadl::AADLObjectIface::IfaceType::Required:
                return requiredIfacePoint();
            case aadl::AADLObjectIface::IfaceType::Provided:
                return providedIfacePoint();
            default:
                break;
            }
            return {};
        }

    private:
        aadlinterface::AADLFunctionGraphicsItem *fn;
        QList<QPointF> points;
    };

    QVector<Data> data;

private:
    void checkEndPoints(aadlinterface::AADLFunctionGraphicsItem *startFn, Data::EndPoint startEp,
            aadlinterface::AADLFunctionGraphicsItem *endFn, Data::EndPoint endEp, bool isReversed, bool shouldFail);
};

void tst_ConnectionUtils::initTestCase()
{
    auto entity1 = new aadl::AADLObjectFunction("F1");
    f1 = new aadlinterface::AADLFunctionGraphicsItem(entity1);
    m_scene.addItem(f1);
    f1->setRect(QRectF(100, 100, 300, 300));

    auto nestedEntity1 = new aadl::AADLObjectFunction("Nested_F1");
    entity1->addChild(nestedEntity1);
    nf1 = new aadlinterface::AADLFunctionGraphicsItem(nestedEntity1, f1);
    nf1->setRect(QRectF(150, 150, 100, 100));

    auto entity2 = new aadl::AADLObjectFunction("F2");
    f2 = new aadlinterface::AADLFunctionGraphicsItem(entity2);
    m_scene.addItem(f2);
    f2->setRect(QRectF(600, 100, 300, 300));

    auto nestedEntity2 = new aadl::AADLObjectFunction("Nested_F2");
    entity2->addChild(nestedEntity2);
    nf2 = new aadlinterface::AADLFunctionGraphicsItem(nestedEntity2, f2);
    nf2->setRect(QRectF(650, 150, 100, 100));

    data = { Data(f1), Data(nf1), Data(f2), Data(nf2) };
}

void tst_ConnectionUtils::cleanupTestCase()
{
    delete f1->entity();
    delete f2->entity();

    m_scene.clear();

    f1 = nullptr;
    f2 = nullptr;
    nf1 = nullptr;
    nf2 = nullptr;
}

void tst_ConnectionUtils::tst_segmentGenerationByPoints()
{
    QPointF startPoint { 100, 100 };
    QPointF endPoint { startPoint };
    auto segments = aadlinterface::generateSegments(startPoint, endPoint);
    QVERIFY(segments.isEmpty());

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
            existingRects(), QPointF(r1.x(), r1.y() + 100), r1, QPointF(r2.right(), r2.y() + 100), r2);
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
    auto path = aadlinterface::findPath(existingRects(), startSegment, endSegment, &intersectedRect);
    QVERIFY(!intersectedRect.isNull());
    QVERIFY(path.isEmpty());

    intersectedRect = {};
    path = aadlinterface::findPath(existingRects(), QLineF(r1.topLeft(), r1.topRight()),
            QLineF(r2.bottomRight(), r2.bottomLeft()), &intersectedRect);
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
    const QPointF startPoint(r1.x() - 100, r1.center().y());
    const QPointF endPoint(r2.right() + 100, r2.center().y());

    auto path = aadlinterface::path(existingRects(), startPoint, endPoint);
    QVERIFY(!path.isEmpty());
}

void tst_ConnectionUtils::tst_endPoints()
{
    auto addIfaces = [this](const aadl::AADLObjectIface::IfaceType ifaceType) {
        for (int idx = 0; idx < data.size(); ++idx) {
            aadl::AADLObjectIface::CreationInfo ci;
            ci.function = data.at(idx).function()->entity();
            ci.name = ci.function->title();
            ci.position = data.at(idx).point(ifaceType);
            ci.type = ifaceType;
            aadl::AADLObjectIface *iface { nullptr };
            if (ifaceType == aadl::AADLObjectIface::IfaceType::Required) {
                ci.name += QLatin1String("_ReqIface");
                iface = new aadl::AADLObjectIfaceRequired(ci);
            } else if (ifaceType == aadl::AADLObjectIface::IfaceType::Provided) {
                ci.name += QLatin1String("_ProvIface");
                iface = new aadl::AADLObjectIfaceProvided(ci);
            } else {
                qFatal("Test for Interface group isn't implemented yet");
            }
            iface->setCoordinates(aadlinterface::coordinates(ci.position));
            iface->postInit();
            if (ci.function->addChild(iface)) {
                auto ifaceItem = new aadlinterface::AADLInterfaceGraphicsItem(iface, data.at(idx).function());
                ifaceItem->init();
                ifaceItem->setTargetItem(data.at(idx).function(), ci.position);
            }
        }
    };

    addIfaces(aadl::AADLObjectIface::IfaceType::Required);
    addIfaces(aadl::AADLObjectIface::IfaceType::Provided);

    /// Nested_F1-Empty <> F1-Empty
    checkEndPoints(nf1, Data::EndPoint::Empty, f1, Data::EndPoint::Empty, false, false);
    /// F1-Empty <> Nested_F1-Empty
    checkEndPoints(f1, Data::EndPoint::Empty, nf1, Data::EndPoint::Empty, true, false);

    /// F1-Empty <> Nested_F1-Req
    checkEndPoints(f1, Data::EndPoint::Empty, nf1, Data::EndPoint::Req, false, false);
    /// Nested_F1-Req <> F1-Empty
    checkEndPoints(nf1, Data::EndPoint::Req, f1, Data::EndPoint::Empty, false, false);

    /// Nested_F1-Prov <> F1-Empty
    checkEndPoints(nf1, Data::EndPoint::Prov, f1, Data::EndPoint::Empty, true, false);
    /// F1-Empty <> Nested_F1-Prov
    checkEndPoints(f1, Data::EndPoint::Empty, nf1, Data::EndPoint::Prov, true, false);

    /// F1-Req <> Nested_F1-Empty
    checkEndPoints(f1, Data::EndPoint::Req, nf1, Data::EndPoint::Empty, true, false);
    /// Nested_F1-Empty <> F1-Req
    checkEndPoints(nf1, Data::EndPoint::Empty, f1, Data::EndPoint::Req, true, false);

    /// F1-Prov <> Nested_F1-Empty
    checkEndPoints(f1, Data::EndPoint::Prov, nf1, Data::EndPoint::Empty, false, false);
    /// Nested_F1-Empty <> F1-Prov
    checkEndPoints(nf1, Data::EndPoint::Empty, f1, Data::EndPoint::Prov, false, false);

    /// Nested_F1-Req <> F1-Req
    checkEndPoints(nf1, Data::EndPoint::Req, f1, Data::EndPoint::Req, false, false);
    /// F1-Req <> Nested_F1-Req
    checkEndPoints(f1, Data::EndPoint::Req, nf1, Data::EndPoint::Req, true, false);

    /// F1-Req <> Nested_F1-Prov
    checkEndPoints(f1, Data::EndPoint::Req, nf1, Data::EndPoint::Prov, true, true);
    /// Nested_F1-Prov <> F1-Req
    checkEndPoints(nf1, Data::EndPoint::Prov, f1, Data::EndPoint::Req, false, true);

    /// Nested_F1-Prov <> F1-Prov
    checkEndPoints(nf1, Data::EndPoint::Prov, f1, Data::EndPoint::Prov, true, false);
    /// F1-Prov <> Nested_F1-Prov
    checkEndPoints(f1, Data::EndPoint::Prov, nf1, Data::EndPoint::Prov, false, false);

    /// F1-Prov <> Nested_F1-Req
    checkEndPoints(f1, Data::EndPoint::Prov, nf1, Data::EndPoint::Req, false, true);
    /// Nested_F1-Prov <> F1-Req
    checkEndPoints(nf1, Data::EndPoint::Prov, f1, Data::EndPoint::Req, true, true);

    /// F1-Req <> F2-Req
    checkEndPoints(f1, Data::EndPoint::Req, f2, Data::EndPoint::Req, false, true);
    /// F2-Req <> F1-Req
    checkEndPoints(f2, Data::EndPoint::Req, f1, Data::EndPoint::Req, false, true);

    /// F1-Req <> F2-Prov
    checkEndPoints(f1, Data::EndPoint::Req, f2, Data::EndPoint::Prov, false, false);
    /// F2-Prov <> F1-Req
    checkEndPoints(f2, Data::EndPoint::Prov, f1, Data::EndPoint::Req, true, false);

    /// F1-Prov <> F2-Prov
    checkEndPoints(f1, Data::EndPoint::Prov, f2, Data::EndPoint::Prov, true, true);
    /// F2-Prov <> F1-Prov
    checkEndPoints(f2, Data::EndPoint::Prov, f1, Data::EndPoint::Prov, false, true);

    /// F1-Req <> F2-Prov
    checkEndPoints(f1, Data::EndPoint::Req, f2, Data::EndPoint::Prov, false, false);
    /// F2-Prov <> F1-Req
    checkEndPoints(f2, Data::EndPoint::Prov, f1, Data::EndPoint::Req, true, false);

    /// F1-Empty <> F2-Empty
    checkEndPoints(f1, Data::EndPoint::Empty, f2, Data::EndPoint::Empty, false, false);
    /// F2-Empty <> F1-Empty
    checkEndPoints(f2, Data::EndPoint::Empty, f1, Data::EndPoint::Empty, false, false);

    /// F2-Empty <> F1-Req
    checkEndPoints(f2, Data::EndPoint::Empty, f1, Data::EndPoint::Req, true, false);
    /// F1-Req <> F2-Empty
    checkEndPoints(f1, Data::EndPoint::Req, f2, Data::EndPoint::Empty, false, false);

    /// F1-Prov <> F2-Empty
    checkEndPoints(f1, Data::EndPoint::Prov, f2, Data::EndPoint::Empty, true, false);
    /// F2-Empty <> F1-Prov
    checkEndPoints(f2, Data::EndPoint::Empty, f1, Data::EndPoint::Prov, false, false);

    /// F2-Req <> F1-Empty
    checkEndPoints(f2, Data::EndPoint::Req, f1, Data::EndPoint::Empty, false, false);
    /// F1-Empty <> F2-Req
    checkEndPoints(f1, Data::EndPoint::Empty, f2, Data::EndPoint::Req, true, false);

    /// F2-Prov <> F1-Empty
    checkEndPoints(f2, Data::EndPoint::Prov, f1, Data::EndPoint::Empty, true, false);
    /// F1-Empty <> F2-Prov
    checkEndPoints(f1, Data::EndPoint::Empty, f2, Data::EndPoint::Prov, false, false);
}

void tst_ConnectionUtils::tst_path()
{
    const QRectF r1 = f1->sceneBoundingRect();
    const QRectF r2 = f2->sceneBoundingRect();
    const QPointF startPoint(r1.x() - 100, r1.center().y());
    const QPointF endPoint(r2.right() + 100, r2.center().y());

    const QLineF startSegment = aadlinterface::ifaceSegment(r1, startPoint, endPoint);
    const QLineF endSegment = aadlinterface::ifaceSegment(r2, endPoint, startPoint);

    auto path = aadlinterface::path(existingRects(), startSegment, endSegment);
    QVERIFY(!path.isEmpty());
}

void tst_ConnectionUtils::checkEndPoints(aadlinterface::AADLFunctionGraphicsItem *startFn, Data::EndPoint startEp,
        aadlinterface::AADLFunctionGraphicsItem *endFn, Data::EndPoint endEp, bool isReversed, bool shouldFail)
{
    const Data start(startFn);
    const Data end(endFn);

    static const QVector<QPointF> offsets { QPointF(0, 0), QPointF(-2, 0), QPointF(0, -2), QPointF(2, 0),
        QPointF(0, 2) };

    for (int startIdx = 0; startIdx < offsets.size(); ++startIdx) {
        for (int endIdx = 0; endIdx < offsets.size(); ++endIdx) {
            const QVector<QPointF> connectionPoints { start.point(startEp) + offsets[startIdx],
                end.point(endEp) + offsets[endIdx] };

            const QPointF p = isReversed ? connectionPoints.last() : connectionPoints.first();
            if (startEp == Data::EndPoint::Empty) {
                const bool validPoint = aadlinterface::isOnVerticalSide(start.rect(), p)
                        || aadlinterface::isOnVerticalSide(start.rect(), p);
                if (!validPoint) {
                    continue;
                }
            }
            if (endEp == Data::EndPoint::Empty) {
                const bool validPoint = aadlinterface::isOnVerticalSide(end.rect(), p)
                        || aadlinterface::isOnVerticalSide(end.rect(), p);
                if (!validPoint) {
                    continue;
                }
            }

            const auto result = aadlinterface::gi::validateConnectionCreate(&m_scene, connectionPoints);
            QVERIFY(result.startIface != result.endIface
                    || (result.startIface == nullptr && result.endIface == nullptr));
            QCOMPARE(result.isToOrFromNested,
                    (start.function()->isAncestorOf(end.function()) || end.function()->isAncestorOf(start.function())));
            QCOMPARE(result.failed(), shouldFail);
            if (!result.failed()) {
                if (isReversed) {
                    QVERIFY(result.connectionPoints.first() == connectionPoints.last()
                            && result.connectionPoints.last() == connectionPoints.first());
                } else {
                    QCOMPARE(result.connectionPoints, connectionPoints);
                }

                const auto path = aadlinterface::createConnectionPath(existingRects(),
                        isReversed ? result.connectionPoints.last() : result.connectionPoints.first(), start.rect(),
                        isReversed ? result.connectionPoints.first() : result.connectionPoints.last(), end.rect());
                QVERIFY(!path.isEmpty());
            }
        }
    }
}

QTEST_MAIN(tst_ConnectionUtils)

#include "tst_connectionutils.moc"
