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

/*!
 * \class taste3::ctx::Condition
 * \brief The serializable collection of taste3::ctx::AttrHandler.
 */
static const QString JSON_FIELD_NAME_ItemType = "itemType";
static const QString JSON_FIELD_NAME_Attributes = "attributes";

QVector<AttrHandler> attrsFromJson(const QJsonObject &jObj)
{
    QVector<AttrHandler> res;
    const QJsonArray &jArr = jObj[JSON_FIELD_NAME_Attributes].toArray();
    for (auto jObj : jArr)
        res.append(AttrHandler(jObj.toObject()));
    return res;
}

Condition::Condition(const QJsonObject &jObj)
    : m_itemType(jObj[JSON_FIELD_NAME_ItemType].toString())
    , m_attrs(attrsFromJson(jObj))
{
}

Condition Condition::createGlobal()
{
    Condition c;
    c.m_itemType = "*";

    return c;
}

QJsonObject Condition::toJson() const
{
    QJsonArray jAttrs;
    for (auto attr : m_attrs)
        jAttrs.append(attr.toJson());

    return { { JSON_FIELD_NAME_ItemType, m_itemType }, { JSON_FIELD_NAME_Attributes, jAttrs } };
}

QStringList Condition::knownTypes()
{
    QStringList res;
    QMetaEnum me = QMetaEnum::fromType<taste3::aadl::AADLObject::Type>();
    for (int i = 0; i < me.keyCount(); ++i)
        if (taste3::aadl::AADLObject::Type(me.value(i)) != taste3::aadl::AADLObject::Type::Unknown)
            res.append(me.key(i));
    return res;
}

bool Condition::isAcceptable(aadl::AADLObject *obj) const
{
    if (m_itemType != "*") {
        static const QStringList &names = knownTypes();
        const QString name(m_itemType);
        if (!names.contains(name))
            return false;
    }

    for (const AttrHandler &attr : m_attrs) {
        if (attr.m_value != "*")
            if (obj && obj->attr(attr.m_title, QString()).toString() != attr.m_value)
                return false;
    }

    return true;
}

} // ns ctx
} // ns taste3
