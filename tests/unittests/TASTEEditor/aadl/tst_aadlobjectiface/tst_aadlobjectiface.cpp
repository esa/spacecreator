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

#include "tab_aadl/aadlobjectiface.h"

#include <QtTest>

class tst_AADLObjectIface : public QObject
{
    Q_OBJECT

private slots:
    void test_defaultCtor();
    void test_paramCtor();
    void test_holder();
    void test_provided();
    void test_required();

private:
    static const QString TestObjectName;
    void testDirectionImpl(taste3::aadl::AADLObjectIface::IfaceType dir);
};

const QString tst_AADLObjectIface::TestObjectName { "Test Object Name" };

void tst_AADLObjectIface::test_defaultCtor()
{
    taste3::aadl::AADLObjectIface obj;

    QCOMPARE(obj.parent(), nullptr);
    QCOMPARE(obj.title(), QString());
    QCOMPARE(obj.direction(), taste3::aadl::AADLObjectIface::DefaultDirection);
    QCOMPARE(obj.holder(), nullptr);

    QVERIFY(!obj.id().toString().isEmpty());
}

void tst_AADLObjectIface::test_paramCtor()
{
    using namespace taste3::aadl;

    static const AADLObjectIface::IfaceType customDirection { AADLObjectIface::IfaceType::Provided };
    AADLObjectIface obj(customDirection, TestObjectName);

    QCOMPARE(obj.parent(), nullptr);
    QCOMPARE(obj.holder(), nullptr);
    QCOMPARE(obj.title(), TestObjectName);
    QCOMPARE(obj.direction(), customDirection);

    QVERIFY(!obj.id().toString().isEmpty());
}

void tst_AADLObjectIface::test_holder()
{
    using namespace taste3::aadl;

    static const AADLObjectIface::IfaceType customDirection { AADLObjectIface::IfaceType::Provided };
    AADLObjectIface holderObj;
    AADLObjectIface obj(customDirection, TestObjectName, &holderObj);

    QCOMPARE(obj.parent(), &holderObj);
    QCOMPARE(obj.holder(), &holderObj);
    QCOMPARE(obj.title(), TestObjectName);
    QCOMPARE(obj.direction(), customDirection);

    QVERIFY(!obj.id().toString().isEmpty());
}

void tst_AADLObjectIface::test_provided()
{
    testDirectionImpl(taste3::aadl::AADLObjectIface::IfaceType::Provided);
}

void tst_AADLObjectIface::test_required()
{
    testDirectionImpl(taste3::aadl::AADLObjectIface::IfaceType::Required);
}

void tst_AADLObjectIface::testDirectionImpl(taste3::aadl::AADLObjectIface::IfaceType dir)
{
    using namespace taste3::aadl;
    AADLObjectIface obj(dir);

    QCOMPARE(obj.parent(), nullptr);
    QCOMPARE(obj.holder(), nullptr);
    QCOMPARE(obj.title(), QString());
    QCOMPARE(obj.direction(), dir);

    QVERIFY(!obj.id().toString().isEmpty());
}

QTEST_APPLESS_MAIN(tst_AADLObjectIface)

#include "tst_aadlobjectiface.moc"
