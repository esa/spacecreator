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
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/export/exceptions.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::Type;
using conversion::exporter::ExportException;

namespace conversion::asn1::exporter {

void PatcherFunctionsExporter::exportModel(const Asn1Model *model, const Options &options)
{
    for (const auto &asn1File : model->data()) {
        const auto sequencesToPatch = collectSequencesToPatch(asn1File.get());

        std::vector<QString> patcherFunctionsFileNames;

        for (const auto sequence : sequencesToPatch) {
            /* auto patcherFunctionsFileName = exportPatcherFunctions(sequence, options); */

            /* if (patcherFunctionsFileName.has_value()) { */
            /*     patcherFunctionsFileNames.push_back(std::move(*patcherFunctionsFileName)); */
            /* } */
        }

        if (!patcherFunctionsFileNames.empty()) {
            exportMappingFunctionsModule(patcherFunctionsFileNames, options);
        }
    }
}

std::optional<QString> PatcherFunctionsExporter::exportPatcherFunctions(
        const Asn1Acn::File *asn1File, const Options &options)
{
    QString headerData;
    QTextStream headerStream(&headerData, QIODevice::WriteOnly);

    QString bodyData;
    QTextStream bodyStream(&bodyData, QIODevice::WriteOnly);

    generatePatcherFunctions(asn1File, headerStream, bodyStream);

    const auto pathPrefix = options.value(Asn1Options::patcherFunctionsFilepathPrefix).value_or("");
    auto outputFileName = QString("%1-postencoding").arg(asn1File->name()).toLower();

    const auto outputHeaderFilePath = QString("%1%2.h").arg(pathPrefix).arg(outputFileName);
    QSaveFile outputHeaderFile(outputHeaderFilePath);
    writeAndCommit(outputHeaderFile, headerData);

    const auto outputBodyFilePath = QString("%1%2.c").arg(pathPrefix).arg(outputFileName);
    QSaveFile outputBodyFile(outputBodyFilePath);
    writeAndCommit(outputBodyFile, bodyData);

    return outputFileName;
}

void PatcherFunctionsExporter::generatePatcherFunctions(
        const Asn1Acn::File *asn1File, QTextStream &headerStream, QTextStream &bodyStream)
{
    initializePatcherFunctionsHeader(headerStream, asn1File);
    initializePatcherFunctionsBody(bodyStream, asn1File);
}

void PatcherFunctionsExporter::initializePatcherFunctionsHeader(QTextStream &stream, const Asn1Acn::File *asn1File)
{
    // clang-format off
    stream << "#pragma once\n\n"
           << "#include \"asn1crt.h\"\n"
           << "#include \"" << asn1File->name() << ".h\"\n";
    // clang-format on
}

void PatcherFunctionsExporter::initializePatcherFunctionsBody(QTextStream &stream, const Asn1Acn::File *asn1File)
{
    // clang-format off
    stream << "#include \"asn1crt.h\"\n"
           << "#include \"asn1crt_encoding.h\"\n"
           << "#include \"asn1crt_encoding_acn.h\"\n"
           << "#include \"" << asn1File->name() << ".h\"\n";
    // clang-format on
}

void PatcherFunctionsExporter::generateEncodingFunctionHeader(QTextStream &stream)
{
    stream << "\n";
    generateEncodingFunctionDeclaration(stream);
    stream << ";\n";
}

void PatcherFunctionsExporter::generateEncodingFunctionBody(QTextStream &stream)
{
    stream << "\n";
    generateEncodingFunctionDeclaration(stream);
    stream << "\n"
           << "{\n"
           << "}\n";
}

void PatcherFunctionsExporter::generateEncodingFunctionDeclaration(QTextStream &stream)
{
    /* const auto &name = patcherFunctionInfo.name; */
    /* const auto &sequenceName = patcherFunctionInfo.sequenceName; */

    // clang-format off
    /* stream << "void " << name << "(" */
    /*                   << "const " << sequenceName << "* pSeq, " */
    /*                   << "BitStream* pStartBitStrm, " */
    /*                   << sequenceName << "_extension_function_positions* pNullPos, " */
    /*                   << "BitStream* pEndBitStrm" */
    /*                   << ")"; */
    // clang-format on
}

void PatcherFunctionsExporter::generateDecodingValidatorHeader(QTextStream &stream)
{
    stream << "\n";
    generateDecodingValidatorDeclaration(stream);
    stream << ";\n";
}

void PatcherFunctionsExporter::generateDecodingValidatorBody(QTextStream &stream)
{
    stream << "\n";
    generateDecodingValidatorDeclaration(stream);
    stream << "\n"
           << "{\n"
           << "\treturn true;"
           << "}\n";
}

void PatcherFunctionsExporter::generateDecodingValidatorDeclaration(QTextStream &stream)
{
    /* const auto &name = patcherFunctionInfo.name; */
    /* const auto &sequenceName = patcherFunctionInfo.sequenceName; */

    // clang-format off
    /* stream << "\nflag " << name << "(" */
    /*                   << "const " << sequenceName << " *pSeq, " */
    /*                   << "BitStream* pStartBitStrm, " */
    /*                   << sequenceName << "_extension_function_positions* pNullPos, " */
    /*                   << "BitStream* pEndBitStrm, " */
    /*                   << "int* pErrCode" */
    /*                   << ")"; */
    // clang-format on
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

void PatcherFunctionsExporter::generateMappingFunctionsModule(
        const std::vector<QString> &patcherFunctionsFileNames, QTextStream &stream)
{
    stream << "#pragma once\n\n";

    for (const auto &patcherFunctionFileName : patcherFunctionsFileNames) {
        stream << "#include \"" << patcherFunctionFileName << ".h\"\n";
    }
}

std::vector<Sequence *> PatcherFunctionsExporter::collectSequencesToPatch(const Asn1Acn::File *asn1File)
{
    std::vector<Sequence *> result;

    for (const auto &definition : asn1File->definitionsList()) {
        for (const auto &type : definition->types()) {
            if (type->typeEnum() != Type::ASN1Type::SEQUENCE) {
                continue;
            }

            const auto sequence = dynamic_cast<Sequence *>(type->type());

            if (sequence->postEncodingFunction().has_value() || sequence->postDecodingValidator().has_value()) {
                result.push_back(sequence);
            }
        }
    }

    return result;
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
