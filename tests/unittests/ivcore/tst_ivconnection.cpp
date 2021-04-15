/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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
#include "ivtestutils.h"

#include <QtTest>

class tst_IVConnection : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testPostInitEmptyConnection();
    void testCorrectConnectionSourceTarget();
};

void tst_IVConnection::testPostInitEmptyConnection()
{
    ivm::IVConnection connection(nullptr, nullptr);
    const bool result = connection.postInit();
    // postInit() for connections without source/target fails
    QCOMPARE(result, false);
}

void tst_IVConnection::testCorrectConnectionSourceTarget()
{
    ivm::IVFunction fn1("Fn1");
    ivm::IVInterface *rifn1 = ivm::testutils::createIface(&fn1, ivm::IVInterface::InterfaceType::Required, "RI1");
    ivm::IVInterface *pifn1 = ivm::testutils::createIface(&fn1, ivm::IVInterface::InterfaceType::Provided, "PI1");
    ivm::IVFunction fn2("Fn2");
    ivm::IVInterface *rifn2 = ivm::testutils::createIface(&fn2, ivm::IVInterface::InterfaceType::Required, "RI1");
    ivm::IVInterface *pifn2 = ivm::testutils::createIface(&fn2, ivm::IVInterface::InterfaceType::Provided, "PI1");

    // Correct as is (PI/RI)
    ivm::IVConnection c1(rifn1, pifn2);
    QCOMPARE(c1.sourceInterface(), rifn1);
    QCOMPARE(c1.targetInterface(), pifn2);
    ivm::IVConnection c2(rifn2, pifn1);
    QCOMPARE(c2.sourceInterface(), rifn2);
    QCOMPARE(c2.targetInterface(), pifn1);
    // Corrected simple PI/RI
    ivm::IVConnection c3(pifn2, rifn1);
    QCOMPARE(c3.sourceInterface(), rifn1);
    QCOMPARE(c3.targetInterface(), pifn2);
    ivm::IVConnection c4(pifn1, rifn2);
    QCOMPARE(c4.sourceInterface(), rifn2);
    QCOMPARE(c4.targetInterface(), pifn1);

    // Parent child relations
    ivm::IVFunction childFn1("ChildFn1", &fn1);
    ivm::IVInterface *childFnRI = ivm::testutils::createIface(&childFn1, ivm::IVInterface::InterfaceType::Required, "RI1");
    ivm::IVInterface *childFnPI = ivm::testutils::createIface(&childFn1, ivm::IVInterface::InterfaceType::Provided, "PI1");
    // Correct as is (RI/PI connections)
    ivm::IVConnection c5(childFnRI, pifn1);
    QCOMPARE(c5.sourceInterface(), childFnRI);
    QCOMPARE(c5.targetInterface(), pifn1);
    ivm::IVConnection c6(rifn1, childFnPI);
    QCOMPARE(c6.sourceInterface(), rifn1);
    QCOMPARE(c6.targetInterface(), childFnPI);
    // Correct child-parent (PI/RI connections)
    ivm::IVConnection c7(childFnPI, rifn1);
    QCOMPARE(c7.sourceInterface(), rifn1);
    QCOMPARE(c7.targetInterface(), childFnPI);
    ivm::IVConnection c8(pifn1, childFnRI);
    QCOMPARE(c8.sourceInterface(), childFnRI);
    QCOMPARE(c8.targetInterface(), pifn1);

    // PI/PI connection
    ivm::IVConnection c9(childFnPI, pifn1);
    QCOMPARE(c9.sourceInterface(), pifn1);
    QCOMPARE(c9.targetInterface(), childFnPI);
    ivm::IVConnection c10(pifn1, childFnPI);
    QCOMPARE(c10.sourceInterface(), pifn1);
    QCOMPARE(c10.targetInterface(), childFnPI);

    // RI/RI connection
    ivm::IVConnection c11(rifn1, childFnRI);
    QCOMPARE(c11.sourceInterface(), childFnRI);
    QCOMPARE(c11.targetInterface(), rifn1);
    ivm::IVConnection c12(childFnRI, rifn1);
    QCOMPARE(c12.sourceInterface(), childFnRI);
    QCOMPARE(c12.targetInterface(), rifn1);
}

QTEST_APPLESS_MAIN(tst_IVConnection)

#include "tst_ivconnection.moc"
