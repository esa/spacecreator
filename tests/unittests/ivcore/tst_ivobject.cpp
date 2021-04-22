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

class IVObjectImp : public ivm::IVObject
{
    Q_OBJECT
public:
    explicit IVObjectImp(const QString &title = QString(), QObject *parent = nullptr)
        : IVObject(IVObject::Type::Function, title, parent)
    {
    }
};

class tst_IVObject : public QObject
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

void tst_IVObject::initTestCase()
{
    ivm::initIVLibrary();
}

void tst_IVObject::test_defaultConstructor()
{
    IVObjectImp obj;

    QCOMPARE(obj.parent(), nullptr);
    QCOMPARE(obj.title(), QString("Function_1"));
    QCOMPARE(obj.titleUI(), QString("Function 1"));
    QVERIFY(!obj.id().toString().isEmpty());
}

void tst_IVObject::test_paramConstructor()
{
    auto nameToken = ivm::meta::Props::token(ivm::meta::Props::Token::name);

    auto obj = new IVObjectImp("Some_name", this);
    QCOMPARE(obj->parent(), this);
    QCOMPARE(obj->entityAttributes().value(nameToken).value<QString>(),
            QString("Some_name")); // This is the stored value
    QCOMPARE(obj->title(), "Some_name"); // This is the stored value
    QCOMPARE(obj->titleUI(), "Some name"); // This is what the user sees
    QVERIFY(!obj->id().toString().isEmpty());

    obj = new IVObjectImp("Another_name", this);
    QCOMPARE(obj->parent(), this);
    QCOMPARE(obj->entityAttributes().value(nameToken).value<QString>(),
            QString("Another_name")); // This is the stored value
    QCOMPARE(obj->title(), "Another_name"); // This is the stored value
    QCOMPARE(obj->titleUI(), "Another name"); // This is what the user sees
    QVERIFY(!obj->id().toString().isEmpty());
}

void tst_IVObject::test_setTitle()
{
    auto nameToken = ivm::meta::Props::token(ivm::meta::Props::Token::name);

    IVObjectImp obj1;
    QSignalSpy spy1(&obj1, &ivm::IVObject::titleChanged);
    QVERIFY(obj1.title() != "Test_Object_Title");
    obj1.setTitle("Test_Object_Title");
    QCOMPARE(obj1.entityAttributes().value(nameToken).value<QString>(), QString("Test_Object_Title"));
    QCOMPARE(obj1.titleUI(), QString("Test Object Title"));
    QVERIFY(spy1.count() == 1);
    auto arguments = spy1.takeFirst();
    QVERIFY(arguments.size() == 1);
    QVERIFY(arguments.at(0).type() == QVariant::String);
    QCOMPARE(arguments.at(0).value<QString>(), QString("Test_Object_Title"));

    IVObjectImp obj2;
    QSignalSpy spy2(&obj2, &ivm::IVObject::titleChanged);
    QVERIFY(obj2.title() != "Test_Object_Title");
    obj2.setTitle("Test_Object_Title");
    QCOMPARE(obj2.entityAttributes().value(nameToken).value<QString>(), QString("Test_Object_Title"));
    QCOMPARE(obj2.titleUI(), QString("Test Object Title"));
    QVERIFY(spy2.count() == 1);
    arguments = spy2.takeFirst();
    QVERIFY(arguments.size() == 1);
    QVERIFY(arguments.at(0).type() == QVariant::String);
    QCOMPARE(arguments.at(0).value<QString>(), QString("Test_Object_Title"));
}

void tst_IVObject::test_coordinatesConverting()
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

void tst_IVObject::test_coordinatesType()
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

void tst_IVObject::test_hasAttributes()
{
    IVObjectImp obj;
    const QHash<QString, EntityAttribute> attributes = {
        { "foo", EntityAttribute("foo", QVariant::fromValue(11), EntityAttribute::Type::Attribute) },
        { "bar", EntityAttribute("bar", QVariant::fromValue(QString("dummy")), EntityAttribute::Type::Attribute) }
    };
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    const int attrsCount = obj.entityAttributes().size();

    obj.setEntityAttribute(QLatin1String("foo"), QVariant::fromValue(999));
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), true);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), false);

    obj.setEntityAttribute(QLatin1String("bar"), QVariant::fromValue(QString("dummy")));
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), true);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), true);

    obj.setEntityAttribute(QLatin1String("foo"), QVariant::fromValue(11));
    QCOMPARE(obj.hasEntityAttributes(attributes), true);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), true);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), true);

    obj.removeEntityAttribute("foo");
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), true);

    obj.removeEntityAttribute("bar");
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), false);
    QCOMPARE(obj.entityAttributes().size(), attrsCount);
}

void tst_IVObject::test_hasProperties()
{
    IVObjectImp obj;
    const QHash<QString, EntityAttribute> attributes = {
        { "foo", EntityAttribute("foo", QVariant::fromValue(11), EntityAttribute::Type::Property) },
        { "bar", EntityAttribute("bar", QVariant::fromValue(QString("dummy")), EntityAttribute::Type::Property) }
    };
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    const int propsCount = obj.entityAttributes().size();

    obj.setEntityProperty(QLatin1String("foo"), QVariant::fromValue(999));
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), true);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), false);

    obj.setEntityProperty(QLatin1String("bar"), QVariant::fromValue(QString("dummy")));
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), true);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), true);

    obj.setEntityProperty(QLatin1String("foo"), QVariant::fromValue(11));
    QCOMPARE(obj.hasEntityAttributes(attributes), true);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), true);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), true);

    obj.removeEntityAttribute("foo");
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), true);

    obj.removeEntityAttribute("bar");
    QCOMPARE(obj.hasEntityAttributes(attributes), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 11), false);
    QCOMPARE(obj.hasEntityAttribute("foo", 999), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "empty"), false);
    QCOMPARE(obj.hasEntityAttribute("bar", "dummy"), false);
    QCOMPARE(obj.entityAttributes().size(), propsCount);
}

QTEST_APPLESS_MAIN(tst_IVObject)

#include "tst_ivobject.moc"
