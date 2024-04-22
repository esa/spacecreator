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

#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivobject.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"

#include <QScopedPointer>
#include <QtTest>

class tst_IVModel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testManageContainers();
    void testManageFunctions();
    void testManageIfaces();
    void testManageMixed();
    void testConnectionQuery();
    void testAvailableFunctionTypes();
    void testGetConnectionsBetweenFunctions();

private:
    ivm::IVPropertyTemplateConfig *m_dynPropConfig;
    QScopedPointer<ivm::IVModel> m_model;
};

void tst_IVModel::initTestCase()
{
    ivm::initIVLibrary();
    m_dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    m_dynPropConfig->init(QLatin1String("default_attributes.xml"));
    m_model.reset(new ivm::IVModel(m_dynPropConfig));
}

void tst_IVModel::testManageContainers()
{
    ivm::IVModel model(m_dynPropConfig);

    QCOMPARE(model.objects().size(), 0);

    ivm::IVFunction *container1 = ivm::testutils::createFunction("Container1");
    ivm::IVFunction *container2 = ivm::testutils::createFunction("Container2");
    container1->addChild(container2);
    ivm::IVFunction *container3 = ivm::testutils::createFunction("Container3");
    container2->addChild(container3);

    const QVector<ivm::IVFunction *> containers { container1, container2, container3 };
    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectsAdded);

        QVERIFY(model.addObject(container));
        QCOMPARE(model.objects().size(), i + 1);

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectsAdded);

        QVERIFY(!model.addObject(nullptr));
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectRemoved);

        ivm::IVFunction dummy;
        const bool dummyRemoved = model.removeObject(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), containers.size());

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < containers.size(); ++i) {
        auto container = containers.at(i);
        QSignalSpy spyConcrete(&model, &ivm::IVModel::objectsAdded);
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectRemoved);

        const bool removed = model.removeObject(container);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), containers.size() - i - 1);

        QVERIFY(spyConcrete.count() == 0);
        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.objects().size(), 0);
}

void tst_IVModel::testManageFunctions()
{
    ivm::IVModel model(m_dynPropConfig);

    QCOMPARE(model.objects().size(), 0);

    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1");
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2");
    fn2->setParent(&model);
    ivm::IVFunction *fn3 = ivm::testutils::createFunction("Fn3");

    const QVector<ivm::IVFunction *> functions { fn1, fn2, fn3 };
    for (int i = 0; i < functions.size(); ++i) {
        auto function = functions.at(i);
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectsAdded);

        QVERIFY(model.addObject(function));
        QCOMPARE(model.objects().size(), i + 1);

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectsAdded);

        QVERIFY(!model.addObject(nullptr));
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectRemoved);

        ivm::IVFunction dummy;
        const bool dummyRemoved = model.removeObject(&dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), functions.size());

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < functions.size(); ++i) {
        auto container = functions.at(i);
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectRemoved);

        const bool removed = model.removeObject(container);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), functions.size() - i - 1);

        QVERIFY(spyCommon.count() == 1);
    }

    QCOMPARE(model.objects().size(), 0);
}

