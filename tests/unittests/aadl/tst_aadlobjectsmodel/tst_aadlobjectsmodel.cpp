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
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectfunctiontype.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "aadltestutils.h"

#include <QScopedPointer>
#include <QtTest>

class tst_AADLObjectsModel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void testManageContainers();
    void testManageFunctions();
    void testManageIfaces();
    void testManageMixed();
    void testConnectionQuery();

private:
    QScopedPointer<aadl::AADLObjectsModel> m_model;
};

void tst_AADLObjectsModel::init()
{
    m_model.reset(new aadl::AADLObjectsModel);
}

void tst_AADLObjectsModel::testManageContainers()
{
    aadl::AADLObjectsModel model;

    QCOMPARE(model.objects().size(), 0);

    aadl::AADLObjectFunction container1("Container1");
    aadl::AADLObjectFunction container2("Container2");
    container1.addChild(&container2);
    aadl::AADLObjectFunction container3("Container3");
    container2.addChild(&container3);

    const QVector<aadl::AADLObjectFunction *> containers { &container1, &container2, &container3 };
    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectsAdded);

        const bool added = model.addObject(container);
        QVERIFY(added);
        QCOMPARE(model.objects().size(), i + 1);

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectsAdded);

        const bool nullAdded = model.addObject(nullptr);
        QVERIFY(!nullAdded);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectRemoved);

        aadl::AADLObjectFunction dummy;
        const bool dummyRemoved = model.removeObject(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyConcrete(&model, &aadl::AADLObjectsModel::aadlObjectsAdded);
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectRemoved);

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
    aadl::AADLObjectsModel model;

    QCOMPARE(model.objects().size(), 0);

    aadl::AADLObjectFunction fn1("Fn1");
    aadl::AADLObjectFunction fn2("Fn2", &model);
    aadl::AADLObjectFunction fn3("Fn3", &model);

    const aadl::AADLFunctionsVector functions { &fn1, &fn2, &fn3 };
    for (int i = 0; i < functions.size(); ++i) {
        auto function = functions.at(i);
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectsAdded);

        const bool added = model.addObject(function);
        QVERIFY(added);
        QCOMPARE(model.objects().size(), i + 1);

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectsAdded);

        const bool nullAdded = model.addObject(nullptr);
        QVERIFY(!nullAdded);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectRemoved);

        aadl::AADLObjectFunction dummy;
        const bool dummyRemoved = model.removeObject(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < functions.size(); ++i) {
        auto container = functions.at(i);
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectRemoved);

        const bool removed = model.removeObject(container);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), functions.size() - i - 1);

        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.objects().size(), 0);
}

void tst_AADLObjectsModel::testManageIfaces()
{
    aadl::AADLObjectsModel model;

    QCOMPARE(model.objects().size(), 0);

    aadl::AADLObjectFunction fn("fn");
    model.addObject(&fn);
    aadl::AADLObjectIface::CreationInfo ci1 = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Provided, &fn);
    ci1.name = "eth0";
    aadl::AADLObjectIface::CreationInfo ci2 = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Required, &fn);
    ci2.name = "wlan0";
    aadl::AADLObjectIface::CreationInfo ci3 = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Provided, &fn);
    ci3.name = "ppp0";

    QVector<aadl::AADLObjectIface *> createdIfaces;
    const QVector<aadl::AADLObjectIface::CreationInfo> ifaces { ci1, ci2, ci3 };
    for (int i = 0; i < ifaces.size(); ++i) {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectsAdded);

        aadl::AADLObjectIface::CreationInfo ci = ifaces.at(i);
        aadl::AADLObjectIface *iface = aadl::AADLObjectIface::createIface(ci);
        createdIfaces.append(iface);
        const bool added1 = ci.function->addChild(iface);
        QVERIFY(added1);
        const bool added2 = model.addObject(iface);
        QVERIFY(added2);
        QCOMPARE(model.objects().size(), i + 2); // the model now contains fn + currently added iface

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectsAdded);

        const bool nullAdded = model.addObject(nullptr);
        QVERIFY(!nullAdded);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // + function

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // +function

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectRemoved);

        aadl::AADLObjectIface *dummy = aadl::AADLObjectIface::createIface(
                aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Provided, nullptr));
        const bool dummyRemoved = model.removeObject(dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // +function

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < createdIfaces.size(); ++i) {
        auto iface = createdIfaces.at(i);
        QSignalSpy spyCommon(&model, &aadl::AADLObjectsModel::aadlObjectRemoved);

        const bool removed = model.removeObject(iface);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), ifaces.size() - i); // +function

        QVERIFY(spyCommon.count() == 1);
        if (auto parent = qobject_cast<aadl::AADLObjectFunction *>(iface->parentObject())) {
            parent->removeChild(iface);
            delete iface;
            iface = nullptr;
        }
    }

    QCOMPARE(model.objects().size(), 1); // the function is still in place!
}

