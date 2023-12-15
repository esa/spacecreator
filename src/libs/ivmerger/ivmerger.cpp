/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2023 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "ivmerger.h"

#include "errorhub.h"
#include "fullivexporter.h"
#include "geometry.h"
#include "graphicsviewutils.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivfunctiontype.h"
#include "ivpropertytemplateconfig.h"

#include <QDebug>
#include <QFileInfo>
#include <QMap>
#include <QPointF>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <ivcore/ivxmlreader.h>
#include <ivcore/ivxmlwriter.h>
#include <limits>

namespace ivmerger {

bool IvMerger::mergeInterfaceViews(const QString &inputTargetIvFile, const QString &inputSourceIvFile)
{
    ivm::IVPropertyTemplateConfig *conf = ivm::IVPropertyTemplateConfig::instance();
    conf->init(QLatin1String("default_attrinbutes.xml"));
    ivm::IVModel targetIvModel(conf);
    ivm::IVModel sourceIvModel(conf);

    if (!parseInterfaceView(&targetIvModel, inputTargetIvFile)) {
        shared::ErrorHub::setCurrentFile(inputTargetIvFile);
        shared::ErrorHub::addError(shared::ErrorItem::Error, "Error parsing target interface view", inputTargetIvFile);
        shared::ErrorHub::clearCurrentFile();
        qDebug() << "Cannot read target IV";
        return false;
    }

    if (!parseInterfaceView(&sourceIvModel, inputSourceIvFile)) {
        shared::ErrorHub::setCurrentFile(inputSourceIvFile);
        shared::ErrorHub::addError(shared::ErrorItem::Error, "Error parsing target interface view", inputSourceIvFile);
        shared::ErrorHub::clearCurrentFile();
        qDebug() << "Cannot read source IV";
        return false;
    }

    if (!mergeInterfaceViews(targetIvModel, sourceIvModel)) {
        return false;
    }

    if (!saveInterfaceView(&targetIvModel, inputTargetIvFile)) {
        shared::ErrorHub::setCurrentFile(inputTargetIvFile);
        shared::ErrorHub::addError(shared::ErrorItem::Error, "Error saving merged interface view", inputTargetIvFile);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    return true;
}

bool IvMerger::mergeInterfaceViews(ivm::IVModel &targetIvModel, ivm::IVModel &sourceIvModel)
{
    if (sourceIvModel.allObjectsByType<ivm::IVFunctionType>().empty()) {
        shared::ErrorHub::addError(
                shared::ErrorItem::Error, "Source Interface View file does not contain any functions");
        return false;
    }

    QMap<ivm::IVFunctionType *, FunctionEndpoints> connectionsToRestore;

    // find top level functions in target
    QVector<ivm::IVFunctionType *> targetFunctions = targetIvModel.allObjectsByType<ivm::IVFunctionType>();
    QVector<ivm::IVFunctionType *> topLevelTargetFunctions;
    collectTopLevelFunctions(targetFunctions, topLevelTargetFunctions);

    // find top level functions in source
    QSet<ivm::IVFunctionType *> insertedSourceFunctions;
    QVector<ivm::IVFunctionType *> sourceFunctions = sourceIvModel.allObjectsByType<ivm::IVFunctionType>();
    QVector<ivm::IVFunctionType *> topLevelSourceFunctions;
    collectTopLevelFunctions(sourceFunctions, topLevelSourceFunctions);

    mergeFunctions(targetIvModel, sourceIvModel, topLevelSourceFunctions, targetFunctions, connectionsToRestore,
            insertedSourceFunctions);

    addNewFunctions(targetIvModel, sourceIvModel, topLevelSourceFunctions, topLevelTargetFunctions, connectionsToRestore,
            insertedSourceFunctions);

    // calculate again set of all top level functions in target
    targetFunctions = targetIvModel.allObjectsByType<ivm::IVFunctionType>();
    topLevelTargetFunctions.clear();
    collectTopLevelFunctions(targetFunctions, topLevelTargetFunctions);

    restoreConnections(targetIvModel, topLevelTargetFunctions, connectionsToRestore);

    return true;
}

void IvMerger::collectTopLevelFunctions(
        QVector<ivm::IVFunctionType *> &allFunctions, QVector<ivm::IVFunctionType *> &topLevelFunctions)
{
    for (ivm::IVFunctionType *function : allFunctions) {
        if (!function->isNested()) {
            topLevelFunctions.append(function);
        }
    }
}

void IvMerger::mergeFunctions(ivm::IVModel &targetIvModel, ivm::IVModel &sourceIvModel,
        QVector<ivm::IVFunctionType *> &topLevelSourceFunctions, QVector<ivm::IVFunctionType *> &targetFunctions,
        QMap<ivm::IVFunctionType *, FunctionEndpoints> &connectionsToRestore,
        QSet<ivm::IVFunctionType *> &insertedSourceFunctions)

{
    // get functions that will not be replaced
    QVector<ivm::IVFunctionType *> functionsThatWillNotBeReplaced;
    for (ivm::IVFunctionType *sourceFunction : topLevelSourceFunctions) {
        for (ivm::IVFunctionType *targetFunction : targetFunctions) {
            if (!sourceFunction->isNested() && sourceFunction->id() != targetFunction->id()) {
                functionsThatWillNotBeReplaced.append(sourceFunction);
            }
        }
    }

    // replace top level functions in target by coresponding top level functions from source
    for (ivm::IVFunctionType *sourceFunction : topLevelSourceFunctions) {
        for (ivm::IVFunctionType *targetFunction : targetFunctions) {
            if (sourceFunction->id() == targetFunction->id()) {
                FunctionEndpoints connectionInfos;

                replaceFunction(targetIvModel, sourceIvModel, targetFunction, sourceFunction, connectionInfos, functionsThatWillNotBeReplaced);

                // remember connections to restore it later
                connectionsToRestore.insert(sourceFunction, connectionInfos);

                // mark target function as inserted
                insertedSourceFunctions.insert(sourceFunction);
            }
        }
    }
}

void IvMerger::addNewFunctions(ivm::IVModel &targetIvModel, ivm::IVModel &sourceIvModel,
        QVector<ivm::IVFunctionType *> &topLevelSourceFunctions, QVector<ivm::IVFunctionType *> &topLevelTargetFunctions,
        QMap<ivm::IVFunctionType *, FunctionEndpoints> &connectionsToRestore,
        QSet<ivm::IVFunctionType *> &insertedSourceFunctions)
{
    // find place to insert new functions (if necessary)
    qreal rightBorder = std::numeric_limits<double>::min();
    qreal topBorder = std::numeric_limits<double>::max();
    const QString coordToken = ivm::meta::Props::token(ivm::meta::Props::Token::coordinates);
    for (ivm::IVFunctionType *function : topLevelTargetFunctions) {
        const QString coordStr = function->entityAttributeValue<QString>(coordToken);

        const QRectF rect = shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(coordStr));
        rightBorder = std::max(rightBorder, rect.right());
        topBorder = std::min(topBorder, rect.top());
    }

    if (rightBorder == std::numeric_limits<double>::min()) {
        rightBorder = 0.0;
    }
    if (topBorder == std::numeric_limits<double>::max()) {
        topBorder = 0.0;
    }

    QVector<ivm::IVComment *> targetComments = targetIvModel.allObjectsByType<ivm::IVComment>();

    for (ivm::IVComment *comment : targetComments) {
        const QString coordStr = comment->entityAttributeValue<QString>(coordToken);
        const QRectF rect = shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(coordStr));
        rightBorder = std::max(rightBorder, rect.right());
        topBorder = std::min(topBorder, rect.top());
    }

