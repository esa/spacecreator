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
#include "tab_aadl/aadlobjectcontainer.h"
#include "tab_aadl/aadlobjectfunction.h"
#include "tab_aadl/aadlobjectiface.h"

#include <QtTest>

class tst_AADLObjectContainer : public QObject
{
    Q_OBJECT

private slots:
    void testAadlType();
    void testRequiredInterfacesManagement();
    void testProvidedInterfacesManagement();
    void testCommonInterfacesManagement();

    void testChildrenManagementFunction();
    void testChildrenManagementFunctionType();
    void testChildrenManagementMixed();

private:
    void testChildrenManagement(taste3::aadl::AADLObjectContainer *obj,
                                const QVector<taste3::aadl::AADLObject *> &children);
};

void tst_AADLObjectContainer::testAadlType()
{
    using namespace taste3::aadl;
    AADLObjectContainer obj;

    QCOMPARE(obj.aadlType(), AADLObject::AADLObjectType::AADLFunctionType);
}

void tst_AADLObjectContainer::testRequiredInterfacesManagement()
{
    using namespace taste3::aadl;
    AADLObjectContainer obj;

    QCOMPARE(obj.ris().size(), 0);

    static constexpr AADLObjectIface::IfaceType ifaceType = AADLObjectIface::IfaceType::Required;
    static constexpr int ifacesCount { 10 };
    for (int i = 0; i < ifacesCount; ++i) {
        AADLObjectIface *ri = new AADLObjectIface(ifaceType, QString("test required iface #%1").arg(i), &obj);
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

void tst_AADLObjectContainer::testProvidedInterfacesManagement()
{
    using namespace taste3::aadl;
    AADLObjectContainer obj;

    QCOMPARE(obj.ris().size(), 0);

    static constexpr AADLObjectIface::IfaceType ifaceType = AADLObjectIface::IfaceType::Provided;
    static constexpr int ifacesCount { 10 };
    for (int i = 0; i < ifacesCount; ++i) {
        AADLObjectIface *pi = new AADLObjectIface(ifaceType, QString("test provided iface #%1").arg(i), &obj);
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

void tst_AADLObjectContainer::testCommonInterfacesManagement()
{
    using namespace taste3::aadl;
    AADLObjectContainer obj;

    QCOMPARE(obj.ris().size(), 0);

    static constexpr int ifacesCountHalf { 5 };
    static constexpr AADLObjectIface::IfaceType itProvided = AADLObjectIface::IfaceType::Provided;
    static constexpr AADLObjectIface::IfaceType itRequired = AADLObjectIface::IfaceType::Required;

    QVector<AADLObjectIface *> ifaces;
    for (int i = 0; i < ifacesCountHalf; ++i) {
        ifaces << new AADLObjectIface(itProvided, QString("test provided iface #%1").arg(i), &obj);
        ifaces << new AADLObjectIface(itRequired, QString("test required iface #%1").arg(i), &obj);
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

void tst_AADLObjectContainer::testChildrenManagementFunction()
{
    using namespace taste3::aadl;
    AADLObjectContainer obj;

    static constexpr AADLObjectIface::IfaceType itProvided = AADLObjectIface::IfaceType::Provided;
    static constexpr AADLObjectIface::IfaceType itRequired = AADLObjectIface::IfaceType::Required;

    AADLObjectFunction fn0("Fn0", &obj);

    AADLObjectFunction fn1("Fn1", &obj);
    fn1.addInterface(new AADLObjectIface(itProvided, QString("test provided iface #%1").arg(fn1.pis().size()), &fn1));

    AADLObjectFunction fn2("Fn2", &obj);
    fn2.addInterface(new AADLObjectIface(itRequired, QString("test required iface #%1").arg(fn2.ris().size()), &fn2));

    AADLObjectFunction fn3("Fn3", &obj);
    fn3.addInterface(new AADLObjectIface(itRequired, QString("test required iface #%1").arg(fn3.ris().size()), &fn3));
    fn3.addInterface(new AADLObjectIface(itProvided, QString("test provided iface #%1").arg(fn3.pis().size()), &fn3));

    const QVector<AADLObject *> functions { &fn0, &fn1, &fn2, &fn3 };
    testChildrenManagement(&obj, functions);
}

void tst_AADLObjectContainer::testChildrenManagement(taste3::aadl::AADLObjectContainer *obj,
                                                     const QVector<taste3::aadl::AADLObject *> &children)
{
    QCOMPARE(obj->children().size(), 0);

    for (auto fn : children) {
        const int prevCount = obj->children().size();
        const bool ok = obj->addChild(fn);
        QVERIFY(ok);
        const int currCount = obj->children().size();
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

void tst_AADLObjectContainer::testChildrenManagementFunctionType()
{
    using namespace taste3::aadl;
    AADLObjectContainer obj;

    static constexpr AADLObjectIface::IfaceType itProvided = AADLObjectIface::IfaceType::Provided;
    static constexpr AADLObjectIface::IfaceType itRequired = AADLObjectIface::IfaceType::Required;

    AADLObjectContainer fnType0("FnType0", &obj);

    AADLObjectContainer fnType1("FnType1", &obj);
    fnType1.addInterface(
            new AADLObjectIface(itProvided, QString("test provided iface #%1").arg(fnType1.pis().size()), &fnType1));

    AADLObjectContainer fnType2("FnType2", &obj);
    fnType2.addInterface(
            new AADLObjectIface(itRequired, QString("test required iface #%1").arg(fnType2.ris().size()), &fnType2));

    AADLObjectContainer fnType3("FnType3", &obj);
    fnType3.addInterface(
            new AADLObjectIface(itRequired, QString("test required iface #%1").arg(fnType3.ris().size()), &fnType3));
    fnType3.addInterface(
            new AADLObjectIface(itProvided, QString("test provided iface #%1").arg(fnType3.pis().size()), &fnType3));

    const QVector<AADLObject *> functionTypes { &fnType0, &fnType1, &fnType2, &fnType3 };
    testChildrenManagement(&obj, functionTypes);
}

void tst_AADLObjectContainer::testChildrenManagementMixed()
{
    using namespace taste3::aadl;
    AADLObjectContainer obj;

    static constexpr AADLObjectIface::IfaceType itProvided = AADLObjectIface::IfaceType::Provided;
    static constexpr AADLObjectIface::IfaceType itRequired = AADLObjectIface::IfaceType::Required;

    AADLObjectFunction fn0("Fn0", &obj);

    AADLObjectFunction fn1("Fn1", &obj);
    fn1.addInterface(new AADLObjectIface(itProvided, QString("test provided iface #%1").arg(fn1.pis().size()), &fn1));

    AADLObjectFunction fn2("Fn2", &obj);
    fn2.addInterface(new AADLObjectIface(itRequired, QString("test required iface #%1").arg(fn2.ris().size()), &fn2));

    AADLObjectFunction fn3("Fn3", &obj);
    fn3.addInterface(new AADLObjectIface(itRequired, QString("test required iface #%1").arg(fn3.ris().size()), &fn3));
    fn3.addInterface(new AADLObjectIface(itProvided, QString("test provided iface #%1").arg(fn3.pis().size()), &fn3));

    AADLObjectContainer fnType0("FnType0", &obj);

    AADLObjectContainer fnType1("FnType1", &obj);
    fnType1.addInterface(
            new AADLObjectIface(itProvided, QString("test provided iface #%1").arg(fnType1.pis().size()), &fnType1));

    AADLObjectContainer fnType2("FnType2", &obj);
    fnType2.addInterface(
            new AADLObjectIface(itRequired, QString("test required iface #%1").arg(fnType2.ris().size()), &fnType2));

    AADLObjectContainer fnType3("FnType3", &obj);
    fnType3.addInterface(
            new AADLObjectIface(itRequired, QString("test required iface #%1").arg(fnType3.ris().size()), &fnType3));
    fnType3.addInterface(
            new AADLObjectIface(itProvided, QString("test provided iface #%1").arg(fnType3.pis().size()), &fnType3));

    const QVector<AADLObject *> children { &fnType0, &fn0, &fnType1, &fn1, &fnType2, &fn3, &fnType3 };
    testChildrenManagement(&obj, children);
}

QTEST_APPLESS_MAIN(tst_AADLObjectContainer)

#include "tst_aadlobjectcontainer.moc"
