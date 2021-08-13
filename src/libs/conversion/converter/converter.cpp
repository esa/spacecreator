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
#include <conversion/common/translation/translator.h>
#include <conversion/seds/SedsRegistrar/registrar.h>

namespace conversion {

using exporter::ModelExporter;
using importer::ImportException;
using importer::ModelImporter;
using translator::Translator;

Converter::Converter(const Registry &registry, Options options)
    : m_registry(registry)
    , m_options(std::move(options))
{
}

void Converter::convert(std::set<ModelType> sourceModelsTypes, ModelType targetModelType)
{
    const auto *translator = m_registry.findTranslator(sourceModelsTypes, targetModelType);
    if (!translator) {
        throw TranslatorNotRegisteredException(sourceModelsTypes, targetModelType);
    }

    const auto *exporter = m_registry.findExporter(targetModelType);

    for (const auto &sourceModelType : sourceModelsTypes) {
        if (!isModelImported(sourceModelType)) {
            importModel(sourceModelType);
        }
    }

    // TODO: Check and resolve translator dependencies
    translateModels(translator, targetModelType);
    exportModel(exporter, targetModelType);
}

void Converter::importModel(ModelType modelType)
{
    const auto *importer = m_registry.findImporter(modelType);
    if (importer == nullptr) {
        throw ImporterNotRegisteredException(modelType);
    }

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

void Converter::translateModels(const Translator *translator, ModelType targetModelType)
{
    std::vector<const Model *> sourceModels;
    for (const auto modelType : translator->getDependencies()) {
        if (!isModelImported(modelType)) {
            const auto message = QString("Required %1 model is missing").arg(modelTypeToString(modelType));
            throw ConverterException(message);
        }

        const auto *model = m_modelCache[modelType].get();
        sourceModels.push_back(model);
    }

    auto targetModel = translator->translateModels(std::move(sourceModels), m_options);

    const auto insertionResult = m_modelCache.insert({ targetModelType, std::move(targetModel) });
    if (!insertionResult.second) {
        const auto message = QString("Failed to save translated %1 model").arg(modelTypeToString(targetModelType));
        throw ConverterException(message);
    }
}

void Converter::exportModel(const ModelExporter *exporter, ModelType modelType)
{
    if (!isModelImported(modelType)) {
        const auto message = QString("%1 model was not found").arg(modelTypeToString(modelType));
        throw ConverterException(message);
    }

    const auto *model = m_modelCache[modelType].get();
    exporter->exportModel(model, m_options);
}

bool Converter::isModelImported(ModelType modelType) const
{
    return (m_modelCache.count(modelType) != 0);
}

} // namespace conversion
