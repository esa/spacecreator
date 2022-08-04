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

#include "importer.h"

#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QXmlStreamReader>
#include <algorithm>
#include <conversion/common/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/model.h>
#include <conversion/common/options.h>
#include <filesystem>
#include <seds/SedsOptions/options.h>
#include <seds/SedsXmlParser/parser.h>
#include <seds/XmlPreprocessor/preprocessor.h>
#include <seds/XmlValidator/validator.h>

using conversion::FileNotFoundException;
using conversion::Options;
using conversion::importer::ImportException;

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#define SEDS_SPLIT_BEHAVIOR QString::SkipEmptyParts
#else
#define SEDS_SPLIT_BEHAVIOR Qt::SkipEmptyParts
#endif

namespace seds::importer {

using conversion::seds::SedsOptions;
using parser::Parser;
using preprocessor::XmlPreprocessor;
using symbolreader::SymbolDefinitionReader;
using validator::XmlValidator;

const QString SedsXmlImporter::preprocessedFilenameTemplate = "preprocessed_%1";
static const std::string PREPROCESSED_FILE_PREFIX = "preprocessed_";

std::unique_ptr<conversion::Model> SedsXmlImporter::importModel(const Options &options) const
{
    const auto externalReferences = readExternalReferences(options);
    const auto preprocessedFilepath = preprocess(externalReferences, options);

    // Validation can be disabled by the user
    if (!options.isSet(SedsOptions::skipValidation)) {
        validate(preprocessedFilepath, options);
    }

    auto model = Parser::parse(preprocessedFilepath);

    // Cleanup working directory after importing
    if (!options.isSet(SedsOptions::keepIntermediateFiles)) {
        QFile preprocessedFile(preprocessedFilepath);
        preprocessedFile.remove();
    }

    return model;
}

SymbolDefinitionReader::ExternalReferencesMap SedsXmlImporter::readExternalReferences(const Options &options) const
{
    SymbolDefinitionReader::ExternalReferencesMap externalReferences;

    // First check if external references should be read from a file
    if (const auto externalRefFilepath = options.value(SedsOptions::externalRefFilepath); externalRefFilepath) {
        externalReferences = SymbolDefinitionReader::readSymbols(*externalRefFilepath);
    }

    // External references can be also be declared via options
    for (const auto &externalReferenceDeclaration : options.values(SedsOptions::externalRef)) {
        const auto externalReference = externalReferenceDeclaration.split(':', SEDS_SPLIT_BEHAVIOR);
        externalReferences.insert({ externalReference[0], externalReference[1] });
    }

    return externalReferences;
}

QString SedsXmlImporter::preprocess(
        const SymbolDefinitionReader::ExternalReferencesMap &externalReferences, const Options &options) const
{
    const auto inputFilepath = options.value(SedsOptions::inputFilepath);
    if (!inputFilepath) {
        throw ImportException("SEDS file to import wasn't specified");
    }

    const QFileInfo inputFileInfo(*inputFilepath);
    if (!inputFileInfo.exists()) {
        throw FileNotFoundException(*inputFilepath, "while importing");
    }

    // Intermediate preprocessed file is generated by preprocessor
    // Either use name provided in the options or use default `preprocessed_%input%`
    auto preprocessedFilepath = [&]() {
        const auto value = options.value(SedsOptions::preprocessedFilepath);
        if (value) {
            return *value;
        } else {
            auto fsPath = std::filesystem::path(inputFilepath->toStdString());
            const auto fileNamePart = std::filesystem::path((*inputFilepath).toStdString()).filename().string();
            return QString::fromStdString(fsPath.replace_filename(PREPROCESSED_FILE_PREFIX + fileNamePart).string());
        }
    }();

    XmlPreprocessor::preprocess(*inputFilepath, preprocessedFilepath, externalReferences);

    return preprocessedFilepath;
}

void SedsXmlImporter::validate(const QString &preprocessedFilepath, const Options &options) const
{
    const auto schemaFilepath = options.value(SedsOptions::schemaFilepath);
    if (schemaFilepath) {
        XmlValidator::validate(preprocessedFilepath, *schemaFilepath);
    } else {
        XmlValidator::validate(preprocessedFilepath);
    }
}

} // namespace seds::importer

#undef SEDS_SPLIT_BEHAVIOR
