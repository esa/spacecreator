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

#include "itemeditor/common/ivutils.h"
#include "itemeditor/ivcommentgraphicsitem.h"
#include "itemeditor/ivconnectiongraphicsitem.h"
#include "itemeditor/ivconnectiongroupgraphicsitem.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivfunctiontypegraphicsitem.h"
#include "itemeditor/ivinterfacegraphicsitem.h"
#include "itemeditor/ivinterfacegroupgraphicsitem.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivinterfacegroup.h"
#include "ivobject.h"
#include "ivtestutils.h"
#include "sharedlibrary.h"
#include "ui/veinteractiveobject.h"

#include <QMetaEnum>
#include <QObject>
#include <QtTest>
#include <memory>

class tst_ItemZOrder : public QObject
{
    Q_OBJECT
private:
    void checkItem(shared::ui::VEInteractiveObject *item, const qreal expectedZ);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testItem_FunctionType();
    void testItem_Function();
    void testItem_Comment();
    void testItem_InterfaceGroup();
    void testItem_RequiredInterface();
    void testItem_ProvidedInterface();
    void testItem_Connection();
    void testItem_ConnectionGroup();
    void testItem_ConnectionLayer();

    void testItem_CheckCoverage();

private:
    int m_itemTypesTested { 0 };
};

void tst_ItemZOrder::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    shared::initSharedLibrary();
    m_itemTypesTested = 0;
}

void tst_ItemZOrder::cleanupTestCase()
{
    QStandardPaths::setTestModeEnabled(false);
}

void tst_ItemZOrder::checkItem(shared::ui::VEInteractiveObject *item, const qreal expectedZ)
{
    if (!item) {
        return;
    }
    auto entity = item->entity();
    if (!entity) {
        return;
    }
    if (auto ivObj = entity->as<ivm::IVObject *>()) {
        if (ivObj->type() == ivm::IVObject::Type::Unknown) {
            return;
        }
    } else {
        return;
    }

    ++m_itemTypesTested;

    item->setSelected(false);
    QVERIFY(item->isSelected() == false);
    QCOMPARE(item->itemLevel(item->isSelected()), expectedZ);

    item->setSelected(true);
    QVERIFY(item->isSelected() == true);
    QCOMPARE(item->itemLevel(item->isSelected()), ive::ZOrder.Selected);
}

void tst_ItemZOrder::testItem_FunctionType()
{
    ivm::IVFunctionType *ivObject = ivm::testutils::createFunctionType(QStringLiteral("TestFunctionType"));
    ive::IVFunctionTypeGraphicsItem item(ivObject);

    checkItem(&item, ive::ZOrder.Function);
    delete ivObject;
}

void tst_ItemZOrder::testItem_Function()
{
    ivm::IVFunction *ivObject = ivm::testutils::createFunction(QStringLiteral("TestFunction"));
    ive::IVFunctionGraphicsItem item(ivObject);

    checkItem(&item, ive::ZOrder.Function);
    delete ivObject;
}

void tst_ItemZOrder::testItem_Comment()
{
    ivm::IVComment *ivObject = ivm::testutils::createComment(QStringLiteral("TestComment"));
    ive::IVCommentGraphicsItem item(ivObject);

    checkItem(&item, ive::ZOrder.Comment);
    delete ivObject;
}

void tst_ItemZOrder::testItem_InterfaceGroup()
{
    ivm::IVFunction *ivFunction = ivm::testutils::createFunction(QStringLiteral("TestFunction"));
    ivm::IVInterface::CreationInfo ci;
    ci.function = ivFunction;
    ci.type = ivm::IVInterface::InterfaceType::Grouped;
    auto pIface = new ivm::IVInterfaceGroup(ci);
    ive::IVInterfaceGroupGraphicsItem item(pIface);

    checkItem(&item, ive::ZOrder.Interface);
    delete ivFunction;
}

void tst_ItemZOrder::testItem_RequiredInterface()
{
    ivm::IVFunction *ivFunction = ivm::testutils::createFunction(QStringLiteral("TestFunction"));
    ivm::IVInterface::CreationInfo ci;
    ci.function = ivFunction;
    ci.type = ivm::IVInterface::InterfaceType::Required;
    auto pIface = ivm::IVInterface::createIface(ci);
    ive::IVInterfaceGraphicsItem item(pIface);

    checkItem(&item, ive::ZOrder.Interface);
    delete ivFunction;
}

