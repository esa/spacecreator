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
#include <QSaveFile>
#include <QString>
#include <asn1library/asn1/asn1model.h>

using Asn1Acn::Asn1Model;
using conversion::asn1::Asn1Options;
using conversion::exporter::ExportException;

namespace conversion::asn1::exporter {

void Asn1Exporter::exportModel(const Model *model, const Options &options) const
{
    if (model == nullptr) {
        throw ExportException("Model to export is null");
    }

    const auto *asn1Model = dynamic_cast<const Asn1Model *>(model);
    if (asn1Model == nullptr) {
        throw ExportException("Invalid ASN.1 model");
    }

    for (const auto &file : asn1Model->data()) {
        // export ASN.1 model
        QString asn1SerializedModelData;
        QTextStream asn1OoutputTextStream(&asn1SerializedModelData, QIODevice::ReadWrite);
        Asn1Acn::Asn1NodeReconstructingVisitor asn1NodeReconVis(asn1OoutputTextStream);

        asn1NodeReconVis.visit(file);

        const auto asn1FilenamePrefix = options.value(Asn1Options::asn1FilenamePrefix);
        const auto asn1OutputFilename =
                QString::fromStdString(asn1FilenamePrefix->toStdString()) + QString(file.name());
        QSaveFile asn1OutputFile(asn1OutputFilename);
        assert(asn1OutputFile.open(QIODevice::WriteOnly));
        assert(asn1OutputFile.write(asn1SerializedModelData.toStdString().c_str()));
        assert(asn1OutputFile.commit());

        // export ACN model
        QString acnSerializedModelData;
        QTextStream acnOutputTextStream(&acnSerializedModelData, QIODevice::ReadWrite);
        Asn1Acn::AcnNodeReconstructingVisitor acnNodeReconVis(acnOutputTextStream);

        acnNodeReconVis.visit(file);

        const auto acnFilenamePrefix = options.value(Asn1Options::acnFilenamePrefix);
        const auto acnOutputFilename =
                QString::fromStdString(acnFilenamePrefix->toStdString()) + QString(file.name()) + QString(".acn");
        QSaveFile acnOutputFile(acnOutputFilename);
        assert(acnOutputFile.open(QIODevice::WriteOnly));
        assert(acnOutputFile.write(acnSerializedModelData.toStdString().c_str()));
        assert(acnOutputFile.commit());
    }
}

} // namespace conversion::asn1::exporter
