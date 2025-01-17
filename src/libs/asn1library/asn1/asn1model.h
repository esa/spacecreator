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

#include "file.h"

#include <QStringList>
#include <conversion/common/model.h>
#include <memory>

namespace Asn1Acn {

/**
 * @brief   Represents an ASN.1 data model
 */
class Asn1Model final : public conversion::Model
{
public:
    /** @brief  Model data type */
    using Data = std::vector<std::unique_ptr<File>>;

public:
    /**
     * @brief   Default constructor
     */
    Asn1Model() = default;
    /**
     * @brief   Constructor
     *
     * @param   data    Model data
     */
    explicit Asn1Model(Data data);
    /**
     * @brief   Deleted copy constructor/
     */
    Asn1Model(const Asn1Model &) = delete;
    /**
     * @brief   Default move constructor
     */
    Asn1Model(Asn1Model &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Asn1Model &operator=(const Asn1Model &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Asn1Model &operator=(Asn1Model &&) = default;

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
    /**
     * @brief   Getter for model data
     *
     * @returns Model data
     */
    auto data() -> Data &;

    /**
     * @brief   Adds an ASN.1 file to the data
     *
     * @param   asn1File    File to add
     */
    auto addAsn1File(std::unique_ptr<File> asn1File) -> void;

private:
    /** @brief  Model data */
    Data m_data;
};

} // namespace Asn1Acn

namespace conversion {

/**
 * @brief   Specialization for ASN.1 model
 */
template<>
struct ModelProperties<Asn1Acn::Asn1Model> {
    /// @brief  Model type
    static const ModelType type = ModelType::Asn1;
    /// @brief  Model name
    static inline const QString name = ModelTypeProperties<type>::name;
    /// @brief  Model extension
    static inline const QStringList extensions = ModelTypeProperties<type>::extensions;
};

} // namespace conversion
