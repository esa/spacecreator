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
#include "basecommand.h"

namespace msc {
namespace cmd {

BaseCommand::BaseCommand(msc::cmd::Id id, QGraphicsItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_id(id)
    , m_graphicsItem(item)
{
}

BaseCommand::BaseCommand(msc::cmd::Id id, QGraphicsItem *item, const QString &text, QUndoCommand *parent)
    : QUndoCommand(text, parent)
    , m_id(id)
    , m_graphicsItem(item)
{
}

int BaseCommand::id() const
{
    return m_id;
}

bool BaseCommand::canMergeWith(const BaseCommand *cmd) const
{
    return cmd && cmd->id() == id() && cmd->m_graphicsItem == m_graphicsItem;
}

} // ns cmd
} // ns msc