void tst_ItemZOrder::testItem_ProvidedInterface()
{
    ivm::IVFunction *ivFunction = ivm::testutils::createFunction(QStringLiteral("TestFunction"));
    ivm::IVInterface::CreationInfo ci;
    ci.function = ivFunction;
    ci.type = ivm::IVInterface::InterfaceType::Provided;
    auto pIface = ivm::IVInterface::createIface(ci);
    ive::IVInterfaceGraphicsItem item(pIface);

    checkItem(&item, ive::ZOrder.Interface);
    delete ivFunction;
}

void tst_ItemZOrder::testItem_Connection()
{
    ivm::IVFunction *ivFunctionA = ivm::testutils::createFunction(QStringLiteral("TestFunctionA"));
    ivm::IVFunction *ivFunctionB = ivm::testutils::createFunction(QStringLiteral("TestFunctionB"));

    ivm::IVInterface::CreationInfo ciA;
    ciA.function = ivFunctionA;
    ciA.type = ivm::IVInterface::InterfaceType::Provided;
    auto pIfaceA = ivm::IVInterface::createIface(ciA);
    ive::IVInterfaceGraphicsItem itemA(pIfaceA);

    ivm::IVInterface::CreationInfo ciB;
    ciB.function = ivFunctionA;
    ciB.type = ivm::IVInterface::InterfaceType::Provided;
    auto pIfaceB = ivm::IVInterface::createIface(ciB);
    ive::IVInterfaceGraphicsItem itemB(pIfaceB);

    ivm::IVConnection ivConnection(pIfaceA, pIfaceB);
    ive::IVConnectionGraphicsItem connectionItem(&ivConnection, &itemA, &itemB);

    checkItem(&connectionItem, ive::ZOrder.Connection);
    delete ivFunctionA;
    delete ivFunctionB;
}

void tst_ItemZOrder::testItem_ConnectionGroup()
{
    ivm::IVFunction *ivFunctionA = ivm::testutils::createFunction(QStringLiteral("TestFunctionA"));
    ivm::IVFunction *ivFunctionB = ivm::testutils::createFunction(QStringLiteral("TestFunctionB"));

    ivm::IVInterface::CreationInfo ciA;
    ciA.function = ivFunctionA;
    ciA.type = ivm::IVInterface::InterfaceType::Provided;
    auto pIfaceA = new ivm::IVInterfaceGroup(ciA);
    ive::IVInterfaceGroupGraphicsItem itemA(pIfaceA);

    ivm::IVInterface::CreationInfo ciB;
    ciB.function = ivFunctionA;
    ciB.type = ivm::IVInterface::InterfaceType::Provided;
    auto pIfaceB = new ivm::IVInterfaceGroup(ciB);
    ive::IVInterfaceGroupGraphicsItem itemB(pIfaceB);

    std::unique_ptr<ivm::IVConnection> ivConnection { new ivm::IVConnection(pIfaceA, pIfaceB) };

    ivm::IVConnectionGroup ivConnectionGroup(
            QStringLiteral("TestConnectionGroup"), pIfaceA, pIfaceB, { ivConnection.get() });

    ive::IVConnectionGroupGraphicsItem connectionItem(&ivConnectionGroup, &itemA, &itemB);

    checkItem(&connectionItem, ive::ZOrder.Connection);
    ivConnectionGroup.removeConnection(ivConnection.get());
    QVERIFY(ivConnectionGroup.groupedConnections().isEmpty());
    delete ivFunctionA;
    delete ivFunctionB;
}

void tst_ItemZOrder::testItem_ConnectionLayer()
{
    auto *layer = ivm::testutils::createConnectionLayer();
    ++m_itemTypesTested;
    QVERIFY(layer->name().compare("default") == 0);
    delete layer;
}

void tst_ItemZOrder::testItem_CheckCoverage()
{
    static const QMetaEnum me = QMetaEnum::fromType<ivm::IVObject::Type>();
    static const int totalTypesCount = me.keyCount() - 1; // exculde IVObject::Type::Unknown

    QCOMPARE(m_itemTypesTested, totalTypesCount);
}

QTEST_MAIN(tst_ItemZOrder)

#include "tst_itemzorder.moc"
