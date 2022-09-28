/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "simulatortrailexporter.h"

#include <QDebug>
#include <conversion/common/export/exceptions.h>
#include <simulatortrail/SimulatorTrailOptions/options.h>

using Asn1Acn::ValuePtr;
using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::exporter::ExportException;
using conversion::exporter::IncorrectModelException;
using conversion::simulatortrail::SimulatorTrailOptions;
using simulatortrail::model::SimulatorTrailModel;

namespace simulatortrail::exporter {
void SimulatorTrailExporter::exportModel(const Model *model, const Options &options) const
{
    if (model == nullptr) {
        throw ExportException("Model to export is null");
    }
    const SimulatorTrailModel *simulatorTrailModel = dynamic_cast<const SimulatorTrailModel *>(model);
    if (simulatorTrailModel == nullptr) {
        throw IncorrectModelException(ModelType::SimulatorTrail, model->modelType());
    }

    std::optional<QString> outputFilepath = options.value(SimulatorTrailOptions::outputFilepath);

    if (outputFilepath.has_value()) {
        QString output;
        QTextStream outputStream(&output, QIODevice::WriteOnly);

        doExport(*simulatorTrailModel, outputStream);

        QSaveFile outputFile(outputFilepath.value());
        const bool opened = outputFile.open(QIODevice::WriteOnly);
        if (!opened) {
            throw ExportException(QString("Failed to open a file %1").arg(outputFile.fileName()));
        }

        const std::string data = output.toStdString();
        const int64_t written = outputFile.write(data.c_str());

        if (written != output.length()) {
            throw ExportException(QString("Failed to write a file %1").arg(outputFile.fileName()));
        }

        const bool commited = outputFile.commit();
        if (!commited) {
            throw ExportException(QString("Failed to commit a transaction in %1").arg(outputFile.fileName()));
        }
    }
}

void SimulatorTrailExporter::doExport(
        const simulatortrail::model::SimulatorTrailModel &model, QTextStream &stream) const
{
    for (const ValuePtr &value : model.getValues()) {
        stream << value->asString() << "\n";
    }
}
}
