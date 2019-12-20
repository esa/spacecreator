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
#include "cmdcontextparameterchange.h"
#include "cmdcontextparametercreate.h"
#include "cmdcontextparameterremove.h"
#include "cmddirectconnectionitemcreate.h"
#include "cmdentityattributechange.h"
#include "cmdentitygeometrychange.h"
#include "cmdentitypropertychange.h"
#include "cmdentitypropertycreate.h"
#include "cmdentitypropertyremove.h"
#include "cmdentitypropertyrename.h"
#include "cmdentityremove.h"
#include "cmdfunctionitemcreate.h"
#include "cmdfunctiontypeitemcreate.h"
#include "cmdifaceparamchange.h"
#include "cmdifaceparamcreate.h"
#include "cmdifaceparamremove.h"
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
    case cmd::CreateFunctionTypeEntity:
        return cmd::CommandsFactory::createFunctionTypeCommand(params);
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
    case cmd::CreateEntityProperty:
        return cmd::CommandsFactory::addEntityPropertyCommand(params);
    case cmd::ChangeEntityProperty:
        return cmd::CommandsFactory::changeEntityPropertyCommand(params);
    case cmd::RenameEntityProperty:
        return cmd::CommandsFactory::renameEntityPropertyCommand(params);
    case cmd::RemoveEntityProperty:
        return cmd::CommandsFactory::removeEntityPropertyCommand(params);
    case cmd::ChangeEntityAttributes:
        return cmd::CommandsFactory::changeEntityAttributesCommand(params);

    case cmd::CreateContextParameter:
        return cmd::CommandsFactory::createContextParameterCommand(params);
    case cmd::ChangeContextParameter:
        return cmd::CommandsFactory::changeContextParameterCommand(params);
    case cmd::RemoveContextParameter:
        return cmd::CommandsFactory::removeContextParameterCommand(params);

    case cmd::CreateIfaceParam:
        return cmd::CommandsFactory::createIfaceParamCommand(params);
    case cmd::RemoveIfaceParam:
        return cmd::CommandsFactory::removeIfaceParamCommand(params);
    case cmd::ChangeIfaceParam:
        return cmd::CommandsFactory::changeIfaceParamCommand(params);

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
        && parent.canConvert<AADLObjectFunction *>())
        return new CmdFunctionItemCreate(model.value<AADLObjectsModel *>(), parent.value<AADLObjectFunction *>(),
                                         geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createFunctionTypeCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant model = params.value(0);
    const QVariant parent = params.value(1);
    const QVariant geometry = params.value(2);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid() && model.canConvert<AADLObjectsModel *>()
        && parent.canConvert<AADLObjectFunction *>())
        return new CmdFunctionTypeItemCreate(model.value<AADLObjectsModel *>(), parent.value<AADLObjectFunction *>(),
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
        && model.canConvert<AADLObjectsModel *>() && parent.canConvert<AADLObjectFunctionType *>())
        return new CmdProvidedInterfaceItemCreate(model.value<AADLObjectsModel *>(),
                                                  parent.value<AADLObjectFunctionType *>(), position.value<QPointF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createRequiredInterfaceCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant model = params.value(0);
    const QVariant parent = params.value(1);
    const QVariant position = params.value(2);
    if (position.isValid() && position.canConvert<QPointF>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>() && parent.canConvert<AADLObjectFunctionType *>())
        return new CmdRequiredInterfaceItemCreate(model.value<AADLObjectsModel *>(),
                                                  parent.value<AADLObjectFunctionType *>(), position.value<QPointF>());

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
        && model.canConvert<AADLObjectsModel *>() && start.isValid() && start.canConvert<AADLObjectFunction *>()
        && end.isValid() && end.canConvert<AADLObjectFunction *>() && pi.isValid()
        && pi.canConvert<AADLObjectIfaceProvided *>() && ri.isValid() && ri.canConvert<AADLObjectIfaceRequired *>()) {
        return new CmdManualConnectionItemCreate(
                model.value<AADLObjectsModel *>(), start.value<AADLObjectFunction *>(),
                end.value<AADLObjectFunction *>(), pi.value<AADLObjectIfaceProvided *>(),
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
        && start.canConvert<AADLObjectFunctionType *>() && prov.isValid()
        && prov.canConvert<AADLObjectIfaceProvided *>() && startPoint.isValid() && startPoint.canConvert<QPointF>()
        && req.isValid() && req.canConvert<AADLObjectIfaceRequired *>() && end.isValid()
        && end.canConvert<AADLObjectFunctionType *>() && endPoint.isValid() && endPoint.canConvert<QPointF>()) {
        return new CmdDirectConnectionItemCreate(model.value<AADLObjectsModel *>(),
                                                 start.value<AADLObjectFunctionType *>(),
                                                 prov.value<AADLObjectIfaceProvided *>(), startPoint.value<QPointF>(),
                                                 end.value<AADLObjectFunctionType *>(),
                                                 req.value<AADLObjectIfaceRequired *>(), endPoint.value<QPointF>());
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

QUndoCommand *CommandsFactory::addEntityPropertyCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariantHash properties = params.value(1).toHash();
    if (entity.isValid() && entity.canConvert<AADLObject *>() && !properties.isEmpty())
        return new CmdEntityPropertyCreate(entity.value<AADLObject *>(), properties);

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

QUndoCommand *CommandsFactory::renameEntityPropertyCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QHash<QString, QString> properties = params.value(1).value<QHash<QString, QString>>();
    if (entity.isValid() && entity.canConvert<AADLObject *>() && !properties.isEmpty())
        return new CmdEntityPropertyRename(entity.value<AADLObject *>(), properties);

    return nullptr;
}

QUndoCommand *CommandsFactory::removeEntityPropertyCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QStringList properties = params.value(1).toStringList();
    if (entity.isValid() && entity.canConvert<AADLObject *>() && !properties.isEmpty())
        return new CmdEntityPropertyRemove(entity.value<AADLObject *>(), properties);

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

QUndoCommand *CommandsFactory::createContextParameterCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const ContextParameter &param = params.at(1).value<ContextParameter>();
    if (entity.isValid() && entity.canConvert<AADLObjectFunctionType *>())
        return new CmdContextParameterCreate(entity.value<AADLObjectFunctionType *>(), param);

    return nullptr;
}

