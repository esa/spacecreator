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

#include "aadlobjectifacegroup.h"

namespace aadl {

AADLObjectIfaceGroup::AADLObjectIfaceGroup(const CreationInfo &ci)
    : AADLObjectIface(AADLObject::Type::InterfaceGroup, ci)
{
}

void AADLObjectIfaceGroup::removeEntity(AADLObjectIface *iface)
{
    m_entities.removeAll(iface);
    iface->setGroupName(QString());
}

void AADLObjectIfaceGroup::addEntity(AADLObjectIface *iface)
{
    iface->setGroupName(groupName());
    if (!m_entities.contains(iface))
        m_entities.append(iface);
}

void AADLObjectIfaceGroup::setAttr(const QString &name, const QVariant &val)
{
    if (name.isEmpty()) {
        return;
    }

    const meta::Props::Token t = meta::Props::token(name);
    if (t != meta::Props::Token::name) {
        AADLObjectIface::setAttr(name, val);
    }
}

QList<QPointer<AADLObjectIface>> AADLObjectIfaceGroup::entities() const
{
    return m_entities;
}

QString AADLObjectIfaceGroup::ifaceLabel() const
{
    return {};
}

} // namespace aadl
