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
#include "geometry.h"
#include "graphicsviewutils.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivfunctiontype.h"
#include "ivpropertytemplateconfig.h"

#include <QFileInfo>
#include <QPointF>
#include <algorithm>
#include <iostream>
#include <ivcore/ivxmlreader.h>
#include <ivcore/ivxmlwriter.h>
#include <limits>

namespace ivmerger {

bool IvMerger::mergeInterfaceViews(const QString &inputTargetIvFile, const QString &inputSourceIvFile)
{
    ivm::IVModel targetIvModel(ivm::IVPropertyTemplateConfig::instance());
    ivm::IVModel sourceIvModel(ivm::IVPropertyTemplateConfig::instance());

    if (!parseInterfaceView(&targetIvModel, inputTargetIvFile)) {
        shared::ErrorHub::setCurrentFile(inputTargetIvFile);
        shared::ErrorHub::addError(shared::ErrorItem::Error, "Error parsing target interface view", inputTargetIvFile);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    if (!parseInterfaceView(&sourceIvModel, inputSourceIvFile)) {
        shared::ErrorHub::setCurrentFile(inputSourceIvFile);
        shared::ErrorHub::addError(shared::ErrorItem::Error, "Error parsing target interface view", inputSourceIvFile);
        shared::ErrorHub::clearCurrentFile();
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
        shared::ErrorHub::addError(shared::ErrorItem::Error,
                "Source Interface View file does not contain any functions");
        return false;
    }

    removeDuplicateObjects(targetIvModel, sourceIvModel);

    QRectF targetRectContainingAllObjects = getRectContainingAllObjects(targetIvModel);
    QRectF sourceRectContainingAllObjects = getRectContainingAllObjects(sourceIvModel);

    qreal verticalOffset = targetRectContainingAllObjects.top() - sourceRectContainingAllObjects.top();
    qreal horizontalOffset =
            targetRectContainingAllObjects.left() - sourceRectContainingAllObjects.right() - MARGIN;

    QVector<shared::VEObject *> allSourceObjects = sourceIvModel.objects().values();

    moveAllObjects(allSourceObjects, verticalOffset, horizontalOffset);

    targetIvModel.addObjects(allSourceObjects);

    return true;
}

void IvMerger::removeDuplicateObjects(ivm::IVModel &targetIvModel, const ivm::IVModel &sourceIvModel)
{
    QVector<ivm::IVFunctionType *> allTargetFunctions = targetIvModel.allObjectsByType<ivm::IVFunctionType>();
    QVector<ivm::IVFunctionType *> allSourceFunctions = sourceIvModel.allObjectsByType<ivm::IVFunctionType>();

    for (ivm::IVFunctionType *sourceFunction : allSourceFunctions) {
        for (ivm::IVFunctionType *targetFunction : allTargetFunctions) {
            if (sourceFunction->id() == targetFunction->id()) {
                replaceConnectionsEndpoints(targetIvModel, targetFunction, sourceFunction);
                removeFunctionAndItsChildren(targetIvModel, targetFunction);
            }
        }
    }

    // Remove rest of objects
    for (shared::VEObject *sourceObject : sourceIvModel.objects().values()) {
        for (shared::VEObject *targetObject : targetIvModel.objects().values()) {
            if (sourceObject->id() == targetObject->id()) {
                targetIvModel.removeObject(targetObject);
            }
        }
    }

    // Remove dangling connections
    for (ivm::IVConnection *connection : targetIvModel.allObjectsByType<ivm::IVConnection>()) {
        if (connection->source() == nullptr || connection->target() == nullptr) {
            targetIvModel.removeObject(connection);
        }
    }
}

void IvMerger::replaceConnectionsEndpoints(
        ivm::IVModel &ivModel, ivm::IVFunctionType *targetFunctionToRemove, ivm::IVFunctionType *sourceFunction)
{
    QVector<ivm::IVConnection *> allConnections = ivModel.allObjectsByType<ivm::IVConnection>();

    for (ivm::IVConnection *connection : ivModel.allObjectsByType<ivm::IVConnection>()) {
        if (connection->source() != nullptr && connection->source()->id() == targetFunctionToRemove->id()) {
            ivm::IVInterface *interfaceToReplace =
                    sourceFunction->getInterfaceByName(connection->sourceInterfaceName());
            if (interfaceToReplace != nullptr) {
                connection->setData(interfaceToReplace, connection->targetInterface());
            }
        }
        if (connection->target() != nullptr && connection->target()->id() == targetFunctionToRemove->id()) {
            ivm::IVInterface *interfaceToReplace =
                    sourceFunction->getInterfaceByName(connection->targetInterfaceName());
            if (interfaceToReplace != nullptr) {
                connection->setData(connection->sourceInterface(), interfaceToReplace);
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

QRectF IvMerger::getRectContainingAllObjects(const ivm::IVModel &ivModel)
{
    QVector<ivm::IVFunctionType *> allModelFunctions = ivModel.allObjectsByType<ivm::IVFunctionType>();
    QVector<ivm::IVComment *> allModelComments = ivModel.allObjectsByType<ivm::IVComment>();

    if (allModelFunctions.isEmpty() && allModelComments.isEmpty()) {
        return QRectF(QPointF(0.0, 0.0), QPointF(0.0, 0.0));
    }

    qreal leftBorder = std::numeric_limits<double>::max();
    qreal topBorder = std::numeric_limits<double>::min();
    qreal rightBorder = std::numeric_limits<double>::min();
    qreal bottomBorder = std::numeric_limits<double>::max();

    const QString coordToken = ivm::meta::Props::token(ivm::meta::Props::Token::coordinates);

    for (auto function : allModelFunctions) {
        const QString coordStr = function->entityAttributeValue<QString>(coordToken);
        const QRectF rect = shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(coordStr));

        leftBorder = std::min(leftBorder, rect.left());
        topBorder = std::max(topBorder, rect.top());
        rightBorder = std::max(rightBorder, rect.right());
        bottomBorder = std::min(bottomBorder, rect.bottom());
    }

    for (auto comment : allModelComments) {
        const QString coordStr = comment->entityAttributeValue<QString>(coordToken);
        const QRectF rect = shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(coordStr));

        leftBorder = std::min(leftBorder, rect.left());
        topBorder = std::max(topBorder, rect.top());
        rightBorder = std::max(rightBorder, rect.right());
        bottomBorder = std::min(bottomBorder, rect.bottom());
    }

    return QRectF(QPointF(leftBorder, topBorder), QPointF(rightBorder, bottomBorder));
}

// DODAĆ KOMENTY

void IvMerger::moveAllObjects(
        QVector<shared::VEObject *> &objects, const qreal verticalOffset, const qreal horizontalOffset)
{
    const QString coordToken = ivm::meta::Props::token(ivm::meta::Props::Token::coordinates);

    for (auto object : objects) {
        ivm::IVObject *ivObject = object->as<ivm::IVObject *>();
        if (ivObject == nullptr) {
            continue;
        }

        switch (ivObject->type()) {
        case ivm::IVObject::Type::Function:
        case ivm::IVObject::Type::FunctionType:
        case ivm::IVObject::Type::Comment: {
            QString coordStr = ivObject->entityAttributeValue<QString>(coordToken);
            QRectF rect = shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(coordStr));

            rect.setLeft(rect.left() + horizontalOffset);
            rect.setTop(rect.top() + verticalOffset);
            rect.setRight(rect.right() + horizontalOffset);
            rect.setBottom(rect.bottom() + verticalOffset);

            ivObject->setCoordinates(shared::graphicsviewutils::coordinates(rect));
            break;
        }
        case ivm::IVObject::Type::RequiredInterface:
        case ivm::IVObject::Type::ProvidedInterface:
        case ivm::IVObject::Type::InterfaceGroup: {
            const QString coordStr = ivObject->entityAttributeValue<QString>(coordToken);
            QPointF point = shared::graphicsviewutils::pos(ivm::IVObject::coordinatesFromString(coordStr));

            point.setY(point.y() + verticalOffset);
            point.setX(point.x() + horizontalOffset);

            ivObject->setCoordinates(shared::graphicsviewutils::coordinates(point));
            break;
        }
        case ivm::IVObject::Type::Connection:
        case ivm::IVObject::Type::ConnectionGroup: {
            const QString coordStr = ivObject->entityAttributeValue<QString>(coordToken);
            QVector<QPointF> polygon =
                    shared::graphicsviewutils::polygon(ivm::IVObject::coordinatesFromString(coordStr));

            for (auto point : polygon) {
                point.setY(point.y() + verticalOffset);
                point.setX(point.x() + horizontalOffset);
            }

            ivObject->setCoordinates(shared::graphicsviewutils::coordinates(polygon));
            break;
        }
        default:
            break;
        }
    }
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

    model->setExtAttributes(parser.externalAttributes());
    model->addObjects(objects);
    shared::ErrorHub::clearCurrentFile();

    return true;
}

bool IvMerger::saveInterfaceView(ivm::IVModel *model, const QString &outputIvFile)
{
    ivm::IVXMLWriter exporter;
    return exporter.exportObjectsSilently(model->objects().values(), outputIvFile);
}

} // namespace sdl::exporter
