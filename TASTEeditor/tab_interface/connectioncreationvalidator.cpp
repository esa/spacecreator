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

#include "connectioncreationvalidator.h"

#include "aadlconnectiongraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "aadlfunctiontypegraphicsitem.h"
#include "aadlinterfacegraphicsitem.h"
#include "graphicsitemhelpers.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtMath>
#include <baseitems/common/utils.h>
#include <tab_aadl/aadlobjectconnection.h>
#include <tab_aadl/aadlobjectfunction.h>
#include <tab_aadl/aadlobjectfunctiontype.h>
#include <tab_aadl/aadlobjectiface.h>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {

const qreal ConnectionCreationValidator::kInterfaceTolerance = 20.;
const qreal ConnectionCreationValidator::kConnectionTolerance = 20.;

ConnectionCreationValidator::ValidationResult
ConnectionCreationValidator::validate(QGraphicsScene *scene, const QVector<QPointF> &connectionPoints)
{
    const ValidationResult info = validateCreate(scene, connectionPoints);

    qDebug() << info.status;

    return info;
}

ConnectionCreationValidator::ValidationResult
ConnectionCreationValidator::validateCreate(QGraphicsScene *scene, const QVector<QPointF> &connectionPoints)
{
    ValidationResult result;

    result.connectionLine = { connectionPoints.first(), connectionPoints.last() };
    result.functionAtStartPos =
            utils::nearestItem(scene, utils::adjustFromPoint(connectionPoints.front(), kInterfaceTolerance),
                               { AADLFunctionGraphicsItem::Type });
    result.functionAtEndPos =
            utils::nearestItem(scene, utils::adjustFromPoint(connectionPoints.last(), kInterfaceTolerance),
                               { AADLFunctionGraphicsItem::Type });
    result.startObject = gi::functionObject(result.functionAtStartPos);
    result.endObject = gi::functionObject(result.functionAtEndPos);
    result.isToOrFromNested =
            (result.functionAtStartPos && result.functionAtStartPos->isAncestorOf(result.functionAtEndPos))
            || (result.functionAtEndPos && result.functionAtEndPos->isAncestorOf(result.functionAtStartPos));

    if (!result.startObject) {
        result.setFailed(FailReason::NoStartFunction);
        return result;
    }

    if (auto startIfaceItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                utils::nearestItem(scene, utils::adjustFromPoint(connectionPoints.front(), kInterfaceTolerance),
                                   { AADLInterfaceGraphicsItem::Type }))) {
        result.startIface = startIfaceItem->entity();
        result.startPointAdjusted = startIfaceItem->scenePos();
    } else if (!utils::intersects(result.functionAtStartPos->sceneBoundingRect(), result.connectionLine,
                                  &result.startPointAdjusted)) {
        result.setFailed(FailReason::CannotCreateStartIface);
        return result;
    }

    if (!result.endObject) {
        result.setFailed(FailReason::NoEndFunction);
        return result;
    }

    if (auto endIfaceItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                utils::nearestItem(scene, utils::adjustFromPoint(connectionPoints.last(), kInterfaceTolerance),
                                   { AADLInterfaceGraphicsItem::Type }))) {
        result.endIface = endIfaceItem->entity();
        result.endPointAdjusted = endIfaceItem->scenePos();
    } else if (!utils::intersects(result.functionAtEndPos->sceneBoundingRect(), result.connectionLine,
                                  &result.endPointAdjusted)) {
        result.setFailed(FailReason::CannotCreateEndIface);
        return result;
    }

    if (result.startIface && result.endIface && result.isToOrFromNested) {
        if (result.startIface->direction() != result.endIface->direction()) {
            result.setFailed(FailReason::ToFromNestedDifferentDirection);
            return result;
        }
    }

    if (result.startIface && result.endIface) {
        if (result.startIface->direction() == result.endIface->direction() && !result.isToOrFromNested) {
            result.setFailed(FailReason::SameDirectionIfaceWrongParents);
            return result;
        }
    }

    if (!result.startIface) {
        if (auto fn = result.startObject->as<const AADLObjectFunction *>())
            if (fn->instanceOf()) {
                result.setFailed(FailReason::DirectIfaceCreationInInstanceOfFunctionType);
                return result;
            }
    }

    if (!result.endIface) {
        if (auto fn = result.endObject->as<const AADLObjectFunction *>())
            if (fn->instanceOf()) {
                result.setFailed(FailReason::DirectIfaceCreationInInstanceOfFunctionType);
                return result;
            }
    }

    result.startIfaceId = result.startIface ? result.startIface->id() : common::createId();
    result.endIfaceId = result.endIface ? result.endIface->id() : common::createId();

    result.status = canConnect(result.startObject, result.endObject, result.startIface, result.endIface);
    return result;
}

