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
#include "commandsfactory.h"

#include "commands/cmdactioninformaltext.h"
#include "commands/cmdactionitemcreate.h"
#include "commands/cmdactionitemmove.h"
#include "commands/cmdchartitemchangegeometry.h"
#include "commands/cmdcommentitemchangegeometry.h"
#include "commands/cmdconditionitemcreate.h"
#include "commands/cmdconditionitemmove.h"
#include "commands/cmdcoregionitemcreate.h"
#include "commands/cmddeleteentity.h"
#include "commands/cmddocumentcreate.h"
#include "commands/cmddocumentmove.h"
#include "commands/cmdentitycommentchange.h"
#include "commands/cmdentitynamechange.h"
#include "commands/cmdhierarchytypechange.h"
#include "commands/cmdinstanceitemcreate.h"
#include "commands/cmdinstanceitemmove.h"
#include "commands/cmdinstancekindchange.h"
#include "commands/cmdinstancestopchange.h"
#include "commands/cmdmessageitemcreate.h"
#include "commands/cmdmessageitemresize.h"
#include "commands/cmdpastechart.h"
#include "commands/cmdsetasn1file.h"
#include "commands/cmdsetmessagedeclarations.h"
#include "commands/cmdsetparameterlist.h"
#include "commands/cmdtimeritemcreate.h"
#include "commands/cmdtimeritemmove.h"
#include "messageitem.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccomment.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "mscinstance.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "msctimer.h"

#include <QDebug>
#include <QPointF>
#include <QVector>

