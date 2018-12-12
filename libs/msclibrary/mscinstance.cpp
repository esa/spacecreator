/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscinstance.h"

namespace msc {

MscInstance::MscInstance(QObject *parent)
    : MscEntity(parent)
{
}

MscInstance::MscInstance(const QString &name, QObject *parent)
    : MscEntity(name, parent)
{
}

const QString &MscInstance::kind() const
{
    return m_kind;
}

void MscInstance::setKind(const QString &kind)
{
    if (kind == m_kind) {
        return;
    }

    m_kind = kind;
    Q_EMIT kindChanged(m_kind);
}

const QString &MscInstance::inheritance() const
{
    return m_inheritance;
}

void MscInstance::setInheritance(const QString &inheritance)
{
    if (inheritance == m_inheritance) {
        return;
    }

    m_inheritance = inheritance;
    Q_EMIT inheritanceChanged(m_inheritance);
}

MscEntity::EntityType MscInstance::entityType() const
{
    return MscEntity::EntityType::Instance;
}

} // namespace msc
