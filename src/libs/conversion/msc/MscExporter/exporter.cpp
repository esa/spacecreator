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

#include "exporter.h"

#include <QString>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/model.h>
#include <conversion/common/modeltype.h>
#include <conversion/common/options.h>
#include <conversion/msc/MscOptions/options.h>
#include <msccore/mscmodel.h>
#include <msccore/mscwriter.h>

using conversion::Options;
using conversion::exporter::ExportException;
using conversion::exporter::IncorrectModelException;
using conversion::exporter::MissingOutputFilenameException;
using conversion::exporter::NullModelException;
using msc::MscModel;
using msc::MscWriter;

namespace conversion::msc::exporter {

void MscExporter::exportModel(const Model *model, const Options &options) const
{
    if (model == nullptr) {
        throw NullModelException();
    }

    if (model->modelType() != ModelType::Msc) {
        throw IncorrectModelException(ModelType::Msc, model->modelType());
    }

    // Unfortunately this has to be const_casted, because Grantlee has some
    // problems with passing const pointer...
    auto *mscModel = const_cast<MscModel *>(dynamic_cast<const MscModel *>(model));

    const auto outputFilepath = options.value(MscOptions::outputFilepath);
    if (!outputFilepath) {
        throw MissingOutputFilenameException(ModelType::Msc);
    }

    MscWriter writer;
    writer.setSaveMode(MscWriter::SaveMode::GRANTLEE);
    const auto savingSuccessful = writer.saveModel(mscModel, *outputFilepath);

    if (!savingSuccessful) {
        throw ExportException("Failed to export MSC model");
    }
}

} // namespace conversion::msc::exporter
