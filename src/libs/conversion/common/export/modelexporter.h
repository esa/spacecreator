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

#include "../model.h"
#include "../options.h"

#include <memory>

namespace conversion::exporter {

/**
 * @brief   Interface for converter model exporters
 */
class ModelExporter
{
public:
    /**
     * @brief   Default constructor
     */
    ModelExporter() = default;
    /**
     * @brief   Default destructor
     */
    virtual ~ModelExporter() = default;
    /**
     * @brief   Deleted copy constructor
     */
    ModelExporter(const ModelExporter &) = delete;
    /**
     * @brief   Default move constructor
     */
    ModelExporter(ModelExporter &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    ModelExporter &operator=(const ModelExporter &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    ModelExporter &operator=(ModelExporter &&) = default;

public:
    /**
     * @brief   Exports model
     *
     * @param   model       Model to export
     * @param   options     Options for export configuration
     */
    virtual auto exportModel(const Model *model, const Options &options) const -> void = 0;
};

} // namespace conversion::exporter
