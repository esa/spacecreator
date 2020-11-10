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

#include "aadlobject.h"
#include "aadlobjectcomment.h"
#include "aadlobjectconnection.h"
#include "aadlobjectconnectiongroup.h"
#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"
#include "aadlobjectifacegroup.h"
#include "baseitems/common/aadlutils.h"
#include "baseitems/interactiveobject.h"
#include "interface/aadlcommentgraphicsitem.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlconnectiongroupgraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlfunctiontypegraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"
#include "interface/aadlinterfacegroupgraphicsitem.h"

#include <QMetaEnum>
#include <QObject>
#include <QtTest>
#include <memory>

class tst_ItemZOrder : public QObject
{
    Q_OBJECT
private:
    void checkItem(aadlinterface::InteractiveObject *item, const qreal expectedZ);

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
    m_itemTypesTested = 0;
}

void tst_ItemZOrder::cleanupTestCase()
{
    QStandardPaths::setTestModeEnabled(false);
}

void tst_ItemZOrder::checkItem(aadlinterface::InteractiveObject *item, const qreal expectedZ)
{
    if (!item || !item->aadlObject() || item->aadlObject()->aadlType() == aadl::AADLObject::Type::Unknown)
        return;

    ++m_itemTypesTested;

    item->setSelected(false);
    QVERIFY(item->isSelected() == false);
    QCOMPARE(aadlinterface::itemLevel(item->aadlObject(), item->isSelected()), expectedZ);

    item->setSelected(true);
    QVERIFY(item->isSelected() == true);
    QCOMPARE(aadlinterface::itemLevel(item->aadlObject(), item->isSelected()), aadlinterface::ZOrder.Selected);
}

void tst_ItemZOrder::testItem_FunctionType()
{
    aadl::AADLObjectFunctionType aadlObject(QStringLiteral("TestFunctionType"));
    aadlinterface::AADLFunctionTypeGraphicsItem item(&aadlObject);

    checkItem(&item, aadlinterface::ZOrder.Function);
}

void tst_ItemZOrder::testItem_Function()
{
    aadl::AADLObjectFunction aadlObject(QStringLiteral("TestFunction"));
    aadlinterface::AADLFunctionGraphicsItem item(&aadlObject);

    checkItem(&item, aadlinterface::ZOrder.Function);
}

void tst_ItemZOrder::testItem_Comment()
{
    aadl::AADLObjectComment aadlObject(QStringLiteral("TestComment"));
    aadlinterface::AADLCommentGraphicsItem item(&aadlObject);

    checkItem(&item, aadlinterface::ZOrder.Comment);
}

void tst_ItemZOrder::testItem_InterfaceGroup()
{
    aadl::AADLObjectFunction aadlFunction(QStringLiteral("TestFunction"));
    aadl::AADLObjectIface::CreationInfo ci;
    ci.function = &aadlFunction;
    ci.type = aadl::AADLObjectIface::IfaceType::Grouped;
    std::unique_ptr<aadl::AADLObjectIfaceGroup> pIface =
            std::unique_ptr<aadl::AADLObjectIfaceGroup>(new aadl::AADLObjectIfaceGroup(ci));
    aadlinterface::AADLInterfaceGroupGraphicsItem item(pIface.get());

    checkItem(&item, aadlinterface::ZOrder.Interface);
}

void tst_ItemZOrder::testItem_RequiredInterface()
{
    aadl::AADLObjectFunction aadlFunction(QStringLiteral("TestFunction"));
    aadl::AADLObjectIface::CreationInfo ci;
    ci.function = &aadlFunction;
    ci.type = aadl::AADLObjectIface::IfaceType::Required;
    std::unique_ptr<aadl::AADLObjectIface> pIface =
            std::unique_ptr<aadl::AADLObjectIface>(aadl::AADLObjectIface::createIface(ci));
    aadlinterface::AADLInterfaceGraphicsItem item(pIface.get());

    checkItem(&item, aadlinterface::ZOrder.Interface);
}

