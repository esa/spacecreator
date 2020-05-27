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

#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "cmdcommentitemcreate.h"
#include "cmdconnectionitemcreate.h"
#include "cmdcontextparameterchange.h"
#include "cmdcontextparametercreate.h"
#include "cmdcontextparameterremove.h"
#include "cmdentityattributechange.h"
#include "cmdentityautolayout.h"
#include "cmdentitygeometrychange.h"
#include "cmdentitypropertychange.h"
#include "cmdentitypropertycreate.h"
#include "cmdentitypropertyremove.h"
#include "cmdentitypropertyrename.h"
#include "cmdentityremove.h"
#include "cmdfunctionitemcreate.h"
#include "cmdfunctiontypeitemcreate.h"
#include "cmdifaceattrchange.h"
#include "cmdifaceparamchange.h"
#include "cmdifaceparamcreate.h"
#include "cmdifaceparamremove.h"
#include "cmdinterfaceitemcreate.h"
#include "cmdrequiredifacepropertychange.h"
#include "cmdrootentitychange.h"
#include "commandids.h"

#include <QRect>
#include <QVariant>
#include <QtDebug>
#include <aadlobjectsmodel.h>

namespace aadlinterface {
namespace cmd {

/*!
 * \namespace aadlinterface::cmd
 * \brief Undo and redo commands code for the Interface View Editor
 */

QUndoCommand *CommandsFactory::create(Id id, const QVariantList &params)
{
    switch (id) {
    case cmd::CreateFunctionTypeEntity:
        return cmd::CommandsFactory::createFunctionTypeCommand(params);
    case cmd::CreateFunctionEntity:
        return cmd::CommandsFactory::createFunctionCommand(params);
    case cmd::CreateCommentEntity:
        return cmd::CommandsFactory::createCommentCommand(params);
    case cmd::CreateInterfaceEntity:
        return cmd::CommandsFactory::createInterfaceCommand(params);
    case cmd::CreateConnectionEntity:
        return cmd::CommandsFactory::createConnectionCommand(params);
    case cmd::ChangeEntityGeometry:
        return cmd::CommandsFactory::changeGeometryCommand(params);
    case cmd::RemoveEntity:
        return cmd::CommandsFactory::removeEntityCommand(params);
    case cmd::ChangeRootEntity:
        return cmd::CommandsFactory::changeRootEntityCommand(params);
    case cmd::AutoLayoutEntity:
        return cmd::CommandsFactory::autoLayoutEntityCommand(params);
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

    case cmd::ChangeRequiredIfaceProperty:
        return cmd::CommandsFactory::changeRiPropertyCommand(params);
    case cmd::ChangeIfaceAttribute:
        return cmd::CommandsFactory::changeIfaceAttributeCommand(params);

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
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid()
            && model.canConvert<aadl::AADLObjectsModel *>() && parent.canConvert<aadl::AADLObjectFunction *>())
        return new CmdFunctionItemCreate(model.value<aadl::AADLObjectsModel *>(),
                parent.value<aadl::AADLObjectFunction *>(), geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createFunctionTypeCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant model = params.value(0);
    const QVariant parent = params.value(1);
    const QVariant geometry = params.value(2);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid()
            && model.canConvert<aadl::AADLObjectsModel *>() && parent.canConvert<aadl::AADLObjectFunction *>())
        return new CmdFunctionTypeItemCreate(model.value<aadl::AADLObjectsModel *>(),
                parent.value<aadl::AADLObjectFunction *>(), geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createCommentCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant model = params.value(0);
    const QVariant parent = params.value(1);
    const QVariant geometry = params.value(2);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid()
            && model.canConvert<aadl::AADLObjectsModel *>())
        return new CmdCommentItemCreate(model.value<aadl::AADLObjectsModel *>(),
                parent.value<aadl::AADLObjectFunctionType *>(), geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createInterfaceCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 1);
    const QVariant creationInfo = params.value(0);
    if (creationInfo.isValid() && creationInfo.canConvert<aadl::AADLObjectIface::CreationInfo>())
        return new CmdInterfaceItemCreate(creationInfo.value<aadl::AADLObjectIface::CreationInfo>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createConnectionCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 5);
    const QVariant model = params.value(0);
    const QVariant parentFunction = params.value(1);
    const QVariant startIfaceId = params.value(2);
    const QVariant endIfaceId = params.value(3);
    const QVariant points = params.value(4);
    if (points.isValid() && points.canConvert<QVector<QPointF>>() && model.isValid()
            && model.canConvert<aadl::AADLObjectsModel *>() && parentFunction.isValid()
            && parentFunction.canConvert<aadl::AADLObjectFunction *>() && startIfaceId.isValid()
            && startIfaceId.canConvert<shared::Id>() && endIfaceId.isValid() && endIfaceId.canConvert<shared::Id>()) {
        return new CmdConnectionItemCreate(model.value<aadl::AADLObjectsModel *>(),
                parentFunction.value<aadl::AADLObjectFunction *>(), startIfaceId.value<shared::Id>(),
                endIfaceId.value<shared::Id>(), points.value<QVector<QPointF>>());
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::changeGeometryCommand(const QVariantList &params)
{
    QList<QPair<aadl::AADLObject *, QVector<QPointF>>> objectsData;
    for (const auto &param : params) {
        if (!param.isValid() || param.type() != QVariant::List)
            return nullptr;

        const QVariantList objectDataList = param.toList();
        if (objectDataList.isEmpty())
            return nullptr;

        Q_ASSERT(objectDataList.size() == 2);
        const QVariant entity = objectDataList.value(0);
        const QVariant points = objectDataList.value(1);
        if (entity.isValid() && entity.canConvert<aadl::AADLObject *>() && points.isValid()
                && points.canConvert<QVector<QPointF>>()) {
            objectsData.append(qMakePair(entity.value<aadl::AADLObject *>(), points.value<QVector<QPointF>>()));
        } else {
            return nullptr;
        }
    }
    return new CmdEntityGeometryChange(objectsData);
}

QUndoCommand *CommandsFactory::removeEntityCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariant model = params.value(1);
    if (entity.isValid() && entity.canConvert<aadl::AADLObject *>() && model.isValid()
            && model.canConvert<aadl::AADLObjectsModel *>()) {
        return new CmdEntityRemove(entity.value<aadl::AADLObject *>(), model.value<aadl::AADLObjectsModel *>());
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::addEntityPropertyCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariantHash properties = params.value(1).toHash();
    if (entity.isValid() && entity.canConvert<aadl::AADLObject *>() && !properties.isEmpty())
        return new CmdEntityPropertyCreate(entity.value<aadl::AADLObject *>(), properties);

    return nullptr;
}

QUndoCommand *CommandsFactory::changeEntityPropertyCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariantHash properties = params.value(1).toHash();
    if (entity.isValid() && entity.canConvert<aadl::AADLObject *>() && !properties.isEmpty()) {
        if (auto aadlObject = entity.value<aadl::AADLObject *>()) {
            if (aadlObject->isInterface())
                if (aadlObject->as<aadl::AADLObjectIfaceRequired *>()) {
                    return changeRiPropertyCommand(params);
                }
            return new CmdEntityPropertyChange(aadlObject, properties);
        }
    }
    return nullptr;
}

QUndoCommand *CommandsFactory::renameEntityPropertyCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QHash<QString, QString> properties = params.value(1).value<QHash<QString, QString>>();
    if (entity.isValid() && entity.canConvert<aadl::AADLObject *>() && !properties.isEmpty())
        return new CmdEntityPropertyRename(entity.value<aadl::AADLObject *>(), properties);

    return nullptr;
}

QUndoCommand *CommandsFactory::removeEntityPropertyCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QStringList properties = params.value(1).toStringList();
    if (entity.isValid() && entity.canConvert<aadl::AADLObject *>() && !properties.isEmpty())
        return new CmdEntityPropertyRemove(entity.value<aadl::AADLObject *>(), properties);

