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
#include <filesystem>
#include <promela/PromelaOptions/options.h>
#include <sdl/SdlOptions/options.h>
#include <seds/SedsOptions/options.h>

using conversion::ConversionException;
using conversion::ModelType;
using conversion::Options;
using conversion::asn1::Asn1Options;
using conversion::iv::IvOptions;
using conversion::promela::PromelaOptions;
using conversion::sdl::SdlOptions;
using conversion::seds::SedsOptions;

typedef shared::CommandLineParser::Positional CommandArg;

namespace sedsconverter {

void SedsConverterCLI::parseArguments(const QStringList &arguments)
{
    m_parser.handlePositional(CommandArg::SedsConverterInputFilepaths);
    m_parser.handlePositional(CommandArg::SedsConverterOutputFilepath);
    m_parser.handlePositional(CommandArg::SedsConverterModelsFrom);
    m_parser.handlePositional(CommandArg::SedsConverterModelTo);
    m_parser.handlePositional(CommandArg::SedsConverterModelsAux);
    m_parser.handlePositional(CommandArg::SedsConverterIvConfig);
    m_parser.handlePositional(CommandArg::SedsConverterSedsSchemaFilepath);
    m_parser.handlePositional(CommandArg::SedsConverterSedsExtRef);
    m_parser.handlePositional(CommandArg::SedsConverterSedsExtRefFilepath);
    m_parser.handlePositional(CommandArg::SedsConverterSkipValidation);
    m_parser.handlePositional(CommandArg::SedsConverterKeepIntermediateFiles);
    m_parser.handlePositional(CommandArg::SedsConverterAcnFilepathPrefix);
    m_parser.handlePositional(CommandArg::SedsConverterAsn1FilepathPrefix);
    m_parser.handlePositional(CommandArg::SedsConverterSdlFilepathPrefix);

    m_parser.process(arguments);
    m_arguments = m_parser.positionalsSet();
    if (m_arguments.contains(CommandArg::SedsConverterInputFilepaths)) {
        for (auto inputFilepath : m_parser.value(CommandArg::SedsConverterInputFilepaths).split(",")) {
            auto extension = std::filesystem::path(inputFilepath.toStdString()).extension();
            m_inputFilepaths.insert({ std::move(extension), std::move(inputFilepath) });
        }
    }
    if (m_arguments.contains(CommandArg::SedsConverterOutputFilepath)) {
        m_outputFilepath = m_parser.value(CommandArg::SedsConverterOutputFilepath);
    }
    if (m_arguments.contains(CommandArg::SedsConverterModelsFrom)) {
        for (const auto &modelName : m_parser.value(CommandArg::SedsConverterModelsFrom).split(",")) {
            m_sourceModels.emplace(conversion::stringToModelType(modelName));
        }
    }
    if (m_arguments.contains(CommandArg::SedsConverterModelTo)) {
        m_targetModel = conversion::stringToModelType(m_parser.value(CommandArg::SedsConverterModelTo));
    }
    if (m_arguments.contains(CommandArg::SedsConverterModelsAux)) {
        for (const auto &modelName : m_parser.value(CommandArg::SedsConverterModelsAux).split(",")) {
            m_auxModels.emplace(conversion::stringToModelType(modelName));
        }
    }
}

void SedsConverterCLI::setOptions(Options &options)
{
    for (auto sourceModel : m_sourceModels) {
        switch (sourceModel) {
        case conversion::ModelType::Asn1:
            addAsn1InputOptions(options);
            break;
        case conversion::ModelType::InterfaceView:
            addIvInputOptions(options);
            break;
        case conversion::ModelType::Promela:
            addPromelaInputOptions(options);
            break;
        case conversion::ModelType::Sdl:
            addSdlInputOptions(options);
            break;
        case conversion::ModelType::Seds:
            addSedsInputOptions(options);
            break;
        default:
            throw ConversionException("Unknown source model");
        }
    }

    std::set<conversion::ModelType> outputModels = m_auxModels;
    outputModels.insert(m_targetModel);
    for (auto outputModel : outputModels) {
        switch (outputModel) {
        case conversion::ModelType::Asn1:
            addAsn1OutputOptions(options);
            break;
        case conversion::ModelType::InterfaceView:
            addIvOutputOptions(options);
            break;
        case conversion::ModelType::Promela:
            addPromelaOutputOptions(options);
            break;
        case conversion::ModelType::Sdl:
            addSdlOutputOptions(options);
            break;
        case conversion::ModelType::Seds:
            addSedsOutputOptions(options);
            break;
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
    for (const auto &inputFilepath : getInputFilepaths(ModelType::Asn1)) {
        options.add(Asn1Options::inputFilepath, std::move(inputFilepath));
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
    for (const auto &inputFilepath : getInputFilepaths(ModelType::InterfaceView)) {
        options.add(IvOptions::inputFilepath, std::move(inputFilepath));
    }

    if (m_arguments.contains(CommandArg::SedsConverterIvConfig)) {
        options.add(IvOptions::configFilepath, m_parser.value(CommandArg::SedsConverterIvConfig));
    } else {
        options.add(IvOptions::configFilepath, IvOptions::defaultConfigFilename);
    }
}

void SedsConverterCLI::addIvOutputOptions(Options &options)
{
    if (!m_outputFilepath.isEmpty()) {
        options.add(IvOptions::outputFilepath, m_outputFilepath);
    } else {
        options.add(IvOptions::outputFilepath, IvOptions::defaultOutputFilename);
    }
}

void SedsConverterCLI::addPromelaInputOptions(conversion::Options &options)
{
    for (const auto &inputFilepath : getInputFilepaths(ModelType::Promela)) {
        options.add(PromelaOptions::inputFilepath, std::move(inputFilepath));
    }
}

void SedsConverterCLI::addPromelaOutputOptions(conversion::Options &options)
{
    if (!m_outputFilepath.isEmpty()) {
        options.add(PromelaOptions::outputFilepath, m_outputFilepath);
    } else {
        options.add(PromelaOptions::outputFilepath, PromelaOptions::defaultOutputFilepath);
    }
}

void SedsConverterCLI::addSdlInputOptions(Options &options)
{
    for (const auto &inputFilepath : getInputFilepaths(ModelType::Sdl)) {
        options.add(SdlOptions::inputFilepath, std::move(inputFilepath));
    }
}

void SedsConverterCLI::addSdlOutputOptions(Options &options)
{
    if (m_arguments.contains(CommandArg::SedsConverterSdlFilepathPrefix)) {
        options.add(SdlOptions::filepathPrefix, m_parser.value(CommandArg::SedsConverterSdlFilepathPrefix));
    }
}

void SedsConverterCLI::addSedsInputOptions(Options &options)
{
    for (const auto &inputFilepath : getInputFilepaths(ModelType::Seds)) {
        options.add(SedsOptions::inputFilepath, std::move(inputFilepath));
    }

    if (m_arguments.contains(CommandArg::SedsConverterSedsSchemaFilepath)) {
        options.add(SedsOptions::schemaFilepath, m_parser.value(CommandArg::SedsConverterSedsSchemaFilepath));
    }

    if (m_arguments.contains(CommandArg::SedsConverterSedsExtRef)) {
        for (auto value : m_parser.value(CommandArg::SedsConverterSedsExtRef).split(",")) {
            options.add(SedsOptions::externalRefFilepath, value);
        }
    }
    if (m_arguments.contains(CommandArg::SedsConverterSedsExtRefFilepath)) {
        options.add(SedsOptions::externalRefFilepath, m_parser.value(CommandArg::SedsConverterSedsExtRefFilepath));
    }

    if (m_arguments.contains(CommandArg::SedsConverterSkipValidation)) {
        options.add(SedsOptions::skipValidation);
    }

    if (m_arguments.contains(CommandArg::SedsConverterKeepIntermediateFiles)) {
        options.add(SedsOptions::keepIntermediateFiles);
    }
}

void SedsConverterCLI::addSedsOutputOptions(Options &options)
{
    if (!m_outputFilepath.isEmpty()) {
        options.add(SedsOptions::outputFilepath, m_outputFilepath);
    } else {
        options.add(SedsOptions::outputFilepath, SedsOptions::defaultOutputFilepath);
    }
}

QStringList SedsConverterCLI::getInputFilepaths(conversion::ModelType modelType)
{
    QStringList result;

    const auto extension = conversion::modelTypeExtension(modelType).toStdString();
    const auto range = m_inputFilepaths.equal_range(extension);
    std::for_each(range.first, range.second, [&result](auto &&filepath) { result << std::move(filepath.second); });

    return result;
}

} // namespace sedsconverter
