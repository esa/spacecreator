/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "converter.h"

#include "exceptions.h"

#include <conversion/common/exceptions.h>
#include <conversion/common/export/modelexporter.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/import/modelimporter.h>
#include <conversion/common/model.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/common/translation/translator.h>
#include <conversion/seds/SedsRegistrar/registrar.h>

namespace conversion {

using exporter::ModelExporter;
using importer::ImportException;
using importer::ModelImporter;
using translator::TranslationException;
using translator::Translator;

Converter::Converter(const Registry &registry, Options options)
    : m_registry(registry)
    , m_options(std::move(options))
{
}

std::set<std::unique_ptr<Model>> Converter::convert(const std::set<ModelType> &sourceModelsTypes,
        ModelType targetModelType, const std::set<ModelType> &auxiliaryModelsTypes)
{
    for (const auto sourceModelType : sourceModelsTypes) {
        if (!m_registry.isImporterRegistered(sourceModelType)) {
            throw ImporterNotRegisteredException(sourceModelType);
        }
    }
    if (!m_registry.isTranslatorRegistered(sourceModelsTypes, targetModelType)) {
        throw TranslatorNotRegisteredException(sourceModelsTypes, targetModelType);
    }
    if (!m_registry.isExporterRegistered(targetModelType)) {
        throw ExporterNotRegisteredException(targetModelType);
    }
    for (const auto auxiliaryModelType : auxiliaryModelsTypes) {
        if (!m_registry.isExporterRegistered(auxiliaryModelType)) {
            throw ExporterNotRegisteredException(auxiliaryModelType);
        }
    }

    for (const auto &sourceModelType : sourceModelsTypes) {
        if (!isModelImported(sourceModelType)) {
            importModel(sourceModelType);
        }
    }

    std::set<std::unique_ptr<Model>> outputModels;

    translateModels(sourceModelsTypes, targetModelType);
    outputModels.insert(exportModel(targetModelType));
    for (const auto &auxiliaryModelType : auxiliaryModelsTypes) {
        outputModels.insert(exportModel(auxiliaryModelType));
    }

    return outputModels;
}

void Converter::importModel(ModelType modelType)
{
    const auto *importer = m_registry.findImporter(modelType);

    auto model = importer->importModel(m_options);
    if (model == nullptr) {
        const auto message = QString("Failed to import %1 model").arg(modelTypeToString(modelType));
        throw ImportException(message);
    }

    const auto insertionResult = m_modelCache.insert({ modelType, std::move(model) });
    if (!insertionResult.second) {
        const auto message = QString("Failed to save imported %1 model").arg(modelTypeToString(modelType));
        throw ConverterException(message);
    }
}

void Converter::translateModels(const std::set<ModelType> &sourceModelsTypes, ModelType targetModelType)
{
    const auto *translator = m_registry.findTranslator(sourceModelsTypes, targetModelType);

    std::vector<Model *> sourceModels;
    for (const auto dependencyModelType : translator->getDependencies()) {
        if (isModelImported(dependencyModelType)) {
            auto *model = m_modelCache[dependencyModelType].get();
            sourceModels.push_back(model);
        } else if (m_registry.isTranslatorRegistered(sourceModelsTypes, dependencyModelType)) {
            translateModels(sourceModelsTypes, dependencyModelType);
            auto *model = m_modelCache[dependencyModelType].get();
            sourceModels.push_back(model);
        } else {
            const auto message = QString("Required %1 model is missing").arg(modelTypeToString(dependencyModelType));
            throw ConverterException(message);
        }
    }

    auto outputModels = translator->translateModels(std::move(sourceModels), m_options);

    for (auto &outputModel : outputModels) {
        const auto outputModelType = outputModel->modelType();

        const auto insertionResult = m_modelCache.insert({ outputModelType, std::move(outputModel) });
        if (!insertionResult.second) {
            const auto message = QString("Failed to save translated %1 model").arg(modelTypeToString(outputModelType));
            throw ConverterException(message);
        }
    }
}

std::unique_ptr<Model> Converter::exportModel(ModelType modelType)
{
    const auto *exporter = m_registry.findExporter(modelType);

    if (!isModelImported(modelType)) {
        const auto message = QString("%1 model was not found").arg(modelTypeToString(modelType));
        throw ConverterException(message);
    }

    const auto *model = m_modelCache[modelType].get();
    exporter->exportModel(model, m_options);

    return std::move(m_modelCache.extract(modelType).mapped());
}

bool Converter::isModelImported(ModelType modelType) const
{
    return (m_modelCache.count(modelType) != 0);
}

} // namespace conversion
