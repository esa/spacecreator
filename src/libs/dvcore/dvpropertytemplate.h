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

#pragma once

#include "propertytemplate.h"

namespace dvm {

class DVPropertyTemplate : public shared::PropertyTemplate
{
    Q_GADGET
public:
    enum class Scope
    {
        None = 0x0,
        Node = 0x1,
        Partition = 0x2,
        Device = 0x4,
        Connection = 0x8,
    };

    Q_ENUM(Scope)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    Q_DECLARE_FLAGS(Scopes, Scope)
#else
    Q_DECLARE_FLAGS(Scopes, dvm::DVPropertyTemplate::Scope)
#endif
    Q_FLAG(Scopes)

    DVPropertyTemplate();
    ~DVPropertyTemplate() override {};

protected:
    QMetaEnum scopeMetaEnum() const override;
    int objectScope(const shared::VEObject *obj) const override;
};

} // namespace dvm

Q_DECLARE_OPERATORS_FOR_FLAGS(dvm::DVPropertyTemplate::Scopes)
Q_DECLARE_METATYPE(dvm::DVPropertyTemplate::Type)
Q_DECLARE_METATYPE(dvm::DVPropertyTemplate::Scopes)