QUndoCommand *CommandsFactory::changeContextParameterCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant entity = params.value(0);
    const ContextParameter &paramOld = params.at(1).value<ContextParameter>();
    const ContextParameter &paramNew = params.at(2).value<ContextParameter>();
    if (entity.isValid() && entity.canConvert<AADLObjectFunctionType *>())
        return new CmdContextParameterChange(entity.value<AADLObjectFunctionType *>(), paramOld, paramNew);

    return nullptr;
}

QUndoCommand *CommandsFactory::removeContextParameterCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const int id = params.at(1).toInt();
    if (entity.isValid() && entity.canConvert<AADLObjectFunctionType *>())
        return new CmdContextParameterRemove(entity.value<AADLObjectFunctionType *>(), id);

    return nullptr;
}

QUndoCommand *CommandsFactory::createIfaceParamCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const IfaceParameter param = params.at(1).value<IfaceParameter>();
    if (entity.isValid() && entity.canConvert<AADLObjectIface *>())
        return new CmdIfaceParamCreate(entity.value<AADLObjectIface *>(), param);

    return nullptr;
}

QUndoCommand *CommandsFactory::removeIfaceParamCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const IfaceParameter param = params.at(1).value<IfaceParameter>();
    if (entity.isValid() && entity.canConvert<AADLObjectIface *>())
        return new CmdIfaceParamRemove(entity.value<AADLObjectIface *>(), param);

    return nullptr;
}

QUndoCommand *CommandsFactory::changeIfaceParamCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant entity = params.value(0);
    const IfaceParameter paramOld = params.at(1).value<IfaceParameter>();
    const IfaceParameter paramNew = params.at(2).value<IfaceParameter>();
    if (entity.isValid() && entity.canConvert<AADLObjectIface *>())
        return new CmdIfaceParamChange(entity.value<AADLObjectIface *>(), paramOld, paramNew);

    return nullptr;
}

} // ns cmd
} // ns aadl
} // ns taste3
