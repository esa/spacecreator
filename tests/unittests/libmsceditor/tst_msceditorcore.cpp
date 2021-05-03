/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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
#include "ivtestutils.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmodel.h"
#include "sharedlibrary.h"
#include "systemchecks.h"

#include <QPointer>
#include <QtTest>
#include <memory>

class tst_MSCEditorCore : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();

    void testCorrespondingInstances();
    void testCorrespondingMessages();

private:
    std::unique_ptr<msc::MSCEditorCore> m_mscCore;
    QPointer<msc::MscChart> m_chart;
};

class CheckerMock : public msc::SystemChecks
{
public:
    CheckerMock(QObject *parent = nullptr)
        : msc::SystemChecks(parent)
    {
    }
    bool correspond(const ivm::IVFunction *ivFunc, const msc::MscInstance *instance) const override
    {
        return instance->name() == ivFunc->title();
    }
    bool correspond(const ivm::IVConnection *connection, const msc::MscMessage *message) const override
    {
        return message->name() == connection->name();
    }
    bool hasValidSystem() const override { return true; }
};

void tst_MSCEditorCore::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    shared::initSharedLibrary();
}

void tst_MSCEditorCore::init()
{
    m_mscCore = std::make_unique<msc::MSCEditorCore>();
    m_mscCore->setSystemChecker(new CheckerMock(m_mscCore.get()));
    m_chart = m_mscCore->mainModel()->mscModel()->allCharts().at(0);
}

void tst_MSCEditorCore::testCorrespondingInstances()
{
    QCOMPARE(m_mscCore->correspondingInstances(nullptr).size(), 0);

    auto instance = new msc::MscInstance("dummy", m_chart);
    m_chart->addInstance(instance);
    ivm::IVFunction fnct("foo");
    QCOMPARE(m_mscCore->correspondingInstances(&fnct).size(), 0);

    fnct.setTitle("dummy");
    QCOMPARE(m_mscCore->correspondingInstances(&fnct).size(), 1);
}

void tst_MSCEditorCore::testCorrespondingMessages()
{
    QCOMPARE(m_mscCore->correspondingMessages(nullptr).size(), 0);

    auto instance1 = new msc::MscInstance("K1", m_chart);
    m_chart->addInstance(instance1);
    auto instance2 = new msc::MscInstance("K2", m_chart);
    m_chart->addInstance(instance2);
    auto message = new msc::MscMessage("ping", instance1, instance2, m_chart);
    m_chart->addInstanceEvent(message, { { instance1, -1 }, { instance2, -1 } });

    ivm::IVFunction f1("K1");
    ivm::IVFunction f2("K2");
    std::unique_ptr<ivm::IVConnection> connection(ivm::testutils::createConnection(&f1, &f2, "call"));
    QCOMPARE(m_mscCore->correspondingMessages(connection.get()).size(), 0);

    connection->targetInterface()->setTitle("ping");
    QCOMPARE(m_mscCore->correspondingMessages(connection.get()).size(), 1);
}

QTEST_MAIN(tst_MSCEditorCore)

#include "tst_msceditorcore.moc"
