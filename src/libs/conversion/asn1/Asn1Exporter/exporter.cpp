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

#include "exporter.h"

#include "Asn1Options/options.h"
#include "export/exceptions.h"

#include <QBuffer>
#include <QString>
#include <asn1library/asn1/asn1model.h>

using Asn1Acn::Asn1Model;
using conversion::asn1::Asn1Options;
using conversion::exporter::ExportException;

namespace conversion::asn1::exporter {

void Asn1Exporter::exportModel(const Model *const model, const Options &options) const
{
    if (model == nullptr) {
        throw ExportException("Model to export is null");
    }

    const auto *const asn1Model = dynamic_cast<const Asn1Model *>(model);
    if (asn1Model == nullptr) {
        throw ExportException("Invalid ASN.1 model");
    }

    for (const auto &file : asn1Model->data()) {
        exportAsn1Model(file, options);
        exportAcnModel(file, options);
    }
}

void Asn1Exporter::exportAsn1Model(const Asn1Acn::File &file, const Options &options)
{
    QString serializedModelData;
    QTextStream outputTextStream(&serializedModelData, QIODevice::WriteOnly);

    Asn1Acn::Asn1NodeReconstructingVisitor asn1NodeReconVis(outputTextStream);
    asn1NodeReconVis.visit(file);

    QSaveFile outputFile(makeFilename(options.value(Asn1Options::asn1FilenamePrefix), file.name(), ".asn"));
    writeAndCommit(outputFile, serializedModelData.toStdString());
}

void Asn1Exporter::exportAcnModel(const Asn1Acn::File &file, const Options &options)
{
    QString serializedModelData;
    QTextStream outputTextStream(&serializedModelData, QIODevice::WriteOnly);

    Asn1Acn::AcnNodeReconstructingVisitor acnNodeReconVis(outputTextStream);
    acnNodeReconVis.visit(file);

    QSaveFile outputFile(makeFilename(options.value(Asn1Options::acnFilenamePrefix), file.name(), ".acn"));
    writeAndCommit(outputFile, serializedModelData.toStdString());
}

QString Asn1Exporter::makeFilename(const std::optional<QString> &prefix, const QString &name, const QString &extension)
{
    return prefix.value_or(QString("")) + name + extension;
}

void Asn1Exporter::writeAndCommit(QSaveFile &outputFile, const std::string &data)
{
    assert(outputFile.open(QIODevice::WriteOnly));
    assert(outputFile.write(data.c_str()));
    assert(outputFile.commit());
}

} // namespace conversion::asn1::exporter
