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

#include "archetypexmlreader.h"

#include "entityattribute.h"
#include "errorhub.h"
#include "parameter.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QVariant>
#include <QVector>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>

namespace ivm {

using namespace ivm::meta;

static inline QString attrValue(const EntityAttributes &attrs, const QString &name, const QString &defaultValue = {})
{
    const EntityAttribute &attr = attrs.value(name);
    return attr.isValid() ? attr.value<QString>() : defaultValue;
}

static inline QString attrValue(
        const EntityAttributes &attrs, meta::ArchetypeProps::Token token, const QString &defaultValue = {})
{
    return attrValue(attrs, meta::ArchetypeProps::token(token), defaultValue);
}

void CurrentObjectHolder::set(ArchetypeObject *object)
{
    m_object = object;
    m_function = m_object ? m_object->as<FunctionArchetype *>() : nullptr;
    m_iface = m_object ? m_object->as<InterfaceArchetype *>() : nullptr;
    m_parameter = m_object ? m_object->as<ParameterArchetype *>() : nullptr;
}

ArchetypeXMLReader::ArchetypeXMLReader(QObject *parent)
    : shared::XmlReader(parent)
{
}

ArchetypeXMLReader::~ArchetypeXMLReader() = default;

QVector<ArchetypeObject *> ArchetypeXMLReader::parsedObjects() const
{
    return m_allObjects;
}

void ArchetypeXMLReader::processTagOpen(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    const EntityAttributes attrs = attributes(xml.attributes());
    const QString objectName = attrValue(attrs, ArchetypeProps::Token::name);

    ArchetypeObject *object { nullptr };
    const ArchetypeProps::Token token = ArchetypeProps::token(tagName);
    switch (token) {
    case ArchetypeProps::Token::FunctionArchetype: {
        object = new FunctionArchetype(objectName);
        break;
    }
    case ArchetypeProps::Token::InterfaceArchetype: {
        if (m_currentObject.getFunctionObject()) {
            object = createInterface(objectName, attrs);
        }
        break;
    }
    case ArchetypeProps::Token::ParameterArchetype: {
        if (m_currentObject.getInterfaceObject()) {
            object = createParameter(objectName, attrs);
        }
        break;
    }
    default:
        static const QString msg("The '%1' is unknown/unexpected here: %2@%3 %4");
        shared::ErrorHub::addError(shared::ErrorItem::Warning,
                msg.arg(tagName, QString::number(xml.lineNumber()), QString::number(xml.columnNumber()),
                        xml.tokenString()));
        break;
    }

    if (object) {
        object->setEntityAttributes(attrs);
        m_currentObject.set(object);
        addCurrentObject();
    }
}

void ArchetypeXMLReader::processTagClose(QXmlStreamReader &xml)
{
    const QString &tagName = xml.name().toString();
    switch (ArchetypeProps::token(tagName)) {
    case ArchetypeProps::Token::FunctionArchetype:
    case ArchetypeProps::Token::InterfaceArchetype:
    case ArchetypeProps::Token::ParameterArchetype: {
        m_currentObject.set(m_currentObject.getObject() ? m_currentObject.getObject()->parentObject() : nullptr);
        break;
    }
    default:
        break;
    }
}

QString ArchetypeXMLReader::rootElementName() const
{
    return ArchetypeProps::token(ArchetypeProps::Token::ArchetypeLibrary);
}

InterfaceArchetype *ArchetypeXMLReader::createInterface(const QString &objectName, const EntityAttributes &attrs)
{
    FunctionArchetype *currentFunction = m_currentObject.getFunctionObject();
    const QString interfaceTypeString = attrValue(attrs, ArchetypeProps::Token::interfaceType);
    const QString interfaceKindString = attrValue(attrs, ArchetypeProps::Token::kind);
    const QString interfaceLayerString = attrValue(attrs, ArchetypeProps::Token::layer);
    InterfaceArchetype::InterfaceType interfaceType = InterfaceArchetype::interfaceTypeFromString(interfaceTypeString);
    InterfaceArchetype::OperationKind operationKind = InterfaceArchetype::operationKindFromString(interfaceKindString);

    InterfaceArchetype *interface =
            new InterfaceArchetype(objectName, currentFunction, interfaceType, operationKind, interfaceLayerString);
    currentFunction->addInterface(interface);
    return interface;
}

ParameterArchetype *ArchetypeXMLReader::createParameter(const QString &objectName, const EntityAttributes &attrs)
{
    InterfaceArchetype *currentInterface = m_currentObject.getInterfaceObject();
    const QString parameterTypeString = attrValue(attrs, ArchetypeProps::Token::type);
    const QString parameterDirectionString = attrValue(attrs, ArchetypeProps::Token::direction);
    ParameterArchetype::ParameterDirection direction =
            ParameterArchetype::directionFromString(parameterDirectionString);

    ParameterArchetype *parameter =
            new ParameterArchetype(objectName, currentInterface, parameterTypeString, direction);
    currentInterface->addParameter(parameter);
    return parameter;
}

void ArchetypeXMLReader::addCurrentObject()
{
    if (!m_currentObject.getObject()) {
        return;
    }
    if (!m_allObjects.contains(m_currentObject.getObject())) {
        m_allObjects.append(m_currentObject.getObject());
    }
}

}
