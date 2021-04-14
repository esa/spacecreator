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

#include "ivobject.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>

namespace ive {

/*!
 * \class ive::Condition
 * \brief The serializable collection of AttrHandler.
 */
static const char *JSON_FIELD_NAME_ItemType = "itemType";
static const char *JSON_FIELD_NAME_Attributes = "attributes";

static QVector<AttrHandler> attrsFromJson(const QJsonObject &jasonObject)
{
    QVector<AttrHandler> res;
    const QJsonArray &arr = jasonObject[JSON_FIELD_NAME_Attributes].toArray();
    for (const auto &obj : arr)
        res.append(AttrHandler(obj.toObject()));
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
    for (const auto &attr : m_attrs)
        jAttrs.append(attr.toJson());

    return { { JSON_FIELD_NAME_ItemType, m_itemType }, { JSON_FIELD_NAME_Attributes, jAttrs } };
}

QStringList Condition::knownTypes()
{
    QStringList res;
    QMetaEnum me = QMetaEnum::fromType<ivm::IVObject::Type>();
    for (int i = 0; i < me.keyCount(); ++i)
        if (ivm::IVObject::Type(me.value(i)) != ivm::IVObject::Type::Unknown)
            res.append(me.key(i));
    return res;
}

bool Condition::isAcceptable(ivm::IVObject *obj) const
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

}
