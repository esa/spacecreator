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

#include "ivcommonprops.h"
#include "ivfunction.h"
#include "ivlibrary.h"
#include "ivmodel.h"
#include "ivobject.h"
#include "propertytemplateconfig.h"

#include <QSignalSpy>
#include <QTest>
#include <QVariant>

class AADLObjectImp : public ivm::IVObject
{
    Q_OBJECT
public:
    explicit AADLObjectImp(const QString &title = QString(), QObject *parent = nullptr)
        : IVObject(IVObject::Type::Function, title, parent)
    {
    }
};

class tst_AADLObject : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test_defaultConstructor();
    void test_paramConstructor();
    void test_setTitle();
    void test_coordinatesConverting();
    void test_coordinatesType();
    void test_hasAttributes();
    void test_hasProperties();
};

void tst_AADLObject::initTestCase()
{
    ivm::initIVLibrary();
}

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
    auto nameToken = ivm::meta::Props::token(ivm::meta::Props::Token::name);

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
    auto nameToken = ivm::meta::Props::token(ivm::meta::Props::Token::name);

    AADLObjectImp obj1;
    QSignalSpy spy1(&obj1, &ivm::IVObject::titleChanged);
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
    QSignalSpy spy2(&obj2, &ivm::IVObject::titleChanged);
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
    QVector<qint32> coordinates = ivm::IVObject::coordinatesFromString("");
    QVERIFY(coordinates.isEmpty());
    coordinates << 100 << 100 << 200 << 200;
    QString coordinatesStr = ivm::IVObject::coordinatesToString(coordinates);
    QCOMPARE(coordinatesStr.compare("10000 10000 20000 20000"), 0);
    coordinatesStr.append(" 30000 30000 40000 40000");
    QVector<qint32> coordinatesEx = ivm::IVObject::coordinatesFromString(coordinatesStr);
    coordinates << 300 << 300 << 400 << 400;
    QCOMPARE(coordinates, coordinatesEx);
}

void tst_AADLObject::test_coordinatesType()
{
    ivm::PropertyTemplateConfig *dynPropConfig = ivm::PropertyTemplateConfig::instance();
    dynPropConfig->init(QLatin1String("default_attributes.xml"));
    ivm::IVModel model(dynPropConfig);

    ivm::IVFunction fn1("Fn1");
    ivm::IVFunction fn2("Fn2", &fn1);
    ivm::IVFunction fn3("Fn3", &fn2);

    QSignalSpy spy(&model, &ivm::IVModel::objectsAdded);
    model.addObjects({ &fn1, &fn2, &fn3 });
    QVERIFY(spy.count() == 1);
    QVERIFY(model.objects().size() == 3);

    QCOMPARE(fn1.coordinatesType(), ivm::meta::Props::Token::coordinates);
    QCOMPARE(fn2.coordinatesType(), ivm::meta::Props::Token::coordinates);
    QCOMPARE(fn3.coordinatesType(), ivm::meta::Props::Token::coordinates);

    model.setRootObject(fn1.id());
    QCOMPARE(fn1.coordinatesType(), ivm::meta::Props::Token::RootCoordinates);
    QCOMPARE(fn2.coordinatesType(), ivm::meta::Props::Token::InnerCoordinates);
    QCOMPARE(fn3.coordinatesType(), ivm::meta::Props::Token::coordinates);

    model.setRootObject(fn2.id());
    QCOMPARE(fn1.coordinatesType(), ivm::meta::Props::Token::coordinates);
    QCOMPARE(fn2.coordinatesType(), ivm::meta::Props::Token::RootCoordinates);
    QCOMPARE(fn3.coordinatesType(), ivm::meta::Props::Token::InnerCoordinates);
}

