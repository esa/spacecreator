/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "itemeditor/ivcommentgraphicsitem.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivfunctiontypegraphicsitem.h"
#include "itemeditor/ivinterfacegraphicsitem.h"
#include "iveditor.h"
#include "sharedlibrary.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QtTest>

class tst_IVInterfaceGraphicsItem : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testMaxWidth();
};

void tst_IVInterfaceGraphicsItem::initTestCase()
{
    ive::initIVEditor();
    shared::initSharedLibrary();
    QStandardPaths::setTestModeEnabled(true);
}

void tst_IVInterfaceGraphicsItem::testMaxWidth()
{
    QGraphicsScene scene;
    auto parentFunc = new ive::IVFunctionGraphicsItem(nullptr);
    scene.addItem(parentFunc);
    parentFunc->setBoundingRect(QRectF(0., 0., 200., 200.));
    auto item = new ive::IVInterfaceGraphicsItem(nullptr, parentFunc);
    item->setBoundingRect(QRectF(200., 10., 50., 30.));

    QCOMPARE(item->maxWidth(), -1.);

    auto funcItem = new ive::IVFunctionGraphicsItem(nullptr);
    scene.addItem(funcItem);
    funcItem->setBoundingRect(QRectF(400., 400., 300., 300.));
    QCOMPARE(item->maxWidth(), -1.);

    // Function moved right of the interface
    funcItem->setBoundingRect(QRectF(400., 0., 300., 300.));
    QCOMPARE(item->maxWidth(), 200.);
    // Invisible items don't limit the width
    funcItem->setVisible(false);
    QCOMPARE(item->maxWidth(), -1.);

    // Another interface on the right
    item->setBoundingRect(QRectF(20., 0., 50., 30.));
    auto item2 = new ive::IVInterfaceGraphicsItem(nullptr, parentFunc);
    item2->setBoundingRect(QRectF(130., 0., 50., 30.));
    QCOMPARE(item->maxWidth(), 110.);
    // Invisible items don't limit the width
    item2->setVisible(false);
    QCOMPARE(item->maxWidth(), -1.);
    // Item above/below don't limit the size
    item2->setVisible(true);
    item2->setBoundingRect(QRectF(130., -40., 50., 30.));
    QCOMPARE(item->maxWidth(), -1.);
    item2->setBoundingRect(QRectF(130., 50., 50., 30.));
    QCOMPARE(item->maxWidth(), -1.);

    // Function type
    auto typeItem = new ive::IVFunctionTypeGraphicsItem(nullptr);
    scene.addItem(typeItem);
    typeItem->setBoundingRect(QRectF(600., 0., 300., 300.));
    QCOMPARE(item->maxWidth(), 580.);

    // Comment
    auto commentItem = new ive::IVCommentGraphicsItem(nullptr);
    scene.addItem(commentItem);
    commentItem->setBoundingRect(QRectF(450., 0., 300., 300.));
    QCOMPARE(item->maxWidth(), 430.);
}

QTEST_MAIN(tst_IVInterfaceGraphicsItem)

#include "tst_ivinterfacegraphicsitem.moc"
