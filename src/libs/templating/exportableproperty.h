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

#pragma once

#include "abstractexportableobject.h"

#include <QVariant>

namespace templating {

/**
 * @brief The ExportableProperty class is needed for string templates only.
 * It seems Grantlee doesn't fully support Django syntax like
 * @code
 *  {% for key, value in iface.properties %}
 *      <Property name="{{ key }}" value="{{ value }}"/>
 *  {% endfor %}
 * @endcode
 * Therefore AADLObject has a property "properties" and "attributes"
 * which are lists of ExportedAADLProperty
 * and thus it's possible to write like this:
 * @code
 *  {% for property in iface.properties %}
 *      <Property name="{{ property.name }}" value="{{ property.value }}"/>
 *  {% endfor %}
 * @endcode
 */

class ExportableProperty
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QVariant value READ value)
public:
    ExportableProperty(const QString &name = QString(), const QVariant &value = QVariant())
        : m_name(name), m_value(value) {}

    inline QString name() const { return m_name; }
    inline QVariant value() const { return m_value; }

private:
    const QString m_name;
    const QVariant m_value;
};

}

Q_DECLARE_METATYPE(templating::ExportableProperty)
Q_DECLARE_TYPEINFO(templating::ExportableProperty, Q_MOVABLE_TYPE);
