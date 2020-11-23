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

#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"
#include "aadltestutils.h"

#include <QtTest>

class tst_AADLObjectConnection : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testPostInitEmptyConnection();
    void testCorrectConnectionSourceTarget();
};

void tst_AADLObjectConnection::testPostInitEmptyConnection()
{
    aadl::AADLObjectConnection connection(nullptr, nullptr);
    const bool result = connection.postInit();
    // postInit() for connections without source/target fails
    QCOMPARE(result, false);
}

void tst_AADLObjectConnection::testCorrectConnectionSourceTarget()
{
    aadl::AADLObjectFunction fn1("Fn1");
    aadl::AADLObjectIface *rifn1 =
            aadl::testutils::createIface(&fn1, aadl::AADLObjectIface::IfaceType::Required, "RI1");
    aadl::AADLObjectIface *pifn1 =
            aadl::testutils::createIface(&fn1, aadl::AADLObjectIface::IfaceType::Provided, "PI1");
    aadl::AADLObjectFunction fn2("Fn2");
    aadl::AADLObjectIface *rifn2 =
            aadl::testutils::createIface(&fn2, aadl::AADLObjectIface::IfaceType::Required, "RI1");
    aadl::AADLObjectIface *pifn2 =
            aadl::testutils::createIface(&fn2, aadl::AADLObjectIface::IfaceType::Provided, "PI1");

    // Correct as is (PI/RI)
    aadl::AADLObjectConnection c1(rifn1, pifn2);
    QCOMPARE(c1.sourceInterface(), rifn1);
    QCOMPARE(c1.targetInterface(), pifn2);
    aadl::AADLObjectConnection c2(rifn2, pifn1);
    QCOMPARE(c2.sourceInterface(), rifn2);
    QCOMPARE(c2.targetInterface(), pifn1);
    // Corrected simple PI/RI
    aadl::AADLObjectConnection c3(pifn2, rifn1);
    QCOMPARE(c3.sourceInterface(), rifn1);
    QCOMPARE(c3.targetInterface(), pifn2);
    aadl::AADLObjectConnection c4(pifn1, rifn2);
    QCOMPARE(c4.sourceInterface(), rifn2);
    QCOMPARE(c4.targetInterface(), pifn1);

    // Parent child relations
    aadl::AADLObjectFunction childFn1("ChildFn1", &fn1);
    aadl::AADLObjectIface *childFnRI =
            aadl::testutils::createIface(&childFn1, aadl::AADLObjectIface::IfaceType::Required, "RI1");
    aadl::AADLObjectIface *childFnPI =
            aadl::testutils::createIface(&childFn1, aadl::AADLObjectIface::IfaceType::Provided, "PI1");
    // Correct as is (RI/PI connections)
    aadl::AADLObjectConnection c5(childFnRI, pifn1);
    QCOMPARE(c5.sourceInterface(), childFnRI);
    QCOMPARE(c5.targetInterface(), pifn1);
    aadl::AADLObjectConnection c6(rifn1, childFnPI);
    QCOMPARE(c6.sourceInterface(), rifn1);
    QCOMPARE(c6.targetInterface(), childFnPI);
    // Correct child-parent (PI/RI connections)
    aadl::AADLObjectConnection c7(childFnPI, rifn1);
    QCOMPARE(c7.sourceInterface(), rifn1);
    QCOMPARE(c7.targetInterface(), childFnPI);
    aadl::AADLObjectConnection c8(pifn1, childFnRI);
    QCOMPARE(c8.sourceInterface(), childFnRI);
    QCOMPARE(c8.targetInterface(), pifn1);

    // PI/PI connection
    aadl::AADLObjectConnection c9(childFnPI, pifn1);
    QCOMPARE(c9.sourceInterface(), pifn1);
    QCOMPARE(c9.targetInterface(), childFnPI);
    aadl::AADLObjectConnection c10(pifn1, childFnPI);
    QCOMPARE(c10.sourceInterface(), pifn1);
    QCOMPARE(c10.targetInterface(), childFnPI);

    // RI/RI connection
    aadl::AADLObjectConnection c11(rifn1, childFnRI);
    QCOMPARE(c11.sourceInterface(), childFnRI);
    QCOMPARE(c11.targetInterface(), rifn1);
    aadl::AADLObjectConnection c12(childFnRI, rifn1);
    QCOMPARE(c12.sourceInterface(), childFnRI);
    QCOMPARE(c12.targetInterface(), rifn1);
}

QTEST_APPLESS_MAIN(tst_AADLObjectConnection)

#include "tst_aadlobjectconnection.moc"
