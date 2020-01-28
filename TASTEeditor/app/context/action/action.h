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

#pragma once
#include "condition.h"

#include <QJsonObject>
#include <QVariant>

namespace taste3 {
namespace aadl {
class AADLObject;
}
}

namespace taste3 {
namespace ctx {

struct Action {
    Action(const QJsonObject &jObj = QJsonObject());
    QJsonObject toJson() const;

    QString m_title;
    //    QStringList m_tabsAllowed;
    QVector<Condition> m_conditions;

    QString m_internalActName;

    QString m_externalApp;
    QStringList m_externalAppParams;
    QString m_externalAppCwd;

    Action &operator=(const Action &other);

    bool isAcceptable(aadl::AADLObject *obj) const;
};

} // ctx
} // ns taste3
