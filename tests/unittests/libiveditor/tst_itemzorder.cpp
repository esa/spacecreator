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

#include "baseitems/common/ivutils.h"
#include "interface/ivcommentgraphicsitem.h"
#include "interface/ivconnectiongraphicsitem.h"
#include "interface/ivconnectiongroupgraphicsitem.h"
#include "interface/ivfunctiongraphicsitem.h"
#include "interface/ivfunctiontypegraphicsitem.h"
#include "interface/ivinterfacegraphicsitem.h"
#include "interface/ivinterfacegroupgraphicsitem.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivinterfacegroup.h"
#include "ivobject.h"
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
    ivm::IVFunctionType ivObject(QStringLiteral("TestFunctionType"));
    ive::IVFunctionTypeGraphicsItem item(&ivObject);

    checkItem(&item, ive::ZOrder.Function);
}

void tst_ItemZOrder::testItem_Function()
{
    ivm::IVFunction ivObject(QStringLiteral("TestFunction"));
    ive::IVFunctionGraphicsItem item(&ivObject);

    checkItem(&item, ive::ZOrder.Function);
}

void tst_ItemZOrder::testItem_Comment()
{
    ivm::IVComment ivObject(QStringLiteral("TestComment"));
    ive::IVCommentGraphicsItem item(&ivObject);

    checkItem(&item, ive::ZOrder.Comment);
}

void tst_ItemZOrder::testItem_InterfaceGroup()
{
    ivm::IVFunction ivFunction(QStringLiteral("TestFunction"));
    ivm::IVInterface::CreationInfo ci;
    ci.function = &ivFunction;
    ci.type = ivm::IVInterface::InterfaceType::Grouped;
    std::unique_ptr<ivm::IVInterfaceGroup> pIface =
            std::unique_ptr<ivm::IVInterfaceGroup>(new ivm::IVInterfaceGroup(ci));
    ive::IVInterfaceGroupGraphicsItem item(pIface.get());

    checkItem(&item, ive::ZOrder.Interface);
}

void tst_ItemZOrder::testItem_RequiredInterface()
{
    ivm::IVFunction ivFunction(QStringLiteral("TestFunction"));
    ivm::IVInterface::CreationInfo ci;
    ci.function = &ivFunction;
    ci.type = ivm::IVInterface::InterfaceType::Required;
    std::unique_ptr<ivm::IVInterface> pIface = std::unique_ptr<ivm::IVInterface>(ivm::IVInterface::createIface(ci));
    ive::IVInterfaceGraphicsItem item(pIface.get());

    checkItem(&item, ive::ZOrder.Interface);
}

void tst_ItemZOrder::testItem_ProvidedInterface()
{
    ivm::IVFunction ivFunction(QStringLiteral("TestFunction"));
    ivm::IVInterface::CreationInfo ci;
    ci.function = &ivFunction;
    ci.type = ivm::IVInterface::InterfaceType::Provided;
    std::unique_ptr<ivm::IVInterface> pIface = std::unique_ptr<ivm::IVInterface>(ivm::IVInterface::createIface(ci));
    ive::IVInterfaceGraphicsItem item(pIface.get());

    checkItem(&item, ive::ZOrder.Interface);
}

void tst_ItemZOrder::testItem_Connection()
{
    ivm::IVFunction ivFunctionA(QStringLiteral("TestFunctionA"));
    ivm::IVFunction ivFunctionB(QStringLiteral("TestFunctionB"));

    ivm::IVInterface::CreationInfo ciA;
    ciA.function = &ivFunctionA;
    ciA.type = ivm::IVInterface::InterfaceType::Provided;
    std::unique_ptr<ivm::IVInterface> pIfaceA = std::unique_ptr<ivm::IVInterface>(ivm::IVInterface::createIface(ciA));
    ive::IVInterfaceGraphicsItem itemA(pIfaceA.get());

    ivm::IVInterface::CreationInfo ciB;
    ciB.function = &ivFunctionA;
    ciB.type = ivm::IVInterface::InterfaceType::Provided;
    std::unique_ptr<ivm::IVInterface> pIfaceB = std::unique_ptr<ivm::IVInterface>(ivm::IVInterface::createIface(ciB));
    ive::IVInterfaceGraphicsItem itemB(pIfaceB.get());

    ivm::IVConnection ivConnection(pIfaceA.get(), pIfaceB.get());
    ive::IVConnectionGraphicsItem connectionItem(&ivConnection, &itemA, &itemB);

    checkItem(&connectionItem, ive::ZOrder.Connection);
}

void tst_ItemZOrder::testItem_ConnectionGroup()
{
    ivm::IVFunction ivFunctionA(QStringLiteral("TestFunctionA"));
    ivm::IVFunction ivFunctionB(QStringLiteral("TestFunctionB"));

    ivm::IVInterface::CreationInfo ciA;
    ciA.function = &ivFunctionA;
    ciA.type = ivm::IVInterface::InterfaceType::Provided;
    std::unique_ptr<ivm::IVInterfaceGroup> pIfaceA = std::make_unique<ivm::IVInterfaceGroup>(ciA);
    ive::IVInterfaceGroupGraphicsItem itemA(pIfaceA.get());

    ivm::IVInterface::CreationInfo ciB;
    ciB.function = &ivFunctionA;
    ciB.type = ivm::IVInterface::InterfaceType::Provided;
    std::unique_ptr<ivm::IVInterfaceGroup> pIfaceB = std::make_unique<ivm::IVInterfaceGroup>(ciB);
    ive::IVInterfaceGroupGraphicsItem itemB(pIfaceB.get());

    std::unique_ptr<ivm::IVConnection> ivConnection { new ivm::IVConnection(pIfaceA.get(), pIfaceB.get()) };

    ivm::IVConnectionGroup ivConnectionGroup(
            QStringLiteral("TestConnectionGroup"), pIfaceA.get(), pIfaceB.get(), { ivConnection.get() });

    ive::IVConnectionGroupGraphicsItem connectionItem(&ivConnectionGroup, &itemA, &itemB);

    checkItem(&connectionItem, ive::ZOrder.Connection);
    ivConnectionGroup.removeConnection(ivConnection.get());
    QVERIFY(ivConnectionGroup.groupedConnections().isEmpty());
}

void tst_ItemZOrder::testItem_CheckCoverage()
{
    static const QMetaEnum me = QMetaEnum::fromType<ivm::IVObject::Type>();
    static const int totalTypesCount = me.keyCount() - 1; // exculde IVObject::Type::Unknown

    QCOMPARE(m_itemTypesTested, totalTypesCount);
}

QTEST_MAIN(tst_ItemZOrder)

#include "tst_itemzorder.moc"
