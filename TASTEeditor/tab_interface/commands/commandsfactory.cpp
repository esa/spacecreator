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
#include "cmdcontaineritemcreate.h"
#include "cmdfunctionitemcreate.h"
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

    default:
        qWarning() << "CommandsStack::push - command ignored" << id;
        break;
    }

    return nullptr;
}

QUndoCommand *CommandsFactory::createFunctionCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant geometry = params.value(0);
    const QVariant model = params.value(1);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>())
        return new CmdFunctionItemCreate(model.value<AADLObjectsModel *>(), geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createContainerCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant geometry = params.value(0);
    const QVariant model = params.value(1);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>())
        return new CmdContainerItemCreate(model.value<AADLObjectsModel *>(), geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createCommentCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant geometry = params.value(0);
    const QVariant model = params.value(1);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>())
        return new CmdCommentItemCreate(model.value<AADLObjectsModel *>(), geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createProvidedInterfaceCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant geometry = params.value(0);
    const QVariant model = params.value(1);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>())
        return new CmdProvidedInterfaceItemCreate(model.value<AADLObjectsModel *>(), geometry.value<QRectF>());

    return nullptr;
}

QUndoCommand *CommandsFactory::createRequiredInterfaceCommand(const QVariantList &params)
{
    Q_ASSERT(params.size() == 2);
    const QVariant geometry = params.value(0);
    const QVariant model = params.value(1);
    if (geometry.isValid() && geometry.canConvert<QRectF>() && model.isValid()
        && model.canConvert<AADLObjectsModel *>())
        return new CmdRequiredInterfaceItemCreate(model.value<AADLObjectsModel *>(), geometry.value<QRectF>());

    return nullptr;
}

} // ns cmd
} // ns aadl
} // ns taste3
