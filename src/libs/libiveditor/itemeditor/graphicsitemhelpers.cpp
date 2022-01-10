﻿/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "graphicsitemhelpers.h"

#include "connectioncreationvalidator.h"
#include "graphicsviewutils.h"
#include "itemeditor/common/ivutils.h"
#include "itemeditor/ivcommentgraphicsitem.h"
#include "itemeditor/ivconnectiongraphicsitem.h"
#include "itemeditor/ivconnectiongroupgraphicsitem.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivfunctiontypegraphicsitem.h"
#include "itemeditor/ivinterfacegraphicsitem.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivobject.h"
#include "ui/veinteractiveobject.h"

#include <QGraphicsScene>
#include <QMetaEnum>

namespace ive {
namespace gi {

namespace utils = shared::graphicsviewutils;

ivm::IVFunction *functionObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<IVFunctionGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

ivm::IVFunctionType *functionTypeObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto functionType = qobject_cast<IVFunctionTypeGraphicsItem *>(item->toGraphicsObject()))
        return functionType->entity();

    return nullptr;
};

ivm::IVInterface *interfaceObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto iface = qobject_cast<IVInterfaceGraphicsItem *>(item->toGraphicsObject()))
        return iface->entity();

    return nullptr;
};

ivm::IVComment *commentObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto comment = qobject_cast<IVCommentGraphicsItem *>(item->toGraphicsObject()))
        return comment->entity();

    return nullptr;
};

ivm::IVConnection *connectionObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto connection = qobject_cast<IVConnectionGraphicsItem *>(item->toGraphicsObject()))
        return connection->entity();

    return nullptr;
};

bool isOwnConnection(const QGraphicsItem *owner, const QGraphicsItem *connection)
{
    if (!owner || !connection)
        return false;

    if (auto con = qobject_cast<const IVConnectionGraphicsItem *>(connection->toGraphicsObject()))
        for (auto item : owner->childItems())
            if (IVInterfaceGraphicsItem::Type == item->type())
                if (con->startItem() == item || con->endItem() == item)
                    return true;

    return false;
}

ivm::IVObject *object(const QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto interactiveObject = qobject_cast<const shared::ui::VEInteractiveObject *>(item->toGraphicsObject()))
        return interactiveObject->entity() ? interactiveObject->entity()->as<ivm::IVObject *>() : nullptr;

    return nullptr;
}

static bool isReversed(const ivm::ValidationResult &result)
{
    bool isReversed { false };

    if (result.isToOrFromNested) {
        if (result.startIface && result.endIface) {
            Q_ASSERT(result.startIface->isProvided() == result.endIface->isProvided()
                    && result.endIface->isRequired() == result.startIface->isRequired());
        }
        if (result.startIface) {
            isReversed = (result.startIface->isRequired() && shared::isAncestorOf(result.startObject, result.endObject))
                    || (result.startIface->isProvided() && shared::isAncestorOf(result.endObject, result.startObject));
        } else if (result.endIface) {
            isReversed = (result.endIface->isRequired() && shared::isAncestorOf(result.startObject, result.endObject))
                    || (result.endIface->isProvided() && shared::isAncestorOf(result.endObject, result.startObject));
        } else {
            isReversed = shared::isAncestorOf(result.startObject, result.endObject);
        }
    } else {
        if (result.startIface && result.endIface) {
            Q_ASSERT(result.startIface->isProvided() == result.endIface->isRequired()
                    && result.endIface->isProvided() == result.startIface->isRequired());
        }

        if (result.startIface) {
            isReversed = result.startIface->isProvided();
        } else if (result.endIface) {
            isReversed = result.endIface->isRequired();
        }
    }

    return isReversed;
}

