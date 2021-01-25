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
#include "aadltestutils.h"
#include "baseitems/common/coordinatesconverter.h"
#include "chartitem.h"
#include "commandsstack.h"
#include "interface/interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"

#include <QGraphicsScene>
#include <QUndoCommand>
#include <QtTest>
#include <memory>

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
    void init();

    void testCheckInstanceNames();
    void testCheckInstanceRelations();
    void testCheckMessageNames();

    void testCorrespondMessage_data();
    void testCorrespondMessage();

    void testCheckMessage();

private:
    msc::ChartItem m_chartItem;
    QUndoStack m_stack;
    std::unique_ptr<msc::AadlSystemChecks> m_checker;
    std::unique_ptr<msc::MSCEditorCore> m_mscCore;
};

void tst_AadlSystemChecks::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    ive::initIvEditor();
    auto converter = msc::CoordinatesConverter::instance();
    converter->setDPI(QPointF(109., 109.), QPointF(96., 96.));
    ive::cmd::CommandsStack::setCurrent(&m_stack);
}

void tst_AadlSystemChecks::init()
{
    m_checker = std::make_unique<msc::AadlSystemChecks>();
    m_mscCore = std::make_unique<msc::MSCEditorCore>();
    m_mscCore->mainModel()->initialModel();
    m_checker->setMscCore(m_mscCore.get());
}

void tst_AadlSystemChecks::testCheckInstanceNames()
{
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result = m_checker->checkInstanceNames();
    QCOMPARE(result.size(), 0);

    msc::MscChart *chart = m_mscCore->mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    QSharedPointer<ive::IVEditorCore> ivPlugin(new ive::IVEditorCore);
    m_checker->setIvCore(ivPlugin);
    result = m_checker->checkInstanceNames();
    QCOMPARE(result.size(), 0);

    // Add instance
    auto instance = new msc::MscInstance("Dummy", chart);
    chart->addInstance(instance);
    result = m_checker->checkInstanceNames();
    QCOMPARE(result.size(), 1);
    QCOMPARE(m_checker->checkInstance(instance), false);

    // Add function with different name
    ive::InterfaceDocument *doc = ivPlugin->document();
    ivm::AADLObjectsModel *aadlModel = doc->objectsModel();
    auto aadlFnct = new ivm::AADLObjectFunction("init");
    aadlModel->addObject(aadlFnct);
    result = m_checker->checkInstanceNames();
    QCOMPARE(result.size(), 1);
    QCOMPARE(m_checker->checkInstance(instance), false);

    // set instance name to the function name
    instance->setName("init");
    result = m_checker->checkInstanceNames();
    QCOMPARE(result.size(), 0);
    QCOMPARE(m_checker->checkInstance(instance), true);

    // Renaming the aadl function invalidates again
    aadlFnct->setTitle("Foo");
    result = m_checker->checkInstanceNames();
    QCOMPARE(result.size(), 1);
    QCOMPARE(m_checker->checkInstance(instance), false);
}

void tst_AadlSystemChecks::testCheckInstanceRelations()
{
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result = m_checker->checkInstanceRelations();
    QCOMPARE(result.size(), 0);

    result = m_checker->checkInstanceRelations();
    msc::MscChart *chart = m_mscCore->mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    QSharedPointer<ive::IVEditorCore> ivPlugin(new ive::IVEditorCore);
    m_checker->setIvCore(ivPlugin);
    result = m_checker->checkInstanceRelations();
    QCOMPARE(result.size(), 0);

    // Add instance2
    auto instance1 = new msc::MscInstance("init", chart);
    chart->addInstance(instance1);
    auto instance2 = new msc::MscInstance("reset", chart);
    chart->addInstance(instance2);
    result = m_checker->checkInstanceRelations();
    QCOMPARE(result.size(), 0);

    // Add function for the instances
    ive::InterfaceDocument *doc = ivPlugin->document();
    ivm::AADLObjectsModel *aadlModel = doc->objectsModel();
    auto function1 = new ivm::AADLObjectFunction("init");
    aadlModel->addObject(function1);
    auto function2 = new ivm::AADLObjectFunction("reset");
    aadlModel->addObject(function2);
    result = m_checker->checkInstanceRelations();
    QCOMPARE(result.size(), 0);

    // Make function2 be nested by function1
    function2->setParent(function1);
    result = m_checker->checkInstanceRelations();
    QCOMPARE(result.size(), 2);

    // Make function2 be nested by function1 via another one
    auto function15 = new ivm::AADLObjectFunction("init");
    aadlModel->addObject(function15);
    function15->setParent(function1);
    function2->setParent(function15);
    result = m_checker->checkInstanceRelations();
    QCOMPARE(result.size(), 2);
}

