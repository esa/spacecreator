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
    parser.handlePositional(CommandArg::SedsConverterInputFilename);
    parser.handlePositional(CommandArg::SedsConverterOutputFilename);
    parser.handlePositional(CommandArg::SedsConverterModelFrom);
    parser.handlePositional(CommandArg::SedsConverterModelTo);
    parser.handlePositional(CommandArg::SedsConverterModelAux);
    parser.handlePositional(CommandArg::SedsConverterIvConfig);
    parser.handlePositional(CommandArg::SedsConverterSedsSchemaFilename);
    parser.handlePositional(CommandArg::SedsConverterSedsExtRef);
    parser.handlePositional(CommandArg::SedsConverterSedsExtRefFilename);
    parser.handlePositional(CommandArg::SedsConverterSkipValidation);
    parser.handlePositional(CommandArg::SedsConverterKeepIntermediateFiles);
    parser.handlePositional(CommandArg::SedsConverterAcnFilepathPrefix);
    parser.handlePositional(CommandArg::SedsConverterAsn1FilepathPrefix);
    parser.handlePositional(CommandArg::SedsConverterSdlFilepathPrefix);
    parser.handlePositional(CommandArg::SedsConverterImportXmlFileForAsn1);

    parser.process(arguments);
    args = parser.positionalsSet();
    if (args.contains(CommandArg::SedsConverterInputFilename)) {
        inputFilename = parser.value(CommandArg::SedsConverterInputFilename);
    }
    if (args.contains(CommandArg::SedsConverterOutputFilename)) {
        outputFilename = parser.value(CommandArg::SedsConverterOutputFilename);
    }
    if (args.contains(CommandArg::SedsConverterModelFrom)) {
        for (auto modelName : parser.value(CommandArg::SedsConverterModelFrom).split(",")) {
            sourceModels.emplace(conversion::stringToModelType(modelName));
        }
    }
    if (args.contains(CommandArg::SedsConverterModelTo)) {
        targetModel = conversion::stringToModelType(parser.value(CommandArg::SedsConverterModelTo));
    }
    if (args.contains(CommandArg::SedsConverterModelAux)) {
        for (auto modelName : parser.value(CommandArg::SedsConverterModelAux).split(",")) {
            auxModels.emplace(conversion::stringToModelType(modelName));
        }
    }
    if (inputFilename.isEmpty()) {
        throw ConversionException("No input file");
    }
}

void SedsConverterCLI::setOptions(Options &options)
{
    addDefaultValues(options);

    for (auto sourceModel : sourceModels) {
        switch (sourceModel) {
        case conversion::ModelType::Asn1: {
            addAsn1InputOption(options);
        } break;
        case conversion::ModelType::Seds: {
            addSedsInputOption(options);
            addSedsAdditionalFilesOptions(options);
            addSedsBehaviourOptions(options);
        } break;
        case conversion::ModelType::InterfaceView: {
            addIvInputOption(options);
            addIvConfigOption(options);
        } break;
        default:
            throw ConversionException("Unknown source model");
        }
    }

    std::set<conversion::ModelType> outputModels = auxModels;
    outputModels.insert(targetModel);
    for (auto outputModel : outputModels) {
        switch (outputModel) {
        case conversion::ModelType::Asn1: {
            addAsn1OutputOptions(options);
        } break;
        case conversion::ModelType::InterfaceView: {
            addIvOutputOption(options);
            addIvConfigOption(options);
        } break;
        case conversion::ModelType::Sdl: {
            addSdlFilepathPrefix(options);
        } break;
        default:
            throw ConversionException("Unknown target model");
        }
    }
}

std::set<conversion::ModelType> SedsConverterCLI::getSourceModelTypes()
{
    if (sourceModels.empty()) {
        throw ConversionException("Model type unspecified");
    }
    for (auto model : sourceModels) {
        if (model == conversion::ModelType::Unspecified) {
            throw ConversionException("Model type unspecified");
        }
    }
    return sourceModels;
}