    return nullptr;
}

QUndoCommand *CommandsFactory::changeEntityAttributesCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariantHash attributess = params.value(1).toHash();
    if (entity.isValid() && entity.canConvert<aadl::AADLObject *>() && !attributess.isEmpty()) {
        if (auto aadlObject = entity.value<aadl::AADLObject *>()) {
            if (aadlObject->isInterface())
                return changeIfaceAttributeCommand(params);

            return new CmdEntityAttributeChange(aadlObject, attributess);
        }
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createContextParameterCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    auto param = params.at(1).value<aadl::ContextParameter>();
    if (entity.isValid() && entity.canConvert<aadl::AADLObjectFunctionType *>())
        return new CmdContextParameterCreate(entity.value<aadl::AADLObjectFunctionType *>(), param);

    return nullptr;
}

QUndoCommand *CommandsFactory::changeContextParameterCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant entity = params.value(0);
    auto paramOld = params.at(1).value<aadl::ContextParameter>();
    auto paramNew = params.at(2).value<aadl::ContextParameter>();
    if (entity.isValid() && entity.canConvert<aadl::AADLObjectFunctionType *>())
        return new CmdContextParameterChange(entity.value<aadl::AADLObjectFunctionType *>(), paramOld, paramNew);

    return nullptr;
}

