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

#include <conversion/common/modeltype.h>
#include <conversion/registry/registry.h>

namespace sedsconverter {

/**
 * @brief   Console application for SEDS conversion
 */
class SedsConverter final
{
public:
    /**
     * @brief   Constructor
     */
    SedsConverter();
    /**
     * @brief   Deleted copy constructor
     */
    SedsConverter(const SedsConverter &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    SedsConverter(SedsConverter &&) = delete;

    /**
     * @brief   Deleted copy assignment operator
     */
    SedsConverter &operator=(const SedsConverter &) = delete;
    /**
     * @brief   Deleted move assignmnet operator
     */
    SedsConverter &operator=(SedsConverter &&) = delete;

public:
    /**
     * @brief   Convert SEDS to target model
     *
     * @param   targetModelType         Target model type
     * @param   auxiliaryModelsTypes    List of types that will be additionally exported
     */
    auto convert(conversion::ModelType targetModelType,
            const std::set<conversion::ModelType> &auxiliaryModelsTypes) const -> void;

private:
    /**
     * @brief   Register all required conversion components in the internal registry
     */
    auto initializeRegistry() -> void;

private:
    /** @brief  Internal registry */
    conversion::Registry m_registry;
};

} // namespace sedsconverter
