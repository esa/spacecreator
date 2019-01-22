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

#include "common/commandids.h"

#include <QPointer>
#include <QUndoCommand>

class QGraphicsObject;

namespace msc {

class MscEntity;

namespace cmd {

class BaseCommand : public QObject, public QUndoCommand
{
public:
    BaseCommand();
    explicit BaseCommand(QGraphicsObject *item, QUndoCommand *parent = nullptr);
    explicit BaseCommand(MscEntity *item, QUndoCommand *parent = nullptr);
    explicit BaseCommand(QGraphicsObject *item, const QString &text, QUndoCommand *parent = nullptr);

protected:
    QPointer<QGraphicsObject> m_graphicsItem;
    QPointer<MscEntity> m_modelItem;

    virtual bool canMergeWith(const BaseCommand *cmd) const;
};

} // ns cmd
} // ns msc
