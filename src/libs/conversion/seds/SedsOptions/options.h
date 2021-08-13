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

namespace conversion::seds {

/**
 * @brief   Possible options for SEDS conversion
 */
class SedsOptions
{
public:
    /** @brief File to import */
    inline static const QString inputFile = "Seds_Import_InputFile";
    /** @brief Intermediate file that will be created during preprocessing */
    inline static const QString preprocessedFile = "Seds_Import_PreprocessedFile";
    /** @brief Schema file that will be used during validation */
    inline static const QString schemaFile = "Seds_Import_SchemaFile";
    /** @brief Declaration of an external reference */
    inline static const QString externalRef = "Seds_Import_ExternalRef";
    /** @brief File with external references declarations */
    inline static const QString externalRefFile = "Seds_Import_ExternalRefFile";
    /** @brief Skip SEDS validation */
    inline static const QString skipValidation = "Seds_Import_SkipValidation";
    /** @brief Don't remove intermediate files after import */
    inline static const QString keepIntermediateFiles = "Seds_Import_KeepIntermediateFiles";
};

} // namespace conversion::seds
