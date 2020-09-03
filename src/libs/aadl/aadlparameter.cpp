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

#include "aadlparameter.h"

#include "asn1/definitions.h"
#include "asn1/file.h"
#include "asn1/typeassignment.h"
#include "asn1/types/type.h"
#include "asn1valueparser.h"
#include "datatypesstorage.h"

namespace aadl {

const QString BasicParameter::ToStringDelemiter = QString("::");

BasicParameter::BasicParameter(const QString &name, Type t, const QString &paramTypeName)
    : m_paramName(name)
    , m_paramType(t)
    , m_typeName(paramTypeName)
{
}

BasicParameter::~BasicParameter() { }

QString BasicParameter::name() const
{
    return m_paramName;
}

bool BasicParameter::setName(const QString &name)
{
    if (m_paramName == name)
        return false;

    m_paramName = name;
    return true;
}

BasicParameter::Type BasicParameter::paramType() const
{
    return m_paramType;
}

QString BasicParameter::typeName(const BasicParameter::Type &type)
{
    switch (type) {
    case BasicParameter::Type::Timer: {
        return QObject::tr("Timer");
    }
    case BasicParameter::Type::Directive: {
        return QObject::tr("Directive");
    }
    default:
        return QObject::tr("Other");
    }
}

bool BasicParameter::setParamType(const BasicParameter::Type &type)
{
    if (m_paramType == type) {
        return false;
    }

    m_paramType = type;

    if (m_paramType != BasicParameter::Type::Other) {
        setParamTypeName(typeName(m_paramType));
    }

    return true;
}

QString BasicParameter::paramTypeName() const
{
    return m_typeName;
}

bool BasicParameter::setParamTypeName(const QString &typeName)
{
    if (m_typeName == typeName)
        return false;

    m_typeName = typeName;
    if (m_typeName == BasicParameter::typeName(BasicParameter::Type::Timer))
        setParamType(BasicParameter::Type::Timer);
    else if (m_typeName == BasicParameter::typeName(BasicParameter::Type::Directive))
        setParamType(BasicParameter::Type::Directive);
    else
        setParamType(BasicParameter::Type::Other);

    return true;
}

bool BasicParameter::operator==(const BasicParameter &other) const
{
    return m_paramName == other.m_paramName && m_paramType == other.m_paramType && m_typeName == other.m_typeName;
}

bool BasicParameter::isValidValue(DataTypesStorage *dataTypes, const QVariant &value) const
{
    if (!dataTypes) {
        return true;
    }
    const Asn1Acn::Types::Type *basicDataType = dataTypes->typeFromName(m_typeName);
    if (!basicDataType) {
        return true;
    }

    Asn1Acn::Asn1ValueParser valueParser;
    bool ok;
    valueParser.parseAsn1Value(basicDataType, value.toString(), &ok);
    return ok;
}

QString BasicParameter::toString() const
{
    QString result;

    const QStringList fields = { paramTypeName(), name() };
    for (const QString &field : fields) {
        if (!result.isEmpty())
            result += ToStringDelemiter;
        result += field;
    }

    return result;
}

bool BasicParameter::isNull() const
{
    return this->operator==({});
}

ContextParameter::ContextParameter(const QString &name, Type t, const QString &paramTypeName, const QVariant &val)
    : BasicParameter(name, t, paramTypeName)
    , m_defaultValue(val)
{
}

ContextParameter::~ContextParameter() { }

QVariant ContextParameter::defaultValue() const
{
    return (paramType() == Type::Timer) ? QVariant() : m_defaultValue;
}

bool ContextParameter::setDefaultValue(DataTypesStorage *dataTypes, const QVariant &value)
{
    if (paramType() == Type::Timer || m_defaultValue == value)
        return false;

    if (!isValidValue(dataTypes, value))
        return false;

    m_defaultValue = value;
    return true;
}

bool ContextParameter::operator==(const ContextParameter &other) const
{
    return BasicParameter::operator==(other) && m_defaultValue == other.m_defaultValue;
}

bool ContextParameter::setParamType(const BasicParameter::Type &type)
{
    if (!BasicParameter::setParamType(type))
        return false;

    m_defaultValue.clear();
    return true;
}

bool ContextParameter::setParamTypeName(const QString &typeName)
{
    if (!BasicParameter::setParamTypeName(typeName))
        return false;

    m_defaultValue.clear();
    return true;
}

QString ContextParameter::toString() const
{
    QString result = BasicParameter::toString();

    const QStringList fields = { defaultValue().toString() };
    for (const QString &field : fields) {
        if (!result.isEmpty())
            result += ToStringDelemiter;

        result += field;
    }

    return result;
}

bool ContextParameter::isNull() const
{
    return BasicParameter::isNull() || this->operator==({});
}

IfaceParameter::IfaceParameter(
        const QString &name, Type t, const QString &paramTypeName, const QString &encoding, Direction dir)
    : BasicParameter(name, t, paramTypeName)
    , m_encoding(encoding)
    , m_direction(dir)
{
}

IfaceParameter::~IfaceParameter() { }

QString IfaceParameter::encoding() const
{
    return m_encoding;
}

bool IfaceParameter::setEncoding(const QString &encoding)
{
    if (m_encoding == encoding)
        return false;

    m_encoding = encoding;
    return true;
}

IfaceParameter::Direction IfaceParameter::direction() const
{
    return m_direction;
}

bool IfaceParameter::setDirection(IfaceParameter::Direction dir)
{
    if (m_direction == dir)
        return false;

    m_direction = dir;
    return true;
}

QString IfaceParameter::directionName(IfaceParameter::Direction dir)
{
    static const QStringList names { QObject::tr("IN"), QObject::tr("OUT") };
    switch (dir) {
    case IfaceParameter::Direction::In:
        return names.first();
    default:
        return names.last();
    }
}

IfaceParameter::Direction IfaceParameter::directionFromName(const QString &dir)
{
    static const QMap<QString, IfaceParameter::Direction> names { { QObject::tr("IN"), IfaceParameter::Direction::In },
        { QObject::tr("OUT"), IfaceParameter::Direction::Out } };
    const QString &name = dir.toUpper();
    return names.contains(name) ? names.value(name) : names.first();
}

bool IfaceParameter::operator==(const IfaceParameter &other) const
{
    return BasicParameter::operator==(other) && m_encoding == other.m_encoding && m_direction == other.m_direction;
}

QString IfaceParameter::toString() const
{
    QString result = BasicParameter::toString();

    const QStringList fields = { directionName(direction()), encoding() };
    for (const QString &field : fields) {
        if (!result.isEmpty())
            result += ToStringDelemiter;

        result += field;
    }

    return result;
}

bool IfaceParameter::isNull() const
{
    return BasicParameter::isNull() || this->operator==({});
}
}
