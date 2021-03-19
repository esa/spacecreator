/*
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

#include "aadlcomment.h"
#include "aadlconnection.h"
#include "aadlfunction.h"
#include "aadlfunctiontype.h"
#include "aadliface.h"
#include "aadlobject.h"
#include "baseitems/common/aadlutils.h"
#include "baseitems/interactiveobject.h"
#include "connectioncreationvalidator.h"
#include "graphicsviewutils.h"
#include "interface/aadlcommentgraphicsitem.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlconnectiongroupgraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlfunctiontypegraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"

#include <QGraphicsScene>
#include <QMetaEnum>

namespace ive {
namespace gi {

static const QList<int> kRectTypes { AADLCommentGraphicsItem::Type, AADLFunctionGraphicsItem::Type,
    AADLFunctionTypeGraphicsItem::Type };

ivm::AADLFunction *functionObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto function = qobject_cast<AADLFunctionGraphicsItem *>(item->toGraphicsObject()))
        return function->entity();

    return nullptr;
};

ivm::AADLFunctionType *functionTypeObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto functionType = qobject_cast<AADLFunctionTypeGraphicsItem *>(item->toGraphicsObject()))
        return functionType->entity();

    return nullptr;
};

ivm::AADLIface *interfaceObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto iface = qobject_cast<AADLInterfaceGraphicsItem *>(item->toGraphicsObject()))
        return iface->entity();

    return nullptr;
};

ivm::AADLComment *commentObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto comment = qobject_cast<AADLCommentGraphicsItem *>(item->toGraphicsObject()))
        return comment->entity();

    return nullptr;
};

ivm::AADLConnection *connectionObject(QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto connection = qobject_cast<AADLConnectionGraphicsItem *>(item->toGraphicsObject()))
        return connection->entity();

    return nullptr;
};

bool isOwnConnection(const QGraphicsItem *owner, const QGraphicsItem *connection)
{
    if (!owner || !connection)
        return false;

    if (auto con = qobject_cast<const AADLConnectionGraphicsItem *>(connection->toGraphicsObject()))
        for (auto item : owner->childItems())
            if (AADLInterfaceGraphicsItem::Type == item->type())
                if (con->startItem() == item || con->endItem() == item)
                    return true;

    return false;
}

ivm::AADLObject *object(const QGraphicsItem *item)
{
    if (!item)
        return nullptr;

    if (auto interactiveObject = qobject_cast<const InteractiveObject *>(item->toGraphicsObject()))
        return interactiveObject->aadlObject();

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

ivm::ValidationResult validateConnectionCreate(QGraphicsScene *scene, const QVector<QPointF> &points)
{
    const QPointF startPos { points.first() };
    const QPointF endPos { points.last() };
    const QLineF connectionLine = { startPos, endPos };

    ivm::ValidationResult result;
    result.connectionPoints = points;
    result.functionAtStartPos = ive::nearestItem(
            scene, ive::adjustFromPoint(startPos, kFunctionTolerance), { ive::AADLFunctionGraphicsItem::Type });
    result.functionAtEndPos = ive::nearestItem(
            scene, ive::adjustFromPoint(endPos, kFunctionTolerance), { ive::AADLFunctionGraphicsItem::Type });
    result.startObject = ive::gi::functionObject(result.functionAtStartPos);
    result.endObject = ive::gi::functionObject(result.functionAtEndPos);
    result.isToOrFromNested =
            (result.functionAtStartPos && result.functionAtStartPos->isAncestorOf(result.functionAtEndPos))
            || (result.functionAtEndPos && result.functionAtEndPos->isAncestorOf(result.functionAtStartPos));

    if (!result.startObject) {
        result.setFailed(ivm::ConnectionCreationValidator::FailReason::NoStartFunction);
        return result;
    }

    const auto startIfaceItem = qgraphicsitem_cast<ive::AADLInterfaceGraphicsItem *>(ive::nearestItem(
            scene, ive::adjustFromPoint(startPos, kInterfaceTolerance), { ive::AADLInterfaceGraphicsItem::Type }));
    if (startIfaceItem
            && startIfaceItem->ifaceShape()
                       .boundingRect()
                       .adjusted(-kInterfaceTolerance / 2, -kInterfaceTolerance / 2, kInterfaceTolerance / 2,
                               kInterfaceTolerance / 2)
                       .contains(startPos)) {
        result.startIface = startIfaceItem->entity();
        result.startPointAdjusted =
                startIfaceItem->connectionEndPoint(result.functionAtStartPos->isAncestorOf(result.functionAtEndPos));
    } else if (!shared::graphicsviewutils::intersects(
                       result.functionAtStartPos->sceneBoundingRect(), connectionLine, &result.startPointAdjusted)) {
        result.setFailed(ivm::ConnectionCreationValidator::FailReason::CannotCreateStartIface);
        return result;
    }

    if (!result.endObject) {
        result.setFailed(ivm::ConnectionCreationValidator::FailReason::NoEndFunction);
        return result;
    }

    const auto endIfaceItem = qgraphicsitem_cast<ive::AADLInterfaceGraphicsItem *>(ive::nearestItem(
            scene, ive::adjustFromPoint(endPos, kInterfaceTolerance), { ive::AADLInterfaceGraphicsItem::Type }));
    if (endIfaceItem
            && endIfaceItem->ifaceShape()
                       .boundingRect()
                       .adjusted(-kInterfaceTolerance / 2, -kInterfaceTolerance / 2, kInterfaceTolerance / 2,
                               kInterfaceTolerance / 2)
                       .contains(endPos)) {
        result.endIface = endIfaceItem->entity();
        result.endPointAdjusted =
                endIfaceItem->connectionEndPoint(result.functionAtEndPos->isAncestorOf(result.functionAtStartPos));
    } else if (!shared::graphicsviewutils::intersects(
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
        if (auto fn = result.startObject->as<const ivm::AADLFunction *>())
            if (fn->instanceOf()) {
                result.setFailed(
                        ivm::ConnectionCreationValidator::FailReason::DirectIfaceCreationInInstanceOfFunctionType);
                return result;
            }
    }

    if (!result.endIface) {
        if (auto fn = result.endObject->as<const ivm::AADLFunction *>())
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
    return kRectTypes;
}

QList<int> knownGraphicsItemTypes()
{
    QList<int> result;

    const QMetaEnum &me = QMetaEnum::fromType<ivm::AADLObject::Type>();
    for (int i = 0; i < me.keyCount(); ++i) {
        int itemType = 0;
        const ivm::AADLObject::Type objectType = static_cast<ivm::AADLObject::Type>(me.value(i));
        switch (objectType) {
        case ivm::AADLObject::Type::Function:
            itemType = ive::AADLFunctionGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::FunctionType:
            itemType = ive::AADLFunctionTypeGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::InterfaceGroup:
        case ivm::AADLObject::Type::ProvidedInterface:
        case ivm::AADLObject::Type::RequiredInterface:
            itemType = ive::AADLInterfaceGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::Comment:
            itemType = ive::AADLCommentGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::Connection:
            itemType = ive::AADLConnectionGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::ConnectionGroup:
            itemType = ive::AADLConnectionGroupGraphicsItem::Type;
            break;
        case ivm::AADLObject::Type::Unknown:
            continue;
        }
        if (itemType != 0) {
            result.append(itemType);
        }
    }

    return result;
}

bool isCollided(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect)
{
    if (!upcomingItem || !upcomingItemRect.isValid()) {
        return false;
    }

    const QList<QRectF> siblingRects = siblingItemsRects(upcomingItem, gi::rectangularTypes());
    return ive::isCollided(siblingRects, upcomingItemRect.marginsAdded(kContentMargins));
}

bool isBounded(const QGraphicsItem *upcomingItem, const QRectF &upcomingItemRect)
{
    if (!upcomingItem || !upcomingItemRect.isValid()) {
        return false;
    }

    if (auto iObj = qobject_cast<const AADLRectGraphicsItem *>(upcomingItem->toGraphicsObject())) {
        const auto parentObj = qobject_cast<const InteractiveObject *>(iObj->parentObject());
        if (parentObj && parentObj->aadlObject()) {
            const QMarginsF margins = parentObj->aadlObject()->isRootObject() ? kRootMargins : kContentMargins;
            const QRectF outerRect = parentObj->sceneBoundingRect().marginsRemoved(margins);
            return ive::isRectBounded(outerRect, upcomingItemRect);
        } else if (iObj->aadlObject()->isRootObject()) {
            return ive::isRectBounded(
                    upcomingItemRect.marginsRemoved(kRootMargins), iObj->nestedItemsSceneBoundingRect());
        }
    }
    return true;
}

} // namespace gi
} // namespace ive