namespace msc {
namespace cmd {

QUndoCommand *CommandsFactory::create(Id id, const QVariantList &params)
{
    switch (id) {
    case cmd::ChangeComment:
        return cmd::CommandsFactory::createChangeComment(params);
    case cmd::RenameEntity:
        return cmd::CommandsFactory::createRenameEntity(params);
    case cmd::DeleteEntity:
        return cmd::CommandsFactory::createDeleteEntity(params);
    case cmd::RetargetMessage:
        return cmd::CommandsFactory::createMessageItemRetarget(params);
    case cmd::CreateMessage:
        return cmd::CommandsFactory::createMessageItemCreate(params);
    case cmd::SetParameterList:
        return cmd::CommandsFactory::createSetParameterList(params);
    case cmd::MoveInstance:
        return cmd::CommandsFactory::createInstanceItemMove(params);
    case cmd::CreateInstance:
        return cmd::CommandsFactory::createInstanceItemCreate(params);
    case cmd::RenameInstanceKind:
        return cmd::CommandsFactory::createInstanceKindChange(params);
    case cmd::StopInstance:
        return cmd::CommandsFactory::createInstanceStopChange(params);
    case cmd::CreateCondition:
        return cmd::CommandsFactory::createConditionItemCreate(params);
    case cmd::MoveCondition:
        return cmd::CommandsFactory::createConditionItemMove(params);
    case cmd::CreateAction:
        return cmd::CommandsFactory::createActionItemCreate(params);
    case cmd::CreateCoregion:
        return cmd::CommandsFactory::createCoregionItemCreate(params);
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
    case cmd::CreateDocument:
        return cmd::CommandsFactory::createDocumentCreate(params);
    case cmd::ChangeChartGeometry:
        return cmd::CommandsFactory::createChartGeometryChange(params);
    case cmd::ChangeCommentGeometry:
        return cmd::CommandsFactory::createCommentGeometryChange(params);
    case cmd::MoveDocument:
        return cmd::CommandsFactory::createDocumentMove(params);
    case cmd::SetMessageDeclarations:
        return cmd::CommandsFactory::createSetMessageDeclarations(params);
    case cmd::PasteChart:
        return cmd::CommandsFactory::createChartPaste(params);
    case cmd::SetAsn1File:
        return cmd::CommandsFactory::createAsn1File(params);
    default:
        qWarning() << "CommandsStack::push - command ignored" << id;
        break;
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createChangeComment(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);

    MscChart *chart = params.first().value<MscChart *>();
    if (!chart)
        return nullptr;

    if (MscEntity *item = params.at(1).value<MscEntity *>()) {
        const QString comment = params.last().toString();
        if (!item->comment() && comment.isEmpty())
            return nullptr;

        if (item->comment() && item->comment()->comment() == comment)
            return nullptr;

        return new CmdEntityCommentChange(chart, item, comment);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createRenameEntity(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (MscEntity *item = params.first().value<MscEntity *>()) {
        const QString &name = params.last().toString();
        if (item->name() != name && !name.isEmpty())
            return new CmdEntityNameChange(item, name);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createDeleteEntity(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);

    QVector<MscEntity *> items = params.at(0).value<QVector<MscEntity *>>();
    if (!items.isEmpty()) {
        return new CmdDeleteEntity(items, params.at(1).value<msc::MscChart *>(),
                                   params.at(2).value<msc::MscDocument *>());
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
    Q_ASSERT(params.size() >= 3);

    msc::MscMessage *message = params.at(0).value<msc::MscMessage *>();
    if (msc::MscChart *chart = params.at(1).value<msc::MscChart *>()) {
        const QVector<QPoint> &points = params.size() == 4 ? params.at(3).value<QVector<QPoint>>() : QVector<QPoint>();
        return new CmdMessageItemCreate(message, chart, params.at(2).toInt(), points);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createSetParameterList(const QVariantList &params)
{
    Q_ASSERT(params.size() >= 2);

    msc::MscEntity *entity = params.at(0).value<msc::MscEntity *>();
    msc::MscParameterList parameters = params.at(1).value<msc::MscParameterList>();
    return new CmdSetParameterList(entity, parameters);
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
    Q_ASSERT(params.size() == 3);

    msc::MscInstance *message = params.at(0).value<msc::MscInstance *>();
    if (msc::MscChart *chart = params.at(1).value<msc::MscChart *>()) {
        const int pos = params.at(2).toInt();
        return new CmdInstanceItemCreate(message, chart, pos);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createInstanceKindChange(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (MscInstance *item = params.first().value<MscInstance *>()) {
        const QString &name = params.last().toString();
        if (item->denominatorAndKind() != name)
            return new CmdInstanceKindChange(item, name);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createInstanceStopChange(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (MscInstance *item = params.first().value<MscInstance *>()) {
        const bool newValue = params.last().toBool();
        return new CmdInstanceStopChange(item, newValue);
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

QUndoCommand *CommandsFactory::createCoregionItemCreate(const QVariantList &params)
{
    Q_ASSERT(params.size() == 5);

    auto begin = params.value(0).value<msc::MscCoregion *>();
    auto end = params.value(1).value<msc::MscCoregion *>();
    if (auto chart = params.value(2).value<msc::MscChart *>()) {
        auto instance = params.value(3).value<msc::MscInstance *>();
        bool ok;
        int eventIndex = params.value(4).toInt(&ok);
        return new CmdCoregionItemCreate(begin, end, chart, instance, ok ? eventIndex : -1);
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

QUndoCommand *CommandsFactory::createDocumentCreate(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (auto document = params.first().value<MscDocument *>()) {
        if (auto parent = params.last().value<MscDocument *>()) {
            return new CmdDocumentCreate(document, parent);
        }
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createChartGeometryChange(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);

    if (auto chart = params.last().value<MscChart *>())
        return new CmdChartItemChangeGeometry(params.value(0).toRectF(), params.value(1).toRectF(), chart);

    return nullptr;
}

QUndoCommand *CommandsFactory::createCommentGeometryChange(const QVariantList &params)
{
    Q_ASSERT(params.size() == 4);

    MscChart *chart = params.first().value<MscChart *>();
    if (!chart)
        return nullptr;

    if (auto entity = params.last().value<MscEntity *>()) {
        const QRect &oldRect = params.value(1).toRect();
        const QRect &newRect = params.value(2).toRect();
        if (newRect == oldRect)
            return nullptr;

        if (entity->comment() && entity->comment()->rect() == newRect)
            return nullptr;

        return new CmdCommentItemChangeGeometry(chart, oldRect, newRect, entity);
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createDocumentMove(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (auto document = params.first().value<MscDocument *>()) {
        if (auto parent = params.last().value<MscDocument *>()) {
            return new CmdDocumentMove(document, parent);
        }
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createSetMessageDeclarations(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (auto document = params.first().value<MscDocument *>()) {
        if (auto declarations = params.last().value<MscMessageDeclarationList *>()) {
            return new CmdSetMessageDeclarations(document, declarations);
        }
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createChartPaste(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);

    if (auto document = params.first().value<MscDocument *>()) {
        return new CmdPasteChart(document, params.last().toString());
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createAsn1File(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);

    if (auto model = params.first().value<MscModel *>()) {
        return new CmdSetAsn1File(model, params.at(1).toString(), params.at(2).toString());
    }

    return nullptr;
}

} // ns cmd
} // ns msc
