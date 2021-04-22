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

#pragma once

#include <QVariant>

class EntityAttribute
{
public:
    enum class Type {
        Attribute,
        Property
    };

    explicit EntityAttribute();
    explicit EntityAttribute(const QString &attrName, const QVariant &attrValue, EntityAttribute::Type type);

    template<typename T>
    T value() const
    {
        return m_value.value<T>();
    }
    QVariant value() const;
    void setValue(const QVariant &value);

    QString name() const;
    bool isProperty() const;
    bool isAttribute() const;
    Type type() const;

    bool isValid() const;
    bool isNull() const;

private:
    QString m_name;
    QVariant m_value;
    Type m_type { Type::Attribute };
};

typedef QHash<QString, EntityAttribute> EntityAttributes;