void tst_AadlSystemChecks::testCheckMessageNames()
{
    msc::MscChart *chart = m_mscCore->mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    QSharedPointer<ive::IVEditorCore> ivPlugin(new ive::IVEditorCore);
    m_checker->setIvCore(ivPlugin);
    QVector<QPair<msc::MscChart *, msc::MscMessage *>> result = m_checker->checkMessages();
    QCOMPARE(result.size(), 0);

    // Add Instance 'A' and message 'Msg1'
    auto instanceA = new msc::MscInstance("Instance A", chart);
    chart->addInstance(instanceA);
    auto message = new msc::MscMessage("Msg1", chart);
    message->setSourceInstance(instanceA);
    chart->addInstanceEvent(message);

    // Add Instance 'B'
    auto instanceB = new msc::MscInstance("Instance B", chart);
    chart->addInstance(instanceB);
    message->setTargetInstance(instanceB);

    result = m_checker->checkMessages();
    QCOMPARE(result.size(), 1);

    // Add function with different source/target
    ive::InterfaceDocument *doc = ivPlugin->document();
    ivm::AADLObjectsModel *aadlModel = doc->objectsModel();
    auto aadlfFuncA = new ivm::AADLObjectFunction("Instance A");
    aadlModel->addObject(aadlfFuncA);
    auto aadlfFuncB = new ivm::AADLObjectFunction("Instance B");
    aadlModel->addObject(aadlfFuncB);

    ivm::AADLObjectIface *requiredInterface =
            ivm::testutils::createIface(aadlfFuncA, ivm::AADLObjectIface::IfaceType::Required, "DummyA");
    aadlModel->addObject(requiredInterface);

    ivm::AADLObjectIface *providedInterface =
            ivm::testutils::createIface(aadlfFuncB, ivm::AADLObjectIface::IfaceType::Provided, "DummyB");
    aadlModel->addObject(providedInterface);
    aadlModel->addObject(new ivm::AADLObjectConnection(requiredInterface, providedInterface));
    result = m_checker->checkMessages();
    QCOMPARE(result.size(), 1);

    // Correct the name
    providedInterface->setTitle("Msg1");
    result = m_checker->checkMessages();
    QCOMPARE(result.size(), 0); // Everything is ok
}

void tst_AadlSystemChecks::testCorrespondMessage_data()
{
    using namespace ivm;
    QTest::addColumn<QString>("sourceFuncName");
    QTest::addColumn<QString>("sourceIfName");
    QTest::addColumn<AADLObjectIface::IfaceType>("sourceIfType");
    QTest::addColumn<QString>("targetFuncName");
    QTest::addColumn<QString>("targetIfName");
    QTest::addColumn<AADLObjectIface::IfaceType>("targetIfType");
    QTest::addColumn<bool>("expected");

    QTest::newRow("Correct") << "A"
                             << "msg" << AADLObjectIface::IfaceType::Required << "B"
                             << "msg" << AADLObjectIface::IfaceType::Provided << true;
    QTest::newRow("Source IF name ignored") << "A"
                                            << "IGNORED" << AADLObjectIface::IfaceType::Required << "B"
                                            << "msg" << AADLObjectIface::IfaceType::Provided << true;
    QTest::newRow("From name wrong") << "ERROR"
                                     << "msg" << AADLObjectIface::IfaceType::Required << "B"
                                     << "msg" << AADLObjectIface::IfaceType::Provided << false;
    QTest::newRow("To name wrong") << "A"
                                   << "msg" << AADLObjectIface::IfaceType::Required << "ERROR"
                                   << "msg" << AADLObjectIface::IfaceType::Provided << false;
    QTest::newRow("Source/targe flip irgnored") << "B"
                                                << "msg" << AADLObjectIface::IfaceType::Provided << "A"
                                                << "msg" << AADLObjectIface::IfaceType::Required << true;
}

