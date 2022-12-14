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
 * The class under test is the abstract class 'VERectGraphicsItem'. FunctionTypeGraphicsItem provides a concrete child class that provides
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
    }

    QString getText()
    {
        return m_textItem->toPlainText();
    }

    // Public access to resizedRect
    virtual QRectF callResizedRect(GripPoint *grip, const QPointF &from, const QPointF &to)
    {
        return IVFunctionTypeGraphicsItem::resizedRect(grip, from, to);
    }
};

/**
 * VEObjectImpl is a mock of the abstract VEObject class, needed by VERectGraphicsItem because it contains the text
 * that is displayed on the VERectGraphicsItemImpl
 */
class FunctionTypeMock : public ivm::IVFunctionType
{
public:

    FunctionTypeMock(QString titleUI)
        : m_mockTitleUI(titleUI) {}

    virtual QString titleUI() const override
    {
        return m_mockTitleUI;
    }

private:
    QString m_mockTitleUI;
};

// Test class
class tst_FunctionTypeGraphicsItem : public QObject
{
    Q_OBJECT

public:
    tst_FunctionTypeGraphicsItem()
    : m_scene(QGraphicsScene(0,0, 800, 600))
    , m_entity(FunctionTypeMock("Gutterdammerung"))
    , m_rectangularGraphicsItem(new FunctionTypeGraphicsItem(&m_entity))


    {
        m_scene.addItem(m_rectangularGraphicsItem);
    }

private Q_SLOTS:
    void testEntityTextIsRendered();
    void testMovingRightGripLeft();
    void testMovingRightGripTooFarLeft();
    void testMovingLeftGripRight();
    void testMovingLeftGripTooFarRight();

private:
    QGraphicsScene m_scene;
    FunctionTypeMock m_entity;
    FunctionTypeGraphicsItem *m_rectangularGraphicsItem;

};


void tst_FunctionTypeGraphicsItem::testEntityTextIsRendered()
{
    QGraphicsScene scene(0,0, 800, 600); // The GraphicsItems need a scene to handle changes to geometry
    FunctionTypeMock entity("Gutterdammerung");
    FunctionTypeGraphicsItem *rectangularGraphicsItem = new FunctionTypeGraphicsItem(&entity);
    scene.addItem(rectangularGraphicsItem);
    rectangularGraphicsItem->updateText();
    rectangularGraphicsItem->setRect(QRectF(QPointF(50, 50), QPointF(400, 400)));
    QCOMPARE(rectangularGraphicsItem->getText(), QString("Gutterdammerung"));
}

/**
 * @brief The right grip is moved a little to the left without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The right side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingRightGripLeft()
{
    QGraphicsScene scene(0,0, 800, 600); // The GraphicsItems need a scene to handle changes to geometry
    FunctionTypeMock entity("i");
    FunctionTypeGraphicsItem *rectangularGraphicsItem = new FunctionTypeGraphicsItem(&entity);
    scene.addItem(rectangularGraphicsItem);
    rectangularGraphicsItem->updateText();
    rectangularGraphicsItem->setRect(QRectF(QPointF(50, 50), QPointF(400, 400)));
    GripPoint grip(GripPoint::Location::Right);
    QPointF from; // stub
    QPointF to(380, 250); // Where the user moved the right grip 10 to the left
    QRectF actualRect = rectangularGraphicsItem->callResizedRect(&grip, from, to);
    QRectF expectedRect(QPointF(50, 50), QPointF(380, 400));
    QCOMPARE(rectangularGraphicsItem->getText(), QString("i"));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The right grip is moved far to the left exeeding the limits of the text. No interface points are present.
 * Expected result: The right side of the item does not go as far left as the movement of the grip-point would indicate, had it
 * not been for the text blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingRightGripTooFarLeft()
{
    QGraphicsScene scene(0,0, 800, 600); // The GraphicsItems need a scene to handle changes to geometry
    FunctionTypeMock entity("Hozenblobbets");
    FunctionTypeGraphicsItem *rectangularGraphicsItem = new FunctionTypeGraphicsItem(&entity);
    scene.addItem(rectangularGraphicsItem);
    rectangularGraphicsItem->updateText();
    rectangularGraphicsItem->setRect(QRectF(QPointF(50, 50), QPointF(300, 300)));
    GripPoint grip(GripPoint::Location::Right);
    QPointF from; // stub
    QPointF to(150, 200); // Where the user moved grip to
    QRectF actualRect = rectangularGraphicsItem->callResizedRect(&grip, from, to);
    QRectF unrestrainedRect(QPointF(50, 50), QPointF(150, 300));
    QCOMPARE(rectangularGraphicsItem->getText(), QString("Hozenblobbets"));
    QVERIFY(actualRect.right() > unrestrainedRect.right());
}

/**
 * @brief The left grip is moved a little to the right without exeeding any limits. No interface points are present. Text does not interfere.
 * Expected result: The left side of the item has been moved according to the movement of the grip-point.
 */
void tst_FunctionTypeGraphicsItem::testMovingLeftGripRight()
{
    QGraphicsScene scene(0,0, 800, 600); // The GraphicsItems need a scene to handle changes to geometry
    FunctionTypeMock entity("i");
    FunctionTypeGraphicsItem *rectangularGraphicsItem = new FunctionTypeGraphicsItem(&entity);
    scene.addItem(rectangularGraphicsItem);
    rectangularGraphicsItem->updateText();
    rectangularGraphicsItem->setRect(QRectF(QPointF(50, 50), QPointF(400, 400)));
    GripPoint grip(GripPoint::Location::Left);
    QPointF from; // stub
    QPointF to(100, 250); // Where the user moved the right grip 10 to the left
    QRectF actualRect = rectangularGraphicsItem->callResizedRect(&grip, from, to);
    QRectF expectedRect(QPointF(100, 50), QPointF(400, 400));
    QCOMPARE(rectangularGraphicsItem->getText(), QString("i"));
    QCOMPARE(actualRect, expectedRect);
}

/**
 * @brief The left grip is moved far to the left exeeding the limits of the text. No interface points are present.
 * Expected result: The left side of the item does not go as far right as the movement of the grip-point would indicate, had it
 * not been for the text blocking the resizing.
 */
void tst_FunctionTypeGraphicsItem::testMovingLeftGripTooFarRight()
{
    QGraphicsScene scene(0,0, 800, 600); // The GraphicsItems need a scene to handle changes to geometry
    FunctionTypeMock entity("Hozenblobbets");
    FunctionTypeGraphicsItem *rectangularGraphicsItem = new FunctionTypeGraphicsItem(&entity);
    scene.addItem(rectangularGraphicsItem);
    rectangularGraphicsItem->updateText();
    rectangularGraphicsItem->setRect(QRectF(QPointF(50, 50), QPointF(300, 300)));
    GripPoint grip(GripPoint::Location::Left);
    QPointF from; // stub
    QPointF to(150, 200); // Where the user moved grip to
    QRectF actualRect = rectangularGraphicsItem->callResizedRect(&grip, from, to);
    QRectF unrestrainedRect(QPointF(50, 50), QPointF(150, 300));
    QCOMPARE(rectangularGraphicsItem->getText(), QString("Hozenblobbets"));
    QVERIFY(actualRect.right() > unrestrainedRect.right());
}


QTEST_MAIN(tst_FunctionTypeGraphicsItem)

#include "tst_functiontypegraphicsitem.moc"
