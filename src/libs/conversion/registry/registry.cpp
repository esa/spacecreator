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

#include "registry.h"

namespace conversion {

Registry::TranslatorsKey::TranslatorsKey(std::set<ModelType> sourceModelsTypes, ModelType targetModelType)
    : m_sourceModelsTypes(std::move(sourceModelsTypes))
    , m_targetModelType(targetModelType)
{
}

bool Registry::TranslatorsKey::operator<(const Registry::TranslatorsKey &rhs) const
{
    if (m_targetModelType != rhs.m_targetModelType) {
        return m_targetModelType < rhs.m_targetModelType;
    }

    if (m_sourceModelsTypes.size() != rhs.m_sourceModelsTypes.size()) {
        return m_sourceModelsTypes.size() < rhs.m_sourceModelsTypes.size();
    }

    auto lhsIt = m_sourceModelsTypes.begin();
    auto rhsIt = rhs.m_sourceModelsTypes.begin();

    while (lhsIt != m_sourceModelsTypes.end() && rhsIt != rhs.m_sourceModelsTypes.end()) {
        if (*lhsIt != *rhsIt) {
            return *lhsIt < *rhsIt;
        }

        ++lhsIt;
        ++rhsIt;
    }

    return false;
}

bool Registry::registerImporter(ModelType modelType, std::unique_ptr<importer::ModelImporter> importer)
{
    const auto result = m_importers.insert({ modelType, std::move(importer) });
    return result.second;
}

bool Registry::registerTranslator(std::set<ModelType> sourceModelsTypes, ModelType targetModelType,
        std::unique_ptr<translator::Translator> translator)
{
    Registry::TranslatorsKey key(std::move(sourceModelsTypes), targetModelType);
    const auto result = m_translators.insert({ std::move(key), std::move(translator) });
    return result.second;
}

bool Registry::registerExporter(ModelType modelType, std::unique_ptr<exporter::ModelExporter> exporter)
{
    const auto result = m_exporters.insert({ modelType, std::move(exporter) });
    return result.second;
}

bool Registry::isImporterRegistered(ModelType modelType) const
{
    const auto found = m_importers.find(modelType);
    return found != m_importers.end();
}

bool Registry::isTranslatorRegistered(std::set<ModelType> sourceModelsTypes, ModelType targetModelType) const
{
    const Registry::TranslatorsKey key(std::move(sourceModelsTypes), targetModelType);
    const auto found = m_translators.find(key);
    return found != m_translators.end();
}

bool Registry::isExporterRegistered(ModelType modelType) const
{
    const auto found = m_exporters.find(modelType);
    return found != m_exporters.end();
}

const importer::ModelImporter *Registry::findImporter(ModelType modelType) const
{
    const auto found = m_importers.find(modelType);

    if (found != m_importers.end()) {
        return found->second.get();
    } else {
        return nullptr;
    }
}

translator::Translator *Registry::findTranslator(std::set<ModelType> sourceModelsTypes, ModelType targetModelType) const
{
    const Registry::TranslatorsKey key(std::move(sourceModelsTypes), targetModelType);
    const auto found = m_translators.find(key);

    if (found != m_translators.end()) {
        return found->second.get();
    } else {
        return nullptr;
    }
}

const exporter::ModelExporter *Registry::findExporter(ModelType modelType) const
{
    const auto found = m_exporters.find(modelType);

    if (found != m_exporters.end()) {
        return found->second.get();
    } else {
        return nullptr;
    }
}

} // namespace conversion
