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

#include "action.h"

#include "aadlobject.h"

#include <QJsonArray>

namespace ive {

/*!
 * \class ive::Action
 * \brief Scriptable action used to customize the context menu.
 * Contains its UI title, the title of target application action or external application and its arguments,
 * the list of conditions to detect if this action can be invoked or not.
 * Stored in JSON.
 */

static const char *JSON_FIELD_NAME_Title = "title";
// static const char* JSON_FIELD_NAME_Tabs = "tabs";
static const char *JSON_FIELD_NAME_Conditions = "conditions";
static const char *JSON_FIELD_NAME_Action = "action";
static const char *JSON_FIELD_NAME_App = "app";
static const char *JSON_FIELD_NAME_AppArgs = "appArgs";
static const char *JSON_FIELD_NAME_AppCwd = "appCwd";

static QStringList stringListFromJArray(const QJsonArray &jArr)
{
    QStringList res;
    for (auto str : jArr)
        res.append(str.toString());
    return res;
}

static QVector<Condition> conditionsFromJArray(const QJsonArray &jArr)
{
    QVector<Condition> res;
    for (auto jObj : jArr)
        res.append(Condition(jObj.toObject()));
    return res;
}

Action &Action::operator=(const Action &other)
{
    this->m_title = other.m_title;
    this->m_conditions = other.m_conditions;
    this->m_internalActName = other.m_internalActName;
    this->m_externalApp = other.m_externalApp;
    this->m_externalAppParams = other.m_externalAppParams;
    this->m_externalAppCwd = other.m_externalAppCwd;

    return *this;
}

Action::Action(const QJsonObject &jObj)
    : m_title(jObj[JSON_FIELD_NAME_Title].toString())
    //    , m_tabsAllowed(stringListFromJArray(jObj[JSON_FIELD_NAME_Tabs].toArray()))
    , m_conditions(conditionsFromJArray(jObj[JSON_FIELD_NAME_Conditions].toArray()))
    , m_internalActName(jObj[JSON_FIELD_NAME_Action].toString())
    , m_externalApp(jObj[JSON_FIELD_NAME_App].toString())
    , m_externalAppParams(stringListFromJArray(jObj[JSON_FIELD_NAME_AppArgs].toArray()))
    , m_externalAppCwd(jObj[JSON_FIELD_NAME_AppCwd].toString())
{
}

QJsonObject Action::toJson() const
{
    QJsonArray res;
    for (const Condition &condition : m_conditions)
        res.append(condition.toJson());

    return {
        { JSON_FIELD_NAME_Title, m_title },
        //             { JSON_FIELD_NAME_Tabs, QJsonArray::fromStringList(m_tabsAllowed) },
        { JSON_FIELD_NAME_Conditions, res },
        { JSON_FIELD_NAME_Action, m_internalActName },
        { JSON_FIELD_NAME_App, m_externalApp },
        { JSON_FIELD_NAME_AppArgs, QJsonArray::fromStringList(m_externalAppParams) },
        { JSON_FIELD_NAME_AppCwd, m_externalAppCwd },
    };
}

bool Action::isAcceptable(ivm::AADLObject *obj) const
{
    for (const Condition &condition : m_conditions)
        if (!condition.isAcceptable(obj))
            return false;

    return true;
}

}