void tst_AADLObject::test_hasAttributes()
{
    AADLObjectImp obj;
    QHash<QString, QVariant> attributes = { { "foo", QVariant::fromValue(11) },
        { "bar", QVariant::fromValue(QString("dummy")) } };
    QCOMPARE(obj.hasAttributes(attributes), false);
    const int attrsCount = obj.attrs().size();

    obj.setAttr("foo", QVariant::fromValue(999));
    QCOMPARE(obj.hasAttributes(attributes), false);
    QCOMPARE(obj.hasAttribute("foo", 11), false);
    QCOMPARE(obj.hasAttribute("foo", 999), true);
    QCOMPARE(obj.hasAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasAttribute("bar", "dummy"), false);

    obj.setAttr("bar", QVariant::fromValue(QString("dummy")));
    QCOMPARE(obj.hasAttributes(attributes), false);
    QCOMPARE(obj.hasAttribute("foo", 11), false);
    QCOMPARE(obj.hasAttribute("foo", 999), true);
    QCOMPARE(obj.hasAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasAttribute("bar", "dummy"), true);

    obj.setAttr("foo", QVariant::fromValue(11));
    QCOMPARE(obj.hasAttributes(attributes), true);
    QCOMPARE(obj.hasAttribute("foo", 11), true);
    QCOMPARE(obj.hasAttribute("foo", 999), false);
    QCOMPARE(obj.hasAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasAttribute("bar", "dummy"), true);

    obj.removeAttr("foo");
    QCOMPARE(obj.hasAttributes(attributes), false);
    QCOMPARE(obj.hasAttribute("foo", 11), false);
    QCOMPARE(obj.hasAttribute("foo", 999), false);
    QCOMPARE(obj.hasAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasAttribute("bar", "dummy"), true);

    obj.removeAttr("bar");
    QCOMPARE(obj.hasAttributes(attributes), false);
    QCOMPARE(obj.hasAttribute("foo", 11), false);
    QCOMPARE(obj.hasAttribute("foo", 999), false);
    QCOMPARE(obj.hasAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasAttribute("bar", "dummy"), false);
    QCOMPARE(obj.attrs().size(), attrsCount);
}

void tst_AADLObject::test_hasProperties()
{
    AADLObjectImp obj;
    QHash<QString, QVariant> properties = { { "foo", QVariant::fromValue(11) },
        { "bar", QVariant::fromValue(QString("dummy")) } };
    QCOMPARE(obj.hasProperties(properties), false);
    const int propsCount = obj.props().size();

    obj.setProp("foo", QVariant::fromValue(999));
    QCOMPARE(obj.hasProperties(properties), false);
    QCOMPARE(obj.hasProperty("foo", 11), false);
    QCOMPARE(obj.hasProperty("foo", 999), true);
    QCOMPARE(obj.hasProperty("bar", "empty"), false);
    QCOMPARE(obj.hasProperty("bar", "dummy"), false);

    obj.setProp("bar", QVariant::fromValue(QString("dummy")));
    QCOMPARE(obj.hasProperties(properties), false);
    QCOMPARE(obj.hasProperty("foo", 11), false);
    QCOMPARE(obj.hasProperty("foo", 999), true);
    QCOMPARE(obj.hasProperty("bar", "empty"), false);
    QCOMPARE(obj.hasProperty("bar", "dummy"), true);

    obj.setProp("foo", QVariant::fromValue(11));
    QCOMPARE(obj.hasProperties(properties), true);
    QCOMPARE(obj.hasProperty("foo", 11), true);
    QCOMPARE(obj.hasProperty("foo", 999), false);
    QCOMPARE(obj.hasProperty("bar", "empty"), false);
    QCOMPARE(obj.hasProperty("bar", "dummy"), true);

    obj.removeProp("foo");
    QCOMPARE(obj.hasProperties(properties), false);
    QCOMPARE(obj.hasProperty("foo", 11), false);
    QCOMPARE(obj.hasProperty("foo", 999), false);
    QCOMPARE(obj.hasProperty("bar", "empty"), false);
    QCOMPARE(obj.hasProperty("bar", "dummy"), true);

    obj.removeProp("bar");
    QCOMPARE(obj.hasProperties(properties), false);
    QCOMPARE(obj.hasProperty("foo", 11), false);
    QCOMPARE(obj.hasProperty("foo", 999), false);
    QCOMPARE(obj.hasProperty("bar", "empty"), false);
    QCOMPARE(obj.hasProperty("bar", "dummy"), false);
    QCOMPARE(obj.props().size(), propsCount);
}

QTEST_APPLESS_MAIN(tst_AADLObject)

#include "tst_aadlobject.moc"
