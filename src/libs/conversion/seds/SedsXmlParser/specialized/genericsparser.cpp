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

#include "specialized/genericsparser.h"

#include <QXmlStreamReader>

#include <seds/SedsModel/generics/genericalternate.h>
#include <seds/SedsModel/generics/genericalternateset.h>
#include <seds/SedsModel/generics/generictype.h>
#include <seds/SedsModel/generics/generictypemap.h>
#include <seds/SedsModel/generics/generictypemapset.h>
#include <seds/SedsModel/interfaces/interfacedeclaration.h>

#include "exceptions.h"
#include "specialized/coreparser.h"
#include "specialized/datatypesparser.h"

namespace seds::parser {

void
GenericsParser::readGenericTypeSet(model::InterfaceDeclaration& interfaceDeclaration, QXmlStreamReader& xmlReader)
{
    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("GenericType")) {
            interfaceDeclaration.addGenericType(readGenericType(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "GenericTypeSet");
        }
    }
}

model::GenericType
GenericsParser::readGenericType(QXmlStreamReader& xmlReader)
{
    model::GenericType genericType;

    for(const auto& attribute : xmlReader.attributes()) {
        if(CoreParser::processForNamedEntity(&genericType, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("baseType")) {
            genericType.setBaseType(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(CoreParser::processForNamedEntity(&genericType, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "GenericType");
        }
    }

    return genericType;
}

model::GenericTypeMapSet
GenericsParser::readGenericTypeMapSet(QXmlStreamReader& xmlReader)
{
    model::GenericTypeMapSet genericTypeMapSet;

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("AlternateSet")) {
            genericTypeMapSet.setAlternateSet(readGenericAlternateSet(xmlReader));
        } else if(xmlReader.name() == QStringLiteral("GenericTypeMap")) {
            genericTypeMapSet.addGenericTypeMap(readGenericTypeMap(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "GenericTypeMapSet");
        }
    }

    return genericTypeMapSet;
}

model::GenericTypeMap
GenericsParser::readGenericTypeMap(QXmlStreamReader& xmlReader)
{
    model::GenericTypeMap genericTypeMap;

    for(const auto& attribute : xmlReader.attributes()) {
        if(DataTypesParser::processForField(&genericTypeMap, attribute)) {
            continue;
        } else if(attribute.name() == QStringLiteral("fixedValue")) {
            genericTypeMap.setFixedValue(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(DataTypesParser::processForField(&genericTypeMap, xmlReader)) {
            continue;
        } else {
            throw UnhandledElement(xmlReader.name(), "GenericTypeMap");
        }
    }

    return genericTypeMap;
}

model::GenericAlternateSet
GenericsParser::readGenericAlternateSet(QXmlStreamReader& xmlReader)
{
    model::GenericAlternateSet genericAlternateSet;

    for(const auto& attribute : xmlReader.attributes()) {
        if(CoreParser::processForDescription(&genericAlternateSet, attribute)) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while(xmlReader.readNextStartElement()) {
        if(CoreParser::processForDescription(&genericAlternateSet, xmlReader)) {
            continue;
        } else if(xmlReader.name() == QStringLiteral("Alternate")) {
            genericAlternateSet.addAlternate(readGenericAlternate(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ProvidedInterfaceSet");
        }
    }

    return genericAlternateSet;
}

model::GenericAlternate
GenericsParser::readGenericAlternate(QXmlStreamReader& xmlReader)
{
    model::GenericAlternate genericAlternate;

    for(const auto& attribute : xmlReader.attributes()) {
        throw UnhandledAttribute(attribute.name(), xmlReader.name());
    }

    while(xmlReader.readNextStartElement()) {
        if(xmlReader.name() == QStringLiteral("GenericTypeMap")) {
            genericAlternate.setGenericTypeMap(readGenericTypeMap(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "GenericAlternate");
        }
    }

    return genericAlternate;
}

} // namespace seds::parser
