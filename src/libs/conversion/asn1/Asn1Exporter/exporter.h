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

#pragma once

#include "export/modelexporter.h"
#include "visitors/acnnodereconstructingvisitor.h"
#include "visitors/asn1nodereconstructingvisitor.h"

#include <QSaveFile>

namespace conversion::asn1::exporter {
class Asn1Exporter final : public ::conversion::exporter::ModelExporter
{
public:
    /**
     * @brief   Exports model
     *
     * @param   model       Model to export
     * @param   options     Options for export configuration
     */
    virtual auto exportModel(const Model *model, const Options &options) const -> void override;

private:
    static void exportAsn1Model(const Asn1Acn::File &file, const Options &options);
    static void exportAcnModel(const Asn1Acn::File &file, const Options &options);

    static inline QString makeFilename(
            const std::optional<QString> &prefix, const QString &name, const QString &extension);
    static void writeAndCommit(QSaveFile &outputFile, const std::string &data);
};
} // namespace conversion::asn1::exporter
