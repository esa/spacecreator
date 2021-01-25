/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscgate.h"
#include "mscinstance.h"

#include <QMetaObject>
#include <QtTest>

using namespace msc;

class tst_MscGate : public QObject
{
    Q_OBJECT
    static const QByteArray TestPropertyMethodNameMarker;
    static const QString TestGateName;

private Q_SLOTS:
    void testConstructor();
    void testPropertyDirection();
    void testPropertyInstance();
    void testPropertyInstanceName();
    void testPropertyParamName();
    void testPropertyParams();

    void testPropertiesCoverage();
};

const QByteArray tst_MscGate::TestPropertyMethodNameMarker = "testProperty";
const QString tst_MscGate::TestGateName = "TestGate";

void tst_MscGate::testConstructor()
{
    MscGate gateDefault;
    QCOMPARE(gateDefault.name(), MscEntity::DefaultName);

    MscGate gateNamed(TestGateName, this);
    QCOMPARE(gateNamed.name(), TestGateName);
}

void tst_MscGate::testPropertyDirection()
{
    MscGate gate(TestGateName, this);
    QCOMPARE(gate.direction(), MscGate::Direction::In); // the default one

    for (const MscGate::Direction dir : { MscGate::Direction::In, MscGate::Direction::Out }) {
        gate.setDirection(dir);
        QCOMPARE(gate.direction(), dir);
    }
}

void tst_MscGate::testPropertyInstance()
{
    MscGate gate(TestGateName, this);
    QCOMPARE(gate.instance(), static_cast<MscInstance *>(nullptr));

    static const QString testInstanceName("TestInstance");
    MscInstance mscInstance(testInstanceName);
    gate.setInstance(&mscInstance);
    QCOMPARE(gate.instance(), &mscInstance);
    QCOMPARE(gate.instance()->name(), testInstanceName);

    gate.setInstance(nullptr);
    QCOMPARE(gate.instance(), static_cast<MscInstance *>(nullptr));
}

void tst_MscGate::testPropertyInstanceName()
{
    MscGate gate(TestGateName, this);
    QCOMPARE(gate.instanceName(), QString());

    static const QString testInstanceName("TestInstance");
    gate.setInstanceName(testInstanceName);
    QCOMPARE(gate.instanceName(), testInstanceName);
    gate.setInstanceName(QString());
    QCOMPARE(gate.instanceName(), QString());
}

void tst_MscGate::testPropertyParamName()
{
    MscGate gate;
    QCOMPARE(gate.paramName(), QString());

    static const QString testParamName("testParamName");
    gate.setParamName(testParamName);
    QCOMPARE(gate.paramName(), testParamName);
}

void tst_MscGate::testPropertyParams()
{
    MscGate gate;
    QCOMPARE(gate.params(), QVariantList());

    static const QVariantList testParams = { "A", 1, true };
    gate.setParams(testParams);
    QCOMPARE(gate.params(), testParams);
}

void tst_MscGate::testPropertiesCoverage()
{
    MscGate gate;
    static const int gateCustomPropsCount(
            gate.metaObject()->propertyCount() - 6); // exclude objectName, MscEntity properties

    int testedPropsCount(0);
    const QMetaObject *myMeta(metaObject());
    for (int i = 0; i < myMeta->methodCount(); ++i) {
        if (myMeta->method(i).name().startsWith(TestPropertyMethodNameMarker))
            ++testedPropsCount;
    }

    QCOMPARE(testedPropsCount, gateCustomPropsCount);
}

QTEST_APPLESS_MAIN(tst_MscGate)

#include "tst_mscgate.moc"
