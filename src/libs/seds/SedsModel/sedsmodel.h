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

#include <conversion/common/model.h>
#include <seds/SedsModel/datasheet.h>
#include <seds/SedsModel/packagefile.h>
#include <variant>

namespace seds::model {

/**
 * @brief   Represents a SEDS data model
 *
 * Stores either a PackageFile or a DataSheet.
 */
class SedsModel final : public conversion::Model
{
public:
    /** @brief  Model data type */
    using Data = std::variant<model::PackageFile, model::DataSheet>;

public:
    /**
     * @brief   Constructor
     *
     * @param   data    Model data
     */
    explicit SedsModel(Data data);
    /**
     * @brief   Deleted copy constructor/
     */
    SedsModel(const SedsModel &) = delete;
    /**
     * @brief   Default move constructor
     */
    SedsModel(SedsModel &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    SedsModel &operator=(const SedsModel &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    SedsModel &operator=(SedsModel &&) = default;

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

} // namespace seds::model

namespace conversion {

/**
 * @brief   Specialization for SEDS model
 */
template<>
struct ModelProperties<::seds::model::SedsModel> {
    /// @brief  Model type
    static const ModelType type = ModelType::Seds;
    /// @brief  Model name
    static inline const QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extension
    static inline const QString extension = ModelTypeProperties<type>::extension;
};

} // namespace conversion