void tst_AADLObjectsModel::testManageMixed()
{
    aadl::AADLObjectsModel model;

    QCOMPARE(model.objects().size(), 0);

    aadl::AADLObjectFunction container1("Container1");
    aadl::AADLObjectFunction container2("Container2", &container1);
    aadl::AADLObjectFunction container3("Container3", &container2);
    aadl::AADLObjectFunction fn1("Fn1");
    aadl::AADLObjectFunction fn2("Fn2", &model);
    aadl::AADLObjectFunction fn3("Fn3", &model);
    const int functionsCount = 6;

    aadl::AADLObjectIface::CreationInfo ci1 = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Provided, &fn1);
    ci1.name = "eth0";
    aadl::AADLObjectIface *iface1 = aadl::AADLObjectIface::createIface(ci1);
    aadl::AADLObjectIface::CreationInfo ci2 = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Required, &fn2);
    ci2.name = "wlan0";
    aadl::AADLObjectIface *iface2 = aadl::AADLObjectIface::createIface(ci2);
    aadl::AADLObjectIface::CreationInfo ci3 = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Provided, &fn3);
    ci3.name = "ppp0";
    aadl::AADLObjectIface *iface3 = aadl::AADLObjectIface::createIface(ci3);

    const QVector<aadl::AADLObject *> objects { &container1, &fn1, iface1, &container2, &fn2, iface2, &container3, &fn3,
        iface3 };

    for (auto object : objects)
        QVERIFY(model.addObject(object));

    QCOMPARE(model.objects().size(), 3 + functionsCount);

    for (auto object : objects)
        QVERIFY(model.removeObject(object));

    QCOMPARE(model.objects().size(), 0);
}

void tst_AADLObjectsModel::testConnectionQuery()
{
    auto fn1 = new aadl::AADLObjectFunction("Fn1");
    m_model->addObject(fn1);
    aadl::AADLObjectIface::CreationInfo ci1 = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Required, fn1);
    ci1.name = "cnt1";
    aadl::AADLObjectIface *iface1 = aadl::AADLObjectIface::createIface(ci1);
    m_model->addObject(iface1);

    auto fn2 = new aadl::AADLObjectFunction("Fn2");
    m_model->addObject(fn2);
    aadl::AADLObjectIface::CreationInfo ci2 = aadl::testutils::init(aadl::AADLObjectIface::IfaceType::Provided, fn2);
    ci2.name = "cnt1";
    aadl::AADLObjectIface *iface2 = aadl::AADLObjectIface::createIface(ci2);
    m_model->addObject(iface2);

    auto connect1 = new aadl::AADLObjectConnection(iface1, iface2);
    m_model->addObject(connect1);

    const Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
    QCOMPARE(m_model->getConnection("Dummy", "Fn1", "Fn2", m_caseCheck), nullptr);
    QCOMPARE(m_model->getConnection("cnt1", "Dummy", "Fn2", m_caseCheck), nullptr);
    QCOMPARE(m_model->getConnection("cnt1", "Fn1", "Dummy", m_caseCheck), nullptr);
    QCOMPARE(m_model->getConnection("cnt1", "Fn1", "Fn2", m_caseCheck), connect1);
}

QTEST_APPLESS_MAIN(tst_AADLObjectsModel)

#include "tst_aadlobjectsmodel.moc"
