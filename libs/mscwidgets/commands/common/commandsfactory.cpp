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
#include "commandsfactory.h"
#include "commandsstack.h"
#include "chartviewmodel.h"

#include "commands/cmdactionitemmove.h"
#include "commands/cmdactioninformaltext.h"

#include "commands/cmdconditionitemmove.h"

#include "commands/cmdentitynamechange.h"

#include "commands/cmdinstanceitemcreate.h"
#include "commands/cmdinstanceitemmove.h"
#include "commands/cmdinstancekindchange.h"

#include "commands/cmdmessageitemcreate.h"
#include "commands/cmdmessageitemmove.h"
#include "commands/cmdmessageitemresize.h"

#include "actionitem.h"
#include "conditionitem.h"
#include "instanceitem.h"
#include "messageitem.h"

#include <mscaction.h>
#include <mscchart.h>
#include <mscinstance.h>

#include <QDebug>
#include <QGraphicsItem>
#include <QPointF>
#include <QRectF>

namespace msc {
namespace cmd {

QUndoCommand *CommandsFactory::create(Id id, const QVariantList &params)
{
    switch (id) {
    case cmd::RenameEntity:
        return cmd::CommandsFactory::createRenameEntity(params);
    case cmd::MoveMessage:
        return cmd::CommandsFactory::createMessageItemMove(params);
    case cmd::RetargetMessage:
        return cmd::CommandsFactory::createMessageItemResize(params);
    case cmd::CreateMessage:
        return cmd::CommandsFactory::createMessageItemCreate(params);
    case cmd::MoveInstance:
        return cmd::CommandsFactory::createInstanceItemMove(params);
    case cmd::CreateInstance:
        return cmd::CommandsFactory::createInstanceItemCreate(params);
    case cmd::RenameInstanceKind:
        return cmd::CommandsFactory::createInstanceKindChange(params);
    case cmd::MoveCondition:
        return cmd::CommandsFactory::createConditionItemMove(params);
    case cmd::MoveAction:
        return cmd::CommandsFactory::createActionItemMove(params);
    case cmd::InformatActionText:
        return cmd::CommandsFactory::createActionInformalText(params);
    default:
        qWarning() << "CommandsStack::push - command ignored" << id;
        break;
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createRenameEntity(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (MscEntity *item = params.first().value<MscEntity *>()) {
        const QString &name = params.last().toString();
        if (item->name() != name)
            return new CmdEntityNameChange(item, name);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createMessageItemMove(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (MessageItem *item = params.first().value<MessageItem *>()) {
        const QPointF &destination = params.last().toPointF();
        if (item->pos() != destination)
            return new CmdMessageItemMove(item, destination);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createMessageItemResize(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);

    if (MessageItem *pItem = params.at(0).value<MessageItem *>()) {
        const QPointF &head(params.at(1).toPointF());
        const QPointF &tail(params.at(2).toPointF());
        return new CmdMessageItemResize(pItem, head, tail);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createMessageItemCreate(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    msc::MscMessage *message = params.at(0).value<msc::MscMessage *>();
    if (msc::MscChart *chart = params.at(1).value<msc::MscChart *>()) {
        return new CmdMessageItemCreate(message, chart);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createInstanceItemMove(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);

    if (MscInstance *item = params.first().value<MscInstance *>()) {
        const int &toIdx = params.at(1).toInt();
        if (msc::MscChart *chart = params.at(2).value<msc::MscChart *>()) {
            return new CmdInstanceItemMove(item, toIdx, chart);
        }
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createInstanceItemCreate(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    msc::MscInstance *message = params.at(0).value<msc::MscInstance *>();
    if (msc::MscChart *chart = params.at(1).value<msc::MscChart *>()) {
        return new CmdInstanceItemCreate(message, chart);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createInstanceKindChange(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (MscInstance *item = params.first().value<MscInstance *>()) {
        const QString &name = params.last().toString();
        if (item->kind() != name)
            return new CmdInstanceKindChange(item, name);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createConditionItemMove(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (ConditionItem *item = params.first().value<ConditionItem *>()) {
        const QPointF &destination = params.last().toPointF();
        if (item->pos() != destination)
            return new CmdConditionItemMove(item, destination);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createActionItemMove(const QVariantList &params)
{
    Q_ASSERT(params.size() == 4);

    if (msc::MscAction *item = params.at(0).value<msc::MscAction *>()) {
        int newPos = params.at(1).toInt();
        if (msc::MscInstance *newInstance = params.at(2).value<msc::MscInstance *>()) {
            if (msc::MscChart *chart = params.at(3).value<msc::MscChart *>()) {
                return new CmdActionItemMove(item, newPos, newInstance, chart);
            }
        }
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createActionInformalText(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (MscAction *item = params.first().value<MscAction *>()) {
        const QString &text = params.last().toString();
        if (item->informalAction() != text)
            return new CmdActionInformalText(item, text);
    }

    return nullptr;
}

} // ns cmd
} // ns msc
