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

namespace conversion::seds {

/**
 * @brief   Possible options for SEDS conversion
 */
class SedsOptions
{
public:
    /** @brief  Filepath of input file */
    inline static const QString inputFilepath = "Seds_Import_InputFilepath";
    /** @brief Intermediate file that will be created during preprocessing */
    inline static const QString preprocessedFilepath = "Seds_Import_PreprocessedFilename";
    /** @brief Schema file that will be used during validation */
    inline static const QString schemaFilepath = "Seds_Import_SchemaFilepath";
    /** @brief Declaration of an external reference */
    inline static const QString externalRef = "Seds_Import_ExternalRef";
    /** @brief File with external references declarations */
    inline static const QString externalRefFilepath = "Seds_Import_ExternalRefFilepath";
    /** @brief Skip SEDS validation */
    inline static const QString skipValidation = "Seds_Import_SkipValidation";
    /** @brief Don't remove intermediate files after import */
    inline static const QString keepIntermediateFiles = "Seds_Import_KeepIntermediateFiles";
    /** @brief  Filepath of output file */
    inline static const QString outputFilepath = "Seds_Export_OutputFilepath";

    /** @brief Default output filename */
    inline static const QString defaultOutputFilepath = "seds.xml";
};

} // namespace conversion::seds
