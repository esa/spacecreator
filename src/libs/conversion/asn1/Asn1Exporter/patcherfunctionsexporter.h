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

namespace conversion::asn1::exporter {

class PatcherFunctionsExporter final
{
public:
    PatcherFunctionsExporter() = delete;

    static auto exportModel(const Asn1Acn::Asn1Model *model, const Options &options) -> void;

private:
    static auto getPatcherFunctions(const Asn1Acn::File *file) -> std::vector<QString>;

    static auto exportPatcherFunctions(const std::vector<QString> &patcherFunctions, const QString &asn1FileName)
            -> QString;
    static auto exportPatcherFunctionsHeader(
            const std::vector<QString> &patcherFunctionsFileNames, const Options &options) -> void;

    static auto generatePatcherFunctionsHeader(
            const std::vector<QString> &patcherFunctionsFileNames, QTextStream &stream) -> void;

    static auto writeAndCommit(QSaveFile &outputFile, const QString &data) -> void;
};

} // namespace conversion::asn1::exporter
