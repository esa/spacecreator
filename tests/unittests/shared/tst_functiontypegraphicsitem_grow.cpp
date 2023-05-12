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

#include "itemeditor/ivfunctiontypegraphicsitem.h"
#include "ivfunctiontype.h"
#include "ui/textitem.h"

#include <QtTest>
#include <QGraphicsScene>
#include <QFontDatabase>

using namespace shared::ui;
using namespace ive;

/*
 * The class under test is the 'IVFunctionTypeGraphicsItem'. FunctionTypeGraphicsItem provides a concrete child
 * class that provides public member function 'callResizedRect' to call VERectGraphicsItem's protected member function
 * 'resizedRect'.
 */
class FunctionTypeGraphicsItem : public IVFunctionTypeGraphicsItem
{
public:
    FunctionTypeGraphicsItem(ivm::IVFunctionType *entity, QGraphicsItem *parent = nullptr)
        : ive::IVFunctionTypeGraphicsItem(entity, parent)
    {
        QDirIterator dirIt(":/fonts");
        while (dirIt.hasNext()) {
            QFontDatabase::addApplicationFont(dirIt.next());
        }
        setFont(QFont(QLatin1String("Ubuntu"), 8));
        m_textItem = initTextItem(); // Sets font on the textItem
        m_textItem->setObjectName("textItem");
    }

    QString getText() { return m_textItem->toPlainText(); }

    // Public access to resizedRect
    virtual QRectF callResizedRect(ResizeLimits resizeLimits)
    {
        return IVFunctionTypeGraphicsItem::resizedRect(resizeLimits);
    }
};

/**
 * VEObjectImpl is a mock of the abstract VEObject class, needed by VERectGraphicsItem because it contains the text
 * that is displayed on the VERectGraphicsItemImpl
 */
class FunctionTypeMock : public ivm::IVFunctionType
{
public:
    virtual void setTitleUI(QString titleUI) { m_mockTitleUI = titleUI; }
    virtual QString titleUI() const override { return m_mockTitleUI; }

private:
    QString m_mockTitleUI;
};

// Test class with some fixture
class tst_FunctionTypeGraphicsItem : public QObject
{
    Q_OBJECT

public:
    tst_FunctionTypeGraphicsItem()
        : m_scene(QGraphicsScene(0, 0, 800, 600))
        , m_rectangularGraphicsItem(nullptr)
    {

    }

private:
    QGraphicsScene m_scene;
    FunctionTypeGraphicsItem *m_rectangularGraphicsItem;

    void addOtherFunctionType(qreal x1, qreal y1, qreal x2, qreal y2)
    {
        auto *entity = new FunctionTypeMock();
        entity->setTitleUI("i");
        FunctionTypeGraphicsItem *other = new FunctionTypeGraphicsItem(entity);
        m_scene.addItem(other);
        other->setBoundingRect(QRectF(QPointF(x1, y1), QPointF(x2, y2)));
    }

private Q_SLOTS:

    void init()
    {
        m_scene.clear(); // Deletes the graphicsitems too
        auto *entity = new FunctionTypeMock();
        entity->setTitleUI("i");
        m_rectangularGraphicsItem = new FunctionTypeGraphicsItem(entity);
        m_scene.addItem(m_rectangularGraphicsItem);
        m_rectangularGraphicsItem->setBoundingRect(QRectF(QPointF(100, 100), QPointF(200, 200)));
        m_rectangularGraphicsItem->updateText();
    }

    void cleanup() { m_scene.clear(); }

    void testMovingTopLeftGripToRightLimit();
    void testMovingTopLeftGripToBottomLimit();
    void testMovingTopLeftGripToBottomRightCorner();

    void testMovingRightGripTooFarRight();
    void testMovingLeftGripTooFarLeft();
    void testMovingTopGripTooFarUp();
    void testMovingBottomGripTooFarDown();

    void testTopLeftGripPointMovingUpAndLeftIntersectingOtherRectThroughRightEdge();
    void testTopLeftGripPointMovingUpAndLeftIntersectingOtherRectThroughBottomRightCorner();
    void testTopLeftGripPointMovingUpAndLeftIntersectingOtherRectThroughBottomEdge();

    void testTopRightGripPointMovingUpAndRightIntersectingOtherRectThroughLeftEdge();
    void testTopRightGripPointMovingUpAndRightIntersectingOtherRectThroughBottomLeftCorner();
    void testTopRightGripPointMovingUpAndRightIntersectingOtherRectThroughBottomEdge();

    void testBottomLeftGripPointMovingDownAndLeftIntersectingOtherRectThroughRightEdge();
    void testBottomLeftGripPointMovingDownAndLeftIntersectingOtherRectThroughTopRightCorner();
    void testBottomLeftGripPointMovinDownAndLeftIntersectingOtherRectThroughTopEdge();

    void testBottomRightGripPointMovingDownAndRightIntersectingOtherRectThroughLeftEdge();
    void testBottomRightGripPointMovingDownAndRightIntersectingOtherRectThroughTopLeftCorner();
    void testBottomRightGripPointMovinDownAndRightIntersectingOtherRectThroughTopEdge();
};


