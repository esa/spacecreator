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

#include "qtestcase.h"
#include "ui/resizelimits.h"

#include <QtTest>
#include <QGraphicsScene>
#include <QFontDatabase>

using namespace shared::ui;

/*
 * The class under test is the class 'ResizeLimits'. It reads a gripPoint, a from-coordinate, a to-coordinate and a
 * rectangle.
 *
 */

// Test class with some fixture
class tst_ResizeLimits : public QObject
{
    Q_OBJECT

public:
    tst_ResizeLimits() { }

private Q_SLOTS:
    // Minimizing
    void testMovingRightGripLeft();
    void testMovingLeftGripRight();
    void testMovingTopGripDown();
    void testMovingBottomGripUp();

    // Maximizing
    void testMovingRightGripRight();
    void testMovingLeftGripLeft();
    void testMovingTopGripUp();
    void testMovingBottomGripDown();

    // Minimizing corners
    void testMovingTopLeftGripDownAndRight();
    void testMovingTopRightGripDownAndLeft();
    void testMovingBottomRightGripUpAndLeft();
    void testMovingBottomLeftGripUpAndRight();

    // Maximizing corners
    void testMovingTopLeftGripUpAndLeft();
    void testMovingTopRightGripUpAndRight();
    void testMovingBottomLeftGripDownAndLeft();
    void testMovingBottomRightGripDownAndRight();

    // Intersection Edge
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
/**
 * The right grip is moved 20 to the left.
 * Expected result: HorizontalResizeType is Min, VerticalResizeType is None, Direction
 */
void tst_ResizeLimits::testMovingRightGripLeft()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::Right);
    QPointF from(300, 200);
    QPointF to(320, 200); // Move the RIGHT grip-point 20 to the LEFT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::None);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Max);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::RightEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::None);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 100), QPointF(320, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, rect);

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(100, 100), QPointF(320, 300)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout20 = qFuzzyCompare(actualVerticalDistance, 20.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout0 = qFuzzyCompare(actualHorizontalDistance, 0.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout0, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}


/**
 * @brief The left grip is moved a little to the right without exeeding any limits. No interface points are present.
 * Text does not interfere. Expected result: The left side of the item has been moved according to the movement of the
 * grip-point.
 */
void tst_ResizeLimits::testMovingLeftGripRight()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::Left);
    QPointF from(100, 200);
    QPointF to(120, 200); // Move the LEFT grip-point 20 to the RIGHT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::None);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Min);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::LeftEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::None);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(120, 100), QPointF(300, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, rect);

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(120, 100), QPointF(300, 300)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout20 = qFuzzyCompare(actualVerticalDistance, 20.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout0 = qFuzzyCompare(actualHorizontalDistance, 0.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout0, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), true);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}


/**
 * @brief The top grip is moved a little down without exeeding any limits. No interface points are present. Text does
 * not interfere. Expected result: The top side of the item has been moved according to the movement of the grip-point.
 */
void tst_ResizeLimits::testMovingTopGripDown()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::Top);
    QPointF from(200, 100);
    QPointF to(200, 120); // Move the TOP grip-point 20 UP

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Min);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::None);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::None);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::TopEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 120), QPointF(300, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 120), QPointF(300, 300)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, rect);

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout0 = qFuzzyCompare(actualVerticalDistance, 0.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout0, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout20 = qFuzzyCompare(actualHorizontalDistance, 20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), true);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}


/**
 * @brief The Bottom grip is moved a little Up without exeeding any limits. No interface points are present. Text does
 * not interfere. Expected result: The b side of the item has been moved according to the movement of the grip-point.
 */
void tst_ResizeLimits::testMovingBottomGripUp()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::Bottom);
    QPointF from(200, 300);
    QPointF to(200, 280); // Move the BOTTOM grip-point 20 UP

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Min);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::None);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::None);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::BottomEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 100), QPointF(300, 280)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 100), QPointF(300, 280)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, rect);

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout20 = qFuzzyCompare(actualVerticalDistance, 0.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualHorizontalDistance, -20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAboutMinus20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), true);
}

