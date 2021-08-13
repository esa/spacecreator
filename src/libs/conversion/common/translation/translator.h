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
#include "modeltype.h"
#include "options.h"

#include <memory>
#include <set>

namespace conversion::translator {

/**
 * @brief   Interface for converter translators
 */
class Translator
{
public:
    /**
     * @brief   Default constructor
     */
    Translator() = default;
    /**
     * @brief   Default destructor
     */
    virtual ~Translator() = default;
    /**
     * @brief   Deleted copy constructor
     */
    Translator(const Translator &) = delete;
    /**
     * @brief   Default move constructor
     */
    Translator(Translator &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Translator &operator=(const Translator &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Translator &operator=(Translator &&) = default;

public:
    /**
     * @brief   Translate given source models into a target model
     *
     * @param   sources     Vector of source models
     * @param   options     Options for export configuration
     *
     * @return  Target model translated from source models
     */
    virtual auto translateModels(std::vector<const Model*> sources, const Options &options) const -> std::unique_ptr<Model> = 0;

    /**
     * @brief   Provides a set of all source model types that are required for the translation
     *
     * @return  Set of required models
     */
    virtual auto getDependencies() const -> std::set<ModelType> = 0;
};

} // namespace conversion::translator
