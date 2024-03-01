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

#include <QBuffer>
#include <QSaveFile>
#include <QString>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/model.h>
#include <conversion/common/modeltype.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivxmlwriter.h>

using conversion::Options;
using conversion::exporter::ExportException;
using conversion::exporter::IncorrectModelException;
using conversion::exporter::MissingOutputFilenameException;
using conversion::exporter::NullModelException;
using conversion::iv::IvOptions;
using ivm::IVModel;
using ivm::IVXMLWriter;

namespace conversion::iv::exporter {

void IvXmlExporter::exportModel(const Model *model, const Options &options) const
{
    if (model == nullptr) {
        throw NullModelException();
    }

    if (model->modelType() != ModelType::InterfaceView) {
        throw IncorrectModelException(ModelType::InterfaceView, model->modelType());
    }

    const auto *ivModel = dynamic_cast<const IVModel *>(model);

    const auto outputFilepath = options.value(IvOptions::outputFilepath);
    if (!outputFilepath) {
        throw MissingOutputFilenameException(ModelType::InterfaceView);
    }

    IVXMLWriter exporter;
    exporter.exportObjectsSilently(ivModel->objects().values(), *outputFilepath);
}

} // namespace conversion::iv::exporter
