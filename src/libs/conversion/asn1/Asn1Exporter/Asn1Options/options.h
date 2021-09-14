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

#include <QString>

namespace conversion::asn1 {

/**
 * @brief   Possible options for ASN.1/ACN conversion
 */
class Asn1Options
{
public:
    /** @brief Prefix to add at the beginning of the filepath */
    inline static const QString asn1FilepathPrefix = "Asn1_Filepath_Export_Prefix";
    /** @brief Prefix to add at the beginning of the filepath */
    inline static const QString acnFilepathPrefix = "Acn_Filepath_Export_Prefix";
};

} // namespace conversion::asn1
