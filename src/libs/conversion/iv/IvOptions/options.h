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

namespace conversion::iv {

/**
 * @brief   Possible options for IV conversion
 */
class IvOptions
{
public:
    /** @brief File to import */
    inline static const QString inputFilename = "Iv_Import_InputFilename";
    /** @brief IV parser file */
    inline static const QString configFilename = "Iv_Import_ConfigFilename";
    /** @brief Use functions instead of a flat structure */
    inline static const QString generateFunctionsForPackages = "Iv_Translation_GenerateFunctionsForPackages";
    /** @biref Export output file */
    inline static const QString outputFilename = "Iv_Export_OutputFilename";
};

} // namespace conversion::seds
