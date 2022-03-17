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

#pragma once

#include <QSaveFile>
#include <QTextStream>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/types/sequence.h>
#include <conversion/common/options.h>
#include <optional>

namespace conversion::asn1::exporter {

class PatcherFunctionsExporter final
{
public:
    PatcherFunctionsExporter() = delete;

    static auto exportModel(const Asn1Acn::Asn1Model *model, const Options &options) -> void;

private:
    static auto exportPatcherFunctions(const Asn1Acn::File *asn1File, const Options &options) -> std::optional<QString>;
    static auto generatePatcherFunctions(
            const Asn1Acn::File *asn1File, QTextStream &headerStream, QTextStream &bodyStream) -> void;
    static auto initializePatcherFunctionsHeader(QTextStream &stream, const Asn1Acn::File *asn1File) -> void;
    static auto initializePatcherFunctionsBody(QTextStream &stream, const Asn1Acn::File *asn1File) -> void;

    static auto generateEncodingFunctionHeader(
            const Asn1Acn::PatcherFunctionInfo &patcherFunctionInfo, QTextStream &stream) -> void;
    static auto generateEncodingFunctionBody(
            const Asn1Acn::PatcherFunctionInfo &patcherFunctionInfo, QTextStream &stream) -> void;
    static auto generateEncodingFunctionDeclaration(
            const Asn1Acn::PatcherFunctionInfo &patcherFunctionInfo, QTextStream &stream) -> void;

    static auto generateDecodingValidatorHeader(
            const Asn1Acn::PatcherFunctionInfo &patcherFunctionInfo, QTextStream &stream) -> void;
    static auto generateDecodingValidatorBody(
            const Asn1Acn::PatcherFunctionInfo &patcherFunctionInfo, QTextStream &stream) -> void;
    static auto generateDecodingValidatorDeclaration(
            const Asn1Acn::PatcherFunctionInfo &patcherFunctionInfo, QTextStream &stream) -> void;

    static auto exportMappingFunctionsModule(
            const std::vector<QString> &patcherFunctionsFileNames, const Options &options) -> void;
    static auto generateMappingFunctionsModule(
            const std::vector<QString> &patcherFunctionsFileNames, QTextStream &stream) -> void;

    static auto writeAndCommit(QSaveFile &outputFile, const QString &data) -> void;
};

} // namespace conversion::asn1::exporter
