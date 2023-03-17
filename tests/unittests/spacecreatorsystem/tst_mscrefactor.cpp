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

#include "ivconnection.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditor.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmodel.h"
#include "mscrefactor.h"
#include "mscsystemchecks.h"
#include "sharedlibrary.h"
#include "spacecreatorproject.h"
#include "standardpaths.h"

#include <QtTest>

using namespace msc;

class tst_MSCRefactor : public QObject
{
    Q_OBJECT

private:
    msc::MscChart *mscChart() const;
    msc::MscInstance *firstInstance() const;
    msc::MscMessage *mscMessage(const QString &name) const;

private Q_SLOTS:
    void initTestCase();
    void init();

    void testFunctionUsed();
    void testFunctionWasRenamed();
    void testMessagesUsed();
    void testInterfaceWasRenamed();
    void testIVConnectionUsed();
    void testIVObjectRemoved();

private:
    std::unique_ptr<scs::MSCRefactor> m_mscRefactor;
    std::unique_ptr<scs::SpaceCreatorProject> m_project;
    std::unique_ptr<scs::MscSystemChecks> m_mscChecks;
    std::unique_ptr<ivm::IVModel> m_ivModel;
};

void tst_MSCRefactor::initTestCase()
{
    shared::StandardPaths::setTestModeEnabled(true);
    msc::initMscEditor();
    shared::initSharedLibrary();
}

MscChart *tst_MSCRefactor::mscChart() const
{
    msc::MscModel *mscModel = m_project->allMscCores().at(0)->mainModel()->mscModel();
    return mscModel->firstChart();
}

MscInstance *tst_MSCRefactor::firstInstance() const
{
    return mscChart()->instances().at(0);
}

MscMessage *tst_MSCRefactor::mscMessage(const QString &name) const
{
    return mscChart()->messageByName(name);
}

void tst_MSCRefactor::init()
{
    m_mscRefactor = std::make_unique<scs::MSCRefactor>();
    m_project = std::make_unique<scs::SpaceCreatorProject>();
    m_mscChecks = std::make_unique<scs::MscSystemChecks>();
    m_mscChecks->setStorage(m_project.get());

    m_mscRefactor->setStorage(m_project.get());

    m_ivModel = std::make_unique<ivm::IVModel>(ivm::IVPropertyTemplateConfig::instance());

    const QString mscFileName = QFINDTESTDATA("/Taste_refactor.msc");
    m_project->mscData(mscFileName);
}

void tst_MSCRefactor::testFunctionUsed()
{
    QCOMPARE(m_mscRefactor->isIVFunctionUsed(nullptr, "foo"), false);
    QCOMPARE(m_mscRefactor->isIVFunctionUsed(nullptr, "Function_1"), true);
}

void tst_MSCRefactor::testFunctionWasRenamed()
{
    const QString oldName = "Function_1";
    const QString newName = "Function_A";

    auto ivFunc1 = ivm::testutils::createFunction(oldName);
    m_ivModel->addObject(ivFunc1);
    QCOMPARE(firstInstance()->name(), oldName);

    ivFunc1->setTitle(newName);
    m_mscRefactor->onIVFunctionRenamed(ivFunc1, oldName, newName);
    QCOMPARE(firstInstance()->name(), newName);
}

void tst_MSCRefactor::testMessagesUsed()
{
    auto ivFunc1 = ivm::testutils::createFunction("Function_1");
    m_ivModel->addObject(ivFunc1);
    auto ivFuncB = ivm::testutils::createFunction("Function_B");
    m_ivModel->addObject(ivFuncB);
    auto ivFunc33 = ivm::testutils::createFunction("Function_33");
    m_ivModel->addObject(ivFunc33);

    {
        ivm::IVConnection *connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, "init");
        ivm::IVInterface *interface = connection->targetInterface();
        QCOMPARE(m_mscRefactor->isIVInterfaceUsed(interface, "nope"), false);
        QCOMPARE(m_mscRefactor->isIVInterfaceUsed(interface, "init"), true);
    }

    // Find when the name was changed ("done" in MSC is "finished" in IV)
    {
        ivm::IVConnection *connection = ivm::testutils::createConnection(ivFunc33, ivFunc1, "finished");
        ivm::IVInterface *interface = connection->targetInterface();
        QCOMPARE(m_mscRefactor->isIVInterfaceUsed(interface, "done"), true);
    }
}

void tst_MSCRefactor::testInterfaceWasRenamed()
{
    const QString oldName = "init";
    const QString newName = "start";

    auto ivFuncB = ivm::testutils::createFunction("Function_B");
    m_ivModel->addObject(ivFuncB);
    auto ivFunc33 = ivm::testutils::createFunction("Function_33");
    m_ivModel->addObject(ivFunc33);
    ivm::IVConnection *connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, oldName);
    ivm::IVInterface *ivIface = connection->targetInterface();

    msc::MscMessage *msg = mscMessage(oldName);
    QCOMPARE_NE(msg, nullptr);

    ivIface->setTitle(newName);
    m_mscRefactor->onIVInterfaceRenamed(ivIface, oldName, newName);
    QCOMPARE(msg->name(), newName);
}

void tst_MSCRefactor::testIVConnectionUsed()
{
    auto ivFuncB = ivm::testutils::createFunction("Function_B");
    m_ivModel->addObject(ivFuncB);
    auto ivFunc33 = ivm::testutils::createFunction("Function_33");
    m_ivModel->addObject(ivFunc33);

    ivm::IVConnection *connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, "init");
    QCOMPARE(m_mscRefactor->isIVConnectionUsed(connection), true);

    connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, "nonsense");
    QCOMPARE(m_mscRefactor->isIVConnectionUsed(connection), false);
}

void tst_MSCRefactor::testIVObjectRemoved()
{
    const QString name = "init";
    auto ivFuncB = ivm::testutils::createFunction("Function_B");
    m_ivModel->addObject(ivFuncB);
    auto ivFunc33 = ivm::testutils::createFunction("Function_33");
    m_ivModel->addObject(ivFunc33);

    ivm::IVConnection *connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, name);
    msc::MscMessage *msg = mscMessage(name);
    QCOMPARE_NE(msg, nullptr);

    m_ivModel->removeObject(connection);
    m_mscRefactor->onRemovingIVObject(connection);
    delete connection;
    msg = mscMessage(name);
    QCOMPARE(msg, nullptr);

    QPointer<msc::MscInstance> instance = firstInstance();
    QCOMPARE(instance.isNull(), false);

    m_ivModel->removeObject(ivFuncB);
    m_mscRefactor->onRemovingIVObject(ivFuncB);
    delete instance;
    QCOMPARE(instance.isNull(), true);
}

QTEST_MAIN(tst_MSCRefactor)

#include "tst_mscrefactor.moc"
