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

#include "cmdcommentitemcreate.h"
#include "cmdcommenttextchange.h"
#include "cmdcontaineritemcreate.h"
#include "cmddirectconnectionitemcreate.h"
#include "cmdentityattributechange.h"
#include "cmdentitygeometrychange.h"
#include "cmdentitypropertychange.h"
#include "cmdentityremove.h"
#include "cmdfunctionitemcreate.h"
#include "cmdmanualconnectionitemcreate.h"
#include "cmdprovidedinterfaceitemcreate.h"
#include "cmdrequiredinterfaceitemcreate.h"
#include "commandids.h"

#include <QRect>
#include <QVariant>
#include <QtDebug>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {
namespace cmd {

QUndoCommand *CommandsFactory::create(Id id, const QVariantList &params)
{
    switch (id) {
    case cmd::CreateContainerEntity:
        return cmd::CommandsFactory::createContainerCommand(params);
    case cmd::CreateFunctionEntity:
        return cmd::CommandsFactory::createFunctionCommand(params);
    case cmd::CreateCommentEntity:
        return cmd::CommandsFactory::createCommentCommand(params);
    case cmd::CreateProvidedInterfaceEntity:
        return cmd::CommandsFactory::createProvidedInterfaceCommand(params);
    case cmd::CreateRequiredInterfaceEntity:
        return cmd::CommandsFactory::createRequiredInterfaceCommand(params);
    case cmd::CreateManualConnectionEntity:
        return cmd::CommandsFactory::createManualConnectionCommand(params);
    case cmd::CreateDirectConnectionEntity:
        return cmd::CommandsFactory::createDirectConnectionCommand(params);
    case cmd::ChangeEntityGeometry:
        return cmd::CommandsFactory::changeGeometryCommand(params);
    case cmd::ChangeCommentText:
        return cmd::CommandsFactory::changeCommentCommand(params);
    case cmd::RemoveEntity:
        return cmd::CommandsFactory::removeEntityCommand(params);
    case cmd::ChangeEntityProperties:
        return cmd::CommandsFactory::changeEntityPropertyCommand(params);
    case cmd::ChangeEntityAttributes:
        return cmd::CommandsFactory::changeEntityAttributesCommand(params);

    default:
        qWarning() << "CommandsStack::push - command ignored" << id;
        break;
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createFunctionCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant model = params.value(0);
    const QVariant parent = params.value(1);
    const QVariant geometry = params.value(2);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid() && model.canConvert<AADLObjectsModel *>()
        && parent.canConvert<AADLObjectContainer *>())
        return new CmdFunctionItemCreate(model.value<AADLObjectsModel *>(), parent.value<AADLObjectContainer *>(),
                                         geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createContainerCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant model = params.value(0);
    const QVariant parent = params.value(1);
    const QVariant geometry = params.value(2);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid() && model.canConvert<AADLObjectsModel *>()
        && parent.canConvert<AADLObjectContainer *>())
        return new CmdContainerItemCreate(model.value<AADLObjectsModel *>(), parent.value<AADLObjectContainer *>(),
                                          geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createCommentCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant model = params.value(0);
    const QVariant geometry = params.value(1);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>())
        return new CmdCommentItemCreate(model.value<AADLObjectsModel *>(), geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createProvidedInterfaceCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant model = params.value(0);
    const QVariant parent = params.value(1);
    const QVariant position = params.value(2);
    if (position.isValid() && position.canConvert<QPointF>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>() && parent.canConvert<AADLObjectContainer *>())
        return new CmdProvidedInterfaceItemCreate(model.value<AADLObjectsModel *>(),
                                                  parent.value<AADLObjectContainer *>(), position.value<QPointF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createRequiredInterfaceCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant model = params.value(0);
    const QVariant parent = params.value(1);
    const QVariant position = params.value(2);
    if (position.isValid() && position.canConvert<QPointF>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>() && parent.canConvert<AADLObjectContainer *>())
        return new CmdRequiredInterfaceItemCreate(model.value<AADLObjectsModel *>(),
                                                  parent.value<AADLObjectContainer *>(), position.value<QPointF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createManualConnectionCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 6);
    const QVariant model = params.value(0);
    const QVariant start = params.value(1);
    const QVariant end = params.value(2);
    const QVariant pi = params.value(3);
    const QVariant ri = params.value(4);
    const QVariant points = params.value(5);
    if (points.isValid() && points.canConvert<QVector<QPointF>>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>() && start.isValid() && start.canConvert<AADLObjectContainer *>()
        && end.isValid() && end.canConvert<AADLObjectContainer *>() && pi.isValid()
        && pi.canConvert<AADLObjectIfaceProvided *>() && ri.isValid() && ri.canConvert<AADLObjectIfaceRequired *>()) {
        return new CmdManualConnectionItemCreate(
                model.value<AADLObjectsModel *>(), start.value<AADLObjectContainer *>(),
                end.value<AADLObjectContainer *>(), pi.value<AADLObjectIfaceProvided *>(),
                ri.value<AADLObjectIfaceRequired *>(), points.value<QVector<QPointF>>());
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createDirectConnectionCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 7);
    const QVariant model = params.value(0);
    const QVariant start = params.value(1);
    const QVariant prov = params.value(2);
    const QVariant startPoint = params.value(3);
    const QVariant end = params.value(4);
    const QVariant req = params.value(5);
    const QVariant endPoint = params.value(6);
    if (model.isValid() && model.canConvert<AADLObjectsModel *>() && start.isValid()
        && start.canConvert<AADLObjectContainer *>() && prov.isValid() && prov.canConvert<AADLObjectIfaceProvided *>()
        && startPoint.isValid() && startPoint.canConvert<QPointF>() && req.isValid()
        && req.canConvert<AADLObjectIfaceRequired *>() && end.isValid() && end.canConvert<AADLObjectContainer *>()
        && endPoint.isValid() && endPoint.canConvert<QPointF>()) {
        return new CmdDirectConnectionItemCreate(
                model.value<AADLObjectsModel *>(), start.value<AADLObjectContainer *>(),
                prov.value<AADLObjectIfaceProvided *>(), startPoint.value<QPointF>(),
                end.value<AADLObjectContainer *>(), req.value<AADLObjectIfaceRequired *>(), endPoint.value<QPointF>());
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::changeGeometryCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariant points = params.value(1);
    if (entity.isValid() && entity.canConvert<AADLObject *>() && points.isValid()
        && points.canConvert<QVector<QPointF>>()) {
        return new CmdEntityGeometryChange(entity.value<AADLObject *>(), points.value<QVector<QPointF>>());
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::changeCommentCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariant comment = params.value(1);
    if (entity.isValid() && entity.canConvert<AADLObjectComment *>() && comment.isValid()
        && comment.canConvert<QString>()) {
        return new CmdCommentTextChange(entity.value<AADLObjectComment *>(), comment.value<QString>());
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::removeEntityCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariant model = params.value(1);
    if (entity.isValid() && entity.canConvert<AADLObject *>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>()) {
        return new CmdEntityRemove(entity.value<AADLObject *>(), model.value<AADLObjectsModel *>());
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::changeEntityPropertyCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariantHash properties = params.value(1).toHash();
    if (entity.isValid() && entity.canConvert<AADLObject *>() && !properties.isEmpty())
        return new CmdEntityPropertyChange(entity.value<AADLObject *>(), properties);

    return nullptr;
}

QUndoCommand *CommandsFactory::changeEntityAttributesCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariantHash attributess = params.value(1).toHash();
    if (entity.isValid() && entity.canConvert<AADLObject *>() && !attributess.isEmpty())
        return new CmdEntityAttributeChange(entity.value<AADLObject *>(), attributess);

    return nullptr;
}

} // ns cmd
} // ns aadl
} // ns taste3
