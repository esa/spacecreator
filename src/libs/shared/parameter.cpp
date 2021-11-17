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

#include "parameter.h"

#include <QMetaEnum>

namespace shared {

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

/*!
   Returns the name where " " characters are replaced by "_"
   @see name
 */
QString BasicParameter::underscoredName() const
{
    QString converted = m_paramName;
    return converted.replace(" ", "_");
}

BasicParameter::Type BasicParameter::paramType() const
{
    return m_paramType;
}

bool BasicParameter::setParamType(const BasicParameter::Type &type)
{
    if (m_paramType == type) {
        return false;
    }

    m_paramType = type;

    if (m_paramType != BasicParameter::Type::Other) {
        setParamTypeName(shared::typeName(m_paramType));
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
    if (m_typeName == shared::typeName(BasicParameter::Type::Timer))
        setParamType(BasicParameter::Type::Timer);
    else if (m_typeName == shared::typeName(BasicParameter::Type::Directive))
        setParamType(BasicParameter::Type::Directive);
    else
        setParamType(BasicParameter::Type::Other);

    return true;
}

/*!
   Returns the param type name where "-" characters are replaced by "_"
   @see paramTypeName
 */
QString BasicParameter::underscoredType() const
{
    QString converted = m_typeName;
    return converted.replace("-", "_");
}

bool BasicParameter::operator==(const BasicParameter &other) const
{
    return m_paramName == other.m_paramName && m_paramType == other.m_paramType && m_typeName == other.m_typeName;
}

bool BasicParameter::isValidValue(const QVariant &value) const
{
    return value.isValid();
}

QString BasicParameter::toString() const
{
    QString result;

    const QStringList fields = { paramTypeName(), name() };
    for (const QString &field : fields) {
        if (!result.isEmpty())
            result += shared::kStringDelemiter;
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

ContextParameter::~ContextParameter() {}

QVariant ContextParameter::defaultValue() const
{
    return (paramType() == Type::Timer) ? QVariant() : m_defaultValue;
}

bool ContextParameter::setDefaultValue(const QVariant &value)
{
    if (paramType() == Type::Timer || m_defaultValue == value)
        return false;

    if (m_defaultValue != value) {
        m_defaultValue = value;
        return true;
    }

    return false;
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
            result += shared::kStringDelemiter;

        result += field;
    }

    return result;
}

bool ContextParameter::isNull() const
{
    return BasicParameter::isNull() || this->operator==({});
}

InterfaceParameter::InterfaceParameter(
        const QString &name, Type t, const QString &paramTypeName, const QString &encoding, Direction dir)
    : BasicParameter(name, t, paramTypeName)
    , m_encoding(encoding)
    , m_direction(dir)
{
}

InterfaceParameter::~InterfaceParameter() {}

QString InterfaceParameter::encoding() const
{
    return m_encoding;
}

bool InterfaceParameter::setEncoding(const QString &encoding)
{
    if (m_encoding == encoding)
        return false;

    m_encoding = encoding;
    return true;
}

InterfaceParameter::Direction InterfaceParameter::direction() const
{
    return m_direction;
}

bool InterfaceParameter::setDirection(InterfaceParameter::Direction dir)
{
    if (m_direction == dir)
        return false;

    m_direction = dir;
    return true;
}

bool InterfaceParameter::operator==(const InterfaceParameter &other) const
{
    return BasicParameter::operator==(other) && m_encoding == other.m_encoding && m_direction == other.m_direction;
}

QString InterfaceParameter::toString() const
{
    QString result = BasicParameter::toString();

    const QStringList fields = { shared::typeName(direction()), encoding() };
    for (const QString &field : fields) {
        if (!result.isEmpty())
            result += shared::kStringDelemiter;

        result += field;
    }

    return result;
}

bool InterfaceParameter::isNull() const
{
    return BasicParameter::isNull() || this->operator==({});
}

QString toString(const BasicParameter &parameter)
{
    return parameter.toString();
}

} // namespace shared
