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

#include "baseitems/common/utils.h"
#include "baseitems/grippoint.h"
#include "baseitems/grippointshandler.h"
#include "baseitems/interactiveobject.h"
#include "tst_common.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointer>
#include <QTest>

using namespace msc;

class InteractiveObjectImpl : public InteractiveObject
{
    Q_OBJECT
public:
    InteractiveObjectImpl(QGraphicsItem *parent = nullptr)
        : InteractiveObject(nullptr, parent)
    {
        prepareGeometryChange();
        m_boundingRect = QRectF(-50., -50., 100., 100.);
        updateGripPoints();
    }

    GripPoint *gripPoint(GripPoint::Location location)
    {
        if (!m_gripPoints)
            return nullptr;

        return m_gripPoints->usedPoints().contains(location) ? m_gripPoints->gripPoint(location) : nullptr;
    }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->drawRect(boundingRect());
    }

    virtual void onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override
    {
        Q_UNUSED(gp);

        const QPointF &offset(to - from);
        moveBy(offset.x(), offset.y());
    }

    virtual void onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to) override
    {
        // Note: the correctness of resizing itself does not matter now,
        // we are here just to ensure that the handler is called in a propper way.

        const QPointF &offset(to - from);
        switch (gp->location()) {
        case GripPoint::Location::Top:
            m_boundingRect.setTop(m_boundingRect.top() + offset.y());
            break;
        case GripPoint::Location::Bottom:
            m_boundingRect.setBottom(m_boundingRect.bottom() - offset.y());
            break;
        case GripPoint::Location::Left:
            m_boundingRect.setLeft(m_boundingRect.left() + offset.x());
            break;
        case GripPoint::Location::Right:
            m_boundingRect.setRight(m_boundingRect.right() - offset.x());
            break;
        case GripPoint::Location::TopLeft:
            m_boundingRect.setTopLeft(m_boundingRect.topLeft() + offset);
            break;
        case GripPoint::Location::BottomLeft:
            m_boundingRect.setBottomLeft(m_boundingRect.bottomLeft() - offset);
            break;
        case GripPoint::Location::TopRight:
            m_boundingRect.setTopRight(m_boundingRect.topRight() + offset);
            break;
        case GripPoint::Location::BottomRight:
            m_boundingRect.setBottomRight(m_boundingRect.bottomRight() - offset);
            break;
        default:
            return;
        }
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
    static constexpr QPoint MoveDistance = { 10, 10 };

    static constexpr bool IsLocalBuild = false; // show the view and process events after each action to see the result
};

constexpr QPoint tst_InteractiveObject::MoveDistance;

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
}

void tst_InteractiveObject::cleanup()
{
    if (!m_scene || !m_item)
        return;

    m_scene->removeItem(m_item);
    delete m_item;
}

void tst_InteractiveObject::testMove()
{
    QVERIFY(m_view);
    QVERIFY(m_view->viewport());
    QVERIFY(m_item);

    const QPointF &prevPos(m_item->pos());
    QVERIFY(prevPos.isNull());

    msc::test::ui::sendMouseMove(m_view->viewport(),
                                 m_view->mapFromScene(m_item->boundingRect().translated(m_item->pos()).center()));

    if (GripPoint *gp = m_item->gripPoint(GripPoint::Center)) {
        const QPointF &gpScene = gp->mapToScene(gp->boundingRect().center());
        const QPoint &pntFrom = m_view->mapFromScene(gpScene);
        const QPoint &pntTo(pntFrom + MoveDistance);

        msc::test::ui::sendMouseMove(m_view->viewport(), pntFrom, Qt::NoButton, Qt::NoButton);
        msc::test::ui::sendMousePress(m_view->viewport(), pntFrom, Qt::LeftButton);

        msc::test::ui::sendMouseMove(m_view->viewport(), pntTo, Qt::LeftButton, Qt::LeftButton);
        msc::test::ui::sendMouseRelease(m_view->viewport(), pntTo, Qt::LeftButton);
    }

    QCOMPARE(m_item->pos(), prevPos + MoveDistance);
}

void tst_InteractiveObject::testResize()
{
    QVERIFY(m_view);
    QVERIFY(m_view->viewport());
    QVERIFY(m_item);

    // place mouse pointer over item to activate its grippointshandler:
    msc::test::ui::sendMouseMove(m_view->viewport(), QPoint(), Qt::NoButton, Qt::NoButton);

    const QMetaEnum &e = QMetaEnum::fromType<msc::GripPoint::Location>();
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

            msc::test::ui::sendMouseMove(m_view->viewport(), pntFrom, Qt::NoButton, Qt::NoButton);
            msc::test::ui::sendMousePress(m_view->viewport(), pntFrom, Qt::LeftButton);

            msc::test::ui::sendMouseMove(m_view->viewport(), pntTo, Qt::LeftButton, Qt::LeftButton);
            msc::test::ui::sendMouseRelease(m_view->viewport(), pntTo, Qt::LeftButton);

            QVERIFY(m_item->boundingRect() != prevRect);
        }
    }
}

QTEST_MAIN(tst_InteractiveObject)

#include "tst_interactiveobject.moc"