void tst_ResizeLimits::testMovingRightGripRight()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::Right);
    QPointF from(300, 200);
    QPointF to(320, 200); // Move the RIGHT grip-point 20 RIGHT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::None);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Max);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::RightEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::None);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 100), QPointF(320, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, rect);

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(100, 100), QPointF(320, 300)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout20 = qFuzzyCompare(actualVerticalDistance, 20.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout20 = qFuzzyCompare(actualHorizontalDistance, 0.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}

void tst_ResizeLimits::testMovingLeftGripLeft()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::Left);
    QPointF from(100, 200);
    QPointF to(80, 200); // Move the LEFT grip-point 20 LEFT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::None);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Max);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::LeftEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::None);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(80, 100), QPointF(300, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, rect);

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(80, 100), QPointF(300, 300)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualVerticalDistance, -20.0);
    QCOMPARE(actualVerticalDistanceEqualsAboutMinus20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout20 = qFuzzyCompare(actualHorizontalDistance, 0.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}

void tst_ResizeLimits::testMovingTopGripUp()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::Top);
    QPointF from(200, 100);
    QPointF to(200, 80); // Move the TOP grip-point 20 UP

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Max);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::None);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::None);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::TopEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 80), QPointF(300, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 80), QPointF(300, 300)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, rect);

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout0 = qFuzzyCompare(actualVerticalDistance, 0.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout0, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualHorizontalDistance, -20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAboutMinus20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}

void tst_ResizeLimits::testMovingBottomGripDown()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::Bottom);
    QPointF from(200, 300);
    QPointF to(200, 320); // Move the BOTTOM grip-point 20 DOWN

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Max);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::None);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::None);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::BottomEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 100), QPointF(300, 320)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 100), QPointF(300, 320)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, rect);

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout0 = qFuzzyCompare(actualVerticalDistance, 0.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout0, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout20 = qFuzzyCompare(actualHorizontalDistance, 20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), true);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}

/**
 * @brief The TopLeft grip is moved a little Down and to the Right without exeeding any limits. No interface points are
 * present. Text does not interfere. Expected result: The top side and the left side of the item has been moved
 * according to the movement of the grip-point.
 */
void tst_ResizeLimits::testMovingTopLeftGripDownAndRight()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(100, 100);
    QPointF to(120, 120); // Move the TOP-LEFT grip-point 20 DOWN and to the RIGHT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Min);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Min);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::LeftEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::TopEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(120, 120), QPointF(300, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 120), QPointF(300, 300)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(120, 100), QPointF(300, 300)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout20 = qFuzzyCompare(actualVerticalDistance, 20.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout20 = qFuzzyCompare(actualHorizontalDistance, 20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), true);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), true);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}


/**
 * @brief The TopRight grip is moved a little Down and to the Left without exeeding any limits. No interface points are
 * present. Text does not interfere. Expected result: The top side and the right side of the item has been moved
 * according to the movement of the grip-point.
 */
void tst_ResizeLimits::testMovingTopRightGripDownAndLeft()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(300, 100);
    QPointF to(280, 120); // Move the TOP-RIGHT grip-point 20 DOWN and to the LEFT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Min);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Min);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::RightEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::TopEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 120), QPointF(280, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 120), QPointF(300, 300)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(100, 100), QPointF(280, 300)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualVerticalDistance, -20.0);
    QCOMPARE(actualVerticalDistanceEqualsAboutMinus20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout20 = qFuzzyCompare(actualHorizontalDistance, 20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), true);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), true);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}


/**
 * @brief The BottomRight grip is moved a little Up and to the Left without exeeding any limits. No interface points are
 * present. Text does not interfere. Expected result: The top side and the right side of the item has been moved
 * according to the movement of the grip-point.
 */
