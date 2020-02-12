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

#include <QObject>
#include <memory>

namespace taste3 {
namespace aadl {

class AADLObjectIface;
struct AADLObjectConnectionPrivate;
class AADLObjectConnection : public AADLObject
{
    Q_OBJECT
public:
    explicit AADLObjectConnection(AADLObject *source, AADLObject *target, AADLObjectIface *ifaceSource,
                                  AADLObjectIface *ifaceTarget, QObject *parent = nullptr);
    ~AADLObjectConnection() override;

    AADLObjectType aadlType() const override;

    AADLObject *source() const;
    void setSource(AADLObject *source);

    AADLObject *target() const;
    void setTarget(AADLObject *target);

    QString sourceInterfaceName() const;
    AADLObjectIface *sourceInterface() const;
    void setSourceInterface(AADLObjectIface *iface);

    QString targetInterfaceName() const;
    AADLObjectIface *targetInterface() const;
    void setTargetInterface(AADLObjectIface *iface);

    void setAttr(const QString &name, const QVariant &val) override;

    enum class LabelInheritancePolicy
    {
        Set = 0,
        Unset
    };
    void inheritLabel();
    void uninheritLabel();

private:
    const std::unique_ptr<AADLObjectConnectionPrivate> d;
    void updateAttributes();
    void handleLabelInheritance(AADLObjectConnection::LabelInheritancePolicy inheritance);
};

} // ns aadl
} // ns taste3
