/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvpropertytemplate.h"

#include "dvobject.h"

#include <QDomElement>
#include <QMetaEnum>
#include <QRegularExpressionMatch>

namespace dvm {

DVPropertyTemplate::DVPropertyTemplate() { }

QMetaEnum DVPropertyTemplate::scopeMetaEnum() const
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    static const QMetaEnum scopeMeta = QMetaEnum::fromType<DVPropertyTemplate::Scopes>();
#else
    static const QMetaEnum scopeMeta = QMetaEnum::fromType<DVPropertyTemplate::Scope>();
#endif
    return scopeMeta;
}

int DVPropertyTemplate::objectScope(const shared::VEObject *obj) const
{
    DVPropertyTemplate::Scope scope = DVPropertyTemplate::Scope::None;
    if (auto dvObj = obj->as<const DVObject *>()) {
        switch (dvObj->type()) {
        case DVObject::Type::Connection:
            scope = DVPropertyTemplate::Scope::Connection;
            break;
        case DVObject::Type::Node:
            scope = DVPropertyTemplate::Scope::Node;
            break;
        case DVObject::Type::Partition:
            scope = DVPropertyTemplate::Scope::Partition;
            break;
        case DVObject::Type::Device:
            scope = DVPropertyTemplate::Scope::Device;
            break;
        default:
            break;
        }
    }
    return static_cast<int>(scope);
};

} // namespace ivm
