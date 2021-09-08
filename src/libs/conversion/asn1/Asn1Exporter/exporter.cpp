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
        // throw IncorrectModelException(ModelType::Asn1Model);
        return;
    }

    for (const auto &file : asn1Model->data()) {
        QByteArray modelData;
        QBuffer modelDataBuffer(&modelData);
        modelDataBuffer.open(QIODevice::WriteOnly);

        QTextStream qtas(modelData);
        Asn1Acn::Asn1NodeReconstructingVisitor asn1nrvis(qtas);

        asn1nrvis.visit(file);

        const auto inputFilename = options.value(Asn1Options::outputFilename);
        if (inputFilename == nullptr) {
            throw ExportException("Input filename wasn't specified");
        }
        const auto outputFilename =
                QString(file.name()) + QString::fromStdString(inputFilename->toStdString()) + QString(".asn");
        QSaveFile outputFile(outputFilename);
        outputFile.open(QIODevice::WriteOnly);
        outputFile.write(modelData);
        outputFile.commit();

        // analogically as ^^ for ACN for a given File
    }
}

} // namespace conversion::asn1::exporter
