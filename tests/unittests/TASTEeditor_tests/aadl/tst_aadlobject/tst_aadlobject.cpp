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

#include <QSignalSpy>
#include <QTest>
#include <QVariant>

class AADLObjectImp : public taste3::aadl::AADLObject
{
    Q_OBJECT
public:
    explicit AADLObjectImp(const QString &title = QString(), QObject *parent = nullptr)
        : AADLObject(title, parent)
    {
    }

    AADLObjectType aadlType() const override { return AADLObjectType::AADLUnknown; }
};

class tst_AADLObject : public QObject
{
    Q_OBJECT

private slots:
    void test_defaultConstructor();
    void test_paramConstructor();
    void test_setTitle();
    void test_setId();

private:
    static const QString TestObjectTitleValid, TestObjectTitleInvalid;
};

const QString tst_AADLObject::TestObjectTitleValid = { "Test_Object_Title" };
const QString tst_AADLObject::TestObjectTitleInvalid = { "Test Object Title" };

void tst_AADLObject::test_defaultConstructor()
{
    AADLObjectImp obj;

    QCOMPARE(obj.parent(), nullptr);
    QCOMPARE(obj.title(), QString());
    QVERIFY(!obj.id().toString().isEmpty());
}

void tst_AADLObject::test_paramConstructor()
{
    QVector<AADLObjectImp *> objects;
    for (const QString &name : { TestObjectTitleValid, TestObjectTitleInvalid })
        objects.append(new AADLObjectImp(name, this));

    for (int i = 0; i < objects.size(); ++i) {
        const AADLObjectImp *obj = objects.at(i);
        QCOMPARE(obj->parent(), this);
        QCOMPARE(obj->title(), TestObjectTitleValid);
        QVERIFY(obj->title() != TestObjectTitleInvalid);
        QVERIFY(!obj->id().toString().isEmpty());
    }
}

void tst_AADLObject::test_setTitle()
{
    using namespace taste3::aadl;

    auto checkNaming = [](const QString &name) {
        AADLObjectImp obj;
        QSignalSpy spy(&obj, &AADLObject::titleChanged);

        QVERIFY(obj.title() != name);
        obj.setTitle(name);
        QCOMPARE(obj.title(), TestObjectTitleValid);
        QVERIFY(obj.title() != TestObjectTitleInvalid);
        QVERIFY(spy.count() == 1);
        const QList<QVariant> &arguments = spy.takeFirst();
        QVERIFY(arguments.size() == 1);
        QVERIFY(arguments.at(0).type() == QVariant::String);
        QCOMPARE(arguments.at(0).value<QString>(), TestObjectTitleValid);
    };

    checkNaming(TestObjectTitleValid);
    checkNaming(TestObjectTitleInvalid);
}

void tst_AADLObject::test_setId()
{
    using namespace taste3::aadl;
    using namespace taste3::common;

    static const Id &testId = Id::createUuid();

    AADLObjectImp obj;
    qRegisterMetaType<Id>();
    QSignalSpy spy(&obj, &AADLObject::idChanged);

    QVERIFY(obj.id() != testId);
    obj.setId(testId);
    QCOMPARE(obj.id(), testId);

    QVERIFY(spy.count() == 1);
    const QList<QVariant> &arguments = spy.takeFirst();
    QVERIFY(arguments.size() == 1);
    QVERIFY(arguments.at(0).type() == QVariant::Uuid);
    QCOMPARE(arguments.at(0).value<QUuid>(), testId);
}

QTEST_APPLESS_MAIN(tst_AADLObject)

#include "tst_aadlobject.moc"
