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

#include "aadlobject.h"

#include <QMetaType>
#include <QObject>
#include <QPointer>
#include <memory>

namespace taste3 {
namespace aadl {

struct AADLObjectIfacePrivate;
class AADLObjectIface : public AADLObject
{
    Q_OBJECT
    Q_PROPERTY(AADLObjectIface::IfaceType direction READ direction)

public:
    enum class IfaceType
    {
        Required = 0,
        Provided
    };
    Q_ENUM(IfaceType)

    static constexpr IfaceType DefaultDirection { IfaceType::Required };

    explicit AADLObjectIface(AADLObjectIface::IfaceType direction = DefaultDirection, const QString &title = QString(),
                             AADLObject *parent = nullptr);
    ~AADLObjectIface();

    AADLObject::AADLObjectType aadlType() const override;

    AADLObjectIface::IfaceType direction() const;

    bool isProvided() const;
    bool isRequired() const;

    AADLObject *holder() const;

private:
    const std::unique_ptr<AADLObjectIfacePrivate> d;
};

typedef QVector<AADLObjectIface *> AADLIfacesVector;

} // ns aadl
} // ns taste3

Q_DECLARE_METATYPE(taste3::aadl::AADLObjectIface::IfaceType);
