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

#include "commandlineparser.h"

#include <QStringList>
#include <conversion/common/options.h>
#include <conversion/converter/converter.h>
#include <map>

namespace sedsconverter {

/**
 * @brief   Command line interface for SEDS conversion
 */
class SedsConverterCLI
{
public:
    /**
     * @brief   Parse CLI arguments
     *
     * @param   arguments               String list with command line arguments
     */
    void parseArguments(const QStringList &arguments);
    /**
     * @brief   Set converter options according to previously parsed data
     *
     * @param   options                 Converter options list
     */
    void processOptions(conversion::Options &options);

    /**
     * @brief   Get source model types set
     *
     * @return  Set of source model types used in conversion
     */
    std::set<conversion::ModelType> getSourceModelTypes();
    /**
     * @brief   Get target model type
     *
     * @return  Target model type used in conversion
     */
    conversion::ModelType getTargetModelType();
    /**
     * @brief   Get auxillary models set
     *
     * @return  Set of auxillary model types used in conversion
     */
    std::set<conversion::ModelType> getAuxModelTypes();

private:
    /**
     * @brief   Process options for input ASN.1 model
     */
    void addAsn1InputOptions(conversion::Options &options);
    /**
     * @brief   Process options for output ASN.1 model
     */
    void addAsn1OutputOptions(conversion::Options &options);
    /**
     * @brief   Process options for input SEDS model
     */
    void addSedsInputOptions(conversion::Options &options);
    /**
     * @brief   Process options for output SEDS model
     */
    void addSedsOutputOptions(conversion::Options &options);
    /**
     * @brief   Process options for input IV model
     */
    void addIvInputOptions(conversion::Options &options);
    /**
     * @brief   Process options for translation IV model
     */
    void addIVTranslationOptions(conversion::Options &options);
    /**
     * @brief   Process options for output IV model
     */
    void addIvOutputOptions(conversion::Options &options);
    /**
     * @brief   Process options for input SDL model
     */
    void addSdlInputOptions(conversion::Options &options);
    /**
     * @brief   Process options for output SDL model
     */
    void addSdlOutputOptions(conversion::Options &options);

private:
    /**
     * @brief Get input filepaths for given model based on the input file extension
     *
     * @param   modelType   Requested model type
     *
     * @throws  ConversionException     When no input filename for given model was passed
     *
     * @return  List of input filepaths for given type
     */
    QStringList getInputFilepaths(conversion::ModelType modelType);

private:
    /**
     * @brief   Input files for converter, key is file extension
     */
    std::multimap<std::string, QString> m_inputFilepaths;
    /**
     * @brief   output file for converter
     */
    QString m_outputFilepath;
    /**
     * @brief   source conversion model types
     */
    std::set<conversion::ModelType> m_sourceModels;
    /**
     * @brief   target conversion model type
     */
    conversion::ModelType m_targetModel;
    /**
     * @brief   auxillary conversion model types
     */
    std::set<conversion::ModelType> m_auxModels;
    /**
     * @brief   parsed command line arguments
     */
    QVector<shared::CommandLineParser::Positional> m_arguments;
    /**
     * @brief   general command line parser
     */
    shared::CommandLineParser m_parser;
};

} // namespace sedsconverter
