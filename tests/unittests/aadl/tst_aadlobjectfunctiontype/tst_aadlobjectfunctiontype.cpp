/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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
#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"
#include "aadltestutils.h"

#include <QDebug>
#include <QtTest>

class tst_AADLObjectFunctionType : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testAadlType();
    void testRequiredInterfacesManagement();
    void testProvidedInterfacesManagement();
    void testCommonInterfacesManagement();

    void testChildrenManagementFunction();
    void testChildrenManagementContainer();
    void testChildrenManagementMixed();

private:
    void testChildrenManagement(aadl::AADLObjectFunctionType *obj,
                                const QVector<aadl::AADLObject *> &children, bool addedOnCreation = false);
};

void tst_AADLObjectFunctionType::testAadlType()
{
    aadl::AADLObjectFunctionType obj;

    QCOMPARE(obj.aadlType(), aadl::AADLObject::Type::FunctionType);
}

void tst_AADLObjectFunctionType::testRequiredInterfacesManagement()
{
    aadl::AADLObjectFunctionType obj;

    QCOMPARE(obj.ris().size(), 0);

    auto ifaceType = aadl::AADLObjectIface::IfaceType::Required;
    const int ifacesCount = 10;
    auto ciRI = aadl::testutils::init(ifaceType, &obj);
    for (int i = 0; i < ifacesCount; ++i) {
        ciRI.name = QString("test required iface #%1").arg(i);
        auto ri = aadl::AADLObjectIface::createIface(ciRI);
        const bool ok = obj.addChild(ri);
        QVERIFY(ok);
        QCOMPARE(obj.ris().size(), i + 1);
    }

    for (int i = 0; i < ifacesCount; ++i) {
        auto ri = obj.ris().first();
        const bool ok = obj.removeChild(ri);
        QVERIFY(ok);
        QCOMPARE(obj.ris().size(), ifacesCount - i - 1);
        delete ri;
    }
}

void tst_AADLObjectFunctionType::testProvidedInterfacesManagement()
{
    aadl::AADLObjectFunctionType obj;

    QCOMPARE(obj.ris().size(), 0);

    auto ifaceType = aadl::AADLObjectIface::IfaceType::Provided;
    const int ifacesCount = 10;
    auto ciPI = aadl::testutils::init(ifaceType, &obj);
    for (int i = 0; i < ifacesCount; ++i) {
        ciPI.name = QString("test provided iface #%1").arg(i);
        auto pi = aadl::AADLObjectIface::createIface(ciPI);
        const bool ok = obj.addChild(pi);
        QVERIFY(ok);
        QCOMPARE(obj.pis().size(), i + 1);
    }

    QCOMPARE(obj.pis().size(), ifacesCount);

    for (int i = 0; i < ifacesCount; ++i) {
        auto pi = obj.pis().first();
        const bool ok = obj.removeChild(pi);
        QVERIFY(ok);
        QCOMPARE(obj.pis().size(), ifacesCount - i - 1);
        delete pi;
    }

    QCOMPARE(obj.pis().size(), 0);
}

void tst_AADLObjectFunctionType::testCommonInterfacesManagement()
{
    aadl::AADLObjectFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    const int ifacesCountHalf = 5;

    QVector<aadl::AADLObjectIface *> ifaces;
    for (int i = 0; i < ifacesCountHalf; ++i) {
        auto ci = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Provided, &obj);
        ci.name = QString("test provided iface #%1").arg(i);
        ifaces << aadl::AADLObjectIface::createIface(ci);

        ci = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Required, &obj);
        ci.name = QString("test required iface #%1").arg(i);
        ifaces << aadl::AADLObjectIface::createIface(ci);
    }

    QCOMPARE(ifaces.size(), ifacesCountHalf * 2);

    for (auto iface : ifaces) {
        const bool ok = obj.addChild(iface);
        QVERIFY(ok);
    }

    QCOMPARE(obj.ris().size(), ifacesCountHalf);
    QCOMPARE(obj.pis().size(), ifacesCountHalf);

    while (ifaces.size()) {
        if (auto iface = ifaces.takeLast()) {
            const int prevCount = iface->isProvided() ? obj.pis().size() : obj.ris().size();
            const bool ok = obj.removeChild(iface);
            QVERIFY(ok);
            const int currCount = iface->isProvided() ? obj.pis().size() : obj.ris().size();
            QVERIFY(prevCount != currCount);
            delete iface;
        }
    }

    QCOMPARE(obj.pis().size(), 0);
    QCOMPARE(obj.ris().size(), 0);
}

void tst_AADLObjectFunctionType::testChildrenManagementFunction()
{
    aadl::AADLObjectFunctionType obj;

    auto itProvided = aadl::AADLObjectIface::IfaceType::Provided;
    auto itRequired = aadl::AADLObjectIface::IfaceType::Required;

    aadl::AADLObjectFunction fn0("Fn0", &obj);
    aadl::AADLObjectFunction fn1("Fn1", &obj);
    fn1.addChild(aadl::testutils::createIface(&fn1, itProvided));
    aadl::AADLObjectFunction fn2("Fn2", &obj);
    fn2.addChild(aadl::testutils::createIface(&fn2, itRequired));
    aadl::AADLObjectFunction fn3("Fn3", &obj);
    fn3.addChild(aadl::testutils::createIface(&fn3, itRequired));
    fn3.addChild(aadl::testutils::createIface(&fn3, itProvided));

    const QVector<aadl::AADLObject *> functions { &fn0, &fn1, &fn2, &fn3 };
    testChildrenManagement(&obj, functions);
}