QList<int> knownGraphicsItemTypes()
{
    QList<int> result;

    const QMetaEnum &me = QMetaEnum::fromType<ivm::IVObject::Type>();
    for (int i = 0; i < me.keyCount(); ++i) {
        int itemType = 0;
        const ivm::IVObject::Type objectType = static_cast<ivm::IVObject::Type>(me.value(i));
        switch (objectType) {
        case ivm::IVObject::Type::Function:
            itemType = ive::IVFunctionGraphicsItem::Type;
            break;
        case ivm::IVObject::Type::FunctionType:
            itemType = ive::IVFunctionTypeGraphicsItem::Type;
            break;
        case ivm::IVObject::Type::InterfaceGroup:
        case ivm::IVObject::Type::ProvidedInterface:
        case ivm::IVObject::Type::RequiredInterface:
            itemType = ive::IVInterfaceGraphicsItem::Type;
            break;
        case ivm::IVObject::Type::Comment:
            itemType = ive::IVCommentGraphicsItem::Type;
            break;
        case ivm::IVObject::Type::Connection:
            itemType = ive::IVConnectionGraphicsItem::Type;
            break;
        case ivm::IVObject::Type::ConnectionGroup:
            itemType = ive::IVConnectionGroupGraphicsItem::Type;
            break;
        case ivm::IVObject::Type::ConnectionLayer:
        case ivm::IVObject::Type::Unknown:
            continue;
        }
        if (itemType != 0) {
            result.append(itemType);
        }
    }

    return result;
}

