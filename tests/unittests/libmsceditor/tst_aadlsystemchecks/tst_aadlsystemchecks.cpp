/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "aadlsystemchecks.h"
#include "baseitems/common/coordinatesconverter.h"
#include "chartitem.h"
#include "commandsstack.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QUndoCommand>
#include <QtTest>

using namespace msc;

class tst_AadlSystemChecks : public QObject
{
    Q_OBJECT
public:
    tst_AadlSystemChecks()
        : m_chartItem(nullptr, nullptr)
    {
    }

private Q_SLOTS:
    void initTestCase();

    void testCheckInstanceNames();
    void testCheckInstanceRelations();
    void testCheckMessageNames();

private:
    QGraphicsView m_view;
    msc::ChartItem m_chartItem;
    QUndoStack m_stack;
};

void tst_AadlSystemChecks::initTestCase()
{
    auto converter = msc::CoordinatesConverter::instance();
    converter->setDPI(QPointF(109., 109.), QPointF(96., 96.));
    aadlinterface::cmd::CommandsStack::setCurrent(&m_stack);
}

void tst_AadlSystemChecks::testCheckInstanceNames()
{
    msc::AadlSystemChecks checker;
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 0);

    msc::MSCEditorCore mscCore;
    mscCore.mainModel()->initialModel();
    checker.setMscCore(&mscCore);
    msc::MscChart *chart = mscCore.mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    QSharedPointer<aadlinterface::IVEditorCore> ivPlugin(new aadlinterface::IVEditorCore);
    checker.setIvCore(ivPlugin);
    result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 0);

    // Add instance
    auto instance = new msc::MscInstance("Dummy", chart);
    chart->addInstance(instance);
    result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 1);
    QCOMPARE(checker.checkInstance(instance), false);

    // Add function with different name
    aadlinterface::InterfaceDocument *doc = ivPlugin->document();
    aadl::AADLObjectsModel *aadlModel = doc->objectsModel();
    auto aadlFnct = new aadl::AADLObjectFunction("init");
    aadlModel->addObject(aadlFnct);
    result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 1);
    QCOMPARE(checker.checkInstance(instance), false);

    // set instance name to the function name
    instance->setName("init");
    result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 0);
    QCOMPARE(checker.checkInstance(instance), true);

    // Renaming the aadl function invalidates again
    aadlFnct->setTitle("Foo");
    result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 1);
    QCOMPARE(checker.checkInstance(instance), false);
}

void tst_AadlSystemChecks::testCheckInstanceRelations()
{
    msc::AadlSystemChecks checker;
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result = checker.checkInstanceRelations();
    QCOMPARE(result.size(), 0);

    msc::MSCEditorCore mscCore;
    mscCore.mainModel()->initialModel();
    checker.setMscCore(&mscCore);
    result = checker.checkInstanceRelations();
    msc::MscChart *chart = mscCore.mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    QSharedPointer<aadlinterface::IVEditorCore> ivPlugin(new aadlinterface::IVEditorCore);
    checker.setIvCore(ivPlugin);
    result = checker.checkInstanceRelations();
    QCOMPARE(result.size(), 0);

    // Add instance2
    auto instance1 = new msc::MscInstance("init", chart);
    chart->addInstance(instance1);
    auto instance2 = new msc::MscInstance("reset", chart);
    chart->addInstance(instance2);
    result = checker.checkInstanceRelations();
    QCOMPARE(result.size(), 0);

    // Add function for the instances
    aadlinterface::InterfaceDocument *doc = ivPlugin->document();
    aadl::AADLObjectsModel *aadlModel = doc->objectsModel();
    auto function1 = new aadl::AADLObjectFunction("init");
    aadlModel->addObject(function1);
    auto function2 = new aadl::AADLObjectFunction("reset");
    aadlModel->addObject(function2);
    result = checker.checkInstanceRelations();
    QCOMPARE(result.size(), 0);

    // Make function2 be nested by function1
    function2->setParent(function1);
    result = checker.checkInstanceRelations();
    QCOMPARE(result.size(), 2);

    // Make function2 be nested by function1 via another one
    auto function15 = new aadl::AADLObjectFunction("init");
    aadlModel->addObject(function15);
    function15->setParent(function1);
    function2->setParent(function15);
    result = checker.checkInstanceRelations();
    QCOMPARE(result.size(), 2);
}

void tst_AadlSystemChecks::testCheckMessageNames()
{
    msc::AadlSystemChecks checker;
    QVector<QPair<msc::MscChart *, msc::MscMessage *>> result = checker.checkMessages();
    QCOMPARE(result.size(), 0);

    msc::MSCEditorCore mscCore;
    mscCore.mainModel()->initialModel();
    checker.setMscCore(&mscCore);
    result = checker.checkMessages();
    msc::MscChart *chart = mscCore.mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    QSharedPointer<aadlinterface::IVEditorCore> ivPlugin(new aadlinterface::IVEditorCore);
    checker.setIvCore(ivPlugin);
    result = checker.checkMessages();
    QCOMPARE(result.size(), 0);

    // Add Instance 'A' and message 'Msg1'
    auto instanceA = new msc::MscInstance("Instance A", chart);
    chart->addInstance(instanceA);
    auto message = new msc::MscMessage("Msg1", chart);
    message->setTargetInstance(instanceA);
    chart->addInstanceEvent(message);

    // Add Instance 'B'
    auto instanceB = new msc::MscInstance("Instance B", chart);
    chart->addInstance(instanceB);
    message->setSourceInstance(instanceB);

    result = checker.checkMessages();
    QCOMPARE(result.size(), 1);

    // Add function with different source/target
    aadlinterface::InterfaceDocument *doc = ivPlugin->document();
    aadl::AADLObjectsModel *aadlModel = doc->objectsModel();
    auto aadlfFuncA = new aadl::AADLObjectFunction("Instance A");
    aadlModel->addObject(aadlfFuncA);
    auto aadlfFuncB = new aadl::AADLObjectFunction("Instance B");
    aadlModel->addObject(aadlfFuncB);

    // Add connection with proper source/target, but a wrong name
    const auto createInfoA = aadl::AADLObjectIface::CreationInfo(aadlModel, aadlfFuncA, QPointF(),
            aadl::AADLObjectIface::IfaceType::Provided, shared::createId(), QVector<aadl::IfaceParameter>(),
            aadl::AADLObjectIface::OperationKind::Sporadic, "DummyA");
    aadl::AADLObjectIface *providedInterface = aadl::AADLObjectIface::createIface(createInfoA);
    aadlModel->addObject(providedInterface);

    const auto createInfoB = aadl::AADLObjectIface::CreationInfo(aadlModel, aadlfFuncB, QPointF(),
            aadl::AADLObjectIface::IfaceType::Required, shared::createId(), QVector<aadl::IfaceParameter>(),
            aadl::AADLObjectIface::OperationKind::Sporadic, "DummyB");
    aadl::AADLObjectIface *requiredInterface = aadl::AADLObjectIface::createIface(createInfoB);

    aadlModel->addObject(providedInterface);
    aadlModel->addObject(new aadl::AADLObjectConnection(aadlfFuncB, aadlfFuncA, requiredInterface, providedInterface));
    result = checker.checkMessages();
    QCOMPARE(result.size(), 1);

    // Correct the name
    providedInterface->setTitle("Msg1");
    result = checker.checkMessages();
    QCOMPARE(result.size(), 0); // Everything is ok
}

QTEST_MAIN(tst_AadlSystemChecks)

#include "tst_aadlsystemchecks.moc"
