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

#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivmodel.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtMath>

namespace ivm {

/*!
 * \class ivm::ConnectionCreationValidator
 * The collection of static functions to detect if it's possible to connect two interfaces/scene points.
 */

/*!
    \enum class ivm::ConnectionCreationValidator::FailReason
    This enum type specifies a reason of why the connection creation is prohibited:

        \value NotFail
            Not prohibited, the connection can be setup for checked interfaces/points.
        \value IsFunctionType
            The start or end item is the AADLFunctionTypeGraphicsItem.
        \value MulticastDisabled
            Attempt to connect the same RI to more than one PI.
        \value KindDiffer
            No one of interfaces neither has kind AADLIface::OperationKind::Any, nor their kinds are the same.
        \value ParamsDiffer
            The checked interfaces have different parameters and no one is a RI with InheritPi property set.
        \value ParentIsFunctionType
            Attempt to directly connect a AADLFunctionTypeGraphicsItem.
        \value IsCyclic
            Attempt to make a connection with a PI which kind is AADLIface::OperationKind::Cyclic.
        \value SameParent
            The source and target AADLFunctionGraphicsItem is the same instance.
        \value SameInterface
            The source and target AADLInterfaceGraphicsItem is the same instance.
        \value NoScene
            Invalid pointer to the QGraphicsScene. The default initial value for ValidationResult::status.
        \value NoStartFunction
            Can not detect a AADLFunctionGraphicsItem at specified start position.
        \value CannotCreateStartIface
            The predicted AADLFunctionGraphicsItem to create Interface in can not be found in specified position.
        \value NoEndFunction
            Can not detect a AADLFunctionGraphicsItem at specified end position.
        \value CannotCreateEndIface
            The predicted AADLFunctionGraphicsItem to create Interface in can not be found in specified position.
        \value SameDirectionIfaceWrongParents
            Attempt to connect PI to PI or RI to RI when parent Functions are not direct ancestors.
        \value ToFromNestedDifferentDirection
            Attempt to connect PI to RI when parent AADLFunctionGraphicsItems are direct ancestors.
        \value DirectIfaceCreationInInstanceOfFunctionType
            The start or end item is the AADLFunctionGraphicsItem which is an instance of AADLFunctionTypeGraphicsItem.
 */

/*!
 * \brief Check if the \a sourceFunction which contains the \a sourceIface
 * can be used to setup a connection to the \a targetFunction winch contains the \a targetIface.
 *
 * Ensure that:
 * 1. the items on edges are not the AADLFunctionTypeGraphicsItem;
 * 2. interface parents are not the AADLFunctionTypeGraphicsItem
 * 3. an iface kind is not IVInterface::OperationKind::Cyclic;
 * 4. parent of the source and target interface differs;
 * 5. both ifaces are either (PI+RI|PI+PI|RI+RI) compatible by kind and params, or PI+RI.inheritPI=true;
 * 6. the RI is not connected to any other RIs;
 *
 * Returns the status of such check as ConnectionCreationValidator::FailReason.
 * Anything except the FailReason::NotFail means that the connection creation is prohibited.
 */
ConnectionCreationValidator::FailReason ConnectionCreationValidator::canConnect(
        IVFunction *sourceFunction, IVFunction *targetFunction, IVInterface *sourceIface, IVInterface *targetIface)
{
    // [1] - the edge functions are not FunctionType
    for (const IVFunction *function : { sourceFunction, targetFunction })
        if (function && function->isFunctionType())
            return FailReason::IsFunctionType;

    // [2] - the edge interfaces parents are not FunctionType
    for (const IVInterface *iface : { sourceIface, targetIface })
        if (iface && iface->isNestedInFunctionType())
            return FailReason::ParentIsFunctionType;

    // [3] - an iface kind is not Cyclic
    for (const IVInterface *iface : { sourceIface, targetIface })
        if (iface && iface->kind() == IVInterface::OperationKind::Cyclic)
            return FailReason::IsCyclic;

    // [4] - parent of the source and target interface differs
    const IVObject *srcParent =
            sourceFunction ? sourceFunction : (sourceIface ? sourceIface->parentObject() : nullptr);
    const IVObject *dstParent =
            targetFunction ? targetFunction : (targetIface ? targetIface->parentObject() : nullptr);
    if ((srcParent || dstParent) && srcParent == dstParent)
        return FailReason::SameParent;

    // [7] - Source and Target is the same interface.
    if (sourceIface == targetIface && sourceIface && targetIface)
        return FailReason::SameInterface;

    // [6] - Multicast is disabled atm
    if (sourceIface) {
        if (auto model = sourceIface->model()) {
            for (const auto connection : model->getConnectionsForIface(sourceIface->id())) {
                if (connection->sourceInterface()->id() == sourceIface->id()) {
                    return FailReason::MulticastDisabled;
                }
            }
        }
    }

    // [5] - both ifaces should be either (PI+RI|PI+PI|RI+RI) compatible by kind and params, or PI+RI.inheritPI=true
    if (sourceIface && targetIface)
        return checkKindAndParams(sourceIface, targetIface);

    return FailReason::NotFail;
}

/*!
 * \brief Check if the \a sourceIface and \a targetIface:
 * have the same kind (or the kind of at least one of them is IVInterface::OperationKind::Any) and
 * have the same parameters,
 * or they are the PI and RI and the RI has InheritPI property set to true.
 * In case the passed interfaces are of different directions (PI and RI),
 * the RI is also checked to be not connected to other PIs to avoid multicast connections.
 * Returns ConnectionCreationValidator::FailReason, anything except the FailReason::NotFail
 * means that the connection creation is prohibited.
 */
ConnectionCreationValidator::FailReason ConnectionCreationValidator::checkKindAndParams(
        IVInterface *sourceIface, IVInterface *targetIface)
{
    if (auto ri = IVConnection::selectIface<const IVInterfaceRequired *>(sourceIface, targetIface))
        if (IVConnection::selectIface<const IVInterfaceProvided *>(sourceIface, targetIface)) {
#ifndef AADL_MULTICAST_CONNECTION
            if (IVModel *model = ri->model()) {
                const QVector<IVConnection *> riConnections = model->getConnectionsForIface(ri->id());
                for (const IVConnection *riConnection : riConnections)
                    if ((riConnection->sourceInterface() && riConnection->sourceInterface()->isProvided())
                            || (riConnection->targetInterface() && riConnection->targetInterface()->isProvided()))
                        return FailReason::MulticastDisabled;
            }
#endif // AADL_MULTICAST_CONNECTION

            if (ri->isInheritPI())
                return FailReason::NotFail;
        }

    const bool weakKind = sourceIface->kind() == IVInterface::OperationKind::Any
            || targetIface->kind() == IVInterface::OperationKind::Any;
    if (!weakKind && sourceIface->kind() != targetIface->kind())
        return FailReason::KindDiffer;

    if (sourceIface->params() != targetIface->params())
        return FailReason::ParamsDiffer;

    return FailReason::NotFail;
}

}
