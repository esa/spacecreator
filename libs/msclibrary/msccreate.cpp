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

#include "msccreate.h"

namespace msc {

MscCreate::MscCreate(QObject *parent)
    : MscInstanceEvent(parent)
{
}

MscCreate::MscCreate(const QString &name, QObject *parent)
    : MscInstanceEvent(name, parent)
{
}

const QStringList &MscCreate::parameters() const
{
    return m_parameters;
}

void MscCreate::addParameter(const QString &parameter)
{
    m_parameters.append(parameter);
}

QString MscCreate::messageName() const
{
    return m_messageName;
}

void MscCreate::setMessageName(const QString &messageName)
{
    m_messageName = messageName;
}

MscInstance *MscCreate::instance() const
{
    return m_instance;
}

void MscCreate::setInstance(MscInstance *instance)
{
    m_instance = instance;
}

MscEntity::EntityType MscCreate::entityType() const
{
    return MscEntity::EntityType::Create;
}

} // namespace msc
