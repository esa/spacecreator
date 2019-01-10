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

#include "commands/basecommand.h"
#include "chartviewmodel.h"
#include "messageitem.h"
#include <mscmessage.h>

#include <QPointer>
#include <QPointF>

#include <QGraphicsScene>

namespace msc {
namespace cmd {

class CmdMessageItemCreate : public BaseCommand
{
public:
    CmdMessageItemCreate(QGraphicsScene *scene, ChartViewModel *model, const QPointF &pos);

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<QGraphicsScene> m_scene;
    QPointer<ChartViewModel> m_model;
    QPointer<MessageItem> m_messageItem;
    QPointF m_pos;

    bool validateStorages(const char *caller) const;
};

} // ns cmd
} // ns msc
