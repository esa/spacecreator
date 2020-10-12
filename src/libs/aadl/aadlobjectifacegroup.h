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

#include "aadlobjectiface.h"

namespace aadl {

class AADLObjectIfaceGroup : public AADLObjectIface
{
    Q_OBJECT
public:
    explicit AADLObjectIfaceGroup(const CreationInfo &ci);

    void addEntity(AADLObjectIface *iface);
    QList<QPointer<AADLObjectIface>> entities() const;

    QString ifaceLabel() const override;

    void setAttr(const QString &name, const QVariant &val) override;

private:
    QList<QPointer<AADLObjectIface>> m_entities;
};

} // namespace aadl
