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

namespace conversion::iv {

/**
 * @brief   Possible options for IV conversion
 */
class IvOptions
{
public:
    /** @brief  Filepath of input file */
    inline static const QString inputFilepath = "Iv_Import_InputFilepath";
    /** @brief  IV parser file */
    inline static const QString configFilepath = "Iv_Import_ConfigFilename";
    /** @brief  Use functions instead of a flat structure */
    inline static const QString generateFunctionsForPackages = "Iv_Translation_GenerateFunctionsForPackages";
    /** @biref  Filepath of output file */
    inline static const QString outputFilepath = "Iv_Export_OutputFilepath";
    /** @biref  Name of a function to convert */
    inline static const QString functionToConvert = "Iv_FunctionToConvert";

    /** @brief  Default IV parser filename */
    inline static const QString defaultConfigFilename = "config.xml";
    /** @brief  Default output filename */
    inline static const QString defaultOutputFilename = "interfaceview.xml";
};

} // namespace conversion::seds
