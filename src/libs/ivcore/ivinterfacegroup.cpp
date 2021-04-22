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

#include "ivinterfacegroup.h"

namespace ivm {

IVInterfaceGroup::IVInterfaceGroup(const CreationInfo &ci)
    : IVInterface(IVObject::Type::InterfaceGroup, ci)
{
}

void IVInterfaceGroup::removeEntity(IVInterface *iface)
{
    m_entities.removeAll(iface);
    iface->setGroupName(QString());
}

void IVInterfaceGroup::addEntity(IVInterface *iface)
{
    iface->setGroupName(groupName());
    if (!m_entities.contains(iface))
        m_entities.append(iface);
}

void IVInterfaceGroup::setAttributeImpl(const QString &name, const QVariant &value, EntityAttribute::Type type)
{
    if (name.isEmpty()) {
        return;
    }

    const meta::Props::Token t = meta::Props::token(name);
    if (t != meta::Props::Token::name) {
        IVInterface::setAttributeImpl(name, value, type);
    }
}

QList<QPointer<IVInterface>> IVInterfaceGroup::entities() const
{
    return m_entities;
}

QString IVInterfaceGroup::ifaceLabel() const
{
    return {};
}

} // namespace ivm
