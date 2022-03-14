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
        auto patcherFunctions = getPatcherFunctions(file.get());

        if (patcherFunctions.empty()) {
            continue;
        }

        auto patcherFunctionsFileName = exportPatcherFunctions(patcherFunctions, file->name());
        patcherFunctionsFileNames.push_back(std::move(patcherFunctionsFileName));
    }

    if (!patcherFunctionsFileNames.empty()) {
        exportPatcherFunctionsHeader(patcherFunctionsFileNames, options);
    }
}

std::vector<QString> PatcherFunctionsExporter::getPatcherFunctions(const Asn1Acn::File *file)
{
    std::vector<QString> patcherFunctions;

    for (const auto &definitions : file->definitionsList()) {
        const auto &types = definitions->types();

        for (const auto &type : types) {
            if (type->typeEnum() != Type::ASN1Type::SEQUENCE) {
                continue;
            }

            const auto sequence = dynamic_cast<const Sequence *>(type->type());

            if (sequence->postEncodingFunction()) {
                auto postEncodingFunctionName = QString("%1-encoding-function").arg(sequence->identifier().toLower());
                patcherFunctions.push_back(std::move(postEncodingFunctionName));
            }

            if (sequence->postDecodingValidator()) {
                auto postDecodingValidatorName = QString("%1-decoding-validator").arg(sequence->identifier().toLower());
                patcherFunctions.push_back(std::move(postDecodingValidatorName));
            }
        }
    }

    return patcherFunctions;
}

QString PatcherFunctionsExporter::exportPatcherFunctions(
        const std::vector<QString> &patcherFunctions, const QString &asn1FileName)
{
    Q_UNUSED(patcherFunctions);
    Q_UNUSED(asn1FileName);

    auto patcherFunctionFileName = QString("%1-post-encoding").arg(asn1FileName).toLower();

    return patcherFunctionFileName;
}

void PatcherFunctionsExporter::exportPatcherFunctionsHeader(
        const std::vector<QString> &patcherFunctionsFileNames, const Options &options)
{
    QString buffer;
    QTextStream stream(&buffer, QIODevice::WriteOnly);

    generatePatcherFunctionsHeader(patcherFunctionsFileNames, stream);

    const auto pathPrefix = options.value(Asn1Options::patcherFunctionsFilepathPrefix).value_or("");
    const auto outputFileName =
            options.value(Asn1Options::outputPatcherFunctionsHeaderFileName).value_or("postEncoding");
    const auto outputFilePath = QString("%1%2.h").arg(pathPrefix).arg(outputFileName);

    QSaveFile outputFile(outputFilePath);
    writeAndCommit(outputFile, buffer);
}

void PatcherFunctionsExporter::generatePatcherFunctionsHeader(
        const std::vector<QString> &patcherFunctionsFileNames, QTextStream &stream)
{
    stream << "#pragma once\n\n";

    for (const auto &patcherFunctionFileName : patcherFunctionsFileNames) {
        stream << "#include <" << patcherFunctionFileName << ".h>\n";
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
