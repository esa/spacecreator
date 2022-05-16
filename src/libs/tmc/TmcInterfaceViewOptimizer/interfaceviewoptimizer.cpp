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
#include <libiveditor/ivexporter.h>
#include <conversion/common/translation/exceptions.h>

using ivm::meta::Props;
using ivm::IVFunction;
using ivm::IVModel;
using ivm::IVObject;

using conversion::translator::TranslationException;

namespace tmc {

void InterfaceViewOptimizer::optimizeModel(IVModel *ivModel)
{
    makeEnvironment("Pinger", ivModel);

    debugExportIv(ivModel);
}

void InterfaceViewOptimizer::makeEnvironment(const QString &functionName, IVModel *ivModel)
{
    auto function = findFunction(functionName, ivModel);
    setGUIImplementation(function);
}

IVFunction *InterfaceViewOptimizer::findFunction(const QString &functionName, IVModel *ivModel)
{
    auto object = ivModel->getObjectByName(functionName, IVObject::Type::Function, Qt::CaseSensitive);

    if (object == nullptr) {
        auto errorMessage = QString("Unable to find function '%1' for environment selection").arg(functionName);
        throw TranslationException(std::move(errorMessage));
    }

    auto function = dynamic_cast<IVFunction*>(object);

    if (function == nullptr) {
        auto errorMessage = QString("Object '%1' specified for environment selection is not a function").arg(functionName);
        throw TranslationException(std::move(errorMessage));
    }

    return function;
}

void InterfaceViewOptimizer::setGUIImplementation(IVFunction *function)
{
    function->addImplementation("environment", "GUI");
    function->setDefaultImplementation("environment");
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
