/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "entityattribute.h"

EntityAttribute::EntityAttribute() { }

EntityAttribute::EntityAttribute(const QString &attrName, const QVariant &attrValue, EntityAttribute::Type type)
    : m_name(attrName)
    , m_value(attrValue)
    , m_type(type)
{
}

QVariant EntityAttribute::value() const
{
    return m_value;
}

void EntityAttribute::setValue(const QVariant &value)
{
    m_value = value;
}

QString EntityAttribute::name() const
{
    return m_name;
}

bool EntityAttribute::isProperty() const
{
    return m_type == Type::Property;
}

bool EntityAttribute::isAttribute() const
{
    return m_type == Type::Attribute;
}

EntityAttribute::Type EntityAttribute::type() const
{
    return m_type;
}

bool EntityAttribute::isValid() const
{
    return !m_name.isEmpty();
}

bool EntityAttribute::isNull() const
{
    return m_value.isNull();
}

bool operator==(const EntityAttribute &lhs, const EntityAttribute &rhs)
{
    return lhs.name() == rhs.name() && lhs.value() == rhs.value() && lhs.type() == rhs.type();
}