conversion::ModelType SedsConverterCLI::getTargetModelType()
{
    if (targetModel == conversion::ModelType::Unspecified) {
        throw ConversionException("Model type unspecified");
    }
    return targetModel;
}

std::set<conversion::ModelType> SedsConverterCLI::getAuxModelTypes()
{
    for (auto model : auxModels) {
        if (model == conversion::ModelType::Unspecified) {
            throw ConversionException("Model type unspecified");
        }
    }
    return auxModels;
}

void SedsConverterCLI::addAsn1InputOption(Options &options)
{
    options.add(Asn1Options::inputFilename, inputFilename);
    if (args.contains(CommandArg::SedsConverterImportXmlFileForAsn1)) {
        options.add(Asn1Options::importXmlFile);
    } else {
        options.add(Asn1Options::importAsn1File);
    }
}

void SedsConverterCLI::addSedsInputOption(Options &options)
{
    options.add(SedsOptions::inputFilename, inputFilename);
}

void SedsConverterCLI::addSedsAdditionalFilesOptions(Options &options)
{
    if (args.contains(CommandArg::SedsConverterIvConfig)) {
        options.add(SedsOptions::schemaFilename, parser.value(CommandArg::SedsConverterSedsSchemaFilename));
    }
    if (args.contains(CommandArg::SedsConverterSedsExtRef)) {
        for(auto value : parser.value(CommandArg::SedsConverterSedsExtRef).split(",")) {
            options.add(SedsOptions::externalRefFilename, value);
        }
    }
    if (args.contains(CommandArg::SedsConverterSedsExtRefFilename)) {
        options.add(SedsOptions::externalRefFilename, parser.value(CommandArg::SedsConverterSedsExtRefFilename));
    }
}

void SedsConverterCLI::addSedsBehaviourOptions(Options &options)
{
    if (args.contains(CommandArg::SedsConverterSkipValidation)) {
        options.add(SedsOptions::skipValidation);
    }
    if (args.contains(CommandArg::SedsConverterKeepIntermediateFiles)) {
        options.add(SedsOptions::keepIntermediateFiles);
    }
}

void SedsConverterCLI::addAsn1OutputOptions(Options &options)
{
    if (args.contains(CommandArg::SedsConverterAcnFilepathPrefix)) {
        options.add(Asn1Options::acnFilepathPrefix, parser.value(CommandArg::SedsConverterAcnFilepathPrefix));
    }
    if (args.contains(CommandArg::SedsConverterAsn1FilepathPrefix)) {
        options.add(Asn1Options::asn1FilepathPrefix, parser.value(CommandArg::SedsConverterAsn1FilepathPrefix));
    }
}

void SedsConverterCLI::addIvInputOption(Options &options)
{
    options.add(IvOptions::inputFilename, inputFilename);
}

void SedsConverterCLI::addIvOutputOption(Options &options)
{
    if (outputFilename.isEmpty()) {
        outputFilename = QString("%1.iv").arg(inputFilename.toLatin1().constData());
    }
    options.add(IvOptions::outputFilename, outputFilename);
}

void SedsConverterCLI::addIvConfigOption(Options &options)
{
    if (args.contains(CommandArg::SedsConverterIvConfig)) {
        options.add(IvOptions::configFilename, parser.value(CommandArg::SedsConverterIvConfig));
    }
}

void SedsConverterCLI::addSdlFilepathPrefix(Options &options)
{
    if (args.contains(CommandArg::SedsConverterSdlFilepathPrefix)) {
        options.add(SdlOptions::sdlFilepathPrefix, parser.value(CommandArg::SedsConverterSdlFilepathPrefix));
    }
}

void SedsConverterCLI::addDefaultValues(Options &options)
{
    options.add(SedsOptions::schemaFilename, SedsOptions::defaultSchemaFilename);
    options.add(IvOptions::configFilename, IvOptions::defaultConfigFilename);
}

} // namespace sedsconverter