void tst_ItemZOrder::testItem_ProvidedInterface()
{
    aadl::AADLObjectFunction aadlFunction(QStringLiteral("TestFunction"));
    aadl::AADLObjectIface::CreationInfo ci;
    ci.function = &aadlFunction;
    ci.type = aadl::AADLObjectIface::IfaceType::Provided;
    std::unique_ptr<aadl::AADLObjectIface> pIface =
            std::unique_ptr<aadl::AADLObjectIface>(aadl::AADLObjectIface::createIface(ci));
    aadlinterface::AADLInterfaceGraphicsItem item(pIface.get());

    checkItem(&item, aadlinterface::ZOrder.Interface);
}

void tst_ItemZOrder::testItem_Connection()
{
    aadl::AADLObjectFunction aadlFunctionA(QStringLiteral("TestFunctionA"));
    aadl::AADLObjectFunction aadlFunctionB(QStringLiteral("TestFunctionB"));

    aadl::AADLObjectIface::CreationInfo ciA;
    ciA.function = &aadlFunctionA;
    ciA.type = aadl::AADLObjectIface::IfaceType::Provided;
    std::unique_ptr<aadl::AADLObjectIface> pIfaceA =
            std::unique_ptr<aadl::AADLObjectIface>(aadl::AADLObjectIface::createIface(ciA));
    aadlinterface::AADLInterfaceGraphicsItem itemA(pIfaceA.get());

    aadl::AADLObjectIface::CreationInfo ciB;
    ciB.function = &aadlFunctionA;
    ciB.type = aadl::AADLObjectIface::IfaceType::Provided;
    std::unique_ptr<aadl::AADLObjectIface> pIfaceB =
            std::unique_ptr<aadl::AADLObjectIface>(aadl::AADLObjectIface::createIface(ciB));
    aadlinterface::AADLInterfaceGraphicsItem itemB(pIfaceB.get());

    aadl::AADLObjectConnection aadlConnection(pIfaceA.get(), pIfaceB.get());
    aadlinterface::AADLConnectionGraphicsItem connectionItem(&aadlConnection, &itemA, &itemB);

    checkItem(&connectionItem, aadlinterface::ZOrder.Connection);
}

void tst_ItemZOrder::testItem_ConnectionGroup()
{
    aadl::AADLObjectFunction aadlFunctionA(QStringLiteral("TestFunctionA"));
    aadl::AADLObjectFunction aadlFunctionB(QStringLiteral("TestFunctionB"));

    aadl::AADLObjectIface::CreationInfo ciA;
    ciA.function = &aadlFunctionA;
    ciA.type = aadl::AADLObjectIface::IfaceType::Provided;
    std::unique_ptr<aadl::AADLObjectIfaceGroup> pIfaceA = std::make_unique<aadl::AADLObjectIfaceGroup>(ciA);
    aadlinterface::AADLInterfaceGroupGraphicsItem itemA(pIfaceA.get());

    aadl::AADLObjectIface::CreationInfo ciB;
    ciB.function = &aadlFunctionA;
    ciB.type = aadl::AADLObjectIface::IfaceType::Provided;
    std::unique_ptr<aadl::AADLObjectIfaceGroup> pIfaceB = std::make_unique<aadl::AADLObjectIfaceGroup>(ciB);
    aadlinterface::AADLInterfaceGroupGraphicsItem itemB(pIfaceB.get());

    std::unique_ptr<aadl::AADLObjectConnection> aadlConnection { new aadl::AADLObjectConnection(
            pIfaceA.get(), pIfaceB.get()) };

    aadl::AADLObjectConnectionGroup aadlConnectionGroup(
            QStringLiteral("TestConnectionGroup"), pIfaceA.get(), pIfaceB.get(), { aadlConnection.get() });

    aadlinterface::AADLConnectionGroupGraphicsItem connectionItem(&aadlConnectionGroup, &itemA, &itemB);

    checkItem(&connectionItem, aadlinterface::ZOrder.Connection);
    aadlConnectionGroup.removeConnection(aadlConnection.get());
    QVERIFY(aadlConnectionGroup.groupedConnections().isEmpty());
}

void tst_ItemZOrder::testItem_CheckCoverage()
{
    static const QMetaEnum me = QMetaEnum::fromType<aadl::AADLObject::Type>();
    static const int totalTypesCount = me.keyCount() - 1; // exculde AADLObject::Type::Unknown

    QCOMPARE(m_itemTypesTested, totalTypesCount);
}

QTEST_MAIN(tst_ItemZOrder)

#include "tst_itemzorder.moc"