ivm::ValidationResult validateConnectionCreate(QGraphicsScene *scene, const QVector<QPointF> &points)
{
    const QPointF startPos { points.first() };
    const QPointF endPos { points.last() };
    const QLineF connectionLine = { startPos, endPos };

    ivm::ValidationResult result;
    result.connectionPoints = points;
    result.functionAtStartPos = utils::nearestItem(
            scene, utils::adjustFromPoint(startPos, utils::kFunctionTolerance), { ive::IVFunctionGraphicsItem::Type });
    result.functionAtEndPos = utils::nearestItem(
            scene, utils::adjustFromPoint(endPos, utils::kFunctionTolerance), { ive::IVFunctionGraphicsItem::Type });
    result.startObject = ive::gi::functionObject(result.functionAtStartPos);
    result.endObject = ive::gi::functionObject(result.functionAtEndPos);
    result.isToOrFromNested =
            (result.functionAtStartPos && result.functionAtStartPos->isAncestorOf(result.functionAtEndPos))
            || (result.functionAtEndPos && result.functionAtEndPos->isAncestorOf(result.functionAtStartPos));

    if (!result.startObject) {
        result.setFailed(ivm::ConnectionCreationValidator::FailReason::NoStartFunction);
        return result;
    }

    const auto startIfaceItem = qgraphicsitem_cast<ive::IVInterfaceGraphicsItem *>(utils::nearestItem(scene,
            utils::adjustFromPoint(startPos, utils::kInterfaceTolerance), { ive::IVInterfaceGraphicsItem::Type }));

    static const QMarginsF kToleranceMargins = { utils::kInterfaceTolerance / 2, utils::kInterfaceTolerance / 2,
        utils::kInterfaceTolerance / 2, utils::kInterfaceTolerance / 2 };

    if (startIfaceItem
            && startIfaceItem->ifaceShape().boundingRect().marginsAdded(kToleranceMargins).contains(startPos)) {
        result.startIface = startIfaceItem->entity();
        result.startPointAdjusted =
                startIfaceItem->connectionEndPoint(result.functionAtStartPos->isAncestorOf(result.functionAtEndPos));
    } else if (!utils::intersects(
                       result.functionAtStartPos->sceneBoundingRect(), connectionLine, &result.startPointAdjusted)) {
        result.setFailed(ivm::ConnectionCreationValidator::FailReason::CannotCreateStartIface);
        return result;
    }

    if (!result.endObject) {
        result.setFailed(ivm::ConnectionCreationValidator::FailReason::NoEndFunction);
        return result;
    }

    const auto endIfaceItem = qgraphicsitem_cast<ive::IVInterfaceGraphicsItem *>(utils::nearestItem(
            scene, utils::adjustFromPoint(endPos, utils::kInterfaceTolerance), { ive::IVInterfaceGraphicsItem::Type }));
    if (endIfaceItem && endIfaceItem->ifaceShape().boundingRect().marginsAdded(kToleranceMargins).contains(endPos)) {
        result.endIface = endIfaceItem->entity();
        result.endPointAdjusted =
                endIfaceItem->connectionEndPoint(result.functionAtEndPos->isAncestorOf(result.functionAtStartPos));
    } else if (!utils::intersects(
                       result.functionAtEndPos->sceneBoundingRect(), connectionLine, &result.endPointAdjusted)) {
        result.setFailed(ivm::ConnectionCreationValidator::FailReason::CannotCreateEndIface);
        return result;
    }

    if (result.startIface && result.endIface && result.isToOrFromNested) {
        if (result.startIface->direction() != result.endIface->direction()) {
            result.setFailed(ivm::ConnectionCreationValidator::FailReason::ToFromNestedDifferentDirection);
            return result;
        }
    }

    if (result.startIface && result.endIface) {
        if (result.startIface->direction() == result.endIface->direction() && !result.isToOrFromNested) {
            result.setFailed(ivm::ConnectionCreationValidator::FailReason::SameDirectionIfaceWrongParents);
            return result;
        }
    }

    if (!result.startIface) {
        if (auto fn = result.startObject->as<const ivm::IVFunction *>())
            if (fn->instanceOf()) {
                result.setFailed(
                        ivm::ConnectionCreationValidator::FailReason::DirectIfaceCreationInInstanceOfFunctionType);
                return result;
            }
    }

    if (!result.endIface) {
        if (auto fn = result.endObject->as<const ivm::IVFunction *>())
            if (fn->instanceOf()) {
                result.setFailed(
                        ivm::ConnectionCreationValidator::FailReason::DirectIfaceCreationInInstanceOfFunctionType);
                return result;
            }
    }

    if (isReversed(result)) {
        std::swap(result.startIface, result.endIface);
        std::swap(result.startIfaceId, result.endIfaceId);
        std::swap(result.startObject, result.endObject);
        std::swap(result.startPointAdjusted, result.endPointAdjusted);
        std::swap(result.functionAtStartPos, result.functionAtEndPos);
        std::reverse(result.connectionPoints.begin(), result.connectionPoints.end());
    }

    result.startIfaceId = result.startIface ? result.startIface->id() : shared::createId();
    result.endIfaceId = result.endIface ? result.endIface->id() : shared::createId();
    result.status = ivm::ConnectionCreationValidator::canConnect(
            result.startObject, result.endObject, result.startIface, result.endIface);
    return result;
}

QList<int> rectangularTypes()
{
    return { IVCommentGraphicsItem::Type, IVFunctionGraphicsItem::Type, IVFunctionTypeGraphicsItem::Type };
}

QList<int> connectionTypes()
{
    return { IVConnectionGraphicsItem::Type, IVConnectionGroupGraphicsItem::Type };
}

qreal itemLevel(const ivm::IVObject *const object, bool itemSelected)
{
    if (!object || itemSelected) {
        return ZOrder.Selected;
    }

    switch (object->type()) {
    case ivm::IVObject::Type::Function:
    case ivm::IVObject::Type::FunctionType:
        return ZOrder.Function;
    case ivm::IVObject::Type::InterfaceGroup:
    case ivm::IVObject::Type::RequiredInterface:
    case ivm::IVObject::Type::ProvidedInterface:
        return ZOrder.Interface;
    case ivm::IVObject::Type::Comment:
        return ZOrder.Comment;
    case ivm::IVObject::Type::ConnectionGroup:
    case ivm::IVObject::Type::Connection:
        return ZOrder.Connection;
    default:
        return ZOrder.Selected;
    }
}


} // namespace gi
} // namespace ive
