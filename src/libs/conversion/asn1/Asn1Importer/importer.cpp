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

#include <QFileInfo>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1reader.h>
#include <conversion/asn1/Asn1Options/options.h>
#include <conversion/common/exceptions.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/model.h>
#include <conversion/common/options.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::Asn1Reader;
using conversion::Options;
using conversion::asn1::Asn1Options;
using conversion::importer::ImportException;

namespace conversion::asn1::importer {

std::unique_ptr<conversion::Model> Asn1Importer::importModel(const Options &options) const
{
    const auto inputFilepaths = options.values(Asn1Options::inputFilepath);
    if (inputFilepaths.empty()) {
        throw ImportException("File to import wasn't specified");
    }

    Asn1Reader asn1Reader;

    QStringList errorMessages;
    QObject::connect(
            &asn1Reader, &Asn1Reader::parseError, [&errorMessages](const QString &error) { errorMessages << error; });

    QVector<QFileInfo> inputFilesList;
    for (const auto &filename : inputFilepaths) {
        inputFilesList.append(QFileInfo(filename));
    }

    auto files = asn1Reader.parseAsn1Files(inputFilesList, &errorMessages);

    if (!errorMessages.isEmpty()) {
        auto message = errorMessages.join("\n");
        throw ImportException(std::move(message));
    }

    return std::make_unique<Asn1Model>(std::move(files));
}

} // namespace conversion::iv::importer
