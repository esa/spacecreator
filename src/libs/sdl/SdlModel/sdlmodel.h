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

#include "process.h"

#include <conversion/common/model.h>

namespace sdl {

/**
 * @brief   Represents an SDL data model.
 *
 * Stores a reference to the Process.
 */
class SdlModel final : public conversion::Model
{
public:
    /** @brief Model data type **/
    using Data = Process;

    /**
     * @brief   Constructor
     *
     * @param data     model data (Process)
     * @param name     name of this data model
     */
    explicit SdlModel(Data &data, const QString &name = "");

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
     * @brief   Deleted move assignment operator
     */
    SdlModel &operator=(SdlModel &&) = delete;

    /**
     * @brief   Getter for this model type
     *
     * @return  Model type
     */
    virtual auto modelType() const -> conversion::ModelType override;

    /**
     * @brief   Getter for model data
     *
     * @returns Model data
     */
    auto data() const -> const Data &;

    /**
     * @brief   Getter for model name
     *
     * @returns Model name
     */
    auto name() const -> const QString &;

    /**
     * @brief   Setter for model name
     *
     * @param   name model name
     */
    auto setName(const QString &name) const -> void;

private:
    /** @brief  Model data */
    Data &m_data;
    /** @brief  Model name */
    QString m_name;
};

} // namespace sdl
