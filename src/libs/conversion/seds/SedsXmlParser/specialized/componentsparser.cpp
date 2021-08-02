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

#include "specialized/componentsparser.h"

#include <QXmlStreamReader>

#include <seds/ThirdParty/magicenum.h>
#include <seds/SedsModel/components/component.h>
#include <seds/SedsModel/components/componentimplementation.h>
#include <seds/SedsModel/components/interface.h>
#include <seds/SedsModel/components/parameteractivitymap.h>
#include <seds/SedsModel/components/parametermap.h>
#include <seds/SedsModel/components/parametermapdata.h>
#include <seds/SedsModel/components/variable.h>
#include <seds/SedsModel/components/variableref.h>
#include <seds/SedsModel/package/package.h>

#include "exceptions.h"
#include "specialized/componentactivitiesparser.h"
#include "specialized/componentprimitivesparser.h"
#include "specialized/componentstatesparser.h"
#include "specialized/coreparser.h"
#include "specialized/datatypesparser.h"
#include "specialized/genericsparser.h"
#include "specialized/interfacesparser.h"

using std::placeholders::_1;

namespace seds::parser {

void
ComponentsParser::readComponentSet(model::Package& package, QXmlStreamReader& xmlReader)
{
    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Component")) {
            package.addComponent(readComponent(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ComponentSet");
        }
    }
}

model::VariableRef
ComponentsParser::readVariableRef(QXmlStreamReader& xmlReader)
{
    common::String value;

    for(const auto& attribute : xmlReader.attributes()) {
        if(attribute.name() == QStringLiteral("variableRef")) {
            value = attribute.value().toString();
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        throw UnhandledElement(xmlReader.name(), "VariableRef");
    }

    return model::VariableRef(value);
}

model::Component
ComponentsParser::readComponent(QXmlStreamReader& xmlReader)
{
    model::Component component;

    for(const auto& attribute : xmlReader.attributes()) {
        if(CoreParser::processForNamedEntity(&component, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(CoreParser::processForNamedEntity(&component, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("ProvidedInterfaceSet")) {
            readProvidedInterfaceSet(component, xmlReader);
        } else if(xmlReader.name() == QStringLiteral("RequiredInterfaceSet")) {
            readRequiredInterfaceSet(component, xmlReader);
        } else if(xmlReader.name() == QStringLiteral("DataTypeSet")) {
            DataTypesParser::readDataTypeSet(std::bind(&model::Component::addDataType, &component, _1), xmlReader);
        } else if(xmlReader.name() == QStringLiteral("DeclaredInterfaceSet")) {
            InterfacesParser::readDeclaredInterfaceSet(
                    std::bind(&model::Component::addInterfaceDeclaration, &component, _1), xmlReader);
        } else if(xmlReader.name() == QStringLiteral("Implementation")) {
            component.setImplementation(readComponentImplementation(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Component");
        }
    }

    return component;
}

model::ComponentImplementation
ComponentsParser::readComponentImplementation(QXmlStreamReader& xmlReader)
{
    model::ComponentImplementation implementation;

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("VariableSet")) {
            readVariableSet(implementation, xmlReader);
        } else if(xmlReader.name() == QStringLiteral("ParameterMapSet")) {
            readParameterMapSet(implementation, xmlReader);
        } else if(xmlReader.name() == QStringLiteral("ParameterActivityMapSet")) {
            readParameterActivityMapSet(implementation, xmlReader);
        } else if(xmlReader.name() == QStringLiteral("ActivitySet")) {
            ComponentActivitiesParser::readActivitySet(implementation, xmlReader);
        } else if(xmlReader.name() == QStringLiteral("StateMachineSet")) {
            ComponentStatesParser::readStateMachineSet(implementation, xmlReader);
        } else {
            throw UnhandledElement(xmlReader.name(), "ComponentImplementation");
        }
    }

    return implementation;
}

void
ComponentsParser::readProvidedInterfaceSet(model::Component& component, QXmlStreamReader& xmlReader)
{
    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Interface")) {
            component.addProvidedInterface(readInterface(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ProvidedInterfaceSet");
        }
    }
}

void
ComponentsParser::readRequiredInterfaceSet(model::Component& component, QXmlStreamReader& xmlReader)
{
    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Interface")) {
            component.addRequiredInterface(readInterface(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "RequiredInterfaceSet");
        }
    }
}

model::Interface
ComponentsParser::readInterface(QXmlStreamReader& xmlReader)
{
    model::Interface interface;

    for(const auto& attribute : xmlReader.attributes()) {
        if(CoreParser::processForNamedEntity(&interface, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("type")) {
            interface.setType(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(CoreParser::processForNamedEntity(&interface, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("GenericTypeMapSet")) {
            interface.setGenericTypeMapSet(GenericsParser::readGenericTypeMapSet(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Interface");
        }
    }

    return interface;
}

void
ComponentsParser::readVariableSet(model::ComponentImplementation& implementation, QXmlStreamReader& xmlReader)
{
    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Variable")) {
            implementation.addVariable(readVariable(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "VariableSet");
        }
    }
}

model::Variable
ComponentsParser::readVariable(QXmlStreamReader& xmlReader)
{
    model::Variable variable;

    for(const auto& attribute : xmlReader.attributes()) {
        if(DataTypesParser::processForField(&variable, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("initialValue")) {
            variable.setInitialValue(attribute.name().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(DataTypesParser::processForField(&variable, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "Variable");
        }
    }

    return variable;
}

void
ComponentsParser::readParameterMapSet(model::ComponentImplementation& implementation, QXmlStreamReader& xmlReader)
{
    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("ParameterMap")) {
            implementation.addParameterMap(readParameterMap(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ParameterMapSet");
        }
    }
}

model::ParameterMap
ComponentsParser::readParameterMap(QXmlStreamReader& xmlReader)
{
    model::ParameterMap parameterMap;

    for(const auto& attribute : xmlReader.attributes()) {
        if(ComponentPrimitivesParser::processForParameterRefAttributes(&parameterMap, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("variableRef")) {
            parameterMap.setVariableRef(attribute.name().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(ComponentPrimitivesParser::processForParameterRefAttributes(&parameterMap, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "ParameterMap");
        }
    }

    return parameterMap;
}

void
ComponentsParser::readParameterActivityMapSet(model::ComponentImplementation& implementation,
                                              QXmlStreamReader& xmlReader)
{
    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("ParameterActivityMap")) {
            implementation.addParameterActivityMap(readParameterActivityMap(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ParameterActivityMapSet");
        }
    }
}

model::ParameterActivityMap
ComponentsParser::readParameterActivityMap(QXmlStreamReader& xmlReader)
{
    model::ParameterActivityMap parameterActivityMap;

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("Provided")) {
            parameterActivityMap.setProvided(readParameterMapData(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("Required")) {
            parameterActivityMap.setRequired(readParameterMapData(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("GetActivity")) {
            parameterActivityMap.setGetActivity(ComponentActivitiesParser::readBody(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("SetActivity")) {
            parameterActivityMap.setSetActivity(ComponentActivitiesParser::readBody(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("SetActivityOnly")) {
            parameterActivityMap.setSetActivity(ComponentActivitiesParser::readBody(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ParameterActivityMap");
        }
    }

    return parameterActivityMap;
}

model::ParameterMapData
ComponentsParser::readParameterMapData(QXmlStreamReader& xmlReader)
{
    model::ParameterMapData parameterMapData;

    for(const auto& attribute : xmlReader.attributes()) {
        if(ComponentPrimitivesParser::processForParameterRefAttributes(&parameterMapData, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("name")) {
            parameterMapData.setName(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(ComponentPrimitivesParser::processForParameterRefAttributes(&parameterMapData, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "ParameterMapData");
        }
    }

    return parameterMapData;
}

} // namespace seds::parser