    // sort top level source functions on x postion
    std::sort(topLevelSourceFunctions.begin(),
          topLevelSourceFunctions.end(),
          [](const ivm::IVFunctionType* a, const ivm::IVFunctionType* b) 
          { 
              QRectF coordinatesA = shared::graphicsviewutils::rect(a->coordinates());
              QRectF coordinatesB = shared::graphicsviewutils::rect(b->coordinates());
              return coordinatesA.x() < coordinatesB.x(); 
          });

    // insert remaining top level functions from source to the target
    for (ivm::IVFunctionType *sourceFunction : topLevelSourceFunctions) {
        if (insertedSourceFunctions.contains(sourceFunction)) {
            continue; // skip already inserted functions
        }

        // remove external connections in function from  source
        FunctionEndpoints connectionInfos;
        QVector<ivm::IVConnection *> sourceConnections = sourceIvModel.getConnectionsForFunction(sourceFunction->id());
        for (ivm::IVConnection *connection : sourceConnections) {
            if (!connection->isNested()) {
                sourceIvModel.removeObject(connection);
                connectionInfos.insert(qMakePair(connection->source()->id(), connection->sourceInterfaceName()),
                        qMakePair(connection->target()->id(), connection->targetInterfaceName()));
            }
        }

        // reparent sourceFunction into targetIvModel
        reparentRecursive(targetIvModel, sourceFunction);

        // set new coordinates of sourceFunction
        QRectF coordinates = shared::graphicsviewutils::rect(sourceFunction->coordinates());
        qreal xIfaceOffset = (rightBorder + MARGIN) - coordinates.x();
        qreal yIfaceOffset = topBorder - coordinates.y();
        QVector<ivm::IVInterface *> interfaces = sourceFunction->interfaces();
        for (ivm::IVInterface *iface : interfaces) {
            QPointF ifacePos = shared::graphicsviewutils::pos(iface->coordinates());
            ifacePos += QPointF(xIfaceOffset, yIfaceOffset);
            iface->setCoordinates(shared::graphicsviewutils::coordinates(ifacePos));
        }

        coordinates.moveTo(rightBorder + MARGIN, topBorder);
        sourceFunction->setCoordinates(shared::graphicsviewutils::coordinates(coordinates));

        // update rightBorder
        rightBorder += MARGIN + coordinates.width();

        connectionsToRestore.insert(sourceFunction, connectionInfos);
    }
}

