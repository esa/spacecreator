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

#include "aadlconnection.h"
#include "aadlfunction.h"
#include "aadlfunctiontype.h"
#include "aadliface.h"
#include "aadllibrary.h"
#include "aadlmodel.h"
#include "aadlobject.h"
#include "aadltestutils.h"
#include "propertytemplateconfig.h"

#include <QScopedPointer>
#include <QtTest>

class tst_AADLModel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testManageContainers();
    void testManageFunctions();
    void testManageIfaces();
    void testManageMixed();
    void testConnectionQuery();

private:
    ivm::PropertyTemplateConfig *m_dynPropConfig;
    QScopedPointer<ivm::AADLModel> m_model;
};

void tst_AADLModel::initTestCase()
{
    ivm::initAadlLibrary();
    m_dynPropConfig = ivm::PropertyTemplateConfig::instance();
    m_dynPropConfig->init(QLatin1String("default_attributes.xml"));
    m_model.reset(new ivm::AADLModel(m_dynPropConfig));
}

void tst_AADLModel::testManageContainers()
{
    ivm::AADLModel model(m_dynPropConfig);

    QCOMPARE(model.objects().size(), 0);

    ivm::AADLFunction container1("Container1");
    ivm::AADLFunction container2("Container2");
    container1.addChild(&container2);
    ivm::AADLFunction container3("Container3");
    container2.addChild(&container3);

    const QVector<ivm::AADLFunction *> containers { &container1, &container2, &container3 };
    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectsAdded);

        QVERIFY(model.addObject(container));
        QCOMPARE(model.objects().size(), i + 1);

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectsAdded);

        QVERIFY(!model.addObject(nullptr));
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectRemoved);

        ivm::AADLFunction dummy;
        const bool dummyRemoved = model.removeObject(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyConcrete(&model, &ivm::AADLModel::aadlObjectsAdded);
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectRemoved);

        const bool removed = model.removeObject(container);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), containers.size() - i - 1);

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.objects().size(), 0);
}

void tst_AADLModel::testManageFunctions()
{
    ivm::AADLModel model(m_dynPropConfig);

    QCOMPARE(model.objects().size(), 0);

    ivm::AADLFunction fn1("Fn1");
    ivm::AADLFunction fn2("Fn2", &model);
    ivm::AADLFunction fn3("Fn3", &model);

    const ivm::AADLFunctionsVector functions { &fn1, &fn2, &fn3 };
    for (int i = 0; i < functions.size(); ++i) {
        auto function = functions.at(i);
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectsAdded);

        QVERIFY(model.addObject(function));
        QCOMPARE(model.objects().size(), i + 1);

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectsAdded);

        QVERIFY(!model.addObject(nullptr));
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectRemoved);

        ivm::AADLFunction dummy;
        const bool dummyRemoved = model.removeObject(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < functions.size(); ++i) {
        auto container = functions.at(i);
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectRemoved);

        const bool removed = model.removeObject(container);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), functions.size() - i - 1);

        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.objects().size(), 0);
}

