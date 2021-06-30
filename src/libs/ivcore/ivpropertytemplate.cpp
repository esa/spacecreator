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

#include "ivpropertytemplate.h"

#include "ivobject.h"

#include <QDomElement>
#include <QMetaEnum>
#include <QRegularExpressionMatch>

namespace ivm {

IVPropertyTemplate::IVPropertyTemplate() { }

QMetaEnum IVPropertyTemplate::scopeMetaEnum() const
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    static const QMetaEnum scopeMeta = QMetaEnum::fromType<IVPropertyTemplate::Scopes>();
#else
    static const QMetaEnum scopeMeta = QMetaEnum::fromType<IVPropertyTemplate::Scope>();
#endif
    return scopeMeta;
}

int IVPropertyTemplate::objectScope(const shared::VEObject *obj) const
{
    IVPropertyTemplate::Scope scope = IVPropertyTemplate::Scope::None;
    if (auto ivObj = obj->as<const IVObject *>()) {
        switch (ivObj->type()) {
        case ivm::IVObject::Type::Connection:
            scope = IVPropertyTemplate::Scope::Connection;
            break;
        case ivm::IVObject::Type::Comment:
            scope = IVPropertyTemplate::Scope::Comment;
            break;
        case ivm::IVObject::Type::FunctionType:
        case ivm::IVObject::Type::Function:
            scope = IVPropertyTemplate::Scope::Function;
            break;
        case ivm::IVObject::Type::RequiredInterface:
            scope = IVPropertyTemplate::Scope::Required_Interface;
            break;
        case ivm::IVObject::Type::ProvidedInterface:
            scope = IVPropertyTemplate::Scope::Provided_Interface;
            break;
        default:
            break;
        }
    }
    return static_cast<int>(scope);
};

} // namespace ivm
