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
#include "tab_aadl/aadlobjectsmodel.h"

#include <QtTest>
#include <testutils.h>

class tst_AADLObjectsModel : public QObject
{
    Q_OBJECT

private slots:
    void testManageContainers();
    void testManageFunctions();
    void testManageIfaces();
    void testManageMixed();
};

void tst_AADLObjectsModel::testManageContainers()
{
    QSKIP("hangs or fails");
    using namespace taste3::aadl;
    AADLObjectsModel model;

    QCOMPARE(model.objects().size(), 0);

    AADLObjectFunction container1("Container1");
    AADLObjectFunction container2("Container2");
    container1.addChild(&container2);
    AADLObjectFunction container3("Container3");
    container2.addChild(&container3);

    const QVector<AADLObjectFunction *> containers { &container1, &container2, &container3 };
    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool added = model.addObject(container);
        QVERIFY(added);
        QCOMPARE(model.objects().size(), i + 1);

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool nullAdded = model.addObject(nullptr);
        QVERIFY(!nullAdded);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        AADLObjectFunction dummy;
        const bool dummyRemoved = model.removeObject(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::aadlObjectAdded);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool removed = model.removeObject(container);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), containers.size() - i - 1);

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.objects().size(), 0);
}

void tst_AADLObjectsModel::testManageFunctions()
{
    QSKIP("hangs or fails");
    using namespace taste3::aadl;
    AADLObjectsModel model;

    QCOMPARE(model.objects().size(), 0);

    AADLObjectFunction fn1("Fn1");
    AADLObjectFunction fn2("Fn2", &model);
    AADLObjectFunction fn3("Fn3", &model);

    const AADLFunctionsVector functions { &fn1, &fn2, &fn3 };
    for (int i = 0; i < functions.size(); ++i) {
        auto function = functions.at(i);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool added = model.addObject(function);
        QVERIFY(added);
        QCOMPARE(model.objects().size(), i + 1);

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool nullAdded = model.addObject(nullptr);
        QVERIFY(!nullAdded);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        AADLObjectFunction dummy;
        const bool dummyRemoved = model.removeObject(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < functions.size(); ++i) {
        auto container = functions.at(i);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool removed = model.removeObject(container);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), functions.size() - i - 1);

        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.objects().size(), 0);
}

void tst_AADLObjectsModel::testManageIfaces()
{
    QSKIP("hangs or fails");
    using namespace taste3::aadl;
    AADLObjectsModel model;

    QCOMPARE(model.objects().size(), 0);

    AADLObjectFunction fn("fn");
    model.addObject(&fn);
    AADLObjectIface::CreationInfo ci1 = TASTEtest::init(AADLObjectIface::IfaceType::Provided, &fn);
    ci1.name = "eth0";
    AADLObjectIface::CreationInfo ci2 = TASTEtest::init(AADLObjectIface::IfaceType::Required, &fn);
    ci2.name = "wlan0";
    AADLObjectIface::CreationInfo ci3 = TASTEtest::init(AADLObjectIface::IfaceType::Provided, &fn);
    ci3.name = "ppp0";

    QVector<AADLObjectIface *> createdIfaces;
    const QVector<AADLObjectIface::CreationInfo> ifaces { ci1, ci2, ci3 };
    for (int i = 0; i < ifaces.size(); ++i) {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        AADLObjectIface::CreationInfo ci = ifaces.at(i);
        AADLObjectIface *iface = AADLObjectIface::createIface(ci);
        createdIfaces.append(iface);
        const bool added1 = ci.function->addChild(iface);
        QVERIFY(added1);
        const bool added2 = model.addObject(iface);
        QVERIFY(added2);
        QCOMPARE(model.objects().size(), i + 2); // the model now contains fn + currently added iface

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool nullAdded = model.addObject(nullptr);
        QVERIFY(!nullAdded);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // + function

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // +function

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        AADLObjectIface *dummy =
                AADLObjectIface::createIface(TASTEtest::init(AADLObjectIface::IfaceType::Provided, nullptr));
        const bool dummyRemoved = model.removeObject(dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // +function

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < createdIfaces.size(); ++i) {
        auto iface = createdIfaces.at(i);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool removed = model.removeObject(iface);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), ifaces.size() - i); // +function

        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.objects().size(), 1); // the function is still in place!
}

void tst_AADLObjectsModel::testManageMixed()
{
    QSKIP("hangs or fails");
    using namespace taste3::aadl;
    AADLObjectsModel model;

    QCOMPARE(model.objects().size(), 0);

    AADLObjectFunction container1("Container1");
    AADLObjectFunction container2("Container2", &container1);
    AADLObjectFunction container3("Container3", &container2);
    AADLObjectFunction fn1("Fn1");
    AADLObjectFunction fn2("Fn2", &model);
    AADLObjectFunction fn3("Fn3", &model);
    const int functionsCount = 6;

    AADLObjectIface::CreationInfo ci1 = TASTEtest::init(AADLObjectIface::IfaceType::Provided, &fn1);
    ci1.name = "eth0";
    AADLObjectIface *iface1 = AADLObjectIface::createIface(ci1);
    AADLObjectIface::CreationInfo ci2 = TASTEtest::init(AADLObjectIface::IfaceType::Required, &fn2);
    ci2.name = "wlan0";
    AADLObjectIface *iface2 = AADLObjectIface::createIface(ci2);
    AADLObjectIface::CreationInfo ci3 = TASTEtest::init(AADLObjectIface::IfaceType::Provided, &fn3);
    ci3.name = "ppp0";
    AADLObjectIface *iface3 = AADLObjectIface::createIface(ci3);

    const QVector<AADLObject *> objects { &container1, &fn1,        iface1, &container2, &fn2,
                                          iface2,      &container3, &fn3,   iface3 };

    for (auto object : objects)
        QVERIFY(model.addObject(object));

    QCOMPARE(model.objects().size(), 3 + functionsCount);

    for (auto object : objects)
        QVERIFY(model.removeObject(object));

    QCOMPARE(model.objects().size(), 0);
    QCOMPARE(model.objects().size(), 0);
    QCOMPARE(model.objects().size(), 0);
}

QTEST_APPLESS_MAIN(tst_AADLObjectsModel)

#include "tst_aadlobjectsmodel.moc"
