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

namespace taste3 {
namespace aadl {

BasicParameter::BasicParameter(const QString &name, Type t, const QString &paramTypeName)
    : m_paramName(name)
    , m_paramType(t)
    , m_typeName(paramTypeName)
{
}

BasicParameter::~BasicParameter() {}

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

bool BasicParameter::setParamType(const BasicParameter::Type &type)
{
    if (m_paramType == type)
        return false;

    m_paramType = type;

    switch (m_paramType) {
    case BasicParameter::Type::Timer: {
        setParamTypeName(QObject::tr("Timer"));
        break;
    }
    case BasicParameter::Type::Directive: {
        setParamTypeName(QObject::tr("Directive"));
        break;
    }
    default:
        break;
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
    if (m_typeName == QObject::tr("Timer"))
        setParamType(BasicParameter::Type::Timer);
    else if (m_typeName == QObject::tr("Directive"))
        setParamType(BasicParameter::Type::Directive);
    else
        setParamType(BasicParameter::Type::Other);

    return true;
}

bool BasicParameter::operator==(const BasicParameter &other) const
{
    return m_paramName == other.m_paramName && m_paramType == other.m_paramType && m_typeName == other.m_typeName;
}

ContextParameter::ContextParameter(const QString &name, Type t, const QString &paramTypeName, const QVariant &val)
    : BasicParameter(name, t, paramTypeName)
    , m_defaultValue(val)
{
}

ContextParameter::~ContextParameter() {}

QVariant ContextParameter::defaultValue() const
{
    return (paramType() == Type::Timer) ? QVariant() : m_defaultValue;
}

bool ContextParameter::setDefaultValue(const QVariant &value)
{
    if (paramType() == Type::Timer || m_defaultValue == value)
        return false;

    m_defaultValue = value;
    return true;
}

bool ContextParameter::operator==(const ContextParameter &other) const
{
    return BasicParameter::operator==(other) && m_defaultValue == other.m_defaultValue;
}

IfaceParameter::IfaceParameter(const QString &name, Type t, const QString &paramTypeName, const QString &encoding,
                               Direction dir)
    : BasicParameter(name, t, paramTypeName)
    , m_encoding(encoding)
    , m_direction(dir)
{
}

IfaceParameter::~IfaceParameter() {}

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

bool IfaceParameter::operator==(const IfaceParameter &other) const
{
    return BasicParameter::operator==(other) && m_encoding == other.m_encoding && m_direction == other.m_direction;
}

} // ns aadl
} // ns taste3
