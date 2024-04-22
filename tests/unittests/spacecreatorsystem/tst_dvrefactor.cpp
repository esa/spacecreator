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

#include "dvappmodel.h"
#include "dveditor.h"
#include "dveditorcore.h"
#include "dvfunction.h"
#include "dvlibrary.h"
#include "dvmessage.h"
#include "dvmodel.h"
#include "dvrefactor.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"
#include "sharedlibrary.h"
#include "spacecreatorproject.h"
#include "standardpaths.h"

#include <QtTest>
#include <dvconnection.h>

using namespace msc;

class tst_DVRefactor : public QObject
{
    Q_OBJECT
private:
    dvm::DVFunction *dvFunction(const QString &name) const;
    dvm::DVMessage *firstMessage() const;

private Q_SLOTS:
    void initTestCase();
    void init();

    void testFunctionUsed();
    void testFunctionWasRenamed();
    void testInterfaceUsed();
    void testInterfaceWasRenamed();
    void testIVConnectionUsed();
    void testIVObjectRemoved();

private:
    std::unique_ptr<scs::DVRefactor> m_dvRefactor;
    std::unique_ptr<scs::SpaceCreatorProject> m_project;
    std::unique_ptr<ivm::IVModel> m_ivModel;
};

void tst_DVRefactor::initTestCase()
{
    shared::StandardPaths::setTestModeEnabled(true);
    shared::initSharedLibrary();
    dvm::initDVLibrary();
    dve::initDvEditor();
}

dvm::DVFunction *tst_DVRefactor::dvFunction(const QString &name) const
{
    dvm::DVModel *model = m_project->allDVCores().at(0)->appModel()->objectsModel();
    return dynamic_cast<dvm::DVFunction *>(model->getObjectByName(name, dvm::DVObject::Type::Function));
}

dvm::DVMessage *tst_DVRefactor::firstMessage() const
{
    dvm::DVModel *model = m_project->allDVCores().at(0)->appModel()->objectsModel();
    if (!model) {
        return nullptr;
    }
    dvm::DVConnection *connection = model->connectionByName("Connection_1");
    if (!connection) {
        return nullptr;
    }
    return connection->messages().isEmpty() ? nullptr : connection->messages().at(0);
}

void tst_DVRefactor::init()
{
    m_dvRefactor = std::make_unique<scs::DVRefactor>();
    m_project = std::make_unique<scs::SpaceCreatorProject>();

    m_dvRefactor->setStorage(m_project.get());

    m_ivModel = std::make_unique<ivm::IVModel>(ivm::IVPropertyTemplateConfig::instance());

    const QString dvFileName = QFINDTESTDATA("/deploy.dv.xml");
    m_project->dvData(dvFileName);
}

void tst_DVRefactor::testFunctionUsed()
{
    QCOMPARE(m_dvRefactor->isIVFunctionUsed(nullptr, "foo"), false);
    QCOMPARE(m_dvRefactor->isIVFunctionUsed(nullptr, "Function_B"), true);
}

void tst_DVRefactor::testFunctionWasRenamed()
{
    const QString oldName = "Function_B";
    const QString newName = "Function_2";

    auto ivFunc1 = ivm::testutils::createFunction(oldName, m_ivModel.get());
    dvm::DVFunction *dvFunc = dvFunction(oldName);
    QCOMPARE_NE(dvFunc, nullptr);

    ivFunc1->setTitle(newName);
    m_dvRefactor->onIVFunctionRenamed(ivFunc1, oldName, newName);
    QCOMPARE(dvFunc->title(), newName);
}

void tst_DVRefactor::testInterfaceUsed()
{
    auto ivFuncB = ivm::testutils::createFunction("Function_B", m_ivModel.get());
    auto ivFunc33 = ivm::testutils::createFunction("Function_33", m_ivModel.get());
    ivm::IVConnection *connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, "init");
    ivm::IVInterface *interface = connection->targetInterface();
    QCOMPARE(m_dvRefactor->isIVInterfaceUsed(interface, "nope"), false);
    QCOMPARE(m_dvRefactor->isIVInterfaceUsed(interface, "init"), true);
}

void tst_DVRefactor::testInterfaceWasRenamed()
{
    const QString oldName = "init";
    const QString newName = "start";

    auto ivFuncB = ivm::testutils::createFunction("Function_B", m_ivModel.get());
    auto ivFunc33 = ivm::testutils::createFunction("Function_33", m_ivModel.get());
    ivm::IVConnection *connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, oldName);
    ivm::IVInterface *ivIface = connection->targetInterface();

    dvm::DVMessage *dvMsg = firstMessage();
    QCOMPARE_NE(dvMsg, nullptr);

    ivIface->setTitle(newName);
    m_dvRefactor->onIVInterfaceRenamed(ivIface, oldName, newName);
    QCOMPARE(dvMsg->toInterface(), newName);
}

void tst_DVRefactor::testIVConnectionUsed()
{
    auto ivFuncB = ivm::testutils::createFunction("Function_B", m_ivModel.get());
    auto ivFunc33 = ivm::testutils::createFunction("Function_33", m_ivModel.get());

    ivm::IVConnection *connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, "init");
    QCOMPARE(m_dvRefactor->isIVConnectionUsed(connection), true);

    connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, "nonsense");
    QCOMPARE(m_dvRefactor->isIVConnectionUsed(connection), false);
}

void tst_DVRefactor::testIVObjectRemoved()
{
    auto ivFuncB = ivm::testutils::createFunction("Function_B", m_ivModel.get());
    auto ivFunc33 = ivm::testutils::createFunction("Function_33", m_ivModel.get());
    ivm::IVConnection *connection = ivm::testutils::createConnection(ivFuncB, ivFunc33, "init");

    dvm::DVMessage *dvMsg = firstMessage();
    QCOMPARE_NE(dvMsg, nullptr);

    m_ivModel->removeObject(connection);
    m_dvRefactor->onRemovingIVObject(connection);
    delete connection;
    dvMsg = firstMessage();
    QCOMPARE(dvMsg, nullptr);
}

QTEST_MAIN(tst_DVRefactor)

#include "tst_dvrefactor.moc"
