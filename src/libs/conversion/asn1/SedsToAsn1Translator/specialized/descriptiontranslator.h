/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include <asn1library/asn1/node.h>
#include <seds/SedsModel/base/description.h>
#include <seds/SedsModel/types/datatype.h>

namespace conversion::asn1::translator {

/**
 * @brief   Translator for SEDS short and long descriptions
 */
class DescriptionTranslator final
{
public:
    /**
     * @brief   Deleted constructor
     */
    DescriptionTranslator() = delete;

public:
    /**
     * @brief   Translate SEDS description to ASN.1 comment
     *
     * @param   sedsDescription     Description to translate
     * @param   asn1Node            Node that should contain the comment
     */
    static auto translate(const seds::model::Description &sedsDescription, Asn1Acn::Node *asn1Node) -> void;
    /**
     * @brief   Translate description of SEDS data type to ASN.1 comment
     *
     * @param   sedsDataType        Data type containing description
     * @param   asn1Node            Node that should contain the comment
     */
    static auto translate(const seds::model::DataType &sedsDataType, Asn1Acn::Node *asn1Node) -> void;
};

} // namespace conversion::asn1::translator
