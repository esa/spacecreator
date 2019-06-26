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
#include "tab_aadl/aadlobjectsmodel.h"

#include <QtTest>

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
    using namespace taste3::aadl;
    AADLObjectsModel model;

    QCOMPARE(model.containers().size(), 0);

    AADLObjectContainer container1("Container1");
    AADLObjectContainer container2("Container2", &container1);
    AADLObjectContainer container3("Container3", &container2);

    const AADLContainersVector containers { &container1, &container2, &container3 };
    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::containerAdded);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool added = model.addContainer(container);
        QVERIFY(added);
        QCOMPARE(model.containers().size(), i + 1);

        QVERIFY(spyConcrete.count() == 1);
        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::containerAdded);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool nullAdded = model.addContainer(nullptr);
        QVERIFY(!nullAdded);
        QCOMPARE(model.containers().size(), containers.size());

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::containerRemoved);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeContainer(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.containers().size(), containers.size());

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::containerRemoved);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        AADLObjectContainer dummy;
        const bool dummyRemoved = model.removeContainer(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.containers().size(), containers.size());

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::containerRemoved);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool removed = model.removeContainer(container);
        QVERIFY(removed);
        QCOMPARE(model.containers().size(), containers.size() - i - 1);

        QVERIFY(spyConcrete.count() == 1);
        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.containers().size(), 0);
}

void tst_AADLObjectsModel::testManageFunctions()
{
    using namespace taste3::aadl;
    AADLObjectsModel model;

    QCOMPARE(model.containers().size(), 0);

    AADLObjectFunction fn1("Fn1");
    AADLObjectFunction fn2("Fn2", &model);
    AADLObjectFunction fn3("Fn3", &model);

    const AADLFunctionsVector functions { &fn1, &fn2, &fn3 };
    for (int i = 0; i < functions.size(); ++i) {
        auto function = functions.at(i);
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::functionAdded);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool added = model.addFunction(function);
        QVERIFY(added);
        QCOMPARE(model.functions().size(), i + 1);

        QVERIFY(spyConcrete.count() == 1);
        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::functionAdded);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool nullAdded = model.addFunction(nullptr);
        QVERIFY(!nullAdded);
        QCOMPARE(model.functions().size(), functions.size());

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::functionRemoved);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeFunction(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.functions().size(), functions.size());

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::functionRemoved);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        AADLObjectFunction dummy;
        const bool dummyRemoved = model.removeFunction(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.functions().size(), functions.size());

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < functions.size(); ++i) {
        auto container = functions.at(i);
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::functionRemoved);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool removed = model.removeFunction(container);
        QVERIFY(removed);
        QCOMPARE(model.functions().size(), functions.size() - i - 1);

        QVERIFY(spyConcrete.count() == 1);
        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.functions().size(), 0);
}

void tst_AADLObjectsModel::testManageIfaces()
{
    using namespace taste3::aadl;
    AADLObjectsModel model;

    QCOMPARE(model.containers().size(), 0);

    AADLObjectIface fn1(AADLObjectIface::IfaceType::Provided, "eth0");
    AADLObjectIface fn2(AADLObjectIface::IfaceType::Required, "wlan0");
    AADLObjectIface fn3(AADLObjectIface::IfaceType::Provided, "ppp0");

    const AADLIfacesVector ifaces { &fn1, &fn2, &fn3 };
    for (int i = 0; i < ifaces.size(); ++i) {
        auto iface = ifaces.at(i);
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::ifaceAdded);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool added = model.addIface(iface);
        QVERIFY(added);
        QCOMPARE(model.ifaces().size(), i + 1);

        QVERIFY(spyConcrete.count() == 1);
        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::ifaceAdded);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectAdded);

        const bool nullAdded = model.addIface(nullptr);
        QVERIFY(!nullAdded);
        QCOMPARE(model.ifaces().size(), ifaces.size());

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::ifaceRemoved);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeIface(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.ifaces().size(), ifaces.size());

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::ifaceRemoved);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        AADLObjectIface dummy;
        const bool dummyRemoved = model.removeIface(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.ifaces().size(), ifaces.size());

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < ifaces.size(); ++i) {
        auto iface = ifaces.at(i);
        QSignalSpy spyConcrete(&model, &AADLObjectsModel::ifaceRemoved);
        QSignalSpy spyCommon(&model, &AADLObjectsModel::aadlObjectRemoved);

        const bool removed = model.removeIface(iface);
        QVERIFY(removed);
        QCOMPARE(model.ifaces().size(), ifaces.size() - i - 1);

        QVERIFY(spyConcrete.count() == 1);
        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.ifaces().size(), 0);
}

void tst_AADLObjectsModel::testManageMixed()
{
    using namespace taste3::aadl;
    AADLObjectsModel model;

    QCOMPARE(model.containers().size(), 0);

    AADLObjectContainer container1("Container1");
    AADLObjectContainer container2("Container2", &container1);
    AADLObjectContainer container3("Container3", &container2);
    AADLObjectFunction fn1("Fn1");
    AADLObjectFunction fn2("Fn2", &model);
    AADLObjectFunction fn3("Fn3", &model);
    AADLObjectIface iface1(AADLObjectIface::IfaceType::Provided, "eth0");
    AADLObjectIface iface2(AADLObjectIface::IfaceType::Required, "wlan0");
    AADLObjectIface iface3(AADLObjectIface::IfaceType::Provided, "ppp0");

    const QVector<AADLObject *> objects { &container1, &fn1,        &iface1, &container2, &fn2,
                                          &iface2,     &container3, &fn3,    &iface3 };

    for (auto object : objects)
        QVERIFY(model.addObject(object));

    QCOMPARE(model.containers().size(), 3);
    QCOMPARE(model.functions().size(), 3);
    QCOMPARE(model.ifaces().size(), 3);

    for (auto object : objects)
        QVERIFY(model.removeObject(object));

    QCOMPARE(model.containers().size(), 0);
    QCOMPARE(model.functions().size(), 0);
    QCOMPARE(model.ifaces().size(), 0);
}

QTEST_APPLESS_MAIN(tst_AADLObjectsModel)

#include "tst_aadlobjectsmodel.moc"
