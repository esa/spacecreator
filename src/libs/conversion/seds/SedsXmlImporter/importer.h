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
#include <converter/import/options.h>
#include <converter/model.h>
#include <memory>
#include <optional>
#include <seds/SymbolDefinitionReader/symboldefinitionreader.h>

namespace seds::importer {

/**
 * @brief   Importer that reads SEDS files
 */
class SedsXmlImporter final
{
public:
    /**
     * @brief   Read given SEDS file and produce SEDS model
     *
     * @param   options     List of options
     *
     * @throws  converter::import::FileNotFound
     * @throws  seds::symbolreader::SymbolDefinitionReaderException
     * @throws  seds::preprocessor::XmlPreprocessorException
     * @throws  seds::preprocessor::UndefinedExternalReference
     * @throws  seds::validator::ValidatorException
     *
     * @return  Imported SEDS model
     */
    static auto import(const converter::import::Options &options) -> std::unique_ptr<converter::Model>;

private:
    /**
     * @brief   Create external references map
     *
     * This checks if file with external references is provided - if yes then
     * it reads those definitions into a map. External references can also
     * be defined separatelty through options
     *
     * @param   options     Importer options
     *
     * @throws  converter::import::FileNotFound
     * @throws  seds::symbolreader::SymbolDefinitionReaderException
     *
     * @returns Map with external references
     */
    static auto readExternalReferences(const converter::import::Options &options)
            -> symbolreader::SymbolDefinitionReader::ExternalReferencesMap;
    /**
     * @brief   Preprocess given input SEDS file
     *
     * Output intermediate filename can be specified via options. If no such option
     * can be found then output file will be named `preprocessed_%inputFilename%`
     *
     * Uses XmlPreprocessor
     *
     * @param   externalReferences      External references map to use
     * @param   options                 Importer options
     *
     * @throws  converter::import::FileNotFound
     * @throws  seds::preprocessor::XmlPreprocessorException
     * @throws  seds::preprocessor::UndefinedExternalReference
     *
     * @returns Preprocessed filename
     */
    static auto preprocess(const symbolreader::SymbolDefinitionReader::ExternalReferencesMap &externalReferences,
            const converter::import::Options &options) -> QString;
    /**
     * @brief   Validate given preprocessed SEDS file
     *
     * Uses XmlValidator
     *
     * @param   preprocessedFilename    File to validate
     * @param   options                 Importer options
     *
     * @throws  converter::import::FileNotFound
     * @throws  seds::validator::ValidatorException
     */
    static auto validate(const QString &preprocessedFilename, const converter::import::Options &options) -> void;

    /**
     * @brief   Get schema filename from imported file
     *
     * @param   filename    File to search in
     *
     * @throws  ImportException
     *
     * @returns Schema filename
     */
    static auto getSchemaFilename(const QString &filename) -> QString;

private:
    static const QString preprocessedFilenameTemplate;
};

} // namespace seds::importer
