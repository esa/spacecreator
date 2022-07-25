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

#include "interfacearchetype.h"

namespace ivm {

struct InterfaceArchetypePrivate {
    explicit InterfaceArchetypePrivate(const InterfaceArchetype::InterfaceType &interfaceType,
            const InterfaceArchetype::OperationKind &operationKind)
        : m_interfaceType(interfaceType)
        , m_operationKind(operationKind)
    {
    }
    InterfaceArchetype::InterfaceType m_interfaceType;
    InterfaceArchetype::OperationKind m_operationKind;
    QVector<ParameterArchetype *> m_parameters;
};

InterfaceArchetype::InterfaceArchetype(const QString &title, QObject *parent, const InterfaceType &interfaceType,
        const OperationKind &operationKind, const QString &layer)
    : ArchetypeObject(title, ArchetypeObject::Type::InterfaceArchetype, parent)
    , m_interfacePrivate(std::make_unique<InterfaceArchetypePrivate>(interfaceType, operationKind))
{
    setEntityAttribute(meta::ArchetypeProps::token(meta::ArchetypeProps::Token::layer), layer);
}

InterfaceArchetype::~InterfaceArchetype() = default;

InterfaceArchetype::InterfaceType InterfaceArchetype::getInterfaceType() const
{
    return m_interfacePrivate->m_interfaceType;
}

void InterfaceArchetype::setInterfaceType(const InterfaceArchetype::InterfaceType &interfaceType)
{
    m_interfacePrivate->m_interfaceType = interfaceType;
}

InterfaceArchetype::OperationKind InterfaceArchetype::getOperationKind() const
{
    return m_interfacePrivate->m_operationKind;
}

void InterfaceArchetype::setOperationKind(const InterfaceArchetype::OperationKind &operationKind)
{
    m_interfacePrivate->m_operationKind = operationKind;
}

QString InterfaceArchetype::getLayer() const
{
    return entityAttributeValue(meta::ArchetypeProps::token(meta::ArchetypeProps::Token::layer)).toString();
}

void InterfaceArchetype::setLayer(const QString &layer)
{
    setEntityAttribute(meta::ArchetypeProps::token(meta::ArchetypeProps::Token::layer), layer);
}

QVector<ParameterArchetype *> InterfaceArchetype::getParameters() const{
    return m_interfacePrivate->m_parameters;
}

void InterfaceArchetype::addParameter(ParameterArchetype *parameterArchetype){
    if(!parameterArchetype){
        return;
    }
    parameterArchetype->setParentObject(this);
    m_interfacePrivate->m_parameters.append(parameterArchetype);
}

void InterfaceArchetype::removeParameter(ParameterArchetype *parameterArchetype){
    if(!parameterArchetype){
        return;
    }
    parameterArchetype->setParentObject(nullptr);
    m_interfacePrivate->m_parameters.removeAll(parameterArchetype);
}

QString InterfaceArchetype::interfaceTypeToString(const InterfaceArchetype::InterfaceType &interfaceType)
{
    const QMetaEnum &qMetaEnum = QMetaEnum::fromType<ivm::InterfaceArchetype::InterfaceType>();
    return QString::fromLatin1(qMetaEnum.valueToKey(static_cast<int>(interfaceType)));
}

InterfaceArchetype::InterfaceType InterfaceArchetype::interfaceTypeFromString(
        const QString &interfaceTypeString, const InterfaceArchetype::InterfaceType &defaultInterfaceType)
{
    const QMetaEnum &qMetaEnum = QMetaEnum::fromType<ivm::InterfaceArchetype::InterfaceType>();
    bool ok = false;
    const auto kind = static_cast<InterfaceArchetype::InterfaceType>(
            qMetaEnum.keyToValue(interfaceTypeString.toLatin1().data(), &ok));
    return ok ? kind : defaultInterfaceType;
}

QString InterfaceArchetype::operationKindToString(const InterfaceArchetype::OperationKind &operationKind)
{
    const QMetaEnum &qMetaEnum = QMetaEnum::fromType<ivm::InterfaceArchetype::OperationKind>();
    return QString::fromLatin1(qMetaEnum.valueToKey(static_cast<int>(operationKind)));
}

InterfaceArchetype::OperationKind InterfaceArchetype::operationKindFromString(
        const QString &operationKindString, const InterfaceArchetype::OperationKind &defaultOperationKind)
{
    const QMetaEnum &qMetaEnum = QMetaEnum::fromType<ivm::InterfaceArchetype::OperationKind>();
    bool ok = false;
    const auto kind = static_cast<InterfaceArchetype::OperationKind>(
            qMetaEnum.keyToValue(operationKindString.toLatin1().data(), &ok));
    return ok ? kind : defaultOperationKind;
}
}
