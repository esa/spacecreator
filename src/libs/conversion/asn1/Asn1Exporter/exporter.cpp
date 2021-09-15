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
#include "visitors/acnnodereconstructingvisitor.h"
#include "visitors/asn1nodereconstructingvisitor.h"

#include <QSaveFile>
#include <QString>
#include <asn1library/asn1/asn1model.h>

using Asn1Acn::Asn1Model;
using conversion::asn1::Asn1Options;
using conversion::exporter::ExportException;
using conversion::exporter::IncorrectModelException;
using conversion::exporter::MissingOutputFilenameException;

namespace conversion::asn1::exporter {

void Asn1Exporter::exportModel(const Model *const model, const Options &options) const
{
    if (model == nullptr) {
        throw ExportException("Model to export is null");
    }

    const auto *const asn1Model = dynamic_cast<const Asn1Model *>(model);
    if (asn1Model == nullptr) {
        throw IncorrectModelException(ModelType::Asn1, model->modelType());
    }

    for (const auto &file : asn1Model->data()) {
        exportAsn1Model(file, options);
        exportAcnModel(file, options);
    }
}

void Asn1Exporter::exportAsn1Model(const Asn1Acn::File &file, const Options &options) const
{
    QString serializedModelData;
    QTextStream outputTextStream(&serializedModelData, QIODevice::WriteOnly);

    Asn1Acn::Asn1NodeReconstructingVisitor asn1NodeReconVis(outputTextStream);
    asn1NodeReconVis.visit(file);

    const auto filePath = buildFilePath(file.name(), "asn", options);
    QSaveFile outputFile(filePath);
    writeAndCommit(outputFile, serializedModelData.toStdString());
}

void Asn1Exporter::exportAcnModel(const Asn1Acn::File &file, const Options &options) const
{
    QString serializedModelData;
    QTextStream outputTextStream(&serializedModelData, QIODevice::WriteOnly);

    Asn1Acn::AcnNodeReconstructingVisitor acnNodeReconVis(outputTextStream);
    acnNodeReconVis.visit(file);

    const auto filePath = buildFilePath(file.name(), "acn", options);
    QSaveFile outputFile(filePath);
    writeAndCommit(outputFile, serializedModelData.toStdString());
}

void Asn1Exporter::writeAndCommit(QSaveFile &outputFile, const std::string &data) const
{
    bool opened = outputFile.open(QIODevice::WriteOnly);
    bool written = outputFile.write(data.c_str());
    bool commited = outputFile.commit();

    if (!opened) {
        throw ExportException(QString("Failed to open a file %1").arg(outputFile.fileName()));
    }
    if (!written) {
        throw ExportException(QString("Failed to write a file %1").arg(outputFile.fileName()));
    }
    if (!commited) {
        throw ExportException(QString("Failed to commit a transaction in %1").arg(outputFile.fileName()));
    }
}

QString Asn1Exporter::buildFilePath(const QString &fileName, const QString &extension, const Options &options) const
{
    if (fileName.isEmpty()) {
        throw MissingOutputFilenameException(ModelType::Asn1);
    }

    const auto pathPrefix = options.value(Asn1Options::acnFilepathPrefix).value_or("");
    return QString("%1%2.%3").arg(pathPrefix, fileName, extension);
}

} // namespace conversion::asn1::exporter
