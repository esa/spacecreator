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
    ValidationResult info;

    if (scene) {
        info.status = validatePreview(scene, connectionPoints);
        if (!info.failed())
            info = validateCreate(scene, connectionPoints);
    }

    qDebug() << info.status;

    return info;
}

ConnectionCreationValidator::FailReason
ConnectionCreationValidator::validatePreview(QGraphicsScene *scene, const QVector<QPointF> &connectionPoints)
{
    // Ensure that:
    // [1] - the pos is not in a FunctionType
    // [2] - the pos is not in an Iface which is in Function type
    // [3] - the pos is not in an Iface which kind is Cyclic
    // [4] - parent of the source and target interface differs
    // [5] - if one iface is RI, another should be PI with different name only

    ConnectionCreationValidator::ValidationResult result;

    auto ifaceAt = [scene](const QPointF &pos) -> AADLObjectIface * {
        if (QGraphicsItem *item =
                    utils::nearestItem(scene, pos, kConnectionTolerance, { AADLInterfaceGraphicsItem::Type }))
            return gi::interfaceObject(item);
        return nullptr;
    };

    auto functionIn = [scene](const QPointF &pos) -> AADLObjectFunctionType * {
        if (QGraphicsItem *item =
                    utils::nearestItem(scene, pos, kConnectionTolerance, { AADLFunctionGraphicsItem::Type }))
            return gi::functionObject(item);
        return nullptr;
    };

    // [1]
    auto isFunctionType = [functionIn, scene](const QPointF &pos) {
        if (AADLObjectFunctionType *fun = functionIn(pos))
            if (fun->isFunctionType())
                return FailReason::IsFunctionType;

        if (QGraphicsItem *item =
                    utils::nearestItem(scene, pos, kConnectionTolerance, { AADLFunctionTypeGraphicsItem::Type })) {
            if (AADLObjectFunctionType *fun = gi::functionTypeObject(item))
                if (fun->isFunctionType())
                    return FailReason::IsFunctionType;
        }

        return FailReason::NotFail;
    };

    auto checkIfaces = [](const AADLObjectIface *source, const AADLObjectIface *target) {
        const AADLObjectIfaceRequired *ri =
                AADLObjectConnection::selectIface<const AADLObjectIfaceRequired *>(source, target);
        const AADLObjectIfaceProvided *pi =
                AADLObjectConnection::selectIface<const AADLObjectIfaceProvided *>(source, target);
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
                    if (ri->kind() != pi->kind()) {
                        return FailReason::KindDiffer;
                    }

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
    };

    auto ifacesConnectable = [functionIn, checkIfaces, ifaceAt, &result](const QPointF &pntA, const QPointF &pntB) {
        const AADLObjectIface *source = ifaceAt(pntA);
        const AADLObjectIface *target = ifaceAt(pntB);
        auto validIface = [](const AADLObjectIface *iface, const AADLObjectFunctionType *ifaceParent) {
            if (iface) {
                // [2]
                if (ifaceParent && ifaceParent->isFunctionType())
                    return FailReason::ParentIsFunctionType;
                // [3]
                if (iface->kind() == AADLObjectIface::OperationKind::Cyclic)
                    return FailReason::IsCyclic;
            }
            return FailReason::NotFail;
        };

        AADLObjectFunctionType *targetParent =
                target ? qobject_cast<AADLObjectFunctionType *>(target->parent()) : functionIn(pntB);
        AADLObjectFunctionType *sourceParent =
                source ? qobject_cast<AADLObjectFunctionType *>(source->parent()) : functionIn(pntA);

        if (result.setFailed(validIface(target, targetParent)))
            return result.status;
        if (result.setFailed(validIface(source, sourceParent)))
            return result.status;

        // [4]
        if (sourceParent && sourceParent == targetParent)
            return FailReason::SameParent;

        // [5]
        if (source && target)
            return checkIfaces(source, target);

        return FailReason::NotFail;
    };

    if (result.setFailed(isFunctionType(connectionPoints.back())))
        return result.status;

    if (result.setFailed(ifacesConnectable(connectionPoints.front(), connectionPoints.back())))
        return result.status;

    return FailReason::NotFail;
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
    result.startRequired = true;
    result.isSameType = (result.functionAtStartPos && result.functionAtStartPos->isAncestorOf(result.functionAtEndPos))
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

    if (result.startIface && result.endIface) {
        if (result.startIface->direction() == result.endIface->direction() && !result.isSameType) {
            result.setFailed(FailReason::SameDirectionIfaceWrongParents);
            return result;
        }

        result.startRequired = result.startIface->isRequired();
    }

    result.setFailed(FailReason::NotFail);
    return result;
}

} // namespace aadl
} // namespace taste3