void tst_AADLModel::testManageIfaces()
{
    ivm::AADLModel model(m_dynPropConfig);

    QCOMPARE(model.objects().size(), 0);

    ivm::AADLFunction fn("fn");
    QVERIFY(model.addObject(&fn));
    ivm::AADLIface::CreationInfo ci1 = ivm::testutils::init(ivm::AADLIface::IfaceType::Provided, &fn);
    ci1.name = "eth0";
    ivm::AADLIface::CreationInfo ci2 = ivm::testutils::init(ivm::AADLIface::IfaceType::Required, &fn);
    ci2.name = "wlan0";
    ivm::AADLIface::CreationInfo ci3 = ivm::testutils::init(ivm::AADLIface::IfaceType::Provided, &fn);
    ci3.name = "ppp0";

    QVector<ivm::AADLIface *> createdIfaces;
    const QVector<ivm::AADLIface::CreationInfo> ifaces { ci1, ci2, ci3 };
    for (int i = 0; i < ifaces.size(); ++i) {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectsAdded);

        ivm::AADLIface::CreationInfo ci = ifaces.at(i);
        ivm::AADLIface *iface = ivm::AADLIface::createIface(ci);
        createdIfaces.append(iface);
        const bool added1 = ci.function->addChild(iface);
        QVERIFY(added1);
        const bool added2 = model.addObject(iface);
        QVERIFY(added2);
        QCOMPARE(model.objects().size(), i + 2); // the model now contains fn + currently added iface

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectsAdded);

        QVERIFY(!model.addObject(nullptr));
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // + function

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // +function

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectRemoved);

        ivm::AADLIface *dummy =
                ivm::AADLIface::createIface(ivm::testutils::init(ivm::AADLIface::IfaceType::Provided, nullptr));
        const bool dummyRemoved = model.removeObject(dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // +function

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < createdIfaces.size(); ++i) {
        auto iface = createdIfaces.at(i);
        QSignalSpy spyCommon(&model, &ivm::AADLModel::aadlObjectRemoved);

        const bool removed = model.removeObject(iface);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), ifaces.size() - i); // +function

        QVERIFY(spyCommon.count() == 1);
        if (auto parent = qobject_cast<ivm::AADLFunction *>(iface->parentObject())) {
            parent->removeChild(iface);
            delete iface;
            iface = nullptr;
        }
    }

    QCOMPARE(model.objects().size(), 1); // the function is still in place!
}

void tst_AADLModel::testManageMixed()
{
    ivm::AADLModel model(m_dynPropConfig);

    QCOMPARE(model.objects().size(), 0);

    ivm::AADLFunction container1("Container1");
    ivm::AADLFunction container2("Container2", &container1);
    ivm::AADLFunction container3("Container3", &container2);
    ivm::AADLFunction fn1("Fn1");
    ivm::AADLFunction fn2("Fn2", &model);
    ivm::AADLFunction fn3("Fn3", &model);
    const int functionsCount = 6;

    ivm::AADLIface::CreationInfo ci1 = ivm::testutils::init(ivm::AADLIface::IfaceType::Provided, &fn1);
    ci1.name = "eth0";
    ivm::AADLIface *iface1 = ivm::AADLIface::createIface(ci1);
    ivm::AADLIface::CreationInfo ci2 = ivm::testutils::init(ivm::AADLIface::IfaceType::Required, &fn2);
    ci2.name = "wlan0";
    ivm::AADLIface *iface2 = ivm::AADLIface::createIface(ci2);
    ivm::AADLIface::CreationInfo ci3 = ivm::testutils::init(ivm::AADLIface::IfaceType::Provided, &fn3);
    ci3.name = "ppp0";
    ivm::AADLIface *iface3 = ivm::AADLIface::createIface(ci3);

    const QVector<ivm::AADLObject *> objects { &container1, &fn1, iface1, &container2, &fn2, iface2, &container3, &fn3,
        iface3 };

    model.addObjects(objects);

    QCOMPARE(model.objects().size(), 3 + functionsCount);

    for (auto object : objects)
        QVERIFY(model.removeObject(object));

    QCOMPARE(model.objects().size(), 0);
}

void tst_AADLModel::testConnectionQuery()
{
    auto fn1 = new ivm::AADLFunction("Fn1");
    auto fn2 = new ivm::AADLFunction("Fn2");
    ivm::AADLConnection *connect1 = ivm::testutils::createConnection(fn1, fn2, "cnt1");
    m_model->addObjects({ fn1, fn2, connect1 });

    const Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
    QCOMPARE(m_model->getConnection("Dummy", "Fn1", "Fn2", m_caseCheck), nullptr);
    QCOMPARE(m_model->getConnection("cnt1", "Dummy", "Fn2", m_caseCheck), nullptr);
    QCOMPARE(m_model->getConnection("cnt1", "Fn1", "Dummy", m_caseCheck), nullptr);
    QCOMPARE(m_model->getConnection("cnt1", "Fn1", "Fn2", m_caseCheck), connect1);
}

QTEST_APPLESS_MAIN(tst_AADLModel)

#include "tst_aadlmodel.moc"