void tst_ResizeLimits::testMovingBottomRightGripUpAndLeft()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(300, 300);
    QPointF to(280, 280); // Move the BOTTOM-RIGHT grip-point 20 UP and to the LEFT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Min);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Min);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::RightEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::BottomEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 100), QPointF(280, 280)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 100), QPointF(300, 280)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(100, 100), QPointF(280, 300)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualVerticalDistance, -20.0);
    QCOMPARE(actualVerticalDistanceEqualsAboutMinus20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualHorizontalDistance, -20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAboutMinus20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), true);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), true);
}



/**
 * @brief The BottomLeft grip is moved a little Up and to the Right without exeeding any limits. No interface points are
 * present. Text does not interfere. Expected result: The Bottom side and the Left side of the item has been moved
 * according to the movement of the grip-point.
 */
void tst_ResizeLimits::testMovingBottomLeftGripUpAndRight()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(100, 300);
    QPointF to(120, 280); // Move the BOTTOM-LEFT grip-point 20 UP and to the RIGHT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Min);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Min);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::LeftEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::BottomEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(120, 100), QPointF(300, 280)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 100), QPointF(300, 280)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(120, 100), QPointF(300, 300)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout20 = qFuzzyCompare(actualVerticalDistance, 20.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualHorizontalDistance, -20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAboutMinus20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), true);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), true);
}

void tst_ResizeLimits::testMovingTopLeftGripUpAndLeft()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(100, 100);
    QPointF to(80, 80); // Move the TOP-LEFT grip-point 20 UP and to the LEFT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Max);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Max);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::LeftEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::TopEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(80, 80), QPointF(300, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 80), QPointF(300, 300)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(80, 100), QPointF(300, 300)));

    QRectF actualTopLeftRect = resizeLimits.getUnlimitedTopLeftRect();
    QCOMPARE(actualTopLeftRect, QRectF(to, from));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualVerticalDistance, -20.0);
    QCOMPARE(actualVerticalDistanceEqualsAboutMinus20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualHorizontalDistance, -20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAboutMinus20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}

void tst_ResizeLimits::testMovingTopRightGripUpAndRight()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(300, 100);
    QPointF to(320, 80); // Move the TOP-RIGH grip-point 20 UP and to the RIGH

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Max);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Max);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::RightEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::TopEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 80), QPointF(320, 300)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 80), QPointF(300, 300)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(100, 100), QPointF(320, 300)));

    QRectF actualUnlimitedTopRightRect = resizeLimits.getUnlimitedTopRightRect();
    QCOMPARE(actualUnlimitedTopRightRect, QRectF(QPointF(300, 80), QPointF(320, 100)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout20 = qFuzzyCompare(actualVerticalDistance, 20.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualHorizontalDistance, -20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAboutMinus20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), false);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}

