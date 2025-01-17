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
#include <conversion/common/import/modelimporter.h>
#include <memory>
#include <seds/SymbolDefinitionReader/symboldefinitionreader.h>

namespace conversion {
class Model;
class Options;
} // namespace conversion

namespace seds::importer {

/**
 * @brief   Importer that reads SEDS files
 */
class SedsXmlImporter final : public conversion::importer::ModelImporter
{
public:
    /**
     * @brief   Read given SEDS file and produce SEDS model
     *
     * @param   options     List of options
     *
     * @throws  conversion::importer::FileNotFound
     * @throws  seds::symbolreader::SymbolDefinitionReaderException
     * @throws  seds::preprocessor::XmlPreprocessorException
     * @throws  seds::preprocessor::UndefinedExternalReference
     * @throws  seds::validator::ValidatorException
     *
     * @return  Imported SEDS model
     */
    virtual auto importModel(const conversion::Options &options) const -> std::unique_ptr<conversion::Model> override;

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
     * @throws  conversion::importer::FileNotFound
     * @throws  seds::symbolreader::SymbolDefinitionReaderException
     *
     * @returns Map with external references
     */
    auto readExternalReferences(const conversion::Options &options) const
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
     * @throws  conversion::importer::FileNotFound
     * @throws  seds::preprocessor::XmlPreprocessorException
     * @throws  seds::preprocessor::UndefinedExternalReference
     *
     * @returns Preprocessed filename
     */
    auto preprocess(const symbolreader::SymbolDefinitionReader::ExternalReferencesMap &externalReferences,
            const conversion::Options &options) const -> QString;
    /**
     * @brief   Validate given preprocessed SEDS file
     *
     * Uses XmlValidator
     *
     * @param   preprocessedFilename    File to validate
     * @param   options                 Importer options
     *
     * @throws  conversion::importer::FileNotFound
     * @throws  seds::validator::ValidatorException
     */
    auto validate(const QString &preprocessedFilepath, const conversion::Options &options) const -> void;

private:
    static const QString preprocessedFilenameTemplate;
};

} // namespace seds::importer
