/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "archetypes/archetypecommonprops.h"
#include "archetypes/archetypeobject.h"
#include "archetypes/archetypexmlreader.h"
#include "archetypes/functionarchetype.h"
#include "archetypes/interfacearchetype.h"
#include "archetypes/parameterarchetype.h"
#include "archetypes/layerarchetype.h"

#include <QBuffer>
#include <QSignalSpy>
#include <QtTest>

class tst_ArchetypeXMLReader : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_readFunction();
    void test_readInterface();
    void test_readParameter();
    void test_readLayer();
};

void tst_ArchetypeXMLReader::test_readFunction()
{
    QByteArray xml(R"(<ArchetypeLibrary>
	                    <FunctionArchetype name="TestArchFunc">
	                    </FunctionArchetype>
                      </ArchetypeLibrary>)");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::ArchetypeXMLReader reader;

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QVector<ivm::ArchetypeObject *> objectsList = reader.parsedObjects();
    QCOMPARE(objectsList.size(), 1);
    ivm::FunctionArchetype *function = qobject_cast<ivm::FunctionArchetype *>(objectsList[0]);

    QVERIFY(function != nullptr);
    QCOMPARE(function->title(), "TestArchFunc");
}

void tst_ArchetypeXMLReader::test_readInterface()
{
    QByteArray xml(R"(<ArchetypeLibrary>
	                    <FunctionArchetype name="TestArchFunc">
                            <InterfaceArchetype name="SomeInterface" interfaceType="REQUIRED" kind="SPORADIC" layer="testLayer">
		                    </InterfaceArchetype>
	                    </FunctionArchetype>
                      </ArchetypeLibrary>)");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::ArchetypeXMLReader reader;

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QVector<ivm::ArchetypeObject *> objectsList = reader.parsedObjects();
    QCOMPARE(objectsList.size(), 2);
    ivm::InterfaceArchetype *interface { nullptr };

    for (auto archetypeObject : objectsList) {
        if (archetypeObject->type() == ivm::ArchetypeObject::Type::InterfaceArchetype) {
            interface = archetypeObject->as<ivm::InterfaceArchetype *>();
        }
    }

    QVERIFY(interface != nullptr);
    QCOMPARE(interface->title(), "SomeInterface");
    QCOMPARE(interface->getInterfaceType(), ivm::InterfaceArchetype::InterfaceType::REQUIRED);
    QCOMPARE(interface->getOperationKind(), ivm::InterfaceArchetype::OperationKind::SPORADIC);
    QCOMPARE(interface->getLayer(), "testLayer");
}

void tst_ArchetypeXMLReader::test_readParameter()
{
    QByteArray xml(R"(<ArchetypeLibrary>
	                    <FunctionArchetype name="TestArchFunc">
                            <InterfaceArchetype name="SomeInterface" interfaceType="REQUIRED" kind="SPORADIC" layer="testLayer">
                                <ParameterArchetype name="ParamaeterTest" type="S32" direction="OUT"/>
		                    </InterfaceArchetype>
	                    </FunctionArchetype>
                      </ArchetypeLibrary>)");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::ArchetypeXMLReader reader;

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QVector<ivm::ArchetypeObject *> objectsList = reader.parsedObjects();
    QCOMPARE(objectsList.size(), 3);
    ivm::ParameterArchetype *parameter { nullptr };

    for (auto archetypeObject : objectsList) {
        if (archetypeObject->type() == ivm::ArchetypeObject::Type::ParameterArchetype) {
            parameter = archetypeObject->as<ivm::ParameterArchetype *>();
        }
    }

    QVERIFY(parameter != nullptr);
    QCOMPARE(parameter->title(), "ParamaeterTest");
    QCOMPARE(parameter->getType(), "S32");
    QCOMPARE(parameter->getDirection(), ivm::ParameterArchetype::ParameterDirection::OUT);
}

void tst_ArchetypeXMLReader::test_readLayer()
{
    QByteArray xml(R"(<ArchetypeLibrary>
	                    <CommunicationLayerTypes>
		                    <CommunicationLayerType name="TestLib1"/>
	                    </CommunicationLayerTypes>
                      </ArchetypeLibrary>)");

    QBuffer buffer(&xml);
    buffer.open(QIODevice::ReadOnly | QIODevice::Text);

    ivm::ArchetypeXMLReader reader;

    const bool ok = reader.read(&buffer);
    QVERIFY(ok);
    const QVector<ivm::ArchetypeObject *> objectsList = reader.parsedObjects();
    QCOMPARE(objectsList.size(), 1);
    ivm::LayerArchetype *layer { nullptr };

    for (auto archetypeObject : objectsList) {
        if (archetypeObject->type() == ivm::ArchetypeObject::Type::LayerArchetype) {
            layer = archetypeObject->as<ivm::LayerArchetype *>();
        }
    }

    QVERIFY(layer != nullptr);
    QCOMPARE(layer->title(), "TestLib1");
}

QTEST_APPLESS_MAIN(tst_ArchetypeXMLReader)

#include "tst_archetypexmlreader.moc"