void tst_ResizeLimits::testMovingBottomLeftGripDownAndLeft()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(100, 300);
    QPointF to(80, 320); // Move the BOTTOM-LEFT grip-point 20 DOWN and to the LEFT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Max);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Max);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::LeftEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::BottomEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(80, 100), QPointF(300, 320)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 100), QPointF(300, 320)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(80, 100), QPointF(300, 300)));

    QRectF actualUnlimitedBottomLeftRect = resizeLimits.getUnlimitedBottomLeftRect();
    QCOMPARE(actualUnlimitedBottomLeftRect, QRectF(QPointF(80, 300), QPointF(100, 320)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAboutMinus20 = qFuzzyCompare(actualVerticalDistance, -20.0);
    QCOMPARE(actualVerticalDistanceEqualsAboutMinus20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout20 = qFuzzyCompare(actualHorizontalDistance, 20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), true);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}

void tst_ResizeLimits::testMovingBottomRightGripDownAndRight()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(300, 300));
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(300, 300);
    QPointF to(320, 320); // Move the BOTTOM-RIGHT grip-point 20 DOWN and to the RIGHT

    // Execution
    ResizeLimits resizeLimits(&grip, from, to, rect);

    // Inspection
    ResizeType actualHorizontalResizeType = resizeLimits.getHorizontalResizeType();
    QCOMPARE(actualHorizontalResizeType, ResizeType::Max);

    ResizeType actualVerticalResizeType = resizeLimits.getVerticalResizeType();
    QCOMPARE(actualVerticalResizeType, ResizeType::Max);

    VerticalEdge actualVerticalEdge = resizeLimits.getVerticalEdge();
    QCOMPARE(actualVerticalEdge, VerticalEdge::RightEdge);

    HorizontalEdge actualHorizontalEdge = resizeLimits.getHorizontalEdge();
    QCOMPARE(actualHorizontalEdge, HorizontalEdge::BottomEdge);

    QRectF actualUnlimitedRect = resizeLimits.getUnlimitedRect();
    QCOMPARE(actualUnlimitedRect, QRectF(QPointF(100, 100), QPointF(320, 320)));

    QRectF actualUnlimitedHorizontalRect = resizeLimits.getUnlimitedHorizontalRect();
    QCOMPARE(actualUnlimitedHorizontalRect, QRectF(QPointF(100, 100), QPointF(300, 320)));

    QRectF actualUnlimitedVerticalRect = resizeLimits.getUnlimitedVerticalRect();
    QCOMPARE(actualUnlimitedVerticalRect, QRectF(QPointF(100, 100), QPointF(320, 300)));

    qreal actualVerticalDistance = resizeLimits.getVerticalDistance();
    bool actualVerticalDistanceEqualsAbout20 = qFuzzyCompare(actualVerticalDistance, 20.0);
    QCOMPARE(actualVerticalDistanceEqualsAbout20, true);

    qreal actualHorizontalDistance = resizeLimits.getHorizontalDistance();
    bool actualHorizontalDistanceEqualsAbout20 = qFuzzyCompare(actualHorizontalDistance, 20.0);
    QCOMPARE(actualHorizontalDistanceEqualsAbout20, true);

    QCOMPARE(resizeLimits.isRightEdgeMaximizing(), true);
    QCOMPARE(resizeLimits.isLeftEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMaximizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMaximizing(), true);

    QCOMPARE(resizeLimits.isRightEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isLeftEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isTopEdgeMinimizing(), false);
    QCOMPARE(resizeLimits.isBottomEdgeMinimizing(), false);
}

void tst_ResizeLimits::testTopLeftGripPointMovingUpAndLeftIntersectingOtherRectThroughRightEdge()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(100, 100);
    QPointF to(70, 40); // Move the TOP-LEFT grip-point through the RIGHT edge of otherRect
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(20, 20), QPointF(80, 80));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdges = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdges = intersectingEdges.second;

    // Inspection
    QCOMPARE(intersectingVerticalEdges, VerticalEdge::RightEdge);
}

void tst_ResizeLimits::testTopLeftGripPointMovingUpAndLeftIntersectingOtherRectThroughBottomRightCorner()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(100, 100);
    QPointF to(60, 60); // Move the TOP-LEFT grip-point through the BOTTOM-RIGHT corner of otherRect
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(20, 20), QPointF(80, 80));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdge = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdge.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdge.second;

    // Inspection
    QCOMPARE(intersectingHorizontalEdge, HorizontalEdge::BottomEdge);
    QCOMPARE(intersectingVerticalEdge, VerticalEdge::RightEdge);
}

void tst_ResizeLimits::testTopLeftGripPointMovingUpAndLeftIntersectingOtherRectThroughBottomEdge()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(100, 100);
    QPointF to(40, 70); // Move the TOP-LEFT grip-point through the BOTTOM edge of otherRect
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(20, 20), QPointF(80, 80));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;

    // Inspection
    QCOMPARE(intersectingHorizontalEdge, HorizontalEdge::BottomEdge);
}

void tst_ResizeLimits::testTopRightGripPointMovingUpAndRightIntersectingOtherRectThroughLeftEdge()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(200, 100);
    QPointF to(230, 40); // Move the TOP-LEFT grip-point through the BOTTOM edge of otherRect
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(220, 20), QPointF(280, 80));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;

    // Inspection
    QCOMPARE(intersectingVerticalEdge, VerticalEdge::LeftEdge);
}

