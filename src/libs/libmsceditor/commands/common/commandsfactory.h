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

#include <QPointer>
#include <QVariantList>

class QUndoCommand;

namespace msc {

class ChartLayoutManager;

namespace cmd {

class CommandsFactory
{
public:
    CommandsFactory();

    QUndoCommand *create(msc::cmd::Id id, const QVariantList &params);

    void setChartLayoutManager(msc::ChartLayoutManager *layoutManager);

    QUndoCommand *createChangeComment(const QVariantList &params);
    QUndoCommand *createRenameEntity(const QVariantList &params);
    QUndoCommand *createDeleteEntity(const QVariantList &params);
    QUndoCommand *createMessageItemRetarget(const QVariantList &params);
    QUndoCommand *createMessageItemCreate(const QVariantList &params);
    QUndoCommand *createSetParameterList(const QVariantList &params);
    QUndoCommand *createChangeInstanceOrder(const QVariantList &params);
    QUndoCommand *createInstanceItemCreate(const QVariantList &params);
    QUndoCommand *createInstanceKindChange(const QVariantList &params);
    QUndoCommand *createInstanceStopChange(const QVariantList &params);
    QUndoCommand *createConditionItemCreate(const QVariantList &params);
    QUndoCommand *createConditionItemMove(const QVariantList &params);
    QUndoCommand *createActionItemCreate(const QVariantList &params);
    QUndoCommand *createCoregionItemCreate(const QVariantList &params);
    QUndoCommand *createCoregionMove(const QVariantList &params);
    QUndoCommand *createActionItemMove(const QVariantList &params);
    QUndoCommand *createActionInformalText(const QVariantList &params);
    QUndoCommand *createTimerItemCreate(const QVariantList &params);
    QUndoCommand *createTimerItemMove(const QVariantList &params);
    QUndoCommand *createHierarchyTypeChange(const QVariantList &params);
    QUndoCommand *createDocumentCreate(const QVariantList &params);
    QUndoCommand *createChartGeometryChange(const QVariantList &params);
    QUndoCommand *createCommentGeometryChange(const QVariantList &params);
    QUndoCommand *createDocumentMove(const QVariantList &params);
    QUndoCommand *createSetMessageDeclarations(const QVariantList &params);
    QUndoCommand *createChartPaste(const QVariantList &params);
    QUndoCommand *createAsn1File(const QVariantList &params);
    QUndoCommand *createChangeInstancePosition(const QVariantList &params);
    QUndoCommand *createEditMessagePoints(const QVariantList &params);

private:
    CommandsFactory(const CommandsFactory &other) = delete;

    QPointer<msc::ChartLayoutManager> m_layoutManager;
};

} // ns cmd
} // ns msc
