/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscinstance.h"

namespace msc {

MscCreate::MscCreate(QObject *parent)
    : MscCreate(QString(), parent) // rename from default one (which is MscEntity::DefaultName - "Untitled")
{
}

MscCreate::MscCreate(const QString &name, QObject *parent)
    : MscMessage(name, parent)
{
    m_msgType = MscMessage::MessageType::Create;
}

MscCreate::MscCreate(const QString &name, MscInstance *source, MscInstance *target, QObject *parent)
    : MscMessage(name, source, target, parent)
{
    m_msgType = MscMessage::MessageType::Create;
}

MscEntity::EntityType MscCreate::entityType() const
{
    return MscEntity::EntityType::Create;
}

} // namespace msc
