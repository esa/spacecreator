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
#include "system.h"

#include <QStringList>
#include <conversion/common/model.h>

namespace sdl {

/**
 * @brief   Represents an SDL process model.
 *
 * Stores a reference to the Process.
 */
class SdlModel final : public conversion::Model
{
public:
    /**
     * @brief   Constructor
     *
     * @param name        name of this process model
     */
    explicit SdlModel(QString name = "");
    /**
     * @brief   Deleted copy constructor/
     */
    SdlModel(const SdlModel &) = delete;
    /**
     * @brief   Default move constructor
     */
    SdlModel(SdlModel &&) = default;

    /**
     * @brief   Default copy assignment operator
     */
    SdlModel &operator=(const SdlModel &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    SdlModel &operator=(SdlModel &&) = default;

public:
    /**
     * @brief   Getter for this model type
     *
     * @return  Model type
     */
    virtual auto modelType() const -> conversion::ModelType override;

    /**
     * @brief   Getter for systems stored in a model
     *
     * @return  Model systems
     */
    auto systems() const -> const std::vector<System> &;
    /**
     * @brief   Add a system to the model
     *
     * @param   system      System
     */
    auto addSystem(System system) -> void;

    /**
     * @brief   Getter for processes stored in a model
     *
     * @returns Model processes
     */
    auto processes() const -> const std::vector<Process> &;
    /**
     * @brief   Add a Process to the model
     *
     * @param   process    process
     */
    auto addProcess(Process process) -> void;

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
    auto setName(QString name) -> void;

private:
    /** @brief  Model systems */
    std::vector<System> m_systems;
    /** @brief  Model processes */
    std::vector<Process> m_processes;
    /** @brief  Model name */
    QString m_name;
};

} // namespace sdl

namespace conversion {

/**
 * @brief   Specialization for SDL model
 */
template<>
struct ModelProperties<::sdl::SdlModel> {
    /// @brief  Model type
    static const ModelType type = ModelType::Sdl;
    /// @brief  Model name
    static inline const QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extension
    static inline const QStringList extensions = ModelTypeProperties<type>::extensions;
};

} // namespace conversion
