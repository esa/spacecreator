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

#include "commandids.h"

#include <QVariant>

class QGraphicsItem;
class QPointF;
class QRectF;
class QUndoCommand;

namespace msc {
namespace cmd {

class CommandsFactory
{
public:
    static QUndoCommand *create(msc::cmd::Id id, const QVariantList &params);

    static QUndoCommand *createRenameEntity(const QVariantList &params);
    static QUndoCommand *createMessageItemMove(const QVariantList &params);
    static QUndoCommand *createMessageItemResize(const QVariantList &params);
    static QUndoCommand *createMessageItemCreate(const QVariantList &params);
    static QUndoCommand *createInstanceItemMove(const QVariantList &params);
    static QUndoCommand *createInstanceItemResize(const QVariantList &params);
    static QUndoCommand *createInstanceItemCreate(const QVariantList &params);
    static QUndoCommand *createConditionItemMove(const QVariantList &params);
    static QUndoCommand *createConditionItemResize(const QVariantList &params);
    static QUndoCommand *createActionItemMove(const QVariantList &params);

private:
    CommandsFactory() = delete;
    CommandsFactory(const CommandsFactory &other) = delete;
};

} // ns cmd
} // ns msc
