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
#include "itemeditor/ivinterfacegraphicsitem.h"
#include "iveditor.h"
#include "ivfunctiontype.h"
#include "ivlibrary.h"
#include "standardpaths.h"
#include "ui/textitem.h"

#include <QFontDatabase>
#include <QGraphicsScene>
#include <QtTest>

using namespace shared::ui;
using namespace ive;

/*
 * The class under test is 'the abstract class 'IVFunctionTypetGraphicsItem'. FunctionTypeGraphicsItem provides a concrete child class that provides
 * public member function 'callResizedRect' to call VERectGraphicsItem's protected member function 'resizedRect'.
 */
class FunctionTypeGraphicsItem : public IVFunctionTypeGraphicsItem
{
public:
    FunctionTypeGraphicsItem(ivm::IVFunctionType *entity, QGraphicsItem *parent = nullptr)
        : ive::IVFunctionTypeGraphicsItem(entity, parent)
    {
        QDirIterator dirIt(":/fonts");
        while (dirIt.hasNext())
        {
            QFontDatabase::addApplicationFont(dirIt.next());
        }
        setFont(QFont(QLatin1String("Ubuntu"), 8));
        m_textItem = initTextItem(); // Sets font on the textItem
        m_textItem->setObjectName("textItem");
    }

    QString getText()
    {
        return m_textItem->toPlainText();
    }

    // Public access to resizedRect
    virtual QRectF callResizedRect(GripPoint *grip, const QPointF &from, const QPointF &to)
    {
        ResizeLimits resizeLimits(grip, from, to, sceneBoundingRect());
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

    virtual void setTitleUI(QString titleUI)
    {
        m_mockTitleUI = titleUI;
    }

    virtual QString titleUI() const override
    {
        return m_mockTitleUI;
    }

private:
    QString m_mockTitleUI;
};

// Test class with some fixture
class tst_FunctionTypeGraphicsItem : public QObject
{
    Q_OBJECT

public:
    tst_FunctionTypeGraphicsItem()
    : m_scene(QGraphicsScene(0,0, 800, 600))
    , m_entity(FunctionTypeMock())
    , m_rectangularGraphicsItem(new FunctionTypeGraphicsItem(&m_entity))
    {
        shared::StandardPaths::setTestModeEnabled(true);
        ivm::initIVLibrary();
        ive::initIVEditor();

        m_scene.addItem(m_rectangularGraphicsItem);
    }

private Q_SLOTS:
    void testEntityTextIsRendered();
    void testMovingRightGripLeft();
    void testMovingRightGripTooFarLeft();
    void testMovingLeftGripRight();
    void testMovingLeftGripTooFarRight();
    void testMovingTopGripDown();
    void testMovingTopGripTooFarDown();
    void testMovingBottomGripUp();
    void testMovingBottomGripTooFarUp();

    void testMovingTopLeftGripDownAndRight();
    void testMovingTopLeftGripTooFarDownAndRight();
    void testMovingTopRightGripDownAndLeft();
    void testMovingTopRightGripTooFarDownAndLeft();
    void testMovingBottomRightGripUpAndLeft();
    void testMovingBottomRightGripTooFarUpAndLeft();
    void testMovingBottomLeftGripUpAndRight();
    void testMovingBottomLeftGripTooFarUpAndRight();

    void testMovingLeftGripPastInterface();
    void testMovingRightGripPastInterface();
    void testMovingTopGripPastInterface();
    void testMovingBottomGripPastInterface();

    void testMovingTopLeftGripPastInterface();
    void testMovingTopRightGripPastInterface();
    void testMovingBottomLeftGripPastInterface();
    void testMovingBottomRightGripPastInterface();

    // Called after each test function
    void cleanup()
    {
        removeInterfaces();
    }

private:
    QGraphicsScene m_scene;
    FunctionTypeMock m_entity;
    FunctionTypeGraphicsItem *m_rectangularGraphicsItem;

private:
    void setup_FunctionType_with_title(QString titleUI, QRectF rect = QRectF(QPointF(50, 50), QPointF(400, 400)))
    {
        m_entity.setTitleUI(titleUI);
        m_rectangularGraphicsItem->updateText();
        m_rectangularGraphicsItem->setRect(rect);
    }

    void add_interface_to_FunctionType(QRect interface, Qt::Alignment alignment)
    {
        IVInterfaceGraphicsItem *endpoint = new IVInterfaceGraphicsItem(nullptr, m_rectangularGraphicsItem);
        endpoint->setBoundingRect(interface);
        endpoint->setAlignment(alignment);
        endpoint->setObjectName("interface");
    }

