/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
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

#include "export/modelexporter.h"

#include <asn1library/asn1/asn1model.h>

namespace Asn1Acn {
class File;
}
class QSaveFile;

namespace conversion::asn1::exporter {
class Asn1Exporter final : public ::conversion::exporter::ModelExporter
{
public:
    /**
     * @brief   Exports ASN.1 and ACN models to separate files
     *
     * @param   model       Model to export
     * @param   options     Options for export configuration
     */
    virtual auto exportModel(const Model *model, const Options &options) const -> void override;

    /**
     * @brief Get the names of files which will be generated for Files within the model
     *
     * @param model         ASN.1 model
     *
     * @return              a list of filenames
     */
    static auto getFilenamesForModel(const Asn1Acn::Asn1Model *model) -> QStringList;

private:
    auto exportAsn1Model(const Asn1Acn::File *file, const Options &options) const -> void;
    auto exportAcnModel(const Asn1Acn::File *file, const Options &options) const -> void;

    static auto getFilename(const Asn1Acn::File *file) -> QString;
    static auto makeAsn1Filename(const Asn1Acn::File *file) -> QString;
    static auto makeAcnFilename(const Asn1Acn::File *file) -> QString;
};
} // namespace conversion::asn1::exporter