void tst_IVModel::testManageIfaces()
{
    ivm::IVModel model(m_dynPropConfig);

    QCOMPARE(model.objects().size(), 0);

    ivm::IVFunction *fn = ivm::testutils::createFunction("fn");
    QVERIFY(model.addObject(fn));
    ivm::IVInterface::CreationInfo ci1 = ivm::testutils::init(ivm::IVInterface::InterfaceType::Provided, fn);
    ci1.name = "eth0";
    ivm::IVInterface::CreationInfo ci2 = ivm::testutils::init(ivm::IVInterface::InterfaceType::Required, fn);
    ci2.name = "wlan0";
    ivm::IVInterface::CreationInfo ci3 = ivm::testutils::init(ivm::IVInterface::InterfaceType::Provided, fn);
    ci3.name = "ppp0";

    QVector<ivm::IVInterface *> createdIfaces;
    const QVector<ivm::IVInterface::CreationInfo> ifaces { ci1, ci2, ci3 };
    for (int i = 0; i < ifaces.size(); ++i) {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectsAdded);

        ivm::IVInterface::CreationInfo ci = ifaces.at(i);
        ivm::IVInterface *iface = ivm::IVInterface::createIface(ci);
        createdIfaces.append(iface);
        const bool added1 = ci.function->addChild(iface);
        QVERIFY(added1);
        const bool added2 = model.addObject(iface);
        QVERIFY(added2);
        QCOMPARE(model.objects().size(), i + 2); // the model now contains fn + currently added iface

        QVERIFY(spyCommon.count() == 1);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectsAdded);

        QVERIFY(!model.addObject(nullptr));
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // + function

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectRemoved);

        const bool nullRemoved = model.removeObject(nullptr);
        QVERIFY(!nullRemoved);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // +function

        QVERIFY(spyCommon.count() == 0);
    }

    {
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectRemoved);

        ivm::IVInterface *dummy =
                ivm::IVInterface::createIface(ivm::testutils::init(ivm::IVInterface::InterfaceType::Provided, nullptr));
        const bool dummyRemoved = model.removeObject(dummy);
        QVERIFY(!dummyRemoved);
        QCOMPARE(model.objects().size(), ifaces.size() + 1); // +function

        QVERIFY(spyCommon.count() == 0);
    }

    for (int i = 0; i < createdIfaces.size(); ++i) {
        auto iface = createdIfaces.at(i);
        QSignalSpy spyCommon(&model, &ivm::IVModel::objectRemoved);

        const bool removed = model.removeObject(iface);
        QVERIFY(removed);
        QCOMPARE(model.objects().size(), ifaces.size() - i); // +function

        QVERIFY(spyCommon.count() == 1);
        if (auto parent = qobject_cast<ivm::IVFunction *>(iface->parentObject())) {
            parent->removeChild(iface);
            delete iface;
            iface = nullptr;
        }
    }

    QCOMPARE(model.objects().size(), 1); // the function is still in place!
}

void tst_IVModel::testManageMixed()
{
    ivm::IVModel model(m_dynPropConfig);

    QCOMPARE(model.objects().size(), 0);

    ivm::IVFunction *container1 = ivm::testutils::createFunction("Container1", &model);
    ivm::IVFunction *container2 = ivm::testutils::createFunction("Container2", container1);
    ivm::IVFunction *container3 = ivm::testutils::createFunction("Container3", container2);
    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1", &model);
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2", &model);
    ivm::IVFunction *fn3 = ivm::testutils::createFunction("Fn3", &model);
    const int functionsCount = 6;

    ivm::IVInterface::CreationInfo ci1 = ivm::testutils::init(ivm::IVInterface::InterfaceType::Provided, fn1);
    ci1.name = "eth0";
    ivm::IVInterface *iface1 = ivm::IVInterface::createIface(ci1);
    model.addObject(iface1);
    ivm::IVInterface::CreationInfo ci2 = ivm::testutils::init(ivm::IVInterface::InterfaceType::Required, fn2);
    ci2.name = "wlan0";
    ivm::IVInterface *iface2 = ivm::IVInterface::createIface(ci2);
    model.addObject(iface2);
    ivm::IVInterface::CreationInfo ci3 = ivm::testutils::init(ivm::IVInterface::InterfaceType::Provided, fn3);
    ci3.name = "ppp0";
    ivm::IVInterface *iface3 = ivm::IVInterface::createIface(ci3);
    model.addObject(iface3);

    const QVector<ivm::IVObject *> objects { container1, fn1, iface1, container2, fn2, iface2, container3, fn3,
        iface3 };

    QCOMPARE(model.objects().size(), 3 + functionsCount);

    for (auto object : objects) {
        QVERIFY(model.removeObject(object));
    }

    QCOMPARE(model.objects().size(), 0);
}

void tst_IVModel::testConnectionQuery()
{
    auto fn1 = ivm::testutils::createFunction("Fn1");
    auto fn2 = ivm::testutils::createFunction("Fn2");
    ivm::IVConnection *connect1 = ivm::testutils::createConnection(fn1, fn2, "cnt1");
    m_model->addObjects(QList<ivm::IVObject *> { fn1, fn2, connect1 });

    const Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
    QCOMPARE(m_model->getConnection("Dummy", "Fn1", "Fn2", m_caseCheck), nullptr);
    QCOMPARE(m_model->getConnection("cnt1", "Dummy", "Fn2", m_caseCheck), nullptr);
    QCOMPARE(m_model->getConnection("cnt1", "Fn1", "Dummy", m_caseCheck), nullptr);
    QCOMPARE(m_model->getConnection("cnt1", "Fn1", "Fn2", m_caseCheck), connect1);
}

