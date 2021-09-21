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

#include "context/action/actionsmanager.h"
#include "interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "ivlibrary.h"
#include "ivsystemchecks.h"
#include "ivsystemqueries.h"
#include "sharedlibrary.h"

#include <QSharedPointer>
#include <QtTest>
#include <memory>

using namespace ivm;

class tst_IvSystemQueries : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

    void testFunctionByName();
    void testConnectedProtectedFunctions();

private:
    std::unique_ptr<scs::IvSystemQueries> m_query;
    IVEditorCorePtr m_ivCore;
};

void tst_IvSystemQueries::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    shared::initSharedLibrary();
    ivm::initIVLibrary();
    ive::initIVEditor();
}

void tst_IvSystemQueries::init()
{
    m_query = std::make_unique<scs::IvSystemQueries>(nullptr);
    m_ivCore.reset(new ive::IVEditorCore);
    m_query->setIVCore(m_ivCore);
}

void tst_IvSystemQueries::cleanup()
{
    ive::ActionsManager::instance()->clearActions();
}

void tst_IvSystemQueries::testFunctionByName()
{
    const QString ivFileName = QFINDTESTDATA("/interfaceview_protect.xml");
    m_ivCore->document()->load(ivFileName);

    QCOMPARE(m_query->functionByName("dummy name"), nullptr);
    QVERIFY(m_query->functionByName("Function_B"));
}

void tst_IvSystemQueries::testConnectedProtectedFunctions()
{
    const QString ivFileName = QFINDTESTDATA("/interfaceview_protect.xml");
    m_ivCore->document()->load(ivFileName);

    QList<ivm::IVFunction *> functions = m_query->connectedProtectedFunctions("dummy name");
    QCOMPARE(functions.size(), 0);

    functions = m_query->connectedProtectedFunctions("Function_G");
    QCOMPARE(functions.size(), 0);

    functions = m_query->connectedProtectedFunctions("Function_B");
    QCOMPARE(functions.size(), 1);

    functions = m_query->connectedProtectedFunctions("Function_33");
    QCOMPARE(functions.size(), 2);
}

QTEST_MAIN(tst_IvSystemQueries)

#include "tst_ivsystemqueries.moc"