void tst_AADLObjectFunctionType::testChildrenManagement(aadl::AADLObjectFunctionType *obj,
                                                        const QVector<aadl::AADLObject *> &children,
                                                        bool addedOnCreation)
{
    QCOMPARE(obj->children().size(), addedOnCreation ? children.size() : 0);

    for (auto fn : children) {
        const int prevCount = obj->children().size();
        const bool ok = obj->addChild(fn);
        if (addedOnCreation)
            QVERIFY(!ok);
        else
            QVERIFY(ok);
        const int currCount = obj->children().size();
        if (addedOnCreation)
            QVERIFY(prevCount == currCount);
        else
            QVERIFY(prevCount != currCount);
    }

    QCOMPARE(obj->children().size(), children.size());

    for (auto fn : children) {
        const int prevCount = obj->children().size();
        const bool ok = obj->addChild(fn);
        QVERIFY(!ok);
        const int currCount = obj->children().size();
        QVERIFY(prevCount == currCount);
    }

    QCOMPARE(obj->children().size(), children.size());

    for (auto fn : children) {
        const int prevCount = obj->children().size();
        const bool ok = obj->removeChild(fn);
        QVERIFY(ok);
        const int currCount = obj->children().size();
        QVERIFY(prevCount != currCount);
    }

    QCOMPARE(obj->children().size(), 0);
    const bool added = obj->addChild(nullptr);
    QVERIFY(!added);
    QCOMPARE(obj->children().size(), 0);
    const bool removed = obj->removeChild(nullptr);
    QVERIFY(!removed);
    QCOMPARE(obj->children().size(), 0);
}

void tst_AADLObjectFunctionType::testChildrenManagementContainer()
{
    aadl::AADLObjectFunctionType obj;

    auto itProvided = aadl::AADLObjectIface::IfaceType::Provided;
    auto itRequired = aadl::AADLObjectIface::IfaceType::Required;

    aadl::AADLObjectFunctionType fnType0("FnType0", &obj);
    obj.addChild(&fnType0);

    aadl::AADLObjectFunctionType fnType1("FnType1", &obj);
    obj.addChild(&fnType1);
    fnType1.addChild(aadl::testutils::createIface(&fnType1, itProvided));

    aadl::AADLObjectFunctionType fnType2("FnType2", &obj);
    obj.addChild(&fnType2);
    fnType2.addChild(aadl::testutils::createIface(&fnType2, itRequired));

    aadl::AADLObjectFunctionType fnType3("FnType3", &obj);
    obj.addChild(&fnType3);
    fnType3.addChild(aadl::testutils::createIface(&fnType3, itRequired));
    fnType3.addChild(aadl::testutils::createIface(&fnType3, itProvided));

    const QVector<aadl::AADLObject *> functionTypes { &fnType0, &fnType1, &fnType2, &fnType3 };
    testChildrenManagement(&obj, functionTypes, true);
}

void tst_AADLObjectFunctionType::testChildrenManagementMixed()
{
    auto itProvided = aadl::AADLObjectIface::IfaceType::Provided;
    auto itRequired = aadl::AADLObjectIface::IfaceType::Required;

    aadl::AADLObjectFunctionType obj;

    aadl::AADLObjectFunction fn0("Fn0", &obj);
    aadl::AADLObjectFunction fn1("Fn1", &obj);
    fn1.addChild(aadl::testutils::createIface(&fn1, itProvided));

    aadl::AADLObjectFunction fn2("Fn2", &obj);
    fn1.addChild(aadl::testutils::createIface(&fn2, itRequired));

    aadl::AADLObjectFunction fn3("Fn3", &obj);
    fn3.addChild(aadl::testutils::createIface(&fn3, itRequired));
    fn3.addChild(aadl::testutils::createIface(&fn3, itProvided));

    aadl::AADLObjectFunctionType fnType0("FnType0", &obj);
    aadl::AADLObjectFunctionType fnType1("FnType1", &obj);
    fnType1.addChild(aadl::testutils::createIface(&fnType1, itProvided));

    aadl::AADLObjectFunctionType fnType2("FnType2", &obj);
    fnType2.addChild(aadl::testutils::createIface(&fnType2, itRequired));

    aadl::AADLObjectFunctionType fnType3("FnType3", &obj);
    fnType3.addChild(aadl::testutils::createIface(&fnType3, itRequired));
    fnType3.addChild(aadl::testutils::createIface(&fnType3, itProvided));

    const QVector<aadl::AADLObject *> children { &fnType0, &fn0, &fnType1, &fn1, &fnType2, &fn3, &fnType3, &fn2 };
    testChildrenManagement(&obj, children);
}

QTEST_APPLESS_MAIN(tst_AADLObjectFunctionType)

#include "tst_aadlobjectfunctiontype.moc"
