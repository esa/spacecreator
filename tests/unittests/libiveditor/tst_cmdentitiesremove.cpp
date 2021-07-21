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

#include "commands/cmdentitiesremove.h"
#include "commandsstack.h"
#include "context/action/actionsmanager.h"
#include "interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivtestutils.h"
#include "sharedlibrary.h"

#include <QObject>
#include <QtTest>
#include <memory>

class tst_CmdEntitiesRemove : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

    void test_removeFunction();
    void test_removeEntityTwice();

private:
    std::unique_ptr<ive::IVEditorCore> ivCore;
};

void tst_CmdEntitiesRemove::initTestCase()
{
    ive::initIVEditor();
    shared::initSharedLibrary();
    QStandardPaths::setTestModeEnabled(true);
}

void tst_CmdEntitiesRemove::init()
{
    ivCore = std::make_unique<ive::IVEditorCore>();
}

void tst_CmdEntitiesRemove::cleanup()
{
    ive::ActionsManager::instance()->clearActions();
}

void tst_CmdEntitiesRemove::test_removeFunction()
{
    ivm::IVModel *ivModel = ivCore->document()->objectsModel();
    QPointer<ivm::IVFunction> fn = ivCore->addFunction("f1");
    ivModel->addObject(fn);

    QList<QPointer<ivm::IVObject>> entities;
    entities.append(fn.data());
    auto cmdRemove = new ive::cmd::CmdEntitiesRemove(entities, ivModel);
    ivCore->commandsStack()->push(cmdRemove);
    QCOMPARE(fn->parent(), cmdRemove);

    ivCore->commandsStack()->clear();
    QVERIFY(fn.isNull());
}

void tst_CmdEntitiesRemove::test_removeEntityTwice()
{
    ivm::IVModel *ivModel = ivCore->document()->objectsModel();

    QPointer<ivm::IVFunction> fn = ivCore->addFunction("f1");
    QPointer<ivm::IVInterface> pi1 = ivm::testutils::createProvidedIface(fn, "PI_1");
    fn->addChild(pi1);
    ivModel->addObject(pi1);

    QList<QPointer<ivm::IVObject>> entities;
    entities.append(pi1.data());
    auto cmdRemove1 = new ive::cmd::CmdEntitiesRemove(entities, ivModel);
    ivCore->commandsStack()->push(cmdRemove1);
    QCOMPARE(pi1->parent(), cmdRemove1);

    ivCore->commandsStack()->undo();
    QCOMPARE(pi1->parent(), fn.data());

    auto cmdRemove2 = new ive::cmd::CmdEntitiesRemove(entities, ivModel);
    ivCore->commandsStack()->push(cmdRemove2);
    QCOMPARE(pi1->parent(), cmdRemove2);
}

QTEST_MAIN(tst_CmdEntitiesRemove)

#include "tst_cmdentitiesremove.moc"
