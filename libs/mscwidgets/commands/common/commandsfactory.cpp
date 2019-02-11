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

#include "commands/cmdactioninformaltext.h"
#include "commands/cmdactionitemcreate.h"
#include "commands/cmdactionitemmove.h"
#include "commands/cmdconditionitemcreate.h"
#include "commands/cmdconditionitemmove.h"
#include "commands/cmddeleteentity.h"
#include "commands/cmdentitynamechange.h"
#include "commands/cmdhierarchytypechange.h"
#include "commands/cmdinstanceitemcreate.h"
#include "commands/cmdinstanceitemmove.h"
#include "commands/cmdinstancekindchange.h"
#include "commands/cmdmessageitemcreate.h"
#include "commands/cmdmessageitemresize.h"
#include "commands/cmdtimeritemcreate.h"
#include "commands/cmdtimeritemmove.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "mscinstance.h"
#include "msctimer.h"

#include <QDebug>
#include <QPointF>
#include <QVector>

namespace msc {
namespace cmd {

QUndoCommand *CommandsFactory::create(Id id, const QVariantList &params)
{
    switch (id) {
    case cmd::RenameEntity:
        return cmd::CommandsFactory::createRenameEntity(params);
    case cmd::DeleteEntity:
        return cmd::CommandsFactory::createDeleteEntity(params);
    case cmd::RetargetMessage:
        return cmd::CommandsFactory::createMessageItemRetarget(params);
    case cmd::CreateMessage:
        return cmd::CommandsFactory::createMessageItemCreate(params);
    case cmd::MoveInstance:
        return cmd::CommandsFactory::createInstanceItemMove(params);
    case cmd::CreateInstance:
        return cmd::CommandsFactory::createInstanceItemCreate(params);
    case cmd::RenameInstanceKind:
        return cmd::CommandsFactory::createInstanceKindChange(params);
    case cmd::CreateCondition:
        return cmd::CommandsFactory::createConditionItemCreate(params);
    case cmd::MoveCondition:
        return cmd::CommandsFactory::createConditionItemMove(params);
    case cmd::CreateAction:
        return cmd::CommandsFactory::createActionItemCreate(params);
    case cmd::MoveAction:
        return cmd::CommandsFactory::createActionItemMove(params);
    case cmd::InformatActionText:
        return cmd::CommandsFactory::createActionInformalText(params);
    case cmd::CreateTimer:
        return cmd::CommandsFactory::createTimerItemCreate(params);
    case cmd::MoveTimer:
        return cmd::CommandsFactory::createTimerItemMove(params);
    case cmd::HierarchyType:
        return cmd::CommandsFactory::createHierarchyTypeChange(params);
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

QUndoCommand *CommandsFactory::createDeleteEntity(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    QVector<MscEntity *> items = params.at(0).value<QVector<MscEntity *>>();
    if (!items.isEmpty()) {
        if (msc::MscChart *chart = params.at(1).value<msc::MscChart *>()) {
            return new CmdDeleteEntity(items, chart);
        }
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createMessageItemRetarget(const QVariantList &params)
{
    Q_ASSERT(params.size() == 5);

    if (msc::MscMessage *item = params.at(0).value<msc::MscMessage *>()) {
        int newPos = params.at(1).toInt();
        msc::MscInstance *newInstance = params.at(2).value<msc::MscInstance *>();
        msc::MscMessage::EndType endtype = params.at(3).value<msc::MscMessage::EndType>();
        if (msc::MscChart *chart = params.at(4).value<msc::MscChart *>()) {
            return new CmdMessageItemResize(item, newPos, newInstance, endtype, chart);
        }
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

QUndoCommand *CommandsFactory::createConditionItemCreate(const QVariantList &params)
{
    Q_ASSERT(params.size() == 4);

    auto condition = params.at(0).value<msc::MscCondition *>();
    if (auto chart = params.at(1).value<msc::MscChart *>()) {
        auto instance = params.at(2).value<msc::MscInstance *>();
        int eventIndex = params.at(3).toInt();
        return new CmdConditionItemCreate(condition, chart, instance, eventIndex);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createConditionItemMove(const QVariantList &params)
{
    Q_ASSERT(params.size() == 4);

    if (msc::MscCondition *item = params.at(0).value<msc::MscCondition *>()) {
        int newPos = params.at(1).toInt();
        if (msc::MscInstance *newInstance = params.at(2).value<msc::MscInstance *>()) {
            if (msc::MscChart *chart = params.at(3).value<msc::MscChart *>()) {
                return new CmdConditionItemMove(item, newPos, newInstance, chart);
            }
        }
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createActionItemCreate(const QVariantList &params)
{
    Q_ASSERT(params.size() == 4);

    auto action = params.at(0).value<msc::MscAction *>();
    if (auto chart = params.at(1).value<msc::MscChart *>()) {
        auto instance = params.at(2).value<msc::MscInstance *>();
        int eventIndex = params.at(3).toInt();
        return new CmdActionItemCreate(action, chart, instance, eventIndex);
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

QUndoCommand *CommandsFactory::createTimerItemCreate(const QVariantList &params)
{
    Q_ASSERT(params.size() == 5);

    auto timer = params.at(0).value<msc::MscTimer *>();
    auto timerType = params.at(1).value<msc::MscTimer::TimerType>();
    if (auto chart = params.at(2).value<msc::MscChart *>()) {
        auto instance = params.at(3).value<msc::MscInstance *>();
        int eventIndex = params.at(4).toInt();
        return new CmdTimerItemCreate(timer, timerType, chart, instance, eventIndex);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createTimerItemMove(const QVariantList &params)
{
    Q_ASSERT(params.size() == 4);

    if (msc::MscTimer *item = params.at(0).value<msc::MscTimer *>()) {
        int newPos = params.at(1).toInt();
        if (msc::MscInstance *newInstance = params.at(2).value<msc::MscInstance *>()) {
            if (msc::MscChart *chart = params.at(3).value<msc::MscChart *>()) {
                return new CmdTimerItemMove(item, newPos, newInstance, chart);
            }
        }
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createHierarchyTypeChange(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (MscDocument *item = params.first().value<MscDocument *>()) {
        const MscDocument::HierarchyType type = static_cast<MscDocument::HierarchyType>(params.last().toInt());
        if (item->hierarchyType() != type)
            return new CmdHierarchyTypeChange(item, type);
    }

    return nullptr;
}

} // ns cmd
} // ns msc
