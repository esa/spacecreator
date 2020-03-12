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

#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectfunction.h"
#include "tab_aadl/aadlobjectiface.h"

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
    using namespace taste3::aadl;
    AADLObjectFunction obj;

    QCOMPARE(obj.aadlType(), AADLObject::Type::Function);
}

void tst_AADLObjectFunction::testRequiredInterfacesManagement() const
{
    using namespace taste3::aadl;
    AADLObjectFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    static constexpr AADLObjectIface::IfaceType ifaceType = AADLObjectIface::IfaceType::Required;
    static constexpr int ifacesCount { 10 };
    for (int i = 0; i < ifacesCount; ++i) {
        AADLObjectIface *ri = TASTEtest::createIface(&obj, ifaceType);
        const bool ok = obj.addChild(ri);
        QVERIFY(ok);
        QCOMPARE(obj.ris().size(), i + 1);
    }

    for (int i = 0; i < ifacesCount; ++i) {
        AADLObjectIface *ri = obj.ris().first();
        const bool ok = obj.removeChild(ri);
        QVERIFY(ok);
        QCOMPARE(obj.ris().size(), ifacesCount - i - 1);
        delete ri;
    }
}

void tst_AADLObjectFunction::testProvidedInterfacesManagement() const
{
    using namespace taste3::aadl;
    AADLObjectFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    static constexpr AADLObjectIface::IfaceType ifaceType = AADLObjectIface::IfaceType::Provided;
    static constexpr int ifacesCount { 10 };
    for (int i = 0; i < ifacesCount; ++i) {
        AADLObjectIface *pi = TASTEtest::createIface(&obj, ifaceType);
        const bool ok = obj.addChild(pi);
        QVERIFY(ok);
        QCOMPARE(obj.pis().size(), i + 1);
    }

    QCOMPARE(obj.pis().size(), ifacesCount);

    for (int i = 0; i < ifacesCount; ++i) {
        AADLObjectIface *pi = obj.pis().first();
        const bool ok = obj.removeChild(pi);
        QVERIFY(ok);
        QCOMPARE(obj.pis().size(), ifacesCount - i - 1);
        delete pi;
    }

    QCOMPARE(obj.pis().size(), 0);
}

void tst_AADLObjectFunction::testCommonInterfacesManagement() const
{
    using namespace taste3::aadl;
    AADLObjectFunction obj;

    QCOMPARE(obj.ris().size(), 0);

    static constexpr int ifacesCountHalf { 5 };
    static constexpr AADLObjectIface::IfaceType itProvided = AADLObjectIface::IfaceType::Provided;
    static constexpr AADLObjectIface::IfaceType itRequired = AADLObjectIface::IfaceType::Required;

    QVector<AADLObjectIface *> ifaces;
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
        if (AADLObjectIface *iface = ifaces.takeLast()) {
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