void IvMerger::replaceFunction(ivm::IVModel &ivModel, ivm::IVModel &sourceIvModel, ivm::IVFunctionType *currentFunction,
        ivm::IVFunctionType *newFunction, FunctionEndpoints &connectionInfos, QVector<ivm::IVFunctionType *> functionsThatWillNotBeReplaced)
{
    QVector<ivm::IVConnection *> targetConnections = ivModel.getConnectionsForFunction(currentFunction->id());

    // remove all connections from/to target top level functions
    // but save information for later
    for (ivm::IVConnection *connection : targetConnections) {
        if (!connection->isNested()) {
            connectionInfos.insert(qMakePair(connection->source()->id(), connection->sourceInterfaceName()),
                    qMakePair(connection->target()->id(), connection->targetInterfaceName()));
            ivModel.removeObject(connection);
        }
    }

    // remove external connections from source
    QVector<ivm::IVConnection *> sourceConnections = sourceIvModel.getConnectionsForFunction(newFunction->id());
    for (ivm::IVConnection *connection : sourceConnections) {

        bool isConnectionToNewFunctionToBeAdded = false;
        for (ivm::IVFunctionType * newFunctionToBeAdded : functionsThatWillNotBeReplaced){
            if (connection->source()->id() == newFunctionToBeAdded->id() || connection->target()->id() == newFunctionToBeAdded->id())
            {
                isConnectionToNewFunctionToBeAdded = true;
                break;
            }
        }

        if (!connection->isNested() && !isConnectionToNewFunctionToBeAdded) {
            sourceIvModel.removeObject(connection);
        }
    }

    // get coordinates of top level function in target
    QRectF targetPosition = shared::graphicsviewutils::rect(currentFunction->coordinates());
    QRectF sourcePosition = shared::graphicsviewutils::rect(newFunction->coordinates());
    // calculate offset
    qreal xIfaceOffset = targetPosition.x() - sourcePosition.x();
    qreal yIfaceOffset = targetPosition.y() - sourcePosition.y();

    // remove target function
    removeFunctionAndItsChildren(ivModel, currentFunction);

    // move source function into position where target function was located
    // (this includes also coordinates)
    QVector<ivm::IVInterface *> interfaces = newFunction->interfaces();
    for (ivm::IVInterface *iface : interfaces) {
        QPointF ifacePos = shared::graphicsviewutils::pos(iface->coordinates());
        ifacePos += QPointF(xIfaceOffset, yIfaceOffset);
        iface->setCoordinates(shared::graphicsviewutils::coordinates(ifacePos));
    }
    sourcePosition.moveTo(targetPosition.x(), targetPosition.y());

    // reparent sourceFunction into targetIvModel
    reparentRecursive(ivModel, newFunction);

    // set updated coordinates of top function from source
    newFunction->setCoordinates(shared::graphicsviewutils::coordinates(sourcePosition));
}

