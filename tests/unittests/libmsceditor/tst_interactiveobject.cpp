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
#include "baseitems/interactiveobject.h"
#include "syntheticinteraction.h"
#include "ui/grippointshandler.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointer>
#include <QTest>

using namespace msc;
using namespace shared::ui;

class InteractiveObjectImpl : public InteractiveObject
{
    Q_OBJECT
public:
    InteractiveObjectImpl(QGraphicsItem *parent = nullptr)
        : InteractiveObject(nullptr, nullptr, parent)
    {
        prepareGeometryChange();
        setBoundingRect(QRectF(-50., -50., 100., 100.));
        initGripPoints();
    }

    GripPoint *gripPoint(GripPoint::Location location)
    {
        if (auto gph = gripPointsHandler()) {
            return gph->gripPoint(location);
        }
        return nullptr;
    }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->drawRect(boundingRect());
    }

    void initGripPoints() override
    {
        InteractiveObject::initGripPoints();
        gripPointsHandler()->setUsedPoints({ GripPoint::Location::Center });
    }

    virtual void onManualMoveProgress(GripPoint *gp, const QPointF &from, const QPointF &to) override
    {
        Q_UNUSED(gp);

        const QPointF &offset(to - from);
        moveBy(offset.x(), offset.y());
    }

    virtual void onManualResizeProgress(GripPoint *gp, const QPointF &from, const QPointF &to) override
    {
        // Note: the correctness of resizing itself does not matter now,
        // we are here just to ensure that the handler is called in a propper way.

        const QPointF &offset(to - from);
        auto br = boundingRect();
        switch (gp->location()) {
        case GripPoint::Location::Top:
            br.setTop(boundingRect().top() + offset.y());
            break;
        case GripPoint::Location::Bottom:
            br.setBottom(boundingRect().bottom() - offset.y());
            break;
        case GripPoint::Location::Left:
            br.setLeft(boundingRect().left() + offset.x());
            break;
        case GripPoint::Location::Right:
            br.setRight(boundingRect().right() - offset.x());
            break;
        case GripPoint::Location::TopLeft:
            br.setTopLeft(boundingRect().topLeft() + offset);
            break;
        case GripPoint::Location::BottomLeft:
            br.setBottomLeft(boundingRect().bottomLeft() - offset);
            break;
        case GripPoint::Location::TopRight:
            br.setTopRight(boundingRect().topRight() + offset);
            break;
        case GripPoint::Location::BottomRight:
            br.setBottomRight(boundingRect().bottomRight() - offset);
            break;
        default:
            return;
        }
        setBoundingRect(br);
        prepareGeometryChange();
        updateGripPoints();
    }
};

class tst_InteractiveObject : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testMove();
    void testResize();

private:
    QPointer<QGraphicsScene> m_scene = nullptr;
    QPointer<QGraphicsView> m_view = nullptr;
    QPointer<InteractiveObjectImpl> m_item = nullptr;

    // EventsDelayMs - In my local environment 20ms is enough, but I'm not sure
    // about CI, so here is some extra time span.
    // The difference in total test duration (20ms per event vs 100ms) is about 3s.
};

static const QPoint MoveDistance = { 10, 10 };

static const bool IsLocalBuild = false; // show the view and process events after each action to see the result

void tst_InteractiveObject::initTestCase()
{
    m_scene = QPointer<QGraphicsScene>(new QGraphicsScene());
    m_scene->setSceneRect(-50., -50., 150, 150);
    m_view = QPointer<QGraphicsView>(new QGraphicsView());
    m_view->setScene(m_scene);
    m_view->resize(m_scene->sceneRect().size().toSize() * 2);

    // This could be usefull during local development,
    // but fails the test in CI environment:
    if (IsLocalBuild)
        m_view->show();
}

void tst_InteractiveObject::cleanupTestCase()
{
    delete m_view;
    delete m_scene;
}

void tst_InteractiveObject::init()
{
    if (!m_scene)
        return;

    m_item = new InteractiveObjectImpl();
    m_scene->addItem(m_item);

    vstest::saveMousePosition();
}

void tst_InteractiveObject::cleanup()
{
    if (!m_scene || !m_item)
        return;

    m_scene->removeItem(m_item);
    delete m_item;

    vstest::restoreMousePosition();
}

void tst_InteractiveObject::testMove()
{
    QVERIFY(m_view);
    QVERIFY(m_view->viewport());
    QVERIFY(m_item);

    const QPointF &prevPos(m_item->pos());
    QVERIFY(prevPos.isNull());

    vstest::sendMouseMove(
            m_view->viewport(), m_view->mapFromScene(m_item->boundingRect().translated(m_item->pos()).center()));

    if (GripPoint *gp = m_item->gripPoint(GripPoint::Center)) {
        const QPointF &gpScene = gp->mapToScene(gp->boundingRect().center());
        const QPoint &pntFrom = m_view->mapFromScene(gpScene);
        const QPoint &pntTo(pntFrom + MoveDistance);

        vstest::sendMouseDrag(m_view->viewport(), pntFrom, pntTo);
    }

    QCOMPARE(m_item->pos(), prevPos + MoveDistance);
}

void tst_InteractiveObject::testResize()
{
    QVERIFY(m_view);
    QVERIFY(m_view->viewport());
    QVERIFY(m_item);

    // place mouse pointer over item to activate its grippointshandler:
    vstest::sendMouseMove(m_view->viewport(), QPoint(), Qt::NoButton, Qt::NoButton);

    const QMetaEnum &e = QMetaEnum::fromType<GripPoint::Location>();
    for (int i = 0; i < e.keyCount(); ++i) {
        const GripPoint::Location loc = static_cast<GripPoint::Location>(e.value(i));

        if (loc == GripPoint::Location::Center)
            continue; // covered in tst_InteractiveObject::testMove()

        if (GripPoint *gp = m_item->gripPoint(loc)) {
            const QRectF &prevRect(m_item->boundingRect());

            const QPointF &gpScene = gp->mapToScene(gp->boundingRect().center());
            const QPoint &gpView = m_view->mapFromScene(gpScene);
            const QPoint &pntFrom = m_view->viewport()->mapFromGlobal(m_view->mapToGlobal(gpView));
            const QPoint &pntTo(pntFrom + MoveDistance);

            vstest::sendMouseDrag(m_view->viewport(), pntFrom, pntTo);
            QVERIFY(m_item->boundingRect() != prevRect);
        }
    }
}

QTEST_MAIN(tst_InteractiveObject)

#include "tst_interactiveobject.moc"