void tst_FunctionTypeGraphicsItem::testMovingTopLeftGripToRightLimit()
{
    // Setup
    QRectF rect = QRectF(QPointF(201, 140), QPointF(240, 180));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(201, 140);
    QPointF to(200, 140); // Move the Top-Left grip-point
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(100, 100, 200, 200);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(222, 140), QPointF(240, 180)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testMovingTopLeftGripToBottomLimit()
{
    // Setup
    QRectF rect = QRectF(QPointF(140, 201), QPointF(180, 240));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(140, 201);
    QPointF to(140, 200); // Move the Top-Left grip-point
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(100, 100, 200, 200);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(140, 222), QPointF(180, 240)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testMovingTopLeftGripToBottomRightCorner()
{
    // Setup
    QRectF rect = QRectF(QPointF(201, 201), QPointF(240, 240));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(201, 201);
    QPointF to(200, 200); // Move the Top-Left grip-point
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(100, 100, 200, 200);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(200, 200), QPointF(240, 240)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testMovingRightGripTooFarRight()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::Right);
    QPointF from(200, 150);
    QPointF to(200 + 30, 150); // Move the RIGH grip-point 30 to the RIGHT, which will collide with OtherFunctionType
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(220, 120, 280, 180);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(100, 100), QPointF(198, 200)));
    QCOMPARE(actualRect, expectedRect);
}


void tst_FunctionTypeGraphicsItem::testMovingLeftGripTooFarLeft()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::Left);
    QPointF from(100, 150);
    QPointF to(100 - 30, 150); // Move the LEFT grip-point 30 to the LEFT, which will collide with OtherFunctionType
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(20, 120, 80, 180);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(102, 100), QPointF(200, 200)));
    QCOMPARE(actualRect, expectedRect);
}


void tst_FunctionTypeGraphicsItem::testMovingTopGripTooFarUp()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::Top);
    QPointF from(150, 100);
    QPointF to(150, 100 - 30); // Move the TOP grip-point 30 UP, which will collide with OtherFunctionType
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType( 120, 20, 180, 80);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(100, 102), QPointF(200, 200)));
    QCOMPARE(actualRect, expectedRect);
}


void tst_FunctionTypeGraphicsItem::testMovingBottomGripTooFarDown()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::Bottom);
    QPointF from(150, 200);
    QPointF to(150, 200 + 30); // Move the bottom grip-point 30 DOWN, which will collide with OtherFunctionType
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(120, 220, 180, 280);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(100, 100), QPointF(200, 198)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testTopLeftGripPointMovingUpAndLeftIntersectingOtherRectThroughRightEdge()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(100, 100);
    QPointF to(70, 40);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(20, 20, 80, 80);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(80, 40), QPointF(200, 200)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testTopLeftGripPointMovingUpAndLeftIntersectingOtherRectThroughBottomRightCorner()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(100, 100);
    QPointF to(60, 60);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(20, 20, 80, 80);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(80, 80), QPointF(200, 200)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testTopLeftGripPointMovingUpAndLeftIntersectingOtherRectThroughBottomEdge()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(100, 100);
    QPointF to(50, 70);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(20, 20, 80, 80);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(50, 80), QPointF(200, 200)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testTopRightGripPointMovingUpAndRightIntersectingOtherRectThroughLeftEdge()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(200, 100);
    QPointF to(230, 40);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(220, 20, 280, 80);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(100, 40), QPointF(220, 200)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testTopRightGripPointMovingUpAndRightIntersectingOtherRectThroughBottomLeftCorner()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(200, 100);
    QPointF to(240, 60);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(220, 20, 280, 80);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(100, 80), QPointF(220, 200)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testTopRightGripPointMovingUpAndRightIntersectingOtherRectThroughBottomEdge()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(200, 100);
    QPointF to(260, 70);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(220, 20, 280, 80);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(100, 80), QPointF(260, 200)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testBottomLeftGripPointMovingDownAndLeftIntersectingOtherRectThroughRightEdge()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(100, 200);
    QPointF to(70, 260);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(20, 220, 80, 280);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(80, 100), QPointF(200, 260)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testBottomLeftGripPointMovingDownAndLeftIntersectingOtherRectThroughTopRightCorner()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(100, 200);
    QPointF to(60, 240);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(20, 220, 80, 280);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(80, 100), QPointF(200, 220)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testBottomLeftGripPointMovinDownAndLeftIntersectingOtherRectThroughTopEdge()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(100, 200);
    QPointF to(40, 230);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(20, 220, 80, 280);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(40, 100), QPointF(200, 220)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testBottomRightGripPointMovingDownAndRightIntersectingOtherRectThroughLeftEdge()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(200, 200);
    QPointF to(220, 260);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(220, 220, 280, 280);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(100, 100), QPointF(220, 260)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testBottomRightGripPointMovingDownAndRightIntersectingOtherRectThroughTopLeftCorner()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(200, 200);
    QPointF to(240, 240);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(220, 220, 280, 280);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(100, 100), QPointF(220, 220)));
    QCOMPARE(actualRect, expectedRect);
}

void tst_FunctionTypeGraphicsItem::testBottomRightGripPointMovinDownAndRightIntersectingOtherRectThroughTopEdge()
{
    // Setup
    QRectF rect = QRectF(QPointF(100, 100), QPointF(200, 200));
    m_rectangularGraphicsItem->setBoundingRect(rect);
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(200, 200);
    QPointF to(260, 230);
    ResizeLimits resizeLimits(&grip, from, to, rect);
    addOtherFunctionType(220, 220, 280, 280);

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(resizeLimits);

    // Inspection
    QRectF expectedRect(QRectF(QPointF(100, 100), QPointF(260, 220)));
    QCOMPARE(actualRect, expectedRect);
}


QTEST_MAIN(tst_FunctionTypeGraphicsItem)

#include "tst_functiontypegraphicsitem_grow.moc"
