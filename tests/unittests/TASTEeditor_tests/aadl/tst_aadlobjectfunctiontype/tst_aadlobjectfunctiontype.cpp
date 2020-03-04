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

#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectfunction.h"
#include "tab_aadl/aadlobjectfunctiontype.h"
#include "tab_aadl/aadlobjectiface.h"

#include <QDebug>
#include <QtTest>
#include <testutils.h>

class tst_AADLObjectFunctionType : public QObject
{
    Q_OBJECT

private slots:
    void testAadlType();
    void testRequiredInterfacesManagement();
    void testProvidedInterfacesManagement();
    void testCommonInterfacesManagement();

    void testChildrenManagementFunction();
    void testChildrenManagementContainer();
    void testChildrenManagementMixed();

private:
    void testChildrenManagement(taste3::aadl::AADLObjectFunctionType *obj,
                                const QVector<taste3::aadl::AADLObject *> &children, bool addedOnCreation = false);
};

void tst_AADLObjectFunctionType::testAadlType()
{
    using namespace taste3::aadl;
    AADLObjectFunctionType obj;

    QCOMPARE(obj.aadlType(), AADLObject::AADLObjectType::AADLFunctionType);
}

void tst_AADLObjectFunctionType::testRequiredInterfacesManagement()
{
    using namespace taste3::aadl;
    AADLObjectFunctionType obj;

    QCOMPARE(obj.ris().size(), 0);

    static constexpr AADLObjectIface::IfaceType ifaceType = AADLObjectIface::IfaceType::Required;
    static constexpr int ifacesCount { 10 };
    AADLObjectIface::CreationInfo ciRI = TASTEtest::init(ifaceType, &obj);
    for (int i = 0; i < ifacesCount; ++i) {
        ciRI.name = QString("test required iface #%1").arg(i);
        AADLObjectIface *ri = AADLObjectIface::createIface(ciRI);
        const bool ok = obj.addRI(ri);
        QVERIFY(ok);
        QCOMPARE(obj.ris().size(), i + 1);
    }

    for (int i = 0; i < ifacesCount; ++i) {
        AADLObjectIface *ri = obj.ris().first();
        const bool ok = obj.removeRI(ri);
        QVERIFY(ok);
        QCOMPARE(obj.ris().size(), ifacesCount - i - 1);
        delete ri;
    }
}

void tst_AADLObjectFunctionType::testProvidedInterfacesManagement()
{
    using namespace taste3::aadl;
    AADLObjectFunctionType obj;

    QCOMPARE(obj.ris().size(), 0);

    static constexpr AADLObjectIface::IfaceType ifaceType = AADLObjectIface::IfaceType::Provided;
    static constexpr int ifacesCount { 10 };
    AADLObjectIface::CreationInfo ciPI = TASTEtest::init(ifaceType, &obj);
    for (int i = 0; i < ifacesCount; ++i) {
        ciPI.name = QString("test provided iface #%1").arg(i);
        AADLObjectIface *pi = AADLObjectIface::createIface(ciPI);
        const bool ok = obj.addPI(pi);
        QVERIFY(ok);
        QCOMPARE(obj.pis().size(), i + 1);
    }

    QCOMPARE(obj.pis().size(), ifacesCount);

    for (int i = 0; i < ifacesCount; ++i) {
        AADLObjectIface *pi = obj.pis().first();
        const bool ok = obj.removePI(pi);
        QVERIFY(ok);
        QCOMPARE(obj.pis().size(), ifacesCount - i - 1);
        delete pi;
    }

    QCOMPARE(obj.pis().size(), 0);
}