void tst_ResizeLimits::testTopRightGripPointMovingUpAndRightIntersectingOtherRectThroughBottomLeftCorner()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(200, 100);
    QPointF to(240, 60); // Move the TOP-LEFT grip-point through the BOTTOM edge of otherRect
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(220, 20), QPointF(280, 80));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;

    // Inspection
    QCOMPARE(intersectingVerticalEdge, VerticalEdge::LeftEdge);
    QCOMPARE(intersectingHorizontalEdge, HorizontalEdge::BottomEdge);

}

void tst_ResizeLimits::testTopRightGripPointMovingUpAndRightIntersectingOtherRectThroughBottomEdge()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(200, 100);
    QPointF to(260, 70); // Move the TOP-LEFT grip-point through the BOTTOM edge of otherRect
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(220, 20), QPointF(280, 80));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;

    // Inspection
    QCOMPARE(intersectingHorizontalEdge, HorizontalEdge::BottomEdge);
}

void tst_ResizeLimits::testBottomLeftGripPointMovingDownAndLeftIntersectingOtherRectThroughRightEdge()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(100, 200);
    QPointF to(70, 260); // Move the BOTTOM-LEFT grip-point through the RIGHT edge of otherRect
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(20, 220), QPointF(80, 280));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;;

    // Inspection
    QCOMPARE(intersectingVerticalEdge, VerticalEdge::RightEdge);
}

void tst_ResizeLimits::testBottomLeftGripPointMovingDownAndLeftIntersectingOtherRectThroughTopRightCorner()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(100, 200);
    QPointF to(60, 240); // Move the BOTTOM-LEFT grip-point through the RIGHT edge of otherRect
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(20, 220), QPointF(80, 280));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;
    // Inspection
    QCOMPARE(intersectingHorizontalEdge, HorizontalEdge::TopEdge);
}

void tst_ResizeLimits::testBottomLeftGripPointMovinDownAndLeftIntersectingOtherRectThroughTopEdge()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(100, 200);
    QPointF to(40, 230); // Move the BOTTOM-LEFT grip-point through the RIGHT edge of otherRect
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(20, 220), QPointF(80, 280));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;

           // Inspection
    QCOMPARE(intersectingHorizontalEdge, HorizontalEdge::TopEdge);
}

void tst_ResizeLimits::testBottomRightGripPointMovingDownAndRightIntersectingOtherRectThroughLeftEdge()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(200, 200);
    QPointF to(240, 260);
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(220, 220), QPointF(280, 280));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;

    // Inspection
    QCOMPARE(intersectingVerticalEdge, VerticalEdge::LeftEdge);
}

void tst_ResizeLimits::testBottomRightGripPointMovingDownAndRightIntersectingOtherRectThroughTopLeftCorner()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(200, 200);
    QPointF to(240, 240);
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(220, 220), QPointF(280, 280));

    // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;

    // Inspection
    QCOMPARE(intersectingVerticalEdge, VerticalEdge::LeftEdge);
}

void tst_ResizeLimits::testBottomRightGripPointMovinDownAndRightIntersectingOtherRectThroughTopEdge()
{
    // Setup
    QRectF rect(QPointF(100, 100), QPointF(200, 200));
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(200, 200);
    QPointF to(260, 230);
    ResizeLimits resizeLimits(&grip, from, to, rect);

    QRectF otherRect(QPointF(220, 220), QPointF(280, 280));

           // Execution
    QPair<HorizontalEdge, VerticalEdge> intersectingEdges = resizeLimits.getIntersectedEdges(otherRect);
    HorizontalEdge intersectingHorizontalEdge = intersectingEdges.first;
    VerticalEdge intersectingVerticalEdge = intersectingEdges.second;

           // Inspection
    QCOMPARE(intersectingHorizontalEdge, HorizontalEdge::TopEdge);
}

QTEST_MAIN(tst_ResizeLimits)

#include "tst_resizelimits.moc"
