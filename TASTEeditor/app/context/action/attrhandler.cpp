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

#include "attrhandler.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

namespace taste3 {
namespace ctx {

/*!
 * \class taste3::ctx::AttrHandler
 * \brief The serializable wrapper for AADLObject's attribute to be used with taste3::ctx::Action
 */
static const QString JSON_FIELD_NAME_Title = "title";
static const QString JSON_FIELD_NAME_Value = "value";

AttrHandler::AttrHandler(const QJsonObject &jObj)
    : m_title(jObj[JSON_FIELD_NAME_Title].toString())
    , m_value(jObj[JSON_FIELD_NAME_Value].toVariant())
{
}

QJsonObject AttrHandler::toJson() const
{
    return { { JSON_FIELD_NAME_Title, m_title }, { JSON_FIELD_NAME_Value, QJsonValue::fromVariant(m_value) } };
}

bool AttrHandler::operator==(const AttrHandler &other) const
{
    return m_title == other.m_title && m_value == other.m_value;
}

} // ns ctx
} // ns taste3
