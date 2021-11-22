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

#include <QDomDocument>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/overloaded.h>
#include <seds/SedsModel/sedsmodel.h>
#include <seds/SedsOptions/options.h>

using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::seds::SedsOptions;
using conversion::exporter::IncorrectModelException;
using conversion::exporter::MissingOutputFilenameException;
using conversion::exporter::NullModelException;
using seds::model::DataSheet;
using seds::model::PackageFile;
using seds::model::SedsModel;

namespace seds::exporter {

void SedsXmlExporter::exportModel(const Model *const model, const Options &options) const
{
    Q_UNUSED(options);

    if (model == nullptr) {
        throw NullModelException();
    }

    const auto *const sedsModel = dynamic_cast<const SedsModel *>(model);
    if (sedsModel == nullptr) {
        throw IncorrectModelException(ModelType::Seds, model->modelType());
    }

    QDomDocument sedsDocument;

    // clang-format off
    std::visit(overloaded {
        [&](const PackageFile &packageFile) { sedsDocument = exportPackageFile(packageFile); },
        [&](const DataSheet &dataSheet) { sedsDocument = exportDataSheet(dataSheet); }
    }, sedsModel->data());
    // clang-format on

    const QString &sedsDocumentContent = sedsDocument.toString();

    const auto outputFilePath = options.value(SedsOptions::outputFilepath);
    if (!outputFilePath) {
        throw MissingOutputFilenameException(ModelType::Seds);
    }

    QSaveFile outputFile(*outputFilePath);
    writeAndCommit(outputFile, sedsDocumentContent);
}

QDomDocument SedsXmlExporter::exportPackageFile(const PackageFile &packageFile) const
{
    Q_UNUSED(packageFile);

    QDomDocument doc;

    return doc;
}

QDomDocument SedsXmlExporter::exportDataSheet(const DataSheet &dataSheet) const
{
    Q_UNUSED(dataSheet);

    QDomDocument doc;

    return doc;
}

} // namespace seds::exporter
