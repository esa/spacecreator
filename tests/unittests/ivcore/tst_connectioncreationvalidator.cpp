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

#include "connectioncreationvalidator.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivpropertytemplateconfig.h"
#include "ivtestutils.h"
#include "standardpaths.h"

#include <QTest>
#include <memory.h>

using namespace ivm;

class tst_ConnectionCreationValidator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();

    void tst_cyclicInterface();
    void tst_cyclicNestedInterface();

private:
    ivm::IVPropertyTemplateConfig *cfg { nullptr };
    std::unique_ptr<IVModel> m_model;
};

void tst_ConnectionCreationValidator::initTestCase()
{
    shared::StandardPaths::setTestModeEnabled(true);
    ivm::initIVLibrary();
    cfg = ivm::IVPropertyTemplateConfig::instance();
    cfg->init(QLatin1String("default_attributes.xml"));
}

void tst_ConnectionCreationValidator::init()
{
    m_model = std::make_unique<ivm::IVModel>(cfg);
}

void tst_ConnectionCreationValidator::tst_cyclicInterface()
{
    IVFunction *fnA = testutils::createFunction("A", m_model.get());
    IVFunction *fnB = testutils::createFunction("B", m_model.get());

    IVInterfaceProvided *piIf = testutils::createProvidedIface(fnA, "cycle");
    piIf->setKind(IVInterface::OperationKind::Cyclic);

    const ConnectionCreationValidator::FailReason result =
            ConnectionCreationValidator::canConnect(fnB, fnA, piIf, nullptr);
    QCOMPARE(result, ConnectionCreationValidator::FailReason::IsCyclic);
}

void tst_ConnectionCreationValidator::tst_cyclicNestedInterface()
{
    IVFunction *parentFn = testutils::createFunction("parentFn", m_model.get());
    IVFunction *nestedFn = testutils::createFunction("nestedFn", parentFn);

    IVInterfaceProvided *piIf = testutils::createProvidedIface(parentFn, "cycle");
    piIf->setKind(IVInterface::OperationKind::Cyclic);

    const ConnectionCreationValidator::FailReason result =
            ConnectionCreationValidator::canConnect(parentFn, nestedFn, piIf, nullptr);
    QCOMPARE(result, ConnectionCreationValidator::FailReason::NotFail);
}

QTEST_APPLESS_MAIN(tst_ConnectionCreationValidator)

#include "tst_connectioncreationvalidator.moc"
