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

#include "sdlmodel/ast.h"

#include <conversion/common/model.h>

namespace Sdl {

/**
 * @brief   Represents an SDL data model.
 *
 * Stores AST.
 */
class SdlModel final : public conversion::Model
{
public:
    /** @brief Model data type **/
    using Data = AST;

public:
    /**
     * @brief   Constructor
     *
     * @param   data    Model data
     */
    explicit SdlModel();
    /**
     * @brief   Deleted copy constructor/
     */
    SdlModel(const SdlModel &) = delete;
    /**
     * @brief   Default move constructor
     */
    SdlModel(SdlModel &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    SdlModel &operator=(const SdlModel &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    SdlModel &operator=(SdlModel &&) = default;

public:
    /**
     * @brief   Get this model type
     *
     * @return  Model type
     */
    virtual auto modelType() const -> conversion::ModelType override;

public:
    /**
     * @brief   Getter for model data
     *
     * @returns Model data
     */
    auto data() const -> const Data &;

private:
    /** @brief  Model data */
    Data m_data;
};

} // namespace Sdl
