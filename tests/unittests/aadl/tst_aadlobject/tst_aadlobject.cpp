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

#include "aadlcommonprops.h"
#include "aadlobject.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "propertytemplateconfig.h"

#include <QSignalSpy>
#include <QTest>
#include <QVariant>

class AADLObjectImp : public aadl::AADLObject
{
    Q_OBJECT
public:
    explicit AADLObjectImp(const QString &title = QString(), QObject *parent = nullptr)
        : AADLObject(AADLObject::Type::Function, title, parent)
    {
    }
};

class tst_AADLObject : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_defaultConstructor();
    void test_paramConstructor();
    void test_setTitle();
    void test_coordinatesConverting();
    void test_coordinatesType();
    void test_hasAttributes();
};

void tst_AADLObject::test_defaultConstructor()
{
    AADLObjectImp obj;

    QCOMPARE(obj.parent(), nullptr);
    QCOMPARE(obj.title(), QString("Function_1"));
    QCOMPARE(obj.titleUI(), QString("Function 1"));
    QVERIFY(!obj.id().toString().isEmpty());
}

void tst_AADLObject::test_paramConstructor()
{
    auto nameToken = aadl::meta::Props::token(aadl::meta::Props::Token::name);

    auto obj = new AADLObjectImp("Some_name", this);
    QCOMPARE(obj->parent(), this);
    QCOMPARE(obj->attrs().value(nameToken).toString(), QString("Some_name")); // This is the stored value
    QCOMPARE(obj->title(), "Some_name"); // This is the stored value
    QCOMPARE(obj->titleUI(), "Some name"); // This is what the user sees
    QVERIFY(!obj->id().toString().isEmpty());

    obj = new AADLObjectImp("Another_name", this);
    QCOMPARE(obj->parent(), this);
    QCOMPARE(obj->attrs().value(nameToken).toString(), QString("Another_name")); // This is the stored value
    QCOMPARE(obj->title(), "Another_name"); // This is the stored value
    QCOMPARE(obj->titleUI(), "Another name"); // This is what the user sees
    QVERIFY(!obj->id().toString().isEmpty());
}

void tst_AADLObject::test_setTitle()
{
    auto nameToken = aadl::meta::Props::token(aadl::meta::Props::Token::name);

    AADLObjectImp obj1;
    QSignalSpy spy1(&obj1, &aadl::AADLObject::titleChanged);
    QVERIFY(obj1.title() != "Test_Object_Title");
    obj1.setTitle("Test_Object_Title");
    QCOMPARE(obj1.attrs().value(nameToken).toString(), QString("Test_Object_Title"));
    QCOMPARE(obj1.titleUI(), QString("Test Object Title"));
    QVERIFY(spy1.count() == 1);
    auto arguments = spy1.takeFirst();
    QVERIFY(arguments.size() == 1);
    QVERIFY(arguments.at(0).type() == QVariant::String);
    QCOMPARE(arguments.at(0).value<QString>(), QString("Test_Object_Title"));

    AADLObjectImp obj2;
    QSignalSpy spy2(&obj2, &aadl::AADLObject::titleChanged);
    QVERIFY(obj2.title() != "Test_Object_Title");
    obj2.setTitle("Test_Object_Title");
    QCOMPARE(obj2.attrs().value(nameToken).toString(), QString("Test_Object_Title"));
    QCOMPARE(obj2.titleUI(), QString("Test Object Title"));
    QVERIFY(spy2.count() == 1);
    arguments = spy2.takeFirst();
    QVERIFY(arguments.size() == 1);
    QVERIFY(arguments.at(0).type() == QVariant::String);
    QCOMPARE(arguments.at(0).value<QString>(), QString("Test_Object_Title"));
}

void tst_AADLObject::test_coordinatesConverting()
{
    QVector<qint32> coordinates = aadl::AADLObject::coordinatesFromString("");
    QVERIFY(coordinates.isEmpty());
    coordinates << 100 << 100 << 200 << 200;
    QString coordinatesStr = aadl::AADLObject::coordinatesToString(coordinates);
    QCOMPARE(coordinatesStr.compare("10000 10000 20000 20000"), 0);
    coordinatesStr.append(" 30000 30000 40000 40000");
    QVector<qint32> coordinatesEx = aadl::AADLObject::coordinatesFromString(coordinatesStr);
    coordinates << 300 << 300 << 400 << 400;
    QCOMPARE(coordinates, coordinatesEx);
}

void tst_AADLObject::test_coordinatesType()
{
    aadl::PropertyTemplateConfig dynPropConfig;
    dynPropConfig.init(QLatin1String("default_attributes.xml"));
    aadl::AADLObjectsModel model(&dynPropConfig);

    aadl::AADLObjectFunction fn1("Fn1");
    aadl::AADLObjectFunction fn2("Fn2", &fn1);
    aadl::AADLObjectFunction fn3("Fn3", &fn2);

    const QVector<aadl::AADLObject *> objects { &fn1, &fn2, &fn3 };

    for (auto object : objects)
        QVERIFY(model.addObject(object));

    QCOMPARE(fn1.coordinatesType(), aadl::meta::Props::Token::coordinates);
    QCOMPARE(fn2.coordinatesType(), aadl::meta::Props::Token::coordinates);
    QCOMPARE(fn3.coordinatesType(), aadl::meta::Props::Token::coordinates);

    model.setRootObject(fn1.id());
    QCOMPARE(fn1.coordinatesType(), aadl::meta::Props::Token::RootCoordinates);
    QCOMPARE(fn2.coordinatesType(), aadl::meta::Props::Token::InnerCoordinates);
    QCOMPARE(fn3.coordinatesType(), aadl::meta::Props::Token::coordinates);

    model.setRootObject(fn2.id());
    QCOMPARE(fn1.coordinatesType(), aadl::meta::Props::Token::coordinates);
    QCOMPARE(fn2.coordinatesType(), aadl::meta::Props::Token::RootCoordinates);
    QCOMPARE(fn3.coordinatesType(), aadl::meta::Props::Token::InnerCoordinates);
}

void tst_AADLObject::test_hasAttributes()
{
    AADLObjectImp obj;
    QHash<QString, QVariant> attributes = { { "foo", QVariant::fromValue(11) },
        { "bar", QVariant::fromValue(QString("dummy")) } };
    QCOMPARE(obj.hasAttributes(attributes), false);

    obj.setAttr("foo", QVariant::fromValue(999));
    QCOMPARE(obj.hasAttributes(attributes), false);

    obj.setAttr("bar", QVariant::fromValue(QString("dummy")));
    QCOMPARE(obj.hasAttributes(attributes), false);

    obj.setAttr("foo", QVariant::fromValue(11));
    QCOMPARE(obj.hasAttributes(attributes), true);
}

QTEST_APPLESS_MAIN(tst_AADLObject)

#include "tst_aadlobject.moc"
