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

#include "importer.h"

#include <QString>
#include <conversion/common/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <ivcore/ivxmlreader.h>

using conversion::Options;
using conversion::importer::ImportException;
using conversion::iv::IvOptions;
using ivm::IVXMLReader;

namespace conversion::iv::importer {

std::unique_ptr<conversion::Model> IvXmlImporter::importModel(const Options &options) const
{
    const auto inputFilename = options.value(IvOptions::inputFile);
    if (!inputFilename) {
        throw ImportException("File to import wasn't specified");
    }

    IVXMLReader parser;

    if (!parser.readFile(*inputFilename)) {
        const auto message = parser.errorString();
        throw ImportException(message);
    }

    return nullptr;
}

} // namespace conversion::iv::importer
