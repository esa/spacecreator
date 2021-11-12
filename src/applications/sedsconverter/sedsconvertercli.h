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
    void setOptions(conversion::Options &options);

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
     * @brief   Add input filename for ASN.1
     */
    void addAsn1InputOption(conversion::Options &options);
    /**
     * @brief   Add options for output in ASN.1 conversion
     */
    void addAsn1OutputOptions(conversion::Options &options);
    /**
     * @brief   Add SEDS input filename
     */
    void addSedsInputOption(conversion::Options &options);
    /**
     * @brief   Specify validation and keeping intermediate files by SEDS converter
     */
    void addSedsBehaviourOptions(conversion::Options &options);
    /**
     * @brief   Add additional files used by SEDS converter
     */
    void addSedsAdditionalFilesOptions(conversion::Options &options);
    /**
     * @brief   Add InterfaceView input filename
     */
    void addIvInputOption(conversion::Options &options);
    /**
     * @brief   Add InterfaceView config xml
     */
    void addIvConfigOption(conversion::Options &options);
    /**
     * @brief   Add InterfaceView output filename
     */
    void addIvOutputOption(conversion::Options &options);
    /**
     * @brief   Add options for SDL model
     */
    void addSdlFilepathPrefix(conversion::Options &options);
    /**
     * @brief   Add default filenames to options
     */
    void addDefaultValues(conversion::Options &options);

private:
    /**
     * @brief   input file for converter
     */
    QString inputFilename;
    /**
     * @brief   output file for converter
     */
    QString outputFilename;
    /**
     * @brief   source conversion model types
     */
    std::set<conversion::ModelType> sourceModels;
    /**
     * @brief   target conversion model type
     */
    conversion::ModelType targetModel;
    /**
     * @brief   auxillary conversion model types
     */
    std::set<conversion::ModelType> auxModels;
    /**
     * @brief   parsed command line arguments
     */
    QVector<shared::CommandLineParser::Positional> args;
    /**
     * @brief   general command line parser
     */
    shared::CommandLineParser parser;
};

} // namespace sedsconverter
