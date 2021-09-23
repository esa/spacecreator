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

#include "SdlOptions/options.h"
#include "export/exceptions.h"
#include "visitors/sdlvisitor.h"

#include <QSaveFile>
#include <QString>
#include <QTextStream>

using conversion::exporter::ExportException;
using conversion::exporter::IncorrectModelException;
using conversion::exporter::MissingOutputFilenameException;
using conversion::Sdl::SdlModel;

namespace conversion::Sdl::exporter {

void SdlExporter::exportModel(const Model *const model, const Options &options) const
{
    if (model == nullptr) {
        throw ExportException("Model to export is null");
    }

    const auto *const sdlModel = dynamic_cast<const SdlModel *>(model);
    if (sdlModel == nullptr) {
        throw IncorrectModelException(ModelType::Sdl, model->modelType());
    }

    exportSdlModel(sdlModel, options);
}

void SdlExporter::exportSdlModel(const SdlModel *const model, const Options &options) const
{
    QString serializedModelData;
    QTextStream outputTextStream(&serializedModelData, QIODevice::WriteOnly);

    Sdl::SdlVisitor sdlvis(outputTextStream);
    sdlvis.visit(model);

    const auto pathPrefix = options.value(SdlOptions::sdlFilepathPrefix).value_or("");
    const auto filePath = makeFilePath(pathPrefix, model->name(), "sdl");

    QSaveFile outputFile(filePath);
    writeAndCommit(outputFile, serializedModelData.toStdString());
}

void SdlExporter::writeAndCommit(QSaveFile &outputFile, const std::string &data) const
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

QString SdlExporter::makeFilePath(const QString &pathPrefix, const QString &fileName, const QString &extension) const
{
    if (fileName.isEmpty()) {
        throw MissingOutputFilenameException(ModelType::Asn1);
    }

    return QString("%1%2.%3").arg(pathPrefix, fileName, extension);
}

} // namespace conversion::Sdl::exporter
