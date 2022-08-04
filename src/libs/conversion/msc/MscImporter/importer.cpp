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

#include "importer.h"

#include <QFileInfo>
#include <conversion/common/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/model.h>
#include <conversion/common/options.h>
#include <conversion/msc/MscOptions/options.h>
#include <msccore/exceptions.h>
#include <msccore/mscmodel.h>
#include <msccore/mscreader.h>

using conversion::Options;
using conversion::importer::ImportException;
using conversion::msc::MscOptions;
using msc::MscModel;
using msc::MscReader;

namespace conversion::msc::importer {

std::unique_ptr<conversion::Model> MscImporter::importModel(const Options &options) const
{
    const auto inputFilepath = options.value(MscOptions::inputFilepath);
    if (!inputFilepath) {
        throw ImportException("MSC file to import wasn't specified");
    }

    const QFileInfo inputFileInfo(*inputFilepath);
    if (!inputFileInfo.exists()) {
        throw conversion::FileNotFoundException(*inputFilepath, "while importing MSC");
    }

    MscReader mscReader;
    MscModel *mscModel = nullptr;

    try {
        mscModel = mscReader.parseFile(*inputFilepath);
    } catch (const ::msc::Exception &ex) {
        auto errorMessage = QString("Error while parsing MSC file: '%1'").arg(ex.errorMessage());
        throw ImportException(std::move(errorMessage));
    }

    const auto &errorMessages = mscReader.getErrorMessages();
    if (!errorMessages.isEmpty()) {
        auto errorMessage = "Errors while parsing MSC file: " + errorMessages.join("\n");
        throw ImportException(std::move(errorMessage));
    }

    std::unique_ptr<MscModel> result;
    result.reset(mscModel);

    return result;
}

} // namespace conversion::msc::importer
