/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "interfaceviewoptimizer.h"

#include <QBuffer>
#include <QSaveFile>
#include <iostream>
#include <ivcore/ivfunction.h>
#include <ivcore/ivconnection.h>
#include <libiveditor/ivexporter.h>
#include <conversion/common/translation/exceptions.h>

using ivm::meta::Props;
using ivm::IVConnection;
using ivm::IVFunction;
using ivm::IVFunctionType;
using ivm::IVModel;
using ivm::IVObject;

using conversion::translator::TranslationException;

namespace tmc {

void InterfaceViewOptimizer::optimizeModel(IVModel *ivModel, const std::vector<QString> &environmentFunctions)
{
    for(const auto &functionName : environmentFunctions) {
        makeEnvironment(functionName, ivModel);
    }

    removeDeadFunctions(ivModel);

    debugExportIv(ivModel);
}

void InterfaceViewOptimizer::makeEnvironment(const QString &functionName, IVModel *ivModel)
{
    auto function = findFunction(functionName, ivModel);
    setGUIImplementation(function);
}

void InterfaceViewOptimizer::removeDeadFunctions(IVModel *ivModel)
{
    std::vector<IVObject *> objectsToRemove;

    for (auto obj : ivModel->objects()) {
        auto connection = dynamic_cast<IVConnection *>(obj);

        if (connection == nullptr || !isConnectionDead(connection)) {
            continue;
        }

        objectsToRemove.push_back(connection);
        objectsToRemove.push_back(connection->sourceInterface());
        objectsToRemove.push_back(connection->targetInterface());
    }

    for (const auto obj : objectsToRemove) {
        ivModel->removeObject(obj);
    }

    std::vector<IVFunctionType *> functionsToRemove;

    for (auto obj : ivModel->objects()) {
        auto func = dynamic_cast<IVFunctionType *>(obj);

        if (func == nullptr || !isFunctionDead(func)) {
            continue;
        }

        functionsToRemove.push_back(func);
    }

    for (const auto func : functionsToRemove) {
        ivModel->removeObject(func);
    }
}

IVFunction *InterfaceViewOptimizer::findFunction(const QString &functionName, IVModel *ivModel)
{
    auto function = ivModel->getFunction(functionName, Qt::CaseSensitive);

    if (function == nullptr) {
        auto errorMessage = QString("Unable to find function '%1' for environment selection").arg(functionName);
        throw TranslationException(std::move(errorMessage));
    }

    return function;
}

void InterfaceViewOptimizer::setGUIImplementation(IVFunction *function)
{
    function->addImplementation("environment", "GUI");
    function->setDefaultImplementation("environment");
}

bool InterfaceViewOptimizer::isConnectionDead(const IVConnection *connection)
{
    const auto source = connection->source();
    const auto target = connection->target();

    if(source->type() != IVObject::Type::Function || target->type() != IVObject::Type::Function)
    {
        return false;
    }

    const auto sourceFunction = dynamic_cast<IVFunction *>(source);
    const auto targetFunction = dynamic_cast<IVFunction *>(target);

    return !isSdlFunction(sourceFunction) && !isSdlFunction(targetFunction);
}

bool InterfaceViewOptimizer::isSdlFunction(const ivm::IVFunction *function)
{
    const QString defaultImplementation = function->defaultImplementation();
    for (const auto &impl : function->implementations()) { // NOLINT(readability-use-anyofallof)
        if (impl.name() == defaultImplementation && impl.value().type() == QVariant::Type::String
                && impl.value().toString().toLower() == "sdl") {
            return true;
        }
    }
    return false;
}

bool InterfaceViewOptimizer::isFunctionDead(const IVFunctionType *function)
{
    return function->interfaces().empty();
}

void InterfaceViewOptimizer::debugExportIv(IVModel *ivModel)
{
    QByteArray modelData;
    QBuffer modelDataBuffer(&modelData);
    modelDataBuffer.open(QIODevice::WriteOnly);

    ive::IVExporter exporter;
    exporter.exportObjects(ivModel->objects().values(), &modelDataBuffer);

    QSaveFile outputFile("modified.xml");
    outputFile.open(QIODevice::WriteOnly);
    outputFile.write(modelData);
    outputFile.commit();
}

} // namespace tmc
