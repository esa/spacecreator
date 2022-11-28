/*
   Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "propertytemplate.h"

#include <QtTest>

class TestPropertyTemplate : public shared::PropertyTemplate {
public:

    // PropertyTemplate interface
public:
    QMetaEnum scopeMetaEnum() const override
    {
        return QMetaEnum();
    }
    int objectScope(const shared::VEObject *obj) const override
    {
        Q_UNUSED(obj);
        return 0;
    }
};

class tst_PropertyTemplate : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDataConversion_data();
    void testDataConversion();
    void testEnumDataFromXML();
};

void tst_PropertyTemplate::testDataConversion_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<shared::PropertyTemplate::Type>("type");
    QTest::addColumn<QVariant>("result");

    QTest::newRow("Bool lowercase") << QVariant::fromValue(QString("true")) << shared::PropertyTemplate::Type::Boolean
                                    << QVariant::fromValue(true);
    QTest::newRow("Bool uppercase") << QVariant::fromValue(QString("FALSE")) << shared::PropertyTemplate::Type::Boolean
                                    << QVariant::fromValue(false);
    QTest::newRow("Bool random string") << QVariant::fromValue(QString("Maybe"))
                                        << shared::PropertyTemplate::Type::Boolean << QVariant();

    QTest::newRow("Integer") << QVariant::fromValue(99) << shared::PropertyTemplate::Type::Integer
                             << QVariant::fromValue(99);
    QTest::newRow("Integer random string")
            << QVariant::fromValue(QString("Null")) << shared::PropertyTemplate::Type::Integer << QVariant();

    QTest::newRow("Double") << QVariant::fromValue(12.34) << shared::PropertyTemplate::Type::Real
                            << QVariant::fromValue(12.34);
    QTest::newRow("Double no decimal") << QVariant::fromValue(11) << shared::PropertyTemplate::Type::Real
                                       << QVariant::fromValue(11);
    QTest::newRow("Double random string")
            << QVariant::fromValue(QString("Zero")) << shared::PropertyTemplate::Type::Real << QVariant();

    QTest::newRow("String") << QVariant::fromValue(QString("Top")) << shared::PropertyTemplate::Type::String
                            << QVariant::fromValue(QString("Top"));
    QTest::newRow("Empty string") << QVariant::fromValue(QString("")) << shared::PropertyTemplate::Type::String
                                  << QVariant();

    QVariantList list;
    list << QVariant::fromValue(QString("one")) << QVariant::fromValue(QString("two"));
    QTest::newRow("Enumeration") << QVariant::fromValue(list) << shared::PropertyTemplate::Type::Enumeration
                                 << QVariant::fromValue(QStringList { "one", "two" });
    QTest::newRow("Enumeration no list") << QVariant::fromValue(1) << shared::PropertyTemplate::Type::Enumeration
                                         << QVariant();
}

void tst_PropertyTemplate::testDataConversion()
{
    QFETCH(QVariant, value);
    QFETCH(shared::PropertyTemplate::Type, type);
    QFETCH(QVariant, result);

    QVariant data = shared::PropertyTemplate::convertData(value, type);
    QCOMPARE(data, result);
}

void tst_PropertyTemplate::testEnumDataFromXML()
{
    QDomDocument xmlDoc;
    QByteArray xmlContent = R"(<Attrs>
    <Attr label="Language" name="language" visible="false">
        <Type>
            <Enumeration defaultValue="SDL">
                <Entry value="Ada" folder_name="Ada_impl"/>
                <Entry value="CPP" folder_name="CPP"/>
                <Entry value="Simulink" folder_name="SIMULINK" dummy="foo"/>
            </Enumeration>
        </Type>
    </Attr></Attrs>)";
    xmlDoc.setContent(xmlContent);
    QDomElement element = xmlDoc.firstChildElement().firstChildElement();
    auto property = shared::PropertyTemplate::createFromXml<TestPropertyTemplate>(element);
    QVERIFY(property !=nullptr);

    QCOMPARE(property->name(), "language");
    QCOMPARE(property->type(), shared::PropertyTemplate::Type::Enumeration);
    QVariantList value {"Ada", "CPP", "Simulink"};
    QCOMPARE(property->value(), QVariant::fromValue(value));

    QMap<QString, QString> enumData = property->enumData("Ada");
    QCOMPARE(enumData.size(), 1); // containing the folder info
    QCOMPARE(enumData.value("folder_name"), QVariant::fromValue(QString("Ada_impl")));

    enumData = property->enumData("Simulink");
    QCOMPARE(enumData.size(), 2);
    QCOMPARE(enumData.value("folder_name"), QVariant::fromValue(QString("SIMULINK")));
}

QTEST_MAIN(tst_PropertyTemplate)

#include "tst_propertytemplate.moc"
