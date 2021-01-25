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

#include "aadlfunction.h"
#include "aadliface.h"
#include "aadlobject.h"
#include "aadltestutils.h"

#include <QtTest>

class tst_AADLFunction : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testAadlType();
    void testRequiredInterfacesManagement() const;
    void testProvidedInterfacesManagement() const;
    void testCommonInterfacesManagement() const;
};

void tst_AADLFunction::testAadlType()
{
    ivm::AADLFunction obj;

    QCOMPARE(obj.aadlType(), ivm::AADLObject::Type::Function);
}

void tst_AADLFunction::testRequiredInterfacesManagement() const
{
    ivm::AADLFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    auto ifaceType = ivm::AADLIface::IfaceType::Required;
    const int ifacesCount = 10;
    for (int i = 0; i < ifacesCount; ++i) {
        auto ri = ivm::testutils::createIface(&obj, ifaceType);
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

void tst_AADLFunction::testProvidedInterfacesManagement() const
{
    ivm::AADLFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    auto ifaceType = ivm::AADLIface::IfaceType::Provided;
    const int ifacesCount = 10;
    for (int i = 0; i < ifacesCount; ++i) {
        auto pi = ivm::testutils::createIface(&obj, ifaceType);
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

void tst_AADLFunction::testCommonInterfacesManagement() const
{
    ivm::AADLFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    const int ifacesCountHalf = 5;
    auto itProvided = ivm::AADLIface::IfaceType::Provided;
    auto itRequired = ivm::AADLIface::IfaceType::Required;

    QVector<ivm::AADLIface *> ifaces;
    for (int i = 0; i < ifacesCountHalf; ++i) {
        ifaces << ivm::testutils::createIface(&obj, itProvided) << ivm::testutils::createIface(&obj, itRequired);
    }

    QCOMPARE(ifaces.size(), ifacesCountHalf * 2);

    for (auto iface : ifaces) {
        const bool ok = obj.addChild(iface);
        QVERIFY(ok);
    }

    QCOMPARE(obj.ris().size(), ifacesCountHalf);
    QCOMPARE(obj.pis().size(), ifacesCountHalf);

    while (ifaces.size()) {
        if (ivm::AADLIface *iface = ifaces.takeLast()) {
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

QTEST_APPLESS_MAIN(tst_AADLFunction)

#include "tst_aadlfunction.moc"
