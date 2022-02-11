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

namespace conversion::promela {

/**
 * @brief   Possible options for Promela conversion
 */
class PromelaOptions
{
public:
    /** @brief  Filepath of output file  */
    inline static const QString outputFilepath = "Promela_Export_OutputFilepath";

    /** @brief Generate promela code for enhanced spin */
    inline static const QString enhancedSpinSupport = "Promela_Export_CodeForEnhancedSpin";

    /** @brief Add additional includes at the end of model */
    inline static const QString additionalIncludes = "Promela_Convert_AdditionalIncludes";

    /** @brief Generate promela inlines to generate values of ASN1 type */
    inline static const QString asn1ValueGeneration = "Promela_Convert_Asn1ValueGeneration";
};

}
