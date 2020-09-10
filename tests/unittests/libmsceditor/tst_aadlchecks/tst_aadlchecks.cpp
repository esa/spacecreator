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

#include "aadlchecks.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
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

class tst_AadlChecks : public QObject
{
    Q_OBJECT
public:
    tst_AadlChecks()
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

void tst_AadlChecks::initTestCase()
{
    auto converter = msc::CoordinatesConverter::instance();
    converter->setDPI(QPointF(109., 109.), QPointF(96., 96.));
    aadlinterface::cmd::CommandsStack::setCurrent(&m_stack);
}

void tst_AadlChecks::testCheckInstanceNames()
{
    msc::AadlChecks checker;
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 0);

    msc::MSCEditorCore mscPlugin;
    mscPlugin.mainModel()->initialModel();
    checker.setMscPlugin(&mscPlugin);
    result = checker.checkInstanceNames();
    msc::MscChart *chart = mscPlugin.mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    QSharedPointer<aadlinterface::IVEditorCore> ivPlugin(new aadlinterface::IVEditorCore);
    checker.setIvPlugin(ivPlugin);
    result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 0);

    // Add instance
    auto instance = new msc::MscInstance("Dummy", chart);
    chart->addInstance(instance);
    result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 1);

    // Add function with different name
    aadlinterface::InterfaceDocument *doc = ivPlugin->document();
    aadl::AADLObjectsModel *aadlModel = doc->objectsModel();
    aadlModel->addObject(new aadl::AADLObjectFunction("init"));
    result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 1);

    // set instance name to the function name
    instance->setName("init");
    result = checker.checkInstanceNames();
    QCOMPARE(result.size(), 0);
}

void tst_AadlChecks::testCheckInstanceRelations()
{
    msc::AadlChecks checker;
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result = checker.checkInstanceRelations();
    QCOMPARE(result.size(), 0);

    msc::MSCEditorCore mscPlugin;
    mscPlugin.mainModel()->initialModel();
    checker.setMscPlugin(&mscPlugin);
    result = checker.checkInstanceRelations();
    msc::MscChart *chart = mscPlugin.mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    QSharedPointer<aadlinterface::IVEditorCore> ivPlugin(new aadlinterface::IVEditorCore);
    checker.setIvPlugin(ivPlugin);
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

void tst_AadlChecks::testCheckMessageNames()
{
    msc::AadlChecks checker;
    QVector<QPair<msc::MscChart *, msc::MscMessage *>> result = checker.checkMessages();
    QCOMPARE(result.size(), 0);

    msc::MSCEditorCore mscPlugin;
    mscPlugin.mainModel()->initialModel();
    checker.setMscPlugin(&mscPlugin);
    result = checker.checkMessages();
    msc::MscChart *chart = mscPlugin.mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    QSharedPointer<aadlinterface::IVEditorCore> ivPlugin(new aadlinterface::IVEditorCore);
    checker.setIvPlugin(ivPlugin);
    result = checker.checkMessages();
    QCOMPARE(result.size(), 0);

    // Add Message
    auto instanceA = new msc::MscInstance("Instance A", chart);
    chart->addInstance(instanceA);
    auto message = new msc::MscMessage("Msg1", chart);
    message->setTargetInstance(instanceA);
    chart->addInstanceEvent(message);
    result = checker.checkMessages();
    QCOMPARE(result.size(), 1);

    // Add function with different source/target
    aadlinterface::InterfaceDocument *doc = ivPlugin->document();
    aadl::AADLObjectsModel *aadlModel = doc->objectsModel();
    auto aadlfFunc = new aadl::AADLObjectFunction("Instance A");
    aadlModel->addObject(aadlfFunc);
    aadlModel->addObject(new aadl::AADLObjectConnection(aadlfFunc, nullptr, nullptr, nullptr));
    result = checker.checkMessages();
    QCOMPARE(result.size(), 1);

    // Add connection with proper source/target, but a wrong name
    auto createInfo = aadl::AADLObjectIface::CreationInfo(aadlModel, aadlfFunc, QPointF(),
            aadl::AADLObjectIface::IfaceType::Provided, shared::createId(), QVector<aadl::IfaceParameter>(),
            aadl::AADLObjectIface::OperationKind::Sporadic, "Dummy");
    aadl::AADLObjectIface *providedInterface = aadl::AADLObjectIface::createIface(createInfo);
    aadlModel->addObject(providedInterface);
    aadlModel->addObject(new aadl::AADLObjectConnection(nullptr, aadlfFunc, nullptr, providedInterface));
    result = checker.checkMessages();
    QCOMPARE(result.size(), 1);

    // Correct the name
    providedInterface->setTitle("Msg1");
    result = checker.checkMessages();
    QCOMPARE(result.size(), 0); // Everything is ok
}

QTEST_MAIN(tst_AadlChecks)

#include "tst_aadlchecks.moc"
