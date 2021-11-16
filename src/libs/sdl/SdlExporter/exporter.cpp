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

#include "export/exceptions.h"
#include "modeltype.h"
#include "visitors/sdlvisitor.h"

#include <QSaveFile>
#include <QString>
#include <QTextStream>
#include <sdl/SdlOptions/options.h>

using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::exporter::ExportException;
using conversion::exporter::IncorrectModelException;
using conversion::exporter::MissingOutputFilenameException;
using sdl::SdlModel;

namespace sdl::exporter {

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
    for (const auto &process : model->processes()) {
        exportProcess(process, options);
    }
}

void SdlExporter::exportProcess(const Process &process, const Options &options) const
{
    QString serializedProcess;
    QTextStream outputTextStream(&serializedProcess, QIODevice::WriteOnly);

    sdl::SdlVisitor visitor(outputTextStream);

    visitor.visit(process);

    const auto pathPrefix = options.value(SdlOptions::sdlFilepathPrefix).value_or("");
    const auto filePath = makeFilePath(pathPrefix, process.name(), "pr");

    QSaveFile outputFile(filePath);
    writeAndCommit(outputFile, serializedProcess.toStdString());
}

void SdlExporter::writeAndCommit(QSaveFile &outputFile, const std::string &data) const
{
    if (!outputFile.open(QIODevice::WriteOnly)) {
        throw ExportException(QString("Failed to open a file %1").arg(outputFile.fileName()));
    }

    if (outputFile.write(data.c_str()) == -1) {
        throw ExportException(QString("Failed to write a file %1").arg(outputFile.fileName()));
    }

    if (!outputFile.commit()) {
        throw ExportException(QString("Failed to commit a transaction in %1").arg(outputFile.fileName()));
    }
}

QString SdlExporter::makeFilePath(const QString &pathPrefix, const QString &fileName, const QString &extension) const
{
    if (fileName.isEmpty()) {
        throw MissingOutputFilenameException(ModelType::Sdl);
    }

    return QString("%1%2.%3").arg(pathPrefix, fileName, extension);
}

} // namespace sdl::exporter
