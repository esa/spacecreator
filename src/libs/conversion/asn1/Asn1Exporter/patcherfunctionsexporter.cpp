/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "patcherfunctionsexporter.h"

#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/common/export/exceptions.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::Type;
using conversion::exporter::ExportException;

namespace conversion::asn1::exporter {

void PatcherFunctionsExporter::exportModel(const Asn1Model *model, const Options &options)
{
    std::vector<QString> patcherFunctionsFileNames;

    for (const auto &file : model->data()) {
        auto patcherFunctionsFileName = exportPatcherFunctions(file.get(), options);

        if (patcherFunctionsFileName.has_value()) {
            patcherFunctionsFileNames.push_back(std::move(*patcherFunctionsFileName));
        }
    }

    if (!patcherFunctionsFileNames.empty()) {
        exportMappingFunctionsModule(patcherFunctionsFileNames, options);
    }
}

std::optional<QString> PatcherFunctionsExporter::exportPatcherFunctions(
        const Asn1Acn::File *file, const Options &options)
{
    QString headerData;
    QTextStream headerStream(&headerData, QIODevice::WriteOnly);

    QString bodyData;
    QTextStream bodyStream(&bodyData, QIODevice::WriteOnly);

    auto createFile = false;

    for (const auto &definitions : file->definitionsList()) {
        const auto &types = definitions->types();

        for (const auto &type : types) {
            if (type->typeEnum() != Type::ASN1Type::SEQUENCE) {
                continue;
            }

            const auto sequence = dynamic_cast<const Sequence *>(type->type());

            if (sequence->postEncodingFunction()) {
                auto postEncodingFunctionName = QString("%1_encoding_function").arg(sequence->identifier().toLower());
                generateEncodingFunctionHeader(postEncodingFunctionName, headerStream);
                generateEncodingFunctionBody(postEncodingFunctionName, bodyStream);

                createFile = true;
            }

            if (sequence->postDecodingValidator()) {
                auto postDecodingValidatorName = QString("%1_decoding_validator").arg(sequence->identifier().toLower());
                generateDecodingValidatorHeader(postDecodingValidatorName, headerStream);
                generateDecodingValidatorBody(postDecodingValidatorName, bodyStream);

                createFile = true;
            }
        }
    }

    if (!createFile) {
        return std::nullopt;
    }

    const auto pathPrefix = options.value(Asn1Options::patcherFunctionsFilepathPrefix).value_or("");
    auto outputFileName = QString("%1-postencoding").arg(file->name()).toLower();

    const auto outputHeaderFilePath = QString("%1%2.h").arg(pathPrefix).arg(outputFileName);
    QSaveFile outputHeaderFile(outputHeaderFilePath);
    writeAndCommit(outputHeaderFile, headerData);

    const auto outputBodyFilePath = QString("%1%2.c").arg(pathPrefix).arg(outputFileName);
    QSaveFile outputBodyFile(outputBodyFilePath);
    writeAndCommit(outputBodyFile, bodyData);

    return outputFileName;
}

void PatcherFunctionsExporter::exportMappingFunctionsModule(
        const std::vector<QString> &patcherFunctionsFileNames, const Options &options)
{
    QString data;
    QTextStream stream(&data, QIODevice::WriteOnly);

    generateMappingFunctionsModule(patcherFunctionsFileNames, stream);

    const auto pathPrefix = options.value(Asn1Options::patcherFunctionsFilepathPrefix).value_or("");
    const auto outputFileName = options.value(Asn1Options::mappingFunctionsModuleFileName).value_or("postEncoding");

    const auto outputFilePath = QString("%1%2.h").arg(pathPrefix).arg(outputFileName);
    QSaveFile outputFile(outputFilePath);
    writeAndCommit(outputFile, data);
}

void PatcherFunctionsExporter::generateEncodingFunctionHeader(
        const QString &postEncodingFunctionName, QTextStream &stream)
{
    Q_UNUSED(postEncodingFunctionName);
    Q_UNUSED(stream);
}

void PatcherFunctionsExporter::generateEncodingFunctionBody(
        const QString &postEncodingFunctionName, QTextStream &stream)
{
    Q_UNUSED(postEncodingFunctionName);
    Q_UNUSED(stream);
}

void PatcherFunctionsExporter::generateDecodingValidatorHeader(
        const QString &postDecodingValidatorName, QTextStream &stream)
{
    Q_UNUSED(postDecodingValidatorName);
    Q_UNUSED(stream);
}

void PatcherFunctionsExporter::generateDecodingValidatorBody(
        const QString &postDecodingValidatorName, QTextStream &stream)
{
    Q_UNUSED(postDecodingValidatorName);
    Q_UNUSED(stream);
}

void PatcherFunctionsExporter::generateMappingFunctionsModule(
        const std::vector<QString> &patcherFunctionsFileNames, QTextStream &stream)
{
    stream << "#pragma once\n\n";

    for (const auto &patcherFunctionFileName : patcherFunctionsFileNames) {
        stream << "#include \"" << patcherFunctionFileName << ".h\"\n";
    }
}

void PatcherFunctionsExporter::writeAndCommit(QSaveFile &outputFile, const QString &data)
{
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw ExportException(QString("Failed to open a file %1").arg(outputFile.fileName()));
    }

    if (outputFile.write(data.toUtf8()) == -1) {
        throw ExportException(QString("Failed to write a file %1").arg(outputFile.fileName()));
    }

    if (!outputFile.commit()) {
        throw ExportException(QString("Failed to commit a transaction in %1").arg(outputFile.fileName()));
    }
}

} // namespace conversion::asn1::exporter
