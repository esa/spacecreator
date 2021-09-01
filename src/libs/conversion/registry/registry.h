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

#pragma once

#include <conversion/common/export/modelexporter.h>
#include <conversion/common/import/modelimporter.h>
#include <conversion/common/modeltype.h>
#include <conversion/common/translation/translator.h>
#include <map>
#include <memory>
#include <unordered_map>

namespace conversion {

/**
 * @brief   Keeps a register of import, export and conversion capabilities.
 */
class Registry final
{
private:
    class TranslatorsKey final
    {
    public:
        TranslatorsKey(std::set<ModelType> sourceModelsTypes, ModelType targetModelType);

        inline bool operator<(const TranslatorsKey &rhs) const;

    private:
        std::set<ModelType> m_sourceModelsTypes;
        ModelType m_targetModelType;
    };

public:
    /** @brief  Type for map of registered importers */
    using ImportersMap = std::unordered_map<ModelType, std::unique_ptr<importer::ModelImporter>>;
    /** @brief  Type for map of registered translators */
    using TranslatorsMap = std::map<TranslatorsKey, std::unique_ptr<translator::Translator>>;
    /** @brief  Type for map of registered exporters */
    using ExportersMap = std::unordered_map<ModelType, std::unique_ptr<exporter::ModelExporter>>;

public:
    /**
     * @brief   Registers new importer
     *
     * @param   modelType   Imported model type
     * @param   improter    Importer to register
     */
    [[nodiscard]] auto registerImporter(ModelType modelType, std::unique_ptr<importer::ModelImporter> importer) -> bool;
    /**
     * @brief   Registers new translator
     *
     * @param   sourceModelType     Source model type
     * @param   targetModelType     Target model type
     * @param   translator          Translator to register
     */
    [[nodiscard]] auto registerTranslator(std::set<ModelType> sourceModelType, ModelType targetModelType,
            std::unique_ptr<translator::Translator> translator) -> bool;
    /**
     * @brief   Registers new exporter
     *
     * @param   modelType   Exported model type
     * @param   exporter    Exporter to register
     */
    [[nodiscard]] auto registerExporter(ModelType modelType, std::unique_ptr<exporter::ModelExporter> exporter) -> bool;

    /**
     * @brief   Checks if importer for given model type is registered
     *
     * @param   modelType   Import model type
     *
     * @return  True if registered, false otherwise
     */
    auto isImporterRegistered(ModelType modelType) const -> bool;
    /**
     * @brief   Checks if translator for given sources and target models types is registered
     *
     * @param   sourceModelType     Source model type
     * @param   targetModelType     Target model type
     *
     * @return  True if registered, false otherwise
     */
    auto isTranslatorRegistered(std::set<ModelType> sourceModelType, ModelType targetModelType) const -> bool;
    /**
     * @brief   Checks if exporter for given model type is registered
     *
     * @param   modelType   Export model type
     *
     * @return  True if found, false otherwise
     */
    auto isExporterRegistered(ModelType modelType) const -> bool;

    /**
     * @brief   Finds importer for given model type
     *
     * @param   modelType   Model type to import
     *
     * @return  Importer if found, nullptr otherwise
     */
    auto findImporter(ModelType modelType) const -> const importer::ModelImporter *;
    /**
     * @brief   Finds translator for given model type
     *
     * @param   sourceModelType     Source model type
     * @param   targetModelType     Target model type
     *
     * @return  Translator if found, nullptr otherwise
     */
    auto findTranslator(std::set<ModelType> sourceModelType, ModelType targetModelType) const
            -> translator::Translator *;
    /**
     * @brief   Finds exporter for given model type
     *
     * @param   modelType   Model type to export
     *
     * @return  Exporter if found, nullptr otherwise
     */
    auto findExporter(ModelType modelType) const -> const exporter::ModelExporter *;

private:
    /** @brief  Map of registered importers */
    ImportersMap m_importers;
    /** @brief  Map of registered translators */
    TranslatorsMap m_translators;
    /** @brief  Map of registered exporters */
    ExportersMap m_exporters;
};

} // namespace conversion
