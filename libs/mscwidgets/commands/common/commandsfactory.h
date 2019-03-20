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

#pragma once

#include "commandids.h"

#include <QVariantList>

class QUndoCommand;

namespace msc {
namespace cmd {

class CommandsFactory
{
public:
    static QUndoCommand *create(msc::cmd::Id id, const QVariantList &params);

    static QUndoCommand *createChangeComment(const QVariantList &params);
    static QUndoCommand *createRenameEntity(const QVariantList &params);
    static QUndoCommand *createDeleteEntity(const QVariantList &params);
    static QUndoCommand *createMessageItemRetarget(const QVariantList &params);
    static QUndoCommand *createMessageItemCreate(const QVariantList &params);
    static QUndoCommand *createInstanceItemMove(const QVariantList &params);
    static QUndoCommand *createInstanceItemCreate(const QVariantList &params);
    static QUndoCommand *createInstanceKindChange(const QVariantList &params);
    static QUndoCommand *createInstanceStopChange(const QVariantList &params);
    static QUndoCommand *createConditionItemCreate(const QVariantList &params);
    static QUndoCommand *createConditionItemMove(const QVariantList &params);
    static QUndoCommand *createActionItemCreate(const QVariantList &params);
    static QUndoCommand *createActionItemMove(const QVariantList &params);
    static QUndoCommand *createActionInformalText(const QVariantList &params);
    static QUndoCommand *createTimerItemCreate(const QVariantList &params);
    static QUndoCommand *createTimerItemMove(const QVariantList &params);
    static QUndoCommand *createHierarchyTypeChange(const QVariantList &params);
    static QUndoCommand *createDocumentCreate(const QVariantList &params);

private:
    CommandsFactory() = delete;
    CommandsFactory(const CommandsFactory &other) = delete;
};

} // ns cmd
} // ns msc
