/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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
#include "aadliface.h"
#include "aadlmodel.h"
#include "aadltestutils.h"
#include "interface/interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"

#include <QObject>
#include <QtTest>
#include <memory>

class tst_IVEditorCore : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();

    void test_addConnection();
    void test_addConnectionFromEnv();
    void test_addConnectionFails();
    void test_renameAadlConnection();
    void test_addToNestedConnection();
    void test_addFromNestedConnection();
    void test_addRootToNestedConnections();
    void test_addToExistingInterface();

private:
    std::unique_ptr<ive::IVEditorCore> ivCore;
};

void tst_IVEditorCore::initTestCase()
{
    ive::initIvEditor();
    QStandardPaths::setTestModeEnabled(true);
}

void tst_IVEditorCore::init()
{
    ivCore = std::make_unique<ive::IVEditorCore>();
}

void tst_IVEditorCore::test_addConnection()
{
    ivCore->addFunction("f1");
    ivCore->addFunction("f2");
    bool ok = ivCore->addConnection("m1", "f1", "f2");
    QCOMPARE(ok, true);
    QVector<ivm::AADLConnection *> connections =
            ivCore->document()->objectsModel()->allObjectsByType<ivm::AADLConnection>();
    QCOMPARE(connections.size(), 1);
    QCOMPARE(connections[0]->parentObject(), nullptr); // placed at root level
}

void tst_IVEditorCore::test_addConnectionFromEnv()
{
    ivm::AADLModel *aadlModel = ivCore->document()->objectsModel();
    ivCore->addFunction("f1");
    bool ok = ivCore->addConnection("m1", "", "f1");
    QCOMPARE(ok, true);
    QVector<ivm::AADLConnection *> connections = aadlModel->allObjectsByType<ivm::AADLConnection>();
    QCOMPARE(connections.size(), 0);

    QVector<ivm::AADLIface *> interfaces = aadlModel->allObjectsByType<ivm::AADLIface>();
    QCOMPARE(interfaces.size(), 1);

    // Adding it again, fails
    ok = ivCore->addConnection("m1", "", "f1");
    QCOMPARE(ok, false);
    interfaces = aadlModel->allObjectsByType<ivm::AADLIface>();
    QCOMPARE(interfaces.size(), 1);
}

void tst_IVEditorCore::test_addConnectionFails()
{
    // No function
    bool ok = ivCore->addConnection("m1", "f1", "f2");
    QCOMPARE(ok, false);

    // Only target function missing
    ivCore->addFunction("f1");
    ok = ivCore->addConnection("m1", "f1", "f2");
    QCOMPARE(ok, false);

    // Connection to the same function fails
    ok = ivCore->addConnection("m1", "f1", "f1");
    QCOMPARE(ok, false);

    // This should add the connection
    ivCore->addFunction("f2");
    ok = ivCore->addConnection("m1", "f1", "f2");
    QCOMPARE(ok, true);
    // Adding it again, fails
    ok = ivCore->addConnection("m1", "f1", "f2");
    QCOMPARE(ok, false);
}

void tst_IVEditorCore::test_renameAadlConnection()
{
    ivm::AADLFunction *funcF1 = ivCore->addFunction("f1");
    ivm::AADLFunction *funcF2 = ivCore->addFunction("f2");
    ivm::AADLConnection *connection = ivm::testutils::createConnection(funcF1, funcF2, "init");

    bool ok = ivCore->renameAadlConnection("init", "doIt", "f1", "f2");
    QCOMPARE(ok, true);
    QCOMPARE(connection->name(), "doIt");

    // cyclic interface only
    ivm::AADLIface *interface = ivCore->addInterface("push", "f2");
    ok = ivCore->renameAadlConnection("push", "call", "", "f2");
    QCOMPARE(ok, true);
    QCOMPARE(interface->title(), "call");
}

void tst_IVEditorCore::test_addToNestedConnection()
{
    ivm::AADLFunction *funcF1 = ivCore->addFunction("f1");
    ivCore->addFunction("f1a", funcF1);
    bool ok = ivCore->addConnection("m1", "f1", "f1a");
    QCOMPARE(ok, true);
    QVector<ivm::AADLConnection *> connections =
            ivCore->document()->objectsModel()->allObjectsByType<ivm::AADLConnection>();
    QCOMPARE(connections.size(), 1);
    QCOMPARE(connections[0]->parentObject(), funcF1); // placed as child of "f1"
}

void tst_IVEditorCore::test_addFromNestedConnection()
{
    ivm::AADLFunction *funcF1 = ivCore->addFunction("f1");
    ivCore->addFunction("f1a", funcF1);
    bool ok = ivCore->addConnection("m1", "f1a", "f1");
    QCOMPARE(ok, true);
    QVector<ivm::AADLConnection *> connections =
            ivCore->document()->objectsModel()->allObjectsByType<ivm::AADLConnection>();
    QCOMPARE(connections.size(), 1);
    QCOMPARE(connections[0]->parentObject(), funcF1); // placed as child of "f1"
}

void tst_IVEditorCore::test_addRootToNestedConnections()
{
    ivm::AADLFunction *funcF1 = ivCore->addFunction("f1");
    ivCore->addFunction("f1a", funcF1);
    ivm::AADLFunction *funcF2 = ivCore->addFunction("f2");
    ivCore->addFunction("f2a", funcF2);
    bool ok = ivCore->addConnection("m1", "f1a", "f2a");
    QCOMPARE(ok, true);
    QVector<ivm::AADLConnection *> connections =
            ivCore->document()->objectsModel()->allObjectsByType<ivm::AADLConnection>();
    QCOMPARE(connections.size(), 3);
}

void tst_IVEditorCore::test_addToExistingInterface()
{
    ivCore->addFunction("f1");
    ivCore->addFunction("f2");
    ivCore->addFunction("f3");
    bool ok = ivCore->addConnection("m1", "f1", "f2");
    QCOMPARE(ok, true);
    QVector<ivm::AADLConnection *> connections =
            ivCore->document()->objectsModel()->allObjectsByType<ivm::AADLConnection>();
    QCOMPARE(connections.size(), 1);

    ok = ivCore->addConnection("m1", "f3", "f2");
    QCOMPARE(ok, true);
    connections = ivCore->document()->objectsModel()->allObjectsByType<ivm::AADLConnection>();
    QCOMPARE(connections.size(), 2);
}

QTEST_MAIN(tst_IVEditorCore)

#include "tst_iveditorcore.moc"
