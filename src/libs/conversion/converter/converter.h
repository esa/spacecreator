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

#include "modeltype.h"
#include "options.h"

#include <conversion/registry/registry.h>
#include <memory>
#include <unordered_map>

namespace conversion {

namespace exporter {
class ModelExporter;
} // namespace exporter

namespace translator {
class Translator;
} // namespace translator

/**
 * @brief   Manages the process of model translation.
 */
class Converter final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   registry    Registry to use
     * @param   options     Conversion options
     */
    Converter(const Registry &registry, Options options);
    /**
     * @brief   Deleted copy constructor
     */
    Converter(const Converter &) = delete;
    /**
     * @brief   Default move constructor
     */
    Converter(Converter &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    Converter &operator=(const Converter &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Converter &operator=(Converter &&) = delete;

public:
    /**
     * @brief   Converts between given models
     *
     * @param   sourceModelsTypes       List of types of source models
     * @param   targetModelType         Type of target model
     */
    auto convert(std::set<ModelType> sourceModelsTypes, ModelType targetModelType) -> void;

private:
    /**
     * @brief   Imports a model and saves it in the model cache
     *
     * @param   modelType       Type of model to import
     */
    auto importModel(ModelType modelType) -> void;
    /**
     * @brief   Translates models and saves output in the model cache
     *
     * @param   translator          Translator to use
     * @param   targetModelType     Target model type
     */
    auto translateModels(const translator::Translator *translator, ModelType targetModelType) -> void;
    /**
     * @brief   Exports a model
     *
     * @param   exporter        Exporter to use
     * @param   modelType       Type of model to export
     */
    auto exportModel(const exporter::ModelExporter *exporter, ModelType modelType) -> void;

    /**
     * @brief   Checks if model of given type is already present in the model cache
     *
     * @param   modelType   Model type to check
     *
     * @return  True if model is present, false otherwise
     */
    auto isModelImported(ModelType modelType) const -> bool;

private:
    /** @brief  Registry to use */
    const Registry &m_registry;
    /** @brief  Options for conversion */
    Options m_options;

    /** @brief  Cache with all imported models */
    std::unordered_map<ModelType, std::unique_ptr<Model>> m_modelCache;
};

} // namespace conversion