void tst_AADLObjectFunctionType::testCommonInterfacesManagement()
{
    using namespace taste3::aadl;
    AADLObjectFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    static constexpr int ifacesCountHalf { 5 };

    QVector<AADLObjectIface *> ifaces;
    for (int i = 0; i < ifacesCountHalf; ++i) {
        AADLObjectIface::CreationInfo ci = TASTEtest::init(AADLObjectIface::IfaceType::Provided, &obj);
        ci.name = QString("test provided iface #%1").arg(i);
        ifaces << AADLObjectIface::createIface(ci);

        ci = TASTEtest::init(AADLObjectIface::IfaceType::Required, &obj);
        ci.name = QString("test required iface #%1").arg(i);
        ifaces << AADLObjectIface::createIface(ci);
    }

    QCOMPARE(ifaces.size(), ifacesCountHalf * 2);

    for (auto iface : ifaces) {
        const bool ok = obj.addInterface(iface);
        QVERIFY(ok);
    }

    QCOMPARE(obj.ris().size(), ifacesCountHalf);
    QCOMPARE(obj.pis().size(), ifacesCountHalf);

    while (ifaces.size()) {
        if (AADLObjectIface *iface = ifaces.takeLast()) {
            const int prevCount = iface->isProvided() ? obj.pis().size() : obj.ris().size();
            const bool ok = obj.removeInterface(iface);
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
    using namespace taste3::aadl;
    AADLObjectFunctionType obj;

    static constexpr AADLObjectIface::IfaceType itProvided = AADLObjectIface::IfaceType::Provided;
    static constexpr AADLObjectIface::IfaceType itRequired = AADLObjectIface::IfaceType::Required;

    AADLObjectFunction fn0("Fn0", &obj);
    AADLObjectFunction fn1("Fn1", &obj);
    fn1.addInterface(TASTEtest::createIface(&fn1, itProvided));
    AADLObjectFunction fn2("Fn2", &obj);
    fn2.addInterface(TASTEtest::createIface(&fn2, itRequired));
    AADLObjectFunction fn3("Fn3", &obj);
    fn3.addInterface(TASTEtest::createIface(&fn3, itRequired));
    fn3.addInterface(TASTEtest::createIface(&fn3, itProvided));

    const QVector<AADLObject *> functions { &fn0, &fn1, &fn2, &fn3 };
    testChildrenManagement(&obj, functions, functions.size());
}

void tst_AADLObjectFunctionType::testChildrenManagement(taste3::aadl::AADLObjectFunctionType *obj,
                                                        const QVector<taste3::aadl::AADLObject *> &children,
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
    using namespace taste3::aadl;
    AADLObjectFunctionType obj;

    static constexpr AADLObjectIface::IfaceType itProvided = AADLObjectIface::IfaceType::Provided;
    static constexpr AADLObjectIface::IfaceType itRequired = AADLObjectIface::IfaceType::Required;

    AADLObjectFunctionType fnType0("FnType0", &obj);
    obj.addChild(&fnType0);

    AADLObjectFunctionType fnType1("FnType1", &obj);
    obj.addChild(&fnType1);
    fnType1.addInterface(TASTEtest::createIface(&fnType1, itProvided));

    AADLObjectFunctionType fnType2("FnType2", &obj);
    obj.addChild(&fnType2);
    fnType2.addInterface(TASTEtest::createIface(&fnType2, itRequired));

    AADLObjectFunctionType fnType3("FnType3", &obj);
    obj.addChild(&fnType3);
    fnType3.addInterface(TASTEtest::createIface(&fnType3, itRequired));
    fnType3.addInterface(TASTEtest::createIface(&fnType3, itProvided));

    const QVector<AADLObject *> functionTypes { &fnType0, &fnType1, &fnType2, &fnType3 };
    testChildrenManagement(&obj, functionTypes, functionTypes.size());
}

void tst_AADLObjectFunctionType::testChildrenManagementMixed()
{
    using namespace taste3::aadl;

    static constexpr AADLObjectIface::IfaceType itProvided = AADLObjectIface::IfaceType::Provided;
    static constexpr AADLObjectIface::IfaceType itRequired = AADLObjectIface::IfaceType::Required;

    AADLObjectFunctionType obj;

    AADLObjectFunction fn0("Fn0", &obj);
    AADLObjectFunction fn1("Fn1", &obj);
    fn1.addInterface(TASTEtest::createIface(&fn1, itProvided));

    AADLObjectFunction fn2("Fn2", &obj);
    fn1.addInterface(TASTEtest::createIface(&fn2, itRequired));

    AADLObjectFunction fn3("Fn3", &obj);
    fn3.addInterface(TASTEtest::createIface(&fn3, itRequired));
    fn3.addInterface(TASTEtest::createIface(&fn3, itProvided));

    AADLObjectFunctionType fnType0("FnType0", &obj);
    AADLObjectFunctionType fnType1("FnType1", &obj);
    fnType1.addInterface(TASTEtest::createIface(&fnType1, itProvided));

    AADLObjectFunctionType fnType2("FnType2", &obj);
    fnType2.addInterface(TASTEtest::createIface(&fnType2, itRequired));

    AADLObjectFunctionType fnType3("FnType3", &obj);
    fnType3.addInterface(TASTEtest::createIface(&fnType3, itRequired));
    fnType3.addInterface(TASTEtest::createIface(&fnType3, itProvided));

    const QVector<AADLObject *> children { &fnType0, &fn0, &fnType1, &fn1, &fnType2, &fn3, &fnType3, &fn2 };
    testChildrenManagement(&obj, children, true);
}

QTEST_APPLESS_MAIN(tst_AADLObjectFunctionType)

#include "tst_aadlobjectfunctiontype.moc"
