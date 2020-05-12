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

#include "dynamicproperty.h"

#include <QDebug>
#include <QJsonArray>
#include <QMetaEnum>

namespace aadlinterface {

struct DynamicProperty::DynamicPropertyPrivate {
    QString m_name;
    DynamicProperty::Type m_type;
    DynamicProperty::Scopes m_scope;
    QVector<QVariant> m_vals;

    static const struct Consts {
        const QString Name { "name" };
        const QString Scope { "scope" };
        const QString Type { "type" };
        const QString Range { "range" };
    } JsonFileds;
};

const DynamicProperty::DynamicPropertyPrivate::Consts DynamicProperty::DynamicPropertyPrivate::JsonFileds = {};

DynamicProperty::DynamicProperty(const QString &name, Type t, const Scopes &s, const QVector<QVariant> &enumVals)
    : d(new DynamicPropertyPrivate({ name, t, s, enumVals }))
{
}

DynamicProperty::~DynamicProperty() { }

QString DynamicProperty::name() const
{
    return d->m_name;
}
void DynamicProperty::setName(const QString &name)
{
    d->m_name = name;
}

DynamicProperty::Type DynamicProperty::type() const
{
    return d->m_type;
}

void DynamicProperty::setType(DynamicProperty::Type t)
{
    d->m_type = t;
}

DynamicProperty::Scopes DynamicProperty::scope() const
{
    return d->m_scope;
}

void DynamicProperty::setScope(const DynamicProperty::Scopes &s)
{
    d->m_scope = s;
}

QVector<QVariant> DynamicProperty::valuesList() const
{
    return d->m_vals;
}

void DynamicProperty::setValuesList(const QVector<QVariant> &range)
{
    d->m_vals = range;
}

QJsonObject DynamicProperty::toJson() const
{
    auto scopesToJSON = [](DynamicProperty::Scopes s) {
        QString result;
        if (s != DynamicProperty::Scopes(DynamicProperty::Scope::None)) {
            static const QMetaEnum &meta = QMetaEnum::fromType<DynamicProperty::Scopes>();
            result = meta.valueToKeys(static_cast<int>(s));
        }
        return result;
    };

    auto typeToString = [](DynamicProperty::Type t) {
        static const QMetaEnum &meta = QMetaEnum::fromType<DynamicProperty::Type>();
        return meta.key(static_cast<int>(t));
    };

    QJsonObject jObj;
    jObj[DynamicPropertyPrivate::JsonFileds.Name] = name();
    jObj[DynamicPropertyPrivate::JsonFileds.Type] = typeToString(type());
    jObj[DynamicPropertyPrivate::JsonFileds.Scope] = scopesToJSON(scope());

    QJsonArray jArr;
    for (const auto &val : valuesList())
        jArr.append(QVariant::fromValue(val).toJsonValue());
    if (!jArr.isEmpty())
        jObj[DynamicPropertyPrivate::JsonFileds.Range] = jArr;
    return jObj;
}

DynamicProperty *DynamicProperty::fromJson(const QJsonObject &json)
{
    const QString &name = json[DynamicPropertyPrivate::JsonFileds.Name].toString();
    if (name.isEmpty())
        return nullptr;

    if (!json.contains(DynamicPropertyPrivate::JsonFileds.Type))
        return nullptr;
    const QString &typeStr = json[DynamicPropertyPrivate::JsonFileds.Type].toString();
    static const QMetaEnum &typeMeta = QMetaEnum::fromType<DynamicProperty::Type>();
    const DynamicProperty::Type t = static_cast<DynamicProperty::Type>(typeMeta.keyToValue(typeStr.toUtf8()));

    if (!json.contains(DynamicPropertyPrivate::JsonFileds.Scope))
        return nullptr;
    const QString &scopeStr = json[DynamicPropertyPrivate::JsonFileds.Scope].toString();
    static const QMetaEnum &scopesMeta = QMetaEnum::fromType<DynamicProperty::Scopes>();
    const DynamicProperty::Scopes s(scopesMeta.keysToValue(scopeStr.toUtf8()));

    return new DynamicProperty(
            name, t, s, json[DynamicPropertyPrivate::JsonFileds.Range].toArray().toVariantList().toVector());
}

}
