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

#include "model.h"
#include "options.h"

#include <memory>

namespace conversion::importer {

/**
 * @brief   Interface for converter model importers
 */
class ModelImporter
{
public:
    /**
     * @brief   Default constructor
     */
    ModelImporter() = default;
    /**
     * @brief   Default destructor
     */
    virtual ~ModelImporter() = default;
    /**
     * @brief   Deleted copy constructor
     */
    ModelImporter(const ModelImporter &) = delete;
    /**
     * @brief   Default move constructor
     */
    ModelImporter(ModelImporter &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    ModelImporter &operator=(const ModelImporter &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    ModelImporter &operator=(ModelImporter &&) = default;

public:
    /**
     * @brief   Imports model
     *
     * @param   options     Options for import configuration
     *
     * @return  Imported model
     */
    virtual auto importModel(const Options &options) const -> std::unique_ptr<Model> = 0;
};

} // namespace conversion::importer
