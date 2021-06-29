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

#pragma once

#include "propertytemplate.h"

#include <QDomElement>
#include <QMetaEnum>
#include <QObject>
#include <QVariant>
#include <QVector>
#include <memory>

class QValidator;
class QDomDocument;

namespace ivm {

class IVPropertyTemplate : public shared::PropertyTemplate
{
    Q_GADGET
public:
    enum class Scope
    {
        None = 0x0,
        Function = 0x1,
        Required_Interface = 0x2,
        Provided_Interface = 0x4,
        Comment = 0x8,
        Connection = 0x10,
    };

    Q_ENUM(Scope)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    Q_DECLARE_FLAGS(Scopes, Scope)
#else
    Q_DECLARE_FLAGS(Scopes, ivm::IVPropertyTemplate::Scope)
#endif
    Q_FLAG(Scopes)

    IVPropertyTemplate();
    ~IVPropertyTemplate() override {};

protected:
    QMetaEnum scopeMetaEnum() const override;
    int objectScope(const shared::VEObject *obj) const override;
};

} // namespace ivm

Q_DECLARE_OPERATORS_FOR_FLAGS(ivm::IVPropertyTemplate::Scopes)
Q_DECLARE_METATYPE(ivm::IVPropertyTemplate::Type)
Q_DECLARE_METATYPE(ivm::IVPropertyTemplate::Scopes)
