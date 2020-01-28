/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "condition.h"

#include "tab_aadl/aadlobject.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>

namespace taste3 {
namespace ctx {

static const QString JSON_FIELD_NAME_ItemType = "itemType";
static const QString JSON_FIELD_NAME_Attributes = "attributes";

Condition::Condition(const QJsonObject &jObj)
    : m_itemType(jObj[JSON_FIELD_NAME_ItemType].toString())
{
    const QJsonArray &jArr = jObj[JSON_FIELD_NAME_Attributes].toArray();
    for (auto jObj : jArr)
        m_attrs.append(AttrHandler(jObj.toObject()));
}

QJsonObject Condition::toJson() const
{
    QJsonArray jAttrs;
    for (auto attr : m_attrs)
        jAttrs.append(attr.toJson());

    return { { JSON_FIELD_NAME_ItemType, m_itemType }, { JSON_FIELD_NAME_Attributes, jAttrs } };
}

QStringList knownTypes()
{
    QStringList res;
    QMetaEnum me = QMetaEnum::fromType<taste3::aadl::AADLObject::AADLObjectType>();
    for (int i = 0; i < me.keyCount(); ++i)
        res.append(me.key(i));
    return res;
}

bool Condition::isAcceptable(aadl::AADLObject *obj) const
{
    if (!obj)
        return false;

    if (m_itemType != "*") {
        static const QStringList &names = knownTypes();
        const QString name = QString("AADL%1").arg(m_itemType);
        if (!names.contains(name))
            return false;
    }

    for (const AttrHandler &attr : m_attrs) {
        if (attr.m_value != "*")
            if (obj->attr(attr.m_title, QString()).toString() != attr.m_value)
                return false;
    }

    return true;
}

} // ns ctx
} // ns taste3
