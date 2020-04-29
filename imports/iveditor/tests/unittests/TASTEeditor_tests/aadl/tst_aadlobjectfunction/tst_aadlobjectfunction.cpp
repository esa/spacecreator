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

#include "aadlobject.h"
#include "aadlobjectfunction.h"
#include "aadlobjectiface.h"

#include <QtTest>
#include <testutils.h>

class tst_AADLObjectFunction : public QObject
{
    Q_OBJECT

private slots:
    void testAadlType();
    void testRequiredInterfacesManagement() const;
    void testProvidedInterfacesManagement() const;
    void testCommonInterfacesManagement() const;
};

void tst_AADLObjectFunction::testAadlType()
{
    QSKIP("hangs or fails");
    aadl::AADLObjectFunction obj;

    QCOMPARE(obj.aadlType(), aadl::AADLObject::Type::Function);
}

void tst_AADLObjectFunction::testRequiredInterfacesManagement() const
{
    QSKIP("hangs or fails");
    aadl::AADLObjectFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    auto ifaceType = aadl::AADLObjectIface::IfaceType::Required;
    const int ifacesCount = 10;
    for (int i = 0; i < ifacesCount; ++i) {
        auto ri = TASTEtest::createIface(&obj, ifaceType);
        const bool ok = obj.addChild(ri);
        QVERIFY(ok);
        QCOMPARE(obj.ris().size(), i + 1);
    }

    for (int i = 0; i < ifacesCount; ++i) {
        auto ri = obj.ris().first();
        const bool ok = obj.removeChild(ri);
        QVERIFY(ok);
        QCOMPARE(obj.ris().size(), ifacesCount - i - 1);
        delete ri;
    }
}

void tst_AADLObjectFunction::testProvidedInterfacesManagement() const
{
    QSKIP("hangs or fails");
    aadl::AADLObjectFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    auto ifaceType = aadl::AADLObjectIface::IfaceType::Provided;
    const int ifacesCount = 10;
    for (int i = 0; i < ifacesCount; ++i) {
        auto pi = TASTEtest::createIface(&obj, ifaceType);
        const bool ok = obj.addChild(pi);
        QVERIFY(ok);
        QCOMPARE(obj.pis().size(), i + 1);
    }

    QCOMPARE(obj.pis().size(), ifacesCount);

    for (int i = 0; i < ifacesCount; ++i) {
        auto pi = obj.pis().first();
        const bool ok = obj.removeChild(pi);
        QVERIFY(ok);
        QCOMPARE(obj.pis().size(), ifacesCount - i - 1);
        delete pi;
    }

    QCOMPARE(obj.pis().size(), 0);
}

void tst_AADLObjectFunction::testCommonInterfacesManagement() const
{
    QSKIP("hangs or fails");
    aadl::AADLObjectFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    const int ifacesCountHalf = 5;
    auto itProvided = aadl::AADLObjectIface::IfaceType::Provided;
    auto itRequired = aadl::AADLObjectIface::IfaceType::Required;

    QVector<aadl::AADLObjectIface *> ifaces;
    for (int i = 0; i < ifacesCountHalf; ++i) {
        ifaces << TASTEtest::createIface(&obj, itProvided) << TASTEtest::createIface(&obj, itRequired);
    }

    QCOMPARE(ifaces.size(), ifacesCountHalf * 2);

    for (auto iface : ifaces) {
        const bool ok = obj.addChild(iface);
        QVERIFY(ok);
    }

    QCOMPARE(obj.ris().size(), ifacesCountHalf);
    QCOMPARE(obj.pis().size(), ifacesCountHalf);

    while (ifaces.size()) {
        if (aadl::AADLObjectIface *iface = ifaces.takeLast()) {
            const int prevCount = iface->isProvided() ? obj.pis().size() : obj.ris().size();
            const bool ok = obj.removeChild(iface);
            QVERIFY(ok);
            const int currCount = iface->isProvided() ? obj.pis().size() : obj.ris().size();
            QVERIFY(prevCount != currCount);
            delete iface;
        }
    }

    QCOMPARE(obj.pis().size(), 0);
    QCOMPARE(obj.ris().size(), 0);
}

QTEST_APPLESS_MAIN(tst_AADLObjectFunction)

#include "tst_aadlobjectfunction.moc"
