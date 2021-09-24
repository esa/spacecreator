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

#include "promelaexporter.h"

#include "PromelaOptions/options.h"
#include "export/exceptions.h"
#include "tmc/PromelaModel/promelamodel.h"
#include "visitors/promelamodelvisitor.h"

#include <QDirIterator>
#include <QSaveFile>
#include <QTextStream>
#include <iostream>

using conversion::exporter::ExportException;
using conversion::exporter::IncorrectModelException;
using tmc::promelamodel::PromelaModel;

namespace conversion::tmc::exporter {
void PromelaExporter::exportModel(const Model *model, const Options &options) const
{
    if (model == nullptr) {
        throw ExportException("Model to export is null");
    }

    const auto *const promelaModel = dynamic_cast<const PromelaModel *>(model);
    if (promelaModel == nullptr) {
        throw IncorrectModelException(ModelType::Asn1, model->modelType());
    }

    QString output;
    QTextStream outputTextStream(&output, QIODevice::WriteOnly);

    PromelaModelVisitor visitor(outputTextStream);

    visitor.visit(*promelaModel);

    outputTextStream.flush();

    std::optional<QString> outputFilepath = options.value(PromelaOptions::promelaOutputFile);

    if (outputFilepath) {
        QSaveFile outputFile(outputFilepath.value());
        bool opened = outputFile.open(QIODevice::WriteOnly);
        if (!opened) {
            throw ExportException(QString("Failed to open a file %1").arg(outputFile.fileName()));
        }
        std::string data = output.toStdString();
        uint64_t written = outputFile.write(data.c_str());
        bool commited = outputFile.commit();
        if (written != output.length()) {
            throw ExportException(QString("Failed to write a file %1").arg(outputFile.fileName()));
        }
        if (!commited) {
            throw ExportException(QString("Failed to commit a transaction in %1").arg(outputFile.fileName()));
        }
    }
}
}