ConnectionCreationValidator::FailReason ConnectionCreationValidator::canConnect(AADLObjectFunction *sourceFunction,
                                                                                AADLObjectFunction *targetFunction,
                                                                                AADLObjectIface *sourceIface,
                                                                                AADLObjectIface *targetIface)
{
    // Ensure that:
    // [1] - the edge functions are not FunctionType
    // [2] - the edge interfaces parents are not FunctionType
    // [3] - an iface kind is not Cyclic
    // [4] - parent of the source and target interface differs
    // [5] - if one is RI.inherited=false, the other should be PI with same kind and params

    // [1] - the edge functions are not FunctionType
    for (const AADLObjectFunction *function : { sourceFunction, targetFunction })
        if (function && function->isFunctionType())
            return FailReason::IsFunctionType;

    // [2] - the edge interfaces parents are not FunctionType
    for (const AADLObjectIface *iface : { sourceIface, targetIface })
        if (iface && iface->isNestedInFunctionType())
            return FailReason::ParentIsFunctionType;

    // [3] - an iface kind is not Cyclic
    for (const AADLObjectIface *iface : { sourceIface, targetIface })
        if (iface && iface->kind() == AADLObjectIface::OperationKind::Cyclic)
            return FailReason::IsCyclic;

    // [4] - parent of the source and target interface differs
    const AADLObject *srcParent =
            sourceFunction ? sourceFunction : (sourceIface ? sourceIface->parentObject() : nullptr);
    const AADLObject *dstParent =
            targetFunction ? targetFunction : (targetIface ? targetIface->parentObject() : nullptr);
    if ((srcParent || dstParent) && srcParent == dstParent)
        return FailReason::SameParent;

    // [5] - if one iface is RI, another should be PI with different name only
    const AADLObjectIfaceRequired *ri =
            AADLObjectConnection::selectIface<const AADLObjectIfaceRequired *>(sourceIface, targetIface);
    const AADLObjectIfaceProvided *pi =
            AADLObjectConnection::selectIface<const AADLObjectIfaceProvided *>(sourceIface, targetIface);

    if (ri) {

#ifndef AADL_MULTICAST_CONNECTION
        if (AADLObjectsModel *model = ri->objectsModel()) {
            const QVector<AADLObjectConnection *> riConnections = model->getConnectionsForIface(ri->id());
            for (const AADLObjectConnection *riConnection : riConnections)
                if ((riConnection->sourceInterface() && riConnection->sourceInterface()->isProvided())
                    || (riConnection->targetInterface() && riConnection->targetInterface()->isProvided()))
                    return FailReason::MulticastDisabled;
        }
#endif // AADL_MULTICAST_CONNECTION

        if (pi) {

            if (!ri->inheritPi()) {
                if (ri->kind() != pi->kind())
                    return FailReason::KindDiffer;

                const QVector<IfaceParameter> &riParams = ri->params();
                const QVector<IfaceParameter> &piParams = pi->params();
                if (riParams.size() != piParams.size())
                    return FailReason::ParamsDiffer;

                for (const IfaceParameter &param : riParams)
                    if (!piParams.contains(param))
                        return FailReason::ParamsDiffer;
            }
        }
    }

    return FailReason::NotFail;
}

} // namespace aadl
} // namespace taste3
