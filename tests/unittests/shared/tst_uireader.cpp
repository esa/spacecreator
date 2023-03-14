/*
  Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "sharedlibrary.h"
#include "uireader.h"
#include "veobject.h"

#include <QBuffer>
#include <QObject>
#include <QtTest>

class UIReader : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void test_EmptyDoc();
    void test_ReadEntries();
};

void UIReader::initTestCase()
{
    shared::initSharedLibrary();
}

void UIReader::test_EmptyDoc()
{
    QByteArray xml = R"(<UI/>)";
    QBuffer buffer(&xml);
    QVERIFY(buffer.open(QIODevice::ReadOnly | QIODevice::Text));
    shared::UIReader reader;
    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QHash<shared::Id, EntityAttributes> objectsData = reader.data();
    QVERIFY(objectsData.isEmpty());
}

void UIReader::test_ReadEntries()
{
    QHash<shared::Id, EntityAttributes> templateData;
    templateData[QUuid::fromString("{8abfdf98-b52c-490e-a77f-63de5a75fb06}")] = {
        { "Taste::coordinates",
                EntityAttribute { "Taste::coordinates", "72100 69400", EntityAttribute::Type::Property } },
        { "Taste::RootCoordinates",
                EntityAttribute { "Taste::RootCoordinates", "28700 42500", EntityAttribute::Type::Property } },
    };
    templateData[QUuid::fromString("{48a4b0d8-30a8-41be-9f0e-023c07c451d1}")] = {
        { "Taste::coordinates",
                EntityAttribute { "Taste::coordinates", "-32300 -33900 -32300 -38000 -33200 -40600 -33200 -42200",
                        EntityAttribute::Type::Property } },
    };
    templateData[QUuid::fromString("{3d106a0b-0039-4a04-887c-c73463c4b7b2}")] = {
        { "Namespace::propValue",
                EntityAttribute { "Namespace::propValue", "PropertyValue", EntityAttribute::Type::Property } },
        { "Namespace::PropertyName::value",
                EntityAttribute { "Namespace::PropertyName::value", "PropertyData", EntityAttribute::Type::Property } },
        { "attr", EntityAttribute { "attr", "AttrData", EntityAttribute::Type::Attribute } },
    };

    QByteArray xml = R"(
        <UI>
            <Entity id="{8abfdf98-b52c-490e-a77f-63de5a75fb06}">
                <Taste coordinates="72100 69400" RootCoordinates="28700 42500"></Taste>
            </Entity>
            <Entity origin="{48a4b0d8-30a8-41be-9f0e-023c07c451d1}">
                <Taste coordinates="-32300 -33900 -32300 -38000 -33200 -40600 -33200 -42200"></Taste>
            </Entity>
            <Entity id="{3d106a0b-0039-4a04-887c-c73463c4b7b2}" attr="AttrData">
                <Namespace propValue="PropertyValue">
                    <PropertyName value="PropertyData"/>
                </Namespace>
            </Entity>
        </UI>
    )";
    QBuffer buffer(&xml);
    QVERIFY(buffer.open(QIODevice::ReadOnly | QIODevice::Text));
    shared::UIReader reader;
    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QHash<shared::Id, EntityAttributes> objectsData = reader.data();
    QVERIFY(objectsData.size() == templateData.size());
    QCOMPARE(templateData, objectsData);
}

QTEST_APPLESS_MAIN(UIReader)

#include "tst_uireader.moc"
