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

#include "exporter.h"

#include "export/exceptions.h"
#include "file.h"
#include "patcherfunctionsexporter.h"
#include "visitors/acnnodereconstructingvisitor.h"
#include "visitors/asn1nodereconstructingvisitor.h"

#include <QDir>
#include <QSaveFile>
#include <QString>
#include <asn1library/asn1/asn1model.h>
#include <conversion/asn1/Asn1Options/options.h>

using Asn1Acn::Asn1Model;
using conversion::asn1::Asn1Options;
using conversion::exporter::ExportException;
using conversion::exporter::IncorrectModelException;
using conversion::exporter::MissingOutputFilenameException;
using conversion::exporter::NullModelException;

namespace conversion::asn1::exporter {

void Asn1Exporter::exportModel(const Model *const model, const Options &options) const
{
    if (model == nullptr) {
        throw NullModelException();
    }

    const auto *const asn1Model = dynamic_cast<const Asn1Model *>(model);
    if (asn1Model == nullptr) {
        throw IncorrectModelException(ModelType::Asn1, model->modelType());
    }

    for (const auto &file : asn1Model->data()) {
        exportAsn1Model(file.get(), options);
        exportAcnModel(file.get(), options);
    }

    PatcherFunctionsExporter::exportModel(asn1Model, options);
}

QStringList Asn1Exporter::getFilenamesForModel(const Asn1Acn::Asn1Model *model)
{
    if (model == nullptr) {
        return QStringList();
    }

    QStringList names;
    for (const auto &file : model->data()) {
        names.append(makeAcnFilename(file.get()));
        names.append(makeAsn1Filename(file.get()));
    }

    return names;
}

void Asn1Exporter::exportAsn1Model(const Asn1Acn::File *file, const Options &options) const
{
    QString serializedModelData;
    QTextStream outputTextStream(&serializedModelData, QIODevice::WriteOnly);

    Asn1Acn::Asn1NodeReconstructingVisitor asn1NodeReconVis(outputTextStream);
    asn1NodeReconVis.visit(*file);

    const auto pathPrefix = options.value(Asn1Options::asn1FilepathPrefix).value_or("");
    const auto filePath = QString("%1%2").arg(pathPrefix).arg(makeAsn1Filename(file));

    QSaveFile outputFile(filePath);
    writeAndCommit(outputFile, serializedModelData);
}

void Asn1Exporter::exportAcnModel(const Asn1Acn::File *file, const Options &options) const
{
    QString serializedModelData;
    QTextStream outputTextStream(&serializedModelData, QIODevice::WriteOnly);

    Asn1Acn::AcnNodeReconstructingVisitor acnNodeReconVis(outputTextStream);
    acnNodeReconVis.visit(*file);

    const auto pathPrefix = options.value(Asn1Options::acnFilepathPrefix).value_or("");
    const auto filePath = QString("%1%2").arg(pathPrefix).arg(makeAcnFilename(file));

    QSaveFile outputFile(filePath);
    writeAndCommit(outputFile, serializedModelData);
}

QString Asn1Exporter::getFilename(const Asn1Acn::File *const file)
{
    if (file == nullptr) {
        throw ExportException("File pointer passed to this function cannot be null");
    }

    QString fileName = file->name();

    if (fileName.isEmpty()) {
        throw MissingOutputFilenameException(ModelType::Asn1);
    }

    return fileName;
}

QString Asn1Exporter::makeAsn1Filename(const Asn1Acn::File *const file)
{
    return QString("%1.%2").arg(getFilename(file)).arg("asn");
}

QString Asn1Exporter::makeAcnFilename(const Asn1Acn::File *const file)
{
    return QString("%1.%2").arg(getFilename(file)).arg("acn");
}

} // namespace conversion::asn1::exporter