void IvMerger::restoreConnections(ivm::IVModel &ivModel, QVector<ivm::IVFunctionType *> &allFunctions,
        QMap<ivm::IVFunctionType *, FunctionEndpoints> connectionsToRestore)
{
    // restore connections between inserted functions and user functions
    for (auto iter = connectionsToRestore.begin(); iter != connectionsToRestore.end(); ++iter) {
        ivm::IVFunctionType *sourceFunction = iter.key();
        const FunctionEndpoints &connectionInfos = iter.value();
        // iterate over saved connection infos
        for (auto infoIter = connectionInfos.begin(); infoIter != connectionInfos.end(); ++infoIter) {
            if (infoIter.key().first == sourceFunction->id()) {
                // find toplevel
                for (ivm::IVFunctionType *functionToConnect : allFunctions) {
                    if (infoIter.value().first == functionToConnect->id()) {
                        realizeConnection(ivModel, allFunctions, sourceFunction, infoIter.key().second,
                                functionToConnect, infoIter.value().second);
                    }
                }
            } else if (infoIter.value().first == sourceFunction->id()) {
                for (ivm::IVFunctionType *functionToConnect : allFunctions) {
                    if (infoIter.key().first == functionToConnect->id()) {
                        realizeConnection(ivModel, allFunctions, functionToConnect, infoIter.key().second,
                                sourceFunction, infoIter.value().second);
                    }
                }
            }
        }
    }
}

void IvMerger::removeFunctionAndItsChildren(ivm::IVModel &ivModel, ivm::IVFunctionType *targetFunctionToRemove)
{
    for (ivm::IVObject *child : targetFunctionToRemove->children()) {
        ivModel.removeObject(child);
    }

    ivModel.removeObject(targetFunctionToRemove);
}

void IvMerger::reparentRecursive(ivm::IVModel &newParent, ivm::IVObject *obj)
{
    if (obj->isFunctionType()) {
        ivm::IVFunctionType *func = obj->as<ivm::IVFunctionType *>();
        QVector<ivm::IVObject *> children = func->children();
        for (ivm::IVObject *child : children) {
            reparentRecursive(newParent, child);
        }
    }

    newParent.addObject(obj);
}

void IvMerger::realizeConnection(ivm::IVModel &ivModel, const QVector<ivm::IVFunctionType *> allTopLevelFunctions,
        ivm::IVFunctionType *fromFunction, const QString &fromInterfaceName, ivm::IVFunctionType *toFunction,
        const QString &toInterfaceName)
{
    QList<QRectF> sibilingRects;
    for (ivm::IVFunctionType *targetFunction : allTopLevelFunctions) {
        sibilingRects.append(shared::graphicsviewutils::rect(targetFunction->coordinates()));
    }
    ivm::IVInterface *fromInterface = fromFunction->getInterfaceByName(fromInterfaceName);
    if (fromInterface == nullptr) {
        // cannot realize connection interface removed in newer version of function
        return;
    }
    ivm::IVInterface *toInterface = toFunction->getInterfaceByName(toInterfaceName);
    if (toInterface == nullptr) {
        // cannot realize connection interface removed in newer version of function
        return;
    }
    const QPointF startPos = shared::graphicsviewutils::pos(fromInterface->coordinates());
    const QPointF endPos = shared::graphicsviewutils::pos(toInterface->coordinates());
    const QRectF startRect = shared::graphicsviewutils::rect(fromFunction->coordinates());
    const QRectF endRect = shared::graphicsviewutils::rect(toFunction->coordinates());
    const QVector<QPointF> points =
            shared::graphicsviewutils::createConnectionPath(sibilingRects, startPos, startRect, endPos, endRect);
    ivm::IVConnection *newConnection = new ivm::IVConnection(fromInterface, toInterface);
    ivModel.addObject(newConnection);
    newConnection->setCoordinates(shared::graphicsviewutils::coordinates(points));
}

bool IvMerger::parseInterfaceView(ivm::IVModel *model, const QString &inputIvFile)
{
    if (inputIvFile.isEmpty() || !QFileInfo::exists(inputIvFile)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, "Input file does not exist", inputIvFile);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    shared::ErrorHub::setCurrentFile(inputIvFile);
    ivm::IVXMLReader parser;
    if (!parser.readFile(inputIvFile)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), inputIvFile);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    QVector<ivm::IVObject *> objects = parser.parsedObjects();
    ivm::IVObject::sortObjectList(objects);

    model->clear();
    model->initFromObjects(objects, parser.externalAttributes());
    shared::ErrorHub::clearCurrentFile();

    return true;
}

bool IvMerger::saveInterfaceView(ivm::IVModel *model, const QString &outputIvFile)
{
    FullIVExporter exporter(model);
    return exporter.doExport(outputIvFile);
}

} // namespace sdl::exporter