QUndoCommand *CommandsFactory::removeContextParameterCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const int id = params.at(1).toInt();
    if (entity.isValid() && entity.canConvert<aadl::AADLObjectFunctionType *>())
        return new CmdContextParameterRemove(entity.value<aadl::AADLObjectFunctionType *>(), id);

    return nullptr;
}

QUndoCommand *CommandsFactory::createIfaceParamCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    auto param = params.at(1).value<aadl::IfaceParameter>();
    if (entity.isValid() && entity.canConvert<aadl::AADLObjectIface *>())
        return new CmdIfaceParamCreate(entity.value<aadl::AADLObjectIface *>(), param);

    return nullptr;
}

QUndoCommand *CommandsFactory::removeIfaceParamCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    auto param = params.at(1).value<aadl::IfaceParameter>();
    if (entity.isValid() && entity.canConvert<aadl::AADLObjectIface *>())
        return new CmdIfaceParamRemove(entity.value<aadl::AADLObjectIface *>(), param);

    return nullptr;
}

QUndoCommand *CommandsFactory::changeIfaceParamCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 3);
    const QVariant entity = params.value(0);
    auto paramOld = params.at(1).value<aadl::IfaceParameter>();
    auto paramNew = params.at(2).value<aadl::IfaceParameter>();
    if (entity.isValid() && entity.canConvert<aadl::AADLObjectIface *>())
        return new CmdIfaceParamChange(entity.value<aadl::AADLObjectIface *>(), paramOld, paramNew);

    return nullptr;
}

QUndoCommand *CommandsFactory::changeRiPropertyCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariantHash properties = params.value(1).toHash();
    if (entity.isValid() && entity.canConvert<aadl::AADLObjectIfaceRequired *>() && !properties.isEmpty()) {
        if (aadl::AADLObjectIfaceRequired *ri = entity.value<aadl::AADLObjectIfaceRequired *>()) {
            const QString propName = properties.keys().first();
            return new CmdRequiredIfacePropertyChange(ri, propName, properties.value(propName));
        }
    }
    return nullptr;
}

QUndoCommand *CommandsFactory::changeIfaceAttributeCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant entity = params.value(0);
    const QVariantHash attrs = params.value(1).toHash();
    if (entity.isValid() && entity.canConvert<aadl::AADLObjectIface *>() && !attrs.isEmpty()) {
        if (auto iface = entity.value<aadl::AADLObjectIface *>()) {
            const QString attrName = attrs.keys().first();
            return new CmdIfaceAttrChange(iface, attrName, attrs.value(attrName));
        }
    }
    return nullptr;
}

QUndoCommand *CommandsFactory::changeRootEntityCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant model = params.value(0);
    const QVariant id = params.value(1);
    if (id.isValid() && id.canConvert<shared::Id>() && model.isValid()
            && model.canConvert<aadl::AADLObjectsModel *>()) {
        return new CmdRootEntityChange(model.value<aadl::AADLObjectsModel *>(), id.value<shared::Id>());
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::autoLayoutEntityCommand(const QVariantList &params)
{
    QList<QPair<aadl::AADLObject *, QVector<QPointF>>> objectsData;
    for (const auto &param : params) {
        const QVariantList objectDataList = param.toList();
        if (objectDataList.isEmpty())
            return nullptr;

        Q_ASSERT(objectDataList.size() == 2);
        const QVariant entity = objectDataList.value(0);
        const QVariant points = objectDataList.value(1);
        if (entity.isValid() && entity.canConvert<aadl::AADLObject *>() && points.isValid()
                && points.canConvert<QVector<QPointF>>()) {
            objectsData.append(qMakePair(entity.value<aadl::AADLObject *>(), points.value<QVector<QPointF>>()));
        } else {
            return nullptr;
        }
    }
    return new CmdEntityAutoLayout(objectsData);
}

}
}
