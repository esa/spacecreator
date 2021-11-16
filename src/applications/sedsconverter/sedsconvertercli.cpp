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

#include "sedsconvertercli.h"

#include "commandlineparser.h"

#include <QStringList>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/common/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <sdl/SdlOptions/options.h>
#include <seds/SedsOptions/options.h>

using conversion::ConversionException;
using conversion::ModelType;
using conversion::Options;
using conversion::asn1::Asn1Options;
using conversion::iv::IvOptions;
using conversion::seds::SedsOptions;
using sdl::exporter::SdlOptions;

typedef shared::CommandLineParser::Positional CommandArg;

namespace sedsconverter {

void SedsConverterCLI::parseArguments(const QStringList &arguments)
{
    m_parser.handlePositional(CommandArg::SedsConverterInputFilename);
    m_parser.handlePositional(CommandArg::SedsConverterOutputFilename);
    m_parser.handlePositional(CommandArg::SedsConverterModelFrom);
    m_parser.handlePositional(CommandArg::SedsConverterModelTo);
    m_parser.handlePositional(CommandArg::SedsConverterModelAux);
    m_parser.handlePositional(CommandArg::SedsConverterIvConfig);
    m_parser.handlePositional(CommandArg::SedsConverterSedsSchemaFilename);
    m_parser.handlePositional(CommandArg::SedsConverterSedsExtRef);
    m_parser.handlePositional(CommandArg::SedsConverterSedsExtRefFilename);
    m_parser.handlePositional(CommandArg::SedsConverterSkipValidation);
    m_parser.handlePositional(CommandArg::SedsConverterKeepIntermediateFiles);
    m_parser.handlePositional(CommandArg::SedsConverterAcnFilepathPrefix);
    m_parser.handlePositional(CommandArg::SedsConverterAsn1FilepathPrefix);
    m_parser.handlePositional(CommandArg::SedsConverterSdlFilepathPrefix);
    m_parser.handlePositional(CommandArg::SedsConverterImportXmlFileForAsn1);

    m_parser.process(arguments);
    m_arguments = m_parser.positionalsSet();
    if (m_arguments.contains(CommandArg::SedsConverterInputFilename)) {
        m_inputFilename = m_parser.value(CommandArg::SedsConverterInputFilename);
    }
    if (m_arguments.contains(CommandArg::SedsConverterOutputFilename)) {
        m_outputFilename = m_parser.value(CommandArg::SedsConverterOutputFilename);
    }
    if (m_arguments.contains(CommandArg::SedsConverterModelFrom)) {
        for (auto modelName : m_parser.value(CommandArg::SedsConverterModelFrom).split(",")) {
            m_sourceModels.emplace(conversion::stringToModelType(modelName));
        }
    }
    if (m_arguments.contains(CommandArg::SedsConverterModelTo)) {
        m_targetModel = conversion::stringToModelType(m_parser.value(CommandArg::SedsConverterModelTo));
    }
    if (m_arguments.contains(CommandArg::SedsConverterModelAux)) {
        for (auto modelName : m_parser.value(CommandArg::SedsConverterModelAux).split(",")) {
            m_auxModels.emplace(conversion::stringToModelType(modelName));
        }
    }
    if (m_inputFilename.isEmpty()) {
        throw ConversionException("No input file");
    }
}

void SedsConverterCLI::setOptions(Options &options)
{
    addDefaultValues(options);

    for (auto sourceModel : m_sourceModels) {
        switch (sourceModel) {
        case conversion::ModelType::Asn1: {
            addAsn1InputOptions(options);
        } break;
        case conversion::ModelType::Seds: {
            addSedsInputOptions(options);
            addSedsAdditionalFilesOptions(options);
            addSedsBehaviourOptions(options);
        } break;
        case conversion::ModelType::InterfaceView: {
            addIvInputOptions(options);
            addIvConfigOption(options);
        } break;
        default:
            throw ConversionException("Unknown source model");
        }
    }

    std::set<conversion::ModelType> outputModels = m_auxModels;
    outputModels.insert(m_targetModel);
    for (auto outputModel : outputModels) {
        switch (outputModel) {
        case conversion::ModelType::Asn1: {
            addAsn1OutputOptions(options);
        } break;
        case conversion::ModelType::InterfaceView: {
            addIvOutputOptions(options);
            addIvConfigOption(options);
        } break;
        case conversion::ModelType::Sdl: {
            addSdlFilepathPrefix(options);
        } break;
        case conversion::ModelType::Seds: {
        } break;
        default:
            throw ConversionException("Unknown target model");
        }
    }
}

std::set<conversion::ModelType> SedsConverterCLI::getSourceModelTypes()
{
    if (m_sourceModels.empty()) {
        throw ConversionException("Model type unspecified");
    }
    for (auto model : m_sourceModels) {
        if (model == conversion::ModelType::Unspecified) {
            throw ConversionException("Model type unspecified");
        }
    }
    return m_sourceModels;
}

conversion::ModelType SedsConverterCLI::getTargetModelType()
{
    if (m_targetModel == conversion::ModelType::Unspecified) {
        throw ConversionException("Model type unspecified");
    }
    return m_targetModel;
}

std::set<conversion::ModelType> SedsConverterCLI::getAuxModelTypes()
{
    for (auto model : m_auxModels) {
        if (model == conversion::ModelType::Unspecified) {
            throw ConversionException("Model type unspecified");
        }
    }
    return m_auxModels;
}

void SedsConverterCLI::addAsn1InputOptions(Options &options)
{
    options.add(Asn1Options::inputFilename, m_inputFilename);
    if (m_arguments.contains(CommandArg::SedsConverterImportXmlFileForAsn1)) {
        options.add(Asn1Options::importXmlFile);
    } else {
        options.add(Asn1Options::importAsn1File);
    }
}

void SedsConverterCLI::addSedsInputOptions(Options &options)
{
    options.add(SedsOptions::inputFilename, m_inputFilename);
}

void SedsConverterCLI::addSedsAdditionalFilesOptions(Options &options)
{
    if (m_arguments.contains(CommandArg::SedsConverterIvConfig)) {
        options.add(SedsOptions::schemaFilename, m_parser.value(CommandArg::SedsConverterSedsSchemaFilename));
    }
    if (m_arguments.contains(CommandArg::SedsConverterSedsExtRef)) {
        for (auto value : m_parser.value(CommandArg::SedsConverterSedsExtRef).split(",")) {
            options.add(SedsOptions::externalRefFilename, value);
        }
    }
    if (m_arguments.contains(CommandArg::SedsConverterSedsExtRefFilename)) {
        options.add(SedsOptions::externalRefFilename, m_parser.value(CommandArg::SedsConverterSedsExtRefFilename));
    }
}

void SedsConverterCLI::addSedsBehaviourOptions(Options &options)
{
    if (m_arguments.contains(CommandArg::SedsConverterSkipValidation)) {
        options.add(SedsOptions::skipValidation);
    }
    if (m_arguments.contains(CommandArg::SedsConverterKeepIntermediateFiles)) {
        options.add(SedsOptions::keepIntermediateFiles);
    }
}

void SedsConverterCLI::addAsn1OutputOptions(Options &options)
{
    if (m_arguments.contains(CommandArg::SedsConverterAcnFilepathPrefix)) {
        options.add(Asn1Options::acnFilepathPrefix, m_parser.value(CommandArg::SedsConverterAcnFilepathPrefix));
    }
    if (m_arguments.contains(CommandArg::SedsConverterAsn1FilepathPrefix)) {
        options.add(Asn1Options::asn1FilepathPrefix, m_parser.value(CommandArg::SedsConverterAsn1FilepathPrefix));
    }
}

void SedsConverterCLI::addIvInputOptions(Options &options)
{
    options.add(IvOptions::inputFilename, m_inputFilename);
}

void SedsConverterCLI::addIvOutputOptions(Options &options)
{
    if (m_outputFilename.isEmpty()) {
        m_outputFilename = QString("%1.iv").arg(m_inputFilename.toLatin1().constData());
    }
    options.add(IvOptions::outputFilename, m_outputFilename);
}

void SedsConverterCLI::addIvConfigOption(Options &options)
{
    if (m_arguments.contains(CommandArg::SedsConverterIvConfig)) {
        options.add(IvOptions::configFilename, m_parser.value(CommandArg::SedsConverterIvConfig));
    }
}

void SedsConverterCLI::addSdlFilepathPrefix(Options &options)
{
    if (m_arguments.contains(CommandArg::SedsConverterSdlFilepathPrefix)) {
        options.add(SdlOptions::sdlFilepathPrefix, m_parser.value(CommandArg::SedsConverterSdlFilepathPrefix));
    }
}

void SedsConverterCLI::addDefaultValues(Options &options)
{
    options.add(SedsOptions::schemaFilename, SedsOptions::defaultSchemaFilename);
    options.add(IvOptions::configFilename, IvOptions::defaultConfigFilename);
}

} // namespace sedsconverter
