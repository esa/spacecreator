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
#include <memory>

#include "file.h"

namespace Asn1Acn {

/**
 * @brief   Represents an ASN.1 data model
 */
class Asn1Model final : public conversion::Model
{
public:
    /** @brief  Model data type */
    using Data = std::vector<File>;

public:
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
     * @brief   Getter for model data
     *
     * @returns Model data
     */
    auto data() const -> const Data&;

private:
    /** @brief  Model data */
    Data m_data;
};

} // namespace Asn1Acn