void tst_AadlSystemChecks::testCorrespondMessage()
{
    using namespace ivm;
    QFETCH(QString, sourceFuncName);
    QFETCH(QString, sourceIfName);
    QFETCH(AADLObjectIface::IfaceType, sourceIfType);
    QFETCH(QString, targetFuncName);
    QFETCH(QString, targetIfName);
    QFETCH(AADLObjectIface::IfaceType, targetIfType);
    QFETCH(bool, expected);

    // Setup message "msg" from "A" to "B"
    auto instanceFrom = std::make_unique<msc::MscInstance>("A", nullptr);
    auto instanceTo = std::make_unique<msc::MscInstance>("B", nullptr);
    auto message = std::make_unique<msc::MscMessage>("msg", nullptr);
    message->setSourceInstance(instanceFrom.get());
    message->setTargetInstance(instanceTo.get());

    // Setup the connection
    auto sourceFunc = std::make_unique<AADLObjectFunction>(sourceFuncName);
    auto targetFunc = std::make_unique<AADLObjectFunction>(targetFuncName);
    std::unique_ptr<AADLObjectIface> sourceIf(
            ivm::testutils::createIface(sourceFunc.get(), sourceIfType, sourceIfName));
    std::unique_ptr<AADLObjectIface> targetIf(
            ivm::testutils::createIface(targetFunc.get(), targetIfType, targetIfName));
    auto connection = std::make_unique<AADLObjectConnection>(sourceIf.get(), targetIf.get());

    const bool doCorrespond = m_checker->correspond(connection.get(), message.get());
    QCOMPARE(doCorrespond, expected);
}

void tst_AadlSystemChecks::testCheckMessage()
{
    msc::MscChart *chart = m_mscCore->mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);

    // Add instance
    auto instance1 = new msc::MscInstance("Dummy1", chart);
    chart->addInstance(instance1);
    auto instance2 = new msc::MscInstance("Dummy2", chart);
    chart->addInstance(instance2);
    // Add message
    auto message = new msc::MscMessage("Msg1", chart);
    message->setSourceInstance(instance1);
    message->setTargetInstance(instance2);
    chart->addInstanceEvent(message);

    // Having no IVEditorCore, is ok for all messages
    QCOMPARE(m_checker->checkMessage(message), true);

    // Add IVEditorCore for real checks
    QSharedPointer<ive::IVEditorCore> ivPlugin(new ive::IVEditorCore);
    m_checker->setIvCore(ivPlugin);
    QCOMPARE(m_checker->checkMessage(message), false);

    // Create corresponding aadl model
    ive::InterfaceDocument *doc = ivPlugin->document();
    ivm::AADLObjectsModel *aadlModel = doc->objectsModel();
    auto sourceFunc = new ivm::AADLObjectFunction("Dummy1");
    aadlModel->addObject(sourceFunc);
    auto targetFunc = new ivm::AADLObjectFunction("Dummy2");
    aadlModel->addObject(targetFunc);
    ivm::AADLObjectConnection *connection = ivm::testutils::createConnection(sourceFunc, targetFunc, "Msg1");
    QCOMPARE(m_checker->checkMessage(message), true);
    aadlModel->removeObject(connection);
    delete connection;

    // Reverse direction fails
    ivm::testutils::createConnection(targetFunc, sourceFunc, "Msg1");
    QCOMPARE(m_checker->checkMessage(message), false);

    // Message from the environment
    auto message1 = new msc::MscMessage("Env1", chart);
    message1->setTargetInstance(instance1);
    chart->addInstanceEvent(message1);
    QCOMPARE(m_checker->checkMessage(message1), false);
    // Default interface is not ok
    ivm::AADLObjectIface *if1 =
            ivm::testutils::createIface(sourceFunc, ivm::AADLObjectIface::IfaceType::Provided, "Env1");
    QCOMPARE(m_checker->checkMessage(message1), false);
    // interface type has to be "cyclic"
    if1->setKind(ivm::AADLObjectIface::OperationKind::Cyclic);
    QCOMPARE(m_checker->checkMessage(message1), true);
}

QTEST_MAIN(tst_AadlSystemChecks)

#include "tst_aadlsystemchecks.moc"
