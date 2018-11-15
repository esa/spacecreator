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

#pragma once

#include <QUndoCommand>

#include "common/commandids.h"

class QGraphicsItem;

namespace msc {
namespace cmd {

class BaseCommand : public QUndoCommand
{
public:
    explicit BaseCommand(msc::cmd::Id id, QGraphicsItem *item, QUndoCommand *parent = nullptr);
    explicit BaseCommand(msc::cmd::Id id, QGraphicsItem *item, const QString &text, QUndoCommand *parent = nullptr);

    virtual int id() const override;

protected:
    const msc::cmd::Id m_id;
    QGraphicsItem *m_graphicsItem;

    bool canMergeWith(const BaseCommand *cmd) const;
};

} // ns cmd
} // ns msc
