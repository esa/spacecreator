/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <QDir>
#include <QObject>
#include <QtTest>
#include <seds/SedsModel/sedsmodel.h>
#include <seds/SedsXmlParser/exceptions.h>
#include <seds/SedsXmlParser/parser.h>

using seds::parser::Parser;
using seds::parser::ParserException;

namespace seds::test {

class tst_SedsXmlParser : public QObject
{
    Q_OBJECT

public:
    virtual ~tst_SedsXmlParser() = default;

public Q_SLOTS:
    void testComponentsParsing();
    void testContainerDataTypesParsing();
    void testDeviceParsing();
    void testInterfacesParsing();
    void testSimpleDataTypesParsing();
};

void tst_SedsXmlParser::testComponentsParsing()
{
    try {
        const auto model = Parser::parse("Components.xml");
        const auto sedsModel = dynamic_cast<model::SedsModel *>(model.get());

        const auto &components = std::get<model::PackageFile>(sedsModel->data()).package().components();
        QCOMPARE(components.size(), 1);

        const auto &component = components.at(0);
        QCOMPARE(component.name().value(), "SampleComponent");
        QCOMPARE(*component.longDescription(), "Sample component");

        const auto &providedInterfaces = component.providedInterfaces();
        QCOMPARE(providedInterfaces.size(), 1);

        const auto &requiredInterfaces = component.requiredInterfaces();
        QCOMPARE(requiredInterfaces.size(), 1);

        const auto &dataTypes = component.dataTypes();
        QCOMPARE(dataTypes.size(), 3);

        const auto &declaredInterfaces = component.declaredInterfaces();
        QCOMPARE(declaredInterfaces.size(), 1);

        const auto &implementation = component.implementation();

        const auto &variables = implementation.variables();
        QCOMPARE(variables.size(), 7);

        const auto &parameterMaps = implementation.parameterMaps();
        QCOMPARE(parameterMaps.size(), 2);

        const auto &parameterActivityMaps = implementation.parameterActivityMaps();
        QCOMPARE(parameterActivityMaps.size(), 1);

        const auto &activities = implementation.activities();
        QCOMPARE(activities.size(), 6);

        const auto &stateMachines = implementation.stateMachines();
        QCOMPARE(stateMachines.size(), 1);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SedsXmlParser::testContainerDataTypesParsing()
{
    try {
        const auto model = Parser::parse("Containers.xml");
        const auto sedsModel = dynamic_cast<model::SedsModel *>(model.get());

        const auto &dataTypeSet = std::get<model::PackageFile>(sedsModel->data()).package().dataTypes();
        QCOMPARE(dataTypeSet.size(), 15);

        const auto &arrayType = std::get<model::ArrayDataType>(dataTypeSet.at(3));
        QCOMPARE(arrayType.name().value(), "ArrayType");
        QCOMPARE(arrayType.type().value().name().value(), "DataItemA");
        QCOMPARE(arrayType.dimensions().size(), 1);

        const auto &dimension = arrayType.dimensions().at(0);
        QCOMPARE(dimension.indexTypeRef()->value().name().value(), "LengthType");

        const auto &containerType = std::get<model::ContainerDataType>(dataTypeSet.at(4));
        QCOMPARE(containerType.name().value(), "BasicContainer");
        QCOMPARE(containerType.entries().size(), 2);

        const auto &entry = std::get<model::Entry>(containerType.entries().at(0));
        QCOMPARE(entry.name().value(), "fieldA");
        QCOMPARE(entry.type().value().name().value(), "DataItemA");
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SedsXmlParser::testDeviceParsing()
{
    try {
        const auto model = Parser::parse("Device.xml");
        const auto sedsModel = dynamic_cast<model::SedsModel *>(model.get());

        const auto &device = std::get<model::DataSheet>(sedsModel->data()).device();
        QCOMPARE(device.name().value(), "ExampleDevice");

        const auto &metadata = *device.metadata();

        const auto &values = metadata.values();
        QCOMPARE(values.size(), 4);

        const auto &categories = metadata.categories();
        QCOMPARE(categories.size(), 2);

        const auto &rootCategory = categories.at(0);
        QCOMPARE(rootCategory.name().value(), "RootCategory");
        QCOMPARE(rootCategory.values().size(), 2);
        QCOMPARE(rootCategory.subcategories().size(), 1);

        const auto &subCategory = rootCategory.subcategories().at(0);
        QCOMPARE(subCategory.name().value(), "SubCategory");
        QCOMPARE(subCategory.values().size(), 2);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SedsXmlParser::testInterfacesParsing()
{
    try {
        const auto model = Parser::parse("Interfaces.xml");
        const auto sedsModel = dynamic_cast<model::SedsModel *>(model.get());

        const auto &declaredInterfaces = std::get<model::PackageFile>(sedsModel->data()).package().declaredInterfaces();
        QCOMPARE(declaredInterfaces.size(), 5);

        const auto &interfaceWithCommands = declaredInterfaces.at(0);
        QCOMPARE(interfaceWithCommands.name().value(), "InterfaceWithSynchronousCommands");

        const auto &commands = interfaceWithCommands.commands();
        QCOMPARE(commands.size(), 3);

        const auto &command = commands.at(0);
        QCOMPARE(command.name().value(), "ExecuteSynchronousAction");
        QCOMPARE(command.mode(), model::InterfaceCommandMode::Sync);
        QCOMPARE(command.arguments().size(), 2);

        const auto &interfaceWithParameters = declaredInterfaces.at(2);
        QCOMPARE(interfaceWithParameters.name().value(), "InterfaceWithParameters");

        const auto &parameters = interfaceWithParameters.parameters();
        QCOMPARE(parameters.size(), 4);

        const auto &parameter = parameters.at(0);
        QCOMPARE(parameter.name().value(), "SynchronousParameter");
        QCOMPARE(parameter.type().value().name().value(), "DataItem");
        QCOMPARE(parameter.mode(), model::InterfaceParameterMode::Sync);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

void tst_SedsXmlParser::testSimpleDataTypesParsing()
{
    try {
        const auto model = Parser::parse("SimpleDataTypes.xml");
        const auto sedsModel = dynamic_cast<model::SedsModel *>(model.get());

        const auto &dataTypesSet = std::get<model::PackageFile>(sedsModel->data()).package().dataTypes();
        QCOMPARE(dataTypesSet.size(), 14);

        const auto &uint8Type = std::get<model::IntegerDataType>(dataTypesSet.at(0));
        QCOMPARE(uint8Type.name().value(), "UnsignedInteger8");
        QVERIFY(uint8Type.encoding().has_value());

        const auto &enumeratedType = std::get<model::EnumeratedDataType>(dataTypesSet.at(13));
        QCOMPARE(enumeratedType.name().value(), "SimpleEnumeration");
        QCOMPARE(enumeratedType.enumerationList().size(), 3);

        const auto &enumeration = enumeratedType.enumerationList().at(2);
        QCOMPARE(enumeration.value(), -12);
        QCOMPARE(enumeration.label().value(), "NegativeValue");
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

} // namespace seds::test

QTEST_MAIN(seds::test::tst_SedsXmlParser)

#include "tst_sedsxmlparser.moc"