    void removeInterfaces()
    {
        QList<QGraphicsItem *> children = m_rectangularGraphicsItem->childItems();
        for (QGraphicsItem *child : children)
        {
            auto *endpoint = qobject_cast<IVInterfaceGraphicsItem *>(child->toGraphicsObject());
            if (endpoint)
            {
                child->setParentItem(nullptr);
                delete child;
            }
        }
    }
};


void tst_FunctionTypeGraphicsItem::testEntityTextIsRendered()
{
    // Setup
    setup_FunctionType_with_title("Gutterdammerung");

    // Execution and Inspection
    QCOMPARE(m_rectangularGraphicsItem->getText(), QString("Gutterdammerung"));
}

/**
 * @brief The right grip is moved a little to the left without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The right side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingRightGripLeft()
{
    // Setup
    setup_FunctionType_with_title("i", QRectF(QPointF(50, 50), QPointF(400, 400)));
    GripPoint grip(GripPoint::Location::Right);
    QPointF from(400, 225);
    QPointF to(380, 250); // Move the RIGH grip-point 20 to the LEFT

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 50), QPointF(380, 400));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The right grip is moved far to the left exeeding the limits of the text. No interface points are present.
 * Expected result: The right side of the item does not go as far left as the movement of the grip-point would indicate, had it
 * not been for the text blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingRightGripTooFarLeft()
{
    // Setup
    setup_FunctionType_with_title("Hozenblobbets");
    GripPoint grip(GripPoint::Location::Right);
    QPointF from(400, 225);
    QPointF to(150, 200); // Move the RIGH grip-point 250 to the LEFT

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF unrestrainedRect(QPointF(50, 50), QPointF(150, 400));
    QVERIFY(actualRect.right() > unrestrainedRect.right());
}

/**
 * @brief The left grip is moved a little to the right without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The left side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingLeftGripRight()
{
    // Setup
    setup_FunctionType_with_title("i");
    GripPoint grip(GripPoint::Location::Left);
    QPointF from(50, 225);
    QPointF to(300, 200); // Move the LEFT grip 20 to the RIGHT

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(300, 50), QPointF(400, 400));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The left grip is moved far to the left exeeding the limits of the text. No interface points are present.
 * Expected result: The left side of the item does not go as far right as the movement of the grip-point would indicate, had it
 * not been for the text blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingLeftGripTooFarRight()
{
    // Setup
    setup_FunctionType_with_title("Hozenblobbets");
    GripPoint grip(GripPoint::Location::Left);
    QPointF from(50, 225);
    QPointF to(300, 200); // Move the LEFT grip 250 to the RIGHT

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF unrestrainedRect(QPointF(300, 50), QPointF(400, 400));
    QVERIFY(actualRect.left() < unrestrainedRect.left());
}

/**
 * @brief The top grip is moved a little down without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The top side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingTopGripDown()
{
    // Setup
    setup_FunctionType_with_title("i");
    GripPoint grip(GripPoint::Location::Top);
    QPointF from(225, 50);
    QPointF to(200, 100); // Move the Top grip 20 Down

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 100), QPointF(400, 400));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The Top grip is moved too far down exeeding the limits of the text. No interface points are present.
 * Expected result: The top side of the item does not go as far down as the movement of the grip-point would indicate, had it
 * not been for the text blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingTopGripTooFarDown()
{
    // Setup
    setup_FunctionType_with_title("L");
    GripPoint grip(GripPoint::Location::Top);
    QPointF from(225, 50);
    QPointF to(200, 350); // Move the TOP grip 300 DOWN

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF unrestrainedRect(QPointF(50, 350), QPointF(400, 400));
    QVERIFY(actualRect.top() < unrestrainedRect.top());
}

/**
 * @brief The Bottom grip is moved a little Up without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The b side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingBottomGripUp()
{
    // Setup
    setup_FunctionType_with_title("P");
    GripPoint grip(GripPoint::Location::Bottom);
    QPointF from(225, 400);
    QPointF to(200, 380); // Move the Bottom grip 20 Up

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 50), QPointF(400, 380));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The Bottom grip is moved too far Up exeeding the limits of the text. No interface points are present.
 * Expected result: The top side of the item does not go as far down as the movement of the grip-point would indicate, had it
 * not been for the text blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingBottomGripTooFarUp()
{
    // Setup
    setup_FunctionType_with_title("L");
    GripPoint grip(GripPoint::Location::Bottom);
    QPointF from(225, 400);
    QPointF to(200, 70); // Move the Bottom grip 300 Up

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF unrestrainedRect(QPointF(50, 50), QPointF(400, 70));
    QVERIFY(actualRect.bottom() > unrestrainedRect.bottom());
}

/**
 * @brief The TopLeft grip is moved a little Down and to the Right without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The top side and the left side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingTopLeftGripDownAndRight()
{
    // Setup
    setup_FunctionType_with_title("0");
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(50, 50);
    QPointF to(70, 70); // Move the TopLeft grip 20 Down and to the Right

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(70, 70), QPointF(400, 400));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The TopLeft grip is moved a little Down and to the Right without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The top side and the left side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingTopLeftGripTooFarDownAndRight()
{
    // Setup
    setup_FunctionType_with_title("0");
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(50, 50);
    QPointF to(380, 380); // Move the TopLeft grip 20 Down and to the Right

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF unrestrainedRect(QPointF(380, 380), QPointF(400, 400));
    QVERIFY(actualRect.top() < unrestrainedRect.top());
    QVERIFY(actualRect.left() < unrestrainedRect.left());
}


/**
 * @brief The TopRight grip is moved a little Down and to the Left without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The top side and the right side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingTopRightGripDownAndLeft()
{
    // Setup
    setup_FunctionType_with_title("0");
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(400, 50);
    QPointF to(380, 70); // Move the TopLeft grip 20 Down and to the Right

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 70), QPointF(380, 400));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The TopRight grip is moved a little Down and to the Left without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The top side and the right side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingTopRightGripTooFarDownAndLeft()
{
    // Setup
    setup_FunctionType_with_title("SprinkleWidget");
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(400, 50);
    QPointF to(70, 380); // Move the TopLeft grip 20 Down and to the Right

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF unrestrainedRect(QPointF(50, 380), QPointF(70, 400));
    QVERIFY(actualRect.top() < unrestrainedRect.top());
    QVERIFY(actualRect.right() > unrestrainedRect.right());
}

/**
 * @brief The BottomRight grip is moved a little Up and to the Left without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The top side and the right side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingBottomRightGripUpAndLeft()
{
    // Setup
    setup_FunctionType_with_title("0");
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(400, 400);
    QPointF to(380, 380); // Move the TopLeft grip 20 Down and to the Right

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 50), QPointF(380, 380));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The BottomRight grip is moved a little Up and to the Left without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The top side and the right side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingBottomRightGripTooFarUpAndLeft()
{
    // Setup
    setup_FunctionType_with_title("SprinkleWidget");
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(400, 400);
    QPointF to(100, 100); // Move the BottomRight grip 50 Up and to the Left

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF unrestrainedRect(QPointF(50, 50), QPointF(100, 100));
    QVERIFY(actualRect.bottom() > unrestrainedRect.bottom());
    QVERIFY(actualRect.right() > unrestrainedRect.right());
}

/**
 * @brief The BottomLeft grip is moved a little Up and to the Right without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The Bottom side and the Left side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingBottomLeftGripUpAndRight()
{
    // Setup
    setup_FunctionType_with_title("0");
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(50, 400);
    QPointF to(80, 380); // Move the BottomLeft grip 20 Up and to the Left

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(80, 50), QPointF(400, 380));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The BottomLeft grip is moved a little Up and to the Right without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The top side and the right side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingBottomLeftGripTooFarUpAndRight()
{
    // Setup
    setup_FunctionType_with_title("SprinkleWidget");
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(50, 400);
    QPointF to(100, 100); // Move the BottomRight grip 50 Up and to the Left

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF unrestrainedRect(QPointF(50, 50), QPointF(100, 100));
    QVERIFY(actualRect.bottom() > unrestrainedRect.bottom());
    QVERIFY(actualRect.right() > unrestrainedRect.right());
}

// ========================================================================================================================================
/**
 * @brief The left grip is moved to the right going past an interface.
 * Expected result: The left side of the item does not go as far right as the movement of the grip-point would indicate, had it
 * not been for the interface blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingLeftGripPastInterface()
{
    // Setup
    setup_FunctionType_with_title("o");
    add_interface_to_FunctionType(QRect(20, 0, 10, 10), Qt::AlignTop);
    GripPoint grip(GripPoint::Location::Left);
    QPointF from(50, 225);
    QPointF to(100, 200); // Move the Left grip-point 50 to the Right

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(70, 50), QPointF(400, 400));
    QCOMPARE(actualRect, expectedRect);
}


/**
 * @brief The right grip is moved to the left going past an interface.
 * Expected result: The right side of the item does not go as far left as the movement of the grip-point would indicate, had it
 * not been for the interface blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingRightGripPastInterface()
{
    // Setup
    setup_FunctionType_with_title("o");
    add_interface_to_FunctionType(QRect(300, 0, 10, 10), Qt::AlignTop);
    GripPoint grip(GripPoint::Location::Right);
    QPointF from(400, 225);
    QPointF to(200, 200); // Move the RIGH grip-point 50 to the LEFT

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 50), QPointF(360, 400));
    QCOMPARE(actualRect, expectedRect);
}


/**
 * @brief The top grip is moved down going past an interface.
 * Expected result: The top side of the item does not go as far down as the movement of the grip-point would indicate, had it
 * not been for the interface blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingTopGripPastInterface()
{
    // Setup
    setup_FunctionType_with_title("o");
    add_interface_to_FunctionType(QRect(0, 20, 10, 10), Qt::AlignLeft);
    GripPoint grip(GripPoint::Location::Top);
    QPointF from(225, 50);
    QPointF to(200, 100); // Move the top grip-point 50 down

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 70), QPointF(400, 400));
    QCOMPARE(actualRect, expectedRect);
}


/**
 * @brief The bottom grip is moved doen going past an interface.
 * Expected result: The bottom side of the item does not go as far up as the movement of the grip-point would indicate, had it
 * not been for the interface blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingBottomGripPastInterface()
{
    // Setup
    setup_FunctionType_with_title("o");
    add_interface_to_FunctionType(QRect(0, 330, 10, 10), Qt::AlignLeft);
    GripPoint grip(GripPoint::Location::Bottom);
    QPointF from(225, 400);
    QPointF to(200, 200); // Move the bottom grip-point 50 up

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 50), QPointF(400, 390));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The bottom grip is moved doen going past an interface.
 * Expected result: The bottom side of the item does not go as far up as the movement of the grip-point would indicate, had it
 * not been for the interface blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingTopLeftGripPastInterface()
{
    // Setup
    setup_FunctionType_with_title("o");
    add_interface_to_FunctionType(QRect(20, 0, 10, 10), Qt::AlignTop);
    add_interface_to_FunctionType(QRect(0, 20, 10, 10), Qt::AlignLeft);
    GripPoint grip(GripPoint::Location::TopLeft);
    QPointF from(50, 50);
    QPointF to(200, 200); // Move the grip-point torwards center

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(70, 70), QPointF(400, 400));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The bottom grip is moved doen going past an interface.
 * Expected result: The bottom side of the item does not go as far up as the movement of the grip-point would indicate, had it
 * not been for the interface blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingTopRightGripPastInterface()
{
    // Setup
    setup_FunctionType_with_title("o");
    add_interface_to_FunctionType(QRect(330, 0, 10, 10), Qt::AlignTop);
    add_interface_to_FunctionType(QRect(350, 20, 10, 10), Qt::AlignRight);
    GripPoint grip(GripPoint::Location::TopRight);
    QPointF from(400, 50);
    QPointF to(200, 200); // Move the grip-point torwards center

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 70), QPointF(390, 400));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The bottom grip is moved doen going past an interface.
 * Expected result: The bottom side of the item does not go as far up as the movement of the grip-point would indicate, had it
 * not been for the interface blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingBottomRightGripPastInterface()
{
    // Setup
    setup_FunctionType_with_title("o");
    add_interface_to_FunctionType(QRect(350, 330, 10, 10), Qt::AlignRight);
    add_interface_to_FunctionType(QRect(330, 350, 10, 10), Qt::AlignBottom);
    GripPoint grip(GripPoint::Location::BottomRight);
    QPointF from(400, 400);
    QPointF to(200, 200); // Move the grip-point torwards center
    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(50, 50), QPointF(390, 390));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The bottom grip is moved doen going past an interface.
 * Expected result: The bottom side of the item does not go as far up as the movement of the grip-point would indicate, had it
 * not been for the interface blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingBottomLeftGripPastInterface()
{
    // Setup
    setup_FunctionType_with_title("o");
    add_interface_to_FunctionType(QRect(20, 350, 10, 10), Qt::AlignBottom);
    add_interface_to_FunctionType(QRect(0, 330, 10, 10), Qt::AlignLeft);
    GripPoint grip(GripPoint::Location::BottomLeft);
    QPointF from(50, 400);
    QPointF to(200, 200); // Move the grip-point torwards center

    // Execution
    QRectF actualRect = m_rectangularGraphicsItem->callResizedRect(&grip, from, to);

    // Inspection
    QRectF expectedRect(QPointF(70, 50), QPointF(400, 390));
    QCOMPARE(actualRect, expectedRect);
}

QTEST_MAIN(tst_FunctionTypeGraphicsItem)

#include "tst_functiontypegraphicsitem.moc"