void tst_IVModel::testAvailableFunctionTypes()
{
    ivm::IVModel sharedModel(m_dynPropConfig);
    ivm::IVFunctionType *sfnt1 = ivm::testutils::createFunctionType("sFnT1", &sharedModel);
    ivm::IVFunctionType *sfnt2 = ivm::testutils::createFunctionType("sFnT2", &sharedModel);
    ivm::IVFunctionType *sfnt3 = ivm::testutils::createFunctionType("sFnT3", &sharedModel);
    const int sharedFunctionTypesCount = 3;

    const QVector<ivm::IVObject *> sharedObjects { sfnt1, sfnt2, sfnt3 };
    QCOMPARE(sharedModel.objects().size(), sharedFunctionTypesCount);

    ivm::IVModel model(m_dynPropConfig, &sharedModel);
    model.setSharedTypeRequester([&]() -> QVector<ivm::IVFunctionType *> {
        QVector<ivm::IVFunctionType *> fnTypes;
        const QHash<shared::Id, shared::VEObject *> objects = sharedModel.objects();
        for (auto it = objects.constBegin(); it != objects.constEnd(); ++it) {
            if (auto ivObj = qobject_cast<ivm::IVObject *>(*it)) {
                if (ivObj->type() == ivm::IVObject::Type::FunctionType)
                    fnTypes << ivObj->as<ivm::IVFunctionType *>();
            }
        }
        return fnTypes;
    });

    ivm::IVFunction *container1 = ivm::testutils::createFunction("Container1", &model);
    ivm::IVFunction *container2 = ivm::testutils::createFunction("Container2", container1);
    ivm::IVFunction *container3 = ivm::testutils::createFunction("Container3", container2);
    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1", &model);
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2", &model);
    ivm::IVFunction *fn3 = ivm::testutils::createFunction("Fn3", &model);
    const int functionsCount = 6;

    ivm::IVFunctionType *fnt1 = ivm::testutils::createFunctionType("FnT1", &model);
    ivm::IVFunctionType *fnt2 = ivm::testutils::createFunctionType("FnT2", &model);
    ivm::IVFunctionType *fnt3 = ivm::testutils::createFunctionType("FnT3", &model);
    const int functionTypesCount = 3;

    const QVector<ivm::IVObject *> objects { container1, container2, container3, fn1, fn2, fn3, fnt1, fnt2, fnt3 };
    QCOMPARE(model.objects().size(), functionsCount + functionTypesCount);

    const auto availableTypes = model.getAvailableFunctionTypes(container1);
    QCOMPARE(availableTypes.size(), functionTypesCount + sharedFunctionTypesCount);

    for (auto object : objects) {
        QCOMPARE(object->isFunctionType(), availableTypes.contains(object->title()));
    }
    for (auto object : sharedObjects) {
        QVERIFY(availableTypes.contains(object->title()));
    }
}

void tst_IVModel::testGetConnectionsBetweenFunctions()
{
    ivm::IVFunction *fn1 = ivm::testutils::createFunction("Fn1");
    ivm::IVFunction *fn2 = ivm::testutils::createFunction("Fn2");
    ivm::IVFunction *fn3 = ivm::testutils::createFunction("Fn3");

    ivm::IVConnection *c1 = ivm::testutils::createConnection(fn1, fn2, "C1");
    ivm::IVConnection *c2 = ivm::testutils::createConnection(fn1, fn2, "C2");
    ivm::IVConnection *c3 = ivm::testutils::createConnection(fn2, fn1, "C3");
    ivm::IVConnection *c4 = ivm::testutils::createConnection(fn1, fn3, "C4");
    ivm::IVConnection *c5 = ivm::testutils::createConnection(fn3, fn2, "C5");

    ivm::IVModel model(m_dynPropConfig, nullptr);
    const QVector<ivm::IVObject *> objects { fn1, fn2, fn3, c1, c2, c3, c4, c5 };
    model.addObjects(objects);

    const QVector<ivm::IVConnection *> connections = model.getConnectionsBetweenFunctions(fn1->id(), fn2->id());
    QVERIFY(connections.contains(c1));
    QVERIFY(connections.contains(c2));
    QVERIFY(connections.contains(c3));
    QVERIFY(!connections.contains(c4));
    QVERIFY(!connections.contains(c5));
}

QTEST_APPLESS_MAIN(tst_IVModel)

#include "tst_ivmodel.moc"
